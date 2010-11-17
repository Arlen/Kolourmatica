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


#ifndef CONVERT_HPP
#define CONVERT_HPP

#include "../../../eigen/Eigen/Core"
#include "../../../eigen/Eigen/Dense"

#include <boost/array.hpp>
#include <boost/tuple/tuple.hpp>

#include <cmath>

using namespace Eigen;


typedef Vector3f XYZ;
typedef Vector3f xyY;
typedef Vector3f Lab;
typedef Vector3f LCHab;
typedef Vector3f Luv;
typedef Vector3f LCHuv;
typedef Vector3f RGB;
typedef Vector3f sRGB;

enum{
    CSXYZ = 0, CSxyY, CSLab, CSLCHab, CSLuv, CSLCHuv, CSAdobeRGB,
    CSAppleRGB, CSBestRGB, CSBetaRGB, CSBruceRGB, CSCIERGB,
    CSColorMatchRGB, CSDonRGB4, CSECIRGB, CSEktaSpacePS5, CSNTSCRGB,
    CSPALSECAMRGB, CSProPhotoRGB, CSSMPTECRGB, CSsRGB, CSWideGamutRGB
};


struct Input{

  Input& operator=(const Input&);

  Vector3f from_;
  Vector3f white_;
  //Matrix3f targetM_;
  Matrix3f targetM_1_;
  float targetGamma_;
  Matrix3f sourceM_;
  //Matrix3f sourceM_1_;
  float sourceGamma_;
};


class BaseRGB{

public:

  BaseRGB(xyY redPrimary,
	  xyY greenPrimary,
	  xyY bluePrimary, 
	  XYZ refWhite, 
	  float gamma);

  Matrix3f GetM() const;
  Matrix3f GetM_1() const;
  Matrix3f GetM_Adapted(const XYZ& rDestination,
			const Matrix3f& rMethod) const;
  Matrix3f GetM_1_Adapted(const XYZ& rDestination,
			  const Matrix3f& rMethod) const;
  float GetGamma() const;
  Matrix3f ConversionMatrixFromRGBtoXYZ(const xyY& rCor,
					const xyY& rCog,
					const xyY& rCob,
					const XYZ& rRefWhite);

private:
  const xyY redPrimary_;
  const xyY greenPrimary_;
  const xyY bluePrimary_;
  const XYZ refWhite_;
  const float gamma_;
  Matrix3f M_;   // RGB to XYZ
  Matrix3f M_1_; // XYZ to RGB
};


class Adobe : public BaseRGB{

public:
  Adobe();
};


class Apple : public BaseRGB{

public:
  Apple();
};


class Best : public BaseRGB{

public:
  Best();
};


class Beta : public BaseRGB{

public:
  Beta();
};


class Bruce : public BaseRGB{

public:
  Bruce();
};


class CIE : public BaseRGB{

public:
  CIE();
};


class ColorMatch : public BaseRGB{

public:
  ColorMatch();
};


class Don4 : public BaseRGB{

public:
  Don4();
};


class ECI : public BaseRGB{

public:
  ECI();
};


class EktaSpacePS5 : public BaseRGB{

public:
  EktaSpacePS5();
};


class NTSC : public BaseRGB{

public:
  NTSC();
};


class PAL_SECAM : public BaseRGB{

public:
  PAL_SECAM();
};


class ProPhoto : public BaseRGB{

public:
  ProPhoto();
};


class SMPTE_C : public BaseRGB{

public:
  SMPTE_C();
};


/* the spec for sRGB says the gamma is =~ 2.2,
   but other sources state 2.4 ? */
class SRGB : public BaseRGB{

public:
  SRGB();
};


class WideGamut : public BaseRGB{

public:
  WideGamut();
};


class Convert{

public: 
  static const XYZ IlluminantA_;
  static const XYZ IlluminantB_;
  static const XYZ IlluminantC_;
  static const XYZ IlluminantD50_;
  static const XYZ IlluminantD55_;
  static const XYZ IlluminantD65_;
  static const XYZ IlluminantD75_;
  static const XYZ IlluminantE_;
  static const XYZ IlluminantF2_;
  static const XYZ IlluminantF7_;
  static const XYZ IlluminantF11_;

  static const Matrix3f XYZScaling_;
  static const Matrix3f VonKries_;
  static const Matrix3f Bradford_;

