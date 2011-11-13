/***********************************************************************
|*  Copyright (C) 2010, 2011 Arlen Avakian
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


#ifndef COLOUR_LUV_HPP
#define COLOUR_LUV_HPP

#include "ColourSpace.hpp"
#include "ForwardDeclarations.hpp"
#include "Illuminant.hpp"

#include <Eigen/Core>
#include <Eigen/Dense>

using namespace Eigen;


template <class Real>
class Colour_Luv : public ColourSpace<Real, Matrix<Real, 3, 1> >{

  typedef Colour_XYZ<Real> XYZ;
  typedef Colour_xyY<Real> xyY;
  typedef Colour_Lab<Real> Lab;
  typedef Colour_LCHab<Real> LCHab;
  typedef Colour_Luv<Real> Luv;
  typedef Colour_LCHuv<Real> LCHuv;
  typedef BaseIlluminant<Real> Illuminant;
  typedef Matrix<Real, 3, 1> Coord3;

public:
  Colour_Luv(const Coord3& tri) : ColourSpace<Real, Coord3>{tri}{ }

  Colour_Luv(const Real L = 100, const Real a = 0, const Real b = 0) :
    ColourSpace<Real, Coord3>{Coord3(L, a, b)}{ }

  Colour_Luv(const Luv& col) : ColourSpace<Real, Coord3>{col._coords}{ }


  Luv& from(const XYZ& col, const Illuminant& rw){

    Real yr, up, vp, urp, vrp, L, u, v;
    yr = col[1] / rw.colour_XYZ()[1];
    ComputeUoVo<Real>(up, vp, col.coords());
    ComputeUoVo<Real>(urp, vrp, rw.colour_XYZ().coords());

    if( yr > Constants<Real>::_cie_epsilon )
      L = 116.0 * pow(yr, 1.0/3.0) - 16.0;
    else
      L = Constants<Real>::_cie_kappa * yr;

    u = 13.0 * L * (up - urp);
    v = 13.0 * L * (vp - vrp);

    this->_coords(0) = L;
    this->_coords(1) = u;
    this->_coords(2) = v;
    return *this;
  }

  Luv& from(const xyY& col, const Illuminant& rw){

    return from(XYZ().from(col), rw);
  }

  Luv& from(const Lab& col, const Illuminant& rw){
    
    return from(XYZ().from(col, rw), rw);
  }

  Luv& from(const LCHab& col, const Illuminant& rw){
    
    return from(XYZ().from(col, rw), rw);
  }

  Luv& from(const LCHuv& col){

    this->_coords(0) = col[0];
    this->_coords(1) = col[1] * cos(col[2] * Constants<Real>::_radian);
    this->_coords(2) = col[1] * sin(col[2] * Constants<Real>::_radian);
    return *this;
  }

  Luv& from(const RGB<Real>& col, const Illuminant& rw){

    return from(XYZ().from(col), rw);
  }


  template <class Colour>
  Colour to() const{ Colour rt; rt.from(*this); return rt; }

  template <class Colour>
  Colour to(const Illuminant& rw) const{
    
    Colour rt; rt.from(*this, rw); return rt;
  }
};

#endif
