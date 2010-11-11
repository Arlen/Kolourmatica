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


#include "Convert.hpp"


const float Convert::CIE_fEpsilon = 216.0f / 24389.0f;
const float Convert::CIE_fKappa = 24389.0f / 27.0f;
const float Convert::CIE_fKE = CIE_fKappa * CIE_fEpsilon;
const float Convert::PIE = 3.1415926535897932;
const float Convert::RAD = PIE / 180.0f;
const float Convert::ANG = 180.0f / PIE;

const Convert::XYZ Convert::IlluminantA_   =
  Convert::XYZ(1.09850, 1.0, 0.35585);
const Convert::XYZ Convert::IlluminantB_   =
  Convert::XYZ(0.99072, 1.0, 0.85223);
const Convert::XYZ Convert::IlluminantC_   =
  Convert::XYZ(0.98074, 1.0, 1.18232);
const Convert::XYZ Convert::IlluminantD50_ =
  Convert::XYZ(0.96422, 1.0, 0.82521);
const Convert::XYZ Convert::IlluminantD55_ =
  Convert::XYZ(0.95682, 1.0, 0.92149);
const Convert::XYZ Convert::IlluminantD65_ =
  Convert::XYZ(0.95047, 1.0, 1.08883);
const Convert::XYZ Convert::IlluminantD75_ =
  Convert::XYZ(0.94972, 1.0, 1.22638);
const Convert::XYZ Convert::IlluminantE_   =
  Convert::XYZ(1.00000, 1.0, 1.00000);
const Convert::XYZ Convert::IlluminantF2_  =
  Convert::XYZ(0.99186, 1.0, 0.67393);
const Convert::XYZ Convert::IlluminantF7_  =
  Convert::XYZ(0.95041, 1.0, 1.08747);
const Convert::XYZ Convert::IlluminantF11_ =
  Convert::XYZ(1.00962, 1.0, 0.64350);

static const float xyzscaling[] = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0,
				   0.0, 0.0, 1.0};
static const float vonkries[] = {0.40024, -0.22630, 0.0, 0.70760, 1.16532,
				 0.0, -0.080810, 0.045700, 0.918220};
static const float bradford[] = {0.89510, -0.75020, 0.03890, 0.2664,
				 1.7135, -0.0685, -0.1614, 0.0367, 1.0296};

const Matrix3f Convert::XYZScaling_ = Matrix3f(xyzscaling);
const Matrix3f Convert::VonKries_   = Matrix3f(vonkries);
const Matrix3f Convert::Bradford_   = Matrix3f(bradford);


/* Convert to __XYZ__ */
Convert::XYZ Convert::From_xyY_To_XYZ(const xyY& rFromxyY){

  XYZ xyz;
  xyz(0, 0) = rFromxyY(0, 0) / rFromxyY(1, 0) * rFromxyY(2, 0);
  xyz(1, 0) = rFromxyY(2, 0);
  xyz(2, 0) = ((1.0f - rFromxyY(0, 0) - rFromxyY(1, 0)) /
	       rFromxyY(1, 0)) * rFromxyY(2, 0);
  return xyz;
}

Convert::XYZ Convert::From_Lab_To_XYZ(const Lab& rFromLab,
				      const XYZ& rRefWhite){

  XYZ xyz;
  float fx, fy, fz, fxCube, fzCube, xr, yr, zr;
  fy = (rFromLab(0, 0) + 16.0f) / 116.0f;
  fx = (rFromLab(1, 0) / 500.0f) + fy;
  fz = fy - (rFromLab(2, 0) / 200.0f);
  fxCube = fx * fx * fx;
  fzCube = fz * fz * fz;
  if(fxCube > CIE_fEpsilon){ xr = fxCube; }
  else{
    xr = ((116.0f * fx) - 16.0f) / CIE_fKappa;
  }
  if(rFromLab(0, 0) > CIE_fKE){
    yr = pow( (rFromLab(0, 0) + 16.0f) / 116.0f, 3.0f );
  }else{

    yr = rFromLab(0, 0) / CIE_fKappa;
  }
  if(fzCube > CIE_fEpsilon){ zr = fzCube; }
  else{
    zr = ((116.0f * fz) - 16.0f) / CIE_fKappa;
  }
  xyz(0, 0) = xr * rRefWhite(0, 0);
  xyz(1, 0) = yr * rRefWhite(1, 0);
  xyz(2, 0) = zr * rRefWhite(2, 0);
  return xyz;
}

