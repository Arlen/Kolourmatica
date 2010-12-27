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


#include "ConversionConsole.hpp"
//#include "Viewer.hpp"

#include <QtGui/QDoubleValidator>
#include <QtCore/QStringList>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QScrollArea>
#include <QtGui/QPushButton>
#include <QtGui/QLineEdit>
#include <QtGui/QComboBox>
#include <QtGui/QLabel>

#include <boost/foreach.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/sequence/intrinsic/at_c.hpp>
#include <boost/fusion/iterator/advance.hpp>
#include <boost/fusion/view/iterator_range.hpp>
#include <boost/fusion/sequence/intrinsic/begin.hpp>

#include <typeinfo>
#include <vector>

using boost::fusion::for_each;
using boost::fusion::at_c;
using std::vector;


ConversionConsole::ConversionConsole() : QWidget(){

  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  colourSpaces_ << "XYZ" << "xyY" << "Lab" << "LCHab" << "Luv" << "LCHuv"
		<< "Adobe RGB" << "Apple RGB" << "Best RGB" << "Beta RGB"
		<< "Bruce RGB" << "CIE RGB" << "ColorMatch RGB"
		<< "Don RGB 4" << "ECI RGB" << "Ekta Space PS5"
		<< "NTSC RGB" << "PAL/SECAM RGB" << "ProPhoto RGB"
		<< "SMPTE-C RGB" << "sRGB" << "Wide Gamut RGB";

  referenceWhites_ << "A" << "B" << "C" << "D50" << "D55" << "D65"
		   << "D75" << "E" << "F2" << "F7" << "F11";
  //pViewer_ = 0;
  initWidgets();
  connectConversionButtons();
  connectWorkingColourSpaceButton();
  connectSystemColourSpaceButton();
  connectRefWhiteButton();
  connectAdaptationButton();
}


/* private */
void ConversionConsole::setViewer(Viewer* pViewer){

  // if(pViewer){
  //   pViewer_ = pViewer;
  //   pViewer_->SetWorkingColourSpace(workingColourSpace_);
  //   pViewer_->SetSystemColourSpace(systemColourSpace_);
  //   pViewer_->SetReferenceWhite(refWhite_);
  //   pViewer_->SetAdaptationMethod(adaptationMethod_);
  // }
}

