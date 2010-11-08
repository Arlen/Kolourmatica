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


#ifndef SPECIFICATION_HPP
#define SPECIFICATION_HPP

#include "Convert.hpp"

#include "../../../eigen/Eigen/Core"
#include "../../../eigen/Eigen/Dense"


class BaseRGB{

public:

  typedef Eigen::Vector3f XYZ;
  typedef Eigen::Vector3f xyY;
  typedef Eigen::Vector3f Lab;
  typedef Eigen::Vector3f LCHab;
  typedef Eigen::Vector3f Luv;
  typedef Eigen::Vector3f LCHuv;
  typedef Eigen::Vector3f RGB;
  typedef Eigen::Vector3f sRGB;

  BaseRGB(xyY redPrimary,
	  xyY greenPrimary,
	  xyY bluePrimary, 
	  XYZ refWhite, 
	  float gamma, 
	  Matrix3f M, 
	  Matrix3f M_1) :
    redPrimary_(redPrimary), 
    greenPrimary_(greenPrimary), 
    bluePrimary_(bluePrimary), 
    refWhite_(refWhite), 
    gamma_(gamma), 
    M_(M), 
    M_1_(M_1){ }

  Matrix3f GetM() const{ return M_; }
  Matrix3f GetM_1() const{ return M_1_; }
  Matrix3f GetM_Adapted(const XYZ& rDestination,
			const Matrix3f& rMethod){

    return
      Convert::ChromaticAdaptationMatrix(refWhite_, rDestination, rMethod)
      * M_;
  }

  Matrix3f GetM_1_Adapted(const XYZ& rDestination,
			  const Matrix3f& rMethod){
    return
      (Convert::ChromaticAdaptationMatrix(refWhite_, rDestination, rMethod)
       * M_).inverse();
  }

  float GetGamma() const{ return gamma_; }


