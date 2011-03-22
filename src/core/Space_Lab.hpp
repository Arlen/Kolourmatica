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


#ifndef SPACELAB_HPP
#define SPACELAB_HPP

#include "ForwardDeclarations.hpp"

#include <Eigen/Core>
#include <Eigen/Dense>

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_floating_point.hpp>

using namespace Eigen;
using namespace boost;


template <class Real>
class Space_Lab{

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
  Space_Lab() : tri_(Vector3::Ones()){ }
  Space_Lab(const Real& L, const Real& a, const Real& b) :
    tri_(Vector3(L, a, b)){ }
  Space_Lab(const Vector3& tri) : tri_(tri){ }
  Space_Lab(const XYZ& colourSpace) : tri_(operator()(colourSpace).tri_){ }
  Space_Lab(const xyY& colourSpace) : tri_(operator()(colourSpace).tri_){ }
  Space_Lab(const Lab& colourSpace) : tri_(colourSpace.tri_){ }
  Space_Lab(const LCHab& colourSpace) : tri_(operator()(colourSpace).tri_){ }
  Space_Lab(const Luv& colourSpace) : tri_(operator()(colourSpace).tri_){ }
  Space_Lab(const LCHuv& colourSpace) : tri_(operator()(colourSpace).tri_){ }
  Space_Lab(const LinearRGB& colourSpace) : tri_(operator()(colourSpace).tri_){}
  Space_Lab(const sRGB& colourSpace) : tri_(operator()(colourSpace).tri_){ }

  const Vector3& position() const{ return tri_; }

  Lab operator()(const Vector3& tri) const{

    return Lab(tri);
  }

  Lab operator()(const XYZ& colourSpace) const{

    const Vector3& tri = colourSpace.position();
    Real xr, yr, zr, fx, fy, fz;
    xr = tri(0) / GRW::value_.position()(0);
    yr = tri(1) / GRW::value_.position()(1);
    zr = tri(2) / GRW::value_.position()(2);

    if( xr > Constants<Real>::CIE_epsilon_ )
      fx = pow(xr, 1.0/3.0);
    else
      fx = ((Constants<Real>::CIE_kappa_ * xr) + 16.0) / 116.0;

    if( yr > Constants<Real>::CIE_epsilon_ )
      fy = pow(yr, 1.0/3.0);
    else
      fy = ((Constants<Real>::CIE_kappa_ * yr) + 16.0) / 116.0;

    if( zr > Constants<Real>::CIE_epsilon_ )
      fz = pow(zr, 1.0/3.0);
    else
      fz = ((Constants<Real>::CIE_kappa_ * zr) + 16.0) / 116.0;

    return Lab(Vector3(116.0 * fy - 16.0,
		       500.0 * (fx - fy),
		       200.0 * (fy - fz)));
  }

  Lab operator()(const xyY& colourSpace) const{

    return operator()(XYZ(colourSpace));
  }

  Lab operator()(const Lab& colourSpace) const{

    return Lab(colourSpace);
  }

  Lab operator()(const LCHab& colourSpace) const{

    const Vector3& tri = colourSpace.position();

    return Lab(tri(0),
	       tri(1) * cos(tri(2) * Constants<Real>::radian_),
	       tri(1) * sin(tri(2) * Constants<Real>::radian_));
  }

  Lab operator()(const Luv& colourSpace) const{

    return operator()(XYZ(colourSpace));
  }

  Lab operator()(const LCHuv& colourSpace) const{

    return operator()(XYZ(colourSpace));
  }

  Lab operator()(const LinearRGB& colourSpace) const{

    return operator()(XYZ(colourSpace));
  }

  Lab operator()(const sRGB& colourSpace) const{

    return operator()(XYZ(colourSpace));
  }

private:
  Vector3 tri_;
};

#endif
