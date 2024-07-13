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

#include "R3BNeulandCalToHitPar.h"
#include <ParResultReader.h>
#include <PedeLauncher.h>

namespace R3B::Neuland::Calibration
{
    constexpr auto DEFAULT_PEDE_ERROR_THRES = 0.1F;

    class PedeHandler
    {
      public:
        PedeHandler() = default;
        void init();
        void calibrate();
        void init_steer_writer(const Cal2HitPar& /*cal_to_hit_par*/);

        void set_pede_interation_number(int number) { pede_interartion_number_ = number; }
        void set_pede_error_threshold(float thres) { pede_error_threshold_ = thres; }
        void set_error_factor(float scale) { error_scale_factor_ = scale; }
        void set_max_number_of_modules(int num_of_modules) { max_number_of_modules_ = num_of_modules; }
        void set_scale_factor(float scale_factor) { scale_factor_ = scale_factor; }
        void set_data_filename(std::string_view filename) { data_filename_ = filename; }

        void fill_module_parameters(Neuland::Cal2HitPar& cal_to_hit_par);

      private:
        int max_number_of_modules_ = 0;
        int pede_interartion_number_ = 3;
        float pede_error_threshold_ = DEFAULT_PEDE_ERROR_THRES;
        float error_scale_factor_ = 1.F;
        float scale_factor_ = 1.F;
        std::string data_filename_;
        Millepede::ResultReader par_result_;
        Millepede::Launcher pede_launcher_;
    };

} // namespace R3B::Neuland::Calibration
