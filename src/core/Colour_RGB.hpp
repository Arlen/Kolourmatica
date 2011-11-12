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


#ifndef COLOUR_RGB_HPP
#define COLOUR_RGB_HPP

#include "RGB.hpp"
#include "ForwardDeclarations.hpp"
#include "Illuminant.hpp"

#include <Eigen/Core>
#include <Eigen/Dense>

using namespace Eigen;


template <class Real>
class Colour_AdobeRGB : public RGB<Real>{

  typedef Colour_xyY<Real> xyY;
  typedef Matrix<Real, 3, 1> Coord3;
  typedef IlluminantD65<Real, _1931_2> D65_1931_2;

public:
  Colour_AdobeRGB(const Coord3& tri) :
    RGB<Real>(D65_1931_2(),
  	      Real(2) + (Real(51) / Real(256)),
  	      xyY(0.64, 0.33, 1.0),
  	      xyY(0.21, 0.71, 1.0),
  	      xyY(0.15, 0.06, 1.0),
  	      tri){
    RGB<Real>::_rw = &_rw;
  }

  Colour_AdobeRGB(const Real r = 1, const Real g = 1, const Real b = 1) :
    RGB<Real>(D65_1931_2(),
  	      Real(2) + (Real(51) / Real(256)),
  	      xyY(0.64, 0.33, 1.0),
  	      xyY(0.21, 0.71, 1.0),
  	      xyY(0.15, 0.06, 1.0),
  	      Coord3(r, g, b)){
    RGB<Real>::_rw = &_rw;
  }

  Colour_AdobeRGB(const Colour_AdobeRGB<Real>& col) :
    RGB<Real>(D65_1931_2(),
  	      Real(2) + (Real(51) / Real(256)),
  	      xyY(0.64, 0.33, 1.0),
  	      xyY(0.21, 0.71, 1.0),
  	      xyY(0.15, 0.06, 1.0),
  	      Coord3(0, 0, 0)){
    RGB<Real>::_rw = &_rw;
  }

  Colour_AdobeRGB<Real>& operator=(const Colour_AdobeRGB<Real>& col){

    this->_coords = col.coords();
    return *this;
  }

  D65_1931_2 _rw;
};


template <class Real>
class Colour_AppleRGB : public RGB<Real>{

  typedef Colour_xyY<Real> xyY;
  typedef Matrix<Real, 3, 1> Coord3;
  typedef IlluminantD65<Real, _1931_2> D65_1931_2;

public:
  Colour_AppleRGB(const Coord3& tri) :
    RGB<Real>(D65_1931_2(),
  	      Real(1.8),
  	      xyY(0.625, 0.340, 1.0),
	      xyY(0.280, 0.595, 1.0),
	      xyY(0.155, 0.070, 1.0),
  	      tri){
    RGB<Real>::_rw = &_rw;
  }

  Colour_AppleRGB(const Real r = 1, const Real g = 1, const Real b = 1) :
    RGB<Real>(D65_1931_2(),
  	      Real(1.8),
  	      xyY(0.625, 0.340, 1.0),
	      xyY(0.280, 0.595, 1.0),
	      xyY(0.155, 0.070, 1.0),
  	      Coord3(r, g, b)){
    RGB<Real>::_rw = &_rw;
  }

  Colour_AppleRGB(const Colour_AppleRGB<Real>& col) :
    RGB<Real>(D65_1931_2(),
  	      Real(1.8),
  	      xyY(0.625, 0.340, 1.0),
	      xyY(0.280, 0.595, 1.0),
	      xyY(0.155, 0.070, 1.0),
  	      Coord3(0, 0, 0)){
    RGB<Real>::_rw = &_rw;
  }

  Colour_AppleRGB<Real>& operator=(const Colour_AppleRGB<Real>& col){

    this->_coords = col.coords();
    return *this;
  }

  D65_1931_2 _rw;
};


template <class Real>
class Colour_BestRGB : public RGB<Real>{

  typedef Colour_xyY<Real> xyY;
  typedef Matrix<Real, 3, 1> Coord3;
  typedef IlluminantD50<Real, _1931_2> D50_1931_2;

public:
  Colour_BestRGB(const Coord3& tri) :
    RGB<Real>(D50_1931_2(),
  	      Real(2.2),
  	      xyY(0.7347, 0.2653, 1.0),
	      xyY(0.2150, 0.7750, 1.0),
	      xyY(0.1300, 0.0350, 1.0),
  	      tri){
    RGB<Real>::_rw = &_rw;
  }