void ConversionConsole::initWidgets(){

  QWidget* pWidget = new QWidget;

  pLayout0_ = new QVBoxLayout(this);
  pLayout0_->setContentsMargins(0, 0, 0, 0);
  pLayout0_->setSpacing(0);

  pLayout1A_ = new QHBoxLayout;
  pLayout1A_->setContentsMargins(0, 0, 0, 0);
  pLayout1A_->setSpacing(1);

  pLayout1B_ = new QGridLayout(pWidget);
  pLayout1B_->setContentsMargins(0, 2, 0, 2);
  pLayout1B_->setSpacing(1);

  pScrollArea_ = new QScrollArea;
  pScrollArea_->setFrameShape(QFrame::Panel);
  pScrollArea_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  pScrollArea_->setAlignment(Qt::AlignHCenter);
  pScrollArea_->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

  pLayout0_->addLayout(pLayout1A_);
  pLayout0_->addWidget(pScrollArea_);

  pWorkingColourSpace_   = new QLabel("  Working Colour Space ");
  pSystemColourSpace_    = new QLabel("  System Colour Space ");
  pReferenceWhite_      = new QLabel("  Reference White ");
  pChromaticAdaptation_ = new QLabel("  Chromatic Adaptation ");

  pWorkingColourSpaces_   = new QComboBox;
  pSystemColourSpaces_    = new QComboBox;
  pReferenceWhites_      = new QComboBox;
  pChromaticAdaptations_ = new QComboBox;

  BOOST_FOREACH(QString& rS, colourSpaces_){
    pWorkingColourSpaces_->addItem(rS);
    pSystemColourSpaces_->addItem(rS);
  }

  BOOST_FOREACH(QString& rS, referenceWhites_){
    pReferenceWhites_->addItem(rS);
  }

  pChromaticAdaptations_->addItem("XYZ Scaling");
  pChromaticAdaptations_->addItem("Von Kries");
  pChromaticAdaptations_->addItem("Bradford");

  pLayout1A_->addWidget(pWorkingColourSpace_);
  pLayout1A_->addWidget(pWorkingColourSpaces_);

  pLayout1A_->addWidget(pSystemColourSpace_);
  pLayout1A_->addWidget(pSystemColourSpaces_);

  pLayout1A_->addWidget(pReferenceWhite_);
  pLayout1A_->addWidget(pReferenceWhites_);

  pLayout1A_->addWidget(pChromaticAdaptation_);
  pLayout1A_->addWidget(pChromaticAdaptations_);
  pLayout1A_->addItem(new QSpacerItem(1, 1,
  				      QSizePolicy::MinimumExpanding,
  				      QSizePolicy::Ignored));

  int i = 0;
  BOOST_FOREACH(QString& rS, colourSpaces_){
    inputLines_[i].get<0>() = new QPushButton(rS);
    ++i;
  }

  int col = 0;
  BOOST_FOREACH(InputLine& rL, inputLines_){
    rL.get<0>()->setFixedSize(144, 24);
    rL.get<1>() = new QLineEdit;
    rL.get<1>()->setFixedSize(128, 24);
    setDoubleValidator(rL.get<1>());
    rL.get<2>() = new QLineEdit;
    rL.get<2>()->setFixedSize(128, 24);
    setDoubleValidator(rL.get<2>());
    rL.get<3>() = new QLineEdit;
    rL.get<3>()->setFixedSize(128, 24);
    setDoubleValidator(rL.get<3>());

    pLayout1B_->addItem(new QSpacerItem(128, 1,
					QSizePolicy::MinimumExpanding,
					QSizePolicy::Ignored), col, 0);
    pLayout1B_->addWidget(rL.get<0>(), col, 1);
    pLayout1B_->addWidget(rL.get<1>(), col, 2);
    pLayout1B_->addWidget(rL.get<2>(), col, 3);
    pLayout1B_->addWidget(rL.get<3>(), col, 4);
    pLayout1B_->addItem(new QSpacerItem(128, 1,
					QSizePolicy::MinimumExpanding,
					QSizePolicy::Ignored), col, 5);
    ++col;
  }

  clearInputs();
  pWorkingColourSpaces_->setCurrentIndex(1);
  pSystemColourSpaces_->setCurrentIndex(20);
  pReferenceWhites_->setCurrentIndex(5);
  pChromaticAdaptations_->setCurrentIndex(2);

  pScrollArea_->setWidget(pWidget);
  pLayout0_->setStretch(1, 1);
}

void ConversionConsole::clearInputs(){

  BOOST_FOREACH(InputLine& rL, inputLines_){
    rL.get<1>()->setText(QString::number(0.0, 'f', 10));
    rL.get<2>()->setText(QString::number(0.0, 'f', 10));
    rL.get<3>()->setText(QString::number(0.0, 'f', 10));
  }
}

