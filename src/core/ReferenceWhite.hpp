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


#ifndef REFERENCEWHITE_HPP
#define REFERENCEWHITE_HPP

#include "ForwardDeclarations.hpp"

#include "../../../eigen/Eigen/Core"
#include "../../../eigen/Eigen/Dense"

#include <boost/mpl/assert.hpp>
#include <boost/noncopyable.hpp>
#include <boost/type_traits/is_floating_point.hpp>

using namespace Eigen;
using namespace boost;


template <class Real>
class BaseIlluminant{

  typedef BaseIlluminant<Real> BaseIllum;
  typedef Space_XYZ<Real> XYZ;
  typedef Matrix<Real, 3, 1> Vector3;

public:
  virtual ~BaseIlluminant(){ }
  const Vector3& position() const{ return rw_.position(); }
  virtual BaseIllum* clone() const = 0;

protected:
  BaseIlluminant(const XYZ& rw) : rw_(rw){ }
  XYZ rw_;
};


template <class Real>
class IlluminantA : public BaseIlluminant<Real>{

  BOOST_MPL_ASSERT(( is_floating_point<Real> ));

  typedef IlluminantA<Real> A;
  typedef BaseIlluminant<Real> BaseIllum;
  typedef Space_XYZ<Real> XYZ;

public:
  IlluminantA() : BaseIllum(XYZ(1.09850, 1.0, 0.35585)){ }
  IlluminantA(const A& that) : BaseIllum(XYZ(that.rw_)){ }
  A* clone() const{
    return new A(*this);
  }
};


template <class Real>
class IlluminantB : public BaseIlluminant<Real>{

  BOOST_MPL_ASSERT(( is_floating_point<Real> ));

  typedef IlluminantB<Real> B;
  typedef BaseIlluminant<Real> BaseIllum;
  typedef Space_XYZ<Real> XYZ;

public:
  IlluminantB() : BaseIllum(XYZ(0.99072, 1.0, 0.85223)){ }
  IlluminantB(const B& that) : BaseIllum(XYZ(that.rw_)){ }
  B* clone() const{
    return new B(*this);
  }
};


template <class Real>
class IlluminantC : public BaseIlluminant<Real>{

  BOOST_MPL_ASSERT(( is_floating_point<Real> ));

  typedef IlluminantC<Real> C;
  typedef BaseIlluminant<Real> BaseIllum;
  typedef Space_XYZ<Real> XYZ;

public:
  IlluminantC() : BaseIllum(XYZ(0.98074, 1.0, 1.18232)){ }
  IlluminantC(const C& that) : BaseIllum(XYZ(that.rw_)){ }
  C* clone() const{
    return new C(*this);
  }
};


template <class Real>
class IlluminantD50 : public BaseIlluminant<Real>{

  BOOST_MPL_ASSERT(( is_floating_point<Real> ));

  typedef IlluminantD50<Real> D50;
  typedef BaseIlluminant<Real> BaseIllum;
  typedef Space_XYZ<Real> XYZ;

public:
  IlluminantD50() : BaseIllum(XYZ(0.96422, 1.0, 0.82521)){ }
  IlluminantD50(const D50& that) : BaseIllum(XYZ(that.rw_)){ }
  D50* clone() const{
    return new D50(*this);
  }
};


template <class Real>
class IlluminantD55 : public BaseIlluminant<Real>{

  BOOST_MPL_ASSERT(( is_floating_point<Real> ));

  typedef IlluminantD55<Real> D55;
  typedef BaseIlluminant<Real> BaseIllum;
  typedef Space_XYZ<Real> XYZ;

public:
  IlluminantD55() : BaseIllum(XYZ(0.95682, 1.0, 0.92149)){ }
  IlluminantD55(const D55& that) : BaseIllum(XYZ(that.rw_)){ }
  D55* clone() const{
    return new D55(*this);
  }
};


template <class Real>
class IlluminantD65 : public BaseIlluminant<Real>{

  BOOST_MPL_ASSERT(( is_floating_point<Real> ));

  typedef IlluminantD65<Real> D65;
  typedef BaseIlluminant<Real> BaseIllum;
  typedef Space_XYZ<Real> XYZ;

public:
  IlluminantD65() : BaseIllum(XYZ(0.95047, 1.0, 1.08883)){ }
  IlluminantD65(const D65& that) : BaseIllum(XYZ(that.rw_)){ }
  D65* clone() const{
    return new D65(*this);
  }
};


template <class Real>
class IlluminantD75 : public BaseIlluminant<Real>{