  Colour_BestRGB(const Real r = 1, const Real g = 1, const Real b = 1) :
    RGB<Real>(D50_1931_2(),
  	      Real(2.2),
  	      xyY(0.7347, 0.2653, 1.0),
	      xyY(0.2150, 0.7750, 1.0),
	      xyY(0.1300, 0.0350, 1.0),
  	      Coord3(r, g, b)){
    RGB<Real>::_rw = &_rw;
  }

  Colour_BestRGB(const Colour_BestRGB<Real>& col) :
    RGB<Real>(D50_1931_2(),
  	      Real(2.2),
  	      xyY(0.7347, 0.2653, 1.0),
	      xyY(0.2150, 0.7750, 1.0),
	      xyY(0.1300, 0.0350, 1.0),
  	      Coord3(0, 0, 0)){
    RGB<Real>::_rw = &_rw;
  }

  Colour_BestRGB<Real>& operator=(const Colour_BestRGB<Real>& col){

    this->_coords = col.coords();
    return *this;
  }

  D50_1931_2 _rw;
};


template <class Real>
class Colour_BetaRGB : public RGB<Real>{

  typedef Colour_xyY<Real> xyY;
  typedef Matrix<Real, 3, 1> Coord3;
  typedef IlluminantD50<Real, _1931_2> D50_1931_2;

public:
  Colour_BetaRGB(const Coord3& tri) :
    RGB<Real>(D50_1931_2(),
  	      Real(2.2),
  	      xyY(0.6888, 0.3112, 1.0),
	      xyY(0.1986, 0.7551, 1.0),
	      xyY(0.1265, 0.0352, 1.0),
  	      tri){
    RGB<Real>::_rw = &_rw;
  }

  Colour_BetaRGB(const Real r = 1, const Real g = 1, const Real b = 1) :
    RGB<Real>(D50_1931_2(),
  	      Real(2.2),
  	      xyY(0.6888, 0.3112, 1.0),
	      xyY(0.1986, 0.7551, 1.0),
	      xyY(0.1265, 0.0352, 1.0),
  	      Coord3(r, g, b)){
    RGB<Real>::_rw = &_rw;
  }

  Colour_BetaRGB(const Colour_BetaRGB<Real>& col) :
    RGB<Real>(D50_1931_2(),
  	      Real(2.2),
  	      xyY(0.6888, 0.3112, 1.0),
	      xyY(0.1986, 0.7551, 1.0),
	      xyY(0.1265, 0.0352, 1.0),
  	      Coord3(0, 0, 0)){
    RGB<Real>::_rw = &_rw;
  }

  Colour_BetaRGB<Real>& operator=(const Colour_BetaRGB<Real>& col){

    this->_coords = col.coords();
    return *this;
  }

  D50_1931_2 _rw;
};


template <class Real>
class Colour_BruceRGB : public RGB<Real>{

  typedef Colour_xyY<Real> xyY;
  typedef Matrix<Real, 3, 1> Coord3;
  typedef IlluminantD65<Real, _1931_2> D65_1931_2;

public:
  Colour_BruceRGB(const Coord3& tri) :
    RGB<Real>(D65_1931_2(),
  	      Real(2.2),
  	      xyY(0.64, 0.33, 1.0),
	      xyY(0.28, 0.65, 1.0),
	      xyY(0.15, 0.06, 1.0),
  	      tri){
    RGB<Real>::_rw = &_rw;
  }

  Colour_BruceRGB(const Real r = 1, const Real g = 1, const Real b = 1) :
    RGB<Real>(D65_1931_2(),
  	      Real(2.2),
  	      xyY(0.64, 0.33, 1.0),
	      xyY(0.28, 0.65, 1.0),
	      xyY(0.15, 0.06, 1.0),
  	      Coord3(r, g, b)){
    RGB<Real>::_rw = &_rw;
  }