void ConversionConsole::connectConversionButtons(){

  QObject::connect(inputLines_[0].get<0>(), SIGNAL(clicked()),
  		   this, SLOT(convertFrom_XYZ_To_all()));
  QObject::connect(inputLines_[1].get<0>(), SIGNAL(clicked()),
  		   this, SLOT(convertFrom_xyY_To_all()));
  QObject::connect(inputLines_[2].get<0>(), SIGNAL(clicked()),
  		   this, SLOT(convertFrom_Lab_To_all()));
  QObject::connect(inputLines_[3].get<0>(), SIGNAL(clicked()),
  		   this, SLOT(convertFrom_LCHab_To_all()));
  QObject::connect(inputLines_[4].get<0>(), SIGNAL(clicked()),
  		   this, SLOT(convertFrom_Luv_To_all()));
  QObject::connect(inputLines_[5].get<0>(), SIGNAL(clicked()),
  		   this, SLOT(convertFrom_LCHuv_To_all()));
  QObject::connect(inputLines_[6].get<0>(), SIGNAL(clicked()),
  		   this, SLOT(convertFrom_Adobe_To_all()));
  QObject::connect(inputLines_[7].get<0>(), SIGNAL(clicked()),
  		   this, SLOT(convertFrom_Apple_To_all()));
  QObject::connect(inputLines_[8].get<0>(), SIGNAL(clicked()),
  		   this, SLOT(convertFrom_Best_To_all()));
  QObject::connect(inputLines_[9].get<0>(), SIGNAL(clicked()),
  		   this, SLOT(convertFrom_Beta_To_all()));
  QObject::connect(inputLines_[10].get<0>(), SIGNAL(clicked()),
  		   this, SLOT(convertFrom_Bruce_To_all()));
  QObject::connect(inputLines_[11].get<0>(), SIGNAL(clicked()),
  		   this, SLOT(convertFrom_CIE_To_all()));
  QObject::connect(inputLines_[12].get<0>(), SIGNAL(clicked()),
  		   this, SLOT(convertFrom_ColorMatch_To_all()));
  QObject::connect(inputLines_[13].get<0>(), SIGNAL(clicked()),
  		   this, SLOT(convertFrom_Don4_To_all()));
  QObject::connect(inputLines_[14].get<0>(), SIGNAL(clicked()),
  		   this, SLOT(convertFrom_ECI_To_all()));
  QObject::connect(inputLines_[15].get<0>(), SIGNAL(clicked()),
  		   this, SLOT(convertFrom_EktaSpacePS5_To_all()));
  QObject::connect(inputLines_[16].get<0>(), SIGNAL(clicked()),
  		   this, SLOT(convertFrom_NTSC_To_all()));
  QObject::connect(inputLines_[17].get<0>(), SIGNAL(clicked()),
  		   this, SLOT(convertFrom_PALSECAM_To_all()));
  QObject::connect(inputLines_[18].get<0>(), SIGNAL(clicked()),
  		   this, SLOT(convertFrom_ProPhoto_To_all()));
  QObject::connect(inputLines_[19].get<0>(), SIGNAL(clicked()),
  		   this, SLOT(convertFrom_SMPTEC_To_all()));
  QObject::connect(inputLines_[20].get<0>(), SIGNAL(clicked()),
  		   this, SLOT(convertFrom_sRGB_To_all()));
  QObject::connect(inputLines_[21].get<0>(), SIGNAL(clicked()),
  		   this, SLOT(convertFrom_WideGamut_To_all()));
}

Vector3d ConversionConsole::readSource(int index){

  return Vector3d(inputLines_[index].get<1>()->text().toFloat(),
		  inputLines_[index].get<2>()->text().toFloat(),
		  inputLines_[index].get<3>()->text().toFloat());
}

void ConversionConsole::writeResults(std::vector<Eigen::Vector3d>& results){

  int i = 0;
  BOOST_FOREACH(InputLine& rL, inputLines_){
    rL.get<1>()->setText(QString::number(results[i](0), 'f', 12));
    rL.get<2>()->setText(QString::number(results[i](1), 'f', 12));
    rL.get<3>()->setText(QString::number(results[i](2), 'f', 12));
    i++;
  }
}

void ConversionConsole::connectWorkingColourSpaceButton(){

  QObject::connect(pWorkingColourSpaces_,
		   SIGNAL(currentIndexChanged(int)),
		   this, SLOT(setWorkingColourSpace(int)));
  setWorkingColourSpace(pWorkingColourSpaces_->currentIndex());
}

void ConversionConsole::connectSystemColourSpaceButton(){

  QObject::connect(pSystemColourSpaces_,
		   SIGNAL(currentIndexChanged(int)),
		   this, SLOT(setSystemColourSpace(int)));
  setSystemColourSpace(pSystemColourSpaces_->currentIndex());
}

void ConversionConsole::connectRefWhiteButton(){

  QObject::connect(pReferenceWhites_, SIGNAL(currentIndexChanged(int)),
		   this, SLOT(setRefWhite(int)));
  setRefWhite(pReferenceWhites_->currentIndex());
}

void ConversionConsole::connectAdaptationButton(){

  QObject::connect(pChromaticAdaptations_,
		   SIGNAL(currentIndexChanged(int)),
		   this, SLOT(setAdaptationMethod(int)));
  setAdaptationMethod(pChromaticAdaptations_->currentIndex());
}

void ConversionConsole::setDoubleValidator(QLineEdit* const pLineEdit){

  QDoubleValidator *pDoubleValidator = new QDoubleValidator(pLineEdit);
  pDoubleValidator->setNotation(QDoubleValidator::StandardNotation);
  pDoubleValidator->setDecimals(10);
  pLineEdit->setValidator(pDoubleValidator);
}


