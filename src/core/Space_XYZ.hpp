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


#ifndef SPACEXYZ_HPP
#define SPACEXYZ_HPP

#include "ForwardDeclarations.hpp"

#include "../../../eigen/Eigen/Core"
#include "../../../eigen/Eigen/Dense"

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_floating_point.hpp>

using namespace Eigen;
using namespace boost;


template <class Real>
class Space_XYZ{

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
  Space_XYZ() : tri_(Vector3::Ones()){ }
  Space_XYZ(const Real& x, const Real& y, const Real& z) : 
    tri_(Vector3(x, y, z)){ }
  Space_XYZ(const Vector3& tri) : tri_(tri){ }
  Space_XYZ(const XYZ& colourSpace) : tri_(colourSpace.tri_){ }
  Space_XYZ(const xyY& colourSpace) : tri_(operator()(colourSpace).tri_){ }
  Space_XYZ(const Lab& colourSpace) : tri_(operator()(colourSpace).tri_){ }
  Space_XYZ(const LCHab& colourSpace) : tri_(operator()(colourSpace).tri_){ }
  Space_XYZ(const Luv& colourSpace) : tri_(operator()(colourSpace).tri_){ }
  Space_XYZ(const LCHuv& colourSpace) : tri_(operator()(colourSpace).tri_){ }
  Space_XYZ(const LinearRGB& colourSpace) : tri_(operator()(colourSpace).tri_){}
  Space_XYZ(const sRGB& colourSpace) : tri_(operator()(colourSpace).tri_){ }

  const Vector3& position() const{ return tri_; }

  XYZ operator()(const Vector3& tri) const{

    return XYZ(tri);
  }

  XYZ operator()(const XYZ& colourSpace) const{

    return XYZ(colourSpace);
  }

  XYZ operator()(const xyY& colourSpace) const{

    const Vector3& tri = colourSpace.position();
    return XYZ(tri(0) / tri(1) *  tri(2),
	       tri(2),
	       (1.0 - tri(0) - tri(1)) / tri(1) * tri(2));
  }

  XYZ operator()(const Lab& colourSpace) const{

    const Vector3& tri = colourSpace.position();
    Real fx, fy, fz, fxCube, fzCube, xr, yr, zr;
    fy = (tri(0) + 16.0) / 116.0;
    fx = (tri(1) / 500.0) + fy;
    fz = fy - (tri(2) / 200.0);
    fxCube = fx * fx * fx;
    fzCube = fz * fz * fz;

    if(fxCube > Constants<Real>::CIE_epsilon_)
      xr = fxCube;
    else
      xr = ((116.0 * fx) - 16.0) / Constants<Real>::CIE_kappa_;

    if(tri(0, 0) > Constants<Real>::CIE_ke_)
      yr = pow( (tri(0) + 16.0) / 116.0, 3.0 );
    else
      yr = tri(0) / Constants<Real>::CIE_kappa_;

    if(fzCube > Constants<Real>::CIE_epsilon_)
      zr = fzCube;
    else
      zr = ((116.0 * fz) - 16.0) / Constants<Real>::CIE_kappa_;

    return XYZ(xr * GRW::value_.position()(0),
	       yr * GRW::value_.position()(1),
	       zr * GRW::value_.position()(2));
  }

  XYZ operator()(const LCHab& colourSpace) const{

    return operator()(Lab(colourSpace));
  }

  XYZ operator()(const Luv& colourSpace) const{

    const Vector3& tri = colourSpace.position();
    Real a, b, c, d, uo, vo, x, y, z;
    Helper<Real>::ComputeUoVo(uo, vo, GRW::value_.position());
    c = -1.0 / 3.0;
    a = (((52.0 * tri(0)) /
	  (tri(1) + 13.0 * tri(0) * uo)) - 1.0) / 3.0;
    if( tri(0) > Constants<Real>::CIE_ke_ )
      y = pow( (tri(0) + 16.0) / 116.0, 3.0 );
    else
      y = tri(0) / Constants<Real>::CIE_kappa_;
   
    b = -5.0 * y;
    d = (((39.0 * tri(0)) /
	  (tri(2) + 13.0 * tri(0) * vo)) - 5.0) * y;
    x = (d - b) / (a - c);
    z = x * a + b;
    return XYZ(x, y, z);
  }

  XYZ operator()(const LCHuv& colourSpace) const{

    return operator()(Luv(colourSpace));
  }

  XYZ operator()(const LinearRGB& colourSpace) const{

    Vector3 tri;
    if(colourSpace.position()(0) < 0.0)
      tri(0) = pow(-colourSpace.position()(0), colourSpace.gamma()) * -1.0;
    else
      tri(0) = pow(colourSpace.position()(0), colourSpace.gamma());

    if(colourSpace.position()(1) < 0.0)
      tri(1) = pow(-colourSpace.position()(1), colourSpace.gamma()) * -1.0;
    else
      tri(1) = pow(colourSpace.position()(1), colourSpace.gamma());

    if(colourSpace.position()(2) < 0.0)
      tri(2) = pow(-colourSpace.position()(2), colourSpace.gamma()) * -1.0;
    else
      tri(2) = pow(colourSpace.position()(2), colourSpace.gamma());

    return XYZ(colourSpace.m_adapted() * tri);
  }

  XYZ operator()(const sRGB& colourSpace) const{

    Vector3 tri;
    if( colourSpace.position()(0) > 0.04045f )
      tri(0) = pow( ((colourSpace.position()(0) + 0.055f) / 1.055f),
		    colourSpace.gamma() );
    else
      tri(0) = colourSpace.position()(0) / 12.92f;

    if( colourSpace.position()(1) > 0.04045f )
      tri(1) = pow( ((colourSpace.position()(1) + 0.055f) / 1.055f),
		    colourSpace.gamma() );
    else
      tri(1) = colourSpace.position()(1) / 12.92f;

    if( colourSpace.position()(2) > 0.04045f )
      tri(2) = pow( ((colourSpace.position()(2) + 0.055f) / 1.055f),
		    colourSpace.gamma() );
    else
      tri(2) = colourSpace.position()(2) / 12.92f;

    return XYZ(colourSpace.m_adapted() * tri);
  }

private:
  Vector3 tri_;
};


#endif
