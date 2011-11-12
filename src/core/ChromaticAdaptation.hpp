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


#ifndef CHROMATIC_ADAPTATION_HPP
#define CHROMATIC_ADAPTATION_HPP

#include "Illuminant.hpp"

#include <Eigen/Core>
#include <Eigen/Dense>

using namespace Eigen;


template <class Real>
struct AdaptationMethod{
 
  static const Matrix<Real, 3, 3> _XYZScaling;
  static const Matrix<Real, 3, 3> _VonKries;
  static const Matrix<Real, 3, 3> _Bradford;
  static const Real xyzscaling[];
  static const Real vonkries[];
  static const Real bradford[];
};

template <class Real>
const Matrix<Real, 3, 3> AdaptationMethod<Real>::_XYZScaling =
  Matrix<Real, 3, 3>(xyzscaling);

template <class Real>
const Matrix<Real, 3, 3> AdaptationMethod<Real>::_VonKries =
  Matrix<Real, 3, 3>(vonkries);

template <class Real>
const Matrix<Real, 3, 3> AdaptationMethod<Real>::_Bradford =
  Matrix<Real, 3, 3>(bradford);

template <class Real>
const Real AdaptationMethod<Real>::xyzscaling[] =
  {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0};

template <class Real>
const Real AdaptationMethod<Real>::vonkries[] =
  {0.40024, -0.2263, 0.0, 0.7076, 1.16532, 0.0, -0.08081, 0.0457, 0.91822};

template <class Real>
const Real AdaptationMethod<Real>::bradford[] =
  {0.8951, -0.7502, 0.0389, 0.2664, 1.7135, -0.0685, -0.1614, 0.0367, 1.0296};


template <typename Real>
Matrix<Real, 3, 3> chromaticAdaptationMatrix(const BaseIlluminant<Real>& source,
					     const BaseIlluminant<Real>& target,
					     const Matrix<Real, 3, 3>& method){

  Matrix<Real, 3, 1> S = method * source.colour_XYZ().coords();
  Matrix<Real, 3, 1> D = method * target.colour_XYZ().coords();
  Matrix<Real, 3, 3> tmp = Matrix<Real, 3, 3>::Zero();
  tmp(0, 0) = D(0) / S(0);
  tmp(1, 1) = D(1) / S(1);
  tmp(2, 2) = D(2) / S(2);
  return method.inverse() * tmp * method;
}

#endif