/* private slots */
template <class ColourSpace>
class ConvertFrom{
  
public:

  ConvertFrom(ColourSpace& from, vector<Vector3d>& result) :
  from_(&from), result_(&result){ }

  template <class T>
  void operator()(const T& to) const{
    if(typeid(ColourSpace) == typeid(T) ){
      result_->push_back( from_->position() );
      return;
    }
    result_->push_back( to.operator()(*from_).position() );
  }
  
  ColourSpace* const from_;
  vector<Vector3d>* const result_;
};

void ConversionConsole::convertFrom_XYZ_To_all(){

  XYZ xyz(readSource(CSXYZ));
  vector<Vector3d> result;
  for_each(allColourSpaces_, ConvertFrom<XYZ>(xyz, result));
  writeResults(result);
}

void ConversionConsole::convertFrom_xyY_To_all(){
  
  xyY xyy(readSource(CSxyY));
  vector<Vector3d> result;
  for_each(allColourSpaces_, ConvertFrom<xyY>(xyy, result));
  writeResults(result);
}

void ConversionConsole::convertFrom_Lab_To_all(){

  Lab lab(readSource(CSLab));
  vector<Vector3d> result;
  for_each(allColourSpaces_, ConvertFrom<Lab>(lab, result));
  writeResults(result);
}

void ConversionConsole::convertFrom_LCHab_To_all(){

  LCHab lchab(readSource(CSLCHab));
  vector<Vector3d> result;
  for_each(allColourSpaces_, ConvertFrom<LCHab>(lchab, result));
  writeResults(result);
}

void ConversionConsole::convertFrom_Luv_To_all(){

  Luv luv(readSource(CSLuv));
  vector<Vector3d> result;
  for_each(allColourSpaces_, ConvertFrom<Luv>(luv, result));
  writeResults(result);
}

void ConversionConsole::convertFrom_LCHuv_To_all(){

  LCHuv lchuv(readSource(CSLCHuv));
  vector<Vector3d> result;
  for_each(allColourSpaces_, ConvertFrom<LCHuv>(lchuv, result));
  writeResults(result);
}

void ConversionConsole::convertFrom_Adobe_To_all(){

  vector<Vector3d> result;
  AdobeRGB adobe(at_c<CSAdobeRGB>(allColourSpaces_).
		 operator()(readSource(CSAdobeRGB)));
  for_each(allColourSpaces_, ConvertFrom<AdobeRGB>(adobe, result));
  writeResults(result);
}

void ConversionConsole::convertFrom_Apple_To_all(){

  vector<Vector3d> result;
  AppleRGB apple(at_c<CSAppleRGB>(allColourSpaces_).
		 operator()(readSource(CSAppleRGB)));
  for_each(allColourSpaces_, ConvertFrom<AppleRGB>(apple, result));
  writeResults(result);
}

void ConversionConsole::convertFrom_Best_To_all(){

  vector<Vector3d> result;
  BestRGB best(at_c<CSBestRGB>(allColourSpaces_).
	       operator()(readSource(CSBestRGB)));
  for_each(allColourSpaces_, ConvertFrom<BestRGB>(best, result));
  writeResults(result);
}

void ConversionConsole::convertFrom_Beta_To_all(){

  vector<Vector3d> result;
  BetaRGB beta(at_c<CSBetaRGB>(allColourSpaces_).
	       operator()(readSource(CSBetaRGB)));
  for_each(allColourSpaces_, ConvertFrom<BetaRGB>(beta, result));
  writeResults(result);
}

void ConversionConsole::convertFrom_Bruce_To_all(){

  vector<Vector3d> result;
  BruceRGB bruce(at_c<CSBruceRGB>(allColourSpaces_).
		 operator()(readSource(CSBruceRGB)));
  for_each(allColourSpaces_, ConvertFrom<BruceRGB>(bruce, result));
  writeResults(result);
}

void ConversionConsole::convertFrom_CIE_To_all(){

  vector<Vector3d> result;
  CIERGB cie(at_c<CSCIERGB>(allColourSpaces_).
	     operator()(readSource(CSCIERGB)));
  for_each(allColourSpaces_, ConvertFrom<CIERGB>(cie, result));
  writeResults(result);
}