Convert::XYZ Convert::From_LCHab_To_XYZ(const LCHab& rFromLCHab,
					const XYZ& rRefWhite){

  return From_Lab_To_XYZ(From_LCHab_To_Lab(rFromLCHab, rRefWhite),
			 rRefWhite);
}

Convert::XYZ Convert::From_Luv_To_XYZ(const Luv& rFromLuv,
				      const XYZ& rRefWhite){

  float a, b, c, d, uo, vo;
  XYZ xyz;
  ComputeUoVo(uo, vo, rRefWhite);
  c = -1.0f / 3.0f;
  a = (((52.0f * rFromLuv(0, 0)) /
	(rFromLuv(1, 0) + 13.0f * rFromLuv(0, 0) * uo)) - 1.0f) / 3.0f;
  if( rFromLuv(0, 0) > CIE_fKE ){
    xyz(1, 0) = pow( (rFromLuv(0, 0) + 16.0f) / 116.0f, 3.0f );
  }else{
    xyz(1, 0) = rFromLuv(0, 0) / CIE_fKappa;
  }      
  b = -5.0 * xyz(1, 0);
  d = (((39.0f * rFromLuv(0, 0)) /
	(rFromLuv(2, 0) + 13.0f * rFromLuv(0, 0) * vo)) - 5.0f) *
    xyz(1, 0);
  xyz(0, 0) = (d - b) / (a - c);
  xyz(2, 0) = xyz(0, 0) * a + b;
  return xyz;
}

Convert::XYZ Convert::From_LCHuv_To_XYZ(const LCHuv& rFromLCHuv,
					const XYZ& rRefWhite){

  return From_Luv_To_XYZ(From_LCHuv_To_Luv(rFromLCHuv, rRefWhite),
			 rRefWhite);
}

Convert::XYZ Convert::From_RGB_To_XYZ(const RGB& rFromRGB,
				      const XYZ& rRefWhite,
				      const Matrix3f& M,
				      const float gamma){

  RGB rgb;
  XYZ xyz;
  rgb(0, 0) = pow(rFromRGB(0, 0), gamma);
  rgb(1, 0) = pow(rFromRGB(1, 0), gamma);
  rgb(2, 0) = pow(rFromRGB(2, 0), gamma);
  xyz = M * rgb;
  return xyz;
}

Convert::XYZ Convert::From_sRGB_To_XYZ(const sRGB& rFromsRGB,
				       const XYZ& rRefWhite,
				       const Matrix3f& M,
				       const float gamma){

  sRGB rgb;
  XYZ xyz;
  if(rFromsRGB(0, 0) > 0.04045f){
    rgb(0, 0) = pow( ((rFromsRGB(0, 0) + 0.055f) / 1.055f), gamma );
  }else{
    rgb(0, 0) = rFromsRGB(0, 0) / 12.92f;
  }
  if(rFromsRGB(1, 0) > 0.04045f){
    rgb(1, 0) = pow( ((rFromsRGB(1, 0) + 0.055f) / 1.055f), gamma );
  }else{
    rgb(1, 0) = rFromsRGB(1, 0) / 12.92f;
  }
  if(rFromsRGB(2, 0) > 0.04045f){
    rgb(2, 0) = pow( ((rFromsRGB(2, 0) + 0.055f) / 1.055f), gamma );
  }else{
    rgb(2, 0) = rFromsRGB(2, 0) / 12.92f;
  }
  xyz = M * rgb;
  return xyz;
}

