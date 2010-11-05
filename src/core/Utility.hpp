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

using namespace Eigen;


static const float CIE_fEpsilon = 216.0f / 24389.0f;
static const float CIE_fKappa = 24389.0f / 27.0f;
static const float CIE_fKE = CIE_fKappa * CIE_fEpsilon;
static const float PIE = 3.1415926535897932;
static const float RAD = PIE / 180.0f;
static const float ANG = 180.0f / PIE;

namespace From{
  typedef struct{} ColorSpaceXYZ;
  typedef struct{} ColorSpacexyY;
  typedef struct{} ColorSpaceLab;
  typedef struct{} ColorSpaceLCHab;
  typedef struct{} ColorSpaceLuv;
  typedef struct{} ColorSpaceLCHuv;
  typedef struct{} ColorSpaceRGB;
  typedef struct{} ColorSpacesRGB;

  ColorSpaceXYZ XYZ;
  ColorSpacexyY xyY;
  ColorSpaceLab Lab;
  ColorSpaceLCHab LCHab;
  ColorSpaceLuv Luv;
  ColorSpaceLCHuv LCHuv;
  ColorSpaceRGB RGB;
  ColorSpacesRGB sRGB;
};

namespace To{

  typedef struct{} ColorSpaceXYZ;
  typedef struct{} ColorSpacexyY;
  typedef struct{} ColorSpaceLab;
  typedef struct{} ColorSpaceLCHab;
  typedef struct{} ColorSpaceLuv;
  typedef struct{} ColorSpaceLCHuv;
  typedef struct{} ColorSpaceRGB;
  typedef struct{} ColorSpacesRGB;
};

typedef Vector3f XYZ;
typedef Vector3f xyY;
typedef Vector3f Lab;
typedef Vector3f LCHab;
typedef Vector3f Luv;
typedef Vector3f LCHuv;
typedef Vector3f RGB;
typedef Vector3f sRGB;


/* reference illuminant tristimulus values. */
XYZ illuminantA(   1.09850, 1.0, 0.35585);
XYZ illuminantB(   0.99072, 1.0, 0.85223);
XYZ illuminantC(   0.98074, 1.0, 1.18232);
XYZ illuminantD50( 0.96422, 1.0, 0.82521);
XYZ illuminantD55( 0.95682, 1.0, 0.92149);
XYZ illuminantD65( 0.95047, 1.0, 1.08883);
XYZ illuminantD75( 0.94972, 1.0, 1.22638);
XYZ illuminantE(   1.00000, 1.0, 1.00000);
XYZ illuminantF2(  0.99186, 1.0, 0.67393);
XYZ illuminantF7(  0.95041, 1.0, 1.08747);
XYZ illuminantF11( 1.00962, 1.0, 0.64350);

/* the uo and vo with different reference whites. */
inline void ComputeUoVo(float &uo, float &vo, const XYZ& rRefWhite){

  float d = (rRefWhite(0, 0) + 15.0f *
	     rRefWhite(1, 0) + 3.0f *
	     rRefWhite(2, 0));
  uo = (4.0f * rRefWhite(0, 0)) / d;
  vo = (9.0f * rRefWhite(1, 0)) / d;
}

typedef enum{ RefWhiteA, RefWhiteB, RefWhiteC, RefWhiteD50, RefWhiteD55,
	      RefWhiteD65, RefWhiteD75, RefWhiteE, RefWhiteF2, RefWhiteF7,
	      RefWhiteF11 } RefWhite;

/* chromatic adaptation transform matrices. */
Matrix3f XYZScaling;
Matrix3f VonKries;
Matrix3f Bradford;


struct InitMatrices{
  InitMatrices(){
    XYZScaling <<
      1.0, 0.0, 0.0,
      0.0, 1.0, 0.0,
      0.0, 0.0, 1.0;

    VonKries <<
      0.4002400, 0.7076000, -0.0808100,
      -0.2263000, 1.1653200,  0.0457000,
      0.0000000, 0.0000000,  0.9182200;

    Bradford <<
      0.8951000,  0.2664000, -0.1614000,
      -0.7502000,  1.7135000,  0.0367000,
      0.0389000, -0.0685000,  1.0296000;
  }
} initMatrices;



Matrix3f ChromaticAdaptationMetrix(const XYZ& rSource,
				   const XYZ& rDestination,
				   const Matrix3f& rMethod){

  XYZ S = rMethod * rSource;
  XYZ D = rMethod * rDestination;
  Matrix3f tmp = Matrix3f::Zero();
  tmp(0, 0) = D(0) / S(0);
  tmp(1, 1) = D(1) / S(1);
  tmp(2, 2) = D(2) / S(2);
  return rMethod.inverse() * tmp * rMethod;
}

Matrix3f ConversionMatrixFromRGBtoXYZ(const xyY& rCor, const xyY& rCog,
				      const xyY& rCob,
				      const XYZ& rRefWhite){

  Matrix3f xyz;
  Matrix3f M;
  Vector3f S;
  xyz(0, 0) = rCor(0, 0) / rCor(1, 0);
  xyz(1, 0) = 1.0f;
  xyz(2, 0) = (1.0f - rCor(0, 0) - rCor(1, 0)) / rCor(1, 0);
  xyz(0, 1) = rCog(0, 0) / rCog(1, 0);
  xyz(1, 1) = 1.0f;
  xyz(2, 1) = (1.0f - rCog(0, 0) - rCog(1, 0)) / rCog(1, 0);
  xyz(0, 2) = rCob(0, 0) / rCob(1, 0);
  xyz(1, 2) = 1.0f;
  xyz(2, 2) = (1.0f - rCob(0, 0) - rCob(1, 0)) / rCob(1, 0);
  S = xyz.inverse() * rRefWhite;
  M(0, 0) = S(0, 0) * xyz(0, 0);
  M(1, 0) = S(0, 0) * xyz(1, 0);
  M(2, 0) = S(0, 0) * xyz(2, 0);
  M(0, 1) = S(1, 0) * xyz(0, 1);
  M(1, 1) = S(1, 0) * xyz(1, 1);
  M(2, 1) = S(1, 0) * xyz(2, 1);
  M(0, 2) = S(2, 0) * xyz(0, 2);
  M(1, 2) = S(2, 0) * xyz(1, 2);
  M(2, 2) = S(2, 0) * xyz(2, 2);
  return M;
}


#endif