void ConversionConsole::convertFrom_ColorMatch_To_all(){

  vector<Vector3d> result;
  ColorMatchRGB colormatch(at_c<CSColorMatchRGB>(allColourSpaces_).
			   operator()(readSource(CSColorMatchRGB)));
  for_each(allColourSpaces_, ConvertFrom<ColorMatchRGB>(colormatch, result));
  writeResults(result);
}

void ConversionConsole::convertFrom_Don4_To_all(){

  vector<Vector3d> result;
  DonRGB4 don4(at_c<CSDonRGB4>(allColourSpaces_).
	       operator()(readSource(CSDonRGB4)));
  for_each(allColourSpaces_, ConvertFrom<DonRGB4>(don4, result));
  writeResults(result);
}

void ConversionConsole::convertFrom_ECI_To_all(){

  vector<Vector3d> result;
  ECIRGB eci(at_c<CSECIRGB>(allColourSpaces_).
	     operator()(readSource(CSECIRGB)));
  for_each(allColourSpaces_, ConvertFrom<ECIRGB>(eci, result));
  writeResults(result);
}

void ConversionConsole::convertFrom_EktaSpacePS5_To_all(){

  vector<Vector3d> result;
  EktaSpacePS5 ektaspace(at_c<CSEktaSpacePS5>(allColourSpaces_).
			 operator()(readSource(CSEktaSpacePS5)));
  for_each(allColourSpaces_, ConvertFrom<EktaSpacePS5>(ektaspace, result));
  writeResults(result);
}

void ConversionConsole::convertFrom_NTSC_To_all(){

  vector<Vector3d> result;
  NTSCRGB ntsc(at_c<CSNTSCRGB>(allColourSpaces_).
	       operator()(readSource(CSNTSCRGB)));
  for_each(allColourSpaces_, ConvertFrom<NTSCRGB>(ntsc, result));
  writeResults(result);
}

void ConversionConsole::convertFrom_PALSECAM_To_all(){

  vector<Vector3d> result;
  PAL_SECAMRGB pal_secam(at_c<CSPALSECAMRGB>(allColourSpaces_).
			 operator()(readSource(CSPALSECAMRGB)));
  for_each(allColourSpaces_, ConvertFrom<PAL_SECAMRGB>(pal_secam, result));
  writeResults(result);
}

void ConversionConsole::convertFrom_ProPhoto_To_all(){

  vector<Vector3d> result;
  ProPhotoRGB prophoto(at_c<CSProPhotoRGB>(allColourSpaces_).
		       operator()(readSource(CSProPhotoRGB)));
  for_each(allColourSpaces_, ConvertFrom<ProPhotoRGB>(prophoto, result));
  writeResults(result);
}

void ConversionConsole::convertFrom_SMPTEC_To_all(){

  vector<Vector3d> result;
  SMPTE_CRGB smpte_c(at_c<CSSMPTECRGB>(allColourSpaces_).
		     operator()(readSource(CSSMPTECRGB)));
  for_each(allColourSpaces_, ConvertFrom<SMPTE_CRGB>(smpte_c, result));
  writeResults(result);
}

void ConversionConsole::convertFrom_sRGB_To_all(){

  vector<Vector3d> result;
  sRGB srgb(at_c<CSsRGB>(allColourSpaces_).
	    operator()(readSource(CSsRGB)));
  for_each(allColourSpaces_, ConvertFrom<sRGB>(srgb, result));
  writeResults(result);
}

void ConversionConsole::convertFrom_WideGamut_To_all(){

  vector<Vector3d> result;
  WideGamutRGB widegamut(at_c<CSWideGamutRGB>(allColourSpaces_).
			 operator()(readSource(CSWideGamutRGB)));
  for_each(allColourSpaces_, ConvertFrom<WideGamutRGB>(widegamut, result));
  writeResults(result);
}

void ConversionConsole::setWorkingColourSpace(int wcs){

  int tmp = workingColourSpace_;
  workingColourSpace_ = wcs;

  if(workingColourSpace_ == systemColourSpace_){
    workingColourSpace_ = tmp;
    pWorkingColourSpaces_->setCurrentIndex(workingColourSpace_);
    return ;
  }

  // Manager::Instance().SetWorkingColourSpace(workingColourSpace_);
  // if(pViewer_){
  //   pViewer_->SetWorkingColourSpace(workingColourSpace_);
  // }
}

