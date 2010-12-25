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


#ifndef SPACEXYY_HPP
#define SPACEXYY_HPP

#include "ForwardDeclarations.hpp"

#include "../../../eigen/Eigen/Core"
#include "../../../eigen/Eigen/Dense"

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_floating_point.hpp>

using namespace Eigen;
using namespace boost;


template <class Real>
class Space_xyY{

  BOOST_MPL_ASSERT(( is_floating_point<Real> ));

  typedef GlobalReferenceWhite<Real> GRW;
  typedef Space_XYZ<Real> XYZ;
  typedef Space_xyY<Real> xyY;
  typedef Space_Lab<Real> Lab;
  typedef Space_LCHab<Real> LCHab;
  typedef Space_Luv<Real> Luv;
  typedef Space_LCHuv<Real> LCHuv;
  typedef Space_LinearRGB<Real> LinearRGB;
  typedef Space_sRGB<Real> sRGB;
  typedef Matrix<Real, 3, 1> Vector3;

public:
  Space_xyY() : tri_(Vector3::Ones()){ }
  Space_xyY(const Real& x, const Real& y, const Real& Y) :
    tri_(Vector3(x, y, Y)){ }
  Space_xyY(const Vector3& tri) : tri_(tri){ }
  Space_xyY(const XYZ& colourSpace) : tri_(operator()(colourSpace).tri_){ }
  Space_xyY(const xyY& colourSpace) : tri_(colourSpace.tri_){ }
  Space_xyY(const Lab& colourSpace) : tri_(operator()(colourSpace).tri_){ }
  Space_xyY(const LCHab& colourSpace) : tri_(operator()(colourSpace).tri_){ }
  Space_xyY(const Luv& colourSpace) : tri_(operator()(colourSpace).tri_){ }
  Space_xyY(const LCHuv& colourSpace) : tri_(operator()(colourSpace).tri_){ }
  Space_xyY(const LinearRGB& colourSpace) : tri_(operator()(colourSpace).tri_){}
  Space_xyY(const sRGB& colourSpace) : tri_(operator()(colourSpace).tri_){ }

  const Vector3& position() const{ return tri_; }

  xyY operator()(const XYZ& colourSpace) const{

    const Vector3& tri = colourSpace.position();
    Real sum = tri(0) + tri(1) + tri(2);
    return xyY(tri(0) / sum, tri(1) / sum, tri(1));
  }

  xyY operator()(const xyY& colourSpace) const{

    return xyY(colourSpace);
  }

  xyY operator()(const Lab& colourSpace) const{

    return operator()(XYZ(colourSpace));
  }

  xyY operator()(const LCHab& colourSpace) const{

    return operator()(Lab(colourSpace));
  }

  xyY operator()(const Luv& colourSpace) const{

    return operator()(XYZ(colourSpace));
  }

  xyY operator()(const LCHuv& colourSpace) const{

    return operator()(Luv(colourSpace));
  }

  xyY operator()(const LinearRGB& colourSpace) const{

    return operator()(XYZ(colourSpace));
  }

  xyY operator()(const sRGB& colourSpace) const{

    return operator()(XYZ(colourSpace));
  }

private:
  Vector3 tri_;
};

#endif
