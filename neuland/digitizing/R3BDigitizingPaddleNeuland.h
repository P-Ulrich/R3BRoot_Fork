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
#include "R3BDigitizingEngine.h"
#include "R3BNeulandCommon.h"

/**
 * Simulation of NeuLAND Bar/Paddle
 * @author Yanzhao Wang
 * @since July 15, 2022
 */

namespace R3B::Digitizing::Neuland
{
    class NeulandPaddle : public Digitizing::Paddle
    {
      public:
        explicit NeulandPaddle(uint16_t paddleID);

        explicit NeulandPaddle(uint16_t paddleID, R3B::Neuland::Cal2HitPar* cal_to_hit_par);

      private:
        [[nodiscard]] auto ComputeTime(const Channel::Signal& firstSignal, const Channel::Signal& secondSignal) const
            -> double override;
        [[nodiscard]] auto ComputeEnergy(const Channel::Signal& firstSignal, const Channel::Signal& secondSignal) const
            -> double override;
        [[nodiscard]] auto ComputePosition(const Channel::Signal& leftSignal, const Channel::Signal& rightSignal) const
            -> double override;
        auto ComputeChannelHits(const Hit& hit) const -> Pair<Channel::Hit> override;

      public:
        double gHalfLength = 135.;   // [cm]
        double gAttenuation = 0.008; // light attenuation of plastic scintillator [1/cm]
        double gLambda = 1. / 2.1;
        double ReverseAttenFac =std::exp(gHalfLength * gAttenuation) ;

        auto MatchSignals(const Channel::Signal& firstSignal, const Channel::Signal& secondSignal) -> float;
        [[nodiscard]] static auto SignalCouplingNeuland(NeulandPaddle& self,const Channel::Signals& firstSignals,
                                                        const Channel::Signals& secondSignals)
            -> std::vector<ChannelSignalPair>;
        auto GenerateChannelHit(Double_t mcTime, Double_t mcLight, Double_t dist) const -> const Channel::Hit;
      private:
         double effective_speed_ = R3B::Neuland::DEFAULT_EFFECTIVE_C;
    };
} // namespace R3B::Digitizing::Neuland
