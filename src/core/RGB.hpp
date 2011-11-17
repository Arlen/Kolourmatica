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


#ifndef RGB_HPP
#define RGB_HPP

#include "ColourSpace.hpp"
#include "ForwardDeclarations.hpp"
#include "Illuminant.hpp"
#include "ChromaticAdaptation.hpp"

#include <Eigen/Core>
#include <Eigen/Dense>

#include <functional>

using namespace Eigen;


/* used when converting XYZ to RGB */
template <class Real>
void gammaCompanding(const Real gamma, Matrix<Real, 3, 1>& coords){

  Real p = Real(1.0) / gamma;

  if(coords(0) < 0.0)
    coords(0) = pow(-coords(0), p) * -1.0;
  else
    coords(0) = pow(coords(0), p);

  if(coords(1) < 0.0)
    coords(1) = pow(-coords(1), p) * -1.0;
  else
    coords(1) = pow(coords(1), p);

  if(coords(2) < 0.0)
    coords(2) = pow(-coords(2), p) * -1.0; 
  else
    coords(2) = pow(coords(2), p);
}

/* used when converting RGB to XYZ */
template <class Real>
Matrix<Real, 3, 1> inverseGammaCompanding(const Real gamma,
					  const Matrix<Real, 3, 1>& coords){

  Matrix<Real, 3, 1> tri;

  if(coords(0) < 0.0)
    tri(0) = pow(-coords(0), gamma) * -1.0;
  else
    tri(0) = pow(coords(0), gamma);

  if(coords(1) < 0.0)
    tri(1) = pow(-coords(1), gamma) * -1.0;
  else
    tri(1) = pow(coords(1), gamma);

  if(coords(2) < 0.0)
    tri(2) = pow(-coords(2), gamma) * -1.0;
  else
    tri(2) = pow(coords(2), gamma);

  return tri;
}


template <class Real>
class RGB : public ColourSpace<Real, Matrix<Real, 3, 1> >{

  typedef Colour_XYZ<Real> XYZ;
  typedef Colour_xyY<Real> xyY;
  typedef Colour_Lab<Real> Lab;
  typedef Colour_LCHab<Real> LCHab;
  typedef Colour_Luv<Real> Luv;
  typedef Colour_LCHuv<Real> LCHuv;
  typedef BaseIlluminant<Real> Illuminant;
  typedef Matrix<Real, 3, 1> Coord3;
  typedef Matrix<Real, 3, 3> Matrix3;
  typedef ColourSpace<Real, Matrix<Real, 3, 1> > Parent;

  typedef std::function<void (const Real, Coord3& coords)> Companding;
  typedef std::function<Coord3 (const Real,
				const Coord3& coords)> InverseCompanding;

public:
  const Real& gamma() const{ return _gamma; }

  const Matrix3& m() const{ return _m; }

  const Matrix3& m_1() const{ return _m_1; }

  const Illuminant& referenceWhite() const{ return *_rw; }

  void referenceWhite(const Illuminant*& rw){ rw = _rw; }

  Coord3 inverseCompanding(const Real gamma, const Coord3& coords) const{

    return _inv_companding(gamma, coords);
  }


  Coord3 to_XYZ(const Illuminant* const rw = nullptr) const{

    XYZ xyz; xyz.from(*this); return xyz.coords();
  }

  Coord3& from_XYZ(const Coord3& coords, const Illuminant* const rw = nullptr){

    from(XYZ(coords)); return Parent::_coords;
  }


  RGB& from(const XYZ& col){

    Parent::_coords = _m_1 * col.coords();
    _companding(_gamma, Parent::_coords);
    return *this;
  }

  RGB& from(const xyY& col){ return from(XYZ().from(col)); }

  RGB& from(const Lab& col, const Illuminant& rw){

    return from(XYZ().from(col, rw));
  }

  RGB& from(const LCHab& col, const Illuminant& rw){

    return from(XYZ().from(col, rw));
  }

  RGB& from(const Luv& col, const Illuminant& rw){

    return from(XYZ().from(col, rw));
  }

  RGB& from(const LCHuv& col, const Illuminant& rw){

    return from(XYZ().from(col, rw));
  }


protected:
  RGB(const Illuminant& rw,
      const Real gamma,
      const xyY& redPrimary,
      const xyY& greenPrimary,
      const xyY& bluePrimary,
      const Coord3& tri) :
    ColourSpace<Real, Coord3>(tri),
    _gamma(gamma),
    _m(computeConversionMatrix(redPrimary, greenPrimary, bluePrimary, rw)),
    _m_1(_m.inverse()),
    _companding(Companding( gammaCompanding<Real> )),
    _inv_companding(InverseCompanding( inverseGammaCompanding<Real> )){ }

  /* so far only used by sRGB to initialize the companding functions. */
  RGB(const Illuminant& rw,
      const Real gamma,
      const xyY& redPrimary,
      const xyY& greenPrimary,
      const xyY& bluePrimary,
      const Companding& c,
      const InverseCompanding& ic,
      const Coord3& tri) :
    ColourSpace<Real, Coord3>(tri),
    _gamma(gamma),
    _m(computeConversionMatrix(redPrimary, greenPrimary, bluePrimary, rw)),
    _m_1(_m.inverse()),
    _companding(c),
    _inv_companding(ic){ }

private:
  /*
   * computes the conversion matrix from RGB to XYZ
   */
  Matrix3 computeConversionMatrix(const xyY& redPrimary,
				  const xyY& greenPrimary,
				  const xyY& bluePrimary,
				  const Illuminant& rw){

    Matrix3 xyzs;
    Matrix3 M;
    Coord3 S;
    XYZ xyz;
    xyzs.col(0) = xyz.from(redPrimary).coords();
    xyzs.col(1) = xyz.from(greenPrimary).coords();
    xyzs.col(2) = xyz.from(bluePrimary).coords();
    S = xyzs.inverse() * rw.colour_XYZ().coords();
    M.col(0) = S(0) * xyzs.col(0);
    M.col(1) = S(1) * xyzs.col(1);
    M.col(2) = S(2) * xyzs.col(2);
    return M;
  }

protected:
  Illuminant* _rw;
  Real _gamma;
  const Matrix3 _m;
  const Matrix3 _m_1;
  const Companding _companding;
  const InverseCompanding _inv_companding;
};

#endif
