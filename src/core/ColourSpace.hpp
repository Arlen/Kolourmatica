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


#ifndef COLOUR_SPACE_HPP
#define COLOUR_SPACE_HPP

#include "Illuminant.hpp"

#include <Eigen/Core>
#include <Eigen/Dense>

using namespace Eigen;


template <class Real>
struct Constants{

  constexpr static Real _cie_epsilon = Real(216) / Real(24389);
  constexpr static Real _cie_kappa = Real(24389) / Real(27);
  constexpr static Real _cie_kappa_inv = Real(1.0) / _cie_kappa;
  constexpr static Real _cie_ke = _cie_kappa * _cie_epsilon;
  constexpr static Real _pi = Real(M_PI);
  constexpr static Real _radian = _pi / Real(180);
  constexpr static Real _angle = Real(180) / _pi;

  constexpr static Real _116_inv = Real(1.0) / Real(116.0);
  constexpr static Real _200_inv = Real(1.0) / Real(200.0);
  constexpr static Real _500_inv = Real(1.0) / Real(500.0);
};


template <class Real, class Coord>
class ColourSpace{

  typedef BaseIlluminant<Real> Illuminant;

public:
  ColourSpace(){ }

  ColourSpace(const Coord& coords) : _coords(coords){ }

  Real operator[](unsigned i) const{ return _coords(i); }

  Real& operator[](unsigned i){ return _coords(i); }

  const Coord& coords() const{ return _coords; }

  Coord& coords(){ return _coords; }

  virtual Coord to_XYZ(const Illuminant* const rw = nullptr) const = 0;

  virtual Coord& from_XYZ(const Coord& coords,
			  const Illuminant* const rw = nullptr) = 0;

  virtual void referenceWhite(const Illuminant*& rw){ rw = nullptr; }

protected:
  Coord _coords;
};


template <class Colour1, class Colour2>
Colour2 to(const Colour1& col){ Colour2 rt; rt.from(col); return rt; }

template <class Colour1, class Colour2, class Illuminant>
Colour2 to(const Colour1& col, const Illuminant& rw){

  Colour2 rt; rt.from(col, rw); return rt;
}


template <typename Real>
void ComputeUoVo(Real& uo, Real& vo, const Matrix<Real, 3, 1>& rw){

  Real d = Real(1.0) / (rw(0) + Real(15.0) * rw(1) + Real(3.0) * rw(2));
  uo = (Real(4.0) * rw(0)) * d;
  vo = (Real(9.0) * rw(1)) * d;
}

#endif 