/* Convert to __xyY__ */
Convert::xyY Convert::From_XYZ_To_xyY(const XYZ& rFromXYZ){

  xyY xyy;
  float s = rFromXYZ(0, 0) + rFromXYZ(1, 0) + rFromXYZ(2, 0);
  xyy(0, 0) = rFromXYZ(0, 0) / s;
  xyy(1, 0) = rFromXYZ(1, 0) / s;
  xyy(2, 0) = rFromXYZ(1, 0);
  return xyy;
}

Convert::xyY Convert::From_Lab_To_xyY(const Lab& rFromLab,
				      const XYZ& rRefWhite){

  return From_XYZ_To_xyY(From_Lab_To_XYZ(rFromLab, rRefWhite));
}

Convert::xyY Convert::From_LCHab_To_xyY(const LCHab& rFromLCHab,
					const XYZ& rRefWhite){

  return From_XYZ_To_xyY(From_LCHab_To_XYZ(rFromLCHab, rRefWhite));
}

Convert::xyY Convert::From_Luv_To_xyY(const Luv& rFromLuv,
				      const XYZ& rRefWhite){

  return From_XYZ_To_xyY(From_Luv_To_XYZ(rFromLuv, rRefWhite));
}

Convert::xyY Convert::From_LCHuv_To_xyY(const LCHuv& rFromLCHuv,
					const XYZ& rRefWhite){

  return From_XYZ_To_xyY(From_LCHuv_To_XYZ(rFromLCHuv, rRefWhite));
}

Convert::xyY Convert::From_RGB_To_xyY(const RGB& rFromRGB,
				      const XYZ& rRefWhite,
				      const Matrix3f& M,
				      const float gamma){

  return From_XYZ_To_xyY(From_RGB_To_XYZ(rFromRGB, rRefWhite, M, gamma));
}

Convert::xyY Convert::From_sRGB_To_xyY(const sRGB& rFromsRGB,
				       const XYZ& rRefWhite,
				       const Matrix3f& M,
				       const float gamma){

  return From_XYZ_To_xyY(From_sRGB_To_XYZ(rFromsRGB, rRefWhite, M, gamma));
}

/* Convert to __Lab__ */
Convert::Lab Convert::From_XYZ_To_Lab(const XYZ& rFromXYZ,
				      const XYZ& rRefWhite){

  float xr, yr, zr, fx, fy, fz;
  xr = rFromXYZ(0, 0) / rRefWhite(0, 0);
  yr = rFromXYZ(1, 0) / rRefWhite(1, 0);
  zr = rFromXYZ(2, 0) / rRefWhite(2, 0);
  if(xr > CIE_fEpsilon){
    fx = pow(xr, 1.0f/3.0f);
  }else{
    fx = ((CIE_fKappa * xr) + 16.0f) / 116.0f;
  }
  if(yr > CIE_fEpsilon){
    fy = pow(yr, 1.0f/3.0f);
  }else{
    fy = ((CIE_fKappa * yr) + 16.0f) / 116.0f;
  }
  if(zr > CIE_fEpsilon){
    fz = pow(zr, 1.0f/3.0f);
  }else{
    fz = ((CIE_fKappa * zr) + 16.0f) / 116.0f;
  }
  Lab lab;
  lab(0, 0) = 116.0f * fy - 16.0f;
  lab(1, 0) = 500.0f * (fx - fy);
  lab(2, 0) = 200.0f * (fy - fz);
  return lab;
}

Convert::Lab Convert::From_xyY_To_Lab(const xyY& rFromxyY,
				      const XYZ& rRefWhite){

  return From_XYZ_To_Lab(From_xyY_To_XYZ(rFromxyY), rRefWhite);
}

Convert::Lab Convert::From_LCHab_To_Lab(const LCHab& rFromLCHab,
					const XYZ& rRefWhite){

  Lab lab;
  lab(0, 0) = rFromLCHab(0, 0);
  lab(1, 0) = rFromLCHab(1, 0) * cos(rFromLCHab(2, 0) * RAD);
  lab(2, 0) = rFromLCHab(1, 0) * sin(rFromLCHab(2, 0) * RAD);
  return lab;
}

