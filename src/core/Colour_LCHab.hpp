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


#ifndef COLOUR_LCHAB_HPP
#define COLOUR_LCHAB_HPP

#include "ColourSpace.hpp"
#include "ForwardDeclarations.hpp"
#include "Illuminant.hpp"

#include <Eigen/Core>
#include <Eigen/Dense>

using namespace Eigen;


template <class Real>
class Colour_LCHab : public ColourSpace<Real, Matrix<Real, 3, 1> >{

  typedef Colour_XYZ<Real> XYZ;
  typedef Colour_xyY<Real> xyY;
  typedef Colour_Lab<Real> Lab;
  typedef Colour_LCHab<Real> LCHab;
  typedef Colour_Luv<Real> Luv;
  typedef Colour_LCHuv<Real> LCHuv;
  typedef BaseIlluminant<Real> Illuminant;
  typedef Matrix<Real, 3, 1> Coord3;
  typedef ColourSpace<Real, Matrix<Real, 3, 1> > Parent;


public:
  Colour_LCHab(const Coord3& tri) : ColourSpace<Real, Coord3>{tri}{ }

  Colour_LCHab(const Real L = 100, const Real C = 0, const Real H = 0) :
    ColourSpace<Real, Coord3>{Coord3(L, C, H)}{ }

  Colour_LCHab(const LCHab& col) : ColourSpace<Real, Coord3>{col._coords}{ }


  Coord3 to_XYZ(const Illuminant* const rw = nullptr) const{

    XYZ xyz; xyz.from(*this, *rw); return xyz.coords();
  }

  Coord3& from_XYZ(const Coord3& coords, const Illuminant* const rw = nullptr){

    from(XYZ(coords), *rw); return Parent::_coords;
  }


  LCHab& from(const XYZ& col, const Illuminant& rw){

    return from(Lab().from(col, rw));
  }

  LCHab& from(const xyY& col, const Illuminant& rw){

    return from(Lab().from(col, rw));
  }

  LCHab& from(const Lab& col){

    Real L, C, H;
    L = col[0];
    C = sqrt(col[1] * col[1] + col[2] * col[2]);
    Real h = atan2(col[2], col[1]) * Constants<Real>::_angle;
    if(h < 0.0){ h += 360.0; }
    if(h >= 360.0){ h -= 360.0; }
    H = h;

    Parent::_coords(0) = L;
    Parent::_coords(1) = C;
    Parent::_coords(2) = H;
    return *this;
  }

  LCHab& from(const Luv& col, const Illuminant& rw){

    return from(Lab().from(col, rw));
  }

  LCHab& from(const LCHuv& col, const Illuminant& rw){

    return from(Lab().from(col, rw));
  }

  LCHab& from(const RGB<Real>& col, const Illuminant& rw){

    return from(Lab().from(col, rw));
  }
};

#endif
