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


#ifndef SPACELINEARRGB_HPP
#define SPACELINEARRGB_HPP

#include "ForwardDeclarations.hpp"
#include "BaseRGB.hpp"

#include <Eigen/Core>
#include <Eigen/Dense>

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_floating_point.hpp>

using namespace Eigen;
using namespace boost;


template <class Real>
class Space_LinearRGB : public BaseRGB<Real>{

  BOOST_MPL_ASSERT(( is_floating_point<Real> ));

  typedef ReferenceWhite<Real> RefWhite;
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
  const Vector3& position() const{ return tri_; }

  LinearRGB operator()(const Vector3& tri) const{

    LinearRGB tmp(*this);
    tmp.tri_ = tri;
    return tmp;
  }

  LinearRGB operator()(const XYZ& colourSpace) const{

    LinearRGB tmp(*this);
    Vector3 rgb;
    Real p = 1.0 / gamma_;
    rgb = m_1_adapted_ * colourSpace.position();

    if(rgb(0) < 0.0)
      rgb(0) = pow(-rgb(0), p) * -1.0;
    else
      rgb(0) = pow(rgb(0), p);

    if(rgb(1) < 0.0)
      rgb(1) = pow(-rgb(1), p) * -1.0;
    else
      rgb(1) = pow(rgb(1), p);

    if(rgb(2) < 0.0)
      rgb(2) = pow(-rgb(2), p) * -1.0; 
    else
      rgb(2) = pow(rgb(2), p);

    tmp.tri_ = rgb;
    return tmp;
  }

  LinearRGB operator()(const xyY& colourSpace) const{

    return operator()(XYZ(colourSpace));
  }

  LinearRGB operator()(const Lab& colourSpace) const{

    return operator()(XYZ(colourSpace));
  }

  LinearRGB operator()(const LCHab& colourSpace) const{

    return operator()(XYZ(colourSpace));
  }

  LinearRGB operator()(const Luv& colourSpace) const{

    return operator()(XYZ(colourSpace));
  }

  LinearRGB operator()(const LCHuv& colourSpace) const{

    return operator()(XYZ(colourSpace));
  }

  LinearRGB operator()(const LinearRGB& colourSpace) const{

    return operator()(XYZ(colourSpace));
  }

  LinearRGB operator()(const sRGB& colourSpace) const{

    return operator()(XYZ(colourSpace));
  }

protected:
  Space_LinearRGB(const RefWhite& rw,
		  const Real& gamma,
		  const xyY& redPrimary,
		  const xyY& greenPrimary,
		  const xyY& bluePrimary,
		  const Vector3& tri) :
    BaseRGB<Real>(rw, gamma, redPrimary, greenPrimary, bluePrimary),
    tri_(tri){ }

  Space_LinearRGB(const RefWhite& rw,
		  const Real& gamma,
		  const xyY& redPrimary,
		  const xyY& greenPrimary,
		  const xyY& bluePrimary,
		  const XYZ& colourSpace) :
    BaseRGB<Real>(rw, gamma, redPrimary, greenPrimary, bluePrimary),
    tri_(operator()(colourSpace).tri_){ }

  Space_LinearRGB(const RefWhite& rw,
		  const Real& gamma,
		  const xyY& redPrimary,
		  const xyY& greenPrimary,
		  const xyY& bluePrimary,
		  const xyY& colourSpace) :
    BaseRGB<Real>(rw, gamma, redPrimary, greenPrimary, bluePrimary),
    tri_(operator()(colourSpace).tri_){ }

  Space_LinearRGB(const RefWhite& rw,
		  const Real& gamma,
		  const xyY& redPrimary,
		  const xyY& greenPrimary,
		  const xyY& bluePrimary,
		  const Lab& colourSpace) :
    BaseRGB<Real>(rw, gamma, redPrimary, greenPrimary, bluePrimary),
    tri_(operator()(colourSpace).tri_){ }

  Space_LinearRGB(const RefWhite& rw,
		  const Real& gamma,
		  const xyY& redPrimary,
		  const xyY& greenPrimary,
		  const xyY& bluePrimary,
		  const LCHab& colourSpace) :
    BaseRGB<Real>(rw, gamma, redPrimary, greenPrimary, bluePrimary),
    tri_(operator()(colourSpace).tri_){ }

  Space_LinearRGB(const RefWhite& rw,
		  const Real& gamma,
		  const xyY& redPrimary,
		  const xyY& greenPrimary,
		  const xyY& bluePrimary,
		  const Luv& colourSpace) :
    BaseRGB<Real>(rw, gamma, redPrimary, greenPrimary, bluePrimary),
    tri_(operator()(colourSpace).tri_){ }

  Space_LinearRGB(const RefWhite& rw,
		  const Real& gamma,
		  const xyY& redPrimary,
		  const xyY& greenPrimary,
		  const xyY& bluePrimary,
		  const LCHuv& colourSpace) :
    BaseRGB<Real>(rw, gamma, redPrimary, greenPrimary, bluePrimary),
    tri_(operator()(colourSpace).tri_){ }

  Space_LinearRGB(const RefWhite& rw,
		  const Real& gamma,
		  const xyY& redPrimary,
		  const xyY& greenPrimary,
		  const xyY& bluePrimary,
		  const LinearRGB& colourSpace) :
    BaseRGB<Real>(rw, gamma, redPrimary, greenPrimary, bluePrimary),
    tri_(operator()(colourSpace).tri_){ }

  Space_LinearRGB(const RefWhite& rw,
		  const Real& gamma,
		  const xyY& redPrimary,
		  const xyY& greenPrimary,
		  const xyY& bluePrimary,
		  const sRGB& colourSpace) :
    BaseRGB<Real>(rw, gamma, redPrimary, greenPrimary, bluePrimary),
    tri_(operator()(colourSpace).tri_){ }

  Space_LinearRGB(const LinearRGB& other) :
    BaseRGB<Real>(other.refWhite_,
		  other.gamma_,
		  other.m_,
		  other.m_adapted_,
		  other.m_1_,
		  other.m_1_adapted_){

    tri_ = other.tri_;
  }

private:
  Vector3 tri_;
};

#endif