Convert::Lab Convert::From_Luv_To_Lab(const Luv& rFromLuv,
				      const XYZ& rRefWhite){

  return From_XYZ_To_Lab(From_Luv_To_XYZ(rFromLuv, rRefWhite), rRefWhite);
}

Convert::Lab Convert::From_LCHuv_To_Lab(const LCHuv& rFromLCHuv,
					const XYZ& rRefWhite){

  return From_XYZ_To_Lab(From_LCHuv_To_XYZ(rFromLCHuv, rRefWhite),
			 rRefWhite);
}

Convert::Lab Convert::From_RGB_To_Lab(const RGB& rFromRGB,
				      const XYZ& rRefWhite,
				      const Matrix3f& M,
				      const float gamma){

  return From_XYZ_To_Lab(From_RGB_To_XYZ(rFromRGB, rRefWhite, M, gamma),
			 rRefWhite);
}

Convert::Lab Convert::From_sRGB_To_Lab(const sRGB& rFromsRGB,
				       const XYZ& rRefWhite,
				       const Matrix3f& M,
				       const float gamma){

  return From_XYZ_To_Lab(From_sRGB_To_XYZ(rFromsRGB, rRefWhite, M, gamma),
			 rRefWhite);
}

/* Convert to __LCHab__ */
Convert::LCHab Convert::From_XYZ_To_LCHab(const XYZ& rFromXYZ,
					  const XYZ& rRefWhite){

  return From_Lab_To_LCHab(From_XYZ_To_Lab(rFromXYZ, rRefWhite),
			   rRefWhite);
}

Convert::LCHab Convert::From_xyY_To_LCHab(const xyY& rFromxyY,
					  const XYZ& rRefWhite){

  return From_XYZ_To_LCHab(From_xyY_To_XYZ(rFromxyY), rRefWhite);
}

Convert::LCHab Convert::From_Lab_To_LCHab(const Lab& rFromLab,
					  const XYZ& rRefWhite){
  LCHab lchab;
  lchab(0, 0) = rFromLab(0, 0);
  lchab(1, 0) = sqrt(rFromLab(1, 0) * rFromLab(1, 0) +
		     rFromLab(2, 0) * rFromLab(2, 0));
  float H = atan2(rFromLab(2, 0), rFromLab(1, 0)) * ANG;
  if(H < 0.0f){ H += 360.0f; }
  if(H >= 360.0f){ H -= 360.0f; }
  lchab(2, 0) = H;
  return lchab;
}

Convert::LCHab Convert::From_Luv_To_LCHab(const Luv& rFromLuv,
					  const XYZ& rRefWhite){

  return From_XYZ_To_LCHab(From_Luv_To_XYZ(rFromLuv, rRefWhite),
			   rRefWhite);
}

Convert::LCHab Convert::From_LCHuv_To_LCHab(const LCHuv& rFromLCHuv,
					    const XYZ& rRefWhite){

  return From_XYZ_To_LCHab(From_LCHuv_To_XYZ(rFromLCHuv, rRefWhite),
			   rRefWhite);
}

Convert::LCHab Convert::From_RGB_To_LCHab(const RGB& rFromRGB,
					  const XYZ& rRefWhite,
					  const Matrix3f& M,
					  const float gamma){

  return From_XYZ_To_LCHab(From_RGB_To_XYZ(rFromRGB, rRefWhite, M, gamma),
			   rRefWhite);
}

Convert::LCHab Convert::From_sRGB_To_LCHab(const sRGB& rFromsRGB,
					   const XYZ& rRefWhite,
					   const Matrix3f& M,
					   const float gamma){

  return
    From_XYZ_To_LCHab(From_sRGB_To_XYZ(rFromsRGB, rRefWhite, M, gamma),
		      rRefWhite);
}

