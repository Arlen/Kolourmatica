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


#ifndef SPACELUV_HPP
#define SPACELUV_HPP

#include "ForwardDeclarations.hpp"

#include "../../../eigen/Eigen/Core"
#include "../../../eigen/Eigen/Dense"

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_floating_point.hpp>

using namespace Eigen;
using namespace boost;


template <class Real>
class Space_Luv{

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
  Space_Luv() : tri_(Vector3::Ones()){ }
  Space_Luv(const Real& L, const Real& a, const Real& b) :
    tri_(Vector3(L, a, b)){ }
  Space_Luv(const Vector3& tri) : tri_(tri){ }
  Space_Luv(const XYZ& colourSpace) : tri_(operator()(colourSpace).tri_){ }
  Space_Luv(const xyY& colourSpace) : tri_(operator()(colourSpace).tri_){ }
  Space_Luv(const Lab& colourSpace) : tri_(operator()(colourSpace).tri_){ }
  Space_Luv(const LCHab& colourSpace) : tri_(operator()(colourSpace).tri_){ }
  Space_Luv(const Luv& colourSpace) : tri_(colourSpace.tri_){ }
  Space_Luv(const LCHuv& colourSpace) : tri_(operator()(colourSpace).tri_){ }
  Space_Luv(const LinearRGB& colourSpace) : tri_(operator()(colourSpace).tri_){}
  Space_Luv(const sRGB& colourSpace) : tri_(operator()(colourSpace).tri_){ }

  const Vector3& position() const{ return tri_; }

  Luv operator()(const Vector3& tri) const{

    return Luv(tri);
  }

  Luv operator()(const XYZ& colourSpace) const{

    const Vector3& tri = colourSpace.position();
    Real yr, up, vp, urp, vrp, l, u, v;

    yr = tri(1) / GRW::value_.position()(1);
    Helper<Real>::ComputeUoVo(up, vp, tri);
    Helper<Real>::ComputeUoVo(urp, vrp, GRW::value_.position());
    if( yr > Constants<Real>::CIE_epsilon_ )
      l = 116.0 * pow(yr, 1.0/3.0) - 16.0;
    else
      l = Constants<Real>::CIE_kappa_ * yr;

    u = 13.0f * l * (up - urp);
    v = 13.0f * l * (vp - vrp);
    return Luv(l, u, v);
  }

  Luv operator()(const xyY& colourSpace) const{

    return operator()(XYZ(colourSpace));
  }

  Luv operator()(const Lab& colourSpace) const{

    return operator()(XYZ(colourSpace));
  }

  Luv operator()(const LCHab& colourSpace) const{

    return operator()(XYZ(colourSpace));
  }

  Luv operator()(const Luv& colourSpace) const{

    return Luv(colourSpace);
  }

  Luv operator()(const LCHuv& colourSpace) const{

    const Vector3& tri = colourSpace.position();
    return Luv(tri(0),
	       tri(1) * cos(tri(2) * Constants<Real>::radian_),
	       tri(1) * sin(tri(2) * Constants<Real>::radian_));
  }

  Luv operator()(const LinearRGB& colourSpace) const{

    return operator()(XYZ(colourSpace));
  }

  Luv operator()(const sRGB& colourSpace) const{

    return operator()(XYZ(colourSpace));
  }

private:
  Vector3 tri_;
};

#endif
