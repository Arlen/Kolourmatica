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


#ifndef CONVERSION_HPP
#define CONVERSION_HPP

#include <cmath>

#include "Utility.hpp"


template <class ColorSpace>
class Convert{ };


template <>
class Convert<To::ColorSpaceXYZ>{

public:
  XYZ operator()(From::ColorSpacexyY, const xyY&);
  XYZ operator()(From::ColorSpaceLab, const Lab&, const XYZ&);
  XYZ operator()(From::ColorSpaceLCHab, const LCHab&, const XYZ&);
  XYZ operator()(From::ColorSpaceLuv, const Luv&, const XYZ&);
  XYZ operator()(From::ColorSpaceLCHuv, const LCHuv&, const XYZ&);
  XYZ operator()(From::ColorSpaceRGB, const RGB&, const XYZ&,
		 const Matrix3f&, const float);
  XYZ operator()(From::ColorSpacesRGB, const sRGB&, const XYZ&,
		 const Matrix3f&, const float);
};


template <>
class Convert<To::ColorSpacexyY>{

public:
  xyY operator()(From::ColorSpaceXYZ, const XYZ&);
  xyY operator()(From::ColorSpaceLab, const Lab&, const XYZ&);
  xyY operator()(From::ColorSpaceLCHab, const LCHab&, const XYZ&);
  xyY operator()(From::ColorSpaceLuv, const Luv&, const XYZ&);
  xyY operator()(From::ColorSpaceLCHuv, const LCHuv&, const XYZ&);
  xyY operator()(From::ColorSpaceRGB, const RGB&, const XYZ&,
		 const Matrix3f&, const float);
  xyY operator()(From::ColorSpacesRGB, const sRGB&, const XYZ&,
		 const Matrix3f&, const float);
};


template <>
class Convert<To::ColorSpaceLab>{

public:
  Lab operator()(From::ColorSpaceXYZ, const XYZ&, const XYZ&);
  Lab operator()(From::ColorSpacexyY, const xyY&, const XYZ&);
  Lab operator()(From::ColorSpaceLCHab, const LCHab&, const XYZ&);
  Lab operator()(From::ColorSpaceLuv, const Luv&, const XYZ&);
  Lab operator()(From::ColorSpaceLCHuv, const LCHuv&, const XYZ&);
  Lab operator()(From::ColorSpaceRGB, const RGB&, const XYZ&,
		 const Matrix3f&, const float);
  Lab operator()(From::ColorSpacesRGB, const sRGB&, const XYZ&,
		 const Matrix3f&, const float);
};


template <>
class Convert<To::ColorSpaceLCHab>{

public:
  LCHab operator()(From::ColorSpaceXYZ, const XYZ&, const XYZ&);
  LCHab operator()(From::ColorSpacexyY, const xyY&, const XYZ&);
  LCHab operator()(From::ColorSpaceLab, const Lab&, const XYZ&);
  LCHab operator()(From::ColorSpaceLuv, const Luv&, const XYZ&);
  LCHab operator()(From::ColorSpaceLCHuv, const LCHuv&, const XYZ&);
  LCHab operator()(From::ColorSpaceRGB, const RGB&, const XYZ&,
		   const Matrix3f&, const float);
  LCHab operator()(From::ColorSpacesRGB, const sRGB&, const XYZ&,
		   const Matrix3f&, const float);
};


template <>
class Convert<To::ColorSpaceLuv>{

public:
  Luv operator()(From::ColorSpaceXYZ, const XYZ&, const XYZ&);
  Luv operator()(From::ColorSpacexyY, const xyY&, const XYZ&);
  Luv operator()(From::ColorSpaceLab, const Lab&, const XYZ&);
  Luv operator()(From::ColorSpaceLCHab, const LCHab&, const XYZ&);
  Luv operator()(From::ColorSpaceLCHuv, const LCHuv&, const XYZ&);
  Luv operator()(From::ColorSpaceRGB, const RGB&, const XYZ&,
		 const Matrix3f&, const float);
  Luv operator()(From::ColorSpacesRGB, const sRGB&, const XYZ&,
		 const Matrix3f&, const float);
};


