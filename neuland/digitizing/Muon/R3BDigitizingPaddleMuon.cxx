// /******************************************************************************
//  *   Copyright (C) 2019 GSI Helmholtzzentrum für Schwerionenforschung GmbH    *
//  *   Copyright (C) 2019-2024 Members of R3B Collaboration                     *
//  *                                                                            *
//  *             This software is distributed under the terms of the            *
//  *                 GNU General Public Licence (GPL) version 3,                *
//  *                    copied verbatim in the file "LICENSE".                  *
//  *                                                                            *
//  * In applying this license GSI does not waive the privileges and immunities  *
//  * granted to it by virtue of its status as an Intergovernmental Organization *
//  * or submit itself to any jurisdiction.                                      *
//  ******************************************************************************/
// #pragma once
// /**
//  * Simulation of Muon Bar/Paddle
//  * @author Yanzhao Wang
//  * @since March 13, 2023
//  *
//  *
//  */
// #include "R3BDigitizingPaddleNeuland.h"
//
// #include "R3BDigitizingPaddle.h"
// #include "R3BNeulandCommon.h"
// #include <cmath>
//
// namespace R3B::Digitizing::Neuland
// {
//     class MuonPaddle : public Digitizing::Paddle
//     {
//       public:
//         explicit MuonPaddle(uint16_t paddleID)
//             : Digitizing::Paddle{ paddleID }
//         {
//         }
//
//       private:
//         [[nodiscard]] auto ComputeTime(const Channel::Signal& firstSignal, const Channel::Signal& secondSignal) const
//             -> double override
//         {
//             return (firstSignal.tdc + secondSignal.tdc) / 2 + gHalfLength / gCMedium;
//         }
//         [[nodiscard]] auto ComputePosition(const Channel::Signal& rightSignal, const Channel::Signal& leftSignal) const
//             -> double override
//         {
//             return (rightSignal.tdc - leftSignal.tdc) * gCMedium;
//         }
//
//         [[nodiscard]] auto ComputeEnergy(const Channel::Signal& firstSignal, const Channel::Signal& secondSignal) const
//             -> double override
//         {
//             return std::sqrt(firstSignal.qdcUnSat * secondSignal.qdcUnSat) *
//                    std::exp(ComputePosition(firstSignal, secondSignal));
//         }
//
//         auto ComputeChannelHits(const Paddle::Hit& hit) const -> Pair<Channel::Hit> override
//         {
//             auto rightChannelHit = GenerateMuonChannelHit(hit.time, hit.LightDep, hit.DistToPaddleCenter);
//             auto leftChannelHit = GenerateMuonChannelHit(hit.time, hit.LightDep, -1 * hit.DistToPaddleCenter);
//             return { leftChannelHit, rightChannelHit };
//         }
//
//       public:
//         static constexpr double gHalfLength = 135.; // [cm]
//         static constexpr double gCMedium = 30.;     // speed of light in material in [cm/ns]
//         static auto GenerateMuonChannelHit(Double_t mcTime, Double_t mcLight, Double_t dist) -> Channel::Hit
//         {
//             auto time = mcTime + (MuonPaddle::gHalfLength - dist) / MuonPaddle::gCMedium;
//             auto light = mcLight * std::exp(-NeulandPaddle::gAttenuation * (NeulandPaddle::gHalfLength - dist));
//             ;
//             return { time, light };
//         }
//     };
// } // namespace R3B::Digitizing::Neuland
//
// #include "R3BDigitizingPaddleNeuland.h"
// #include <R3BNeulandCommon.h>
// #include <cmath>
//
// using R3B::Neuland::DEFAULT_EFFECTIVE_C;
//
// namespace R3B::Digitizing::Neuland
// {
//     static const uint8_t DEFAULT_ITERATION = 8U;
//     static auto CheckMatchValidity(const std::vector<Paddle::ChannelSignalPair>& matchedPairs,
//                                    const Channel::Signal& signal) -> bool;
//     // reversing attentuation factor:
//     const double NeulandPaddle::ReverseAttenFac = std::exp(NeulandPaddle::gHalfLength * NeulandPaddle::gAttenuation);
//
//     template <uint8_t iterations = DEFAULT_ITERATION>
//     auto FastExp(const Float_t val) -> Float_t
//     {
//         auto exp = 1.F + val / (iterations >> 1U);
//         for (auto i = 0; i < iterations; ++i)
//         {
//             exp *= exp;
//         }
//         return exp;
//     }
//
//     NeulandPaddle::NeulandPaddle(uint16_t paddleID)
//         : Digitizing::Paddle(paddleID, SignalCouplingNeuland)
//     {
//     }
//
//     auto NeulandPaddle::MatchSignals(const Channel::Signal& firstSignal, const Channel::Signal& secondSignal) -> float
//     {
//         auto firstE = static_cast<Float_t>(firstSignal.qdcUnSat);
//         auto secondE = static_cast<Float_t>(secondSignal.qdcUnSat);
//         auto firstT = firstSignal.tdc;
//         auto secondT = secondSignal.tdc;
//
//         // keep the exponent always negative to prevent the exploding of exponential function
//         auto res = 0.F;
//         if (firstT > secondT)
//         {
//             res =
//                 std::abs((firstE / secondE) *
//                              FastExp<4>(static_cast<Float_t>(gAttenuation * DEFAULT_EFFECTIVE_C * (firstT - secondT))) -
//                          1);
//         }
//         else
//         {
//             res =
//                 std::abs((secondE / firstE) * FastExp<4>(static_cast<Float_t>(gAttenuation * DEFAULT_EFFECTIVE_C *
//                                                                               static_cast<Float_t>(secondT - firstT))) -
//                          1);
//         }
//         return res;
//     }
//
//     inline auto NeulandPaddle::ComputeEnergy(const Channel::Signal& firstSignal,
//                                              const Channel::Signal& secondSignal) const -> double
//     {
//         return std::sqrt(firstSignal.qdcUnSat * secondSignal.qdcUnSat) * ReverseAttenFac;
//     }
//
//     inline auto NeulandPaddle::ComputeTime(const Channel::Signal& firstSignal,
//                                            const Channel::Signal& secondSignal) const -> double
//     {
//         return (firstSignal.tdc + secondSignal.tdc) / 2 - gHalfLength / DEFAULT_EFFECTIVE_C;
//     }
//
//     inline auto NeulandPaddle::ComputePosition(const Channel::Signal& leftSignal,
//                                                const Channel::Signal& rightSignal) const -> double
//     {
//         if (leftSignal.side == rightSignal.side)
//         {
//             R3BLOG(fatal, "cannot compute position with signals from same side!");
//             return 0.F;
//         }
//         return (leftSignal.side == ChannelSide::left) ? (leftSignal.tdc - rightSignal.tdc) / 2 * DEFAULT_EFFECTIVE_C
//                                                       : (rightSignal.tdc - leftSignal.tdc) / 2 * DEFAULT_EFFECTIVE_C;
//     }
//
//     auto NeulandPaddle::ComputeChannelHits(const Hit& hit) const -> Paddle::Pair<Channel::Hit>
//     {
//         auto rightChannelHit = GenerateChannelHit(hit.time, hit.LightDep, hit.DistToPaddleCenter);
//         auto leftChannelHit = GenerateChannelHit(hit.time, hit.LightDep, -1 * hit.DistToPaddleCenter);
//         return { leftChannelHit, rightChannelHit };
//     }
//
//     auto NeulandPaddle::GenerateChannelHit(const Double_t mcTime, const Double_t mcLight, const Double_t dist)
//         -> Channel::Hit
//     {
//         auto time = mcTime + (NeulandPaddle::gHalfLength - dist) / DEFAULT_EFFECTIVE_C;
//         auto light = mcLight * std::exp(-NeulandPaddle::gAttenuation * (NeulandPaddle::gHalfLength - dist));
//         return { time, light };
//     }
//
//     auto NeulandPaddle::ComputeChannelHitsCorr(const Hit& hit) -> Paddle::Pair<Channel::Hit>
//     {
//         // ToDo: Not sure if the +/- is the rightway around check tDiff definition
//         auto hit_module_par{ GetHitModulePar() };
//
//         auto pmt_saturation_left = hit_module_par.PMTSaturation.left();
//
//         auto pmt_saturation_right = hit_module_par.PMTSaturation.right();
//
//         auto rightChannelHit = GenerateChannelHitCorr(
//             (hit_module_par.tDiff / 2) + hit.time, hit.LightDep, hit.DistToPaddleCenter, pmt_saturation_right);
//         auto leftChannelHit = GenerateChannelHitCorr(
//             -((hit_module_par.tDiff / 2) - hit.time), hit.LightDep, -1 * hit.DistToPaddleCenter, pmt_saturation_left);
//         return { leftChannelHit, rightChannelHit };
//     }
//
//     auto NeulandPaddle::GenerateChannelHitCorr(const ValueError<double> mcTime,
//                                                const Double_t mcLight,
//                                                const Double_t dist,
//                                                ValueError<double> PMTSaturation) -> Channel::Hit
//     {
//         // Energy Correction
//         ValueError<double> mcLightValueError{ mcLight, 0 };
//
//         auto mcLightCorr = mcLightValueError / (PMTSaturation * mcLightValueError + 1);
//         ValueError<double> time = mcTime + (NeulandPaddle::gHalfLength - dist) / NeulandPaddle::gCMedium;
//
//         ValueError<double> efunc{ std::exp(-NeulandPaddle::gAttenuation * (NeulandPaddle::gHalfLength - dist)), 0 };
//         ValueError<double> light = mcLightCorr * efunc;
//
//         if (!GetErrorCalculation())
//         {
//             TRandom randGen;
//             return { randGen.Gaus(time.value, time.error), randGen.Gaus(light.value, light.error) };
//         }
//         return { time.value, light.value };
//     }
//
//     auto NeulandPaddle::SignalCouplingNeuland(const Channel::Signals& firstSignals,
//                                               const Channel::Signals& secondSignals) -> std::vector<ChannelSignalPair>
//     {
//         // step1: determine the signals with smaller size:
//         decltype(auto) smallerSizeSignals = (firstSignals.size() < secondSignals.size()) ? firstSignals : secondSignals;
//         decltype(auto) largerSizeSignals = (firstSignals.size() >= secondSignals.size()) ? firstSignals : secondSignals;
//
//         // create empty pairs to be filled:
//         auto channelPairs = std::vector<ChannelSignalPair>{};
//         channelPairs.reserve(smallerSizeSignals.size());
//
//         // step2: signal matching:
//         for (const auto& it : smallerSizeSignals)
//         {
//             // find the element from largerSizeSignals with minimum matching value
//             auto it_min = std::min_element(largerSizeSignals.begin(),
//                                            largerSizeSignals.end(),
//                                            [&it = std::as_const(it)](const auto& left, const auto& right) -> bool
//                                            { return (MatchSignals(it, left) < MatchSignals(it, right)); });
//             if (it_min == largerSizeSignals.end())
//             {
//                 LOG(warn) << "DigitizingPaddleNeuland.cxx::SignalCouplingNeuland(): failed to find minimum value!";
//             }
//             else
//             {
//                 if (CheckMatchValidity(channelPairs, *it_min))
//                 {
//                     channelPairs.emplace_back(it, *it_min);
//                 }
//             }
//         }
//         // step3: output pairs
//         return channelPairs;
//     }
//
//     // check if a signal is matched to two or more signals. If so, discard the lastest match.
//     static auto CheckMatchValidity(const std::vector<Paddle::ChannelSignalPair>& matchedPairs,
//                                    const Channel::Signal& signal) -> bool
//     {
//         auto is_valid = true;
//         auto it_existed = find_if(matchedPairs.begin(),
//                                   matchedPairs.end(),
//                                   [&signal](const auto& pair) -> bool { return &(pair.right.get()) == &(signal); });
//         if (it_existed != matchedPairs.end())
//         {
//             LOG(debug) << "DigitizingPaddleNeuland.cxx::CheckMatchValidity(): one signal is matched again to another "
//                           "signal! The signal is discarded.";
//             is_valid = false;
//         }
//         return is_valid;
//     }
//
// } // namespace R3B::Digitizing::Neuland
