// clang-format off

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

// clang-format off
//
#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class R3B::FTChannel2TimeRelation+;
#pragma link C++ class R3B::Map2CalPar+;
#pragma link C++ class R3B::TCalVFTXModulePar+;
#pragma link C++ class unordered_map<unsigned int, R3B::TCalVFTXModulePar>+;
#pragma link C++ class R3B::Neuland::CalDataSignal+;
#pragma link C++ class vector<R3B::Neuland::CalDataSignal>+;
#pragma link C++ class R3B::Neuland::BarCalData+;
#pragma link C++ class unordered_map<int, R3B::Neuland::BarCalData>+;
#pragma link C++ class R3B::Neuland::PlaneCalData+;
#pragma link C++ class unordered_map<int, R3B::Neuland::PlaneCalData>+;
#endif