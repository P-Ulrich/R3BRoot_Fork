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
#include <R3BNeulandHit.h>

namespace R3B::Neuland
{
    class HitXYCanvas : public OnlineCanvas
    {
      public:
        explicit HitXYCanvas(std::string_view name)
            : OnlineCanvas(name)
        {
        }

      private:
        InputVectorConnector<R3BNeulandHit> hit_data_{ "NeulandHits" };
        std::vector<CanvasElement<TH2D>> h_xy_per_plane_;

        void DataInit() override;
        void CanvasInit(DataMonitor& histograms) override;
        void CanvasFill(DataMonitor& histograms) override;
        void CanvasFinish() override;
    };
} // namespace R3B::Neuland