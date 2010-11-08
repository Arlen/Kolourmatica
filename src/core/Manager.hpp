/***********************************************************************
|*  Copyright (C) 2010 Arlen Avakian
|*
|*  This file is part of Kolourmatica.
|*
|*  Kolourmatica is free software: you can redistribute it and/or modify
|*  it under the terms of the GNU General Public License as published by
|*  the Free Software Foundation, either version 3 of the License, or
|*  (at your option) any later version.
|*
|*  Kolourmatica is distributed in the hope that it will be useful,
|*  but WITHOUT ANY WARRANTY; without even the implied warranty of
|*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
|*  GNU General Public License for more details.
|*
|*  You should have received a copy of the GNU General Public License
|*  along with Kolourmatica.  If not, see <http://www.gnu.org/licenses/>.
|*
|************************************************************************/


#ifndef MANAGER_HPP
#define MANAGER_HPP


class Manager{

public:
  typedef enum{
    CSXYZ, CSxyY, CSLab, CSLCHab, CSLuv, CSLCHuv, CSAdobeRGB, CSAppleRGB,
    CSBestRGB, CSBetaRGB, CSBruceRGB, CSCIERGB, CSColorMatchRGB, CSDonRGB4,
    CSECIRGB, CSEktaSpacePS5, CSNTSCRGB, CSPALSECAMRGB, CSProPhotoRGB,
    CSSMPTECRGB, CSsRGB, CSWideGamutRGB
  } CSType;

  typedef enum{
    RefWhiteA, RefWhiteB, RefWhiteC, RefWhiteD50, RefWhiteD55, RefWhiteD65,
    RefWhiteD75, RefWhiteE, RefWhiteF2, RefWhitef7, RefWhiteF11
  } RefWhiteType;

  static const Manager& Instance();
  void SetSystemColorSpace(CSType cs);
  void SetWorkingColorSpace(CSType cs);

  ~Manager();

private:
  Manager();
  Manager(const Manager& ){ }
  Manager& operator=(const Manager& ){ return *pManager_; }

  static Manager* pManager_;
  static bool deleted_;

  CSType working_;
  CSType system_;
};


#endif
