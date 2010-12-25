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


#ifndef SPACELCHAB_HPP
#define SPACELCHAB_HPP

#include "ForwardDeclarations.hpp"

#include "../../../eigen/Eigen/Core"
#include "../../../eigen/Eigen/Dense"

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_floating_point.hpp>

using namespace Eigen;
using namespace boost;


template <class Real>
class Space_LCHab{

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
  Space_LCHab() : tri_(Vector3::Ones()){ }
  Space_LCHab(const Real& L, const Real& C, const Real& H) :
    tri_(Vector3(L, C, H)){ }
  Space_LCHab(const Vector3& tri) : tri_(tri){ }
  Space_LCHab(const XYZ& colourSpace) : tri_(operator()(colourSpace).tri_){ }
  Space_LCHab(const xyY& colourSpace) : tri_(operator()(colourSpace).tri_){ }
  Space_LCHab(const Lab& colourSpace) : tri_(operator()(colourSpace).tri_){ }
  Space_LCHab(const LCHab& colourSpace) : tri_(colourSpace.tri_){ }
  Space_LCHab(const Luv& colourSpace) : tri_(operator()(colourSpace).tri_){ }
  Space_LCHab(const LCHuv& colourSpace) : tri_(operator()(colourSpace).tri_){ }
  Space_LCHab(const LinearRGB& colourSpace) :
    tri_(operator()(colourSpace).tri_){ }
  Space_LCHab(const sRGB& colourSpace) : tri_(operator()(colourSpace).tri_){ }

  const Vector3& position() const{ return tri_; }

  LCHab operator()(const XYZ& colourSpace) const{

    return operator()(Lab(colourSpace));
  }

  LCHab operator()(const xyY& colourSpace) const{

    return operator()(XYZ(colourSpace));
  }

  LCHab operator()(const Lab& colourSpace) const{

    const Vector3& tri = colourSpace.position();
    Real l, c, h;

    l = tri(0);
    c = sqrt(tri(1) * tri(1) + tri(2) * tri(2));
    float H = atan2(tri(2), tri(1)) * Constants<Real>::angle_;
    if(H < 0.0){ H += 360.0; }
    if(H >= 360.0){ H -= 360.0; }
    h = H;
    return LCHab(l, c, h);
  }

  LCHab operator()(const LCHab& colourSpace) const{

    return LCHab(colourSpace);
  }

  LCHab operator()(const Luv& colourSpace) const{

    return operator()(XYZ(colourSpace));
  }

  LCHab operator()(const LCHuv& colourSpace) const{

    return operator()(XYZ(colourSpace));
  }

  LCHab operator()(const LinearRGB& colourSpace) const{

    return operator()(XYZ(colourSpace));
  }

  LCHab operator()(const sRGB& colourSpace) const{

    return operator()(XYZ(colourSpace));
  }

private:
  Vector3 tri_;
};

#endif
