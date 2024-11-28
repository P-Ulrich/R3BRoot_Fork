#pragma once

#include <FairTask.h>
#include <NeulandSimCalData.h>
#include <R3BIOConnector.h>
#include <R3BNeulandCalData2.h>
#include <unordered_map>
namespace R3B::Neuland
{
    class SimCal2Cal : public FairTask
    {
      public:
        SimCal2Cal() = default;

      private:
        InputVectorConnector<R3B::Neuland::SimCalData> sim_cal_data_{ "NeulandSimCal" };
        OutputVectorConnector<BarCalData> cal_data_{ "NeulandCalData" };

        void Exec(Option_t* /*option*/) override;
        void SetParContainers() override {}
        auto Init() -> InitStatus override;

        void convert(const std::vector<R3B::Neuland::SimCalData>& sim_cal_data, std::vector<BarCalData>& cal_data);
        std::unordered_map<int, BarCalData> bar_map_data_;
    };
} // namespace R3B::Neuland
