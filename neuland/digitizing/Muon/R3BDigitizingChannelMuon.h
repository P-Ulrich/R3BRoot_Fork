// #pragma once
// #include "R3BDigitizingChannel.h"
// #include "R3BDigitizingPaddle.h"
//
// namespace R3B::Digitizing::Neuland
// {
//     class CalChannel : public Digitizing::Channel
//     {
//       public:
//         explicit CalChannel(ChannelSide side)
//             : Digitizing::Channel{ side } {};
//         void AddHit(Hit newHit) override
//         {
//             auto signal = Signal{};
//             signal.qdc = newHit.light;
//             signal.qdcUnSat = newHit.light;
//             signal.tdc = newHit.time;
//             signal.side = GetSide();
//             m_Signals.emplace_back(signal);
//         }
//
//         void AttachToPaddle(Digitizing::Paddle* paddle) override {}
//
//       private:
//         Signals m_Signals{};
//         auto ConstructSignals() -> Signals override
//         {
//             auto signal = Signal{};
//             signal.qdcUnSat = GetEnergy();
//             signal.qdc = GetQDC();
//             signal.tdc = GetTDC();
//             signal.side = this->GetSide();
//             return { signal };
//         }
//     };
// } // namespace R3B::Digitizing::Neuland
//   //
//   //
//   //
//
//
//         auto ComputeChannelHits(const Paddle::Hit& hit) const -> Pair<Channel::Hit> override
//         {
//             auto rightChannelHit = GenerateCalChannelHit(hit.time, hit.LightDep, hit.DistToPaddleCenter);
//             auto leftChannelHit = GenerateCalChannelHit(hit.time, hit.LightDep, -1 * hit.DistToPaddleCenter);
//             return { leftChannelHit, rightChannelHit };
//         }
//       public:
//
//         static constexpr double gHalfLength = 135.; // [cm]
//         static constexpr double gCMedium = 30.;     // speed of light in material in [cm/ns]
//     };
//         static auto GenerateCalChannelHit(Double_t mcTime, Double_t mcLight, Double_t dist) -> Channel::Hit
//         {
//             auto time = mcTime + (CalPaddle::gHalfLength - dist) / CalPaddle::gCMedium;
//             auto light = mcLight * std::exp(-NeulandPaddle::gAttenuation * (NeulandPaddle::gHalfLength - dist));
//             ;
//             return { time, light };
//         }
//
//

#pragma once
#include "R3BDigitizingChannel.h"
#include "R3BDigitizingPaddle.h"
#include <vector>

namespace R3B::Digitizing::Neuland
{
    struct CalSignal
    {
        double time_over_threshold{};
        double time_leading_edge{};
        ChannelSide side{};
        double bar_id{};
    };

    class CalChannel : public Digitizing::Channel
    {
      public:
        using CalSignals = std::vector<CalSignal>;

        explicit CalChannel(ChannelSide side)
            : Digitizing::Channel{ side } {};
        void AddHit(Hit newHit) override
        {
            auto signal = Signal{};
            signal.qdc = newHit.light;
            signal.qdcUnSat = newHit.light;
            signal.tdc = newHit.time;
            signal.side = GetSide();
            m_Signals.emplace_back(signal);
        }

        void AttachToPaddle(Digitizing::Paddle* paddle) override {}

      private:
        Signals m_Signals{};
        auto ConstructSignals() -> Signals override
        {
            auto signal = Signal{};
            signal.qdcUnSat = GetEnergy();
            signal.qdc = GetQDC();
            signal.tdc = GetTDC();
            signal.side = this->GetSide();
            return { signal };
        }

        auto ConstructCalSignals() -> CalSignals override
        {
                fqt_peaks_ = ConstructFQTPeaks(pmt_peaks_);
        // signal pileup:
        FQTPeakPileUp(fqt_peaks_);

        // construct Channel signals:
        auto cal_signals = std::vector<CalSignal>{};
        signals.reserve(fqt_peaks_.size());

        for (const auto& peak : fqt_peaks_)
        {
            signals.emplace_back(CreateCalSignal(peak));
        }
        return cal_signals;
        }


            auto CreateCalSignal(const FQTPeak& peak) const -> CalSignal
    {
        auto peakQdc = peak.GetQDC();
        auto peakTime = peak.GetLETime();
        auto qdc = ToQdc(peakQdc);

        auto signal = CalSignal{};
        signal.time_over_threshold = CalculateTOT(qdc);
        signal.time_leading_edge = peakTime;
        signal.side = this->GetSide();
        LOG(debug) << "R3BDigitizingTamex: Create a Cal signal with qdc " << signal.qdc << " and tdc " << signal.tdc
                   << std::endl;
        return signal;
    }

    };
} // namespace R3B::Digitizing::Neuland
