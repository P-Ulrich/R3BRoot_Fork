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

#include "R3BNeulandOnlineCanvas.h"
#include <R3BIOConnector.h>
#include <R3BNeulandCalData2.h>

class TH1D;
class TH2D;

namespace R3B::Neuland
{
    class TJumpCanvas : public OnlineCanvas
    {
      public:
        explicit TJumpCanvas(std::string_view name)
            : OnlineCanvas(name)
        {
        }

      private:
        InputVectorConnector<BarCalData> cal_data_{ "NeulandCalData" };
        unsigned int last_event_num_ = 0;
        CanvasElement<TH1D> hTstart_;
        CanvasElement<TH2D> hTestJump_;
        CanvasElement<TH2D> hTestJumpvsEvnt_;
        CanvasElement<TH2D> hTestJumpvsEvntzoom_;

        void DataInit() override;
        void CanvasInit(DataMonitor& histograms) override;
        void CanvasFill(DataMonitor& histograms) override;
        void CanvasFinish() override;
    };
} // namespace R3B::Neuland