  static XYZ From_xyY_To_XYZ(const Input&);
  static XYZ From_Lab_To_XYZ(const Input&);
  static XYZ From_LCHab_To_XYZ(const Input&);
  static XYZ From_Luv_To_XYZ(const Input&);
  static XYZ From_LCHuv_To_XYZ(const Input&);
  static XYZ From_RGB_To_XYZ(const Input&);
  static XYZ From_sRGB_To_XYZ(const Input&);

  static xyY From_XYZ_To_xyY(const Input&);
  static xyY From_Lab_To_xyY(const Input&);
  static xyY From_LCHab_To_xyY(const Input&);
  static xyY From_Luv_To_xyY(const Input&);
  static xyY From_LCHuv_To_xyY(const Input&);
  static xyY From_RGB_To_xyY(const Input&);
  static xyY From_sRGB_To_xyY(const Input&);

  static Lab From_XYZ_To_Lab(const Input&);
  static Lab From_xyY_To_Lab(const Input&);
  static Lab From_LCHab_To_Lab(const Input&);
  static Lab From_Luv_To_Lab(const Input&);
  static Lab From_LCHuv_To_Lab(const Input&);
  static Lab From_RGB_To_Lab(const Input&);
  static Lab From_sRGB_To_Lab(const Input&);

  static LCHab From_XYZ_To_LCHab(const Input&);
  static LCHab From_xyY_To_LCHab(const Input&);
  static LCHab From_Lab_To_LCHab(const Input&);
  static LCHab From_Luv_To_LCHab(const Input&);
  static LCHab From_LCHuv_To_LCHab(const Input&);
  static LCHab From_RGB_To_LCHab(const Input&);
  static LCHab From_sRGB_To_LCHab(const Input&);

  static Luv From_XYZ_To_Luv(const Input&);
  static Luv From_xyY_To_Luv(const Input&);
  static Luv From_Lab_To_Luv(const Input&);
  static Luv From_LCHab_To_Luv(const Input&);
  static Luv From_LCHuv_To_Luv(const Input&);
  static Luv From_RGB_To_Luv(const Input&);
  static Luv From_sRGB_To_Luv(const Input&);

  static LCHuv From_XYZ_To_LCHuv(const Input&);
  static LCHuv From_xyY_To_LCHuv(const Input&);
  static LCHuv From_Lab_To_LCHuv(const Input&);
  static LCHuv From_LCHab_To_LCHuv(const Input&);
  static LCHuv From_Luv_To_LCHuv(const Input&);
  static LCHuv From_RGB_To_LCHuv(const Input&);
  static LCHuv From_sRGB_To_LCHuv(const Input&);

  static RGB From_XYZ_To_RGB(const Input&);
  static RGB From_xyY_To_RGB(const Input&);
  static RGB From_Lab_To_RGB(const Input&);
  static RGB From_LCHab_To_RGB(const Input&);
  static RGB From_Luv_To_RGB(const Input&);
  static RGB From_LCHuv_To_RGB(const Input&);
  static RGB From_RGB_To_RGB(const Input&);
  static RGB From_sRGB_To_RGB(const Input&);

  static sRGB From_XYZ_To_sRGB(const Input&);
  static sRGB From_xyY_To_sRGB(const Input&);
  static sRGB From_Lab_To_sRGB(const Input&);
  static sRGB From_LCHab_To_sRGB(const Input&);
  static sRGB From_Luv_To_sRGB(const Input&);
  static sRGB From_LCHuv_To_sRGB(const Input&);
  static sRGB From_RGB_To_sRGB(const Input&);

  static Matrix3f ChromaticAdaptationMatrix(const XYZ&, const XYZ&,
					    const Matrix3f&);

private:
  static void ComputeUoVo(float &uo, float &vo, const XYZ& rRefWhite);

  static const float CIE_fEpsilon;
  static const float CIE_fKappa;
  static const float CIE_fKE;
  static const float PIE;
  static const float RAD;
  static const float ANG;
};


struct Conversions{

  typedef Vector3f (*Function)(const Input&);
  typedef boost::tuple<Function, BaseRGB*, BaseRGB*> FromTo;
  typedef boost::array<FromTo, 22> Row;
  typedef boost::array<Row, 22> Table;

  Conversions();
  const Table& GetTable() const;

  Table t_;

  Adobe adobe_;
  Apple apple_;
  Best best_;
  Beta beta_;
  Bruce bruce_;
  CIE cie_;
  ColorMatch cm_;
  Don4 don4_;
  ECI eci_;
  EktaSpacePS5 ekps5_;
  NTSC ntsc_;
  PAL_SECAM pals_;
  ProPhoto pro_;
  SMPTE_C smpt_;
  SRGB srgb_;
  WideGamut wide_;
};

#endif