template <>
class Convert<To::ColorSpaceLCHuv>{

public:
  LCHuv operator()(From::ColorSpaceXYZ, const XYZ&, const XYZ&);
  LCHuv operator()(From::ColorSpacexyY, const xyY&, const XYZ&);
  LCHuv operator()(From::ColorSpaceLab, const Lab&, const XYZ&);
  LCHuv operator()(From::ColorSpaceLCHab, const LCHab&, const XYZ&);
  LCHuv operator()(From::ColorSpaceLuv, const Luv&, const XYZ&);
  LCHuv operator()(From::ColorSpaceRGB, const RGB&, const XYZ&,
		   const Matrix3f&, const float);
  LCHuv operator()(From::ColorSpacesRGB, const sRGB&, const XYZ&,
		   const Matrix3f&, const float);
};


template <>
class Convert<To::ColorSpaceRGB>{

public:
  RGB operator()(From::ColorSpaceXYZ, const XYZ&, const XYZ&,
		 const Matrix3f&, const float);
  RGB operator()(From::ColorSpacexyY, const xyY&, const XYZ&,
		 const Matrix3f&, const float);
  RGB operator()(From::ColorSpaceLab, const Lab&, const XYZ&,
		 const Matrix3f&, const float);
  RGB operator()(From::ColorSpaceLCHab, const LCHab&, const XYZ&,
		 const Matrix3f&, const float);
  RGB operator()(From::ColorSpaceLuv, const Luv&, const XYZ&,
		 const Matrix3f&, const float);
  RGB operator()(From::ColorSpaceLCHuv, const LCHuv&, const XYZ&,
		 const Matrix3f&, const float);
  RGB operator()(From::ColorSpacesRGB, const sRGB&, const XYZ&,
		 const Matrix3f&, const float,
		 const Matrix3f&, const float);
};


template <>
class Convert<To::ColorSpacesRGB>{

public:
  sRGB operator()(From::ColorSpaceXYZ, const XYZ&, const XYZ&,
		  const Matrix3f&, const float);
  sRGB operator()(From::ColorSpacexyY, const xyY&, const XYZ&,
		  const Matrix3f&, const float);
  sRGB operator()(From::ColorSpaceLab, const Lab&, const XYZ&,
		  const Matrix3f&, const float);
  sRGB operator()(From::ColorSpaceLCHab, const LCHab&, const XYZ&,
		  const Matrix3f&, const float);
  sRGB operator()(From::ColorSpaceLuv, const Luv&, const XYZ&,
		  const Matrix3f&, const float);
  sRGB operator()(From::ColorSpaceLCHuv, const LCHuv&, const XYZ&,
		  const Matrix3f&, const float);
  sRGB operator()(From::ColorSpaceRGB, const RGB&, const XYZ&,
		  const Matrix3f&, const float,
		  const Matrix3f&, const float);
};



/* Convert to __XYZ__ */
XYZ Convert<To::ColorSpaceXYZ>::operator()(From::ColorSpacexyY space,
					   const xyY& rFromxyY){
  XYZ xyz;
  xyz(0, 0) = rFromxyY(0, 0) / rFromxyY(1, 0) * rFromxyY(2, 0);
  xyz(1, 0) = rFromxyY(2, 0);
  xyz(2, 0) = ((1.0f - rFromxyY(0, 0) - rFromxyY(1, 0)) /
	       rFromxyY(1, 0)) * rFromxyY(2, 0);
  return xyz;
}

