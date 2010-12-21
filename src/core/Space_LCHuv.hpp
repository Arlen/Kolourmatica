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


#ifndef SPACELCHUV_HPP
#define SPACELCHUV_HPP

#include "Space_XYZ.hpp"
#include "Space_xyY.hpp"
#include "Space_Lab.hpp"
#include "Space_LCHab.hpp"
#include "Space_Luv.hpp"
#include "Space_LinearRGB.hpp"
#include "Space_sRGB.hpp"

#include "../eigen/Eigen/Core"
#include "../eigen/Eigen/Dense"

#include <boost/mpl/bool.hpp>
#include <boost/mpl/assert.hpp>


template <class Real>
class Space_LCHuv{

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
  Space_LCHuv() : tri_(Vector3::Ones()){ }
  Space_LCHuv(const Real& L, const Real& C, const Real& H) :
    tri_(Vector3(L, C, H)){ }
  Space_LCHuv(const Vector3& tri) : tri_(tri){ }
  Space_LCHuv(const XYZ& colourSpace) : tri_(operator()(colourSpace).tri_){ }
  Space_LCHuv(const xyY& colourSpace) : tri_(operator()(colourSpace).tri_){ }
  Space_LCHuv(const Lab& colourSpace) : tri_(operator()(colourSpace).tri_){ }
  Space_LCHuv(const LCHab& colourSpace) : tri_(operator()(colourSpace).tri_){ }
  Space_LCHuv(const Luv& colourSpace) : tri_(operator()(colourSpace).tri_){ }
  Space_LCHuv(const LCHuv& colourSpace) : tri_(colourSpace.tri_){ }
  Space_LCHuv(const LinearRGB& colourSpace) : tri_(colourSpace.tri_){ }
  Space_LCHuv(const sRGB& colourSpace) : tri_(colourSpace.tri_){ }

  const Vector3& position() const{ return tri_; }

  LCHuv operator()(const XYZ& colourSpace) const{

    return operator()(Luv(colourSpace));
  }

  LCHuv operator()(const xyY& colourSpace) const{

    return operator()(XYZ(colourSpace));
  }

  LCHuv operator()(const Lab& colourSpace) const{

    return operator()(XYZ(colourSpace));
  }

  LCHuv operator()(const LCHab& colourSpace) const{

    return operator()(XYZ(colourSpace));
  }

  LCHuv operator()(const Luv& colourSpace) const{

    const Vector3& tri = colourSpace.position();
    Real l, c, h;

    l = tri(0);
    c = sqrt(tri(1) * tri(1) + tri(2) * tri(2));
    Real H = atan2(tri(2), tri(1)) * Constants<Real>::angle_;
    if(H < 0.0){ H += 360.0; }
    if(H >= 360.0){ H -= 360.0; }
    h = H;
    return LCHuv(l, c, h);
  }

  LCHuv operator()(const LinearRGB& colourSpace) const{

    return operator()(XYZ(colourSpace));
  }

  LCHuv operator()(const sRGB& colourSpace) const{

    return operator()(XYZ(colourSpace));
  }

private:
  Vector3 tri_;
};

#endif