  Colour_BruceRGB(const Colour_BruceRGB<Real>& col) :
    RGB<Real>(D65_1931_2(),
  	      Real(2.2),
  	      xyY(0.64, 0.33, 1.0),
	      xyY(0.28, 0.65, 1.0),
	      xyY(0.15, 0.06, 1.0),
  	      Coord3(0, 0, 0)){
    RGB<Real>::_rw = &_rw;
  }

  Colour_BruceRGB<Real>& operator=(const Colour_BruceRGB<Real>& col){

    this->_coords = col.coords();
    return *this;
  }

  D65_1931_2 _rw;
};


template <class Real>
class Colour_CIERGB : public RGB<Real>{

  typedef Colour_xyY<Real> xyY;
  typedef Matrix<Real, 3, 1> Coord3;
  typedef IlluminantE<Real, _1931_2> E_1931_2;

public:
  Colour_CIERGB(const Coord3& tri) :
    RGB<Real>(E_1931_2(),
  	      Real(2.2),
  	      xyY(0.735, 0.265, 1.0),
	      xyY(0.274, 0.717, 1.0),
	      xyY(0.167, 0.009, 1.0),
  	      tri){
    RGB<Real>::_rw = &_rw;
  }

  Colour_CIERGB(const Real r = 1, const Real g = 1, const Real b = 1) :
    RGB<Real>(E_1931_2(),
  	      Real(2.2),
  	      xyY(0.735, 0.265, 1.0),
	      xyY(0.274, 0.717, 1.0),
	      xyY(0.167, 0.009, 1.0),
  	      Coord3(r, g, b)){
    RGB<Real>::_rw = &_rw;
  }

  Colour_CIERGB(const Colour_CIERGB<Real>& col) :
    RGB<Real>(E_1931_2(),
  	      Real(2.2),
  	      xyY(0.735, 0.265, 1.0),
	      xyY(0.274, 0.717, 1.0),
	      xyY(0.167, 0.009, 1.0),
  	      Coord3(0, 0, 0)){
    RGB<Real>::_rw = &_rw;
  }

  Colour_CIERGB<Real>& operator=(const Colour_CIERGB<Real>& col){

    this->_coords = col.coords();
    return *this;
  }

  E_1931_2 _rw;
};


template <class Real>
class Colour_ColorMatchRGB : public RGB<Real>{

  typedef Colour_xyY<Real> xyY;
  typedef Matrix<Real, 3, 1> Coord3;
  typedef IlluminantD50<Real, _1931_2> D50_1931_2;

public:
  Colour_ColorMatchRGB(const Coord3& tri) :
    RGB<Real>(D50_1931_2(),
  	      Real(1.8),
  	      xyY(0.630, 0.340, 1.0),
	      xyY(0.295, 0.605, 1.0),
	      xyY(0.150, 0.075, 1.0),
  	      tri){
    RGB<Real>::_rw = &_rw;
  }

  Colour_ColorMatchRGB(const Real r = 1, const Real g = 1, const Real b = 1) :
    RGB<Real>(D50_1931_2(),
  	      Real(1.8),
  	      xyY(0.630, 0.340, 1.0),
	      xyY(0.295, 0.605, 1.0),
	      xyY(0.150, 0.075, 1.0),
  	      Coord3(r, g, b)){
    RGB<Real>::_rw = &_rw;
  }

  Colour_ColorMatchRGB(const Colour_ColorMatchRGB<Real>& col) :
    RGB<Real>(D50_1931_2(),
  	      Real(1.8),
  	      xyY(0.630, 0.340, 1.0),
	      xyY(0.295, 0.605, 1.0),
	      xyY(0.150, 0.075, 1.0),
  	      Coord3(0, 0, 0)){
    RGB<Real>::_rw = &_rw;
  }

  Colour_ColorMatchRGB<Real>& operator=(const Colour_ColorMatchRGB<Real>& col){

    this->_coords = col.coords();
    return *this;
  }

  D50_1931_2 _rw;
};


template <class Real>
class Colour_DonRGB4 : public RGB<Real>{

  typedef Colour_xyY<Real> xyY;
  typedef Matrix<Real, 3, 1> Coord3;
  typedef IlluminantD50<Real, _1931_2> D50_1931_2;

public:
  Colour_DonRGB4(const Coord3& tri) :
    RGB<Real>(D50_1931_2(),
  	      Real(2.2),
  	      xyY(0.696, 0.300, 1.0),
	      xyY(0.215, 0.765, 1.0),
	      xyY(0.130, 0.035, 1.0),
  	      tri){
    RGB<Real>::_rw = &_rw;
  }

