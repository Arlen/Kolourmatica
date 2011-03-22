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


#ifndef SPACE_SRGB_HPP
#define SPACE_SRGB_HPP

#include "ForwardDeclarations.hpp"
#include "BaseRGB.hpp"

#include <Eigen/Core>
#include <Eigen/Dense>

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_floating_point.hpp>

using namespace Eigen;
using namespace boost;


template <class Real>
class Space_sRGB : public BaseRGB<Real>{

  BOOST_MPL_ASSERT(( is_floating_point<Real> ));

  typedef ReferenceWhite<Real> RefWhite;
  typedef IlluminantD65<Real> D65;
  typedef Space_XYZ<Real> XYZ;
  typedef Space_xyY<Real> xyY;
  typedef Space_Lab<Real> Lab;
  typedef Space_LCHab<Real> LCHab;
  typedef Space_Luv<Real> Luv;
  typedef Space_LCHuv<Real> LCHuv;
  typedef Space_LinearRGB<Real> LinearRGB;
  typedef Space_sRGB<Real> sRGB;
  typedef Matrix<Real, 3, 1> Vector3;
  typedef Matrix<Real, 3, 3> Matrix3;

  using BaseRGB<Real>::refWhite_;
  using BaseRGB<Real>::gamma_;
  using BaseRGB<Real>::m_;
  using BaseRGB<Real>::m_adapted_;
  using BaseRGB<Real>::m_1_;
  using BaseRGB<Real>::m_1_adapted_;

public:
  Space_sRGB(Real r = 1, Real g = 1, Real b = 1) :
    BaseRGB<Real>(RefWhite(D65()),
		  Real(2.4),
		  xyY(0.64, 0.33, 1.0),
		  xyY(0.30, 0.60, 1.0),
		  xyY(0.15, 0.06, 1.0)){
    tri_ = Vector3(r, g, b);
  }

  Space_sRGB(const Vector3& tri) :
    BaseRGB<Real>(RefWhite(D65()),
		  Real(2.4),
		  xyY(0.64, 0.33, 1.0),
		  xyY(0.30, 0.60, 1.0),
		  xyY(0.15, 0.06, 1.0)){
    tri_ = tri;
  }

  Space_sRGB(const sRGB& other) :
    BaseRGB<Real>(other.refWhite_,
		  other.gamma_,
		  other.m_,
		  other.m_adapted_,
		  other.m_1_,
		  other.m_1_adapted_){

    tri_ = other.tri_;
  }

  const Vector3& position() const{ return tri_; }

  sRGB operator()(const Vector3& tri) const{

    sRGB tmp(*this);
    tmp.tri_ = tri;
    return tmp;
  }

  sRGB operator()(const XYZ& colourSpace) const{

    sRGB tmp(*this);
    Vector3 rgb;
    Real p = 1.0 / gamma_;
    rgb = m_1_adapted_ * colourSpace.position();

    if( rgb(0) > 0.0031308f )
      rgb(0) = 1.055f * pow(rgb(0), p) - 0.055f;
    else
      rgb(0) = 12.92f * rgb(0);

    if( rgb(1) > 0.0031308f )
      rgb(1) = 1.055f * pow(rgb(1), p) - 0.055f;
    else
      rgb(1) = 12.92f * rgb(1);

    if( rgb(2) > 0.0031308f )
      rgb(2) = 1.055f * pow(rgb(2), p) - 0.055f;
    else
      rgb(2) = 12.92f * rgb(2);

    tmp.tri_ = rgb;
    return tmp;
  }

  sRGB operator()(const xyY& colourSpace) const{

    return operator()(XYZ(colourSpace));
  }

  sRGB operator()(const Lab& colourSpace) const{

    return operator()(XYZ(colourSpace));
  }

  sRGB operator()(const LCHab& colourSpace) const{

    return operator()(XYZ(colourSpace));
  }

  sRGB operator()(const Luv& colourSpace) const{

    return operator()(XYZ(colourSpace));
  }

  sRGB operator()(const LCHuv& colourSpace) const{

    return operator()(XYZ(colourSpace));
  }

  sRGB operator()(const LinearRGB& colourSpace) const{

    return operator()(XYZ(colourSpace));
  }

  sRGB operator()(const sRGB& colourSpace) const{

    return operator()(XYZ(colourSpace));
  }

private:
  Vector3 tri_;
};

#endif