/* Convert to __Luv__ */
Convert::Luv Convert::From_XYZ_To_Luv(const XYZ& rFromXYZ,
				      const XYZ& rRefWhite){

  float yr, up, vp, urp, vrp;
  Luv luv;
  yr = rFromXYZ(1, 0) / rRefWhite(1, 0);
  ComputeUoVo(up, vp, rFromXYZ);
  ComputeUoVo(urp, vrp, rRefWhite);
  if(yr > CIE_fEpsilon){
    luv(0, 0) = 116.0f * pow(yr, 1.0f/3.0f) - 16.0f;
  }else{
    luv(0, 0) = CIE_fKappa * yr;
  }
  luv(1, 0) = 13.0f * luv(0, 0) * (up - urp);
  luv(2, 0) = 13.0f * luv(0, 0) * (vp - vrp);
  return luv;
}

Convert::Luv Convert::From_xyY_To_Luv(const xyY& rFromxyY,
				      const XYZ& rRefWhite){

  return From_XYZ_To_Luv(From_xyY_To_XYZ(rFromxyY), rRefWhite);
}

Convert::Luv Convert::From_Lab_To_Luv(const Luv& rFromLab,
				      const XYZ& rRefWhite){

  return From_XYZ_To_Luv(From_Lab_To_XYZ(rFromLab, rRefWhite), rRefWhite);
}

Convert::Luv Convert::From_LCHab_To_Luv(const LCHab& rFromLCHab,
					const XYZ& rRefWhite){

  return From_XYZ_To_Luv(From_LCHab_To_XYZ(rFromLCHab, rRefWhite),
			 rRefWhite);
}

Convert::Luv Convert::From_LCHuv_To_Luv(const LCHuv& rFromLCHuv,
					const XYZ& rRefWhite){
  Luv luv;
  luv(0, 0) = rFromLCHuv(0, 0);
  luv(1, 0) = rFromLCHuv(1, 0) * cos(rFromLCHuv(2, 0) * RAD);
  luv(2, 0) = rFromLCHuv(1, 0) * sin(rFromLCHuv(2, 0) * RAD);
  return luv;
}

Convert::Luv Convert::From_RGB_To_Luv(const RGB& rFromRGB,
				      const XYZ& rRefWhite,
				      const Matrix3f& M,
				      const float gamma){

  return From_XYZ_To_Luv(From_RGB_To_XYZ(rFromRGB, rRefWhite, M, gamma),
			 rRefWhite);
}

Convert::Luv Convert::From_sRGB_To_Luv(const sRGB& rFromsRGB,
				       const XYZ& rRefWhite,
				       const Matrix3f& M,
				       const float gamma){

  return From_XYZ_To_Luv(From_sRGB_To_XYZ(rFromsRGB, rRefWhite, M, gamma),
			 rRefWhite);
}

/* Convert to __LCHuv__ */
Convert::LCHuv Convert::From_XYZ_To_LCHuv(const XYZ& rFromXYZ,
					  const XYZ& rRefWhite){

  return From_Luv_To_LCHuv(From_XYZ_To_Luv(rFromXYZ, rRefWhite),
			   rRefWhite);
}

Convert::LCHuv Convert::From_xyY_To_LCHuv(const xyY& rFromxyY,
					  const XYZ& rRefWhite){

  return From_XYZ_To_LCHuv(From_xyY_To_XYZ(rFromxyY), rRefWhite);
}

Convert::LCHuv Convert::From_Lab_To_LCHuv(const Lab& rFromLab,
					  const XYZ& rRefWhite){

  return From_XYZ_To_LCHuv(From_Lab_To_XYZ(rFromLab, rRefWhite),
			   rRefWhite);
}

Convert::LCHuv Convert::From_LCHab_To_LCHuv(const LCHab& rFromLCHab,
					    const XYZ& rRefWhite){

  return From_XYZ_To_LCHuv(From_LCHab_To_XYZ(rFromLCHab, rRefWhite),
			   rRefWhite);
}