  Colour_DonRGB4(const Real r = 1, const Real g = 1, const Real b = 1) :
    RGB<Real>(D50_1931_2(),
  	      Real(2.2),
  	      xyY(0.696, 0.300, 1.0),
	      xyY(0.215, 0.765, 1.0),
	      xyY(0.130, 0.035, 1.0),
  	      Coord3(r, g, b)){
    RGB<Real>::_rw = &_rw;
  }

  Colour_DonRGB4(const Colour_DonRGB4<Real>& col) :
    RGB<Real>(D50_1931_2(),
  	      Real(2.2),
  	      xyY(0.696, 0.300, 1.0),
	      xyY(0.215, 0.765, 1.0),
	      xyY(0.130, 0.035, 1.0),
  	      Coord3(0, 0, 0)){
    RGB<Real>::_rw = &_rw;
  }

  Colour_DonRGB4<Real>& operator=(const Colour_DonRGB4<Real>& col){

    this->_coords = col.coords();
    return *this;
  }

  D50_1931_2 _rw;
};


template <class Real>
class Colour_ECIRGB : public RGB<Real>{

  typedef Colour_xyY<Real> xyY;
  typedef Matrix<Real, 3, 1> Coord3;
  typedef IlluminantD50<Real, _1931_2> D50_1931_2;

public:
  Colour_ECIRGB(const Coord3& tri) :
    RGB<Real>(D50_1931_2(),
  	      Real(1.8),
  	      xyY(0.67, 0.33, 1.0),
	      xyY(0.21, 0.71, 1.0),
	      xyY(0.14, 0.08, 1.0),
  	      tri){
    RGB<Real>::_rw = &_rw;
  }

  Colour_ECIRGB(const Real r = 1, const Real g = 1, const Real b = 1) :
    RGB<Real>(D50_1931_2(),
  	      Real(1.8),
  	      xyY(0.67, 0.33, 1.0),
	      xyY(0.21, 0.71, 1.0),
	      xyY(0.14, 0.08, 1.0),
  	      Coord3(r, g, b)){
    RGB<Real>::_rw = &_rw;
  }

  Colour_ECIRGB(const Colour_ECIRGB<Real>& col) :
    RGB<Real>(D50_1931_2(),
  	      Real(1.8),
  	      xyY(0.67, 0.33, 1.0),
	      xyY(0.21, 0.71, 1.0),
	      xyY(0.14, 0.08, 1.0),
  	      Coord3(0, 0, 0)){
    RGB<Real>::_rw = &_rw;
  }

  Colour_ECIRGB<Real>& operator=(const Colour_ECIRGB<Real>& col){

    this->_coords = col.coords();
    return *this;
  }

  D50_1931_2 _rw;
};


template <class Real>
class Colour_EktaSpacePS5 : public RGB<Real>{

  typedef Colour_xyY<Real> xyY;
  typedef Matrix<Real, 3, 1> Coord3;
  typedef IlluminantD50<Real, _1931_2> D50_1931_2;

public:
  Colour_EktaSpacePS5(const Coord3& tri) :
    RGB<Real>(D50_1931_2(),
  	      Real(2.2),
  	      xyY(0.695, 0.305, 1.0),
	      xyY(0.260, 0.700, 1.0),
	      xyY(0.110, 0.005, 1.0),
  	      tri){
    RGB<Real>::_rw = &_rw;
  }

  Colour_EktaSpacePS5(const Real r = 1, const Real g = 1, const Real b = 1) :
    RGB<Real>(D50_1931_2(),
  	      Real(2.2),
  	      xyY(0.695, 0.305, 1.0),
	      xyY(0.260, 0.700, 1.0),
	      xyY(0.110, 0.005, 1.0),
  	      Coord3(r, g, b)){
    RGB<Real>::_rw = &_rw;
  }

  Colour_EktaSpacePS5(const Colour_EktaSpacePS5<Real>& col) :
    RGB<Real>(D50_1931_2(),
  	      Real(2.2),
  	      xyY(0.695, 0.305, 1.0),
	      xyY(0.260, 0.700, 1.0),
	      xyY(0.110, 0.005, 1.0),
  	      Coord3(0, 0, 0)){
    RGB<Real>::_rw = &_rw;
  }

