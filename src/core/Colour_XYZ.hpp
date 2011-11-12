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


#ifndef COLOUR_XYZ_HPP
#define COLOUR_XYZ_HPP

#include "ColourSpace.hpp"
#include "ForwardDeclarations.hpp"
#include "Illuminant.hpp"

#include <Eigen/Core>
#include <Eigen/Dense>

using namespace Eigen;


template <class Real>
class Colour_XYZ : public ColourSpace<Real, Matrix<Real, 3, 1> >{

  typedef Colour_XYZ<Real> XYZ;
  typedef Colour_xyY<Real> xyY;
  typedef Colour_Lab<Real> Lab;
  typedef Colour_LCHab<Real> LCHab;
  typedef Colour_Luv<Real> Luv;
  typedef Colour_LCHuv<Real> LCHuv;
  typedef BaseIlluminant<Real> Illuminant;
  typedef Matrix<Real, 3, 1> Coord3;

public:
  Colour_XYZ(const Coord3& tri) : ColourSpace<Real, Coord3>{tri}{ }

  Colour_XYZ(const Real X = 1, const Real Y = 1, const Real Z = 1) :
    ColourSpace<Real, Coord3>{Coord3(X, Y, Z)}{ }

  Colour_XYZ(const XYZ& col) : ColourSpace<Real, Coord3>{col._coords}{ }


  XYZ& from(const xyY& col){

    /*
      X = (x * Y) / y
      Y = Y
      Z = ((1 - x - y) * Y) / y
     */

    Real y_inv = 1.0 / col[1];
    this->_coords(0) = col[0] * col[2] * y_inv;
    this->_coords(1) = col[2];
    this->_coords(2) = (1.0 - col[0] - col[1]) * col[2] * y_inv;
    return *this;
  }

  XYZ& from(const Lab& col, const Illuminant& rw){

    Real fx, fy, fz, fxCube, fzCube, xr, yr, zr;
    fy = (col[0] + 16.0) * Constants<Real>::_116_inv;
    fx = (col[1] / 500.0) + fy;
    fz = fy - (col[2] / 200.0);
    fxCube = fx * fx * fx;
    fzCube = fz * fz * fz;

    if(fxCube > Constants<Real>::_cie_epsilon)
      xr = fxCube;
    else
      xr = ((116.0 * fx) - 16.0) / Constants<Real>::_cie_kappa;

    if(col[0] > Constants<Real>::_cie_ke){
      Real tmp = (col[0] + 16.0) * Constants<Real>::_116_inv;
      yr = tmp * tmp * tmp;
    }else{
      yr = col[0] / Constants<Real>::_cie_kappa;
    }

    if(fzCube > Constants<Real>::_cie_epsilon)
      zr = fzCube;
    else
      zr = ((116.0 * fz) - 16.0) / Constants<Real>::_cie_kappa;

    this->_coords(0) = xr * rw.colour_XYZ()[0];
    this->_coords(1) = yr * rw.colour_XYZ()[1];
    this->_coords(2) = zr * rw.colour_XYZ()[2];
    return *this;
  }

  XYZ& from(const LCHab& col, const Illuminant& rw){

    return from(Lab().from(col), rw);
  }

  XYZ& from(const Luv& col, const Illuminant& rw){

    Real a, b, c, d, uo, vo, x, y, z;
    ComputeUoVo<Real, Coord3>(uo, vo, rw.colour_XYZ().coords());

    c = -1.0 / 3.0;
    a = (((52.0 * col[0]) /
	  (col[1] + 13.0 * col[0] * uo)) - 1.0) / 3.0;
    if( col[0] > Constants<Real>::_cie_ke )
      y = pow( (col[0] + 16.0) * Constants<Real>::_116_inv, 3.0 );
    else
      y = col[0] * Constants<Real>::_cie_kappa_inv;
   
    b = -5.0 * y;
    d = (((39.0 * col[0]) /
	  (col[2] + 13.0 * col[0] * vo)) - 5.0) * y;
    x = (d - b) / (a - c);
    z = x * a + b;

    this->_coords(0) = x;
    this->_coords(1) = y;
    this->_coords(2) = z;
    return *this;
  }

  XYZ& from(const LCHuv& col, const Illuminant& rw){

    return from(Luv().from(col), rw);
  }

  XYZ& from(const RGB<Real>& col){

    Coord3 tri = col.inverseCompanding(col.gamma(), col.coords());
    this->_coords = col.m_adapted() * tri;
    return *this;
  }


  template <class Colour>
  Colour to() const{ return Colour().from(*this); }

  template <class Colour>
  Colour to(const Illuminant& rw) const{ return Colour().from(*this, rw); }
};

#endif
