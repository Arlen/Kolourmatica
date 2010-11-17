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
#include "Viewer.hpp"
#include "../core/Convert.hpp"

#include <QtGui/QDoubleValidator>
#include <QtCore/QStringList>
#include <QtGui/QGridLayout>
#include <QtGui/QPushButton>
#include <QtGui/QLineEdit>
#include <QtGui/QComboBox>
#include <QtGui/QLabel>

#include <boost/foreach.hpp>

#include <iostream>


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
  InitWidgets();
  ConnectConversionButtons();
  ConnectWorkingColorSpaceButton();
  ConnectSystemColorSpaceButton();
  ConnectRefWhiteButton();
  ConnectAdaptationButton();
}

void ConversionConsole::SetViewer(Viewer* pViewer){

  if(pViewer){
    pViewer_ = pViewer;
    pViewer_->SetWorkingColorSpace(workingColorSpace_);
    pViewer_->SetSystemColorSpace(systemColorSpace_);
    pViewer_->SetReferenceWhite(refWhite_);
    pViewer_->SetAdaptationMethod(adaptationMethod_);
  }
}

void ConversionConsole::InitWidgets(){

  pMainLayout_ = new QGridLayout(this);
  pMainLayout_->setContentsMargins(0, 2, 0, 2);
  pMainLayout_->setSpacing(1);

  pWorkingColorSpace_ = new QLabel("Working Color Space");
  pSystemColorSpace_ = new QLabel("System Color Space");
  pReferenceWhite_ = new QLabel("Reference White");
  pChromaticAdaptation_ = new QLabel("Chromatic Adaptation");
  pWorkingColorSpaces_ = new QComboBox;
  pWorkingColorSpaces_->setMaximumWidth(128);
  pSystemColorSpaces_ = new QComboBox;
  pSystemColorSpaces_->setMaximumWidth(128);
  pReferenceWhites_ = new QComboBox;
  pReferenceWhites_->setMaximumWidth(128);
  pChromaticAdaptations_ = new QComboBox;
  pChromaticAdaptations_->setMaximumWidth(128);

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

  pMainLayout_->addItem(new QSpacerItem(128, 1,
					QSizePolicy::MinimumExpanding,
					QSizePolicy::Minimum), 0, 0);
  pMainLayout_->addWidget(pWorkingColorSpace_, 0, 1);
  pMainLayout_->addWidget(pWorkingColorSpaces_, 0, 2);
  pMainLayout_->addItem(new QSpacerItem(128, 1,
					QSizePolicy::MinimumExpanding,
					QSizePolicy::Minimum), 0, 6);

  pMainLayout_->addItem(new QSpacerItem(128, 1,
					QSizePolicy::MinimumExpanding,
					QSizePolicy::Minimum), 1, 0);
  pMainLayout_->addWidget(pSystemColorSpace_, 1, 1);
  pMainLayout_->addWidget(pSystemColorSpaces_, 1, 2);
  pMainLayout_->addItem(new QSpacerItem(128, 1,
					QSizePolicy::MinimumExpanding,
					QSizePolicy::Minimum), 1, 6);

  pMainLayout_->addItem(new QSpacerItem(128, 1,
					QSizePolicy::MinimumExpanding,
					QSizePolicy::Minimum), 2, 0);
  pMainLayout_->addWidget(pReferenceWhite_, 2, 1);
  pMainLayout_->addWidget(pReferenceWhites_, 2, 2);
  pMainLayout_->addItem(new QSpacerItem(128, 1,
					QSizePolicy::MinimumExpanding,
					QSizePolicy::Minimum), 2, 6);

  pMainLayout_->addItem(new QSpacerItem(128, 1,
					QSizePolicy::MinimumExpanding,
					QSizePolicy::Minimum), 3, 0);
  pMainLayout_->addWidget(pChromaticAdaptation_, 3, 1);
  pMainLayout_->addWidget(pChromaticAdaptations_, 3, 2);
  pMainLayout_->addItem(new QSpacerItem(128, 1,
					QSizePolicy::MinimumExpanding,
					QSizePolicy::Minimum), 3, 6);

  int i = 0;
  BOOST_FOREACH(QString& rS, colorSpaces_){
    inputLines_[i].get<0>() = new QPushButton(rS);
    ++i;
  }

  int col = 4;
  BOOST_FOREACH(InputLine& rL, inputLines_){
    rL.get<0>()->setFixedSize(144, 24);
    rL.get<1>() = new QLineEdit;
    rL.get<1>()->setFixedSize(128, 24);
    SetDoubleValidator(rL.get<1>());
    rL.get<2>() = new QLineEdit;
    rL.get<2>()->setFixedSize(128, 24);
    SetDoubleValidator(rL.get<2>());
    rL.get<3>() = new QLineEdit;
    rL.get<3>()->setFixedSize(128, 24);
    SetDoubleValidator(rL.get<3>());

    pMainLayout_->addItem(new QSpacerItem(128, 1,
    					  QSizePolicy::MinimumExpanding,
    					  QSizePolicy::Minimum), col, 0);
    pMainLayout_->addWidget(rL.get<0>(), col, 1);
    pMainLayout_->addWidget(rL.get<1>(), col, 2);
    pMainLayout_->addWidget(rL.get<2>(), col, 3);
    pMainLayout_->addWidget(rL.get<3>(), col, 4);
    pMainLayout_->addItem(new QSpacerItem(128, 1,
    					  QSizePolicy::MinimumExpanding,
    					  QSizePolicy::Minimum), col, 5);
    ++col;
  }
  ClearInputs();
  pWorkingColorSpaces_->setCurrentIndex(1);
  pSystemColorSpaces_->setCurrentIndex(20);
  pReferenceWhites_->setCurrentIndex(5);
  pChromaticAdaptations_->setCurrentIndex(2);

  pViewer_ = NULL;
}

