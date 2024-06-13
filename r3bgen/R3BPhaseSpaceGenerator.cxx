/******************************************************************************
 *   Copyright (C) 2019 GSI Helmholtzzentrum für Schwerionenforschung GmbH    *
 *   Copyright (C) 2019-2024 Members of R3B Collaboration                     *
 *                                                                            *
 *             This software is distributed under the terms of the            *
 *                 GNU General Public Licence (GPL) version 3,                *
 *                    copied verbatim in the file "LICENSE".                  *
 *                                                                            *
 * In applying this license GSI does not waive the privileges and immunities  *
 * granted to it by virtue of its status as an Intergovernmental Organization *
 * or submit itself to any jurisdiction.                                      *
 ******************************************************************************/

#include "R3BPhaseSpaceGenerator.h"
#include "FairPrimaryGenerator.h"
#include "FairRunSim.h"
#include "R3BException.h"
#include "TLorentzVector.h"
#include "TVector3.h"

#include <numeric>

constexpr auto DEFAULT_ENERGY = 100.;

R3BPhaseSpaceGenerator::R3BPhaseSpaceGenerator(unsigned int seed)
    : fErel_keV(R3BDistribution1D::Delta(DEFAULT_ENERGY))
{
    rnd_gen_.SetSeed(seed);
}

auto R3BPhaseSpaceGenerator::Init() -> bool
{
    if (masses.size() < 2)
    {
        throw R3B::logic_error("R3BPhaseSpaceGenerator::Init: Not enough Particles! At least two are required.");
    }

    total_mass_ = std::accumulate(masses.begin(), masses.end(), 0.);
    return true;
}

auto R3BPhaseSpaceGenerator::ReadEvent(FairPrimaryGenerator* primGen) -> bool
{
    const auto pos_cm =
        beam_properties_.GetVertexDistribution().GetRandomValues({ rnd_gen_.Rndm(), rnd_gen_.Rndm(), rnd_gen_.Rndm() });
    const auto spread_mRad =
        beam_properties_.GetSpreadDistribution().GetRandomValues({ rnd_gen_.Rndm(), rnd_gen_.Rndm() });
    const auto beamBeta = beam_properties_.GetBetaDistribution().GetRandomValues({ rnd_gen_.Rndm() })[0];
    const auto relative_energy_GeV = fErel_keV.GetRandomValues({ rnd_gen_.Rndm() })[0] * (1e-6);

    const auto total_energy_GeV = relative_energy_GeV + total_mass_;
    auto init_vec = TLorentzVector{ 0.0, 0.0, 0.0, total_energy_GeV };
    phase_space_gen_.SetDecay(init_vec, static_cast<int>(masses.size()), masses.data());
    phase_space_gen_.Generate();

    const auto beamVector = [&]()
    {
        auto beam_vector = TVector3{ 0, 0, beamBeta };
        beam_vector.RotateX(spread_mRad[0] * 1e-3);
        beam_vector.RotateZ(spread_mRad[1] * 1e-3);
        return beam_vector;
    }();

    for (auto idx = 0; idx < pdg_codes_.size(); ++idx)
    {
        auto* decay = phase_space_gen_.GetDecay(idx);
        decay->Boost(beamVector);

        const auto totalEnergy = sqrt(decay->Mag() * decay->Mag() + masses[idx] * masses[idx]);

        const auto pdg_code = pdg_codes_.at(idx);

        if (is_whitelist_enabled and particle_whitelist_.find(pdg_code) == particle_whitelist_.end())
        {
            continue;
        }
        primGen->AddTrack(
            pdg_code, decay->Px(), decay->Py(), decay->Pz(), pos_cm[0], pos_cm[1], pos_cm[2], -1, true, totalEnergy);
    }
    return true;
}

// NOLINTNEXTLINE
void R3BPhaseSpaceGenerator::addParticle(int pdgCode, double mass)
{
    pdg_codes_.push_back(pdgCode);
    masses.push_back(mass);
}