  Colour_EktaSpacePS5<Real>& operator=(const Colour_EktaSpacePS5<Real>& col){

    this->_coords = col.coords();
    return *this;
  }

  D50_1931_2 _rw;
};


template <class Real>
class Colour_NTSCRGB : public RGB<Real>{

  typedef Colour_xyY<Real> xyY;
  typedef Matrix<Real, 3, 1> Coord3;
  typedef IlluminantC<Real, _1931_2> C_1931_2;

public:
  Colour_NTSCRGB(const Coord3& tri) :
    RGB<Real>(C_1931_2(),
  	      Real(2.2),
  	      xyY(0.67, 0.33, 1.0),
	      xyY(0.21, 0.71, 1.0),
	      xyY(0.14, 0.08, 1.0),
  	      tri){
    RGB<Real>::_rw = &_rw;
  }

  Colour_NTSCRGB(const Real r = 1, const Real g = 1, const Real b = 1) :
    RGB<Real>(C_1931_2(),
  	      Real(2.2),
  	      xyY(0.67, 0.33, 1.0),
	      xyY(0.21, 0.71, 1.0),
	      xyY(0.14, 0.08, 1.0),
  	      Coord3(r, g, b)){
    RGB<Real>::_rw = &_rw;
  }

  Colour_NTSCRGB(const Colour_NTSCRGB<Real>& col) :
    RGB<Real>(C_1931_2(),
  	      Real(2.2),
  	      xyY(0.67, 0.33, 1.0),
	      xyY(0.21, 0.71, 1.0),
	      xyY(0.14, 0.08, 1.0),
  	      Coord3(0, 0, 0)){
    RGB<Real>::_rw = &_rw;
  }

  Colour_NTSCRGB<Real>& operator=(const Colour_NTSCRGB<Real>& col){

    this->_coords = col.coords();
    return *this;
  }

  C_1931_2 _rw;
};


template <class Real>
class Colour_PAL_SECAMRGB : public RGB<Real>{

  typedef Colour_xyY<Real> xyY;
  typedef Matrix<Real, 3, 1> Coord3;
  typedef IlluminantD65<Real, _1931_2> D65_1931_2;

public:
  Colour_PAL_SECAMRGB(const Coord3& tri) :
    RGB<Real>(D65_1931_2(),
  	      Real(2.2),
  	      xyY(0.64, 0.33, 1.0),
	      xyY(0.29, 0.60, 1.0),
	      xyY(0.15, 0.06, 1.0),
  	      tri){
    RGB<Real>::_rw = &_rw;
  }

  Colour_PAL_SECAMRGB(const Real r = 1, const Real g = 1, const Real b = 1) :
    RGB<Real>(D65_1931_2(),
  	      Real(2.2),
  	      xyY(0.64, 0.33, 1.0),
	      xyY(0.29, 0.60, 1.0),
	      xyY(0.15, 0.06, 1.0),
  	      Coord3(r, g, b)){
    RGB<Real>::_rw = &_rw;
  }

  Colour_PAL_SECAMRGB(const Colour_PAL_SECAMRGB<Real>& col) :
    RGB<Real>(D65_1931_2(),
  	      Real(2.2),
  	      xyY(0.64, 0.33, 1.0),
	      xyY(0.29, 0.60, 1.0),
	      xyY(0.15, 0.06, 1.0),
  	      Coord3(0, 0, 0)){
    RGB<Real>::_rw = &_rw;
  }

  Colour_PAL_SECAMRGB<Real>& operator=(const Colour_PAL_SECAMRGB<Real>& col){

    this->_coords = col.coords();
    return *this;
  }

  D65_1931_2 _rw;
};


template <class Real>
class Colour_ProPhotoRGB : public RGB<Real>{

  typedef Colour_xyY<Real> xyY;
  typedef Matrix<Real, 3, 1> Coord3;
  typedef IlluminantD50<Real, _1931_2> D50_1931_2;

public:
  Colour_ProPhotoRGB(const Coord3& tri) :
    RGB<Real>(D50_1931_2(),
  	      Real(1.8),
  	      xyY(0.7347, 0.2653, 1.0),
	      xyY(0.1596, 0.8404, 1.0),
	      xyY(0.0366, 0.0001, 1.0),
  	      tri){
    RGB<Real>::_rw = &_rw;
  }

