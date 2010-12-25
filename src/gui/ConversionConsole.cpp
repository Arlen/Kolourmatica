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


ConversionConsole::ConversionConsole() : QWidget(){

  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  colorSpaces_ << "XYZ" << "xyY" << "Lab" << "LCHab" << "Luv" << "LCHuv"
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
  connectWorkingColorSpaceButton();
  connectSystemColorSpaceButton();
  connectRefWhiteButton();
  connectAdaptationButton();
}

/* private */
void ConversionConsole::setViewer(Viewer* pViewer){

  // if(pViewer){
  //   pViewer_ = pViewer;
  //   pViewer_->SetWorkingColorSpace(workingColorSpace_);
  //   pViewer_->SetSystemColorSpace(systemColorSpace_);
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

  pWorkingColorSpace_   = new QLabel("  Working Color Space ");
  pSystemColorSpace_    = new QLabel("  System Color Space ");
  pReferenceWhite_      = new QLabel("  Reference White ");
  pChromaticAdaptation_ = new QLabel("  Chromatic Adaptation ");

  pWorkingColorSpaces_   = new QComboBox;
  pSystemColorSpaces_    = new QComboBox;
  pReferenceWhites_      = new QComboBox;
  pChromaticAdaptations_ = new QComboBox;

  BOOST_FOREACH(QString& rS, colorSpaces_){
    pWorkingColorSpaces_->addItem(rS);
    pSystemColorSpaces_->addItem(rS);
  }

  BOOST_FOREACH(QString& rS, referenceWhites_){
    pReferenceWhites_->addItem(rS);
  }

  pChromaticAdaptations_->addItem("XYZ Scaling");
  pChromaticAdaptations_->addItem("Von Kries");
  pChromaticAdaptations_->addItem("Bradford");

  pLayout1A_->addWidget(pWorkingColorSpace_);
  pLayout1A_->addWidget(pWorkingColorSpaces_);

  pLayout1A_->addWidget(pSystemColorSpace_);
  pLayout1A_->addWidget(pSystemColorSpaces_);

  pLayout1A_->addWidget(pReferenceWhite_);
  pLayout1A_->addWidget(pReferenceWhites_);

  pLayout1A_->addWidget(pChromaticAdaptation_);
  pLayout1A_->addWidget(pChromaticAdaptations_);
  pLayout1A_->addItem(new QSpacerItem(1, 1,
  				      QSizePolicy::MinimumExpanding,
  				      QSizePolicy::Ignored));

  int i = 0;
  BOOST_FOREACH(QString& rS, colorSpaces_){
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
  pWorkingColorSpaces_->setCurrentIndex(1);
  pSystemColorSpaces_->setCurrentIndex(20);
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
  		   this, SLOT(ConvertFrom_XYZ_To_all()));
  QObject::connect(inputLines_[1].get<0>(), SIGNAL(clicked()),
  		   this, SLOT(ConvertFrom_xyY_To_all()));
  QObject::connect(inputLines_[2].get<0>(), SIGNAL(clicked()),
  		   this, SLOT(ConvertFrom_Lab_To_all()));
  QObject::connect(inputLines_[3].get<0>(), SIGNAL(clicked()),
  		   this, SLOT(ConvertFrom_LCHab_To_all()));
  QObject::connect(inputLines_[4].get<0>(), SIGNAL(clicked()),
  		   this, SLOT(ConvertFrom_Luv_To_all()));
  QObject::connect(inputLines_[5].get<0>(), SIGNAL(clicked()),
  		   this, SLOT(ConvertFrom_LCHuv_To_all()));
  QObject::connect(inputLines_[6].get<0>(), SIGNAL(clicked()),
  		   this, SLOT(ConvertFrom_Adobe_To_all()));
  QObject::connect(inputLines_[7].get<0>(), SIGNAL(clicked()),
  		   this, SLOT(ConvertFrom_Apple_To_all()));
  QObject::connect(inputLines_[8].get<0>(), SIGNAL(clicked()),
  		   this, SLOT(ConvertFrom_Best_To_all()));
  QObject::connect(inputLines_[9].get<0>(), SIGNAL(clicked()),
  		   this, SLOT(ConvertFrom_Beta_To_all()));
  QObject::connect(inputLines_[10].get<0>(), SIGNAL(clicked()),
  		   this, SLOT(ConvertFrom_Bruce_To_all()));
  QObject::connect(inputLines_[11].get<0>(), SIGNAL(clicked()),
  		   this, SLOT(ConvertFrom_CIE_To_all()));
  QObject::connect(inputLines_[12].get<0>(), SIGNAL(clicked()),
  		   this, SLOT(ConvertFrom_ColorMatch_To_all()));
  QObject::connect(inputLines_[13].get<0>(), SIGNAL(clicked()),
  		   this, SLOT(ConvertFrom_Don4_To_all()));
  QObject::connect(inputLines_[14].get<0>(), SIGNAL(clicked()),
  		   this, SLOT(ConvertFrom_ECI_To_all()));
  QObject::connect(inputLines_[15].get<0>(), SIGNAL(clicked()),
  		   this, SLOT(ConvertFrom_EktaSpacePS5_To_all()));
  QObject::connect(inputLines_[16].get<0>(), SIGNAL(clicked()),
  		   this, SLOT(ConvertFrom_NTSC_To_all()));
  QObject::connect(inputLines_[17].get<0>(), SIGNAL(clicked()),
  		   this, SLOT(ConvertFrom_PALSECAM_To_all()));
  QObject::connect(inputLines_[18].get<0>(), SIGNAL(clicked()),
  		   this, SLOT(ConvertFrom_ProPhoto_To_all()));
  QObject::connect(inputLines_[19].get<0>(), SIGNAL(clicked()),
  		   this, SLOT(ConvertFrom_SMPTEC_To_all()));
  QObject::connect(inputLines_[20].get<0>(), SIGNAL(clicked()),
  		   this, SLOT(ConvertFrom_sRGB_To_all()));
  QObject::connect(inputLines_[21].get<0>(), SIGNAL(clicked()),
  		   this, SLOT(ConvertFrom_WideGamut_To_all()));
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

void ConversionConsole::connectWorkingColorSpaceButton(){

  QObject::connect(pWorkingColorSpaces_,
		   SIGNAL(currentIndexChanged(int)),
		   this, SLOT(setWorkingColorSpace(int)));
  setWorkingColorSpace(pWorkingColorSpaces_->currentIndex());
}

void ConversionConsole::connectSystemColorSpaceButton(){

  QObject::connect(pSystemColorSpaces_,
		   SIGNAL(currentIndexChanged(int)),
		   this, SLOT(setSystemColorSpace(int)));
  setSystemColorSpace(pSystemColorSpaces_->currentIndex());
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

  ConvertFrom(const ColourSpace& from, vector<Vector3d>& result) :
  from_(from), result_(&result){ }

  template <class T>
  void operator()(const T& to) const{
    result_->push_back( to.operator()(from_).position() );
  }
  
  ColourSpace from_;
  vector<Vector3d>* const result_;
};

void ConversionConsole::convertFrom_XYZ_To_all(){

  XYZ xyz(readSource(CSXYZ));
  vector<Vector3d> result;
  /* without reference type the copy-constructor gets called. */
  for_each(allColorSpaces_, ConvertFrom<XYZ&>(xyz, result));
  writeResults(result);
}

void ConversionConsole::convertFrom_xyY_To_all(){
  
  xyY xyy(readSource(CSxyY));
  vector<Vector3d> result;
  for_each(allColorSpaces_, ConvertFrom<xyY&>(xyy, result));
  writeResults(result);
}

void ConversionConsole::convertFrom_Lab_To_all(){

  Lab lab(readSource(CSLab));
  vector<Vector3d> result;
  for_each(allColorSpaces_, ConvertFrom<Lab&>(lab, result));
  writeResults(result);
}

void ConversionConsole::convertFrom_LCHab_To_all(){

  LCHab lchab(readSource(CSLCHab));
  vector<Vector3d> result;
  for_each(allColorSpaces_, ConvertFrom<LCHab&>(lchab, result));
  writeResults(result);
}

void ConversionConsole::convertFrom_Luv_To_all(){

  Luv luv(readSource(CSLuv));
  vector<Vector3d> result;
  for_each(allColorSpaces_, ConvertFrom<Luv&>(luv, result));
  writeResults(result);
}

void ConversionConsole::convertFrom_LCHuv_To_all(){

  LCHuv lchuv(readSource(CSLCHuv));
  vector<Vector3d> result;
  for_each(allColorSpaces_, ConvertFrom<LCHuv&>(lchuv, result));
  writeResults(result);
}

void ConversionConsole::convertFrom_Adobe_To_all(){

  AdobeRGB adobe(readSource(CSAdobeRGB));
  vector<Vector3d> result;
  for_each(allColorSpaces_, ConvertFrom<AdobeRGB&>(adobe, result));
  writeResults(result);
}

void ConversionConsole::convertFrom_Apple_To_all(){

  AppleRGB apple(readSource(CSAppleRGB));
  vector<Vector3d> result;
  for_each(allColorSpaces_, ConvertFrom<AppleRGB&>(apple, result));
  writeResults(result);
}

void ConversionConsole::convertFrom_Best_To_all(){

  BestRGB best(readSource(CSBestRGB));
  vector<Vector3d> result;
  for_each(allColorSpaces_, ConvertFrom<BestRGB&>(best, result));
  writeResults(result);
}

void ConversionConsole::convertFrom_Beta_To_all(){

  BetaRGB beta(readSource(CSBetaRGB));
  vector<Vector3d> result;
  for_each(allColorSpaces_, ConvertFrom<BetaRGB&>(beta, result));
  writeResults(result);
}

void ConversionConsole::convertFrom_Bruce_To_all(){

  BruceRGB bruce(readSource(CSBruceRGB));
  vector<Vector3d> result;
  for_each(allColorSpaces_, ConvertFrom<BruceRGB&>(bruce, result));
  writeResults(result);
}

void ConversionConsole::convertFrom_CIE_To_all(){

  CIERGB cie(readSource(CSCIERGB));
  vector<Vector3d> result;
  for_each(allColorSpaces_, ConvertFrom<CIERGB&>(cie, result));
  writeResults(result);
}

void ConversionConsole::convertFrom_ColorMatch_To_all(){

  ColorMatchRGB colormatch(readSource(CSColorMatchRGB));
  vector<Vector3d> result;
  for_each(allColorSpaces_, ConvertFrom<ColorMatchRGB&>(colormatch, result));
  writeResults(result);
}
void ConversionConsole::convertFrom_Don4_To_all(){

  DonRGB4 don4(readSource(CSDonRGB4));
  vector<Vector3d> result;
  for_each(allColorSpaces_, ConvertFrom<DonRGB4&>(don4, result));
  writeResults(result);
}

void ConversionConsole::convertFrom_ECI_To_all(){

  ECIRGB eci(readSource(CSECIRGB));
  vector<Vector3d> result;
  for_each(allColorSpaces_, ConvertFrom<ECIRGB&>(eci, result));
  writeResults(result);
}

void ConversionConsole::convertFrom_EktaSpacePS5_To_all(){

  EktaSpacePS5 ektaspace(readSource(CSECIRGB));
  vector<Vector3d> result;
  for_each(allColorSpaces_, ConvertFrom<EktaSpacePS5&>(ektaspace, result));
  writeResults(result);
}

void ConversionConsole::convertFrom_NTSC_To_all(){

  NTSCRGB ntsc(readSource(CSNTSCRGB));
  vector<Vector3d> result;
  for_each(allColorSpaces_, ConvertFrom<NTSCRGB&>(ntsc, result));
  writeResults(result);
}

void ConversionConsole::convertFrom_PALSECAM_To_all(){

  PAL_SECAMRGB pal_secam(readSource(CSPALSECAMRGB));
  vector<Vector3d> result;
  for_each(allColorSpaces_, ConvertFrom<PAL_SECAMRGB&>(pal_secam, result));
  writeResults(result);
}

void ConversionConsole::convertFrom_ProPhoto_To_all(){

  ProPhotoRGB prophoto(readSource(CSProPhotoRGB));
  vector<Vector3d> result;
  for_each(allColorSpaces_, ConvertFrom<ProPhotoRGB&>(prophoto, result));
  writeResults(result);
}

void ConversionConsole::convertFrom_SMPTEC_To_all(){

  SMPTE_CRGB smpte_c(readSource(CSSMPTECRGB));
  vector<Vector3d> result;
  for_each(allColorSpaces_, ConvertFrom<SMPTE_CRGB&>(smpte_c, result));
  writeResults(result);
}

void ConversionConsole::convertFrom_sRGB_To_all(){

  sRGB srgb(readSource(CSsRGB));
  vector<Vector3d> result;
  for_each(allColorSpaces_, ConvertFrom<sRGB&>(srgb, result));
  writeResults(result);
}

void ConversionConsole::convertFrom_WideGamut_To_all(){

  WideGamutRGB widegamut(readSource(CSWideGamutRGB));
  vector<Vector3d> result;
  for_each(allColorSpaces_, ConvertFrom<WideGamutRGB&>(widegamut, result));
  writeResults(result);
}

void ConversionConsole::setWorkingColorSpace(int wcs){

  int tmp = workingColorSpace_;
  workingColorSpace_ = wcs;

  if(workingColorSpace_ == systemColorSpace_){
    workingColorSpace_ = tmp;
    pWorkingColorSpaces_->setCurrentIndex(workingColorSpace_);
    return ;
  }

  // Manager::Instance().SetWorkingColorSpace(workingColorSpace_);
  // if(pViewer_){
  //   pViewer_->SetWorkingColorSpace(workingColorSpace_);
  // }
}

void ConversionConsole::setSystemColorSpace(int scs){

  int tmp = systemColorSpace_;
  systemColorSpace_ = scs;

  if(systemColorSpace_ == workingColorSpace_){
    systemColorSpace_ = tmp;
    pSystemColorSpaces_->setCurrentIndex(systemColorSpace_);
    return;
  }

  // Manager::Instance().SetSystemColorSpace(systemColorSpace_);
  // if(pViewer_){
  //   pViewer_->SetSystemColorSpace(systemColorSpace_);
  // }
}

void ConversionConsole::setRefWhite(int r){

  switch(r){
  // case 0:  refWhite_.setReferenceWhite(IlluminantA<double>());    break;
  // case 1:  refWhite_.setReferenceWhite(IlluminantB<double>());    break;
  // case 2:  refWhite_.setReferenceWhite(IlluminantC<double>());    break;
  // case 3:  refWhite_.setReferenceWhite(IlluminantD50<double>());  break;
  // case 4:  refWhite_.setReferenceWhite(IlluminantD55<double>());  break;
  // case 5:  refWhite_.setReferenceWhite(IlluminantD65<double>());  break;
  // case 6:  refWhite_.setReferenceWhite(IlluminantD75<double>());  break;
  // case 7:  refWhite_.setReferenceWhite(IlluminantE<double>());    break;
  // case 8:  refWhite_.setReferenceWhite(IlluminantF2<double>());   break;
  // case 9:  refWhite_.setReferenceWhite(IlluminantF7<double>());   break;
  // case 10: refWhite_.setReferenceWhite(IlluminantF11<double>());  break;
  }
  //Manager::Instance().SetReferenceWhite(refWhite_);
  // if(pViewer_){
  //   pViewer_->SetReferenceWhite(refWhite_);
  // }
}

void ConversionConsole::setAdaptationMethod(int a){

  switch(a){
  // case 0: adaptationMethod_ = AdaptationMethod<double>::XYZScaling_; break;
  // case 1: adaptationMethod_ = AdaptationMethod<double>::VonKries_;   break;
  // case 2: adaptationMethod_ = AdaptationMethod<double>::Bradford_;   break;
  }
  //Manager::Instance().SetAdaptationMethod(adaptationMethod_);
  // if(pViewer_){
  //   pViewer_->SetAdaptationMethod(adaptationMethod_);
  // }
}

