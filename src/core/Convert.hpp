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


  static XYZ From_xyY_To_XYZ(const xyY&);
  static XYZ From_Lab_To_XYZ(const Lab&, const XYZ&);
  static XYZ From_LCHab_To_XYZ(const LCHab&, const XYZ&);
  static XYZ From_Luv_To_XYZ(const Luv&, const XYZ&);
  static XYZ From_LCHuv_To_XYZ(const LCHuv&, const XYZ&);
  static XYZ From_RGB_To_XYZ(const RGB&, const XYZ&, const Matrix3f&,
			     const float);
  static XYZ From_sRGB_To_XYZ(const sRGB&, const XYZ&, const Matrix3f&,
			      const float);

  static xyY From_XYZ_To_xyY(const XYZ&);
  static xyY From_Lab_To_xyY(const Lab&, const XYZ&);
  static xyY From_LCHab_To_xyY(const LCHab&, const XYZ&);
  static xyY From_Luv_To_xyY(const Luv&, const XYZ&);
  static xyY From_LCHuv_To_xyY(const LCHuv&, const XYZ&);
  static xyY From_RGB_To_xyY(const RGB&, const XYZ&, const Matrix3f&,
			     const float);
  static xyY From_sRGB_To_xyY(const sRGB&, const XYZ&, const Matrix3f&,
			      const float);

  static Lab From_XYZ_To_Lab(const XYZ&, const XYZ&);
  static Lab From_xyY_To_Lab(const xyY&, const XYZ&);
  static Lab From_LCHab_To_Lab(const LCHab&, const XYZ&);
  static Lab From_Luv_To_Lab(const Luv&, const XYZ&);
  static Lab From_LCHuv_To_Lab(const LCHuv&, const XYZ&);
  static Lab From_RGB_To_Lab(const RGB&, const XYZ&, const Matrix3f&,
			     const float);
  static Lab From_sRGB_To_Lab(const sRGB&, const XYZ&, const Matrix3f&,
			      const float);

  static LCHab From_XYZ_To_LCHab(const XYZ&, const XYZ&);
  static LCHab From_xyY_To_LCHab(const xyY&, const XYZ&);
  static LCHab From_Lab_To_LCHab(const Lab&, const XYZ&);
  static LCHab From_Luv_To_LCHab(const Luv&, const XYZ&);
  static LCHab From_LCHuv_To_LCHab(const LCHuv&, const XYZ&);
  static LCHab From_RGB_To_LCHab(const RGB&, const XYZ&, const Matrix3f&,
				 const float);
  static LCHab From_sRGB_To_LCHab(const sRGB&, const XYZ&, const Matrix3f&,
				  const float);

  static Luv From_XYZ_To_Luv(const XYZ&, const XYZ&);
  static Luv From_xyY_To_Luv(const xyY&, const XYZ&);
  static Luv From_Lab_To_Luv(const Lab&, const XYZ&);
  static Luv From_LCHab_To_Luv(const LCHab&, const XYZ&);
  static Luv From_LCHuv_To_Luv(const LCHuv&, const XYZ&);
  static Luv From_RGB_To_Luv(const RGB&, const XYZ&, const Matrix3f&,
			     const float);
  static Luv From_sRGB_To_Luv(const sRGB&, const XYZ&, const Matrix3f&,
			      const float);

  static LCHuv From_XYZ_To_LCHuv(const XYZ&, const XYZ&);
  static LCHuv From_xyY_To_LCHuv(const xyY&, const XYZ&);
  static LCHuv From_Lab_To_LCHuv(const Lab&, const XYZ&);
  static LCHuv From_LCHab_To_LCHuv(const LCHab&, const XYZ&);
  static LCHuv From_Luv_To_LCHuv(const Luv&, const XYZ&);
  static LCHuv From_RGB_To_LCHuv(const RGB&, const XYZ&, const Matrix3f&,
				 const float);
  static LCHuv From_sRGB_To_LCHuv(const sRGB&, const XYZ&, const Matrix3f&,
				  const float);

  static RGB From_XYZ_To_RGB(const XYZ&, const XYZ&, const Matrix3f&,
			     const float);
  static RGB From_xyY_To_RGB(const xyY&, const XYZ&, const Matrix3f&,
			     const float);
  static RGB From_Lab_To_RGB(const Lab&, const XYZ&, const Matrix3f&,
			     const float);
  static RGB From_LCHab_To_RGB(const LCHab&, const XYZ&, const Matrix3f&,
			       const float);
  static RGB From_Luv_To_RGB(const Luv&, const XYZ&, const Matrix3f&,
			     const float);
  static RGB From_LCHuv_To_RGB(const LCHuv&, const XYZ&, const Matrix3f&,
			       const float);
  static RGB From_RGB_To_RGB(const RGB&, const XYZ&, const Matrix3f&,
			     const float, const Matrix3f&, const float);
  static RGB From_sRGB_To_RGB(const sRGB&, const XYZ&, const Matrix3f&,
			      const float, const Matrix3f&, const float);

  static sRGB From_XYZ_To_sRGB(const XYZ&, const XYZ&, const Matrix3f&,
			       const float);
  static sRGB From_xyY_To_sRGB(const xyY&, const XYZ&, const Matrix3f&,
			       const float);
  static sRGB From_Lab_To_sRGB(const Lab&, const XYZ&, const Matrix3f&,
			       const float);
  static sRGB From_LCHab_To_sRGB(const LCHab&, const XYZ&, const Matrix3f&,
				 const float);
  static sRGB From_Luv_To_sRGB(const Luv&, const XYZ&, const Matrix3f&,
			       const float);
  static sRGB From_LCHuv_To_sRGB(const LCHuv&, const XYZ&, const Matrix3f&,
				 const float);
  static sRGB From_RGB_To_sRGB(const RGB&, const XYZ&, const Matrix3f&,
			       const float, const Matrix3f&, const float);

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
