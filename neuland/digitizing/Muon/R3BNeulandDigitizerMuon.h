#pragma once

#include "FairTask.h"
#include "Filterable.h"
#include "R3BDigitizingEngine.h"
#include "R3BDigitizingPaddleNeuland.h"
#include "R3BDigitizingTacQuila.h"
#include "R3BDigitizingTamex.h"
#include "R3BNeulandGeoPar.h"
#include "R3BNeulandHit.h"
#include "R3BNeulandHitPar.h"
#include "R3BNeulandPoint.h"
#include "TCAConnector.h"
#include <TClonesArray.h>
#include <TH1.h>

class TGeoNode;
class TH1F;
class TH2F;

class R3BNeulandDigitizerCalTask : public FairTask

{
    public:

    using NeulandPaddle = Digitizing::Neuland::NeulandPaddle;
    using TamexChannel = Digitizing::Neuland::Tamex::Channel;
    template <typename Type>
    using UseChannel = Digitizing::UseChannel<Type>;
    template <typename Type>
    using UsePaddle = Digitizing::UsePaddle<Type>;


explicit R3BNeulandDigitizer(TString input = "NeulandPoints", TString output = "NeulandCalData");
    explicit R3BNeulandDigitizer(std::unique_ptr<Digitizing::DigitizingEngineInterface> engine,
                                 TString input = "NeulandPoints",
                                 TString output = "NeulandCalData");

    ~R3BNeulandDigitizer() override = default;

    // No copy and no move is allowed (Rule of three/five)
    R3BNeulandDigitizer(const R3BNeulandDigitizer&) = delete;            // copy constructor
    R3BNeulandDigitizer(R3BNeulandDigitizer&&) = delete;                 // move constructor
    R3BNeulandDigitizer& operator=(const R3BNeulandDigitizer&) = delete; // copy assignment
    R3BNeulandDigitizer& operator=(R3BNeulandDigitizer&&) = delete;      // move assignment
};
