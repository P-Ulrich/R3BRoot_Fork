#include "R3BNeulandSimCalToCal.h"

namespace R3B::Neuland
{
    auto SimCal2Cal::Init() -> InitStatus
    {
        sim_cal_data_.init();
        cal_data_.init();
        return kSUCCESS;
    }

    void SimCal2Cal::Exec(Option_t* /*option*/)
    {
        const auto& sim_cal_data = sim_cal_data_.get();
        auto& cal_data = cal_data_.get();
        cal_data.clear();

        convert(sim_cal_data, cal_data);
    }

    void SimCal2Cal::convert(const std::vector<R3B::Neuland::SimCalData>& sim_cal_data,
                             std::vector<BarCalData>& cal_data)
    {
        // TODO: Implement conversion here.
    }
} // namespace R3B::Neuland
