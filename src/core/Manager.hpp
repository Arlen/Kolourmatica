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

#include "../../../eigen/Eigen/Core"

using namespace Eigen;


class Manager{

public:
  typedef enum{
    CSXYZ = 0, CSxyY, CSLab, CSLCHab, CSLuv, CSLCHuv, CSAdobeRGB,
    CSAppleRGB, CSBestRGB, CSBetaRGB, CSBruceRGB, CSCIERGB,
    CSColorMatchRGB, CSDonRGB4, CSECIRGB, CSEktaSpacePS5, CSNTSCRGB,
    CSPALSECAMRGB, CSProPhotoRGB, CSSMPTECRGB, CSsRGB, CSWideGamutRGB
  } CSType;

  static Manager& Instance();
  void SetWorkingColorSpace(CSType cs);
  CSType GetWorkingColorSpace() const{ return working_; }
  void SetSystemColorSpace(CSType cs);
  CSType GetSystemColorSpace() const{ return system_; }
  void SetReferenceWhite(const Vector3f& rRw);
  Vector3f GetReferenceWhite() const{ return white_; }
  void SetAdaptationMethod(const Matrix3f& rAd);
  Matrix3f GetAdaptationMethod() const{ return adaptation_; }

  ~Manager();

private:
  Manager();
  Manager(const Manager& ){ }
  Manager& operator=(const Manager& ){ return *pManager_; }

  static Manager* pManager_;
  static bool deleted_;

  CSType working_;
  CSType system_;
  Vector3f white_;
  Matrix3f adaptation_;
};


#endif