void ConversionConsole::setSystemColourSpace(int scs){

  int tmp = systemColourSpace_;
  systemColourSpace_ = scs;

  if(systemColourSpace_ == workingColourSpace_){
    systemColourSpace_ = tmp;
    pSystemColourSpaces_->setCurrentIndex(systemColourSpace_);
    return;
  }

  // Manager::Instance().SetSystemColourSpace(systemColourSpace_);
  // if(pViewer_){
  //   pViewer_->SetSystemColourSpace(systemColourSpace_);
  // }
}


struct UpdateRefWhite{

  UpdateRefWhite(ReferenceWhite<double>& rw, Matrix<double, 3, 3>& ad ) :
    rw_(&rw), ad_(&ad){ }

  template <class T>
  void operator()(T& cs) const{
    cs.adapt(*rw_, *ad_);
  }

  ReferenceWhite<double>* rw_;
  Matrix<double, 3, 3>* ad_;
};

void ConversionConsole::setRefWhite(int r){

  switch(r){
  case 0:  refWhite_.setIlluminant(IlluminantA<double>());    break;
  case 1:  refWhite_.setIlluminant(IlluminantB<double>());    break;
  case 2:  refWhite_.setIlluminant(IlluminantC<double>());    break;
  case 3:  refWhite_.setIlluminant(IlluminantD50<double>());  break;
  case 4:  refWhite_.setIlluminant(IlluminantD55<double>());  break;
  case 5:  refWhite_.setIlluminant(IlluminantD65<double>());  break;
  case 6:  refWhite_.setIlluminant(IlluminantD75<double>());  break;
  case 7:  refWhite_.setIlluminant(IlluminantE<double>());    break;
  case 8:  refWhite_.setIlluminant(IlluminantF2<double>());   break;
  case 9:  refWhite_.setIlluminant(IlluminantF7<double>());   break;
  case 10: refWhite_.setIlluminant(IlluminantF11<double>());  break;
  }

  GRW::value_ = refWhite_;

  // using namespace boost::fusion;
  // using boost::fusion::for_each;

  // typedef result_of::begin<ColourSpaces>::type Begin;
  // typedef result_of::advance_c<Begin, 6>::type From;
  // typedef result_of::advance_c<Begin, 22>::type To;

  // From from(fusion::advance_c<6>(fusion::begin(allColourSpaces_) ) );
  // To to(fusion::advance_c<22>(fusion::begin(allColourSpaces_) ) );
  // fusion::iterator_range<From, To> it(from, to);
  // for_each(it, UpdateRefWhite(refWhite_, adaptationMethod_));


  using namespace boost::fusion;
  using boost::fusion::begin;
  using boost::fusion::for_each;

  fusion::iterator_range<
    result_of::advance_c<result_of::begin<ColourSpaces>::type, 6>::type,
    result_of::advance_c<result_of::begin<ColourSpaces>::type, 22>::type
    > it(advance_c<6 >(begin(allColourSpaces_)),
	 advance_c<22>(begin(allColourSpaces_)));

  for_each(it, UpdateRefWhite(refWhite_, adaptationMethod_));

  //Manager::Instance().SetReferenceWhite(refWhite_);
  // if(pViewer_){
  //   pViewer_->SetReferenceWhite(refWhite_);
  // }
}

void ConversionConsole::setAdaptationMethod(int a){

  switch(a){
  case 0: adaptationMethod_ = AdaptationMethod<double>::XYZScaling_; break;
  case 1: adaptationMethod_ = AdaptationMethod<double>::VonKries_;   break;
  case 2: adaptationMethod_ = AdaptationMethod<double>::Bradford_;   break;
  }

  using namespace boost::fusion;
  using boost::fusion::begin;
  using boost::fusion::for_each;

  fusion::iterator_range<
    result_of::advance_c<result_of::begin<ColourSpaces>::type, 6>::type,
    result_of::advance_c<result_of::begin<ColourSpaces>::type, 22>::type
    > it(advance_c<6 >(begin(allColourSpaces_)),
	 advance_c<22>(begin(allColourSpaces_)));

  for_each(it, UpdateRefWhite(refWhite_, adaptationMethod_));

  //Manager::Instance().SetAdaptationMethod(adaptationMethod_);
  // if(pViewer_){
  //   pViewer_->SetAdaptationMethod(adaptationMethod_);
  // }
}