Convert::LCHuv Convert::From_Luv_To_LCHuv(const Luv& rFromLuv,
					  const XYZ& rRefWhite){
  LCHuv lchuv;
  lchuv(0, 0) = rFromLuv(0, 0);
  lchuv(1, 0) = sqrt(rFromLuv(1, 0) * rFromLuv(1, 0) +
		     rFromLuv(2, 0) * rFromLuv(2, 0));
  float H = atan2(rFromLuv(2, 0), rFromLuv(1, 0)) * ANG;
  if(H < 0.0f){ H += 360.0f; }
  if(H >= 360.0f){ H -= 360.0f; }
  lchuv(2, 0) = H;
  return lchuv;
}

Convert::LCHuv Convert::From_RGB_To_LCHuv(const RGB& rFromRGB,
					  const XYZ& rRefWhite,
					  const Matrix3f& M,
					  const float gamma){

  return
    From_XYZ_To_LCHuv(From_RGB_To_XYZ(rFromRGB, rRefWhite, M, gamma),
		      rRefWhite);
}

Convert::LCHuv Convert::From_sRGB_To_LCHuv(const sRGB& rFromsRGB,
					   const XYZ& rRefWhite,
					   const Matrix3f& M,
					   const float gamma){

  return
    From_XYZ_To_LCHuv(From_sRGB_To_XYZ(rFromsRGB, rRefWhite, M, gamma),
		      rRefWhite);
}

/* Convert to any __RGB__ */
Convert::RGB Convert::From_XYZ_To_RGB(const XYZ& rFromXYZ,
				      const XYZ& rRefWhite,
				      const Matrix3f& M,
				      const float gamma){
  RGB rgb;
  float p = 1.0f / gamma;
  rgb = M * rFromXYZ;
  rgb(0, 0) = pow(rgb(0, 0), p);
  rgb(1, 0) = pow(rgb(1, 0), p);
  rgb(2, 0) = pow(rgb(2, 0), p);
  return rgb;
}

Convert::RGB Convert::From_xyY_To_RGB(const xyY& rFromxyY,
				      const XYZ& rRefWhite,
				      const Matrix3f& M,
				      const float gamma){

  return From_XYZ_To_RGB(From_xyY_To_XYZ(rFromxyY), rRefWhite, M, gamma);
}

Convert::RGB Convert::From_Lab_To_RGB(const Lab& rFromLab,
				      const XYZ& rRefWhite,
				      const Matrix3f& M,
				      const float gamma){

  return From_XYZ_To_RGB(From_Lab_To_XYZ(rFromLab, rRefWhite), rRefWhite,
			 M, gamma);
}

Convert::RGB Convert::From_LCHab_To_RGB(const LCHab& rFromLCHab,
					const XYZ& rRefWhite,
					const Matrix3f& M,
					const float gamma){

  return From_XYZ_To_RGB(From_LCHab_To_XYZ(rFromLCHab, rRefWhite),
			 rRefWhite, M, gamma);
}

Convert::RGB Convert::From_Luv_To_RGB(const Luv& rFromLuv,
				      const XYZ& rRefWhite,
				      const Matrix3f& M,
				      const float gamma){

  return From_XYZ_To_RGB(From_Luv_To_XYZ(rFromLuv, rRefWhite), rRefWhite,
			 M, gamma);
}

Convert::RGB Convert::From_LCHuv_To_RGB(const LCHuv& rFromLCHuv,
					const XYZ& rRefWhite,
					const Matrix3f& M,
					const float gamma){

  return From_XYZ_To_RGB(From_LCHuv_To_XYZ(rFromLCHuv, rRefWhite),
			 rRefWhite, M, gamma);
}

Convert::RGB Convert::From_RGB_To_RGB(const RGB& rFromRGB,
				      const XYZ& rRefWhite,
				      const Matrix3f& M,
				      const float gamma,
				      const Matrix3f& sM,
				      const float sgamma){

  return
    From_XYZ_To_RGB(From_RGB_To_XYZ(rFromRGB, rRefWhite, sM, sgamma),
    		    rRefWhite, M, gamma);
}

Convert::RGB Convert::From_sRGB_To_RGB(const sRGB& rFromsRGB,
				       const XYZ& rRefWhite,
				       const Matrix3f& M,
				       const float gamma,
				       const Matrix3f& sM,
				       const float sgamma){

  return
    From_XYZ_To_RGB(From_sRGB_To_XYZ(rFromsRGB, rRefWhite, sM, sgamma),
		    rRefWhite, M, gamma);
  /* sM and sgamma are needed to convert sRGB to XYZ first. */
}

