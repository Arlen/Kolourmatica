/***********************************************************************
|*  Copyright (C) 2011 Arlen Avakian
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


#ifndef ILLUMINANT_HPP
#define ILLUMINANT_HPP

#include "ForwardDeclarations.hpp"

#include <Eigen/Core>
#include <Eigen/Dense>

using namespace Eigen;


class _1964_10{ };
class _1931_2{ };


template <class Real>
class BaseIlluminant{

  typedef Colour_XYZ<Real> XYZ;
  typedef Colour_xyY<Real> xyY;

public:
  BaseIlluminant(){ }
  const XYZ& colour_XYZ() const{ return _rw; }
  virtual ~BaseIlluminant(){ }

protected:
  BaseIlluminant(const xyY& rw) : _rw(rw.template to<XYZ>()){ }

private:
  XYZ _rw;
};


template <class Real, class Observer> class IlluminantA;

template <class Real>
class IlluminantA<Real, _1964_10> : public BaseIlluminant<Real>{

  typedef BaseIlluminant<Real> Illuminant;
  typedef Colour_xyY<Real> xyY;

public:
  IlluminantA() : Illuminant(xyY(0.45117, 0.40594)){ }
};

template <class Real>
class IlluminantA<Real, _1931_2> : public BaseIlluminant<Real>{

  typedef BaseIlluminant<Real> Illuminant;
  typedef Colour_xyY<Real> xyY;

public:
  IlluminantA() : Illuminant(xyY(0.44757, 0.40745)){ }
};


template <class Real, class Observer> class IlluminantB;

template <class Real>
class IlluminantB<Real, _1964_10> : public BaseIlluminant<Real>{

  typedef BaseIlluminant<Real> Illuminant;
  typedef Colour_xyY<Real> xyY;

public:
  IlluminantB() : Illuminant(xyY(0.3498, 0.3527)){ }
};

template <class Real>
class IlluminantB<Real, _1931_2> : public BaseIlluminant<Real>{

  typedef BaseIlluminant<Real> Illuminant;
  typedef Colour_xyY<Real> xyY;

public:
  IlluminantB() : Illuminant(xyY(0.34842, 0.35161)){ }
};


template <class Real, class Observer> class IlluminantC;

template <class Real>
class IlluminantC<Real, _1964_10> : public BaseIlluminant<Real>{

  typedef BaseIlluminant<Real> Illuminant;
  typedef Colour_xyY<Real> xyY;

public:
  IlluminantC() : Illuminant(xyY(0.31039, 0.31905)){ }
};

template <class Real>
class IlluminantC<Real, _1931_2> : public BaseIlluminant<Real>{

  typedef BaseIlluminant<Real> Illuminant;
  typedef Colour_xyY<Real> xyY;

public:
  IlluminantC() : Illuminant(xyY(0.31006, 0.31616)){ }
};


template <class Real, class Observer> class IlluminantD50;

template <class Real>
class IlluminantD50<Real, _1964_10> : public BaseIlluminant<Real>{

  typedef BaseIlluminant<Real> Illuminant;
  typedef Colour_xyY<Real> xyY;

public:
  IlluminantD50() : Illuminant(xyY(0.34773, 0.35952)){ }
};

template <class Real>
class IlluminantD50<Real, _1931_2> : public BaseIlluminant<Real>{

  typedef BaseIlluminant<Real> Illuminant;
  typedef Colour_xyY<Real> xyY;

public:
  IlluminantD50() : Illuminant(xyY(0.34567, 0.3585)){ }
};


template <class Real, class Observer> class IlluminantD55;

template <class Real>
class IlluminantD55<Real, _1964_10> : public BaseIlluminant<Real>{

  typedef BaseIlluminant<Real> Illuminant;
  typedef Colour_xyY<Real> xyY;

public:
  IlluminantD55() : Illuminant(xyY(0.33411, 0.34877)){ }
};

template <class Real>
class IlluminantD55<Real, _1931_2> : public BaseIlluminant<Real>{

  typedef BaseIlluminant<Real> Illuminant;
  typedef Colour_xyY<Real> xyY;

public:
  IlluminantD55() : Illuminant(xyY(0.33242, 0.34743)){ }
};


template <class Real, class Observer> class IlluminantD65;

template <class Real>
class IlluminantD65<Real, _1964_10> : public BaseIlluminant<Real>{

  typedef BaseIlluminant<Real> Illuminant;
  typedef Colour_xyY<Real> xyY;

public:
  IlluminantD65() : Illuminant(xyY(0.31382, 0.331)){ }
};

template <class Real>
class IlluminantD65<Real, _1931_2> : public BaseIlluminant<Real>{

  typedef BaseIlluminant<Real> Illuminant;
  typedef Colour_xyY<Real> xyY;

public:
  IlluminantD65() : Illuminant(xyY(0.31271, 0.32902)){ }
};


template <class Real, class Observer> class IlluminantD75;

template <class Real>
class IlluminantD75<Real, _1964_10> : public BaseIlluminant<Real>{

  typedef BaseIlluminant<Real> Illuminant;
  typedef Colour_xyY<Real> xyY;

public:
  IlluminantD75() : Illuminant(xyY(0.29968, 0.3174)){ }
};

template <class Real>
class IlluminantD75<Real, _1931_2> : public BaseIlluminant<Real>{

  typedef BaseIlluminant<Real> Illuminant;
  typedef Colour_xyY<Real> xyY;

public:
  IlluminantD75() : Illuminant(xyY(0.29902, 0.31485)){ }
};


template <class Real, class Observer> class IlluminantE;

template <class Real>
class IlluminantE<Real, _1964_10> : public BaseIlluminant<Real>{

  typedef BaseIlluminant<Real> Illuminant;
  typedef Colour_xyY<Real> xyY;

public:
  IlluminantE() : Illuminant(xyY(1/3, 1/3)){ }
};

template <class Real>
class IlluminantE<Real, _1931_2> : public BaseIlluminant<Real>{

  typedef BaseIlluminant<Real> Illuminant;
  typedef Colour_xyY<Real> xyY;

public:
  IlluminantE() : Illuminant(xyY(1/3, 1/3)){ }
};


template <class Real, class Observer> class IlluminantF1;

template <class Real>
class IlluminantF1<Real, _1964_10> : public BaseIlluminant<Real>{

  typedef BaseIlluminant<Real> Illuminant;
  typedef Colour_xyY<Real> xyY;

public:
  IlluminantF1() : Illuminant(xyY(0.31811, 0.33559)){ }
};

template <class Real>
class IlluminantF1<Real, _1931_2> : public BaseIlluminant<Real>{

  typedef BaseIlluminant<Real> Illuminant;
  typedef Colour_xyY<Real> xyY;

public:
  IlluminantF1() : Illuminant(xyY(0.3131, 0.33727)){ }
};


template <class Real, class Observer> class IlluminantF2;

template <class Real>
class IlluminantF2<Real, _1964_10> : public BaseIlluminant<Real>{

  typedef BaseIlluminant<Real> Illuminant;
  typedef Colour_xyY<Real> xyY;

public:
  IlluminantF2() : Illuminant(xyY(0.37925, 0.36733)){ }
};

template <class Real>
class IlluminantF2<Real, _1931_2> : public BaseIlluminant<Real>{

  typedef BaseIlluminant<Real> Illuminant;
  typedef Colour_xyY<Real> xyY;

public:
  IlluminantF2() : Illuminant(xyY(0.37208, 0.37529)){ }
};


template <class Real, class Observer> class IlluminantF3;

template <class Real>
class IlluminantF3<Real, _1964_10> : public BaseIlluminant<Real>{

  typedef BaseIlluminant<Real> Illuminant;
  typedef Colour_xyY<Real> xyY;

public:
  IlluminantF3() : Illuminant(xyY(0.41761, 0.38324)){ }
};

template <class Real>
class IlluminantF3<Real, _1931_2> : public BaseIlluminant<Real>{

  typedef BaseIlluminant<Real> Illuminant;
  typedef Colour_xyY<Real> xyY;

public:
  IlluminantF3() : Illuminant(xyY(0.4091, 0.3943)){ }
};


template <class Real, class Observer> class IlluminantF4;

template <class Real>
class IlluminantF4<Real, _1964_10> : public BaseIlluminant<Real>{

  typedef BaseIlluminant<Real> Illuminant;
  typedef Colour_xyY<Real> xyY;

public:
  IlluminantF4() : Illuminant(xyY(0.4492, 0.39074)){ }
};

template <class Real>
class IlluminantF4<Real, _1931_2> : public BaseIlluminant<Real>{

  typedef BaseIlluminant<Real> Illuminant;
  typedef Colour_xyY<Real> xyY;

public:
  IlluminantF4() : Illuminant(xyY(0.44018, 0.40329)){ }
};


template <class Real, class Observer> class IlluminantF5;

template <class Real>
class IlluminantF5<Real, _1964_10> : public BaseIlluminant<Real>{

  typedef BaseIlluminant<Real> Illuminant;
  typedef Colour_xyY<Real> xyY;

public:
  IlluminantF5() : Illuminant(xyY(0.31975, 0.34246)){ }
};

template <class Real>
class IlluminantF5<Real, _1931_2> : public BaseIlluminant<Real>{

  typedef BaseIlluminant<Real> Illuminant;
  typedef Colour_xyY<Real> xyY;

public:
  IlluminantF5() : Illuminant(xyY(0.31379, 0.34531)){ }
};


template <class Real, class Observer> class IlluminantF6;

template <class Real>
class IlluminantF6<Real, _1964_10> : public BaseIlluminant<Real>{

  typedef BaseIlluminant<Real> Illuminant;
  typedef Colour_xyY<Real> xyY;

public:
  IlluminantF6() : Illuminant(xyY(0.3866, 0.37847)){ }
};

template <class Real>
class IlluminantF6<Real, _1931_2> : public BaseIlluminant<Real>{

  typedef BaseIlluminant<Real> Illuminant;
  typedef Colour_xyY<Real> xyY;

public:
  IlluminantF6() : Illuminant(xyY(0.3779, 0.38835)){ }
};


template <class Real, class Observer> class IlluminantF7;

template <class Real>
class IlluminantF7<Real, _1964_10> : public BaseIlluminant<Real>{

  typedef BaseIlluminant<Real> Illuminant;
  typedef Colour_xyY<Real> xyY;

public:
  IlluminantF7() : Illuminant(xyY(0.31569, 0.3296)){ }
};

template <class Real>
class IlluminantF7<Real, _1931_2> : public BaseIlluminant<Real>{

  typedef BaseIlluminant<Real> Illuminant;
  typedef Colour_xyY<Real> xyY;

public:
  IlluminantF7() : Illuminant(xyY(0.31292, 0.32933)){ }
};


template <class Real, class Observer> class IlluminantF8;

template <class Real>
class IlluminantF8<Real, _1964_10> : public BaseIlluminant<Real>{

  typedef BaseIlluminant<Real> Illuminant;
  typedef Colour_xyY<Real> xyY;

public:
  IlluminantF8() : Illuminant(xyY(0.34902, 0.35939)){ }
};

template <class Real>
class IlluminantF8<Real, _1931_2> : public BaseIlluminant<Real>{

  typedef BaseIlluminant<Real> Illuminant;
  typedef Colour_xyY<Real> xyY;

public:
  IlluminantF8() : Illuminant(xyY(0.34588, 0.35875)){ }
};


template <class Real, class Observer> class IlluminantF9;

template <class Real>
class IlluminantF9<Real, _1964_10> : public BaseIlluminant<Real>{

  typedef BaseIlluminant<Real> Illuminant;
  typedef Colour_xyY<Real> xyY;

public:
  IlluminantF9() : Illuminant(xyY(0.37829, 0.37045)){ }
};

template <class Real>
class IlluminantF9<Real, _1931_2> : public BaseIlluminant<Real>{

  typedef BaseIlluminant<Real> Illuminant;
  typedef Colour_xyY<Real> xyY;

public:
  IlluminantF9() : Illuminant(xyY(0.37417, 0.37281)){ }
};


template <class Real, class Observer> class IlluminantF10;

template <class Real>
class IlluminantF10<Real, _1964_10> : public BaseIlluminant<Real>{

  typedef BaseIlluminant<Real> Illuminant;
  typedef Colour_xyY<Real> xyY;

public:
  IlluminantF10() : Illuminant(xyY(0.3509, 0.35444)){ }
};

template <class Real>
class IlluminantF10<Real, _1931_2> : public BaseIlluminant<Real>{

  typedef BaseIlluminant<Real> Illuminant;
  typedef Colour_xyY<Real> xyY;

public:
  IlluminantF10() : Illuminant(xyY(0.34609, 0.35986)){ }
};


template <class Real, class Observer> class IlluminantF11;

template <class Real>
class IlluminantF11<Real, _1964_10> : public BaseIlluminant<Real>{

  typedef BaseIlluminant<Real> Illuminant;
  typedef Colour_xyY<Real> xyY;

public:
  IlluminantF11() : Illuminant(xyY(0.38541, 0.37123)){ }
};

template <class Real>
class IlluminantF11<Real, _1931_2> : public BaseIlluminant<Real>{

  typedef BaseIlluminant<Real> Illuminant;
  typedef Colour_xyY<Real> xyY;

public:
  IlluminantF11() : Illuminant(xyY(0.38052, 0.37713)){ }
};


template <class Real, class Observer> class IlluminantF12;

template <class Real>
class IlluminantF12<Real, _1964_10> : public BaseIlluminant<Real>{

  typedef BaseIlluminant<Real> Illuminant;
  typedef Colour_xyY<Real> xyY;

public:
  IlluminantF12() : Illuminant(xyY(0.44256, 0.39717)){ }
};

template <class Real>
class IlluminantF12<Real, _1931_2> : public BaseIlluminant<Real>{

  typedef BaseIlluminant<Real> Illuminant;
  typedef Colour_xyY<Real> xyY;

public:
  IlluminantF12() : Illuminant(xyY(0.43695, 0.40441)){ }
};

#endif
