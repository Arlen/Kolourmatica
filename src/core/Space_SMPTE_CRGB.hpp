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


#ifndef SPACE_SMPTE_CRGB_HPP
#define SPACE_SMPTE_CRGB_HPP

#include "ForwardDeclarations.hpp"
#include "Space_LinearRGB.hpp"

#include "../../../eigen/Eigen/Core"
#include "../../../eigen/Eigen/Dense"

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_floating_point.hpp>

using namespace Eigen;
using namespace boost;


template <class Real>
class Space_SMPTE_CRGB : public Space_LinearRGB<Real>{

  BOOST_MPL_ASSERT(( is_floating_point<Real> ));

  typedef ReferenceWhite<Real> RefWhite;
  typedef IlluminantD65<Real> D65;
  typedef Space_xyY<Real> xyY;
  typedef Space_LinearRGB<Real> LinearRGB;
  typedef Matrix<Real, 3, 1> Vector3;

public:
  Space_SMPTE_CRGB(Real r = 1, Real g = 1, Real b = 1) :
    Space_LinearRGB<Real>(RefWhite(D65()),
			  Real(2.2),
			  xyY(0.630, 0.340, 1.0),
			  xyY(0.310, 0.595, 1.0),
			  xyY(0.155, 0.070, 1.0),
			  Vector3(r, g, b)){ }

  Space_SMPTE_CRGB(const Vector3& tri) :
    Space_LinearRGB<Real>(RefWhite(D65()),
			  Real(2.2),
			  xyY(0.630, 0.340, 1.0),
			  xyY(0.310, 0.595, 1.0),
			  xyY(0.155, 0.070, 1.0),
			  tri){ }

  Space_SMPTE_CRGB(const LinearRGB& other) :
    Space_LinearRGB<Real>(other){ }
};

#endif