  Matrix3f ConversionMatrixFromRGBtoXYZ(const xyY& rCor,
					const xyY& rCog,
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

public:
  const xyY redPrimary_;
  const xyY greenPrimary_;
  const xyY bluePrimary_;
  const XYZ refWhite_;
  const float gamma_;
  const Matrix3f M_;   // RGB to XYZ
  const Matrix3f M_1_; // XYZ to RGB
};


class Adobe : public BaseRGB{

public:
  Adobe() :
    BaseRGB(xyY(0.64, 0.33, 0.297361),
	    xyY(0.21, 0.71, 0.627355), 
	    xyY(0.15, 0.06, 0.075285), 
	    Convert::IlluminantD65_, 
	    2.2f, 
	    ConversionMatrixFromRGBtoXYZ(xyY(0.64, 0.33, 0.297361),
					 xyY(0.21, 0.71, 0.627355),
					 xyY(0.15, 0.06, 0.075285),
					 Convert::IlluminantD65_),
	    ConversionMatrixFromRGBtoXYZ(xyY(0.64, 0.33, 0.297361),
					 xyY(0.21, 0.71, 0.627355),
					 xyY(0.15, 0.06, 0.075285),
					 Convert::IlluminantD65_).
	    inverse()){ }

};


class Apple : public BaseRGB{

public:
  Apple() :
    BaseRGB(xyY(0.625, 0.34, 0.244634),
	    xyY(0.28, 0.595, 0.672034),
	    xyY(0.155, 0.07, 0.083332),
	    Convert::IlluminantD65_,
	    1.8f,
	    ConversionMatrixFromRGBtoXYZ(xyY(0.625, 0.34, 0.244634),
					 xyY(0.28, 0.595, 0.672034),
					 xyY(0.155, 0.07, 0.083332),
					 Convert::IlluminantD65_),
	    ConversionMatrixFromRGBtoXYZ(xyY(0.625, 0.34, 0.244634),
					 xyY(0.28, 0.595, 0.672034),
					 xyY(0.155, 0.07, 0.083332),
					 Convert::IlluminantD65_).
	    inverse()){ }
};


class Best : public BaseRGB{

public:
  Best() :
    BaseRGB(xyY(0.7347, 0.2653, 0.228457),
	    xyY(0.2150, 0.7750, 0.737352),
	    xyY(0.1300, 0.0350, 0.034191),
	    Convert::IlluminantD50_,
	    2.2f,
	    ConversionMatrixFromRGBtoXYZ(xyY(0.7347, 0.2653, 0.228457),
					 xyY(0.2150, 0.7750, 0.737352),
					 xyY(0.1300, 0.0350, 0.034191),
					 Convert::IlluminantD50_),
	    ConversionMatrixFromRGBtoXYZ(xyY(0.7347, 0.2653, 0.228457),
					 xyY(0.2150, 0.7750, 0.737352),
					 xyY(0.1300, 0.0350, 0.034191),
					 Convert::IlluminantD50_).
	    inverse()){ }
};


class Beta : public BaseRGB{

public:
  Beta() :
    BaseRGB(xyY(0.6888, 0.3112, 0.303273),
	    xyY(0.1986, 0.7551, 0.663786),
	    xyY(0.1265, 0.0352, 0.032941),
	    Convert::IlluminantD50_,
	    2.2f,
	    ConversionMatrixFromRGBtoXYZ(xyY(0.6888, 0.3112, 0.303273),
					 xyY(0.1986, 0.7551, 0.663786),
					 xyY(0.1265, 0.0352, 0.032941),
					 Convert::IlluminantD50_),
	    ConversionMatrixFromRGBtoXYZ(xyY(0.6888, 0.3112, 0.303273),
					 xyY(0.1986, 0.7551, 0.663786),
					 xyY(0.1265, 0.0352, 0.032941),
					 Convert::IlluminantD50_).
	    inverse()){ }
};


class Bruce : public BaseRGB{

public:
  Bruce() :
    BaseRGB(xyY(0.6400, 0.3300, 0.240995),
	    xyY(0.2800, 0.6500, 0.683554),
	    xyY(0.1500, 0.0600, 0.075452),
	    Convert::IlluminantD65_,
	    2.2f,
	    ConversionMatrixFromRGBtoXYZ(xyY(0.6400, 0.3300, 0.240995),
					 xyY(0.2800, 0.6500, 0.683554),
					 xyY(0.1500, 0.0600, 0.075452),
					 Convert::IlluminantD65_),
	    ConversionMatrixFromRGBtoXYZ(xyY(0.6400, 0.3300, 0.240995),
					 xyY(0.2800, 0.6500, 0.683554),
					 xyY(0.1500, 0.0600, 0.075452),
					 Convert::IlluminantD65_).
	    inverse()){ }
};


class CIE : public BaseRGB{

public:
  CIE() :
    BaseRGB(xyY(0.7350, 0.2650, 0.176204),
	    xyY(0.2740, 0.7170, 0.812985),
	    xyY(0.1670, 0.0090, 0.010811),
	    Convert::IlluminantE_,
	    2.2f,
	    ConversionMatrixFromRGBtoXYZ(xyY(0.7350, 0.2650, 0.176204),
					 xyY(0.2740, 0.7170, 0.812985),
					 xyY(0.1670, 0.0090, 0.010811),
					 Convert::IlluminantE_),
	    ConversionMatrixFromRGBtoXYZ(xyY(0.7350, 0.2650, 0.176204),
					 xyY(0.2740, 0.7170, 0.812985),
					 xyY(0.1670, 0.0090, 0.010811),
					 Convert::IlluminantE_).
	    inverse()){ }
};


class ColorMatch : public BaseRGB{

public:
  ColorMatch() :
    BaseRGB(xyY(0.6300, 0.3400, 0.274884),
	    xyY(0.2950, 0.6050, 0.658132),
	    xyY(0.1500, 0.0750, 0.066985),
	    Convert::IlluminantD50_,
	    1.8f,
	    ConversionMatrixFromRGBtoXYZ(xyY(0.6300, 0.3400, 0.274884),
					 xyY(0.2950, 0.6050, 0.658132),
					 xyY(0.1500, 0.0750, 0.066985),
					 Convert::IlluminantD50_),
	    ConversionMatrixFromRGBtoXYZ(xyY(0.6300, 0.3400, 0.274884),
					 xyY(0.2950, 0.6050, 0.658132),
					 xyY(0.1500, 0.0750, 0.066985),
					 Convert::IlluminantD50_).
	    inverse()){ }
};


class Don4 : public BaseRGB{

public:
  Don4() :
    BaseRGB(xyY(0.6960, 0.3000, 0.278350),
	    xyY(0.2150, 0.7650, 0.687970),
	    xyY(0.1300, 0.0350, 0.033680),
	    Convert::IlluminantD50_,
	    2.2f,
	    ConversionMatrixFromRGBtoXYZ(xyY(0.6960, 0.3000, 0.278350),
					 xyY(0.2150, 0.7650, 0.687970),
					 xyY(0.1300, 0.0350, 0.033680),
					 Convert::IlluminantD50_),
	    ConversionMatrixFromRGBtoXYZ(xyY(0.6960, 0.3000, 0.278350),
					 xyY(0.2150, 0.7650, 0.687970),
					 xyY(0.1300, 0.0350, 0.033680),
					 Convert::IlluminantD50_).
	    inverse()){ }
};


class ECI : public BaseRGB{

public:
  ECI() :
    BaseRGB(xyY(0.6700, 0.3300, 0.320250),
	    xyY(0.2100, 0.7100, 0.602071),
	    xyY(0.1400, 0.0800, 0.077679),
	    Convert::IlluminantD50_,
	    1.8f,
	    ConversionMatrixFromRGBtoXYZ(xyY(0.6700, 0.3300, 0.320250),
					 xyY(0.2100, 0.7100, 0.602071),
					 xyY(0.1400, 0.0800, 0.077679),
					 Convert::IlluminantD50_),
	    ConversionMatrixFromRGBtoXYZ(xyY(0.6700, 0.3300, 0.320250),
					 xyY(0.2100, 0.7100, 0.602071),
					 xyY(0.1400, 0.0800, 0.077679),
					 Convert::IlluminantD50_).
	    inverse()){ }
};


class EktaSpacePS5 : public BaseRGB{

public:
  EktaSpacePS5() :
    BaseRGB(xyY(0.6950, 0.3050, 0.260629),
	    xyY(0.2600, 0.7000, 0.734946),
	    xyY(0.1100, 0.0050, 0.004425),
	    Convert::IlluminantD50_,
	    2.2f,
	    ConversionMatrixFromRGBtoXYZ(xyY(0.6950, 0.3050, 0.260629),
					 xyY(0.2600, 0.7000, 0.734946),
					 xyY(0.1100, 0.0050, 0.004425),
					 Convert::IlluminantD50_),
	    ConversionMatrixFromRGBtoXYZ(xyY(0.6950, 0.3050, 0.260629),
					 xyY(0.2600, 0.7000, 0.734946),
					 xyY(0.1100, 0.0050, 0.004425),
					 Convert::IlluminantD50_).
	    inverse()){ }
};


class NTSC : public BaseRGB{

public:
  NTSC() :
    BaseRGB(xyY(0.6700, 0.3300, 0.298839),
	    xyY(0.2100, 0.7100, 0.586811),
	    xyY(0.1400, 0.0800, 0.114350),
	    Convert::IlluminantC_,
	    2.2f,
	    ConversionMatrixFromRGBtoXYZ(xyY(0.6700, 0.3300, 0.298839),
					 xyY(0.2100, 0.7100, 0.586811),
					 xyY(0.1400, 0.0800, 0.114350),
					 Convert::IlluminantC_),
	    ConversionMatrixFromRGBtoXYZ(xyY(0.6700, 0.3300, 0.298839),
					 xyY(0.2100, 0.7100, 0.586811),
					 xyY(0.1400, 0.0800, 0.114350),
					 Convert::IlluminantC_).
	    inverse()){ }
};


class PAL_SECAM : public BaseRGB{

public:
  PAL_SECAM() :
    BaseRGB(xyY(0.6400, 0.3300, 0.222021),
	    xyY(0.2900, 0.6000, 0.706645),
	    xyY(0.1500, 0.0600, 0.071334),
	    Convert::IlluminantD65_,
	    2.2f,
	    ConversionMatrixFromRGBtoXYZ(xyY(0.6400, 0.3300, 0.222021),
					 xyY(0.2900, 0.6000, 0.706645),
					 xyY(0.1500, 0.0600, 0.071334),
					 Convert::IlluminantD65_),
	    ConversionMatrixFromRGBtoXYZ(xyY(0.6400, 0.3300, 0.222021),
					 xyY(0.2900, 0.6000, 0.706645),
					 xyY(0.1500, 0.0600, 0.071334),
					 Convert::IlluminantD65_).
	    inverse()){ }
};


class ProPhoto : public BaseRGB{

public:
  ProPhoto() :
    BaseRGB(xyY(0.7347, 0.2653, 0.288040),
	    xyY(0.1596, 0.8404, 0.711874),
	    xyY(0.0366, 0.0001, 0.000086),
	    Convert::IlluminantD50_,
	    1.8f,
	    ConversionMatrixFromRGBtoXYZ(xyY(0.7347, 0.2653, 0.288040),
					 xyY(0.1596, 0.8404, 0.711874),
					 xyY(0.0366, 0.0001, 0.000086),
					 Convert::IlluminantD50_),
	    ConversionMatrixFromRGBtoXYZ(xyY(0.7347, 0.2653, 0.288040),
					 xyY(0.1596, 0.8404, 0.711874),
					 xyY(0.0366, 0.0001, 0.000086),
					 Convert::IlluminantD50_).
	    inverse()){ }
};


class SMPTE_C : public BaseRGB{

public:
  SMPTE_C() :
    BaseRGB(xyY(0.6300, 0.3400, 0.212395),
	    xyY(0.3100, 0.5950, 0.701049),
	    xyY(0.1550, 0.0700, 0.086556),
	    Convert::IlluminantD65_,
	    2.2f,
	    ConversionMatrixFromRGBtoXYZ(xyY(0.6300, 0.3400, 0.212395),
					 xyY(0.3100, 0.5950, 0.701049),
					 xyY(0.1550, 0.0700, 0.086556),
					 Convert::IlluminantD65_),
	    ConversionMatrixFromRGBtoXYZ(xyY(0.6300, 0.3400, 0.212395),
					 xyY(0.3100, 0.5950, 0.701049),
					 xyY(0.1550, 0.0700, 0.086556),
					 Convert::IlluminantD65_).
	    inverse()){ }
};

/* the spec for sRGB says the gamma is =~ 2.2,
   but other sources state 2.4 ? */
class SRGB : public BaseRGB{

public:
  SRGB() :
    BaseRGB(xyY(0.6400, 0.3300, 0.212656),
	    xyY(0.3000, 0.6000, 0.715158),
	    xyY(0.1500, 0.0600, 0.072186),
	    Convert::IlluminantD65_,
	    2.4f,
	    ConversionMatrixFromRGBtoXYZ(xyY(0.6400, 0.3300, 0.212656),
					 xyY(0.3000, 0.6000, 0.715158),
					 xyY(0.1500, 0.0600, 0.072186),
					 Convert::IlluminantD65_),
	    ConversionMatrixFromRGBtoXYZ(xyY(0.6400, 0.3300, 0.212656),
					 xyY(0.3000, 0.6000, 0.715158),
					 xyY(0.1500, 0.0600, 0.072186),
					 Convert::IlluminantD65_).
	    inverse()){ }
};


class WideGamut : public BaseRGB{

public:
  WideGamut() :
    BaseRGB(xyY(0.7350, 0.2650, 0.258187),
	    xyY(0.1150, 0.8260, 0.724938),
	    xyY(0.1570, 0.0180, 0.016875),
	    Convert::IlluminantD50_,
	    2.2f,
	    ConversionMatrixFromRGBtoXYZ(xyY(0.7350, 0.2650, 0.258187),
					 xyY(0.1150, 0.8260, 0.724938),
					 xyY(0.1570, 0.0180, 0.016875),
					 Convert::IlluminantD50_),
	    ConversionMatrixFromRGBtoXYZ(xyY(0.7350, 0.2650, 0.258187),
					 xyY(0.1150, 0.8260, 0.724938),
					 xyY(0.1570, 0.0180, 0.016875),
					 Convert::IlluminantD50_).
	    inverse()){ }
};


#endif
