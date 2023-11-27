#include "R3BNeulandDigitizer.h"
#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "TGeoManager.h"
#include "TGeoNode.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TMath.h"
#include "TString.h"
#include <R3BShared.h>
#include <TFile.h>
#include <iostream>
#include <stdexcept>
#include <utility>

R3BNeulandDigitizerCalDat::R3BNeulandDigitizerCalDat(TString input, TString output)
    : R3BNeulandDigitizer(Digitizing::CreateEngine(UsePaddle<NeulandPaddle>(), UseChannel<MuonChannel>()),
                          std::move(input),
                          std::move(output))
{
}

R3BNeulandDigitizerCalDat::R3BNeulandDigitizerCalDat(std::unique_ptr<Digitizing::DigitizingEngineInterface> engine,
                                         TString input,
                                         TString output)
    : FairTask("R3BNeulandDigitizer")
    , fPoints(std::move(input))
    , fHits(std::move(output))
    , fDigitizingEngine(std::move(engine))
{
}

void R3BNeulandDigitizer::SetParContainers()
{
    FairRunAna* run = FairRunAna::Instance();
    if (run == nullptr)
    {
        LOG(fatal) << "R3BNeulandDigitizer::SetParContainers: No analysis run";
    }

    FairRuntimeDb* rtdb = run->GetRuntimeDb();
    if (rtdb == nullptr)
    {
        LOG(fatal) << "R3BNeulandDigitizer::SetParContainers: No runtime database";
    }

    fNeulandGeoPar = dynamic_cast<R3BNeulandGeoPar*>(rtdb->getContainer("R3BNeulandGeoPar"));
    if (fNeulandGeoPar == nullptr)
    {
        LOG(fatal) << "R3BNeulandDigitizer::SetParContainers: No R3BNeulandGeoPar";
    }

    fDigitizingEngine->Init();
}

InitStatus R3BNeulandDigitizer::Init()
{
    fPoints.Init();
    fHits.Init();

    // Initialize control histograms
    auto const PaddleMulSize = 3000;
    hMultOne = R3B::root_owned<TH1I>(
        "MultiplicityOne", "Paddle multiplicity: only one PMT per paddle", PaddleMulSize, 0, PaddleMulSize);
    hMultTwo = R3B::root_owned<TH1I>(
        "MultiplicityTwo", "Paddle multiplicity: both PMTs of a paddle", PaddleMulSize, 0, PaddleMulSize);
    auto const timeBinSize = 200;
    hRLTimeToTrig = R3B::root_owned<TH1F>("hRLTimeToTrig", "R/Ltime-triggerTime", timeBinSize, -100., 100.);

    return kSUCCESS;
}
void R3BNeulandDigitizer::Exec(Option_t* /*option*/)
{
    fHits.Reset();
    const auto GeVToMeVFac = 1000.;

    std::map<UInt_t, Double_t> paddleEnergyDeposit;
    // Look at each Land Point, if it deposited energy in the scintillator, store it with reference to the bar
    for (const auto& point : fPoints.Retrieve())
    {
        if (point->GetEnergyLoss() > 0.)
        {
            const Int_t paddleID = point->GetPaddle();

            // Convert position of point to paddle-coordinates, including any rotation or translation
            const TVector3 position = point->GetPosition();
            const TVector3 converted_position = fNeulandGeoPar->ConvertToLocalCoordinates(position, paddleID);
            LOG(debug2) << "NeulandDigitizer: Point in paddle " << paddleID
                        << " with global position XYZ: " << position.X() << " " << position.Y() << " " << position.Z();
            LOG(debug2) << "NeulandDigitizer: Converted to local position XYZ: " << converted_position.X() << " "
                        << converted_position.Y() << " " << converted_position.Z();

            // Within the paddle frame, the relevant distance of the light from the pmt is always given by the
            // X-Coordinate
            const Double_t dist = converted_position.X();
            fDigitizingEngine->DepositLight(paddleID, point->GetTime(), point->GetLightYield() * GeVToMeVFac, dist);
            paddleEnergyDeposit[paddleID] += point->GetEnergyLoss() * GeVToMeVFac;
        } // eloss
    }     // points

    const Double_t triggerTime = fDigitizingEngine->GetTriggerTime();
    const auto paddles = fDigitizingEngine->ExtractPaddles();

    // Fill control histograms
    hMultOne->Fill(static_cast<int>(std::count_if(
        paddles.begin(), paddles.end(), [](const auto& keyValue) { return keyValue.second->HasHalfFired(); })));

    hMultTwo->Fill(static_cast<int>(std::count_if(
        paddles.begin(), paddles.end(), [](const auto& keyValue) { return keyValue.second->HasFired(); })));

    hRLTimeToTrig->Fill(triggerTime);

    // Create Hits
    for (const auto& [paddleID, paddle] : paddles)
    {
        if (!paddle->HasFired())
        {
            continue;
        }

        auto signals = paddle->GetSignals(true); //get channel

        for (const auto signal : signals)
        {

            R3BCalData hit(paddleID,
                              signal.leftChannel.tot,
                              signal.rightChannel.tot,
                              signal.time,
                              signal.leftChannel.tle,
                              signal.rightChannel.tle);

            if (fHitFilters.IsValid(hit))
            {
                fHits.Insert(std::move(hit));
                LOG(debug) << "Adding neuland hit with id = " << paddleID << ", time = " << signal.time
                           << ", energy = " << signal.energy;
            }
        } // loop over all hits for each paddle
    }     // loop over paddles

    LOG(debug) << "R3BNeulandDigitizer: produced " << fHits.Size() << " hits";
}