/* Convert to __sRGB__ */
Convert::sRGB Convert::From_XYZ_To_sRGB(const XYZ& rFromXYZ,
					const XYZ& rRefWhite,
					const Matrix3f& M,
					const float gamma){
  sRGB rgb;
  float p = 1.0f / gamma;
  rgb = M * rFromXYZ;
  if(rgb(0, 0) > 0.0031308f){
    rgb(0, 0) = 1.055f * pow(rgb(0, 0), p) - 0.055f;
  }else{
    rgb(0, 0) = 12.92f * rgb(0, 0);
  }
  if(rgb(1, 0) > 0.0031308f){
    rgb(1, 0) = 1.055f * pow(rgb(1, 0), p) - 0.055f;
  }else{
    rgb(1, 0) = 12.92f * rgb(1, 0);
  }
  if(rgb(2, 0) > 0.0031308f){
    rgb(2, 0) = 1.055f * pow(rgb(2, 0), p) - 0.055f;
  }else{
    rgb(2, 0) = 12.92f * rgb(2, 0);
  }
  return rgb;
}

Convert::sRGB Convert::From_xyY_To_sRGB(const xyY& rFromxyY,
					const XYZ& rRefWhite,
					const Matrix3f& M,
					const float gamma){

  return From_XYZ_To_sRGB(From_xyY_To_XYZ(rFromxyY), rRefWhite, M, gamma);
}

Convert::sRGB Convert::From_Lab_To_sRGB(const Lab& rFromLab,
					const XYZ& rRefWhite,
					const Matrix3f& M,
					const float gamma){

  return From_XYZ_To_sRGB(From_Lab_To_XYZ(rFromLab, rRefWhite), rRefWhite,
			  M, gamma);
}

Convert::sRGB Convert::From_LCHab_To_sRGB(const LCHab& rFromLCHab,
					  const XYZ& rRefWhite,
					  const Matrix3f& M,
					  const float gamma){

  return From_XYZ_To_sRGB(From_LCHab_To_XYZ(rFromLCHab, rRefWhite),
			  rRefWhite, M, gamma);
}

Convert::sRGB Convert::From_Luv_To_sRGB(const Luv& rFromLuv,
					const XYZ& rRefWhite,
					const Matrix3f& M,
					const float gamma){

  return From_XYZ_To_sRGB(From_Luv_To_XYZ(rFromLuv, rRefWhite), rRefWhite,
			  M, gamma);
}

Convert::sRGB Convert::From_LCHuv_To_sRGB(const LCHuv& rFromLCHuv,
					  const XYZ& rRefWhite,
					  const Matrix3f& M,
					  const float gamma){

  return From_XYZ_To_sRGB(From_LCHuv_To_XYZ(rFromLCHuv, rRefWhite),
			  rRefWhite, M, gamma);
}

Convert::sRGB Convert::From_RGB_To_sRGB(const RGB& rFromRGB,
					const XYZ& rRefWhite,
					const Matrix3f& M,
					const float gamma,
					const Matrix3f& sM,
					const float sgamma){
  /* sM and sgamma are needed to convert RGB to XYZ first. */

  return From_XYZ_To_sRGB(From_RGB_To_XYZ(rFromRGB, rRefWhite, sM, sgamma),
			  rRefWhite, M, gamma);
}


Matrix3f Convert::ChromaticAdaptationMatrix(const XYZ& rSource,
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


void Convert::ComputeUoVo(float &uo, float &vo, const XYZ& rRefWhite){

  float d = (rRefWhite(0, 0) + 15.0f *
	     rRefWhite(1, 0) + 3.0f *
	     rRefWhite(2, 0));
  uo = (4.0f * rRefWhite(0, 0)) / d;
  vo = (9.0f * rRefWhite(1, 0)) / d;
}