XYZ Convert<To::ColorSpaceXYZ>::operator()(From::ColorSpaceLab space,
					   const Lab& rFromLab,
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

XYZ Convert<To::ColorSpaceXYZ>::operator()(From::ColorSpaceLCHab space,
					   const LCHab& rFromLCHab,
					   const XYZ& rRefWhite){
  Convert<To::ColorSpaceLab> con;
  return
    operator()(From::Lab, con(space, rFromLCHab, rRefWhite), rRefWhite);
}

XYZ Convert<To::ColorSpaceXYZ>::operator()(From::ColorSpaceLuv space,
					   const Luv& rFromLuv,
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

XYZ Convert<To::ColorSpaceXYZ>::operator()(From::ColorSpaceLCHuv space,
					   const LCHuv& rFromLCHuv,
					   const XYZ& rRefWhite){
  Convert<To::ColorSpaceLuv> con;
  return
    operator()(From::Luv, con(space, rFromLCHuv, rRefWhite), rRefWhite);
}

XYZ Convert<To::ColorSpaceXYZ>::operator()(From::ColorSpaceRGB space,
					   const RGB& rFromRGB,
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

XYZ Convert<To::ColorSpaceXYZ>::operator()(From::ColorSpacesRGB space,
					   const sRGB& rFromsRGB,
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
xyY Convert<To::ColorSpacexyY>::operator()(From::ColorSpaceXYZ space,
					   const XYZ& rFromXYZ){
  xyY xyy;
  float s = rFromXYZ(0, 0) + rFromXYZ(1, 0) + rFromXYZ(2, 0);
  xyy(0, 0) = rFromXYZ(0, 0) / s;
  xyy(1, 0) = rFromXYZ(1, 0) / s;
  xyy(2, 0) = rFromXYZ(2, 0);
  return xyy;
}

xyY Convert<To::ColorSpacexyY>::operator()(From::ColorSpaceLab space,
					   const Lab& rFromLab,
					   const XYZ& rRefWhite){
  Convert<To::ColorSpaceXYZ> con;
  return operator()(From::XYZ, con(space, rFromLab, rRefWhite));
}

xyY Convert<To::ColorSpacexyY>::operator()(From::ColorSpaceLCHab space,
					   const LCHab& rFromLCHab,
					   const XYZ& rRefWhite){
  Convert<To::ColorSpaceXYZ> con;
  return operator()(From::XYZ, con(space, rFromLCHab, rRefWhite));
}

xyY Convert<To::ColorSpacexyY>::operator()(From::ColorSpaceLuv space,
					   const Luv& rFromLuv,
					   const XYZ& rRefWhite){
  Convert<To::ColorSpaceXYZ> con;
  return operator()(From::XYZ, con(space, rFromLuv, rRefWhite));
}

xyY Convert<To::ColorSpacexyY>::operator()(From::ColorSpaceLCHuv space,
					   const LCHuv& rFromLCHuv,
					   const XYZ& rRefWhite){
  Convert<To::ColorSpaceXYZ> con;
  return operator()(From::XYZ, con(space, rFromLCHuv, rRefWhite));
}

xyY Convert<To::ColorSpacexyY>::operator()(From::ColorSpaceRGB space,
					   const RGB& rFromRGB,
					   const XYZ& rRefWhite,
					   const Matrix3f& M,
					   const float gamma){
  Convert<To::ColorSpaceXYZ> con;
  return
    operator()(From::XYZ, con(space, rFromRGB, rRefWhite, M, gamma));
}

xyY Convert<To::ColorSpacexyY>::operator()(From::ColorSpacesRGB space,
					   const sRGB& rFromsRGB,
					   const XYZ& rRefWhite,
					   const Matrix3f& M,
					   const float gamma){
  Convert<To::ColorSpaceXYZ> con;
  return
    operator()(From::XYZ, con(space, rFromsRGB, rRefWhite, M, gamma));
}


/* Convert to __Lab__ */
Lab Convert<To::ColorSpaceLab>::operator()(From::ColorSpaceXYZ space,
					   const XYZ& rFromXYZ,
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

Lab Convert<To::ColorSpaceLab>::operator()(From::ColorSpacexyY space,
					   const xyY& rFromxyY,
					   const XYZ& rRefWhite){
  Convert<To::ColorSpaceXYZ> con;
  return operator()(From::XYZ, con(space, rFromxyY), rRefWhite);
}

Lab Convert<To::ColorSpaceLab>::operator()(From::ColorSpaceLCHab,
					   const LCHab& rFromLCHab,
					   const XYZ& rRefWhite){
  Lab lab;
  lab(0, 0) = rFromLCHab(0, 0);
  lab(1, 0) = rFromLCHab(1, 0) * cos(rFromLCHab(2, 0) * RAD);
  lab(2, 0) = rFromLCHab(1, 0) * sin(rFromLCHab(2, 0) * RAD);
  return lab;
}

Lab Convert<To::ColorSpaceLab>::operator()(From::ColorSpaceLuv space,
					   const Luv& rFromLuv,
					   const XYZ& rRefWhite){
  Convert<To::ColorSpaceXYZ> con;
  return operator()(From::XYZ, con(space, rFromLuv, rRefWhite), rRefWhite);
}

Lab Convert<To::ColorSpaceLab>::operator()(From::ColorSpaceLCHuv space,
					   const LCHuv& rFromLCHuv,
					   const XYZ& rRefWhite){
  Convert<To::ColorSpaceXYZ> con;
  return
    operator()(From::XYZ, con(space, rFromLCHuv, rRefWhite), rRefWhite);
}

Lab Convert<To::ColorSpaceLab>::operator()(From::ColorSpaceRGB space,
					   const RGB& rFromRGB,
					   const XYZ& rRefWhite,
					   const Matrix3f& M,
					   const float gamma){
  Convert<To::ColorSpaceXYZ> con;
  return operator()(From::XYZ, con(space, rFromRGB, rRefWhite, M, gamma),
		    rRefWhite);
}

Lab Convert<To::ColorSpaceLab>::operator()(From::ColorSpacesRGB space,
					   const sRGB& rFromsRGB,
					   const XYZ& rRefWhite,
					   const Matrix3f& M,
					   const float gamma){
  Convert<To::ColorSpaceXYZ> con;
  return operator()(From::XYZ, con(space, rFromsRGB, rRefWhite, M, gamma),
		    rRefWhite);
}


/* Convert to __LCHab__ */
LCHab Convert<To::ColorSpaceLCHab>::operator()(From::ColorSpaceXYZ space,
					       const XYZ& rFromXYZ,
					       const XYZ& rRefWhite){
  Convert<To::ColorSpaceLab> con;
  return operator()(From::Lab, con(space, rFromXYZ, rRefWhite), rRefWhite);
}

LCHab Convert<To::ColorSpaceLCHab>::operator()(From::ColorSpacexyY space,
					       const xyY& rFromxyY,
					       const XYZ& rRefWhite){
  Convert<To::ColorSpaceXYZ> con;
  return operator()(From::XYZ, con(space, rFromxyY), rRefWhite);
}

LCHab Convert<To::ColorSpaceLCHab>::operator()(From::ColorSpaceLab space,
					       const Lab& rFromLab,
					       const XYZ& rRefWhite){
  LCHab lchab;
  lchab(0, 0) = rFromLab(0, 0);
  lchab(1, 0) = sqrt(rFromLab(1, 0) * rFromLab(1, 0) +
		     rFromLab(2, 0) * rFromLab(2, 0));
  lchab(2, 0) = atan2(rFromLab(2, 0), rFromLab(1, 0)) * ANG;
  return lchab;
}

LCHab Convert<To::ColorSpaceLCHab>::operator()(From::ColorSpaceLuv space,
					       const Luv& rFromLuv,
					       const XYZ& rRefWhite){
  Convert<To::ColorSpaceXYZ> con;
  return operator()(From::XYZ, con(space, rFromLuv, rRefWhite), rRefWhite);
}

LCHab Convert<To::ColorSpaceLCHab>::operator()(From::ColorSpaceLCHuv space,
					       const LCHuv& rFromLCHuv,
					       const XYZ& rRefWhite){
  Convert<To::ColorSpaceXYZ> con;
  return
    operator()(From::XYZ, con(space, rFromLCHuv, rRefWhite), rRefWhite);
}

LCHab Convert<To::ColorSpaceLCHab>::operator()(From::ColorSpaceRGB space,
					       const RGB& rFromRGB,
					       const XYZ& rRefWhite,
					       const Matrix3f& M,
					       const float gamma){
  Convert<To::ColorSpaceXYZ> con;
  return operator()(From::XYZ, con(space, rFromRGB, rRefWhite, M, gamma),
		    rRefWhite);
}

LCHab Convert<To::ColorSpaceLCHab>::operator()(From::ColorSpacesRGB space,
					       const sRGB& rFromsRGB,
					       const XYZ& rRefWhite,
					       const Matrix3f& M,
					       const float gamma){
  Convert<To::ColorSpaceXYZ> con;
  return operator()(From::XYZ, con(space, rFromsRGB, rRefWhite, M, gamma),
		    rRefWhite);
}


/* Convert to __Luv__ */
Luv Convert<To::ColorSpaceLuv>::operator()(From::ColorSpaceXYZ space,
					   const XYZ& rFromXYZ,
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

Luv Convert<To::ColorSpaceLuv>::operator()(From::ColorSpacexyY space,
					   const xyY& rFromxyY,
					   const XYZ& rRefWhite){
  Convert<To::ColorSpaceXYZ> con;
  return operator()(From::XYZ, con(space, rFromxyY), rRefWhite);
}

Luv Convert<To::ColorSpaceLuv>::operator()(From::ColorSpaceLab space,
					   const Luv& rFromLab,
					   const XYZ& rRefWhite){
  Convert<To::ColorSpaceXYZ> con;
  return
    operator()(From::XYZ, con(space, rFromLab, rRefWhite), rRefWhite);
}

Luv Convert<To::ColorSpaceLuv>::operator()(From::ColorSpaceLCHab space,
					   const LCHab& rFromLCHab,
					   const XYZ& rRefWhite){
  Convert<To::ColorSpaceXYZ> con;
  return
    operator()(From::XYZ, con(space, rFromLCHab, rRefWhite), rRefWhite);
}

Luv Convert<To::ColorSpaceLuv>::operator()(From::ColorSpaceLCHuv space,
					   const LCHuv& rFromLCHuv,
					   const XYZ& rRefWhite){
  Luv luv;
  luv(0, 0) = rFromLCHuv(0, 0);
  luv(1, 0) = rFromLCHuv(1, 0) * cos(rFromLCHuv(2, 0) * RAD);
  luv(2, 0) = rFromLCHuv(1, 0) * sin(rFromLCHuv(2, 0) * RAD);
  return luv;
}

Luv Convert<To::ColorSpaceLuv>::operator()(From::ColorSpaceRGB space,
					   const RGB& rFromRGB,
					   const XYZ& rRefWhite,
					   const Matrix3f& M,
					   const float gamma){
  Convert<To::ColorSpaceXYZ> con;
  return operator()(From::XYZ, con(space, rFromRGB, rRefWhite, M, gamma),
		    rRefWhite);
}

Luv Convert<To::ColorSpaceLuv>::operator()(From::ColorSpacesRGB space,
					   const sRGB& rFromsRGB,
					   const XYZ& rRefWhite,
					   const Matrix3f& M,
					   const float gamma){
  Convert<To::ColorSpaceXYZ> con;
  return operator()(From::XYZ, con(space, rFromsRGB, rRefWhite, M, gamma),
		    rRefWhite);
}


/* Convert to __LCHuv__ */
LCHuv Convert<To::ColorSpaceLCHuv>::operator()(From::ColorSpaceXYZ space,
					       const XYZ& rFromXYZ,
					       const XYZ& rRefWhite){
  Convert<To::ColorSpaceLuv> con;
  return operator()(From::Luv, con(space, rFromXYZ, rRefWhite), rRefWhite);
}

LCHuv Convert<To::ColorSpaceLCHuv>::operator()(From::ColorSpacexyY space,
					       const xyY& rFromxyY,
					       const XYZ& rRefWhite){
  Convert<To::ColorSpaceXYZ> con;
  return operator()(From::XYZ, con(space, rFromxyY), rRefWhite);
}

LCHuv Convert<To::ColorSpaceLCHuv>::operator()(From::ColorSpaceLab space,
					       const Lab& rFromLab,
					       const XYZ& rRefWhite){
  Convert<To::ColorSpaceXYZ> con;
  return operator()(From::XYZ, con(space, rFromLab, rRefWhite), rRefWhite);
}

LCHuv Convert<To::ColorSpaceLCHuv>::operator()(From::ColorSpaceLCHab space,
					       const LCHab& rFromLCHab,
					       const XYZ& rRefWhite){
  Convert<To::ColorSpaceXYZ> con;
  return
    operator()(From::XYZ, con(space, rFromLCHab, rRefWhite), rRefWhite);
}

LCHuv Convert<To::ColorSpaceLCHuv>::operator()(From::ColorSpaceLuv space,
					       const Luv& rFromLuv,
					       const XYZ& rRefWhite){
  LCHuv lchuv;
  lchuv(0, 0) = rFromLuv(0, 0);
  lchuv(1, 0) = sqrt(rFromLuv(1, 0) * rFromLuv(1, 0) +
		     rFromLuv(2, 0) * rFromLuv(2, 0));
  lchuv(2, 0) = atan2(rFromLuv(2, 0), rFromLuv(1, 0)) * ANG;
  return lchuv;
}

LCHuv Convert<To::ColorSpaceLCHuv>::operator()(From::ColorSpaceRGB space,
					       const RGB& rFromRGB,
					       const XYZ& rRefWhite,
					       const Matrix3f& M,
					       const float gamma){
  Convert<To::ColorSpaceXYZ> con;
  return operator()(From::XYZ, con(space, rFromRGB, rRefWhite, M, gamma),
		    rRefWhite);
}

LCHuv Convert<To::ColorSpaceLCHuv>::operator()(From::ColorSpacesRGB space,
					       const sRGB& rFromsRGB,
					       const XYZ& rRefWhite,
					       const Matrix3f& M,
					       const float gamma){
  Convert<To::ColorSpaceXYZ> con;
  return operator()(From::XYZ, con(space, rFromsRGB, rRefWhite, M, gamma),
		    rRefWhite);
}


/* Convert to any __RGB__ */
RGB Convert<To::ColorSpaceRGB>::operator()(From::ColorSpaceXYZ space,
					   const XYZ& rFromXYZ,
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

RGB Convert<To::ColorSpaceRGB>::operator()(From::ColorSpacexyY space,
					   const xyY& rFromxyY,
					   const XYZ& rRefWhite,
					   const Matrix3f& M,
					   const float gamma){
  Convert<To::ColorSpaceXYZ> con;
  return operator()(From::XYZ, con(space, rFromxyY), rRefWhite, M, gamma);
}

RGB Convert<To::ColorSpaceRGB>::operator()(From::ColorSpaceLab space,
					   const Lab& rFromLab,
					   const XYZ& rRefWhite,
					   const Matrix3f& M,
					   const float gamma){
  Convert<To::ColorSpaceXYZ> con;
  return operator()(From::XYZ, con(space, rFromLab, rRefWhite), rRefWhite,
		    M, gamma);
}

RGB Convert<To::ColorSpaceRGB>::operator()(From::ColorSpaceLCHab space,
					   const LCHab& rFromLCHab,
					   const XYZ& rRefWhite,
					   const Matrix3f& M,
					   const float gamma){
  Convert<To::ColorSpaceXYZ> con;
  return operator()(From::XYZ, con(space, rFromLCHab, rRefWhite),
		    rRefWhite, M, gamma);
}

RGB Convert<To::ColorSpaceRGB>::operator()(From::ColorSpaceLuv space,
					   const Luv& rFromLuv,
					   const XYZ& rRefWhite,
					   const Matrix3f& M,
					   const float gamma){
  Convert<To::ColorSpaceXYZ> con;
  return operator()(From::XYZ, con(space, rFromLuv, rRefWhite), rRefWhite,
		    M, gamma);
}

RGB Convert<To::ColorSpaceRGB>::operator()(From::ColorSpaceLCHuv space,
					   const LCHuv& rFromLCHuv,
					   const XYZ& rRefWhite,
					   const Matrix3f& M,
					   const float gamma){
  Convert<To::ColorSpaceXYZ> con;
  return operator()(From::XYZ, con(space, rFromLCHuv, rRefWhite),
		    rRefWhite, M, gamma);
}

RGB Convert<To::ColorSpaceRGB>::operator()(From::ColorSpacesRGB space,
					   const sRGB& rFromsRGB,
					   const XYZ& rRefWhite,
					   const Matrix3f& M,
					   const float gamma,
					   const Matrix3f& sM,
					   const float sgamma){
  /* sM and sgamma are needed to convert sRGB to XYZ first. */
  Convert<To::ColorSpaceXYZ> con;
  return
    operator()(From::XYZ, con(space, rFromsRGB, rRefWhite, sM, sgamma),
	       rRefWhite, M, gamma);
}


/* Convert to __sRGB__ */
sRGB Convert<To::ColorSpacesRGB>::operator()(From::ColorSpaceXYZ space,
					     const XYZ& rFromXYZ,
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

sRGB Convert<To::ColorSpacesRGB>::operator()(From::ColorSpacexyY space,
					     const xyY& rFromxyY,
					     const XYZ& rRefWhite,
					     const Matrix3f& M,
					     const float gamma){
  Convert<To::ColorSpaceXYZ> con;
  return operator()(From::XYZ, con(space, rFromxyY), rRefWhite, M, gamma);
}

sRGB Convert<To::ColorSpacesRGB>::operator()(From::ColorSpaceLab space,
					     const Lab& rFromLab,
					     const XYZ& rRefWhite,
					     const Matrix3f& M,
					     const float gamma){
  Convert<To::ColorSpaceXYZ> con;
  return operator()(From::XYZ, con(space, rFromLab, rRefWhite), rRefWhite,
		    M, gamma);
}

sRGB Convert<To::ColorSpacesRGB>::operator()(From::ColorSpaceLCHab space,
					     const LCHab& rFromLCHab,
					     const XYZ& rRefWhite,
					     const Matrix3f& M,
					     const float gamma){
  Convert<To::ColorSpaceXYZ> con;
  return operator()(From::XYZ, con(space, rFromLCHab, rRefWhite),
		    rRefWhite, M, gamma);
}

sRGB Convert<To::ColorSpacesRGB>::operator()(From::ColorSpaceLuv space,
					     const Luv& rFromLuv,
					     const XYZ& rRefWhite,
					     const Matrix3f& M,
					     const float gamma){
  Convert<To::ColorSpaceXYZ> con;
  return operator()(From::XYZ, con(space, rFromLuv, rRefWhite), rRefWhite,
		    M, gamma);
}

sRGB Convert<To::ColorSpacesRGB>::operator()(From::ColorSpaceLCHuv space,
					     const LCHuv& rFromLCHuv,
					     const XYZ& rRefWhite,
					     const Matrix3f& M,
					     const float gamma){
  Convert<To::ColorSpaceXYZ> con;
  return operator()(From::XYZ, con(space, rFromLCHuv, rRefWhite),
		    rRefWhite, M, gamma);
}

sRGB Convert<To::ColorSpacesRGB>::operator()(From::ColorSpaceRGB space,
					     const RGB& rFromRGB,
					     const XYZ& rRefWhite,
					     const Matrix3f& M,
					     const float gamma,
					     const Matrix3f& sM,
					     const float sgamma){
  /* sM and sgamma are needed to convert RGB to XYZ first. */
  Convert<To::ColorSpaceXYZ> con;
  return operator()(From::XYZ, con(space, rFromRGB, rRefWhite, sM, sgamma),
		    rRefWhite, M, gamma);
}

#endif
