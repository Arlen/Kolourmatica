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


#ifndef BASERGB_HPP
#define BASERGB_HPP

#include "ReferenceWhite.hpp"
#include "Space_xyY.hpp"

#include "../eigen/Eigen/Core"
#include "../eigen/Eigen/Dense"

#include <boost/mpl/bool.hpp>
#include <boost/mpl/assert.hpp>


template <class Real>
class BaseRGB{

  BOOST_MPL_ASSERT(( is_floating_point<Real> ));

  typedef ReferenceWhite<Real> RefWhite;
  typedef Space_xyY<Real> xyY;
  typedef Matrix<Real, 3, 1> Vector3;
  typedef Matrix<Real, 3, 3> Matrix3;

public:
  const Matrix3& m() const{ return m_; }
  const Matrix3& m_adapted() const{ return m_adapted_; }
  const Matrix3& m_1() const{ return m_1_; }
  const Matrix3& m_1_adapted() const{ return m_1_adapted_; }

  void adapt(const RefWhite& destination, const Matrix3& method){

    m_adapted_ = ChromaticAdaptationMatrix(refWhite_, destination, method) * m_;
    m_1_adapted_ = (ChromaticAdaptationMatrix(refWhite_, destination, method) *
		    m_).inverse();
  }

  const Real& gamma() const{ return gamma_; }

protected:
  BaseRGB(){ }
  BaseRGB(const RefWhite& refWhite,
	  Real gamma,
	  const xyY& redPrimary,
	  const xyY& greenPrimary,
	  const xyY& bluePrimary) : refWhite_(refWhite), gamma_(gamma){

    m_ = ConversionMatrix_From_RGB_To_XYZ(redPrimary.position(),
					  greenPrimary.position(),
					  bluePrimary.position(),
					  refWhite_);

    m_1_ = m_.inverse();
  }

private:
  /* primaryRed, primaryGreen, primaryBlue are in xyY space.
   * refWhite is in XYZ space.
   */
  Matrix3 ConversionMatrix_From_RGB_To_XYZ(const Vector3& primaryRed,
					   const Vector3& primaryGreen,
					   const Vector3& primaryBlue,
					   const RefWhite& refWhite){

    Matrix3 xyzs;
    Matrix3 M;
    Vector3 S;
    Space_XYZ<Real> xyz;
    Space_xyY<Real> xyy;
    xyy = primaryRed;   xyzs.col(0) = xyz(xyy).position();
    xyy = primaryGreen; xyzs.col(1) = xyz(xyy).position();
    xyy = primaryBlue;  xyzs.col(2) = xyz(xyy).position();
    S = xyzs.inverse() * refWhite.position();
    M.col(0) = S(0) * xyzs.col(0);
    M.col(1) = S(1) * xyzs.col(1);
    M.col(2) = S(2) * xyzs.col(2);
    return M;
  }

  /*
   * source and destination are in XYZ space.
   */
  Matrix3 ChromaticAdaptationMatrix(const RefWhite& source,
				    const RefWhite& destination,
				    const Matrix3& method){

    Vector3 S = method * source.position();
    Vector3 D = method * destination.position();
    Matrix3 tmp = Matrix3::Zero();
    tmp(0, 0) = D(0) / S(0);
    tmp(1, 1) = D(1) / S(1);
    tmp(2, 2) = D(2) / S(2);
    return method.inverse() * tmp * method;
  }

protected:
  RefWhite refWhite_;
  Real gamma_;
  Matrix3 m_;
  Matrix3 m_adapted_;
  Matrix3 m_1_;
  Matrix3 m_1_adapted_;
};

#endif
