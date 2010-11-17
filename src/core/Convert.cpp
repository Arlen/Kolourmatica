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


Input& Input::operator=(const Input& rI){

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


BaseRGB::BaseRGB(xyY redPrimary,
		 xyY greenPrimary,
		 xyY bluePrimary, 
		 XYZ refWhite, 
		 float gamma) :
  redPrimary_(redPrimary),
  greenPrimary_(greenPrimary),
  bluePrimary_(bluePrimary),
  refWhite_(refWhite),
  gamma_(gamma){

  M_ = ConversionMatrixFromRGBtoXYZ(redPrimary,
				    greenPrimary,
				    bluePrimary,
				    refWhite);
  M_1_ = M_.inverse();
  }

Matrix3f BaseRGB::GetM() const{ return M_; }

Matrix3f BaseRGB::GetM_1() const{ return M_1_; }

Matrix3f BaseRGB::GetM_Adapted(const XYZ& rDestination,
			       const Matrix3f& rMethod) const{

  return
    Convert::ChromaticAdaptationMatrix(refWhite_, rDestination, rMethod)
    * M_;
}

Matrix3f BaseRGB::GetM_1_Adapted(const XYZ& rDestination,
				 const Matrix3f& rMethod) const{
  return
    (Convert::ChromaticAdaptationMatrix(refWhite_, rDestination, rMethod)
     * M_).inverse();
}

float BaseRGB::GetGamma() const{ return gamma_; }

Matrix3f BaseRGB::ConversionMatrixFromRGBtoXYZ(const xyY& rCor,
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


Adobe::Adobe() : BaseRGB(
			 xyY(0.64, 0.33, 0.297361),
			 xyY(0.21, 0.71, 0.627355),
			 xyY(0.15, 0.06, 0.075285),
			 Convert::IlluminantD65_,
			 2.2f){ }


Apple::Apple() : BaseRGB(
			 xyY(0.625, 0.34, 0.244634),
			 xyY(0.28, 0.595, 0.672034),
			 xyY(0.155, 0.07, 0.083332),
			 Convert::IlluminantD65_,
			 1.8f){ }


Best::Best() : BaseRGB(
		       xyY(0.7347, 0.2653, 0.228457),
		       xyY(0.2150, 0.7750, 0.737352),
		       xyY(0.1300, 0.0350, 0.034191),
		       Convert::IlluminantD50_,
		       2.2f){ }


Beta::Beta() : BaseRGB(
		       xyY(0.6888, 0.3112, 0.303273),
		       xyY(0.1986, 0.7551, 0.663786),
		       xyY(0.1265, 0.0352, 0.032941),
		       Convert::IlluminantD50_,
		       2.2f){ }


Bruce::Bruce() : BaseRGB(
			 xyY(0.6400, 0.3300, 0.240995),
			 xyY(0.2800, 0.6500, 0.683554),
			 xyY(0.1500, 0.0600, 0.075452),
			 Convert::IlluminantD65_,
			 2.2f){ }


CIE::CIE() : BaseRGB(
		     xyY(0.7350, 0.2650, 0.176204),
		     xyY(0.2740, 0.7170, 0.812985),
		     xyY(0.1670, 0.0090, 0.010811),
		     Convert::IlluminantE_,
		     2.2f){ }


ColorMatch::ColorMatch() : BaseRGB(
				   xyY(0.6300, 0.3400, 0.274884),
				   xyY(0.2950, 0.6050, 0.658132),
				   xyY(0.1500, 0.0750, 0.066985),
				   Convert::IlluminantD50_,
				   1.8f){ }


Don4::Don4() : BaseRGB(
		       xyY(0.6960, 0.3000, 0.278350),
		       xyY(0.2150, 0.7650, 0.687970),
		       xyY(0.1300, 0.0350, 0.033680),
		       Convert::IlluminantD50_,
		       2.2f){ }


ECI::ECI() : BaseRGB(
		     xyY(0.6700, 0.3300, 0.320250),
		     xyY(0.2100, 0.7100, 0.602071),
		     xyY(0.1400, 0.0800, 0.077679),
		     Convert::IlluminantD50_,
		     1.8f){ }


EktaSpacePS5::EktaSpacePS5() : BaseRGB(
				       xyY(0.6950, 0.3050, 0.260629),
				       xyY(0.2600, 0.7000, 0.734946),
				       xyY(0.1100, 0.0050, 0.004425),
				       Convert::IlluminantD50_,
				       2.2f){ }


NTSC::NTSC() : BaseRGB(
		       xyY(0.6700, 0.3300, 0.298839),
		       xyY(0.2100, 0.7100, 0.586811),
		       xyY(0.1400, 0.0800, 0.114350),
		       Convert::IlluminantC_,
		       2.2f){ }


PAL_SECAM::PAL_SECAM() : BaseRGB(
				 xyY(0.6400, 0.3300, 0.222021),
				 xyY(0.2900, 0.6000, 0.706645),
				 xyY(0.1500, 0.0600, 0.071334),
				 Convert::IlluminantD65_,
				 2.2f){ }


ProPhoto::ProPhoto() : BaseRGB(
			       xyY(0.7347, 0.2653, 0.288040),
			       xyY(0.1596, 0.8404, 0.711874),
			       xyY(0.0366, 0.0001, 0.000086),
			       Convert::IlluminantD50_,
			       1.8f){ }


SMPTE_C::SMPTE_C() : BaseRGB(
			     xyY(0.6300, 0.3400, 0.212395),
			     xyY(0.3100, 0.5950, 0.701049),
			     xyY(0.1550, 0.0700, 0.086556),
			     Convert::IlluminantD65_,
			     2.2f){ }


SRGB::SRGB() : BaseRGB(
		       xyY(0.6400, 0.3300, 0.212656),
		       xyY(0.3000, 0.6000, 0.715158),
		       xyY(0.1500, 0.0600, 0.072186),
		       Convert::IlluminantD65_,
		       2.4f){ }


WideGamut::WideGamut() : BaseRGB(
				 xyY(0.7350, 0.2650, 0.258187),
				 xyY(0.1150, 0.8260, 0.724938),
				 xyY(0.1570, 0.0180, 0.016875),
				 Convert::IlluminantD50_,
				 2.2f){ }


const float Convert::CIE_fEpsilon = 216.0f / 24389.0f;
const float Convert::CIE_fKappa = 24389.0f / 27.0f;
const float Convert::CIE_fKE = CIE_fKappa * CIE_fEpsilon;
const float Convert::PIE = 3.1415926535897932;
const float Convert::RAD = PIE / 180.0f;
const float Convert::ANG = 180.0f / PIE;

const XYZ Convert::IlluminantA_   =
  XYZ(1.09850, 1.0, 0.35585);
const XYZ Convert::IlluminantB_   =
  XYZ(0.99072, 1.0, 0.85223);
const XYZ Convert::IlluminantC_   =
  XYZ(0.98074, 1.0, 1.18232);
const XYZ Convert::IlluminantD50_ =
  XYZ(0.96422, 1.0, 0.82521);
const XYZ Convert::IlluminantD55_ =
  XYZ(0.95682, 1.0, 0.92149);
const XYZ Convert::IlluminantD65_ =
  XYZ(0.95047, 1.0, 1.08883);
const XYZ Convert::IlluminantD75_ =
  XYZ(0.94972, 1.0, 1.22638);
const XYZ Convert::IlluminantE_   =
  XYZ(1.00000, 1.0, 1.00000);
const XYZ Convert::IlluminantF2_  =
  XYZ(0.99186, 1.0, 0.67393);
const XYZ Convert::IlluminantF7_  =
  XYZ(0.95041, 1.0, 1.08747);
const XYZ Convert::IlluminantF11_ =
  XYZ(1.00962, 1.0, 0.64350);

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
XYZ Convert::From_xyY_To_XYZ(const Input& rI){

  XYZ xyz;
  xyz(0, 0) = rI.from_(0, 0) / rI.from_(1, 0) * rI.from_(2, 0);
  xyz(1, 0) = rI.from_(2, 0);
  xyz(2, 0) = ((1.0f - rI.from_(0, 0) - rI.from_(1, 0)) /
	       rI.from_(1, 0)) * rI.from_(2, 0);
  return xyz;
}

XYZ Convert::From_Lab_To_XYZ(const Input& rI){

  XYZ xyz;
  float fx, fy, fz, fxCube, fzCube, xr, yr, zr;
  fy = (rI.from_(0, 0) + 16.0f) / 116.0f;
  fx = (rI.from_(1, 0) / 500.0f) + fy;
  fz = fy - (rI.from_(2, 0) / 200.0f);
  fxCube = fx * fx * fx;
  fzCube = fz * fz * fz;
  if(fxCube > CIE_fEpsilon){ xr = fxCube; }
  else{
    xr = ((116.0f * fx) - 16.0f) / CIE_fKappa;
  }
  if(rI.from_(0, 0) > CIE_fKE){
    yr = pow( (rI.from_(0, 0) + 16.0f) / 116.0f, 3.0f );
  }else{

    yr = rI.from_(0, 0) / CIE_fKappa;
  }
  if(fzCube > CIE_fEpsilon){ zr = fzCube; }
  else{
    zr = ((116.0f * fz) - 16.0f) / CIE_fKappa;
  }
  xyz(0, 0) = xr * rI.white_(0, 0);
  xyz(1, 0) = yr * rI.white_(1, 0);
  xyz(2, 0) = zr * rI.white_(2, 0);
  return xyz;
}

XYZ Convert::From_LCHab_To_XYZ(const Input& rI){

  Input tmp = rI;
  tmp.from_ = From_LCHab_To_Lab(rI);
  return From_Lab_To_XYZ(tmp);
}

XYZ Convert::From_Luv_To_XYZ(const Input& rI){

  float a, b, c, d, uo, vo;
  XYZ xyz;
  ComputeUoVo(uo, vo, rI.white_);
  c = -1.0f / 3.0f;
  a = (((52.0f * rI.from_(0, 0)) /
	(rI.from_(1, 0) + 13.0f * rI.from_(0, 0) * uo)) - 1.0f) / 3.0f;
  if( rI.from_(0, 0) > CIE_fKE ){
    xyz(1, 0) = pow( (rI.from_(0, 0) + 16.0f) / 116.0f, 3.0f );
  }else{
    xyz(1, 0) = rI.from_(0, 0) / CIE_fKappa;
  }      
  b = -5.0 * xyz(1, 0);
  d = (((39.0f * rI.from_(0, 0)) /
	(rI.from_(2, 0) + 13.0f * rI.from_(0, 0) * vo)) - 5.0f) *
    xyz(1, 0);
  xyz(0, 0) = (d - b) / (a - c);
  xyz(2, 0) = xyz(0, 0) * a + b;
  return xyz;
}

XYZ Convert::From_LCHuv_To_XYZ(const Input& rI){

  Input tmp = rI;
  tmp.from_ = From_LCHuv_To_Luv(rI);
  return From_Luv_To_XYZ(tmp);
}

XYZ Convert::From_RGB_To_XYZ(const Input& rI){

  RGB rgb;
  XYZ xyz;
  rgb(0, 0) = pow(rI.from_(0, 0), rI.sourceGamma_);
  rgb(1, 0) = pow(rI.from_(1, 0), rI.sourceGamma_);
  rgb(2, 0) = pow(rI.from_(2, 0), rI.sourceGamma_);
  xyz = rI.sourceM_ * rgb;
  return xyz;
}

XYZ Convert::From_sRGB_To_XYZ(const Input& rI){

  sRGB rgb;
  XYZ xyz;
  if(rI.from_(0, 0) > 0.04045f){
    rgb(0, 0) = pow( ((rI.from_(0, 0) + 0.055f) / 1.055f),
		     rI.sourceGamma_ );
  }else{
    rgb(0, 0) = rI.from_(0, 0) / 12.92f;
  }
  if(rI.from_(1, 0) > 0.04045f){
    rgb(1, 0) = pow( ((rI.from_(1, 0) + 0.055f) / 1.055f),
		     rI.sourceGamma_ );
  }else{
    rgb(1, 0) = rI.from_(1, 0) / 12.92f;
  }
  if(rI.from_(2, 0) > 0.04045f){
    rgb(2, 0) = pow( ((rI.from_(2, 0) + 0.055f) / 1.055f),
		     rI.sourceGamma_ );
  }else{
    rgb(2, 0) = rI.from_(2, 0) / 12.92f;
  }
  xyz = rI.sourceM_ * rgb;
  return xyz;
}

/* Convert to __xyY__ */
xyY Convert::From_XYZ_To_xyY(const Input& rI){

  xyY xyy;
  float s = rI.from_(0, 0) + rI.from_(1, 0) + rI.from_(2, 0);
  xyy(0, 0) = rI.from_(0, 0) / s;
  xyy(1, 0) = rI.from_(1, 0) / s;
  xyy(2, 0) = rI.from_(1, 0);
  return xyy;
}

xyY Convert::From_Lab_To_xyY(const Input& rI){

  Input tmp = rI;
  tmp.from_ = From_Lab_To_XYZ(rI);
  return From_XYZ_To_xyY(tmp);
}

xyY Convert::From_LCHab_To_xyY(const Input& rI){

  Input tmp = rI;
  tmp.from_ = From_LCHab_To_XYZ(rI);
  return From_XYZ_To_xyY(tmp);
}

xyY Convert::From_Luv_To_xyY(const Input& rI){

  Input tmp = rI;
  tmp.from_ = From_Luv_To_XYZ(rI);
  return From_XYZ_To_xyY(tmp);
}

xyY Convert::From_LCHuv_To_xyY(const Input& rI){

  Input tmp = rI;
  tmp.from_ = From_LCHuv_To_XYZ(rI);
  return From_XYZ_To_xyY(tmp);
}

xyY Convert::From_RGB_To_xyY(const Input& rI){

  Input tmp = rI;
  tmp.from_ = From_RGB_To_XYZ(rI);
  return From_XYZ_To_xyY(tmp);
}

xyY Convert::From_sRGB_To_xyY(const Input& rI){

  Input tmp = rI;
  tmp.from_ = From_sRGB_To_XYZ(rI);
  return From_XYZ_To_xyY(tmp);
}

/* Convert to __Lab__ */
Lab Convert::From_XYZ_To_Lab(const Input& rI){

  float xr, yr, zr, fx, fy, fz;
  xr = rI.from_(0, 0) / rI.white_(0, 0);
  yr = rI.from_(1, 0) / rI.white_(1, 0);
  zr = rI.from_(2, 0) / rI.white_(2, 0);
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

Lab Convert::From_xyY_To_Lab(const Input& rI){

  Input tmp = rI;
  tmp.from_ = From_xyY_To_XYZ(rI);
  return From_XYZ_To_Lab(tmp);
}

Lab Convert::From_LCHab_To_Lab(const Input& rI){

  Lab lab;
  lab(0, 0) = rI.from_(0, 0);
  lab(1, 0) = rI.from_(1, 0) * cos(rI.from_(2, 0) * RAD);
  lab(2, 0) = rI.from_(1, 0) * sin(rI.from_(2, 0) * RAD);
  return lab;
}

Lab Convert::From_Luv_To_Lab(const Input& rI){

  Input tmp = rI;
  tmp.from_ = From_Luv_To_XYZ(rI);
  return From_XYZ_To_Lab(tmp);
}

Lab Convert::From_LCHuv_To_Lab(const Input& rI){

  Input tmp = rI;
  tmp.from_ = From_LCHuv_To_XYZ(rI);
  return From_XYZ_To_Lab(tmp);
}

Lab Convert::From_RGB_To_Lab(const Input& rI){

  Input tmp = rI;
  tmp.from_ = From_RGB_To_XYZ(rI);
  return From_XYZ_To_Lab(tmp);
}

Lab Convert::From_sRGB_To_Lab(const Input& rI){

  Input tmp = rI;
  tmp.from_ = From_sRGB_To_XYZ(rI);
  return From_XYZ_To_Lab(tmp);
}

/* Convert to __LCHab__ */
LCHab Convert::From_XYZ_To_LCHab(const Input& rI){

  Input tmp = rI;
  tmp.from_ = From_XYZ_To_Lab(rI);
  return From_Lab_To_LCHab(tmp);
}

LCHab Convert::From_xyY_To_LCHab(const Input& rI){

  Input tmp = rI;
  tmp.from_ = From_xyY_To_XYZ(rI);
  return From_XYZ_To_LCHab(tmp);
}

LCHab Convert::From_Lab_To_LCHab(const Input& rI){

  LCHab lchab;
  lchab(0, 0) = rI.from_(0, 0);
  lchab(1, 0) = sqrt(rI.from_(1, 0) * rI.from_(1, 0) +
		     rI.from_(2, 0) * rI.from_(2, 0));
  float H = atan2(rI.from_(2, 0), rI.from_(1, 0)) * ANG;
  if(H < 0.0f){ H += 360.0f; }
  if(H >= 360.0f){ H -= 360.0f; }
  lchab(2, 0) = H;
  return lchab;
}

LCHab Convert::From_Luv_To_LCHab(const Input& rI){

  Input tmp = rI;
  tmp.from_ = From_Luv_To_XYZ(rI);
  return From_XYZ_To_LCHab(tmp);
}

LCHab Convert::From_LCHuv_To_LCHab(const Input& rI){

  Input tmp = rI;
  tmp.from_ = From_LCHuv_To_XYZ(rI);
  return From_XYZ_To_LCHab(tmp);
}

LCHab Convert::From_RGB_To_LCHab(const Input& rI){

  Input tmp = rI;
  tmp.from_ = From_RGB_To_XYZ(rI);
  return From_XYZ_To_LCHab(tmp);
}

LCHab Convert::From_sRGB_To_LCHab(const Input& rI){

  Input tmp = rI;
  tmp.from_ = From_sRGB_To_XYZ(rI);
  return From_XYZ_To_LCHab(tmp);
}

/* Convert to __Luv__ */
Luv Convert::From_XYZ_To_Luv(const Input& rI){

  float yr, up, vp, urp, vrp;
  Luv luv;
  yr = rI.from_(1, 0) / rI.white_(1, 0);
  ComputeUoVo(up, vp, rI.from_);
  ComputeUoVo(urp, vrp, rI.white_);
  if(yr > CIE_fEpsilon){
    luv(0, 0) = 116.0f * pow(yr, 1.0f/3.0f) - 16.0f;
  }else{
    luv(0, 0) = CIE_fKappa * yr;
  }
  luv(1, 0) = 13.0f * luv(0, 0) * (up - urp);
  luv(2, 0) = 13.0f * luv(0, 0) * (vp - vrp);
  return luv;
}

Luv Convert::From_xyY_To_Luv(const Input& rI){

  Input tmp = rI;
  tmp.from_ = From_xyY_To_XYZ(rI);
  return From_XYZ_To_Luv(tmp);
}

Luv Convert::From_Lab_To_Luv(const Input& rI){

  Input tmp = rI;
  tmp.from_ = From_Lab_To_XYZ(rI);
  return From_XYZ_To_Luv(tmp);
}

Luv Convert::From_LCHab_To_Luv(const Input& rI){

  Input tmp = rI;
  tmp.from_ = From_LCHab_To_XYZ(rI);
  return From_XYZ_To_Luv(tmp);
}

Luv Convert::From_LCHuv_To_Luv(const Input& rI){

  Luv luv;
  luv(0, 0) = rI.from_(0, 0);
  luv(1, 0) = rI.from_(1, 0) * cos(rI.from_(2, 0) * RAD);
  luv(2, 0) = rI.from_(1, 0) * sin(rI.from_(2, 0) * RAD);
  return luv;
}

Luv Convert::From_RGB_To_Luv(const Input& rI){

  Input tmp = rI;
  tmp.from_ = From_RGB_To_XYZ(rI);
  return From_XYZ_To_Luv(tmp);
}

Luv Convert::From_sRGB_To_Luv(const Input& rI){

  Input tmp = rI;
  tmp.from_ = From_sRGB_To_XYZ(rI);
  return From_XYZ_To_Luv(tmp);
}

/* Convert to __LCHuv__ */
LCHuv Convert::From_XYZ_To_LCHuv(const Input& rI){

  Input tmp = rI;
  tmp.from_ = From_XYZ_To_Luv(rI);
  return From_Luv_To_LCHuv(tmp);
}

LCHuv Convert::From_xyY_To_LCHuv(const Input& rI){

  Input tmp = rI;
  tmp.from_ = From_xyY_To_XYZ(rI);
  return From_XYZ_To_LCHuv(tmp);
}

LCHuv Convert::From_Lab_To_LCHuv(const Input& rI){

  Input tmp = rI;
  tmp.from_ = From_Lab_To_XYZ(rI);
  return From_XYZ_To_LCHuv(tmp);
}

LCHuv Convert::From_LCHab_To_LCHuv(const Input& rI){

  Input tmp = rI;
  tmp.from_ = From_LCHab_To_XYZ(rI);
  return From_XYZ_To_LCHuv(tmp);
}

LCHuv Convert::From_Luv_To_LCHuv(const Input& rI){

  LCHuv lchuv;
  lchuv(0, 0) = rI.from_(0, 0);
  lchuv(1, 0) = sqrt(rI.from_(1, 0) * rI.from_(1, 0) +
		     rI.from_(2, 0) * rI.from_(2, 0));
  float H = atan2(rI.from_(2, 0), rI.from_(1, 0)) * ANG;
  if(H < 0.0f){ H += 360.0f; }
  if(H >= 360.0f){ H -= 360.0f; }
  lchuv(2, 0) = H;
  return lchuv;
}

LCHuv Convert::From_RGB_To_LCHuv(const Input& rI){

  Input tmp = rI;
  tmp.from_ = From_RGB_To_XYZ(rI);
  return From_XYZ_To_LCHuv(tmp);
}

LCHuv Convert::From_sRGB_To_LCHuv(const Input& rI){

  Input tmp = rI;
  tmp.from_ = From_sRGB_To_XYZ(rI);
  return From_XYZ_To_LCHuv(tmp);
}

/* Convert to any __RGB__ */
RGB Convert::From_XYZ_To_RGB(const Input& rI){

  RGB rgb;
  float p = 1.0f / rI.targetGamma_;
  rgb = rI.targetM_1_ * rI.from_;
  rgb(0, 0) = pow(rgb(0, 0), p);
  rgb(1, 0) = pow(rgb(1, 0), p);
  rgb(2, 0) = pow(rgb(2, 0), p);
  return rgb;
}

RGB Convert::From_xyY_To_RGB(const Input& rI){

  Input tmp = rI;
  tmp.from_ = From_xyY_To_XYZ(rI);
  return From_XYZ_To_RGB(tmp);
}

RGB Convert::From_Lab_To_RGB(const Input& rI){

  Input tmp = rI;
  tmp.from_ = From_Lab_To_XYZ(rI);
  return From_XYZ_To_RGB(tmp);
}

RGB Convert::From_LCHab_To_RGB(const Input& rI){

  Input tmp = rI;
  tmp.from_ = From_LCHab_To_XYZ(rI);
  return From_XYZ_To_RGB(tmp);
}

RGB Convert::From_Luv_To_RGB(const Input& rI){

  Input tmp = rI;
  tmp.from_ = From_Luv_To_XYZ(rI);
  return From_XYZ_To_RGB(tmp);
}

RGB Convert::From_LCHuv_To_RGB(const Input& rI){

  Input tmp = rI;
  tmp.from_ = From_LCHuv_To_XYZ(rI);
  return From_XYZ_To_RGB(tmp);
}

RGB Convert::From_RGB_To_RGB(const Input& rI){

  Input tmp = rI;
  tmp.from_ = From_RGB_To_XYZ(rI);
  return From_XYZ_To_RGB(tmp);
}

RGB Convert::From_sRGB_To_RGB(const Input& rI){

  Input tmp = rI;
  tmp.from_ = From_sRGB_To_XYZ(rI);
  return From_XYZ_To_RGB(tmp);
  /* sM and sgamma are needed to convert sRGB to XYZ first. */
}

/* Convert to __sRGB__ */
sRGB Convert::From_XYZ_To_sRGB(const Input& rI){

  sRGB rgb;
  float p = 1.0f / rI.targetGamma_;
  rgb = rI.targetM_1_ * rI.from_;
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

sRGB Convert::From_xyY_To_sRGB(const Input& rI){

  Input tmp = rI;
  tmp.from_ = From_xyY_To_XYZ(rI);
  return From_XYZ_To_sRGB(tmp);
}

sRGB Convert::From_Lab_To_sRGB(const Input& rI){

  Input tmp = rI;
  tmp.from_ = From_Lab_To_XYZ(rI);
  return From_XYZ_To_sRGB(tmp);
}

sRGB Convert::From_LCHab_To_sRGB(const Input& rI){

  Input tmp = rI;
  tmp.from_ = From_LCHab_To_XYZ(rI);
  return From_XYZ_To_sRGB(tmp);
}

sRGB Convert::From_Luv_To_sRGB(const Input& rI){

  Input tmp = rI;
  tmp.from_ = From_Luv_To_XYZ(rI);
  return From_XYZ_To_sRGB(tmp);
}

sRGB Convert::From_LCHuv_To_sRGB(const Input& rI){

  Input tmp = rI;
  tmp.from_ = From_LCHuv_To_XYZ(rI);
  return From_XYZ_To_sRGB(tmp);
}

sRGB Convert::From_RGB_To_sRGB(const Input& rI){
  /* sM and sgamma are needed to convert RGB to XYZ first. */

  Input tmp = rI;
  tmp.from_ = From_RGB_To_XYZ(rI);
  return From_XYZ_To_sRGB(tmp);
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


Conversions::Conversions(){

  /*
    C is the conversion function index.
    F is the From index.
    T is the To index.
  */
  enum{ C = 0, F = 1, T = 2 };

  /* from XYZ to all. */
  t_[CSXYZ][CSxyY].get<C>() = Convert::From_XYZ_To_xyY;
  t_[CSXYZ][CSLab].get<C>() = Convert::From_XYZ_To_Lab;
  t_[CSXYZ][CSLCHab].get<C>() = Convert::From_XYZ_To_LCHab;
  t_[CSXYZ][CSLuv].get<C>() = Convert::From_XYZ_To_Luv;
  t_[CSXYZ][CSLCHuv].get<C>() = Convert::From_XYZ_To_LCHuv;
  t_[CSXYZ][CSAdobeRGB].get<C>() = Convert::From_XYZ_To_RGB;
  t_[CSXYZ][CSAdobeRGB].get<T>() = &adobe_;
  t_[CSXYZ][CSAppleRGB].get<C>() = Convert::From_XYZ_To_RGB;
  t_[CSXYZ][CSAppleRGB].get<T>() = &apple_;
  t_[CSXYZ][CSBestRGB].get<C>() = Convert::From_XYZ_To_RGB;
  t_[CSXYZ][CSBestRGB].get<T>() = &best_;
  t_[CSXYZ][CSBetaRGB].get<C>() = Convert::From_XYZ_To_RGB;
  t_[CSXYZ][CSBetaRGB].get<T>() = &beta_;
  t_[CSXYZ][CSBruceRGB].get<C>() = Convert::From_XYZ_To_RGB;
  t_[CSXYZ][CSBruceRGB].get<T>() = &bruce_;
  t_[CSXYZ][CSCIERGB].get<C>() = Convert::From_XYZ_To_RGB;
  t_[CSXYZ][CSCIERGB].get<T>() = &cie_;
  t_[CSXYZ][CSColorMatchRGB].get<C>() = Convert::From_XYZ_To_RGB;
  t_[CSXYZ][CSColorMatchRGB].get<T>() = &cm_;
  t_[CSXYZ][CSDonRGB4].get<C>() = Convert::From_XYZ_To_RGB;
  t_[CSXYZ][CSDonRGB4].get<T>() = &don4_;
  t_[CSXYZ][CSECIRGB].get<C>() = Convert::From_XYZ_To_RGB;
  t_[CSXYZ][CSECIRGB].get<T>() = &eci_;
  t_[CSXYZ][CSEktaSpacePS5].get<C>() = Convert::From_XYZ_To_RGB;
  t_[CSXYZ][CSEktaSpacePS5].get<T>() = &ekps5_;
  t_[CSXYZ][CSNTSCRGB].get<C>() = Convert::From_XYZ_To_RGB;
  t_[CSXYZ][CSNTSCRGB].get<T>() = &ntsc_;
  t_[CSXYZ][CSPALSECAMRGB].get<C>() = Convert::From_XYZ_To_RGB;
  t_[CSXYZ][CSPALSECAMRGB].get<T>() = &pals_;
  t_[CSXYZ][CSProPhotoRGB].get<C>() = Convert::From_XYZ_To_RGB;
  t_[CSXYZ][CSProPhotoRGB].get<T>() = &pro_;
  t_[CSXYZ][CSSMPTECRGB].get<C>() = Convert::From_XYZ_To_RGB;
  t_[CSXYZ][CSSMPTECRGB].get<T>() = &smpt_;
  t_[CSXYZ][CSsRGB].get<C>() = Convert::From_XYZ_To_sRGB;
  t_[CSXYZ][CSsRGB].get<T>() = &srgb_;
  t_[CSXYZ][CSWideGamutRGB].get<C>() = Convert::From_XYZ_To_RGB;
  t_[CSXYZ][CSWideGamutRGB].get<T>() = &wide_;

  /* from xyY to all. */
  t_[CSxyY][CSXYZ].get<C>() = Convert::From_xyY_To_XYZ;
  t_[CSxyY][CSLab].get<C>() = Convert::From_xyY_To_Lab;
  t_[CSxyY][CSLCHab].get<C>() = Convert::From_xyY_To_LCHab;
  t_[CSxyY][CSLuv].get<C>() = Convert::From_xyY_To_Luv;
  t_[CSxyY][CSLCHuv].get<C>() = Convert::From_xyY_To_LCHuv;
  t_[CSxyY][CSAdobeRGB].get<C>() = Convert::From_xyY_To_RGB;
  t_[CSxyY][CSAdobeRGB].get<T>() = &adobe_;
  t_[CSxyY][CSAppleRGB].get<C>() = Convert::From_xyY_To_RGB;
  t_[CSxyY][CSAppleRGB].get<T>() = &apple_;
  t_[CSxyY][CSBestRGB].get<C>() = Convert::From_xyY_To_RGB;
  t_[CSxyY][CSBestRGB].get<T>() = &best_;
  t_[CSxyY][CSBetaRGB].get<C>() = Convert::From_xyY_To_RGB;
  t_[CSxyY][CSBetaRGB].get<T>() = &beta_;
  t_[CSxyY][CSBruceRGB].get<C>() = Convert::From_xyY_To_RGB;
  t_[CSxyY][CSBruceRGB].get<T>() = &bruce_;
  t_[CSxyY][CSCIERGB].get<C>() = Convert::From_xyY_To_RGB;
  t_[CSxyY][CSCIERGB].get<T>() = &cie_;
  t_[CSxyY][CSColorMatchRGB].get<C>() = Convert::From_xyY_To_RGB;
  t_[CSxyY][CSColorMatchRGB].get<T>() = &cm_;
  t_[CSxyY][CSDonRGB4].get<C>() = Convert::From_xyY_To_RGB;
  t_[CSxyY][CSDonRGB4].get<T>() = &don4_;
  t_[CSxyY][CSECIRGB].get<C>() = Convert::From_xyY_To_RGB;
  t_[CSxyY][CSECIRGB].get<T>() = &eci_;
  t_[CSxyY][CSEktaSpacePS5].get<C>() = Convert::From_xyY_To_RGB;
  t_[CSxyY][CSEktaSpacePS5].get<T>() = &ekps5_;
  t_[CSxyY][CSNTSCRGB].get<C>() = Convert::From_xyY_To_RGB;
  t_[CSxyY][CSNTSCRGB].get<T>() = &ntsc_;
  t_[CSxyY][CSPALSECAMRGB].get<C>() = Convert::From_xyY_To_RGB;
  t_[CSxyY][CSPALSECAMRGB].get<T>() = &pals_;
  t_[CSxyY][CSProPhotoRGB].get<C>() = Convert::From_xyY_To_RGB;
  t_[CSxyY][CSProPhotoRGB].get<T>() = &pro_;
  t_[CSxyY][CSSMPTECRGB].get<C>() = Convert::From_xyY_To_RGB;
  t_[CSxyY][CSSMPTECRGB].get<T>() = &smpt_;
  t_[CSxyY][CSsRGB].get<C>() = Convert::From_xyY_To_sRGB;
  t_[CSxyY][CSsRGB].get<T>() = &srgb_;
  t_[CSxyY][CSWideGamutRGB].get<C>() = Convert::From_xyY_To_RGB;
  t_[CSxyY][CSWideGamutRGB].get<T>() = &wide_;

  /* from Lab to all. */
  t_[CSLab][CSXYZ].get<C>() = Convert::From_Lab_To_XYZ;
  t_[CSLab][CSxyY].get<C>() = Convert::From_Lab_To_xyY;
  t_[CSLab][CSLCHab].get<C>() = Convert::From_Lab_To_LCHab;
  t_[CSLab][CSLuv].get<C>() = Convert::From_Lab_To_Luv;
  t_[CSLab][CSLCHuv].get<C>() = Convert::From_Lab_To_LCHuv;
  t_[CSLab][CSAdobeRGB].get<C>() = Convert::From_Lab_To_RGB;
  t_[CSLab][CSAdobeRGB].get<T>() = &adobe_;
  t_[CSLab][CSAppleRGB].get<C>() = Convert::From_Lab_To_RGB;
  t_[CSLab][CSAppleRGB].get<T>() = &apple_;
  t_[CSLab][CSBestRGB].get<C>() = Convert::From_Lab_To_RGB;
  t_[CSLab][CSBestRGB].get<T>() = &best_;
  t_[CSLab][CSBetaRGB].get<C>() = Convert::From_Lab_To_RGB;
  t_[CSLab][CSBetaRGB].get<T>() = &beta_;
  t_[CSLab][CSBruceRGB].get<C>() = Convert::From_Lab_To_RGB;
  t_[CSLab][CSBruceRGB].get<T>() = &bruce_;
  t_[CSLab][CSCIERGB].get<C>() = Convert::From_Lab_To_RGB;
  t_[CSLab][CSCIERGB].get<T>() = &cie_;
  t_[CSLab][CSColorMatchRGB].get<C>() = Convert::From_Lab_To_RGB;
  t_[CSLab][CSColorMatchRGB].get<T>() = &cm_;
  t_[CSLab][CSDonRGB4].get<C>() = Convert::From_Lab_To_RGB;
  t_[CSLab][CSDonRGB4].get<T>() = &don4_;
  t_[CSLab][CSECIRGB].get<C>() = Convert::From_Lab_To_RGB;
  t_[CSLab][CSECIRGB].get<T>() = &eci_;
  t_[CSLab][CSEktaSpacePS5].get<C>() = Convert::From_Lab_To_RGB;
  t_[CSLab][CSEktaSpacePS5].get<T>() = &ekps5_;
  t_[CSLab][CSNTSCRGB].get<C>() = Convert::From_Lab_To_RGB;
  t_[CSLab][CSNTSCRGB].get<T>() = &ntsc_;
  t_[CSLab][CSPALSECAMRGB].get<C>() = Convert::From_Lab_To_RGB;
  t_[CSLab][CSPALSECAMRGB].get<T>() = &pals_;
  t_[CSLab][CSProPhotoRGB].get<C>() = Convert::From_Lab_To_RGB;
  t_[CSLab][CSProPhotoRGB].get<T>() = &pro_;
  t_[CSLab][CSSMPTECRGB].get<C>() = Convert::From_Lab_To_RGB;
  t_[CSLab][CSSMPTECRGB].get<T>() = &smpt_;
  t_[CSLab][CSsRGB].get<C>() = Convert::From_Lab_To_sRGB;
  t_[CSLab][CSsRGB].get<T>() = &srgb_;
  t_[CSLab][CSWideGamutRGB].get<C>() = Convert::From_Lab_To_RGB;
  t_[CSLab][CSWideGamutRGB].get<T>() = &wide_;

  /* from LCHab to all. */
  t_[CSLCHab][CSXYZ].get<C>() = Convert::From_LCHab_To_XYZ;
  t_[CSLCHab][CSxyY].get<C>() = Convert::From_LCHab_To_xyY;
  t_[CSLCHab][CSLab].get<C>() = Convert::From_LCHab_To_Lab;
  t_[CSLCHab][CSLuv].get<C>() = Convert::From_LCHab_To_Luv;
  t_[CSLCHab][CSLCHuv].get<C>() = Convert::From_LCHab_To_LCHuv;
  t_[CSLCHab][CSAdobeRGB].get<C>() = Convert::From_LCHab_To_RGB;
  t_[CSLCHab][CSAdobeRGB].get<T>() = &adobe_;
  t_[CSLCHab][CSAppleRGB].get<C>() = Convert::From_LCHab_To_RGB;
  t_[CSLCHab][CSAppleRGB].get<T>() = &apple_;
  t_[CSLCHab][CSBestRGB].get<C>() = Convert::From_LCHab_To_RGB;
  t_[CSLCHab][CSBestRGB].get<T>() = &best_;
  t_[CSLCHab][CSBetaRGB].get<C>() = Convert::From_LCHab_To_RGB;
  t_[CSLCHab][CSBetaRGB].get<T>() = &beta_;
  t_[CSLCHab][CSBruceRGB].get<C>() = Convert::From_LCHab_To_RGB;
  t_[CSLCHab][CSBruceRGB].get<T>() = &bruce_;
  t_[CSLCHab][CSCIERGB].get<C>() = Convert::From_LCHab_To_RGB;
  t_[CSLCHab][CSCIERGB].get<T>() = &cie_;
  t_[CSLCHab][CSColorMatchRGB].get<C>() = Convert::From_LCHab_To_RGB;
  t_[CSLCHab][CSColorMatchRGB].get<T>() = &cm_;
  t_[CSLCHab][CSDonRGB4].get<C>() = Convert::From_LCHab_To_RGB;
  t_[CSLCHab][CSDonRGB4].get<T>() = &don4_;
  t_[CSLCHab][CSECIRGB].get<C>() = Convert::From_LCHab_To_RGB;
  t_[CSLCHab][CSECIRGB].get<T>() = &eci_;
  t_[CSLCHab][CSEktaSpacePS5].get<C>() = Convert::From_LCHab_To_RGB;
  t_[CSLCHab][CSEktaSpacePS5].get<T>() = &ekps5_;
  t_[CSLCHab][CSNTSCRGB].get<C>() = Convert::From_LCHab_To_RGB;
  t_[CSLCHab][CSNTSCRGB].get<T>() = &ntsc_;
  t_[CSLCHab][CSPALSECAMRGB].get<C>() = Convert::From_LCHab_To_RGB;
  t_[CSLCHab][CSPALSECAMRGB].get<T>() = &pals_;
  t_[CSLCHab][CSProPhotoRGB].get<C>() = Convert::From_LCHab_To_RGB;
  t_[CSLCHab][CSProPhotoRGB].get<T>() = &pro_;
  t_[CSLCHab][CSSMPTECRGB].get<C>() = Convert::From_LCHab_To_RGB;
  t_[CSLCHab][CSSMPTECRGB].get<T>() = &smpt_;
  t_[CSLCHab][CSsRGB].get<C>() = Convert::From_LCHab_To_sRGB;
  t_[CSLCHab][CSsRGB].get<T>() = &srgb_;
  t_[CSLCHab][CSWideGamutRGB].get<C>() = Convert::From_LCHab_To_RGB;
  t_[CSLCHab][CSWideGamutRGB].get<T>() = &wide_;

  /* from Luv to all. */
  t_[CSLuv][CSXYZ].get<C>() = Convert::From_Luv_To_XYZ;
  t_[CSLuv][CSxyY].get<C>() = Convert::From_Luv_To_xyY;
  t_[CSLuv][CSLab].get<C>() = Convert::From_Luv_To_Lab;
  t_[CSLuv][CSLCHab].get<C>() = Convert::From_Luv_To_LCHab;
  t_[CSLuv][CSLCHuv].get<C>() = Convert::From_Luv_To_LCHuv;
  t_[CSLuv][CSAdobeRGB].get<C>() = Convert::From_Luv_To_RGB;
  t_[CSLuv][CSAdobeRGB].get<T>() = &adobe_;
  t_[CSLuv][CSAppleRGB].get<C>() = Convert::From_Luv_To_RGB;
  t_[CSLuv][CSAppleRGB].get<T>() = &apple_;
  t_[CSLuv][CSBestRGB].get<C>() = Convert::From_Luv_To_RGB;
  t_[CSLuv][CSBestRGB].get<T>() = &best_;
  t_[CSLuv][CSBetaRGB].get<C>() = Convert::From_Luv_To_RGB;
  t_[CSLuv][CSBetaRGB].get<T>() = &beta_;
  t_[CSLuv][CSBruceRGB].get<C>() = Convert::From_Luv_To_RGB;
  t_[CSLuv][CSBruceRGB].get<T>() = &bruce_;
  t_[CSLuv][CSCIERGB].get<C>() = Convert::From_Luv_To_RGB;
  t_[CSLuv][CSCIERGB].get<T>() = &cie_;
  t_[CSLuv][CSColorMatchRGB].get<C>() = Convert::From_Luv_To_RGB;
  t_[CSLuv][CSColorMatchRGB].get<T>() = &cm_;
  t_[CSLuv][CSDonRGB4].get<C>() = Convert::From_Luv_To_RGB;
  t_[CSLuv][CSDonRGB4].get<T>() = &don4_;
  t_[CSLuv][CSECIRGB].get<C>() = Convert::From_Luv_To_RGB;
  t_[CSLuv][CSECIRGB].get<T>() = &eci_;
  t_[CSLuv][CSEktaSpacePS5].get<C>() = Convert::From_Luv_To_RGB;
  t_[CSLuv][CSEktaSpacePS5].get<T>() = &ekps5_;
  t_[CSLuv][CSNTSCRGB].get<C>() = Convert::From_Luv_To_RGB;
  t_[CSLuv][CSNTSCRGB].get<T>() = &ntsc_;
  t_[CSLuv][CSPALSECAMRGB].get<C>() = Convert::From_Luv_To_RGB;
  t_[CSLuv][CSPALSECAMRGB].get<T>() = &pals_;
  t_[CSLuv][CSProPhotoRGB].get<C>() = Convert::From_Luv_To_RGB;
  t_[CSLuv][CSProPhotoRGB].get<T>() = &pro_;
  t_[CSLuv][CSSMPTECRGB].get<C>() = Convert::From_Luv_To_RGB;
  t_[CSLuv][CSSMPTECRGB].get<T>() = &smpt_;
  t_[CSLuv][CSsRGB].get<C>() = Convert::From_Luv_To_sRGB;
  t_[CSLuv][CSsRGB].get<T>() = &srgb_;
  t_[CSLuv][CSWideGamutRGB].get<C>() = Convert::From_Luv_To_RGB;
  t_[CSLuv][CSWideGamutRGB].get<T>() = &wide_;

  /* from LCHuv to all. */
  t_[CSLCHuv][CSXYZ].get<C>() = Convert::From_LCHuv_To_XYZ;
  t_[CSLCHuv][CSxyY].get<C>() = Convert::From_LCHuv_To_xyY;
  t_[CSLCHuv][CSLab].get<C>() = Convert::From_LCHuv_To_Lab;
  t_[CSLCHuv][CSLCHab].get<C>() = Convert::From_LCHuv_To_LCHab;
  t_[CSLCHuv][CSLuv].get<C>() = Convert::From_LCHuv_To_Luv;
  t_[CSLCHuv][CSAdobeRGB].get<C>() = Convert::From_LCHuv_To_RGB;
  t_[CSLCHuv][CSAdobeRGB].get<T>() = &adobe_;
  t_[CSLCHuv][CSAppleRGB].get<C>() = Convert::From_LCHuv_To_RGB;
  t_[CSLCHuv][CSAppleRGB].get<T>() = &apple_;
  t_[CSLCHuv][CSBestRGB].get<C>() = Convert::From_LCHuv_To_RGB;
  t_[CSLCHuv][CSBestRGB].get<T>() = &best_;
  t_[CSLCHuv][CSBetaRGB].get<C>() = Convert::From_LCHuv_To_RGB;
  t_[CSLCHuv][CSBetaRGB].get<T>() = &beta_;
  t_[CSLCHuv][CSBruceRGB].get<C>() = Convert::From_LCHuv_To_RGB;
  t_[CSLCHuv][CSBruceRGB].get<T>() = &bruce_;
  t_[CSLCHuv][CSCIERGB].get<C>() = Convert::From_LCHuv_To_RGB;
  t_[CSLCHuv][CSCIERGB].get<T>() = &cie_;
  t_[CSLCHuv][CSColorMatchRGB].get<C>() = Convert::From_LCHuv_To_RGB;
  t_[CSLCHuv][CSColorMatchRGB].get<T>() = &cm_;
  t_[CSLCHuv][CSDonRGB4].get<C>() = Convert::From_LCHuv_To_RGB;
  t_[CSLCHuv][CSDonRGB4].get<T>() = &don4_;
  t_[CSLCHuv][CSECIRGB].get<C>() = Convert::From_LCHuv_To_RGB;
  t_[CSLCHuv][CSECIRGB].get<T>() = &eci_;
  t_[CSLCHuv][CSEktaSpacePS5].get<C>() = Convert::From_LCHuv_To_RGB;
  t_[CSLCHuv][CSEktaSpacePS5].get<T>() = &ekps5_;
  t_[CSLCHuv][CSNTSCRGB].get<C>() = Convert::From_LCHuv_To_RGB;
  t_[CSLCHuv][CSNTSCRGB].get<T>() = &ntsc_;
  t_[CSLCHuv][CSPALSECAMRGB].get<C>() = Convert::From_LCHuv_To_RGB;
  t_[CSLCHuv][CSPALSECAMRGB].get<T>() = &pals_;
  t_[CSLCHuv][CSProPhotoRGB].get<C>() = Convert::From_LCHuv_To_RGB;
  t_[CSLCHuv][CSProPhotoRGB].get<T>() = &pro_;
  t_[CSLCHuv][CSSMPTECRGB].get<C>() = Convert::From_LCHuv_To_RGB;
  t_[CSLCHuv][CSSMPTECRGB].get<T>() = &smpt_;
  t_[CSLCHuv][CSsRGB].get<C>() = Convert::From_LCHuv_To_sRGB;
  t_[CSLCHuv][CSsRGB].get<T>() = &srgb_;
  t_[CSLCHuv][CSWideGamutRGB].get<C>() = Convert::From_LCHuv_To_RGB;
  t_[CSLCHuv][CSWideGamutRGB].get<T>() = &wide_;

  /* from Adobe to all. */
  t_[CSAdobeRGB][CSXYZ].get<C>() = Convert::From_RGB_To_XYZ;
  t_[CSAdobeRGB][CSXYZ].get<F>() = &adobe_;
  t_[CSAdobeRGB][CSxyY].get<C>() = Convert::From_RGB_To_xyY;
  t_[CSAdobeRGB][CSxyY].get<F>() = &adobe_;
  t_[CSAdobeRGB][CSLab].get<C>() = Convert::From_RGB_To_Lab;
  t_[CSAdobeRGB][CSLab].get<F>() = &adobe_;
  t_[CSAdobeRGB][CSLCHab].get<C>() = Convert::From_RGB_To_LCHab;
  t_[CSAdobeRGB][CSLCHab].get<F>() = &adobe_;
  t_[CSAdobeRGB][CSLuv].get<C>() = Convert::From_RGB_To_Luv;
  t_[CSAdobeRGB][CSLuv].get<F>() = &adobe_;
  t_[CSAdobeRGB][CSLCHuv].get<C>() = Convert::From_RGB_To_LCHuv;
  t_[CSAdobeRGB][CSLCHuv].get<F>() = &adobe_;
  t_[CSAdobeRGB][CSAppleRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSAdobeRGB][CSAppleRGB].get<F>() = &adobe_;
  t_[CSAdobeRGB][CSAppleRGB].get<T>() = &apple_;
  t_[CSAdobeRGB][CSBestRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSAdobeRGB][CSBestRGB].get<F>() = &adobe_;
  t_[CSAdobeRGB][CSBestRGB].get<T>() = &best_;
  t_[CSAdobeRGB][CSBetaRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSAdobeRGB][CSBetaRGB].get<F>() = &adobe_;
  t_[CSAdobeRGB][CSBetaRGB].get<T>() = &beta_;
  t_[CSAdobeRGB][CSBruceRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSAdobeRGB][CSBruceRGB].get<F>() = &adobe_;
  t_[CSAdobeRGB][CSBruceRGB].get<T>() = &bruce_;
  t_[CSAdobeRGB][CSCIERGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSAdobeRGB][CSCIERGB].get<F>() = &adobe_;
  t_[CSAdobeRGB][CSCIERGB].get<T>() = &cie_;
  t_[CSAdobeRGB][CSColorMatchRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSAdobeRGB][CSColorMatchRGB].get<F>() = &adobe_;
  t_[CSAdobeRGB][CSColorMatchRGB].get<T>() = &cm_;
  t_[CSAdobeRGB][CSDonRGB4].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSAdobeRGB][CSDonRGB4].get<F>() = &adobe_;
  t_[CSAdobeRGB][CSDonRGB4].get<T>() = &don4_;
  t_[CSAdobeRGB][CSECIRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSAdobeRGB][CSECIRGB].get<F>() = &adobe_;
  t_[CSAdobeRGB][CSECIRGB].get<T>() = &eci_;
  t_[CSAdobeRGB][CSEktaSpacePS5].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSAdobeRGB][CSEktaSpacePS5].get<F>() = &adobe_;
  t_[CSAdobeRGB][CSEktaSpacePS5].get<T>() = &ekps5_;
  t_[CSAdobeRGB][CSNTSCRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSAdobeRGB][CSNTSCRGB].get<F>() = &adobe_;
  t_[CSAdobeRGB][CSNTSCRGB].get<T>() = &ntsc_;
  t_[CSAdobeRGB][CSPALSECAMRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSAdobeRGB][CSPALSECAMRGB].get<F>() = &adobe_;
  t_[CSAdobeRGB][CSPALSECAMRGB].get<T>() = &pals_;
  t_[CSAdobeRGB][CSProPhotoRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSAdobeRGB][CSProPhotoRGB].get<F>() = &adobe_;
  t_[CSAdobeRGB][CSProPhotoRGB].get<T>() = &pro_;
  t_[CSAdobeRGB][CSSMPTECRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSAdobeRGB][CSSMPTECRGB].get<F>() = &adobe_;
  t_[CSAdobeRGB][CSSMPTECRGB].get<T>() = &smpt_;
  t_[CSAdobeRGB][CSsRGB].get<C>() = Convert::From_RGB_To_sRGB;
  t_[CSAdobeRGB][CSsRGB].get<F>() = &adobe_;
  t_[CSAdobeRGB][CSsRGB].get<T>() = &srgb_;
  t_[CSAdobeRGB][CSWideGamutRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSAdobeRGB][CSWideGamutRGB].get<F>() = &adobe_;
  t_[CSAdobeRGB][CSWideGamutRGB].get<T>() = &wide_;

  /* from Apple to all. */
  t_[CSAppleRGB][CSXYZ].get<C>() = Convert::From_RGB_To_XYZ;
  t_[CSAppleRGB][CSXYZ].get<F>() = &apple_;
  t_[CSAppleRGB][CSxyY].get<C>() = Convert::From_RGB_To_xyY;
  t_[CSAppleRGB][CSxyY].get<F>() = &apple_;
  t_[CSAppleRGB][CSLab].get<C>() = Convert::From_RGB_To_Lab;
  t_[CSAppleRGB][CSLab].get<F>() = &apple_;
  t_[CSAppleRGB][CSLCHab].get<C>() = Convert::From_RGB_To_LCHab;
  t_[CSAppleRGB][CSLCHab].get<F>() = &apple_;
  t_[CSAppleRGB][CSLuv].get<C>() = Convert::From_RGB_To_Luv;
  t_[CSAppleRGB][CSLuv].get<F>() = &apple_;
  t_[CSAppleRGB][CSLCHuv].get<C>() = Convert::From_RGB_To_LCHuv;
  t_[CSAppleRGB][CSLCHuv].get<F>() = &apple_;
  t_[CSAppleRGB][CSAdobeRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSAppleRGB][CSAdobeRGB].get<F>() = &apple_;
  t_[CSAppleRGB][CSAdobeRGB].get<T>() = &adobe_;
  t_[CSAppleRGB][CSBestRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSAppleRGB][CSBestRGB].get<F>() = &apple_;
  t_[CSAppleRGB][CSBestRGB].get<T>() = &best_;
  t_[CSAppleRGB][CSBetaRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSAppleRGB][CSBetaRGB].get<F>() = &apple_;
  t_[CSAppleRGB][CSBetaRGB].get<T>() = &beta_;
  t_[CSAppleRGB][CSBruceRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSAppleRGB][CSBruceRGB].get<F>() = &apple_;
  t_[CSAppleRGB][CSBruceRGB].get<T>() = &bruce_;
  t_[CSAppleRGB][CSCIERGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSAppleRGB][CSCIERGB].get<F>() = &apple_;
  t_[CSAppleRGB][CSCIERGB].get<T>() = &cie_;
  t_[CSAppleRGB][CSColorMatchRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSAppleRGB][CSColorMatchRGB].get<F>() = &apple_;
  t_[CSAppleRGB][CSColorMatchRGB].get<T>() = &cm_;
  t_[CSAppleRGB][CSDonRGB4].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSAppleRGB][CSDonRGB4].get<F>() = &apple_;
  t_[CSAppleRGB][CSDonRGB4].get<T>() = &don4_;
  t_[CSAppleRGB][CSECIRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSAppleRGB][CSECIRGB].get<F>() = &apple_;
  t_[CSAppleRGB][CSECIRGB].get<T>() = &eci_;
  t_[CSAppleRGB][CSEktaSpacePS5].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSAppleRGB][CSEktaSpacePS5].get<F>() = &apple_;
  t_[CSAppleRGB][CSEktaSpacePS5].get<T>() = &ekps5_;
  t_[CSAppleRGB][CSNTSCRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSAppleRGB][CSNTSCRGB].get<F>() = &apple_;
  t_[CSAppleRGB][CSNTSCRGB].get<T>() = &ntsc_;
  t_[CSAppleRGB][CSPALSECAMRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSAppleRGB][CSPALSECAMRGB].get<F>() = &apple_;
  t_[CSAppleRGB][CSPALSECAMRGB].get<T>() = &pals_;
  t_[CSAppleRGB][CSProPhotoRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSAppleRGB][CSProPhotoRGB].get<F>() = &apple_;
  t_[CSAppleRGB][CSProPhotoRGB].get<T>() = &pro_;
  t_[CSAppleRGB][CSSMPTECRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSAppleRGB][CSSMPTECRGB].get<F>() = &apple_;
  t_[CSAppleRGB][CSSMPTECRGB].get<T>() = &smpt_;
  t_[CSAppleRGB][CSsRGB].get<C>() = Convert::From_RGB_To_sRGB;
  t_[CSAppleRGB][CSsRGB].get<F>() = &apple_;
  t_[CSAppleRGB][CSsRGB].get<T>() = &srgb_;
  t_[CSAppleRGB][CSWideGamutRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSAppleRGB][CSWideGamutRGB].get<F>() = &apple_;
  t_[CSAppleRGB][CSWideGamutRGB].get<T>() = &wide_;

  /* from Best to all. */
  t_[CSBestRGB][CSXYZ].get<C>() = Convert::From_RGB_To_XYZ;
  t_[CSBestRGB][CSXYZ].get<F>() = &best_;
  t_[CSBestRGB][CSxyY].get<C>() = Convert::From_RGB_To_xyY;
  t_[CSBestRGB][CSxyY].get<F>() = &best_;
  t_[CSBestRGB][CSLab].get<C>() = Convert::From_RGB_To_Lab;
  t_[CSBestRGB][CSLab].get<F>() = &best_;
  t_[CSBestRGB][CSLCHab].get<C>() = Convert::From_RGB_To_LCHab;
  t_[CSBestRGB][CSLCHab].get<F>() = &best_;
  t_[CSBestRGB][CSLuv].get<C>() = Convert::From_RGB_To_Luv;
  t_[CSBestRGB][CSLuv].get<F>() = &best_;
  t_[CSBestRGB][CSLCHuv].get<C>() = Convert::From_RGB_To_LCHuv;
  t_[CSBestRGB][CSLCHuv].get<F>() = &best_;
  t_[CSBestRGB][CSAdobeRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSBestRGB][CSAdobeRGB].get<F>() = &best_;
  t_[CSBestRGB][CSAdobeRGB].get<T>() = &adobe_;
  t_[CSBestRGB][CSAppleRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSBestRGB][CSAppleRGB].get<F>() = &best_;
  t_[CSBestRGB][CSAppleRGB].get<T>() = &apple_;
  t_[CSBestRGB][CSBetaRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSBestRGB][CSBetaRGB].get<F>() = &best_;
  t_[CSBestRGB][CSBetaRGB].get<T>() = &beta_;
  t_[CSBestRGB][CSBruceRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSBestRGB][CSBruceRGB].get<F>() = &best_;
  t_[CSBestRGB][CSBruceRGB].get<T>() = &bruce_;
  t_[CSBestRGB][CSCIERGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSBestRGB][CSCIERGB].get<F>() = &best_;
  t_[CSBestRGB][CSCIERGB].get<T>() = &cie_;
  t_[CSBestRGB][CSColorMatchRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSBestRGB][CSColorMatchRGB].get<F>() = &best_;
  t_[CSBestRGB][CSColorMatchRGB].get<T>() = &cm_;
  t_[CSBestRGB][CSDonRGB4].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSBestRGB][CSDonRGB4].get<F>() = &best_;
  t_[CSBestRGB][CSDonRGB4].get<T>() = &don4_;
  t_[CSBestRGB][CSECIRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSBestRGB][CSECIRGB].get<F>() = &best_;
  t_[CSBestRGB][CSECIRGB].get<T>() = &eci_;
  t_[CSBestRGB][CSEktaSpacePS5].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSBestRGB][CSEktaSpacePS5].get<F>() = &best_;
  t_[CSBestRGB][CSEktaSpacePS5].get<T>() = &ekps5_;
  t_[CSBestRGB][CSNTSCRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSBestRGB][CSNTSCRGB].get<F>() = &best_;
  t_[CSBestRGB][CSNTSCRGB].get<T>() = &ntsc_;
  t_[CSBestRGB][CSPALSECAMRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSBestRGB][CSPALSECAMRGB].get<F>() = &best_;
  t_[CSBestRGB][CSPALSECAMRGB].get<T>() = &pals_;
  t_[CSBestRGB][CSProPhotoRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSBestRGB][CSProPhotoRGB].get<F>() = &best_;
  t_[CSBestRGB][CSProPhotoRGB].get<T>() = &pro_;
  t_[CSBestRGB][CSSMPTECRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSBestRGB][CSSMPTECRGB].get<F>() = &best_;
  t_[CSBestRGB][CSSMPTECRGB].get<T>() = &smpt_;
  t_[CSBestRGB][CSsRGB].get<C>() = Convert::From_RGB_To_sRGB;
  t_[CSBestRGB][CSsRGB].get<F>() = &best_;
  t_[CSBestRGB][CSsRGB].get<T>() = &srgb_;
  t_[CSBestRGB][CSWideGamutRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSBestRGB][CSWideGamutRGB].get<F>() = &best_;
  t_[CSBestRGB][CSWideGamutRGB].get<T>() = &wide_;

  /* from Beta to all. */
  t_[CSBetaRGB][CSXYZ].get<C>() = Convert::From_RGB_To_XYZ;
  t_[CSBetaRGB][CSXYZ].get<F>() = &beta_;
  t_[CSBetaRGB][CSxyY].get<C>() = Convert::From_RGB_To_xyY;
  t_[CSBetaRGB][CSxyY].get<F>() = &beta_;
  t_[CSBetaRGB][CSLab].get<C>() = Convert::From_RGB_To_Lab;
  t_[CSBetaRGB][CSLab].get<F>() = &beta_;
  t_[CSBetaRGB][CSLCHab].get<C>() = Convert::From_RGB_To_LCHab;
  t_[CSBetaRGB][CSLCHab].get<F>() = &beta_;
  t_[CSBetaRGB][CSLuv].get<C>() = Convert::From_RGB_To_Luv;
  t_[CSBetaRGB][CSLuv].get<F>() = &beta_;
  t_[CSBetaRGB][CSLCHuv].get<C>() = Convert::From_RGB_To_LCHuv;
  t_[CSBetaRGB][CSLCHuv].get<F>() = &beta_;
  t_[CSBetaRGB][CSAdobeRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSBetaRGB][CSAdobeRGB].get<F>() = &beta_;
  t_[CSBetaRGB][CSAdobeRGB].get<T>() = &adobe_;
  t_[CSBetaRGB][CSAppleRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSBetaRGB][CSAppleRGB].get<F>() = &beta_;
  t_[CSBetaRGB][CSAppleRGB].get<T>() = &apple_;
  t_[CSBetaRGB][CSBestRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSBetaRGB][CSBestRGB].get<F>() = &beta_;
  t_[CSBetaRGB][CSBestRGB].get<T>() = &best_;
  t_[CSBetaRGB][CSBruceRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSBetaRGB][CSBruceRGB].get<F>() = &beta_;
  t_[CSBetaRGB][CSBruceRGB].get<T>() = &bruce_;
  t_[CSBetaRGB][CSCIERGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSBetaRGB][CSCIERGB].get<F>() = &beta_;
  t_[CSBetaRGB][CSCIERGB].get<T>() = &cie_;
  t_[CSBetaRGB][CSColorMatchRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSBetaRGB][CSColorMatchRGB].get<F>() = &beta_;
  t_[CSBetaRGB][CSColorMatchRGB].get<T>() = &cm_;
  t_[CSBetaRGB][CSDonRGB4].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSBetaRGB][CSDonRGB4].get<F>() = &beta_;
  t_[CSBetaRGB][CSDonRGB4].get<T>() = &don4_;
  t_[CSBetaRGB][CSECIRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSBetaRGB][CSECIRGB].get<F>() = &beta_;
  t_[CSBetaRGB][CSECIRGB].get<T>() = &eci_;
  t_[CSBetaRGB][CSEktaSpacePS5].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSBetaRGB][CSEktaSpacePS5].get<F>() = &beta_;
  t_[CSBetaRGB][CSEktaSpacePS5].get<T>() = &ekps5_;
  t_[CSBetaRGB][CSNTSCRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSBetaRGB][CSNTSCRGB].get<F>() = &beta_;
  t_[CSBetaRGB][CSNTSCRGB].get<T>() = &ntsc_;
  t_[CSBetaRGB][CSPALSECAMRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSBetaRGB][CSPALSECAMRGB].get<F>() = &beta_;
  t_[CSBetaRGB][CSPALSECAMRGB].get<T>() = &pals_;
  t_[CSBetaRGB][CSProPhotoRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSBetaRGB][CSProPhotoRGB].get<F>() = &beta_;
  t_[CSBetaRGB][CSProPhotoRGB].get<T>() = &pro_;
  t_[CSBetaRGB][CSSMPTECRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSBetaRGB][CSSMPTECRGB].get<F>() = &beta_;
  t_[CSBetaRGB][CSSMPTECRGB].get<T>() = &smpt_;
  t_[CSBetaRGB][CSsRGB].get<C>() = Convert::From_RGB_To_sRGB;
  t_[CSBetaRGB][CSsRGB].get<F>() = &beta_;
  t_[CSBetaRGB][CSsRGB].get<T>() = &srgb_;
  t_[CSBetaRGB][CSWideGamutRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSBetaRGB][CSWideGamutRGB].get<F>() = &beta_;
  t_[CSBetaRGB][CSWideGamutRGB].get<T>() = &wide_;

  /* from Bruce to all. */
  t_[CSBruceRGB][CSXYZ].get<C>() = Convert::From_RGB_To_XYZ;
  t_[CSBruceRGB][CSXYZ].get<F>() = &bruce_;
  t_[CSBruceRGB][CSxyY].get<C>() = Convert::From_RGB_To_xyY;
  t_[CSBruceRGB][CSxyY].get<F>() = &bruce_;
  t_[CSBruceRGB][CSLab].get<C>() = Convert::From_RGB_To_Lab;
  t_[CSBruceRGB][CSLab].get<F>() = &bruce_;
  t_[CSBruceRGB][CSLCHab].get<C>() = Convert::From_RGB_To_LCHab;
  t_[CSBruceRGB][CSLCHab].get<F>() = &bruce_;
  t_[CSBruceRGB][CSLuv].get<C>() = Convert::From_RGB_To_Luv;
  t_[CSBruceRGB][CSLuv].get<F>() = &bruce_;
  t_[CSBruceRGB][CSLCHuv].get<C>() = Convert::From_RGB_To_LCHuv;
  t_[CSBruceRGB][CSLCHuv].get<F>() = &bruce_;
  t_[CSBruceRGB][CSAdobeRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSBruceRGB][CSAdobeRGB].get<F>() = &bruce_;
  t_[CSBruceRGB][CSAdobeRGB].get<T>() = &adobe_;
  t_[CSBruceRGB][CSAppleRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSBruceRGB][CSAppleRGB].get<F>() = &bruce_;
  t_[CSBruceRGB][CSAppleRGB].get<T>() = &apple_;
  t_[CSBruceRGB][CSBestRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSBruceRGB][CSBestRGB].get<F>() = &bruce_;
  t_[CSBruceRGB][CSBestRGB].get<T>() = &best_;
  t_[CSBruceRGB][CSBetaRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSBruceRGB][CSBetaRGB].get<F>() = &bruce_;
  t_[CSBruceRGB][CSBetaRGB].get<T>() = &beta_;
  t_[CSBruceRGB][CSCIERGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSBruceRGB][CSCIERGB].get<F>() = &bruce_;
  t_[CSBruceRGB][CSCIERGB].get<T>() = &cie_;
  t_[CSBruceRGB][CSColorMatchRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSBruceRGB][CSColorMatchRGB].get<F>() = &bruce_;
  t_[CSBruceRGB][CSColorMatchRGB].get<T>() = &cm_;
  t_[CSBruceRGB][CSDonRGB4].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSBruceRGB][CSDonRGB4].get<F>() = &bruce_;
  t_[CSBruceRGB][CSDonRGB4].get<T>() = &don4_;
  t_[CSBruceRGB][CSECIRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSBruceRGB][CSECIRGB].get<F>() = &bruce_;
  t_[CSBruceRGB][CSECIRGB].get<T>() = &eci_;
  t_[CSBruceRGB][CSEktaSpacePS5].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSBruceRGB][CSEktaSpacePS5].get<F>() = &bruce_;
  t_[CSBruceRGB][CSEktaSpacePS5].get<T>() = &ekps5_;
  t_[CSBruceRGB][CSNTSCRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSBruceRGB][CSNTSCRGB].get<F>() = &bruce_;
  t_[CSBruceRGB][CSNTSCRGB].get<T>() = &ntsc_;
  t_[CSBruceRGB][CSPALSECAMRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSBruceRGB][CSPALSECAMRGB].get<F>() = &bruce_;
  t_[CSBruceRGB][CSPALSECAMRGB].get<T>() = &pals_;
  t_[CSBruceRGB][CSProPhotoRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSBruceRGB][CSProPhotoRGB].get<F>() = &bruce_;
  t_[CSBruceRGB][CSProPhotoRGB].get<T>() = &pro_;
  t_[CSBruceRGB][CSSMPTECRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSBruceRGB][CSSMPTECRGB].get<F>() = &bruce_;
  t_[CSBruceRGB][CSSMPTECRGB].get<T>() = &smpt_;
  t_[CSBruceRGB][CSsRGB].get<C>() = Convert::From_RGB_To_sRGB;
  t_[CSBruceRGB][CSsRGB].get<F>() = &bruce_;
  t_[CSBruceRGB][CSsRGB].get<T>() = &srgb_;
  t_[CSBruceRGB][CSWideGamutRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSBruceRGB][CSWideGamutRGB].get<F>() = &bruce_;
  t_[CSBruceRGB][CSWideGamutRGB].get<T>() = &wide_;

  /* from CIE to all. */
  t_[CSCIERGB][CSXYZ].get<C>() = Convert::From_RGB_To_XYZ;
  t_[CSCIERGB][CSXYZ].get<F>() = &cie_;
  t_[CSCIERGB][CSxyY].get<C>() = Convert::From_RGB_To_xyY;
  t_[CSCIERGB][CSxyY].get<F>() = &cie_;
  t_[CSCIERGB][CSLab].get<C>() = Convert::From_RGB_To_Lab;
  t_[CSCIERGB][CSLab].get<F>() = &cie_;
  t_[CSCIERGB][CSLCHab].get<C>() = Convert::From_RGB_To_LCHab;
  t_[CSCIERGB][CSLCHab].get<F>() = &cie_;
  t_[CSCIERGB][CSLuv].get<C>() = Convert::From_RGB_To_Luv;
  t_[CSCIERGB][CSLuv].get<F>() = &cie_;
  t_[CSCIERGB][CSLCHuv].get<C>() = Convert::From_RGB_To_LCHuv;
  t_[CSCIERGB][CSLCHuv].get<F>() = &cie_;
  t_[CSCIERGB][CSAdobeRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSCIERGB][CSAdobeRGB].get<F>() = &cie_;
  t_[CSCIERGB][CSAdobeRGB].get<T>() = &adobe_;
  t_[CSCIERGB][CSAppleRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSCIERGB][CSAppleRGB].get<F>() = &cie_;
  t_[CSCIERGB][CSAppleRGB].get<T>() = &apple_;
  t_[CSCIERGB][CSBestRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSCIERGB][CSBestRGB].get<F>() = &cie_;
  t_[CSCIERGB][CSBestRGB].get<T>() = &best_;
  t_[CSCIERGB][CSBetaRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSCIERGB][CSBetaRGB].get<F>() = &cie_;
  t_[CSCIERGB][CSBetaRGB].get<T>() = &beta_;
  t_[CSCIERGB][CSBruceRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSCIERGB][CSBruceRGB].get<F>() = &cie_;
  t_[CSCIERGB][CSBruceRGB].get<T>() = &bruce_;
  t_[CSCIERGB][CSColorMatchRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSCIERGB][CSColorMatchRGB].get<F>() = &cie_;
  t_[CSCIERGB][CSColorMatchRGB].get<T>() = &cm_;
  t_[CSCIERGB][CSDonRGB4].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSCIERGB][CSDonRGB4].get<F>() = &cie_;
  t_[CSCIERGB][CSDonRGB4].get<T>() = &don4_;
  t_[CSCIERGB][CSECIRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSCIERGB][CSECIRGB].get<F>() = &cie_;
  t_[CSCIERGB][CSECIRGB].get<T>() = &eci_;
  t_[CSCIERGB][CSEktaSpacePS5].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSCIERGB][CSEktaSpacePS5].get<F>() = &cie_;
  t_[CSCIERGB][CSEktaSpacePS5].get<T>() = &ekps5_;
  t_[CSCIERGB][CSNTSCRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSCIERGB][CSNTSCRGB].get<F>() = &cie_;
  t_[CSCIERGB][CSNTSCRGB].get<T>() = &ntsc_;
  t_[CSCIERGB][CSPALSECAMRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSCIERGB][CSPALSECAMRGB].get<F>() = &cie_;
  t_[CSCIERGB][CSPALSECAMRGB].get<T>() = &pals_;
  t_[CSCIERGB][CSProPhotoRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSCIERGB][CSProPhotoRGB].get<F>() = &cie_;
  t_[CSCIERGB][CSProPhotoRGB].get<T>() = &pro_;
  t_[CSCIERGB][CSSMPTECRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSCIERGB][CSSMPTECRGB].get<F>() = &cie_;
  t_[CSCIERGB][CSSMPTECRGB].get<T>() = &smpt_;
  t_[CSCIERGB][CSsRGB].get<C>() = Convert::From_RGB_To_sRGB;
  t_[CSCIERGB][CSsRGB].get<F>() = &cie_;
  t_[CSCIERGB][CSsRGB].get<T>() = &srgb_;
  t_[CSCIERGB][CSWideGamutRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSCIERGB][CSWideGamutRGB].get<F>() = &cie_;
  t_[CSCIERGB][CSWideGamutRGB].get<T>() = &wide_;

  /* from ColorMatch to all. */
  t_[CSColorMatchRGB][CSXYZ].get<C>() = Convert::From_RGB_To_XYZ;
  t_[CSColorMatchRGB][CSXYZ].get<F>() = &cm_;
  t_[CSColorMatchRGB][CSxyY].get<C>() = Convert::From_RGB_To_xyY;
  t_[CSColorMatchRGB][CSxyY].get<F>() = &cm_;
  t_[CSColorMatchRGB][CSLab].get<C>() = Convert::From_RGB_To_Lab;
  t_[CSColorMatchRGB][CSLab].get<F>() = &cm_;
  t_[CSColorMatchRGB][CSLCHab].get<C>() = Convert::From_RGB_To_LCHab;
  t_[CSColorMatchRGB][CSLCHab].get<F>() = &cm_;
  t_[CSColorMatchRGB][CSLuv].get<C>() = Convert::From_RGB_To_Luv;
  t_[CSColorMatchRGB][CSLuv].get<F>() = &cm_;
  t_[CSColorMatchRGB][CSLCHuv].get<C>() = Convert::From_RGB_To_LCHuv;
  t_[CSColorMatchRGB][CSLCHuv].get<F>() = &cm_;
  t_[CSColorMatchRGB][CSAdobeRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSColorMatchRGB][CSAdobeRGB].get<F>() = &cm_;
  t_[CSColorMatchRGB][CSAdobeRGB].get<T>() = &adobe_;
  t_[CSColorMatchRGB][CSAppleRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSColorMatchRGB][CSAppleRGB].get<F>() = &cm_;
  t_[CSColorMatchRGB][CSAppleRGB].get<T>() = &apple_;
  t_[CSColorMatchRGB][CSBestRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSColorMatchRGB][CSBestRGB].get<F>() = &cm_;
  t_[CSColorMatchRGB][CSBestRGB].get<T>() = &best_;
  t_[CSColorMatchRGB][CSBetaRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSColorMatchRGB][CSBetaRGB].get<F>() = &cm_;
  t_[CSColorMatchRGB][CSBetaRGB].get<T>() = &beta_;
  t_[CSColorMatchRGB][CSBruceRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSColorMatchRGB][CSBruceRGB].get<F>() = &cm_;
  t_[CSColorMatchRGB][CSBruceRGB].get<T>() = &bruce_;
  t_[CSColorMatchRGB][CSCIERGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSColorMatchRGB][CSCIERGB].get<F>() = &cm_;
  t_[CSColorMatchRGB][CSCIERGB].get<T>() = &cie_;
  t_[CSColorMatchRGB][CSDonRGB4].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSColorMatchRGB][CSDonRGB4].get<F>() = &cm_;
  t_[CSColorMatchRGB][CSDonRGB4].get<T>() = &don4_;
  t_[CSColorMatchRGB][CSECIRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSColorMatchRGB][CSECIRGB].get<F>() = &cm_;
  t_[CSColorMatchRGB][CSECIRGB].get<T>() = &eci_;
  t_[CSColorMatchRGB][CSEktaSpacePS5].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSColorMatchRGB][CSEktaSpacePS5].get<F>() = &cm_;
  t_[CSColorMatchRGB][CSEktaSpacePS5].get<T>() = &ekps5_;
  t_[CSColorMatchRGB][CSNTSCRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSColorMatchRGB][CSNTSCRGB].get<F>() = &cm_;
  t_[CSColorMatchRGB][CSNTSCRGB].get<T>() = &ntsc_;
  t_[CSColorMatchRGB][CSPALSECAMRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSColorMatchRGB][CSPALSECAMRGB].get<F>() = &cm_;
  t_[CSColorMatchRGB][CSPALSECAMRGB].get<T>() = &pals_;
  t_[CSColorMatchRGB][CSProPhotoRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSColorMatchRGB][CSProPhotoRGB].get<F>() = &cm_;
  t_[CSColorMatchRGB][CSProPhotoRGB].get<T>() = &pro_;
  t_[CSColorMatchRGB][CSSMPTECRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSColorMatchRGB][CSSMPTECRGB].get<F>() = &cm_;
  t_[CSColorMatchRGB][CSSMPTECRGB].get<T>() = &smpt_;
  t_[CSColorMatchRGB][CSsRGB].get<C>() = Convert::From_RGB_To_sRGB;
  t_[CSColorMatchRGB][CSsRGB].get<F>() = &cm_;
  t_[CSColorMatchRGB][CSsRGB].get<T>() = &srgb_;
  t_[CSColorMatchRGB][CSWideGamutRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSColorMatchRGB][CSWideGamutRGB].get<F>() = &cm_;
  t_[CSColorMatchRGB][CSWideGamutRGB].get<T>() = &wide_;

  /* from Don4 to all. */
  t_[CSDonRGB4][CSXYZ].get<C>() = Convert::From_RGB_To_XYZ;
  t_[CSDonRGB4][CSXYZ].get<F>() = &don4_;
  t_[CSDonRGB4][CSxyY].get<C>() = Convert::From_RGB_To_xyY;
  t_[CSDonRGB4][CSxyY].get<F>() = &don4_;
  t_[CSDonRGB4][CSLab].get<C>() = Convert::From_RGB_To_Lab;
  t_[CSDonRGB4][CSLab].get<F>() = &don4_;
  t_[CSDonRGB4][CSLCHab].get<C>() = Convert::From_RGB_To_LCHab;
  t_[CSDonRGB4][CSLCHab].get<F>() = &don4_;
  t_[CSDonRGB4][CSLuv].get<C>() = Convert::From_RGB_To_Luv;
  t_[CSDonRGB4][CSLuv].get<F>() = &don4_;
  t_[CSDonRGB4][CSLCHuv].get<C>() = Convert::From_RGB_To_LCHuv;
  t_[CSDonRGB4][CSLCHuv].get<F>() = &don4_;
  t_[CSDonRGB4][CSAdobeRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSDonRGB4][CSAdobeRGB].get<F>() = &don4_;
  t_[CSDonRGB4][CSAdobeRGB].get<T>() = &adobe_;
  t_[CSDonRGB4][CSAppleRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSDonRGB4][CSAppleRGB].get<F>() = &don4_;
  t_[CSDonRGB4][CSAppleRGB].get<T>() = &apple_;
  t_[CSDonRGB4][CSBestRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSDonRGB4][CSBestRGB].get<F>() = &don4_;
  t_[CSDonRGB4][CSBestRGB].get<T>() = &best_;
  t_[CSDonRGB4][CSBetaRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSDonRGB4][CSBetaRGB].get<F>() = &don4_;
  t_[CSDonRGB4][CSBetaRGB].get<T>() = &beta_;
  t_[CSDonRGB4][CSBruceRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSDonRGB4][CSBruceRGB].get<F>() = &don4_;
  t_[CSDonRGB4][CSBruceRGB].get<T>() = &bruce_;
  t_[CSDonRGB4][CSCIERGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSDonRGB4][CSCIERGB].get<F>() = &don4_;
  t_[CSDonRGB4][CSCIERGB].get<T>() = &cie_;
  t_[CSDonRGB4][CSColorMatchRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSDonRGB4][CSColorMatchRGB].get<F>() = &don4_;
  t_[CSDonRGB4][CSColorMatchRGB].get<T>() = &cm_;
  t_[CSDonRGB4][CSECIRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSDonRGB4][CSECIRGB].get<F>() = &don4_;
  t_[CSDonRGB4][CSECIRGB].get<T>() = &eci_;
  t_[CSDonRGB4][CSEktaSpacePS5].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSDonRGB4][CSEktaSpacePS5].get<F>() = &don4_;
  t_[CSDonRGB4][CSEktaSpacePS5].get<T>() = &ekps5_;
  t_[CSDonRGB4][CSNTSCRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSDonRGB4][CSNTSCRGB].get<F>() = &don4_;
  t_[CSDonRGB4][CSNTSCRGB].get<T>() = &ntsc_;
  t_[CSDonRGB4][CSPALSECAMRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSDonRGB4][CSPALSECAMRGB].get<F>() = &don4_;
  t_[CSDonRGB4][CSPALSECAMRGB].get<T>() = &pals_;
  t_[CSDonRGB4][CSProPhotoRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSDonRGB4][CSProPhotoRGB].get<F>() = &don4_;
  t_[CSDonRGB4][CSProPhotoRGB].get<T>() = &pro_;
  t_[CSDonRGB4][CSSMPTECRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSDonRGB4][CSSMPTECRGB].get<F>() = &don4_;
  t_[CSDonRGB4][CSSMPTECRGB].get<T>() = &smpt_;
  t_[CSDonRGB4][CSsRGB].get<C>() = Convert::From_RGB_To_sRGB;
  t_[CSDonRGB4][CSsRGB].get<F>() = &don4_;
  t_[CSDonRGB4][CSsRGB].get<T>() = &srgb_;
  t_[CSDonRGB4][CSWideGamutRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSDonRGB4][CSWideGamutRGB].get<F>() = &don4_;
  t_[CSDonRGB4][CSWideGamutRGB].get<T>() = &wide_;

  /* from ECI to all. */
  t_[CSECIRGB][CSXYZ].get<C>() = Convert::From_RGB_To_XYZ;
  t_[CSECIRGB][CSXYZ].get<F>() = &eci_;
  t_[CSECIRGB][CSxyY].get<C>() = Convert::From_RGB_To_xyY;
  t_[CSECIRGB][CSxyY].get<F>() = &eci_;
  t_[CSECIRGB][CSLab].get<C>() = Convert::From_RGB_To_Lab;
  t_[CSECIRGB][CSLab].get<F>() = &eci_;
  t_[CSECIRGB][CSLCHab].get<C>() = Convert::From_RGB_To_LCHab;
  t_[CSECIRGB][CSLCHab].get<F>() = &eci_;
  t_[CSECIRGB][CSLuv].get<C>() = Convert::From_RGB_To_Luv;
  t_[CSECIRGB][CSLuv].get<F>() = &eci_;
  t_[CSECIRGB][CSLCHuv].get<C>() = Convert::From_RGB_To_LCHuv;
  t_[CSECIRGB][CSLCHuv].get<F>() = &eci_;
  t_[CSECIRGB][CSAdobeRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSECIRGB][CSAdobeRGB].get<F>() = &eci_;
  t_[CSECIRGB][CSAdobeRGB].get<T>() = &adobe_;
  t_[CSECIRGB][CSAppleRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSECIRGB][CSAppleRGB].get<F>() = &eci_;
  t_[CSECIRGB][CSAppleRGB].get<T>() = &apple_;
  t_[CSECIRGB][CSBestRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSECIRGB][CSBestRGB].get<F>() = &eci_;
  t_[CSECIRGB][CSBestRGB].get<T>() = &best_;
  t_[CSECIRGB][CSBetaRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSECIRGB][CSBetaRGB].get<F>() = &eci_;
  t_[CSECIRGB][CSBetaRGB].get<T>() = &beta_;
  t_[CSECIRGB][CSBruceRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSECIRGB][CSBruceRGB].get<F>() = &eci_;
  t_[CSECIRGB][CSBruceRGB].get<T>() = &bruce_;
  t_[CSECIRGB][CSCIERGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSECIRGB][CSCIERGB].get<F>() = &eci_;
  t_[CSECIRGB][CSCIERGB].get<T>() = &cie_;
  t_[CSECIRGB][CSColorMatchRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSECIRGB][CSColorMatchRGB].get<F>() = &eci_;
  t_[CSECIRGB][CSColorMatchRGB].get<T>() = &cm_;
  t_[CSECIRGB][CSDonRGB4].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSECIRGB][CSDonRGB4].get<F>() = &eci_;
  t_[CSECIRGB][CSDonRGB4].get<T>() = &don4_;
  t_[CSECIRGB][CSEktaSpacePS5].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSECIRGB][CSEktaSpacePS5].get<F>() = &eci_;
  t_[CSECIRGB][CSEktaSpacePS5].get<T>() = &ekps5_;
  t_[CSECIRGB][CSNTSCRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSECIRGB][CSNTSCRGB].get<F>() = &eci_;
  t_[CSECIRGB][CSNTSCRGB].get<T>() = &ntsc_;
  t_[CSECIRGB][CSPALSECAMRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSECIRGB][CSPALSECAMRGB].get<F>() = &eci_;
  t_[CSECIRGB][CSPALSECAMRGB].get<T>() = &pals_;
  t_[CSECIRGB][CSProPhotoRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSECIRGB][CSProPhotoRGB].get<F>() = &eci_;
  t_[CSECIRGB][CSProPhotoRGB].get<T>() = &pro_;
  t_[CSECIRGB][CSSMPTECRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSECIRGB][CSSMPTECRGB].get<F>() = &eci_;
  t_[CSECIRGB][CSSMPTECRGB].get<T>() = &smpt_;
  t_[CSECIRGB][CSsRGB].get<C>() = Convert::From_RGB_To_sRGB;
  t_[CSECIRGB][CSsRGB].get<F>() = &eci_;
  t_[CSECIRGB][CSsRGB].get<T>() = &srgb_;
  t_[CSECIRGB][CSWideGamutRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSECIRGB][CSWideGamutRGB].get<F>() = &eci_;
  t_[CSECIRGB][CSWideGamutRGB].get<T>() = &wide_;

  /* from EktaSpacePS5 to all. */
  t_[CSEktaSpacePS5][CSXYZ].get<C>() = Convert::From_RGB_To_XYZ;
  t_[CSEktaSpacePS5][CSXYZ].get<F>() = &ekps5_;
  t_[CSEktaSpacePS5][CSxyY].get<C>() = Convert::From_RGB_To_xyY;
  t_[CSEktaSpacePS5][CSxyY].get<F>() = &ekps5_;
  t_[CSEktaSpacePS5][CSLab].get<C>() = Convert::From_RGB_To_Lab;
  t_[CSEktaSpacePS5][CSLab].get<F>() = &ekps5_;
  t_[CSEktaSpacePS5][CSLCHab].get<C>() = Convert::From_RGB_To_LCHab;
  t_[CSEktaSpacePS5][CSLCHab].get<F>() = &ekps5_;
  t_[CSEktaSpacePS5][CSLuv].get<C>() = Convert::From_RGB_To_Luv;
  t_[CSEktaSpacePS5][CSLuv].get<F>() = &ekps5_;
  t_[CSEktaSpacePS5][CSLCHuv].get<C>() = Convert::From_RGB_To_LCHuv;
  t_[CSEktaSpacePS5][CSLCHuv].get<F>() = &ekps5_;
  t_[CSEktaSpacePS5][CSAdobeRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSEktaSpacePS5][CSAdobeRGB].get<F>() = &ekps5_;
  t_[CSEktaSpacePS5][CSAdobeRGB].get<T>() = &adobe_;
  t_[CSEktaSpacePS5][CSAppleRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSEktaSpacePS5][CSAppleRGB].get<F>() = &ekps5_;
  t_[CSEktaSpacePS5][CSAppleRGB].get<T>() = &apple_;
  t_[CSEktaSpacePS5][CSBestRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSEktaSpacePS5][CSBestRGB].get<F>() = &ekps5_;
  t_[CSEktaSpacePS5][CSBestRGB].get<T>() = &best_;
  t_[CSEktaSpacePS5][CSBetaRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSEktaSpacePS5][CSBetaRGB].get<F>() = &ekps5_;
  t_[CSEktaSpacePS5][CSBetaRGB].get<T>() = &beta_;
  t_[CSEktaSpacePS5][CSBruceRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSEktaSpacePS5][CSBruceRGB].get<F>() = &ekps5_;
  t_[CSEktaSpacePS5][CSBruceRGB].get<T>() = &bruce_;
  t_[CSEktaSpacePS5][CSCIERGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSEktaSpacePS5][CSCIERGB].get<F>() = &ekps5_;
  t_[CSEktaSpacePS5][CSCIERGB].get<T>() = &cie_;
  t_[CSEktaSpacePS5][CSColorMatchRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSEktaSpacePS5][CSColorMatchRGB].get<F>() = &ekps5_;
  t_[CSEktaSpacePS5][CSColorMatchRGB].get<T>() = &cm_;
  t_[CSEktaSpacePS5][CSDonRGB4].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSEktaSpacePS5][CSDonRGB4].get<F>() = &ekps5_;
  t_[CSEktaSpacePS5][CSDonRGB4].get<T>() = &don4_;
  t_[CSEktaSpacePS5][CSECIRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSEktaSpacePS5][CSECIRGB].get<F>() = &ekps5_;
  t_[CSEktaSpacePS5][CSECIRGB].get<T>() = &eci_;
  t_[CSEktaSpacePS5][CSNTSCRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSEktaSpacePS5][CSNTSCRGB].get<F>() = &ekps5_;
  t_[CSEktaSpacePS5][CSNTSCRGB].get<T>() = &ntsc_;
  t_[CSEktaSpacePS5][CSPALSECAMRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSEktaSpacePS5][CSPALSECAMRGB].get<F>() = &ekps5_;
  t_[CSEktaSpacePS5][CSPALSECAMRGB].get<T>() = &pals_;
  t_[CSEktaSpacePS5][CSProPhotoRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSEktaSpacePS5][CSProPhotoRGB].get<F>() = &ekps5_;
  t_[CSEktaSpacePS5][CSProPhotoRGB].get<T>() = &pro_;
  t_[CSEktaSpacePS5][CSSMPTECRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSEktaSpacePS5][CSSMPTECRGB].get<F>() = &ekps5_;
  t_[CSEktaSpacePS5][CSSMPTECRGB].get<T>() = &smpt_;
  t_[CSEktaSpacePS5][CSsRGB].get<C>() = Convert::From_RGB_To_sRGB;
  t_[CSEktaSpacePS5][CSsRGB].get<F>() = &ekps5_;
  t_[CSEktaSpacePS5][CSsRGB].get<T>() = &srgb_;
  t_[CSEktaSpacePS5][CSWideGamutRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSEktaSpacePS5][CSWideGamutRGB].get<F>() = &ekps5_;
  t_[CSEktaSpacePS5][CSWideGamutRGB].get<T>() = &wide_;

  /* from NTSC to all. */
  t_[CSNTSCRGB][CSXYZ].get<C>() = Convert::From_RGB_To_XYZ;
  t_[CSNTSCRGB][CSXYZ].get<F>() = &ntsc_;
  t_[CSNTSCRGB][CSxyY].get<C>() = Convert::From_RGB_To_xyY;
  t_[CSNTSCRGB][CSxyY].get<F>() = &ntsc_;
  t_[CSNTSCRGB][CSLab].get<C>() = Convert::From_RGB_To_Lab;
  t_[CSNTSCRGB][CSLab].get<F>() = &ntsc_;
  t_[CSNTSCRGB][CSLCHab].get<C>() = Convert::From_RGB_To_LCHab;
  t_[CSNTSCRGB][CSLCHab].get<F>() = &ntsc_;
  t_[CSNTSCRGB][CSLuv].get<C>() = Convert::From_RGB_To_Luv;
  t_[CSNTSCRGB][CSLuv].get<F>() = &ntsc_;
  t_[CSNTSCRGB][CSLCHuv].get<C>() = Convert::From_RGB_To_LCHuv;
  t_[CSNTSCRGB][CSLCHuv].get<F>() = &ntsc_;
  t_[CSNTSCRGB][CSAdobeRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSNTSCRGB][CSAdobeRGB].get<F>() = &ntsc_;
  t_[CSNTSCRGB][CSAdobeRGB].get<T>() = &adobe_;
  t_[CSNTSCRGB][CSAppleRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSNTSCRGB][CSAppleRGB].get<F>() = &ntsc_;
  t_[CSNTSCRGB][CSAppleRGB].get<T>() = &apple_;
  t_[CSNTSCRGB][CSBestRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSNTSCRGB][CSBestRGB].get<F>() = &ntsc_;
  t_[CSNTSCRGB][CSBestRGB].get<T>() = &best_;
  t_[CSNTSCRGB][CSBetaRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSNTSCRGB][CSBetaRGB].get<F>() = &ntsc_;
  t_[CSNTSCRGB][CSBetaRGB].get<T>() = &beta_;
  t_[CSNTSCRGB][CSBruceRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSNTSCRGB][CSBruceRGB].get<F>() = &ntsc_;
  t_[CSNTSCRGB][CSBruceRGB].get<T>() = &bruce_;
  t_[CSNTSCRGB][CSCIERGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSNTSCRGB][CSCIERGB].get<F>() = &ntsc_;
  t_[CSNTSCRGB][CSCIERGB].get<T>() = &cie_;
  t_[CSNTSCRGB][CSColorMatchRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSNTSCRGB][CSColorMatchRGB].get<F>() = &ntsc_;
  t_[CSNTSCRGB][CSColorMatchRGB].get<T>() = &cm_;
  t_[CSNTSCRGB][CSDonRGB4].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSNTSCRGB][CSDonRGB4].get<F>() = &ntsc_;
  t_[CSNTSCRGB][CSDonRGB4].get<T>() = &don4_;
  t_[CSNTSCRGB][CSECIRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSNTSCRGB][CSECIRGB].get<F>() = &ntsc_;
  t_[CSNTSCRGB][CSECIRGB].get<T>() = &eci_;
  t_[CSNTSCRGB][CSEktaSpacePS5].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSNTSCRGB][CSEktaSpacePS5].get<F>() = &ntsc_;
  t_[CSNTSCRGB][CSEktaSpacePS5].get<T>() = &ekps5_;
  t_[CSNTSCRGB][CSPALSECAMRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSNTSCRGB][CSPALSECAMRGB].get<F>() = &ntsc_;
  t_[CSNTSCRGB][CSPALSECAMRGB].get<T>() = &pals_;
  t_[CSNTSCRGB][CSProPhotoRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSNTSCRGB][CSProPhotoRGB].get<F>() = &ntsc_;
  t_[CSNTSCRGB][CSProPhotoRGB].get<T>() = &pro_;
  t_[CSNTSCRGB][CSSMPTECRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSNTSCRGB][CSSMPTECRGB].get<F>() = &ntsc_;
  t_[CSNTSCRGB][CSSMPTECRGB].get<T>() = &smpt_;
  t_[CSNTSCRGB][CSsRGB].get<C>() = Convert::From_RGB_To_sRGB;
  t_[CSNTSCRGB][CSsRGB].get<F>() = &ntsc_;
  t_[CSNTSCRGB][CSsRGB].get<T>() = &srgb_;
  t_[CSNTSCRGB][CSWideGamutRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSNTSCRGB][CSWideGamutRGB].get<F>() = &ntsc_;
  t_[CSNTSCRGB][CSWideGamutRGB].get<T>() = &wide_;

  /* from PAL_SECAM to all. */
  t_[CSPALSECAMRGB][CSXYZ].get<C>() = Convert::From_RGB_To_XYZ;
  t_[CSPALSECAMRGB][CSXYZ].get<F>() = &pals_;
  t_[CSPALSECAMRGB][CSxyY].get<C>() = Convert::From_RGB_To_xyY;
  t_[CSPALSECAMRGB][CSxyY].get<F>() = &pals_;
  t_[CSPALSECAMRGB][CSLab].get<C>() = Convert::From_RGB_To_Lab;
  t_[CSPALSECAMRGB][CSLab].get<F>() = &pals_;
  t_[CSPALSECAMRGB][CSLCHab].get<C>() = Convert::From_RGB_To_LCHab;
  t_[CSPALSECAMRGB][CSLCHab].get<F>() = &pals_;
  t_[CSPALSECAMRGB][CSLuv].get<C>() = Convert::From_RGB_To_Luv;
  t_[CSPALSECAMRGB][CSLuv].get<F>() = &pals_;
  t_[CSPALSECAMRGB][CSLCHuv].get<C>() = Convert::From_RGB_To_LCHuv;
  t_[CSPALSECAMRGB][CSLCHuv].get<F>() = &pals_;
  t_[CSPALSECAMRGB][CSAdobeRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSPALSECAMRGB][CSAdobeRGB].get<F>() = &pals_;
  t_[CSPALSECAMRGB][CSAdobeRGB].get<T>() = &adobe_;
  t_[CSPALSECAMRGB][CSAppleRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSPALSECAMRGB][CSAppleRGB].get<F>() = &pals_;
  t_[CSPALSECAMRGB][CSAppleRGB].get<T>() = &apple_;
  t_[CSPALSECAMRGB][CSBestRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSPALSECAMRGB][CSBestRGB].get<F>() = &pals_;
  t_[CSPALSECAMRGB][CSBestRGB].get<T>() = &best_;
  t_[CSPALSECAMRGB][CSBetaRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSPALSECAMRGB][CSBetaRGB].get<F>() = &pals_;
  t_[CSPALSECAMRGB][CSBetaRGB].get<T>() = &beta_;
  t_[CSPALSECAMRGB][CSBruceRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSPALSECAMRGB][CSBruceRGB].get<F>() = &pals_;
  t_[CSPALSECAMRGB][CSBruceRGB].get<T>() = &bruce_;
  t_[CSPALSECAMRGB][CSCIERGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSPALSECAMRGB][CSCIERGB].get<F>() = &pals_;
  t_[CSPALSECAMRGB][CSCIERGB].get<T>() = &cie_;
  t_[CSPALSECAMRGB][CSColorMatchRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSPALSECAMRGB][CSColorMatchRGB].get<F>() = &pals_;
  t_[CSPALSECAMRGB][CSColorMatchRGB].get<T>() = &cm_;
  t_[CSPALSECAMRGB][CSDonRGB4].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSPALSECAMRGB][CSDonRGB4].get<F>() = &pals_;
  t_[CSPALSECAMRGB][CSDonRGB4].get<T>() = &don4_;
  t_[CSPALSECAMRGB][CSECIRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSPALSECAMRGB][CSECIRGB].get<F>() = &pals_;
  t_[CSPALSECAMRGB][CSECIRGB].get<T>() = &eci_;
  t_[CSPALSECAMRGB][CSEktaSpacePS5].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSPALSECAMRGB][CSEktaSpacePS5].get<F>() = &pals_;
  t_[CSPALSECAMRGB][CSEktaSpacePS5].get<T>() = &ekps5_;
  t_[CSPALSECAMRGB][CSNTSCRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSPALSECAMRGB][CSNTSCRGB].get<F>() = &pals_;
  t_[CSPALSECAMRGB][CSNTSCRGB].get<T>() = &ntsc_;
  t_[CSPALSECAMRGB][CSProPhotoRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSPALSECAMRGB][CSProPhotoRGB].get<F>() = &pals_;
  t_[CSPALSECAMRGB][CSProPhotoRGB].get<T>() = &pro_;
  t_[CSPALSECAMRGB][CSSMPTECRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSPALSECAMRGB][CSSMPTECRGB].get<F>() = &pals_;
  t_[CSPALSECAMRGB][CSSMPTECRGB].get<T>() = &smpt_;
  t_[CSPALSECAMRGB][CSsRGB].get<C>() = Convert::From_RGB_To_sRGB;
  t_[CSPALSECAMRGB][CSsRGB].get<F>() = &pals_;
  t_[CSPALSECAMRGB][CSsRGB].get<T>() = &srgb_;
  t_[CSPALSECAMRGB][CSWideGamutRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSPALSECAMRGB][CSWideGamutRGB].get<F>() = &pals_;
  t_[CSPALSECAMRGB][CSWideGamutRGB].get<T>() = &wide_;

  /* from ProPhoto to all. */
  t_[CSProPhotoRGB][CSXYZ].get<C>() = Convert::From_RGB_To_XYZ;
  t_[CSProPhotoRGB][CSXYZ].get<F>() = &pro_;
  t_[CSProPhotoRGB][CSxyY].get<C>() = Convert::From_RGB_To_xyY;
  t_[CSProPhotoRGB][CSxyY].get<F>() = &pro_;
  t_[CSProPhotoRGB][CSLab].get<C>() = Convert::From_RGB_To_Lab;
  t_[CSProPhotoRGB][CSLab].get<F>() = &pro_;
  t_[CSProPhotoRGB][CSLCHab].get<C>() = Convert::From_RGB_To_LCHab;
  t_[CSProPhotoRGB][CSLCHab].get<F>() = &pro_;
  t_[CSProPhotoRGB][CSLuv].get<C>() = Convert::From_RGB_To_Luv;
  t_[CSProPhotoRGB][CSLuv].get<F>() = &pro_;
  t_[CSProPhotoRGB][CSLCHuv].get<C>() = Convert::From_RGB_To_LCHuv;
  t_[CSProPhotoRGB][CSLCHuv].get<F>() = &pro_;
  t_[CSProPhotoRGB][CSAdobeRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSProPhotoRGB][CSAdobeRGB].get<F>() = &pro_;
  t_[CSProPhotoRGB][CSAdobeRGB].get<T>() = &adobe_;
  t_[CSProPhotoRGB][CSAppleRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSProPhotoRGB][CSAppleRGB].get<F>() = &pro_;
  t_[CSProPhotoRGB][CSAppleRGB].get<T>() = &apple_;
  t_[CSProPhotoRGB][CSBestRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSProPhotoRGB][CSBestRGB].get<F>() = &pro_;
  t_[CSProPhotoRGB][CSBestRGB].get<T>() = &best_;
  t_[CSProPhotoRGB][CSBetaRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSProPhotoRGB][CSBetaRGB].get<F>() = &pro_;
  t_[CSProPhotoRGB][CSBetaRGB].get<T>() = &beta_;
  t_[CSProPhotoRGB][CSBruceRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSProPhotoRGB][CSBruceRGB].get<F>() = &pro_;
  t_[CSProPhotoRGB][CSBruceRGB].get<T>() = &bruce_;
  t_[CSProPhotoRGB][CSCIERGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSProPhotoRGB][CSCIERGB].get<F>() = &pro_;
  t_[CSProPhotoRGB][CSCIERGB].get<T>() = &cie_;
  t_[CSProPhotoRGB][CSColorMatchRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSProPhotoRGB][CSColorMatchRGB].get<F>() = &pro_;
  t_[CSProPhotoRGB][CSColorMatchRGB].get<T>() = &cm_;
  t_[CSProPhotoRGB][CSDonRGB4].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSProPhotoRGB][CSDonRGB4].get<F>() = &pro_;
  t_[CSProPhotoRGB][CSDonRGB4].get<T>() = &don4_;
  t_[CSProPhotoRGB][CSECIRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSProPhotoRGB][CSECIRGB].get<F>() = &pro_;
  t_[CSProPhotoRGB][CSECIRGB].get<T>() = &eci_;
  t_[CSProPhotoRGB][CSEktaSpacePS5].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSProPhotoRGB][CSEktaSpacePS5].get<F>() = &pro_;
  t_[CSProPhotoRGB][CSEktaSpacePS5].get<T>() = &ekps5_;
  t_[CSProPhotoRGB][CSNTSCRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSProPhotoRGB][CSNTSCRGB].get<F>() = &pro_;
  t_[CSProPhotoRGB][CSNTSCRGB].get<T>() = &ntsc_;
  t_[CSProPhotoRGB][CSPALSECAMRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSProPhotoRGB][CSPALSECAMRGB].get<F>() = &pro_;
  t_[CSProPhotoRGB][CSPALSECAMRGB].get<T>() = &pals_;
  t_[CSProPhotoRGB][CSSMPTECRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSProPhotoRGB][CSSMPTECRGB].get<F>() = &pro_;
  t_[CSProPhotoRGB][CSSMPTECRGB].get<T>() = &smpt_;
  t_[CSProPhotoRGB][CSsRGB].get<C>() = Convert::From_RGB_To_sRGB;
  t_[CSProPhotoRGB][CSsRGB].get<F>() = &pro_;
  t_[CSProPhotoRGB][CSsRGB].get<T>() = &srgb_;
  t_[CSProPhotoRGB][CSWideGamutRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSProPhotoRGB][CSWideGamutRGB].get<F>() = &pro_;
  t_[CSProPhotoRGB][CSWideGamutRGB].get<T>() = &wide_;

  /* from SMPTE_C to all. */
  t_[CSSMPTECRGB][CSXYZ].get<C>() = Convert::From_RGB_To_XYZ;
  t_[CSSMPTECRGB][CSXYZ].get<F>() = &smpt_;
  t_[CSSMPTECRGB][CSxyY].get<C>() = Convert::From_RGB_To_xyY;
  t_[CSSMPTECRGB][CSxyY].get<F>() = &smpt_;
  t_[CSSMPTECRGB][CSLab].get<C>() = Convert::From_RGB_To_Lab;
  t_[CSSMPTECRGB][CSLab].get<F>() = &smpt_;
  t_[CSSMPTECRGB][CSLCHab].get<C>() = Convert::From_RGB_To_LCHab;
  t_[CSSMPTECRGB][CSLCHab].get<F>() = &smpt_;
  t_[CSSMPTECRGB][CSLuv].get<C>() = Convert::From_RGB_To_Luv;
  t_[CSSMPTECRGB][CSLuv].get<F>() = &smpt_;
  t_[CSSMPTECRGB][CSLCHuv].get<C>() = Convert::From_RGB_To_LCHuv;
  t_[CSSMPTECRGB][CSLCHuv].get<F>() = &smpt_;
  t_[CSSMPTECRGB][CSAdobeRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSSMPTECRGB][CSAdobeRGB].get<F>() = &smpt_;
  t_[CSSMPTECRGB][CSAdobeRGB].get<T>() = &adobe_;
  t_[CSSMPTECRGB][CSAppleRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSSMPTECRGB][CSAppleRGB].get<F>() = &smpt_;
  t_[CSSMPTECRGB][CSAppleRGB].get<T>() = &apple_;
  t_[CSSMPTECRGB][CSBestRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSSMPTECRGB][CSBestRGB].get<F>() = &smpt_;
  t_[CSSMPTECRGB][CSBestRGB].get<T>() = &best_;
  t_[CSSMPTECRGB][CSBetaRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSSMPTECRGB][CSBetaRGB].get<F>() = &smpt_;
  t_[CSSMPTECRGB][CSBetaRGB].get<T>() = &beta_;
  t_[CSSMPTECRGB][CSBruceRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSSMPTECRGB][CSBruceRGB].get<F>() = &smpt_;
  t_[CSSMPTECRGB][CSBruceRGB].get<T>() = &bruce_;
  t_[CSSMPTECRGB][CSCIERGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSSMPTECRGB][CSCIERGB].get<F>() = &smpt_;
  t_[CSSMPTECRGB][CSCIERGB].get<T>() = &cie_;
  t_[CSSMPTECRGB][CSColorMatchRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSSMPTECRGB][CSColorMatchRGB].get<F>() = &smpt_;
  t_[CSSMPTECRGB][CSColorMatchRGB].get<T>() = &cm_;
  t_[CSSMPTECRGB][CSDonRGB4].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSSMPTECRGB][CSDonRGB4].get<F>() = &smpt_;
  t_[CSSMPTECRGB][CSDonRGB4].get<T>() = &don4_;
  t_[CSSMPTECRGB][CSECIRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSSMPTECRGB][CSECIRGB].get<F>() = &smpt_;
  t_[CSSMPTECRGB][CSECIRGB].get<T>() = &eci_;
  t_[CSSMPTECRGB][CSEktaSpacePS5].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSSMPTECRGB][CSEktaSpacePS5].get<F>() = &smpt_;
  t_[CSSMPTECRGB][CSEktaSpacePS5].get<T>() = &ekps5_;
  t_[CSSMPTECRGB][CSNTSCRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSSMPTECRGB][CSNTSCRGB].get<F>() = &smpt_;
  t_[CSSMPTECRGB][CSNTSCRGB].get<T>() = &ntsc_;
  t_[CSSMPTECRGB][CSPALSECAMRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSSMPTECRGB][CSPALSECAMRGB].get<F>() = &smpt_;
  t_[CSSMPTECRGB][CSPALSECAMRGB].get<T>() = &pals_;
  t_[CSSMPTECRGB][CSProPhotoRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSSMPTECRGB][CSProPhotoRGB].get<F>() = &smpt_;
  t_[CSSMPTECRGB][CSProPhotoRGB].get<T>() = &pro_;
  t_[CSSMPTECRGB][CSsRGB].get<C>() = Convert::From_RGB_To_sRGB;
  t_[CSSMPTECRGB][CSsRGB].get<F>() = &smpt_;
  t_[CSSMPTECRGB][CSsRGB].get<T>() = &srgb_;
  t_[CSSMPTECRGB][CSWideGamutRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSSMPTECRGB][CSWideGamutRGB].get<F>() = &smpt_;
  t_[CSSMPTECRGB][CSWideGamutRGB].get<T>() = &wide_;

  /* from SRGB to all. */
  t_[CSsRGB][CSXYZ].get<C>() = Convert::From_RGB_To_XYZ;
  t_[CSsRGB][CSXYZ].get<F>() = &srgb_;
  t_[CSsRGB][CSxyY].get<C>() = Convert::From_RGB_To_xyY;
  t_[CSsRGB][CSxyY].get<F>() = &srgb_;
  t_[CSsRGB][CSLab].get<C>() = Convert::From_RGB_To_Lab;
  t_[CSsRGB][CSLab].get<F>() = &srgb_;
  t_[CSsRGB][CSLCHab].get<C>() = Convert::From_RGB_To_LCHab;
  t_[CSsRGB][CSLCHab].get<F>() = &srgb_;
  t_[CSsRGB][CSLuv].get<C>() = Convert::From_RGB_To_Luv;
  t_[CSsRGB][CSLuv].get<F>() = &srgb_;
  t_[CSsRGB][CSLCHuv].get<C>() = Convert::From_RGB_To_LCHuv;
  t_[CSsRGB][CSLCHuv].get<F>() = &srgb_;
  t_[CSsRGB][CSAdobeRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSsRGB][CSAdobeRGB].get<F>() = &srgb_;
  t_[CSsRGB][CSAdobeRGB].get<T>() = &adobe_;
  t_[CSsRGB][CSAppleRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSsRGB][CSAppleRGB].get<F>() = &srgb_;
  t_[CSsRGB][CSAppleRGB].get<T>() = &apple_;
  t_[CSsRGB][CSBestRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSsRGB][CSBestRGB].get<F>() = &srgb_;
  t_[CSsRGB][CSBestRGB].get<T>() = &best_;
  t_[CSsRGB][CSBetaRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSsRGB][CSBetaRGB].get<F>() = &srgb_;
  t_[CSsRGB][CSBetaRGB].get<T>() = &beta_;
  t_[CSsRGB][CSBruceRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSsRGB][CSBruceRGB].get<F>() = &srgb_;
  t_[CSsRGB][CSBruceRGB].get<T>() = &bruce_;
  t_[CSsRGB][CSCIERGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSsRGB][CSCIERGB].get<F>() = &srgb_;
  t_[CSsRGB][CSCIERGB].get<T>() = &cie_;
  t_[CSsRGB][CSColorMatchRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSsRGB][CSColorMatchRGB].get<F>() = &srgb_;
  t_[CSsRGB][CSColorMatchRGB].get<T>() = &cm_;
  t_[CSsRGB][CSDonRGB4].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSsRGB][CSDonRGB4].get<F>() = &srgb_;
  t_[CSsRGB][CSDonRGB4].get<T>() = &don4_;
  t_[CSsRGB][CSECIRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSsRGB][CSECIRGB].get<F>() = &srgb_;
  t_[CSsRGB][CSECIRGB].get<T>() = &eci_;
  t_[CSsRGB][CSEktaSpacePS5].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSsRGB][CSEktaSpacePS5].get<F>() = &srgb_;
  t_[CSsRGB][CSEktaSpacePS5].get<T>() = &ekps5_;
  t_[CSsRGB][CSNTSCRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSsRGB][CSNTSCRGB].get<F>() = &srgb_;
  t_[CSsRGB][CSNTSCRGB].get<T>() = &ntsc_;
  t_[CSsRGB][CSPALSECAMRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSsRGB][CSPALSECAMRGB].get<F>() = &srgb_;
  t_[CSsRGB][CSPALSECAMRGB].get<T>() = &pals_;
  t_[CSsRGB][CSProPhotoRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSsRGB][CSProPhotoRGB].get<F>() = &srgb_;
  t_[CSsRGB][CSProPhotoRGB].get<T>() = &pro_;
  t_[CSsRGB][CSSMPTECRGB].get<C>() = Convert::From_RGB_To_sRGB;
  t_[CSsRGB][CSSMPTECRGB].get<F>() = &srgb_;
  t_[CSsRGB][CSSMPTECRGB].get<T>() = &smpt_;
  t_[CSsRGB][CSWideGamutRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSsRGB][CSWideGamutRGB].get<F>() = &srgb_;
  t_[CSsRGB][CSWideGamutRGB].get<T>() = &wide_;

  /* from WideGamut to all. */
  t_[CSWideGamutRGB][CSXYZ].get<C>() = Convert::From_RGB_To_XYZ;
  t_[CSWideGamutRGB][CSXYZ].get<F>() = &wide_;
  t_[CSWideGamutRGB][CSxyY].get<C>() = Convert::From_RGB_To_xyY;
  t_[CSWideGamutRGB][CSxyY].get<F>() = &wide_;
  t_[CSWideGamutRGB][CSLab].get<C>() = Convert::From_RGB_To_Lab;
  t_[CSWideGamutRGB][CSLab].get<F>() = &wide_;
  t_[CSWideGamutRGB][CSLCHab].get<C>() = Convert::From_RGB_To_LCHab;
  t_[CSWideGamutRGB][CSLCHab].get<F>() = &wide_;
  t_[CSWideGamutRGB][CSLuv].get<C>() = Convert::From_RGB_To_Luv;
  t_[CSWideGamutRGB][CSLuv].get<F>() = &wide_;
  t_[CSWideGamutRGB][CSLCHuv].get<C>() = Convert::From_RGB_To_LCHuv;
  t_[CSWideGamutRGB][CSLCHuv].get<F>() = &wide_;
  t_[CSWideGamutRGB][CSAdobeRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSWideGamutRGB][CSAdobeRGB].get<F>() = &wide_;
  t_[CSWideGamutRGB][CSAdobeRGB].get<T>() = &adobe_;
  t_[CSWideGamutRGB][CSAppleRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSWideGamutRGB][CSAppleRGB].get<F>() = &wide_;
  t_[CSWideGamutRGB][CSAppleRGB].get<T>() = &apple_;
  t_[CSWideGamutRGB][CSBestRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSWideGamutRGB][CSBestRGB].get<F>() = &wide_;
  t_[CSWideGamutRGB][CSBestRGB].get<T>() = &best_;
  t_[CSWideGamutRGB][CSBetaRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSWideGamutRGB][CSBetaRGB].get<F>() = &wide_;
  t_[CSWideGamutRGB][CSBetaRGB].get<T>() = &beta_;
  t_[CSWideGamutRGB][CSBruceRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSWideGamutRGB][CSBruceRGB].get<F>() = &wide_;
  t_[CSWideGamutRGB][CSBruceRGB].get<T>() = &bruce_;
  t_[CSWideGamutRGB][CSCIERGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSWideGamutRGB][CSCIERGB].get<F>() = &wide_;
  t_[CSWideGamutRGB][CSCIERGB].get<T>() = &cie_;
  t_[CSWideGamutRGB][CSColorMatchRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSWideGamutRGB][CSColorMatchRGB].get<F>() = &wide_;
  t_[CSWideGamutRGB][CSColorMatchRGB].get<T>() = &cm_;
  t_[CSWideGamutRGB][CSDonRGB4].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSWideGamutRGB][CSDonRGB4].get<F>() = &wide_;
  t_[CSWideGamutRGB][CSDonRGB4].get<T>() = &don4_;
  t_[CSWideGamutRGB][CSECIRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSWideGamutRGB][CSECIRGB].get<F>() = &wide_;
  t_[CSWideGamutRGB][CSECIRGB].get<T>() = &eci_;
  t_[CSWideGamutRGB][CSEktaSpacePS5].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSWideGamutRGB][CSEktaSpacePS5].get<F>() = &wide_;
  t_[CSWideGamutRGB][CSEktaSpacePS5].get<T>() = &ekps5_;
  t_[CSWideGamutRGB][CSNTSCRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSWideGamutRGB][CSNTSCRGB].get<F>() = &wide_;
  t_[CSWideGamutRGB][CSNTSCRGB].get<T>() = &ntsc_;
  t_[CSWideGamutRGB][CSPALSECAMRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSWideGamutRGB][CSPALSECAMRGB].get<F>() = &wide_;
  t_[CSWideGamutRGB][CSPALSECAMRGB].get<T>() = &pals_;
  t_[CSWideGamutRGB][CSProPhotoRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSWideGamutRGB][CSProPhotoRGB].get<F>() = &wide_;
  t_[CSWideGamutRGB][CSProPhotoRGB].get<T>() = &pro_;
  t_[CSWideGamutRGB][CSSMPTECRGB].get<C>() = Convert::From_RGB_To_sRGB;
  t_[CSWideGamutRGB][CSSMPTECRGB].get<F>() = &wide_;
  t_[CSWideGamutRGB][CSSMPTECRGB].get<T>() = &smpt_;
  t_[CSWideGamutRGB][CSsRGB].get<C>() = Convert::From_RGB_To_RGB;
  t_[CSWideGamutRGB][CSsRGB].get<F>() = &wide_;
  t_[CSWideGamutRGB][CSsRGB].get<T>() = &srgb_;
}

const Conversions::Table& Conversions::GetTable() const{ return t_; }
