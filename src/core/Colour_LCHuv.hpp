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


#ifndef COLOUR_LCHUV_HPP
#define COLOUR_LCHUV_HPP

#include "ColourSpace.hpp"
#include "ForwardDeclarations.hpp"
#include "Illuminant.hpp"

#include <Eigen/Core>
#include <Eigen/Dense>

using namespace Eigen;


template <class Real>
class Colour_LCHuv : public ColourSpace<Real, Matrix<Real, 3, 1> >{

  typedef Colour_XYZ<Real> XYZ;
  typedef Colour_xyY<Real> xyY;
  typedef Colour_Lab<Real> Lab;
  typedef Colour_LCHab<Real> LCHab;
  typedef Colour_Luv<Real> Luv;
  typedef Colour_LCHuv<Real> LCHuv;
  typedef BaseIlluminant<Real> Illuminant;
  typedef Matrix<Real, 3, 1> Coord3;

public:
  Colour_LCHuv(const Coord3& tri) : ColourSpace<Real, Coord3>{tri}{ }

  Colour_LCHuv(const Real L = 100, const Real C = 0, const Real H = 0) :
    ColourSpace<Real, Coord3>{Coord3(L, C, H)}{ }

  Colour_LCHuv(const LCHuv& col) : ColourSpace<Real, Coord3>{col._coords}{ }


  LCHuv& from(const XYZ& col, const Illuminant& rw){

    return from(Luv().from(col, rw));
  }

  LCHuv& from(const xyY& col, const Illuminant& rw){

    return from(Luv().from(col, rw));
  }

  LCHuv& from(const Lab& col, const Illuminant& rw){
    
    return from(Luv().from(col, rw));
  }

  LCHuv& from(const LCHab& col, const Illuminant& rw){

    return from(Luv().from(col, rw));
  }

  LCHuv& from(const Luv& col){

    Real L, C, H;
    L = col[0];
    C = sqrt(col[1] * col[1] + col[2] * col[2]);
    Real h = atan2(col[2], col[1]) * Constants<Real>::_angle;
    if(h < 0.0){ h += 360.0; }
    if(h >= 360.0){ h -= 360.0; }
    H = h;

    this->_coords(0) = L;
    this->_coords(1) = C;
    this->_coords(2) = H;
    return *this;
  }

  LCHuv& from(const RGB<Real>& col, const Illuminant& rw){

    return from(Luv().from(col, rw));
  }


  template <class Colour>
  Colour to() const{ Colour rt; rt.from(*this); return rt; }

  template <class Colour>
  Colour to(const Illuminant& rw) const{

    Colour rt; rt.from(*this, rw); return rt;
  }
};

#endif
