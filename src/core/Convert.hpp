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

#include <cmath>

using namespace Eigen;

struct Input{

  Input& operator=(const Input& rI){
    from_ = rI.from_;
    white_ = rI.white_;
    //targetM_ = rI.targetM_;
    targetM_1_ = rI.targetM_1_;
    targetGamma_ = rI.targetGamma_;
    sourceM_ = rI.sourceM_;
    //sourceM_1_ = rI.sourceM_1_;
    sourceGamma_ = rI.sourceGamma_;
    return *this;
  }

  Eigen::Vector3f from_;
  Eigen::Vector3f white_;
  //Eigen::Matrix3f targetM_;
  Eigen::Matrix3f targetM_1_;
  float targetGamma_;
  Eigen::Matrix3f sourceM_;
  //Eigen::Matrix3f sourceM_1_;
  float sourceGamma_;
};

class Convert{

public: 
  typedef Eigen::Vector3f XYZ;
  typedef Eigen::Vector3f xyY;
  typedef Eigen::Vector3f Lab;
  typedef Eigen::Vector3f LCHab;
  typedef Eigen::Vector3f Luv;
  typedef Eigen::Vector3f LCHuv;
  typedef Eigen::Vector3f RGB;
  typedef Eigen::Vector3f sRGB;

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

#endif
