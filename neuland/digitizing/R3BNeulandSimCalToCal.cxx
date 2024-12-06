#include "R3BNeulandBasePar.h"
#include "R3BNeulandSimCalToCal.h"

namespace R3B::Neuland
{
    auto SimCal2Cal::Init() -> InitStatus
    {
        sim_cal_data_.init();
        cal_data_.init();
        auto* rtdb = FairRuntimeDb::instance();
        base_par_ = std::make_unique<CalibrationBasePar>().release();
        base_par_->set_num_of_planes(number_of_dp_ * 2);
        if (rtdb->addContainer(base_par_); base_par_ == nullptr)
        {
            throw R3B::runtime_error("Calibration parameter becomes nullptr!");
        }
        return kSUCCESS;
    }

    void SimCal2Cal::Exec(Option_t* /*option*/)
    {
        const auto& sim_cal_data = sim_cal_data_.get();
        auto& cal_data = cal_data_.get();
        cal_data.clear();

        convert(sim_cal_data, cal_data);
    }

    void SimCal2Cal::FinishTask() { base_par_->setChanged(); }

    void SimCal2Cal::SetParContainers() {}

    void SimCal2Cal::convert(const std::vector<R3B::Neuland::SimCalData>& sim_cal_data,
                             std::vector<BarCalData>& cal_data)
    {
        bar_map_data_.clear();
        for (const auto& sim_data : sim_cal_data)
        {
            auto module_id = sim_data.bar_module;
            auto [iter, _] = bar_map_data_.try_emplace(module_id, static_cast<unsigned int>(module_id));

            auto left_signal = CalDataSignal{};
            left_signal.leading_time = ValueError<double>{ sim_data.let_l, 0 };
            left_signal.time_over_threshold = ValueError<double>{ sim_data.tot_l, 0 };

            auto right_signal = CalDataSignal{};
            right_signal.leading_time = ValueError<double>{ sim_data.let_r, 0 };
            right_signal.time_over_threshold = ValueError<double>{ sim_data.tot_r, 0 };

            auto& obj = iter->second;
            obj.left.push_back(left_signal);
            obj.right.push_back(right_signal);
        }
        for (const auto& pair : bar_map_data_)
        {
            cal_data.push_back(pair.second);
        }
    }
} // namespace R3B::Neuland
