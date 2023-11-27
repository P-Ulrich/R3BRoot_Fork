
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
#pragma once

#include "TObject.h"
#include "TVector3.h"
#include <R3BFormatters.h>
#include <Rtypes.h>
#include <iostream>
#include "R3BNeulandHit.h"


struct R3BNeulandCalData : public TObject
{
    int module_id = 0;
    double tot_left = 0.;
    double tot_right = 0.;
    double tle_left = 0.;
    double tle_right = 0.;

    R3BNeulandCalData() = default;
    R3BNeulandCalData(Int_t paddle,
                  double totL,
                  double totR,
                  double tleL,
                  double tleR
                  );

    void Print(const Option_t* /*option*/) const override;
    auto operator==(const R3BNeulandCalData& other) const -> bool
    {
        // TODO: Change this if multi-hit capability is introduced
        return this->GetPaddle() == other.GetPaddle();
    }

    // NOTE: for backward compabitlity
    [[nodiscard]] auto GetPaddle() const -> int { return module_id; }
    [[nodiscard]] auto GettotR() const -> double { return tot_right; }
    [[nodiscard]] auto GettotL() const -> double { return tot_left; }
    [[nodiscard]] auto GettleR() const -> double { return tle_right; }
    [[nodiscard]] auto GettleL() const -> double { return tle_left; }

    [[nodiscard]] auto GetBeta() const -> double;
    [[nodiscard]] auto GetEToF(double mass = NEUTRON_MASS_MEV) const -> double;

    ClassDefOverride(R3BNeulandCalData, 2);
};

auto operator<<(std::ostream&, const R3BNeulandCalData&) -> std::ostream&; // Support easy printing
                                                                       //
template <>
class fmt::formatter<R3BNeulandCalData>
{
  public:
    static constexpr auto parse(format_parse_context& ctx) { return ctx.end(); }
    template <typename FmtContent>
    constexpr auto format(const R3BNeulandCalData& hit, FmtContent& ctn) const
    {
        return format_to(
            ctn.out(),
            "{{module_id: {}, left_tot: {}, right_tot: {}, time: {} ns, left_tle: {}, right_tle: {}, energy: "
            "{} MeV, position: {} cm, pixel: {}}}",
            hit.module_id,
            hit.tot_left,
            hit.tot_right,
            hit.tle_left,
            hit.tle_right);
    }
};
