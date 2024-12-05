#pragma once

#include <FairTask.h>
#include <NeulandSimCalData.h>
#include <R3BIOConnector.h>
#include <R3BNeulandCalData2.h>
#include <R3BNeulandCommon.h>
#include <unordered_map>

namespace R3B::Neuland
{
    class CalibrationBasePar;

    class SimCal2Cal : public FairTask
    {
      public:
        SimCal2Cal() = default;
        void set_num_of_dp(int number_of_dp) { number_of_dp_ = number_of_dp; }

      private:
        int number_of_dp_ = MaxNumberOfPlanes / 2;
        InputVectorConnector<R3B::Neuland::SimCalData> sim_cal_data_{ "NeulandSimCal" };
        OutputVectorConnector<BarCalData> cal_data_{ "NeulandCalData" };
        CalibrationBasePar* base_par_ = nullptr;

        void Exec(Option_t* /*option*/) override;
        void SetParContainers() override;
        auto Init() -> InitStatus override;
        void FinishTask() override;

        void convert(const std::vector<R3B::Neuland::SimCalData>& sim_cal_data, std::vector<BarCalData>& cal_data);
        std::unordered_map<int, BarCalData> bar_map_data_;
    };
} // namespace R3B::Neuland
