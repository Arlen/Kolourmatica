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


#ifndef UTILITY_HPP
#define UTILITY_HPP

#include "../../../eigen/Eigen/Core"
#include "../../../eigen/Eigen/Dense"


template <class Real>
struct Helper{

  typedef Eigen::Matrix<Real, 3, 1> Vector3;

  /* refWhite is in XYZ space. */
  static void ComputeUoVo(Real& uo, Real& vo, const Vector3& refWhite){

    Real d = (refWhite(0) + Real(15.0) * refWhite(1) + Real(3.0) * refWhite(2));
    uo = (Real(4.0) * refWhite(0)) / d;
    vo = (Real(9.0) * refWhite(1)) / d;
  }
};


template <class Real>
struct Constants{

  static const Real CIE_epsilon_ = Real(216) / Real(24389);
  static const Real CIE_kappa_ = Real(24389) / Real(27);
  static const Real CIE_ke_ = CIE_kappa_ * CIE_epsilon_;
  static const Real pi_ = Real(M_PI);
  static const Real radian_ = pi_ / Real(180);
  static const Real angle_ = Real(180) / pi_;
};

enum{
  CSXYZ = 0,
  CSxyY,
  CSLab,
  CSLCHab,
  CSLuv,
  CSLCHuv,
  CSAdobeRGB,
  CSAppleRGB,
  CSBestRGB,
  CSBetaRGB,
  CSBruceRGB,
  CSCIERGB,
  CSColorMatchRGB,
  CSDonRGB4,
  CSECIRGB,
  CSEktaSpacePS5,
  CSNTSCRGB,
  CSPALSECAMRGB,
  CSProPhotoRGB,
  CSSMPTECRGB,
  CSsRGB,
  CSWideGamutRGB
};
  
#endif
