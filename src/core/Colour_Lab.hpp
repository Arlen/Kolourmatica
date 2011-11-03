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


#ifndef COLOUR_LAB_HPP
#define COLOUR_LAB_HPP

#include "ColourSpace.hpp"
#include "ForwardDeclarations.hpp"
#include "Illuminant.hpp"

#include <Eigen/Core>
#include <Eigen/Dense>

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_floating_point.hpp>

using namespace Eigen;
using namespace boost;


template <class Real>
class Colour_Lab : public ColourSpace<Real, Matrix<Real, 3, 1> >{

  BOOST_MPL_ASSERT(( is_floating_point<Real> ));

  typedef Colour_XYZ<Real> XYZ;
  typedef Colour_xyY<Real> xyY;
  typedef Colour_Lab<Real> Lab;
  typedef Colour_LCHab<Real> LCHab;
  typedef Colour_Luv<Real> Luv;
  typedef Colour_LCHuv<Real> LCHuv;
  typedef BaseIlluminant<Real> Illuminant;
  typedef Matrix<Real, 3, 1> Coord3;

public:
  Colour_Lab(const Coord3& tri) : ColourSpace<Real, Coord3>{tri}{ }

  Colour_Lab(const Real L = 100, const Real a = 0, const Real b = 0) :
    ColourSpace<Real, Coord3>{Coord3(L, a, b)}{ }

  Colour_Lab(const Lab& col) : ColourSpace<Real, Coord3>{col._coords}{ }


  const Lab& from(const XYZ& col, const Illuminant& rw){

    Real xr, yr, zr, fx, fy, fz;
    xr = col[0] / rw.colour_XYZ()[0];
    yr = col[1] / rw.colour_XYZ()[1];
    zr = col[2] / rw.colour_XYZ()[2];

    if( xr > Constants<Real>::_cie_epsilon )
      fx = pow(xr, Real(1.0/3.0));
    else
      fx = ((Constants<Real>::_cie_kappa * xr) + 16.0) / 116.0;

    if( yr > Constants<Real>::_cie_epsilon )
      fy = pow(yr, Real(1.0/3.0));
    else
      fy = ((Constants<Real>::_cie_kappa * yr) + 16.0) / 116.0;

    if( zr > Constants<Real>::_cie_epsilon )
      fz = pow(zr, Real(1.0/3.0));
    else
      fz = ((Constants<Real>::_cie_kappa * zr) + 16.0) / 116.0;

    this->_coords(0) = 116.0 * fy - 16.0;
    this->_coords(1) = 500.0 * (fx - fy);
    this->_coords(2) = 200.0 * (fy - fz);
    return *this;
  }

  const Lab& from(const xyY& col, const Illuminant& rw){

    return from(XYZ().from(col), rw);
  }

  const Lab& from(const LCHab& col){

    this->_coords(0) = col[0];
    this->_coords(1) = col[1] * cos(col[2] * Constants<Real>::_radian);
    this->_coords(2) = col[1] * sin(col[2] * Constants<Real>::_radian);
    return *this;
  }

  const Lab& from(const Luv& col, const Illuminant& rw){

    return from(XYZ().from(col, rw), rw);
  }

  const Lab& from(const LCHuv& col, const Illuminant& rw){

    return from(XYZ().from(col, rw), rw);
  }

  const Lab& from(const RGB<Real>& col, const Illuminant& rw){

    return from(XYZ().from(col), rw);
  }


  template <class Colour>
  Colour to() const{ return Colour().from(*this); }

  template <class Colour>
  Colour to(const Illuminant& rw) const{ return Colour().from(*this, rw); }
};

#endif