void ConversionConsole::ClearInputs(){

  BOOST_FOREACH(InputLine& rL, inputLines_){
    rL.get<1>()->setText(QString::number(0.0, 'f', 10));
    rL.get<2>()->setText(QString::number(0.0, 'f', 10));
    rL.get<3>()->setText(QString::number(0.0, 'f', 10));
  }
}

void ConversionConsole::ConnectConversionButtons(){

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

void ConversionConsole::ConvertAll(){

  Eigen::Vector3f tmp;
  Eigen::Matrix3f ada = adaptationMethod_;
  Eigen::Vector3f r = refWhite_;
  boost::array<Vector3f, 22> out;

  Input input;
  Conversions con;
  Eigen::Vector3f (*c)(const Input&);

  tmp <<
    inputLines_[convertingFrom_].get<1>()->text().toFloat(),
    inputLines_[convertingFrom_].get<2>()->text().toFloat(),
    inputLines_[convertingFrom_].get<3>()->text().toFloat();

  for(int i = 0; i < 22; ++i){

    if(i == convertingFrom_){
      out[i] = tmp;
      continue;
    }
    input.from_ = tmp;
    input.white_ = r;
    if(convertingFrom_ > CSLCHuv){
      input.sourceM_ = con.GetTable()[convertingFrom_][i].
	get<1>()->GetM_Adapted(r, ada);
      input.sourceGamma_ = con.GetTable()[convertingFrom_][i].
	get<1>()->GetGamma();
    }
    if(i > CSLCHuv){
      input.targetM_1_ = con.GetTable()[convertingFrom_][i].
	get<2>()->GetM_1_Adapted(r, ada);
      input.targetGamma_ = con.GetTable()[convertingFrom_][i].
	get<2>()->GetGamma();
    }
    c = con.GetTable()[convertingFrom_][i].get<0>();
    out[i] = c(input);
  }

  int i = 0;
  BOOST_FOREACH(InputLine& rL, inputLines_){
    rL.get<1>()->setText(QString::number(out[i](0, 0), 'f', 10));
    rL.get<2>()->setText(QString::number(out[i](1, 0), 'f', 10));
    rL.get<3>()->setText(QString::number(out[i](2, 0), 'f', 10));
    i++;
  }
}

void ConversionConsole::ConnectWorkingColorSpaceButton(){

  QObject::connect(pWorkingColorSpaces_,
		   SIGNAL(currentIndexChanged(int)),
		   this, SLOT(SetWorkingColorSpace(int)));
  SetWorkingColorSpace(pWorkingColorSpaces_->currentIndex());
}

void ConversionConsole::ConnectSystemColorSpaceButton(){

  QObject::connect(pSystemColorSpaces_,
		   SIGNAL(currentIndexChanged(int)),
		   this, SLOT(SetSystemColorSpace(int)));
  SetSystemColorSpace(pSystemColorSpaces_->currentIndex());
}

void ConversionConsole::ConnectRefWhiteButton(){

  QObject::connect(pReferenceWhites_, SIGNAL(currentIndexChanged(int)),
		   this, SLOT(SetRefWhite(int)));
  SetRefWhite(pReferenceWhites_->currentIndex());
}

void ConversionConsole::ConnectAdaptationButton(){

  QObject::connect(pChromaticAdaptations_,
		   SIGNAL(currentIndexChanged(int)),
		   this, SLOT(SetAdaptationMethod(int)));
  SetAdaptationMethod(pChromaticAdaptations_->currentIndex());
}

void ConversionConsole::SetDoubleValidator(QLineEdit* const pLineEdit){

  QDoubleValidator *pDoubleValidator = new QDoubleValidator(pLineEdit);
  pDoubleValidator->setNotation(QDoubleValidator::StandardNotation);
  pDoubleValidator->setDecimals(10);
  pLineEdit->setValidator(pDoubleValidator);
}

/* private slots */
void ConversionConsole::ConvertFrom_XYZ_To_all(){
  convertingFrom_ = CSXYZ; ConvertAll();
}
void ConversionConsole::ConvertFrom_xyY_To_all(){
  convertingFrom_ = CSxyY; ConvertAll();
}
void ConversionConsole::ConvertFrom_Lab_To_all(){
  convertingFrom_ = CSLab; ConvertAll();
}
void ConversionConsole::ConvertFrom_LCHab_To_all(){
  convertingFrom_ = CSLCHab; ConvertAll();
}
void ConversionConsole::ConvertFrom_Luv_To_all(){
  convertingFrom_ = CSLuv; ConvertAll();
}
void ConversionConsole::ConvertFrom_LCHuv_To_all(){
  convertingFrom_ = CSLCHuv; ConvertAll();
}
void ConversionConsole::ConvertFrom_Adobe_To_all(){
  convertingFrom_ = CSAdobeRGB; ConvertAll();
}
void ConversionConsole::ConvertFrom_Apple_To_all(){
  convertingFrom_ = CSAppleRGB; ConvertAll();
}
void ConversionConsole::ConvertFrom_Best_To_all(){
  convertingFrom_ = CSBestRGB; ConvertAll();
}
void ConversionConsole::ConvertFrom_Beta_To_all(){
  convertingFrom_ = CSBetaRGB; ConvertAll();
}
void ConversionConsole::ConvertFrom_Bruce_To_all(){
  convertingFrom_ = CSBruceRGB; ConvertAll();
}
void ConversionConsole::ConvertFrom_CIE_To_all(){
  convertingFrom_ = CSCIERGB; ConvertAll();
}
void ConversionConsole::ConvertFrom_ColorMatch_To_all(){
  convertingFrom_ = CSColorMatchRGB; ConvertAll();
}
void ConversionConsole::ConvertFrom_Don4_To_all(){
  convertingFrom_ = CSDonRGB4; ConvertAll();
}
void ConversionConsole::ConvertFrom_ECI_To_all(){
  convertingFrom_ = CSECIRGB; ConvertAll();
}
void ConversionConsole::ConvertFrom_EktaSpacePS5_To_all(){
  convertingFrom_ = CSEktaSpacePS5; ConvertAll();
}
void ConversionConsole::ConvertFrom_NTSC_To_all(){
  convertingFrom_ = CSNTSCRGB; ConvertAll();
}
void ConversionConsole::ConvertFrom_PALSECAM_To_all(){
  convertingFrom_ = CSPALSECAMRGB; ConvertAll();
}
void ConversionConsole::ConvertFrom_ProPhoto_To_all(){
  convertingFrom_ = CSProPhotoRGB; ConvertAll();
}
void ConversionConsole::ConvertFrom_SMPTEC_To_all(){
  convertingFrom_ = 19;
  convertingFrom_ = CSSMPTECRGB; ConvertAll();
}
void ConversionConsole::ConvertFrom_sRGB_To_all(){
  convertingFrom_ = CSsRGB; ConvertAll();
}
void ConversionConsole::ConvertFrom_WideGamut_To_all(){
  convertingFrom_ = CSWideGamutRGB; ConvertAll();
}

void ConversionConsole::SetWorkingColorSpace(int wcs){

  int tmp = workingColorSpace_;
  workingColorSpace_ = wcs;

  if(workingColorSpace_ == systemColorSpace_){
    workingColorSpace_ = tmp;
    pWorkingColorSpaces_->setCurrentIndex(workingColorSpace_);
    return ;
  }

  // Manager::Instance().SetWorkingColorSpace(workingColorSpace_);
  if(pViewer_){
    pViewer_->SetWorkingColorSpace(workingColorSpace_);
  }
}

void ConversionConsole::SetSystemColorSpace(int scs){

  int tmp = systemColorSpace_;
  systemColorSpace_ = scs;

  if(systemColorSpace_ == workingColorSpace_){
    systemColorSpace_ = tmp;
    pSystemColorSpaces_->setCurrentIndex(systemColorSpace_);
    return;
  }

  // Manager::Instance().SetSystemColorSpace(systemColorSpace_);
  if(pViewer_){
    pViewer_->SetSystemColorSpace(systemColorSpace_);
  }
}

void ConversionConsole::SetRefWhite(int r){

  switch(r){
  case 0: refWhite_ = Convert::IlluminantA_;    break;
  case 1: refWhite_ = Convert::IlluminantB_;    break;
  case 2: refWhite_ = Convert::IlluminantC_;    break;
  case 3: refWhite_ = Convert::IlluminantD50_;  break;
  case 4: refWhite_ = Convert::IlluminantD55_;  break;
  case 5: refWhite_ = Convert::IlluminantD65_;  break;
  case 6: refWhite_ = Convert::IlluminantD75_;  break;
  case 7: refWhite_ = Convert::IlluminantE_;    break;
  case 8: refWhite_ = Convert::IlluminantF2_;   break;
  case 9: refWhite_ = Convert::IlluminantF7_;   break;
  case 10: refWhite_ = Convert::IlluminantF11_; break;
  }
  //Manager::Instance().SetReferenceWhite(refWhite_);
  if(pViewer_){
    pViewer_->SetReferenceWhite(refWhite_);
  }
}

void ConversionConsole::SetAdaptationMethod(int a){

  switch(a){
  case 0: adaptationMethod_ = Convert::XYZScaling_; break;
  case 1: adaptationMethod_ = Convert::VonKries_;   break;
  case 2: adaptationMethod_ = Convert::Bradford_;   break;
  }
  //Manager::Instance().SetAdaptationMethod(adaptationMethod_);
  if(pViewer_){
    pViewer_->SetAdaptationMethod(adaptationMethod_);
  }
}