  BOOST_MPL_ASSERT(( is_floating_point<Real> ));

  typedef IlluminantD75<Real> D75;
  typedef BaseIlluminant<Real> BaseIllum;
  typedef Space_XYZ<Real> XYZ;

public:
  IlluminantD75() : BaseIllum(XYZ(0.94972, 1.0, 1.22638)){ }
  IlluminantD75(const D75& that) : BaseIllum(XYZ(that.rw_)){ }
  D75* clone() const{
    return new D75(*this);
  }
};


template <class Real>
class IlluminantE : public BaseIlluminant<Real>{

  BOOST_MPL_ASSERT(( is_floating_point<Real> ));

  typedef IlluminantE<Real> E;
  typedef BaseIlluminant<Real> BaseIllum;
  typedef Space_XYZ<Real> XYZ;

public:
  IlluminantE() : BaseIllum(XYZ(1.00000, 1.0, 1.00000)){ }
  IlluminantE(const E& that) : BaseIllum(XYZ(that.rw_)){ }
  E* clone() const{
    return new E(*this);
  }
};


template <class Real>
class IlluminantF2 : public BaseIlluminant<Real>{

  BOOST_MPL_ASSERT(( is_floating_point<Real> ));

  typedef IlluminantF2<Real> F2;
  typedef BaseIlluminant<Real> BaseIllum;
  typedef Space_XYZ<Real> XYZ;

public:
  IlluminantF2() : BaseIllum(XYZ(0.99186, 1.0, 0.67393)){ }
  IlluminantF2(const F2& that) : BaseIllum(XYZ(that.rw_)){ }
  F2* clone() const{
    return new F2(*this);
  }
};


template <class Real>
class IlluminantF7 : public BaseIlluminant<Real>{

  BOOST_MPL_ASSERT(( is_floating_point<Real> ));

  typedef IlluminantF7<Real> F7;
  typedef BaseIlluminant<Real> BaseIllum;
  typedef Space_XYZ<Real> XYZ;

public:
  IlluminantF7() : BaseIllum(XYZ(0.95041, 1.0, 1.08747)){ }
  IlluminantF7(const F7& that) : BaseIllum(XYZ(that.rw_)){ }
  F7* clone() const{
    return new F7(*this);
  }
};


template <class Real>
class IlluminantF11 : public BaseIlluminant<Real>{

  BOOST_MPL_ASSERT(( is_floating_point<Real> ));

  typedef IlluminantF11<Real> F11;
  typedef BaseIlluminant<Real> BaseIllum;
  typedef Space_XYZ<Real> XYZ;

public:
  IlluminantF11() : BaseIllum(XYZ(1.00962, 1.0, 0.64350)){ }
  IlluminantF11(const F11& that) : BaseIllum(XYZ(that.rw_)){ }
  F11* clone() const{
    return new F11(*this);
  }
};


template <class Real>
class ReferenceWhite{

  BOOST_MPL_ASSERT(( is_floating_point<Real> ));

  typedef ReferenceWhite<Real> RefWhite;
  typedef BaseIlluminant<Real> BaseIllum;
  typedef Matrix<Real, 3, 1> Vector3;

public:
  ReferenceWhite() : pRefWhite_(0){ pRefWhite_ = new IlluminantD65<Real>; }
  ReferenceWhite(const BaseIllum& illum) : pRefWhite_(0){

    BaseIllum* tmp = illum.clone();
    std::swap(pRefWhite_, tmp);
    delete tmp;
  }

  ReferenceWhite(const RefWhite& rw) : pRefWhite_(0){

    pRefWhite_ = rw.pRefWhite_->clone();
  }

  const RefWhite& operator=(RefWhite rw){

    std::swap(pRefWhite_, rw.pRefWhite_);
    return *this;
  }

  void setReferenceWhite(const BaseIllum& illum){

    BaseIllum* old = illum.clone();
    std::swap(pRefWhite_, old);
    delete old;
  }

  ~ReferenceWhite(){
    if(pRefWhite_)
      delete pRefWhite_;
  }

  const Vector3& position() const{ return pRefWhite_->position(); }

private:
  BaseIllum* pRefWhite_;
};


/* Global Reference White */
template <class Real>
struct GlobalReferenceWhite : noncopyable{

  BOOST_MPL_ASSERT(( is_floating_point<Real> ));

  static ReferenceWhite<Real> value_;
};

template <class Real>
ReferenceWhite<Real> GlobalReferenceWhite<Real>::value_;


#endif
