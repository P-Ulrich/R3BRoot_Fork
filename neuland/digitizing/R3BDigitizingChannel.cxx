/******************************************************************************
 *   Copyright (C) 2019 GSI Helmholtzzentrum für Schwerionenforschung GmbH    *
 *   Copyright (C) 2019 Members of R3B Collaboration                          *
 *                                                                            *
 *             This software is distributed under the terms of the            *
 *                 GNU General Public Licence (GPL) version 3,                *
 *                    copied verbatim in the file "LICENSE".                  *
 *                                                                            *
 * In applying this license GSI does not waive the privileges and immunities  *
 * granted to it by virtue of its status as an Intergovernmental Organization *
 * or submit itself to any jurisdiction.                                      *
 ******************************************************************************/

#include "R3BDigitizingChannel.h"
#include <algorithm>

namespace R3B::Digitizing
{
    Channel::Channel(ChannelSide side)
        : fSide(side)
    {
    }

    auto Channel::GetDefaultRandomGen() -> TRandom3&
    {
        static auto random3 = TRandom3{};
        return random3;
    }

    // ToDo: Add a flag in which constructcalsignals will be called instead of constructSignals
    auto Channel::GetSignals() -> const Signals&
    {
        if (!fSignals.valid())
        {
            if (JustCalData_==true)
            {
                fCalSignals.set(ConstructCalSignals());
            }
            else
            {
                fSignals.set(ConstructSignals());
            }
        }
        return fSignals.getRef();
    }

    // auto Channel::GetCalSignals() -> const CalSignals&
    // {
    //     if (!fCalSignals.valid())
    //     {
    //         fCalSignals.set(ConstructCalSignals());
    //     }
    //     return fCalSignals.getRef();
    // }

    auto Channel::HasFired() -> bool { return (!GetSignals().empty()); }
/*     auto Channel::HasCalFired() -> bool { return (!GetCalSignals().empty()); } */

    auto Channel::GetTrigTime() -> double
    {
        if (!fTrigTime.valid())
        {
            const auto& signals = GetSignals();
            auto it =
                std::min_element(signals.begin(),
                                 signals.end(),
                                 [](const auto& left_v, const auto& right_v) { return left_v.tdc < right_v.tdc; });
            fTrigTime.set((it != signals.end() ? it->tdc : MAXTIME));
        }
        return fTrigTime.get();
    }
} // namespace R3B::Digitizing