  Colour_ProPhotoRGB(const Real r = 1, const Real g = 1, const Real b = 1) :
    RGB<Real>(D50_1931_2(),
  	      Real(1.8),
  	      xyY(0.7347, 0.2653, 1.0),
	      xyY(0.1596, 0.8404, 1.0),
	      xyY(0.0366, 0.0001, 1.0),
  	      Coord3(r, g, b)){
    RGB<Real>::_rw = &_rw;
  }

  Colour_ProPhotoRGB(const Colour_ProPhotoRGB<Real>& col) :
    RGB<Real>(D50_1931_2(),
  	      Real(1.8),
  	      xyY(0.7347, 0.2653, 1.0),
	      xyY(0.1596, 0.8404, 1.0),
	      xyY(0.0366, 0.0001, 1.0),
  	      Coord3(0, 0, 0)){
    RGB<Real>::_rw = &_rw;
  }

  Colour_ProPhotoRGB<Real>& operator=(const Colour_ProPhotoRGB<Real>& col){

    this->_coords = col.coords();
    return *this;
  }

  D50_1931_2 _rw;
};


template <class Real>
class Colour_SMPTE_CRGB : public RGB<Real>{

  typedef Colour_xyY<Real> xyY;
  typedef Matrix<Real, 3, 1> Coord3;
  typedef IlluminantD65<Real, _1931_2> D65_1931_2;

public:
  Colour_SMPTE_CRGB(const Coord3& tri) :
    RGB<Real>(D65_1931_2(),
  	      Real(2.2),
  	      xyY(0.630, 0.340, 1.0),
	      xyY(0.310, 0.595, 1.0),
	      xyY(0.155, 0.070, 1.0),
  	      tri){
    RGB<Real>::_rw = &_rw;
  }

  Colour_SMPTE_CRGB(const Real r = 1, const Real g = 1, const Real b = 1) :
    RGB<Real>(D65_1931_2(),
  	      Real(2.2),
  	      xyY(0.630, 0.340, 1.0),
	      xyY(0.310, 0.595, 1.0),
	      xyY(0.155, 0.070, 1.0),
  	      Coord3(r, g, b)){
    RGB<Real>::_rw = &_rw;
  }

  Colour_SMPTE_CRGB(const Colour_SMPTE_CRGB<Real>& col) :
    RGB<Real>(D65_1931_2(),
  	      Real(2.2),
  	      xyY(0.630, 0.340, 1.0),
	      xyY(0.310, 0.595, 1.0),
	      xyY(0.155, 0.070, 1.0),
  	      Coord3(0, 0, 0)){
    RGB<Real>::_rw = &_rw;
  }

  Colour_SMPTE_CRGB<Real>& operator=(const Colour_SMPTE_CRGB<Real>& col){

    this->_coords = col.coords();
    return *this;
  }

  D65_1931_2 _rw;
};


template <class Real>
class Colour_WideGamutRGB : public RGB<Real>{

  typedef Colour_xyY<Real> xyY;
  typedef Matrix<Real, 3, 1> Coord3;
  typedef IlluminantD50<Real, _1931_2> D50_1931_2;

public:
  Colour_WideGamutRGB(const Coord3& tri) :
    RGB<Real>(D50_1931_2(),
  	      Real(2.2),
  	      xyY(0.735, 0.265, 1.0),
	      xyY(0.115, 0.826, 1.0),
	      xyY(0.157, 0.018, 1.0),
  	      tri){
    RGB<Real>::_rw = &_rw;
  }

  Colour_WideGamutRGB(const Real r = 1, const Real g = 1, const Real b = 1) :
    RGB<Real>(D50_1931_2(),
  	      Real(2.2),
  	      xyY(0.735, 0.265, 1.0),
	      xyY(0.115, 0.826, 1.0),
	      xyY(0.157, 0.018, 1.0),
  	      Coord3(r, g, b)){
    RGB<Real>::_rw = &_rw;
  }

