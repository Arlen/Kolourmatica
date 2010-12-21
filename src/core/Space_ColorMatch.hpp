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


#ifndef SPACE_COLORMATCH_HPP
#define SPACE_COLORMATCH_HPP

#include "ReferenceWhite.hpp"
#include "Space_xyY.hpp"
#include "Space_LinearRGB.hpp"

#include "../eigen/Eigen/Core"
#include "../eigen/Eigen/Dense"

#include <boost/mpl/bool.hpp>
#include <boost/mpl/assert.hpp>


template <class Real>
class Space_ColorMatch : public Space_LinearRGB<Real>{

  BOOST_MPL_ASSERT(( is_floating_point<Real> ));

  typedef ReferenceWhite<Real> RefWhite;
  typedef IlluminantD50<Real> D50;
  typedef Space_xyY<Real> xyY;
  typedef Matrix<Real, 3, 1> Vector3;

public:
  Space_ColorMatch(Real r, Real g, Real b) :
    Space_LinearRGB<Real>(RefWhite(D50()),
			  Real(1.8),
			  xyY(0.630, 0.340, 1.0),
			  xyY(0.295, 0.605, 1.0),
			  xyY(0.150, 0.075, 1.0),
			  Vector3(r, g, b)){ }
};

#endif
