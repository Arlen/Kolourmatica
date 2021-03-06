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


#ifndef COLOUR_XYY_HPP
#define COLOUR_XYY_HPP

#include "ColourSpace.hpp"
#include "ForwardDeclarations.hpp"
#include "Illuminant.hpp"

#include <Eigen/Core>
#include <Eigen/Dense>

using namespace Eigen;


template <class Real>
class Colour_xyY : public ColourSpace<Real, Matrix<Real, 3, 1> >{

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
  Colour_xyY(const Coord3& tri) : ColourSpace<Real, Coord3>{tri}{ }

  Colour_xyY(const Real X = 1, const Real Y = 1, const Real Z = 1) :
    ColourSpace<Real, Coord3>{Coord3(X, Y, Z)}{ }

  Colour_xyY(const xyY& col) : ColourSpace<Real, Coord3>{col._coords}{ }


  Coord3 to_XYZ(const Illuminant* const rw = nullptr) const{

    XYZ xyz; xyz.from(*this); return xyz.coords();
  }

  Coord3& from_XYZ(const Coord3& coords, const Illuminant* const rw = nullptr){

    this->from(XYZ(coords)); return Parent::_coords;
  }


  xyY& from(const XYZ& col){

    Real sum = col.coords().sum();
    Parent::_coords(0) = col[0] / sum;
    Parent::_coords(1) = col[1] / sum;
    Parent::_coords(2) = col[1];
    return *this;
  }

  xyY& from(const Lab& col, const Illuminant& rw){

    return from(XYZ().from(col, rw));
  }

  xyY& from(const LCHab& col, const Illuminant& rw){

    return from(XYZ().from(col, rw));
  }

  xyY& from(const Luv& col, const Illuminant& rw){

    return from(XYZ().from(col, rw));
  }

  xyY& from(const LCHuv& col, const Illuminant& rw){

    return from(XYZ().from(col, rw));
  }

  xyY& from(const RGB<Real>& col){

    return from(XYZ().from(col));
  }
};

#endif