  Colour_WideGamutRGB(const Colour_WideGamutRGB<Real>& col) :
    RGB<Real>(D50_1931_2(),
  	      Real(2.2),
  	      xyY(0.735, 0.265, 1.0),
	      xyY(0.115, 0.826, 1.0),
	      xyY(0.157, 0.018, 1.0),
  	      Coord3(0, 0, 0)){
    RGB<Real>::_rw = &_rw;
  }

  Colour_WideGamutRGB<Real>& operator=(const Colour_WideGamutRGB<Real>& col){

    this->_coords = col.coords();
    return *this;
  }

  D50_1931_2 _rw;
};


/* used when converting XYZ to sRGB */
template <class Real>
void sRGB_Companding(const Real gamma, Matrix<Real, 3, 1>& coords){

  Real p = Real(1.0) / gamma;

  if(coords(0) > 0.0031308)
    coords(0) = 1.055 * pow(coords(0), p) - 0.055;
  else
    coords(0) = 12.92 * coords(0);

  if(coords(1) > 0.0031308)
    coords(1) = 1.055 * pow(coords(1), p) - 0.055;
  else
    coords(1) = 12.92 * coords(1);

  if(coords(2) > 0.0031308)
    coords(2) = 1.055 * pow(coords(2), p) - 0.055;
  else
    coords(2) = 12.92 * coords(2);
}

/* used when converting sRGB to XYZ */
template <class Real>
Matrix<Real, 3, 1> inverse_sRGB_Companding(const Real gamma,
					   const Matrix<Real, 3, 1>& coords){

  Matrix<Real, 3, 1> tri;

  if(coords(0) > 0.04045)
    tri(0) = pow( ((coords(0) + 0.055) / 1.055), gamma );
  else
    tri(0) = coords(0) / 12.92;

  if(coords(1) > 0.04045)
    tri(1) = pow( ((coords(1) + 0.055) / 1.055), gamma );
  else
    tri(1) = coords(1) / 12.92;

  if(coords(2) > 0.04045)
    tri(2) = pow( ((coords(2) + 0.055) / 1.055), gamma );
  else
    tri(2) = coords(2) / 12.92;

  return tri;
}


template <class Real>
class Colour_sRGB : public RGB<Real>{

  typedef Colour_xyY<Real> xyY;
  typedef Matrix<Real, 3, 1> Coord3;
  typedef IlluminantD65<Real, _1931_2> D65_1931_2;
  typedef std::function<void (const Real, Coord3& coords)> Companding;
  typedef std::function<Coord3 (const Real,
				const Coord3& coords)> InverseCompanding;

public:
  Colour_sRGB(const Coord3& tri) :
    RGB<Real>(D65_1931_2(),
  	      Real(2.4),
  	      xyY(0.64, 0.33, 1.0),
	      xyY(0.30, 0.60, 1.0),
	      xyY(0.15, 0.06, 1.0),
	      Companding( sRGB_Companding<Real> ),
	      InverseCompanding( inverse_sRGB_Companding<Real> ),
  	      tri){
    RGB<Real>::_rw = &_rw;
  }

  Colour_sRGB(const Real r = 1, const Real g = 1, const Real b = 1) :
    RGB<Real>(D65_1931_2(),
  	      Real(2.4),
  	      xyY(0.64, 0.33, 1.0),
	      xyY(0.30, 0.60, 1.0),
	      xyY(0.15, 0.06, 1.0),
	      Companding( sRGB_Companding<Real> ),
	      InverseCompanding( inverse_sRGB_Companding<Real> ),
  	      Coord3(r, g, b)){
    RGB<Real>::_rw = &_rw;
  }

  Colour_sRGB(const Colour_sRGB<Real>& col) :
    RGB<Real>(D65_1931_2(),
  	      Real(2.4),
  	      xyY(0.64, 0.33, 1.0),
	      xyY(0.30, 0.60, 1.0),
	      xyY(0.15, 0.06, 1.0),
	      Companding( sRGB_Companding<Real> ),
	      InverseCompanding( inverse_sRGB_Companding<Real> ),
  	      Coord3(0, 0, 0)){
    RGB<Real>::_rw = &_rw;
  }

  Colour_sRGB<Real>& operator=(const Colour_sRGB<Real>& col){

    this->_coords = col.coords();
    return *this;
  }

  D65_1931_2 _rw;
};

#endif
