/******************************************************************************
 *   Copyright (C) 2019 GSI Helmholtzzentrum für Schwerionenforschung GmbH    *
 *   Copyright (C) 2019-2023 Members of R3B Collaboration                     *
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

#include <Fit/Fitter.h>
#include <TF1.h>
#include <range/v3/view.hpp>

namespace R3B::Neuland::Calibration
{
    class BarPositionFitter
    {
      public:
        struct BarPosition
        {
            bool is_horizontal = true;
            double displacement = 0.;
            double pos_z = 0.;
            double displacement_error = 0.;
        };

        BarPositionFitter();
        inline void clear()
        {
            horizontal_bars_.clear();
            vertical_bars_.clear();
            y_pos_fitting_function_.SetParameter(0, 0.);
            y_pos_fitting_function_.SetParameter(1, 0.);
            x_pos_fitting_function_.SetParameter(0, 0.);
            x_pos_fitting_function_.SetParameter(1, 0.);
        }

        void add_one_bar_signal(const BarPosition& bar_position)
        {
            auto& bar_positions = bar_position.is_horizontal ? horizontal_bars_ : vertical_bars_;
            bar_positions.displacements.push_back(bar_position.displacement);
            bar_positions.displacement_errors.push_back(bar_position.displacement_error);
            bar_positions.pos_z.push_back(bar_position.pos_z);
        }

        auto fit_positions() -> bool;
        [[nodiscard]] inline auto get_prediction(bool is_horizontal, double pos_z) const -> double
        {
            return is_horizontal ? x_pos_fitting_function_(pos_z) : y_pos_fitting_function_(pos_z);
        }

        template <typename FillAction>
        void fill_to_mille_writer(FillAction&& do_action) const
        {
            // TODO: reformat duplications here
            namespace rng = ranges;
            auto bar_position = BarPosition{};
            for (const auto& [displacement, error, pos_z] : rng::views::zip(
                     horizontal_bars_.displacements, horizontal_bars_.displacement_errors, horizontal_bars_.pos_z))
            {
                bar_position.is_horizontal = true;
                bar_position.pos_z = pos_z;
                bar_position.displacement = displacement;
                bar_position.displacement_error = error;
                do_action(bar_position);
            }
            for (const auto& [displacement, error, pos_z] : rng::views::zip(
                     vertical_bars_.displacements, vertical_bars_.displacement_errors, vertical_bars_.pos_z))
            {
                bar_position.is_horizontal = false;
                bar_position.pos_z = pos_z;
                bar_position.displacement = displacement;
                bar_position.displacement_error = error;
                do_action(bar_position);
            }
        }

      private:
        static constexpr auto fitting_function_range = 200.; // cm
        struct BarPositions
        {
            std::vector<double> displacements;
            std::vector<double> displacement_errors;
            std::vector<double> pos_z;
            void clear()
            {
                displacements.clear();
                displacement_errors.clear();
                pos_z.clear();
            }
            [[nodiscard]] auto size() const -> unsigned int { return displacements.size(); }
        };

        ROOT::Fit::Fitter fitter_;
        TF1 y_pos_fitting_function_{ "hfun", "pol1", -fitting_function_range, fitting_function_range };
        TF1 x_pos_fitting_function_{ "vfun", "pol1", -fitting_function_range, fitting_function_range };
        BarPositions horizontal_bars_;
        BarPositions vertical_bars_;

        auto fit_with(const BarPositions& positions, TF1& func) -> bool;
    };
} // namespace R3B::Neuland::Calibration
