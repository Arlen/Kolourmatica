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
#include "../core/Specification.hpp"
#include "../core/Convert.hpp"

#include <QtGui/QDoubleValidator>
#include <QtGui/QGridLayout>
#include <QtGui/QPushButton>
#include <QtGui/QLineEdit>
#include <QtGui/QComboBox>
#include <QtGui/QLabel>

#include <iostream>
using namespace std;

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
  InitWidgets();
  ConnectConversionButtons();
  ConnectRefWhiteButton();
  ConnectAdaptationButton();
}

void ConversionConsole::InitWidgets(){

  pMainLayout_ = new QGridLayout(this);
  pMainLayout_->setContentsMargins(0, 2, 0, 2);
  pMainLayout_->setSpacing(1);

  pColorSpace_ = new QLabel("Color Space");
  pSystemColorSpace_ = new QLabel("System Color Space");
  pReferenceWhite_ = new QLabel("Reference White");
  pChromaticAdaptation_ = new QLabel("Chromatic Adaptation");
  pColorSpaces_ = new QComboBox;
  pColorSpaces_->setMaximumWidth(128);
  pSystemColorSpaces_ = new QComboBox;
  pSystemColorSpaces_->setMaximumWidth(128);
  pReferenceWhites_ = new QComboBox;
  pReferenceWhites_->setMaximumWidth(128);
  pChromaticAdaptations_ = new QComboBox;
  pChromaticAdaptations_->setMaximumWidth(128);

  BOOST_FOREACH(QString& rS, colorSpaces_){
    pColorSpaces_->addItem(rS);
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
  pMainLayout_->addWidget(pColorSpace_, 0, 1);
  pMainLayout_->addWidget(pColorSpaces_, 0, 2);
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
  pColorSpaces_->setCurrentIndex(1);
  pSystemColorSpaces_->setCurrentIndex(20);
  pReferenceWhites_->setCurrentIndex(5);
  pChromaticAdaptations_->setCurrentIndex(2);
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

void ConversionConsole::ConvertFrom_XYZ_To_all(){
  convertingFrom_ = Manager::CSXYZ; ConvertAll();
}
void ConversionConsole::ConvertFrom_xyY_To_all(){
  convertingFrom_ = Manager::CSxyY; ConvertAll();
}
void ConversionConsole::ConvertFrom_Lab_To_all(){
  convertingFrom_ = Manager::CSLab; ConvertAll();
}
void ConversionConsole::ConvertFrom_LCHab_To_all(){
  convertingFrom_ = Manager::CSLCHab; ConvertAll();
}
void ConversionConsole::ConvertFrom_Luv_To_all(){
  convertingFrom_ = Manager::CSLuv; ConvertAll();
}
void ConversionConsole::ConvertFrom_LCHuv_To_all(){
  convertingFrom_ = Manager::CSLCHuv; ConvertAll();
}
void ConversionConsole::ConvertFrom_Adobe_To_all(){
  convertingFrom_ = Manager::CSAdobeRGB; ConvertAll();
}
void ConversionConsole::ConvertFrom_Apple_To_all(){
  convertingFrom_ = Manager::CSAppleRGB; ConvertAll();
}
void ConversionConsole::ConvertFrom_Best_To_all(){
  convertingFrom_ = Manager::CSBestRGB; ConvertAll();
}
void ConversionConsole::ConvertFrom_Beta_To_all(){
  convertingFrom_ = Manager::CSBetaRGB; ConvertAll();
}
void ConversionConsole::ConvertFrom_Bruce_To_all(){
  convertingFrom_ = Manager::CSBruceRGB; ConvertAll();
}
void ConversionConsole::ConvertFrom_CIE_To_all(){
  convertingFrom_ = Manager::CSCIERGB; ConvertAll();
}
void ConversionConsole::ConvertFrom_ColorMatch_To_all(){
  convertingFrom_ = Manager::CSColorMatchRGB; ConvertAll();
}
void ConversionConsole::ConvertFrom_Don4_To_all(){
  convertingFrom_ = Manager::CSDonRGB4; ConvertAll();
}
void ConversionConsole::ConvertFrom_ECI_To_all(){
  convertingFrom_ = Manager::CSECIRGB; ConvertAll();
}
void ConversionConsole::ConvertFrom_EktaSpacePS5_To_all(){
  convertingFrom_ = Manager::CSEktaSpacePS5; ConvertAll();
}
void ConversionConsole::ConvertFrom_NTSC_To_all(){
  convertingFrom_ = Manager::CSNTSCRGB; ConvertAll();
}
void ConversionConsole::ConvertFrom_PALSECAM_To_all(){
  convertingFrom_ = Manager::CSPALSECAMRGB; ConvertAll();
}
void ConversionConsole::ConvertFrom_ProPhoto_To_all(){
  convertingFrom_ = Manager::CSProPhotoRGB; ConvertAll();
}
void ConversionConsole::ConvertFrom_SMPTEC_To_all(){
  convertingFrom_ = Manager::CSSMPTECRGB; ConvertAll();
}
void ConversionConsole::ConvertFrom_sRGB_To_all(){
  convertingFrom_ = Manager::CSsRGB; ConvertAll();
}
void ConversionConsole::ConvertFrom_WideGamut_To_all(){
  convertingFrom_ = Manager::CSWideGamutRGB; ConvertAll();
}

void ConversionConsole::SetRefWhite(int r){

  switch(r){
  case 0:
    refWhite_ = Convert::IlluminantA_;
    break;
  case 1:
    refWhite_ = Convert::IlluminantB_;
    break;
  case 2:
    refWhite_ = Convert::IlluminantC_;
    break;
  case 3:
    refWhite_ = Convert::IlluminantD50_;
    break;
  case 4:
    refWhite_ = Convert::IlluminantD55_;
    break;
  case 5:
    refWhite_ = Convert::IlluminantD65_;
    break;
  case 6:
    refWhite_ = Convert::IlluminantD75_;
    break;
  case 7:
    refWhite_ = Convert::IlluminantE_;
    break;
  case 8:
    refWhite_ = Convert::IlluminantF2_;
    break;
  case 9:
    refWhite_ = Convert::IlluminantF7_;
    break;
  case 10:
    refWhite_ = Convert::IlluminantF11_;
    break;
  }
}

void ConversionConsole::SetAdaptationMethod(int a){

  switch(a){
  case 0:
    adaptationMethod_ = Convert::XYZScaling_;
    break;
  case 1:
    adaptationMethod_ = Convert::VonKries_;
    break;
  case 2:
    adaptationMethod_ = Convert::Bradford_;
    break;
  }
}

void ConversionConsole::ConvertAll(){

  Eigen::Vector3f tmp;
  Eigen::Matrix3f ada = adaptationMethod_;
  Eigen::Vector3f r = refWhite_;
  boost::array<Vector3f, 22> out;

  Adobe adobe;
  Apple apple;
  Best best;
  Beta beta;
  Bruce bruce;
  CIE cie;
  ColorMatch colormatch;
  Don4 don4;
  ECI eci;
  EktaSpacePS5 ektaspaceps5;
  NTSC ntsc;
  PAL_SECAM pal_secam;
  ProPhoto prophoto;
  SMPTE_C smpte_c;
  SRGB srgb;
  WideGamut widegamut;

  Input input;

  switch(convertingFrom_){

  case Manager::CSXYZ:
    tmp <<
      inputLines_[0].get<1>()->text().toFloat(),
      inputLines_[0].get<2>()->text().toFloat(),
      inputLines_[0].get<3>()->text().toFloat();

    input.from_ = tmp;
    input.white_ = r;

    out[0] = tmp;
    out[1] = Convert::From_XYZ_To_xyY(input);
    out[2] = Convert::From_XYZ_To_Lab(input);
    out[3] = Convert::From_XYZ_To_LCHab(input);
    out[4] = Convert::From_XYZ_To_Luv(input);
    out[5] = Convert::From_XYZ_To_LCHuv(input);

    input.targetM_1_ = adobe.GetM_1_Adapted(r, ada);
    input.targetGamma_ = adobe.GetGamma();
    out[6] = Convert::From_XYZ_To_RGB(input);

    input.targetM_1_ = apple.GetM_1_Adapted(r, ada);
    input.targetGamma_ = apple.GetGamma();
    out[7] = Convert::From_XYZ_To_RGB(input);

    input.targetM_1_ = best.GetM_1_Adapted(r, ada);
    input.targetGamma_ = best.GetGamma();
    out[8] = Convert::From_XYZ_To_RGB(input);

    input.targetM_1_ = beta.GetM_1_Adapted(r, ada);
    input.targetGamma_ = beta.GetGamma();
    out[9] = Convert::From_XYZ_To_RGB(input);

    input.targetM_1_ = bruce.GetM_1_Adapted(r, ada);
    input.targetGamma_ = bruce.GetGamma();
    out[10] = Convert::From_XYZ_To_RGB(input);

    input.targetM_1_ = cie.GetM_1_Adapted(r, ada);
    input.targetGamma_ = cie.GetGamma();
    out[11] = Convert::From_XYZ_To_RGB(input);

    input.targetM_1_ = colormatch.GetM_1_Adapted(r, ada);
    input.targetGamma_ = colormatch.GetGamma();
    out[12] = Convert::From_XYZ_To_RGB(input);

    input.targetM_1_ = don4.GetM_1_Adapted(r, ada);
    input.targetGamma_ = don4.GetGamma();
    out[13] = Convert::From_XYZ_To_RGB(input);

    input.targetM_1_ = eci.GetM_1_Adapted(r, ada);
    input.targetGamma_ = eci.GetGamma();
    out[14] = Convert::From_XYZ_To_RGB(input);

    input.targetM_1_ = ektaspaceps5.GetM_1_Adapted(r, ada);
    input.targetGamma_ = ektaspaceps5.GetGamma();
    out[15] = Convert::From_XYZ_To_RGB(input);

    input.targetM_1_ = ntsc.GetM_1_Adapted(r, ada);
    input.targetGamma_ = ntsc.GetGamma();
    out[16] = Convert::From_XYZ_To_RGB(input);

    input.targetM_1_ = pal_secam.GetM_1_Adapted(r, ada);
    input.targetGamma_ = pal_secam.GetGamma();
    out[17] = Convert::From_XYZ_To_RGB(input);

    input.targetM_1_ = prophoto.GetM_1_Adapted(r, ada);
    input.targetGamma_ = prophoto.GetGamma();
    out[18] = Convert::From_XYZ_To_RGB(input);

    input.targetM_1_ = smpte_c.GetM_1_Adapted(r, ada);
    input.targetGamma_ = smpte_c.GetGamma();
    out[19] = Convert::From_XYZ_To_RGB(input);

    input.targetM_1_ = srgb.GetM_1_Adapted(r, ada);
    input.targetGamma_ = srgb.GetGamma();
    out[20] = Convert::From_XYZ_To_sRGB(input);

    input.targetM_1_ = widegamut.GetM_1_Adapted(r, ada);
    input.targetGamma_ = widegamut.GetGamma();
    out[21] = Convert::From_XYZ_To_RGB(input);
    break;
 
  case Manager::CSxyY:
    tmp <<
      inputLines_[1].get<1>()->text().toFloat(),
      inputLines_[1].get<2>()->text().toFloat(),
      inputLines_[1].get<3>()->text().toFloat();

    input.from_ = tmp;
    input.white_ = r;

    out[0] = Convert::From_xyY_To_XYZ(input);
    out[1] = tmp;
    out[2] = Convert::From_xyY_To_Lab(input);
    out[3] = Convert::From_xyY_To_LCHab(input);
    out[4] = Convert::From_xyY_To_Luv(input);
    out[5] = Convert::From_xyY_To_LCHuv(input);

    input.targetM_1_ = adobe.GetM_1_Adapted(r, ada);
    input.targetGamma_ = adobe.GetGamma();
    out[6] = Convert::From_xyY_To_RGB(input);

    input.targetM_1_ = apple.GetM_1_Adapted(r, ada);
    input.targetGamma_ = apple.GetGamma();
    out[7] = Convert::From_xyY_To_RGB(input);

    input.targetM_1_ = best.GetM_1_Adapted(r, ada);
    input.targetGamma_ = best.GetGamma();
    out[8] = Convert::From_xyY_To_RGB(input);

    input.targetM_1_ = beta.GetM_1_Adapted(r, ada);
    input.targetGamma_ = beta.GetGamma();
    out[9] = Convert::From_xyY_To_RGB(input);

    input.targetM_1_ = bruce.GetM_1_Adapted(r, ada);
    input.targetGamma_ = bruce.GetGamma();
    out[10] = Convert::From_xyY_To_RGB(input);

    input.targetM_1_ = cie.GetM_1_Adapted(r, ada);
    input.targetGamma_ = cie.GetGamma();
    out[11] = Convert::From_xyY_To_RGB(input);

    input.targetM_1_ = colormatch.GetM_1_Adapted(r, ada);
    input.targetGamma_ = colormatch.GetGamma();
    out[12] = Convert::From_xyY_To_RGB(input);

    input.targetM_1_ = don4.GetM_1_Adapted(r, ada);
    input.targetGamma_ = don4.GetGamma();
    out[13] = Convert::From_xyY_To_RGB(input);

    input.targetM_1_ = eci.GetM_1_Adapted(r, ada);
    input.targetGamma_ = eci.GetGamma();
    out[14] = Convert::From_xyY_To_RGB(input);

    input.targetM_1_ = ektaspaceps5.GetM_1_Adapted(r, ada);
    input.targetGamma_ = ektaspaceps5.GetGamma();
    out[15] = Convert::From_xyY_To_RGB(input);

    input.targetM_1_ = ntsc.GetM_1_Adapted(r, ada);
    input.targetGamma_ = ntsc.GetGamma();
    out[16] = Convert::From_xyY_To_RGB(input);

    input.targetM_1_ = pal_secam.GetM_1_Adapted(r, ada);
    input.targetGamma_ = pal_secam.GetGamma();
    out[17] = Convert::From_xyY_To_RGB(input);

    input.targetM_1_ = prophoto.GetM_1_Adapted(r, ada);
    input.targetGamma_ = prophoto.GetGamma();
    out[18] = Convert::From_xyY_To_RGB(input);

    input.targetM_1_ = smpte_c.GetM_1_Adapted(r, ada);
    input.targetGamma_ = smpte_c.GetGamma();
    out[19] = Convert::From_xyY_To_RGB(input);

    input.targetM_1_ = srgb.GetM_1_Adapted(r, ada);
    input.targetGamma_ = srgb.GetGamma();
    out[20] = Convert::From_xyY_To_sRGB(input);

    input.targetM_1_ = widegamut.GetM_1_Adapted(r, ada);
    input.targetGamma_ = widegamut.GetGamma();
    out[21] = Convert::From_xyY_To_RGB(input);
    break;

  case Manager::CSLab:
    tmp <<
      inputLines_[2].get<1>()->text().toFloat(),
      inputLines_[2].get<2>()->text().toFloat(),
      inputLines_[2].get<3>()->text().toFloat();

    input.from_ = tmp;
    input.white_ = r;

    out[0] = Convert::From_Lab_To_XYZ(input);
    out[1] = Convert::From_Lab_To_xyY(input);
    out[2] = tmp;
    out[3] = Convert::From_Lab_To_LCHab(input);
    out[4] = Convert::From_Lab_To_Luv(input);
    out[5] = Convert::From_Lab_To_LCHuv(input);

    input.targetM_1_ = adobe.GetM_1_Adapted(r, ada);
    input.targetGamma_ = adobe.GetGamma();
    out[6] = Convert::From_Lab_To_RGB(input);

    input.targetM_1_ = apple.GetM_1_Adapted(r, ada);
    input.targetGamma_ = apple.GetGamma();
    out[7] = Convert::From_Lab_To_RGB(input);

    input.targetM_1_ = best.GetM_1_Adapted(r, ada);
    input.targetGamma_ = best.GetGamma();
    out[8] = Convert::From_Lab_To_RGB(input);

    input.targetM_1_ = beta.GetM_1_Adapted(r, ada);
    input.targetGamma_ = beta.GetGamma();
    out[9] = Convert::From_Lab_To_RGB(input);

    input.targetM_1_ = bruce.GetM_1_Adapted(r, ada);
    input.targetGamma_ = bruce.GetGamma();
    out[10] = Convert::From_Lab_To_RGB(input);

    input.targetM_1_ = cie.GetM_1_Adapted(r, ada);
    input.targetGamma_ = cie.GetGamma();
    out[11] = Convert::From_Lab_To_RGB(input);

    input.targetM_1_ = colormatch.GetM_1_Adapted(r, ada);
    input.targetGamma_ = colormatch.GetGamma();
    out[12] = Convert::From_Lab_To_RGB(input);

    input.targetM_1_ = don4.GetM_1_Adapted(r, ada);
    input.targetGamma_ = don4.GetGamma();
    out[13] = Convert::From_Lab_To_RGB(input);

    input.targetM_1_ = eci.GetM_1_Adapted(r, ada);
    input.targetGamma_ = eci.GetGamma();
    out[14] = Convert::From_Lab_To_RGB(input);

    input.targetM_1_ = ektaspaceps5.GetM_1_Adapted(r, ada);
    input.targetGamma_ = ektaspaceps5.GetGamma();
    out[15] = Convert::From_Lab_To_RGB(input);

    input.targetM_1_ = ntsc.GetM_1_Adapted(r, ada);
    input.targetGamma_ = ntsc.GetGamma();
    out[16] = Convert::From_Lab_To_RGB(input);

    input.targetM_1_ = pal_secam.GetM_1_Adapted(r, ada);
    input.targetGamma_ = pal_secam.GetGamma();
    out[17] = Convert::From_Lab_To_RGB(input);

    input.targetM_1_ = prophoto.GetM_1_Adapted(r, ada);
    input.targetGamma_ = prophoto.GetGamma();
    out[18] = Convert::From_Lab_To_RGB(input);

    input.targetM_1_ = smpte_c.GetM_1_Adapted(r, ada);
    input.targetGamma_ = smpte_c.GetGamma();
    out[19] = Convert::From_Lab_To_RGB(input);

    input.targetM_1_ = srgb.GetM_1_Adapted(r, ada);
    input.targetGamma_ = srgb.GetGamma();
    out[20] = Convert::From_Lab_To_sRGB(input);

    input.targetM_1_ = widegamut.GetM_1_Adapted(r, ada);
    input.targetGamma_ = widegamut.GetGamma();
    out[21] = Convert::From_Lab_To_RGB(input);
    break;

  case Manager::CSLCHab:
    tmp <<
      inputLines_[3].get<1>()->text().toFloat(),
      inputLines_[3].get<2>()->text().toFloat(),
      inputLines_[3].get<3>()->text().toFloat();

    input.from_ = tmp;
    input.white_ = r;

    out[0] = Convert::From_LCHab_To_XYZ(input);
    out[1] = Convert::From_LCHab_To_xyY(input);
    out[2] = Convert::From_LCHab_To_Lab(input);
    out[3] = tmp;
    out[4] = Convert::From_LCHab_To_Luv(input);
    out[5] = Convert::From_LCHab_To_LCHuv(input);

    input.targetM_1_ = adobe.GetM_1_Adapted(r, ada);
    input.targetGamma_ = adobe.GetGamma();
    out[6] = Convert::From_LCHab_To_RGB(input);

    input.targetM_1_ = apple.GetM_1_Adapted(r, ada);
    input.targetGamma_ = apple.GetGamma();
    out[7] = Convert::From_LCHab_To_RGB(input);

    input.targetM_1_ = best.GetM_1_Adapted(r, ada);
    input.targetGamma_ = best.GetGamma();
    out[8] = Convert::From_LCHab_To_RGB(input);

    input.targetM_1_ = beta.GetM_1_Adapted(r, ada);
    input.targetGamma_ = beta.GetGamma();
    out[9] = Convert::From_LCHab_To_RGB(input);

    input.targetM_1_ = bruce.GetM_1_Adapted(r, ada);
    input.targetGamma_ = bruce.GetGamma();
    out[10] = Convert::From_LCHab_To_RGB(input);

    input.targetM_1_ = cie.GetM_1_Adapted(r, ada);
    input.targetGamma_ = cie.GetGamma();
    out[11] = Convert::From_LCHab_To_RGB(input);

    input.targetM_1_ = colormatch.GetM_1_Adapted(r, ada);
    input.targetGamma_ = colormatch.GetGamma();
    out[12] = Convert::From_LCHab_To_RGB(input);

    input.targetM_1_ = don4.GetM_1_Adapted(r, ada);
    input.targetGamma_ = don4.GetGamma();
    out[13] = Convert::From_LCHab_To_RGB(input);

    input.targetM_1_ = eci.GetM_1_Adapted(r, ada);
    input.targetGamma_ = eci.GetGamma();
    out[14] = Convert::From_LCHab_To_RGB(input);

    input.targetM_1_ = ektaspaceps5.GetM_1_Adapted(r, ada);
    input.targetGamma_ = ektaspaceps5.GetGamma();
    out[15] = Convert::From_LCHab_To_RGB(input);

    input.targetM_1_ = ntsc.GetM_1_Adapted(r, ada);
    input.targetGamma_ = ntsc.GetGamma();
    out[16] = Convert::From_LCHab_To_RGB(input);

    input.targetM_1_ = pal_secam.GetM_1_Adapted(r, ada);
    input.targetGamma_ = pal_secam.GetGamma();
    out[17] = Convert::From_LCHab_To_RGB(input);

    input.targetM_1_ = prophoto.GetM_1_Adapted(r, ada);
    input.targetGamma_ = prophoto.GetGamma();
    out[18] = Convert::From_LCHab_To_RGB(input);

    input.targetM_1_ = smpte_c.GetM_1_Adapted(r, ada);
    input.targetGamma_ = smpte_c.GetGamma();
    out[19] = Convert::From_LCHab_To_RGB(input);

    input.targetM_1_ = srgb.GetM_1_Adapted(r, ada);
    input.targetGamma_ = srgb.GetGamma();
    out[20] = Convert::From_LCHab_To_sRGB(input);

    input.targetM_1_ = widegamut.GetM_1_Adapted(r, ada);
    input.targetGamma_ = widegamut.GetGamma();
    out[21] = Convert::From_LCHab_To_RGB(input);
    break;

  case Manager::CSLuv:
    tmp <<
      inputLines_[4].get<1>()->text().toFloat(),
      inputLines_[4].get<2>()->text().toFloat(),
      inputLines_[4].get<3>()->text().toFloat();

    input.from_ = tmp;
    input.white_ = r;

    out[0] = Convert::From_Luv_To_XYZ(input);
    out[1] = Convert::From_Luv_To_xyY(input);
    out[2] = Convert::From_Luv_To_Lab(input);
    out[3] = Convert::From_Luv_To_LCHab(input);
    out[4] = tmp;
    out[5] = Convert::From_Luv_To_LCHuv(input);

    input.targetM_1_ = adobe.GetM_1_Adapted(r, ada);
    input.targetGamma_ = adobe.GetGamma();
    out[6] = Convert::From_Luv_To_RGB(input);

    input.targetM_1_ = apple.GetM_1_Adapted(r, ada);
    input.targetGamma_ = apple.GetGamma();
    out[7] = Convert::From_Luv_To_RGB(input);

    input.targetM_1_ = best.GetM_1_Adapted(r, ada);
    input.targetGamma_ = best.GetGamma();
    out[8] = Convert::From_Luv_To_RGB(input);

    input.targetM_1_ = beta.GetM_1_Adapted(r, ada);
    input.targetGamma_ = beta.GetGamma();
    out[9] = Convert::From_Luv_To_RGB(input);

    input.targetM_1_ = bruce.GetM_1_Adapted(r, ada);
    input.targetGamma_ = bruce.GetGamma();
    out[10] = Convert::From_Luv_To_RGB(input);

    input.targetM_1_ = cie.GetM_1_Adapted(r, ada);
    input.targetGamma_ = cie.GetGamma();
    out[11] = Convert::From_Luv_To_RGB(input);

    input.targetM_1_ = colormatch.GetM_1_Adapted(r, ada);
    input.targetGamma_ = colormatch.GetGamma();
    out[12] = Convert::From_Luv_To_RGB(input);

    input.targetM_1_ = don4.GetM_1_Adapted(r, ada);
    input.targetGamma_ = don4.GetGamma();
    out[13] = Convert::From_Luv_To_RGB(input);

    input.targetM_1_ = eci.GetM_1_Adapted(r, ada);
    input.targetGamma_ = eci.GetGamma();
    out[14] = Convert::From_Luv_To_RGB(input);

    input.targetM_1_ = ektaspaceps5.GetM_1_Adapted(r, ada);
    input.targetGamma_ = ektaspaceps5.GetGamma();
    out[15] = Convert::From_Luv_To_RGB(input);

    input.targetM_1_ = ntsc.GetM_1_Adapted(r, ada);
    input.targetGamma_ = ntsc.GetGamma();
    out[16] = Convert::From_Luv_To_RGB(input);

    input.targetM_1_ = pal_secam.GetM_1_Adapted(r, ada);
    input.targetGamma_ = pal_secam.GetGamma();
    out[17] = Convert::From_Luv_To_RGB(input);

    input.targetM_1_ = prophoto.GetM_1_Adapted(r, ada);
    input.targetGamma_ = prophoto.GetGamma();
    out[18] = Convert::From_Luv_To_RGB(input);

    input.targetM_1_ = smpte_c.GetM_1_Adapted(r, ada);
    input.targetGamma_ = smpte_c.GetGamma();
    out[19] = Convert::From_Luv_To_RGB(input);

    input.targetM_1_ = srgb.GetM_1_Adapted(r, ada);
    input.targetGamma_ = srgb.GetGamma();
    out[20] = Convert::From_Luv_To_sRGB(input);

    input.targetM_1_ = widegamut.GetM_1_Adapted(r, ada);
    input.targetGamma_ = widegamut.GetGamma();
    out[21] = Convert::From_Luv_To_RGB(input);
    break;

  case Manager::CSLCHuv:
    tmp <<
      inputLines_[5].get<1>()->text().toFloat(),
      inputLines_[5].get<2>()->text().toFloat(),
      inputLines_[5].get<3>()->text().toFloat();

    input.from_ = tmp;
    input.white_ = r;

    out[0] = Convert::From_LCHuv_To_XYZ(input);
    out[1] = Convert::From_LCHuv_To_xyY(input);
    out[2] = Convert::From_LCHuv_To_Lab(input);
    out[3] = Convert::From_LCHuv_To_LCHab(input);
    out[4] = Convert::From_LCHuv_To_Luv(input);;
    out[5] = tmp;

    input.targetM_1_ = adobe.GetM_1_Adapted(r, ada);
    input.targetGamma_ = adobe.GetGamma();
    out[6] = Convert::From_LCHuv_To_RGB(input);

    input.targetM_1_ = apple.GetM_1_Adapted(r, ada);
    input.targetGamma_ = apple.GetGamma();
    out[7] = Convert::From_LCHuv_To_RGB(input);

    input.targetM_1_ = best.GetM_1_Adapted(r, ada);
    input.targetGamma_ = best.GetGamma();
    out[8] = Convert::From_LCHuv_To_RGB(input);

    input.targetM_1_ = beta.GetM_1_Adapted(r, ada);
    input.targetGamma_ = beta.GetGamma();
    out[9] = Convert::From_LCHuv_To_RGB(input);

    input.targetM_1_ = bruce.GetM_1_Adapted(r, ada);
    input.targetGamma_ = bruce.GetGamma();
    out[10] = Convert::From_LCHuv_To_RGB(input);

    input.targetM_1_ = cie.GetM_1_Adapted(r, ada);
    input.targetGamma_ = cie.GetGamma();
    out[11] = Convert::From_LCHuv_To_RGB(input);

    input.targetM_1_ = colormatch.GetM_1_Adapted(r, ada);
    input.targetGamma_ = colormatch.GetGamma();
    out[12] = Convert::From_LCHuv_To_RGB(input);

    input.targetM_1_ = don4.GetM_1_Adapted(r, ada);
    input.targetGamma_ = don4.GetGamma();
    out[13] = Convert::From_LCHuv_To_RGB(input);

    input.targetM_1_ = eci.GetM_1_Adapted(r, ada);
    input.targetGamma_ = eci.GetGamma();
    out[14] = Convert::From_LCHuv_To_RGB(input);

    input.targetM_1_ = ektaspaceps5.GetM_1_Adapted(r, ada);
    input.targetGamma_ = ektaspaceps5.GetGamma();
    out[15] = Convert::From_LCHuv_To_RGB(input);

    input.targetM_1_ = ntsc.GetM_1_Adapted(r, ada);
    input.targetGamma_ = ntsc.GetGamma();
    out[16] = Convert::From_LCHuv_To_RGB(input);

    input.targetM_1_ = pal_secam.GetM_1_Adapted(r, ada);
    input.targetGamma_ = pal_secam.GetGamma();
    out[17] = Convert::From_LCHuv_To_RGB(input);

    input.targetM_1_ = prophoto.GetM_1_Adapted(r, ada);
    input.targetGamma_ = prophoto.GetGamma();
    out[18] = Convert::From_LCHuv_To_RGB(input);

    input.targetM_1_ = smpte_c.GetM_1_Adapted(r, ada);
    input.targetGamma_ = smpte_c.GetGamma();
    out[19] = Convert::From_LCHuv_To_RGB(input);

    input.targetM_1_ = srgb.GetM_1_Adapted(r, ada);
    input.targetGamma_ = srgb.GetGamma();
    out[20] = Convert::From_LCHuv_To_sRGB(input);

    input.targetM_1_ = widegamut.GetM_1_Adapted(r, ada);
    input.targetGamma_ = widegamut.GetGamma();
    out[21] = Convert::From_LCHuv_To_RGB(input);
    break;

  case Manager::CSAdobeRGB:
    tmp <<
      inputLines_[6].get<1>()->text().toFloat(),
      inputLines_[6].get<2>()->text().toFloat(),
      inputLines_[6].get<3>()->text().toFloat();

    input.from_ = tmp;
    input.white_ = r;
    input.sourceM_ = adobe.GetM_Adapted(r, ada);
    input.sourceGamma_ = adobe.GetGamma();

    out[0] = Convert::From_RGB_To_XYZ(input);
    out[1] = Convert::From_RGB_To_xyY(input);
    out[2] = Convert::From_RGB_To_Lab(input);
    out[3] = Convert::From_RGB_To_LCHab(input);
    out[4] = Convert::From_RGB_To_Luv(input);
    out[5] = Convert::From_RGB_To_LCHuv(input);
    out[6] = tmp;

    input.targetM_1_ = apple.GetM_1_Adapted(r, ada);
    input.targetGamma_ = apple.GetGamma();
    out[7] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = best.GetM_1_Adapted(r, ada);
    input.targetGamma_ = best.GetGamma();
    out[8] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = beta.GetM_1_Adapted(r, ada);
    input.targetGamma_ = beta.GetGamma();
    out[9] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = bruce.GetM_1_Adapted(r, ada);
    input.targetGamma_ = bruce.GetGamma();
    out[10] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = cie.GetM_1_Adapted(r, ada);
    input.targetGamma_ = cie.GetGamma();
    out[11] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = colormatch.GetM_1_Adapted(r, ada);
    input.targetGamma_ = colormatch.GetGamma();
    out[12] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = don4.GetM_1_Adapted(r, ada);
    input.targetGamma_ = don4.GetGamma();
    out[13] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = eci.GetM_1_Adapted(r, ada);
    input.targetGamma_ = eci.GetGamma();
    out[14] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = ektaspaceps5.GetM_1_Adapted(r, ada);
    input.targetGamma_ = ektaspaceps5.GetGamma();
    out[15] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = ntsc.GetM_1_Adapted(r, ada);
    input.targetGamma_ = ntsc.GetGamma();
    out[16] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = pal_secam.GetM_1_Adapted(r, ada);
    input.targetGamma_ = pal_secam.GetGamma();
    out[17] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = prophoto.GetM_1_Adapted(r, ada);
    input.targetGamma_ = prophoto.GetGamma();
    out[18] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = smpte_c.GetM_1_Adapted(r, ada);
    input.targetGamma_ = smpte_c.GetGamma();
    out[19] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = srgb.GetM_1_Adapted(r, ada);
    input.targetGamma_ = srgb.GetGamma();
    out[20] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = widegamut.GetM_1_Adapted(r, ada);
    input.targetGamma_ = widegamut.GetGamma();
    out[21] = Convert::From_RGB_To_RGB(input);
    break;
  
  case Manager::CSAppleRGB:
    tmp <<
      inputLines_[7].get<1>()->text().toFloat(),
      inputLines_[7].get<2>()->text().toFloat(),
      inputLines_[7].get<3>()->text().toFloat();

    input.from_ = tmp;
    input.white_ = r;
    input.sourceM_ = apple.GetM_Adapted(r, ada);
    input.sourceGamma_ = apple.GetGamma();

    out[0] = Convert::From_RGB_To_XYZ(input);
    out[1] = Convert::From_RGB_To_xyY(input);
    out[2] = Convert::From_RGB_To_Lab(input);
    out[3] = Convert::From_RGB_To_LCHab(input);
    out[4] = Convert::From_RGB_To_Luv(input);
    out[5] = Convert::From_RGB_To_LCHuv(input);

    input.targetM_1_ = adobe.GetM_1_Adapted(r, ada);
    input.targetGamma_ = adobe.GetGamma();
    out[6] = Convert::From_RGB_To_RGB(input);
    out[7] = tmp;

    input.targetM_1_ = best.GetM_1_Adapted(r, ada);
    input.targetGamma_ = best.GetGamma();
    out[8] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = beta.GetM_1_Adapted(r, ada);
    input.targetGamma_ = beta.GetGamma();
    out[9] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = bruce.GetM_1_Adapted(r, ada);
    input.targetGamma_ = bruce.GetGamma();
    out[10] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = cie.GetM_1_Adapted(r, ada);
    input.targetGamma_ = cie.GetGamma();
    out[11] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = colormatch.GetM_1_Adapted(r, ada);
    input.targetGamma_ = colormatch.GetGamma();
    out[12] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = don4.GetM_1_Adapted(r, ada);
    input.targetGamma_ = don4.GetGamma();
    out[13] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = eci.GetM_1_Adapted(r, ada);
    input.targetGamma_ = eci.GetGamma();
    out[14] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = ektaspaceps5.GetM_1_Adapted(r, ada);
    input.targetGamma_ = ektaspaceps5.GetGamma();
    out[15] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = ntsc.GetM_1_Adapted(r, ada);
    input.targetGamma_ = ntsc.GetGamma();
    out[16] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = pal_secam.GetM_1_Adapted(r, ada);
    input.targetGamma_ = pal_secam.GetGamma();
    out[17] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = prophoto.GetM_1_Adapted(r, ada);
    input.targetGamma_ = prophoto.GetGamma();
    out[18] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = smpte_c.GetM_1_Adapted(r, ada);
    input.targetGamma_ = smpte_c.GetGamma();
    out[19] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = srgb.GetM_1_Adapted(r, ada);
    input.targetGamma_ = srgb.GetGamma();
    out[20] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = widegamut.GetM_1_Adapted(r, ada);
    input.targetGamma_ = widegamut.GetGamma();
    out[21] = Convert::From_RGB_To_RGB(input);
    break;

  case Manager::CSBestRGB:
    tmp <<
      inputLines_[8].get<1>()->text().toFloat(),
      inputLines_[8].get<2>()->text().toFloat(),
      inputLines_[8].get<3>()->text().toFloat();

    input.from_ = tmp;
    input.white_ = r;
    input.sourceM_ = best.GetM_Adapted(r, ada);
    input.sourceGamma_ = best.GetGamma();

    out[0] = Convert::From_RGB_To_XYZ(input);
    out[1] = Convert::From_RGB_To_xyY(input);
    out[2] = Convert::From_RGB_To_Lab(input);
    out[3] = Convert::From_RGB_To_LCHab(input);
    out[4] = Convert::From_RGB_To_Luv(input);
    out[5] = Convert::From_RGB_To_LCHuv(input);

    input.targetM_1_ = adobe.GetM_1_Adapted(r, ada);
    input.targetGamma_ = adobe.GetGamma();
    out[6] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = apple.GetM_1_Adapted(r, ada);
    input.targetGamma_ = apple.GetGamma();
    out[7] = Convert::From_RGB_To_RGB(input);
    out[8] = tmp;

    input.targetM_1_ = beta.GetM_1_Adapted(r, ada);
    input.targetGamma_ = beta.GetGamma();
    out[9] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = bruce.GetM_1_Adapted(r, ada);
    input.targetGamma_ = bruce.GetGamma();
    out[10] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = cie.GetM_1_Adapted(r, ada);
    input.targetGamma_ = cie.GetGamma();
    out[11] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = colormatch.GetM_1_Adapted(r, ada);
    input.targetGamma_ = colormatch.GetGamma();
    out[12] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = don4.GetM_1_Adapted(r, ada);
    input.targetGamma_ = don4.GetGamma();
    out[13] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = eci.GetM_1_Adapted(r, ada);
    input.targetGamma_ = eci.GetGamma();
    out[14] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = ektaspaceps5.GetM_1_Adapted(r, ada);
    input.targetGamma_ = ektaspaceps5.GetGamma();
    out[15] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = ntsc.GetM_1_Adapted(r, ada);
    input.targetGamma_ = ntsc.GetGamma();
    out[16] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = pal_secam.GetM_1_Adapted(r, ada);
    input.targetGamma_ = pal_secam.GetGamma();
    out[17] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = prophoto.GetM_1_Adapted(r, ada);
    input.targetGamma_ = prophoto.GetGamma();
    out[18] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = smpte_c.GetM_1_Adapted(r, ada);
    input.targetGamma_ = smpte_c.GetGamma();
    out[19] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = srgb.GetM_1_Adapted(r, ada);
    input.targetGamma_ = srgb.GetGamma();
    out[20] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = widegamut.GetM_1_Adapted(r, ada);
    input.targetGamma_ = widegamut.GetGamma();
    out[21] = Convert::From_RGB_To_RGB(input);
    break;

  case Manager::CSBetaRGB:
    tmp <<
      inputLines_[9].get<1>()->text().toFloat(),
      inputLines_[9].get<2>()->text().toFloat(),
      inputLines_[9].get<3>()->text().toFloat();

    input.from_ = tmp;
    input.white_ = r;
    input.sourceM_ = beta.GetM_Adapted(r, ada);
    input.sourceGamma_ = beta.GetGamma();

    out[0] = Convert::From_RGB_To_XYZ(input);
    out[1] = Convert::From_RGB_To_xyY(input);
    out[2] = Convert::From_RGB_To_Lab(input);
    out[3] = Convert::From_RGB_To_LCHab(input);
    out[4] = Convert::From_RGB_To_Luv(input);
    out[5] = Convert::From_RGB_To_LCHuv(input);

    input.targetM_1_ = adobe.GetM_1_Adapted(r, ada);
    input.targetGamma_ = adobe.GetGamma();
    out[6] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = apple.GetM_1_Adapted(r, ada);
    input.targetGamma_ = apple.GetGamma();
    out[7] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = best.GetM_1_Adapted(r, ada);
    input.targetGamma_ = best.GetGamma();
    out[8] = Convert::From_RGB_To_RGB(input);
    out[9] = tmp;

    input.targetM_1_ = bruce.GetM_1_Adapted(r, ada);
    input.targetGamma_ = bruce.GetGamma();
    out[10] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = cie.GetM_1_Adapted(r, ada);
    input.targetGamma_ = cie.GetGamma();
    out[11] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = colormatch.GetM_1_Adapted(r, ada);
    input.targetGamma_ = colormatch.GetGamma();
    out[12] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = don4.GetM_1_Adapted(r, ada);
    input.targetGamma_ = don4.GetGamma();
    out[13] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = eci.GetM_1_Adapted(r, ada);
    input.targetGamma_ = eci.GetGamma();
    out[14] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = ektaspaceps5.GetM_1_Adapted(r, ada);
    input.targetGamma_ = ektaspaceps5.GetGamma();
    out[15] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = ntsc.GetM_1_Adapted(r, ada);
    input.targetGamma_ = ntsc.GetGamma();
    out[16] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = pal_secam.GetM_1_Adapted(r, ada);
    input.targetGamma_ = pal_secam.GetGamma();
    out[17] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = prophoto.GetM_1_Adapted(r, ada);
    input.targetGamma_ = prophoto.GetGamma();
    out[18] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = smpte_c.GetM_1_Adapted(r, ada);
    input.targetGamma_ = smpte_c.GetGamma();
    out[19] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = srgb.GetM_1_Adapted(r, ada);
    input.targetGamma_ = srgb.GetGamma();
    out[20] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = widegamut.GetM_1_Adapted(r, ada);
    input.targetGamma_ = widegamut.GetGamma();
    out[21] = Convert::From_RGB_To_RGB(input);
    break;

  case Manager::CSBruceRGB:
    tmp <<
      inputLines_[10].get<1>()->text().toFloat(),
      inputLines_[10].get<2>()->text().toFloat(),
      inputLines_[10].get<3>()->text().toFloat();

    input.from_ = tmp;
    input.white_ = r;
    input.sourceM_ = bruce.GetM_Adapted(r, ada);
    input.sourceGamma_ = bruce.GetGamma();

    out[0] = Convert::From_RGB_To_XYZ(input);
    out[1] = Convert::From_RGB_To_xyY(input);
    out[2] = Convert::From_RGB_To_Lab(input);
    out[3] = Convert::From_RGB_To_LCHab(input);
    out[4] = Convert::From_RGB_To_Luv(input);
    out[5] = Convert::From_RGB_To_LCHuv(input);

    input.targetM_1_ = adobe.GetM_1_Adapted(r, ada);
    input.targetGamma_ = adobe.GetGamma();
    out[6] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = apple.GetM_1_Adapted(r, ada);
    input.targetGamma_ = apple.GetGamma();
    out[7] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = best.GetM_1_Adapted(r, ada);
    input.targetGamma_ = best.GetGamma();
    out[8] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = beta.GetM_1_Adapted(r, ada);
    input.targetGamma_ = beta.GetGamma();
    out[9] = Convert::From_RGB_To_RGB(input);
    out[10] = tmp;

    input.targetM_1_ = cie.GetM_1_Adapted(r, ada);
    input.targetGamma_ = cie.GetGamma();
    out[11] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = colormatch.GetM_1_Adapted(r, ada);
    input.targetGamma_ = colormatch.GetGamma();
    out[12] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = don4.GetM_1_Adapted(r, ada);
    input.targetGamma_ = don4.GetGamma();
    out[13] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = eci.GetM_1_Adapted(r, ada);
    input.targetGamma_ = eci.GetGamma();
    out[14] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = ektaspaceps5.GetM_1_Adapted(r, ada);
    input.targetGamma_ = ektaspaceps5.GetGamma();
    out[15] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = ntsc.GetM_1_Adapted(r, ada);
    input.targetGamma_ = ntsc.GetGamma();
    out[16] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = pal_secam.GetM_1_Adapted(r, ada);
    input.targetGamma_ = pal_secam.GetGamma();
    out[17] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = prophoto.GetM_1_Adapted(r, ada);
    input.targetGamma_ = prophoto.GetGamma();
    out[18] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = smpte_c.GetM_1_Adapted(r, ada);
    input.targetGamma_ = smpte_c.GetGamma();
    out[19] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = srgb.GetM_1_Adapted(r, ada);
    input.targetGamma_ = srgb.GetGamma();
    out[20] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = widegamut.GetM_1_Adapted(r, ada);
    input.targetGamma_ = widegamut.GetGamma();
    out[21] = Convert::From_RGB_To_RGB(input);
    break;

  case Manager::CSCIERGB:
    tmp <<
      inputLines_[11].get<1>()->text().toFloat(),
      inputLines_[11].get<2>()->text().toFloat(),
      inputLines_[11].get<3>()->text().toFloat();

    input.from_ = tmp;
    input.white_ = r;
    input.sourceM_ = cie.GetM_Adapted(r, ada);
    input.sourceGamma_ = cie.GetGamma();

    out[0] = Convert::From_RGB_To_XYZ(input);
    out[1] = Convert::From_RGB_To_xyY(input);
    out[2] = Convert::From_RGB_To_Lab(input);
    out[3] = Convert::From_RGB_To_LCHab(input);
    out[4] = Convert::From_RGB_To_Luv(input);
    out[5] = Convert::From_RGB_To_LCHuv(input);

    input.targetM_1_ = adobe.GetM_1_Adapted(r, ada);
    input.targetGamma_ = adobe.GetGamma();
    out[6] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = apple.GetM_1_Adapted(r, ada);
    input.targetGamma_ = apple.GetGamma();
    out[7] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = best.GetM_1_Adapted(r, ada);
    input.targetGamma_ = best.GetGamma();
    out[8] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = beta.GetM_1_Adapted(r, ada);
    input.targetGamma_ = beta.GetGamma();
    out[9] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = bruce.GetM_1_Adapted(r, ada);
    input.targetGamma_ = bruce.GetGamma();
    out[10] = Convert::From_RGB_To_RGB(input);
    out[11] = tmp;

    input.targetM_1_ = colormatch.GetM_1_Adapted(r, ada);
    input.targetGamma_ = colormatch.GetGamma();
    out[12] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = don4.GetM_1_Adapted(r, ada);
    input.targetGamma_ = don4.GetGamma();
    out[13] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = eci.GetM_1_Adapted(r, ada);
    input.targetGamma_ = eci.GetGamma();
    out[14] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = ektaspaceps5.GetM_1_Adapted(r, ada);
    input.targetGamma_ = ektaspaceps5.GetGamma();
    out[15] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = ntsc.GetM_1_Adapted(r, ada);
    input.targetGamma_ = ntsc.GetGamma();
    out[16] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = pal_secam.GetM_1_Adapted(r, ada);
    input.targetGamma_ = pal_secam.GetGamma();
    out[17] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = prophoto.GetM_1_Adapted(r, ada);
    input.targetGamma_ = prophoto.GetGamma();
    out[18] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = smpte_c.GetM_1_Adapted(r, ada);
    input.targetGamma_ = smpte_c.GetGamma();
    out[19] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = srgb.GetM_1_Adapted(r, ada);
    input.targetGamma_ = srgb.GetGamma();
    out[20] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = widegamut.GetM_1_Adapted(r, ada);
    input.targetGamma_ = widegamut.GetGamma();
    out[21] = Convert::From_RGB_To_RGB(input);
    break;

  case Manager::CSColorMatchRGB:
    tmp <<
      inputLines_[12].get<1>()->text().toFloat(),
      inputLines_[12].get<2>()->text().toFloat(),
      inputLines_[12].get<3>()->text().toFloat();

    input.from_ = tmp;
    input.white_ = r;
    input.sourceM_ = colormatch.GetM_Adapted(r, ada);
    input.sourceGamma_ = colormatch.GetGamma();

    out[0] = Convert::From_RGB_To_XYZ(input);
    out[1] = Convert::From_RGB_To_xyY(input);
    out[2] = Convert::From_RGB_To_Lab(input);
    out[3] = Convert::From_RGB_To_LCHab(input);
    out[4] = Convert::From_RGB_To_Luv(input);
    out[5] = Convert::From_RGB_To_LCHuv(input);

    input.targetM_1_ = adobe.GetM_1_Adapted(r, ada);
    input.targetGamma_ = adobe.GetGamma();
    out[6] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = apple.GetM_1_Adapted(r, ada);
    input.targetGamma_ = apple.GetGamma();
    out[7] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = best.GetM_1_Adapted(r, ada);
    input.targetGamma_ = best.GetGamma();
    out[8] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = beta.GetM_1_Adapted(r, ada);
    input.targetGamma_ = beta.GetGamma();
    out[9] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = bruce.GetM_1_Adapted(r, ada);
    input.targetGamma_ = bruce.GetGamma();
    out[10] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = cie.GetM_1_Adapted(r, ada);
    input.targetGamma_ = cie.GetGamma();
    out[11] = Convert::From_RGB_To_RGB(input);
    out[12] = tmp;

    input.targetM_1_ = don4.GetM_1_Adapted(r, ada);
    input.targetGamma_ = don4.GetGamma();
    out[13] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = eci.GetM_1_Adapted(r, ada);
    input.targetGamma_ = eci.GetGamma();
    out[14] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = ektaspaceps5.GetM_1_Adapted(r, ada);
    input.targetGamma_ = ektaspaceps5.GetGamma();
    out[15] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = ntsc.GetM_1_Adapted(r, ada);
    input.targetGamma_ = ntsc.GetGamma();
    out[16] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = pal_secam.GetM_1_Adapted(r, ada);
    input.targetGamma_ = pal_secam.GetGamma();
    out[17] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = prophoto.GetM_1_Adapted(r, ada);
    input.targetGamma_ = prophoto.GetGamma();
    out[18] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = smpte_c.GetM_1_Adapted(r, ada);
    input.targetGamma_ = smpte_c.GetGamma();
    out[19] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = srgb.GetM_1_Adapted(r, ada);
    input.targetGamma_ = srgb.GetGamma();
    out[20] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = widegamut.GetM_1_Adapted(r, ada);
    input.targetGamma_ = widegamut.GetGamma();
    out[21] = Convert::From_RGB_To_RGB(input);
    break;

  case Manager::CSDonRGB4:
    tmp <<
      inputLines_[13].get<1>()->text().toFloat(),
      inputLines_[13].get<2>()->text().toFloat(),
      inputLines_[13].get<3>()->text().toFloat();

    input.from_ = tmp;
    input.white_ = r;
    input.sourceM_ = don4.GetM_Adapted(r, ada);
    input.sourceGamma_ = don4.GetGamma();

    out[0] = Convert::From_RGB_To_XYZ(input);
    out[1] = Convert::From_RGB_To_xyY(input);
    out[2] = Convert::From_RGB_To_Lab(input);
    out[3] = Convert::From_RGB_To_LCHab(input);
    out[4] = Convert::From_RGB_To_Luv(input);
    out[5] = Convert::From_RGB_To_LCHuv(input);

    input.targetM_1_ = adobe.GetM_1_Adapted(r, ada);
    input.targetGamma_ = adobe.GetGamma();
    out[6] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = apple.GetM_1_Adapted(r, ada);
    input.targetGamma_ = apple.GetGamma();
    out[7] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = best.GetM_1_Adapted(r, ada);
    input.targetGamma_ = best.GetGamma();
    out[8] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = beta.GetM_1_Adapted(r, ada);
    input.targetGamma_ = beta.GetGamma();
    out[9] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = bruce.GetM_1_Adapted(r, ada);
    input.targetGamma_ = bruce.GetGamma();
    out[10] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = cie.GetM_1_Adapted(r, ada);
    input.targetGamma_ = cie.GetGamma();
    out[11] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = colormatch.GetM_1_Adapted(r, ada);
    input.targetGamma_ = colormatch.GetGamma();
    out[12] = Convert::From_RGB_To_RGB(input);
    out[13] = tmp;

    input.targetM_1_ = eci.GetM_1_Adapted(r, ada);
    input.targetGamma_ = eci.GetGamma();
    out[14] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = ektaspaceps5.GetM_1_Adapted(r, ada);
    input.targetGamma_ = ektaspaceps5.GetGamma();
    out[15] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = ntsc.GetM_1_Adapted(r, ada);
    input.targetGamma_ = ntsc.GetGamma();
    out[16] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = pal_secam.GetM_1_Adapted(r, ada);
    input.targetGamma_ = pal_secam.GetGamma();
    out[17] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = prophoto.GetM_1_Adapted(r, ada);
    input.targetGamma_ = prophoto.GetGamma();
    out[18] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = smpte_c.GetM_1_Adapted(r, ada);
    input.targetGamma_ = smpte_c.GetGamma();
    out[19] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = srgb.GetM_1_Adapted(r, ada);
    input.targetGamma_ = srgb.GetGamma();
    out[20] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = widegamut.GetM_1_Adapted(r, ada);
    input.targetGamma_ = widegamut.GetGamma();
    out[21] = Convert::From_RGB_To_RGB(input);
    break;

  case Manager::CSECIRGB:
    tmp <<
      inputLines_[14].get<1>()->text().toFloat(),
      inputLines_[14].get<2>()->text().toFloat(),
      inputLines_[14].get<3>()->text().toFloat();

    input.from_ = tmp;
    input.white_ = r;
    input.sourceM_ = eci.GetM_Adapted(r, ada);
    input.sourceGamma_ = eci.GetGamma();

    out[0] = Convert::From_RGB_To_XYZ(input);
    out[1] = Convert::From_RGB_To_xyY(input);
    out[2] = Convert::From_RGB_To_Lab(input);
    out[3] = Convert::From_RGB_To_LCHab(input);
    out[4] = Convert::From_RGB_To_Luv(input);
    out[5] = Convert::From_RGB_To_LCHuv(input);

    input.targetM_1_ = adobe.GetM_1_Adapted(r, ada);
    input.targetGamma_ = adobe.GetGamma();
    out[6] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = apple.GetM_1_Adapted(r, ada);
    input.targetGamma_ = apple.GetGamma();
    out[7] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = best.GetM_1_Adapted(r, ada);
    input.targetGamma_ = best.GetGamma();
    out[8] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = beta.GetM_1_Adapted(r, ada);
    input.targetGamma_ = beta.GetGamma();
    out[9] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = bruce.GetM_1_Adapted(r, ada);
    input.targetGamma_ = bruce.GetGamma();
    out[10] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = cie.GetM_1_Adapted(r, ada);
    input.targetGamma_ = cie.GetGamma();
    out[11] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = colormatch.GetM_1_Adapted(r, ada);
    input.targetGamma_ = colormatch.GetGamma();
    out[12] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = don4.GetM_1_Adapted(r, ada);
    input.targetGamma_ = don4.GetGamma();
    out[13] = Convert::From_RGB_To_RGB(input);
    out[14] = tmp;

    input.targetM_1_ = ektaspaceps5.GetM_1_Adapted(r, ada);
    input.targetGamma_ = ektaspaceps5.GetGamma();
    out[15] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = ntsc.GetM_1_Adapted(r, ada);
    input.targetGamma_ = ntsc.GetGamma();
    out[16] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = pal_secam.GetM_1_Adapted(r, ada);
    input.targetGamma_ = pal_secam.GetGamma();
    out[17] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = prophoto.GetM_1_Adapted(r, ada);
    input.targetGamma_ = prophoto.GetGamma();
    out[18] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = smpte_c.GetM_1_Adapted(r, ada);
    input.targetGamma_ = smpte_c.GetGamma();
    out[19] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = srgb.GetM_1_Adapted(r, ada);
    input.targetGamma_ = srgb.GetGamma();
    out[20] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = widegamut.GetM_1_Adapted(r, ada);
    input.targetGamma_ = widegamut.GetGamma();
    out[21] = Convert::From_RGB_To_RGB(input);
    break;

  case Manager::CSEktaSpacePS5:
    tmp <<
      inputLines_[15].get<1>()->text().toFloat(),
      inputLines_[15].get<2>()->text().toFloat(),
      inputLines_[15].get<3>()->text().toFloat();

    input.from_ = tmp;
    input.white_ = r;
    input.sourceM_ = ektaspaceps5.GetM_Adapted(r, ada);
    input.sourceGamma_ = ektaspaceps5.GetGamma();

    out[0] = Convert::From_RGB_To_XYZ(input);
    out[1] = Convert::From_RGB_To_xyY(input);
    out[2] = Convert::From_RGB_To_Lab(input);
    out[3] = Convert::From_RGB_To_LCHab(input);
    out[4] = Convert::From_RGB_To_Luv(input);
    out[5] = Convert::From_RGB_To_LCHuv(input);

    input.targetM_1_ = adobe.GetM_1_Adapted(r, ada);
    input.targetGamma_ = adobe.GetGamma();
    out[6] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = apple.GetM_1_Adapted(r, ada);
    input.targetGamma_ = apple.GetGamma();
    out[7] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = best.GetM_1_Adapted(r, ada);
    input.targetGamma_ = best.GetGamma();
    out[8] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = beta.GetM_1_Adapted(r, ada);
    input.targetGamma_ = beta.GetGamma();
    out[9] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = bruce.GetM_1_Adapted(r, ada);
    input.targetGamma_ = bruce.GetGamma();
    out[10] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = cie.GetM_1_Adapted(r, ada);
    input.targetGamma_ = cie.GetGamma();
    out[11] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = colormatch.GetM_1_Adapted(r, ada);
    input.targetGamma_ = colormatch.GetGamma();
    out[12] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = don4.GetM_1_Adapted(r, ada);
    input.targetGamma_ = don4.GetGamma();
    out[13] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = eci.GetM_1_Adapted(r, ada);
    input.targetGamma_ = eci.GetGamma();
    out[14] = Convert::From_RGB_To_RGB(input);
    out[15] = tmp;

    input.targetM_1_ = ntsc.GetM_1_Adapted(r, ada);
    input.targetGamma_ = ntsc.GetGamma();
    out[16] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = pal_secam.GetM_1_Adapted(r, ada);
    input.targetGamma_ = pal_secam.GetGamma();
    out[17] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = prophoto.GetM_1_Adapted(r, ada);
    input.targetGamma_ = prophoto.GetGamma();
    out[18] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = smpte_c.GetM_1_Adapted(r, ada);
    input.targetGamma_ = smpte_c.GetGamma();
    out[19] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = srgb.GetM_1_Adapted(r, ada);
    input.targetGamma_ = srgb.GetGamma();
    out[20] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = widegamut.GetM_1_Adapted(r, ada);
    input.targetGamma_ = widegamut.GetGamma();
    out[21] = Convert::From_RGB_To_RGB(input);
    break;

  case Manager::CSNTSCRGB:
    tmp <<
      inputLines_[16].get<1>()->text().toFloat(),
      inputLines_[16].get<2>()->text().toFloat(),
      inputLines_[16].get<3>()->text().toFloat();

    input.from_ = tmp;
    input.white_ = r;
    input.sourceM_ = ntsc.GetM_Adapted(r, ada);
    input.sourceGamma_ = ntsc.GetGamma();

    out[0] = Convert::From_RGB_To_XYZ(input);
    out[1] = Convert::From_RGB_To_xyY(input);
    out[2] = Convert::From_RGB_To_Lab(input);
    out[3] = Convert::From_RGB_To_LCHab(input);
    out[4] = Convert::From_RGB_To_Luv(input);
    out[5] = Convert::From_RGB_To_LCHuv(input);

    input.targetM_1_ = adobe.GetM_1_Adapted(r, ada);
    input.targetGamma_ = adobe.GetGamma();
    out[6] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = apple.GetM_1_Adapted(r, ada);
    input.targetGamma_ = apple.GetGamma();
    out[7] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = best.GetM_1_Adapted(r, ada);
    input.targetGamma_ = best.GetGamma();
    out[8] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = beta.GetM_1_Adapted(r, ada);
    input.targetGamma_ = beta.GetGamma();
    out[9] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = bruce.GetM_1_Adapted(r, ada);
    input.targetGamma_ = bruce.GetGamma();
    out[10] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = cie.GetM_1_Adapted(r, ada);
    input.targetGamma_ = cie.GetGamma();
    out[11] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = colormatch.GetM_1_Adapted(r, ada);
    input.targetGamma_ = colormatch.GetGamma();
    out[12] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = don4.GetM_1_Adapted(r, ada);
    input.targetGamma_ = don4.GetGamma();
    out[13] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = eci.GetM_1_Adapted(r, ada);
    input.targetGamma_ = eci.GetGamma();
    out[14] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = ektaspaceps5.GetM_1_Adapted(r, ada);
    input.targetGamma_ = ektaspaceps5.GetGamma();
    out[15] = Convert::From_RGB_To_RGB(input);
    out[16] = tmp;

    input.targetM_1_ = pal_secam.GetM_1_Adapted(r, ada);
    input.targetGamma_ = pal_secam.GetGamma();
    out[17] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = prophoto.GetM_1_Adapted(r, ada);
    input.targetGamma_ = prophoto.GetGamma();
    out[18] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = smpte_c.GetM_1_Adapted(r, ada);
    input.targetGamma_ = smpte_c.GetGamma();
    out[19] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = srgb.GetM_1_Adapted(r, ada);
    input.targetGamma_ = srgb.GetGamma();
    out[20] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = widegamut.GetM_1_Adapted(r, ada);
    input.targetGamma_ = widegamut.GetGamma();
    out[21] = Convert::From_RGB_To_RGB(input);
    break;

  case Manager::CSPALSECAMRGB:
    tmp <<
      inputLines_[17].get<1>()->text().toFloat(),
      inputLines_[17].get<2>()->text().toFloat(),
      inputLines_[17].get<3>()->text().toFloat();

    input.from_ = tmp;
    input.white_ = r;
    input.sourceM_ = pal_secam.GetM_Adapted(r, ada);
    input.sourceGamma_ = pal_secam.GetGamma();

    out[0] = Convert::From_RGB_To_XYZ(input);
    out[1] = Convert::From_RGB_To_xyY(input);
    out[2] = Convert::From_RGB_To_Lab(input);
    out[3] = Convert::From_RGB_To_LCHab(input);
    out[4] = Convert::From_RGB_To_Luv(input);
    out[5] = Convert::From_RGB_To_LCHuv(input);

    input.targetM_1_ = adobe.GetM_1_Adapted(r, ada);
    input.targetGamma_ = adobe.GetGamma();
    out[6] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = apple.GetM_1_Adapted(r, ada);
    input.targetGamma_ = apple.GetGamma();
    out[7] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = best.GetM_1_Adapted(r, ada);
    input.targetGamma_ = best.GetGamma();
    out[8] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = beta.GetM_1_Adapted(r, ada);
    input.targetGamma_ = beta.GetGamma();
    out[9] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = bruce.GetM_1_Adapted(r, ada);
    input.targetGamma_ = bruce.GetGamma();
    out[10] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = cie.GetM_1_Adapted(r, ada);
    input.targetGamma_ = cie.GetGamma();
    out[11] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = colormatch.GetM_1_Adapted(r, ada);
    input.targetGamma_ = colormatch.GetGamma();
    out[12] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = don4.GetM_1_Adapted(r, ada);
    input.targetGamma_ = don4.GetGamma();
    out[13] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = eci.GetM_1_Adapted(r, ada);
    input.targetGamma_ = eci.GetGamma();
    out[14] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = ektaspaceps5.GetM_1_Adapted(r, ada);
    input.targetGamma_ = ektaspaceps5.GetGamma();
    out[15] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = ntsc.GetM_1_Adapted(r, ada);
    input.targetGamma_ = ntsc.GetGamma();
    out[16] = Convert::From_RGB_To_RGB(input);
    out[17] = tmp;

    input.targetM_1_ = prophoto.GetM_1_Adapted(r, ada);
    input.targetGamma_ = prophoto.GetGamma();
    out[18] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = smpte_c.GetM_1_Adapted(r, ada);
    input.targetGamma_ = smpte_c.GetGamma();
    out[19] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = srgb.GetM_1_Adapted(r, ada);
    input.targetGamma_ = srgb.GetGamma();
    out[20] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = widegamut.GetM_1_Adapted(r, ada);
    input.targetGamma_ = widegamut.GetGamma();
    out[21] = Convert::From_RGB_To_RGB(input);
    break;

  case Manager::CSProPhotoRGB:
    tmp <<
      inputLines_[18].get<1>()->text().toFloat(),
      inputLines_[18].get<2>()->text().toFloat(),
      inputLines_[18].get<3>()->text().toFloat();

    input.from_ = tmp;
    input.white_ = r;
    input.sourceM_ = prophoto.GetM_Adapted(r, ada);
    input.sourceGamma_ = prophoto.GetGamma();

    out[0] = Convert::From_RGB_To_XYZ(input);
    out[1] = Convert::From_RGB_To_xyY(input);
    out[2] = Convert::From_RGB_To_Lab(input);
    out[3] = Convert::From_RGB_To_LCHab(input);
    out[4] = Convert::From_RGB_To_Luv(input);
    out[5] = Convert::From_RGB_To_LCHuv(input);

    input.targetM_1_ = adobe.GetM_1_Adapted(r, ada);
    input.targetGamma_ = adobe.GetGamma();
    out[6] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = apple.GetM_1_Adapted(r, ada);
    input.targetGamma_ = apple.GetGamma();
    out[7] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = best.GetM_1_Adapted(r, ada);
    input.targetGamma_ = best.GetGamma();
    out[8] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = beta.GetM_1_Adapted(r, ada);
    input.targetGamma_ = beta.GetGamma();
    out[9] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = bruce.GetM_1_Adapted(r, ada);
    input.targetGamma_ = bruce.GetGamma();
    out[10] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = cie.GetM_1_Adapted(r, ada);
    input.targetGamma_ = cie.GetGamma();
    out[11] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = colormatch.GetM_1_Adapted(r, ada);
    input.targetGamma_ = colormatch.GetGamma();
    out[12] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = don4.GetM_1_Adapted(r, ada);
    input.targetGamma_ = don4.GetGamma();
    out[13] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = eci.GetM_1_Adapted(r, ada);
    input.targetGamma_ = eci.GetGamma();
    out[14] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = ektaspaceps5.GetM_1_Adapted(r, ada);
    input.targetGamma_ = ektaspaceps5.GetGamma();
    out[15] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = ntsc.GetM_1_Adapted(r, ada);
    input.targetGamma_ = ntsc.GetGamma();
    out[16] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = pal_secam.GetM_1_Adapted(r, ada);
    input.targetGamma_ = pal_secam.GetGamma();
    out[17] = Convert::From_RGB_To_RGB(input);
    out[18] = tmp;

    input.targetM_1_ = smpte_c.GetM_1_Adapted(r, ada);
    input.targetGamma_ = smpte_c.GetGamma();
    out[19] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = srgb.GetM_1_Adapted(r, ada);
    input.targetGamma_ = srgb.GetGamma();
    out[20] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = widegamut.GetM_1_Adapted(r, ada);
    input.targetGamma_ = widegamut.GetGamma();
    out[21] = Convert::From_RGB_To_RGB(input);
    break;

  case Manager::CSSMPTECRGB:
    tmp <<
      inputLines_[19].get<1>()->text().toFloat(),
      inputLines_[19].get<2>()->text().toFloat(),
      inputLines_[19].get<3>()->text().toFloat();

    input.from_ = tmp;
    input.white_ = r;
    input.sourceM_ = smpte_c.GetM_Adapted(r, ada);
    input.sourceGamma_ = smpte_c.GetGamma();

    out[0] = Convert::From_RGB_To_XYZ(input);
    out[1] = Convert::From_RGB_To_xyY(input);
    out[2] = Convert::From_RGB_To_Lab(input);
    out[3] = Convert::From_RGB_To_LCHab(input);
    out[4] = Convert::From_RGB_To_Luv(input);
    out[5] = Convert::From_RGB_To_LCHuv(input);

    input.targetM_1_ = adobe.GetM_1_Adapted(r, ada);
    input.targetGamma_ = adobe.GetGamma();
    out[6] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = apple.GetM_1_Adapted(r, ada);
    input.targetGamma_ = apple.GetGamma();
    out[7] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = best.GetM_1_Adapted(r, ada);
    input.targetGamma_ = best.GetGamma();
    out[8] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = beta.GetM_1_Adapted(r, ada);
    input.targetGamma_ = beta.GetGamma();
    out[9] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = bruce.GetM_1_Adapted(r, ada);
    input.targetGamma_ = bruce.GetGamma();
    out[10] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = cie.GetM_1_Adapted(r, ada);
    input.targetGamma_ = cie.GetGamma();
    out[11] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = colormatch.GetM_1_Adapted(r, ada);
    input.targetGamma_ = colormatch.GetGamma();
    out[12] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = don4.GetM_1_Adapted(r, ada);
    input.targetGamma_ = don4.GetGamma();
    out[13] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = eci.GetM_1_Adapted(r, ada);
    input.targetGamma_ = eci.GetGamma();
    out[14] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = ektaspaceps5.GetM_1_Adapted(r, ada);
    input.targetGamma_ = ektaspaceps5.GetGamma();
    out[15] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = ntsc.GetM_1_Adapted(r, ada);
    input.targetGamma_ = ntsc.GetGamma();
    out[16] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = pal_secam.GetM_1_Adapted(r, ada);
    input.targetGamma_ = pal_secam.GetGamma();
    out[17] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = prophoto.GetM_1_Adapted(r, ada);
    input.targetGamma_ = prophoto.GetGamma();
    out[18] = Convert::From_RGB_To_RGB(input);
    out[19] = tmp;

    input.targetM_1_ = srgb.GetM_1_Adapted(r, ada);
    input.targetGamma_ = srgb.GetGamma();
    out[20] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = widegamut.GetM_1_Adapted(r, ada);
    input.targetGamma_ = widegamut.GetGamma();
    out[21] = Convert::From_RGB_To_RGB(input);
    break;

  case Manager::CSsRGB:
    tmp <<
      inputLines_[20].get<1>()->text().toFloat(),
      inputLines_[20].get<2>()->text().toFloat(),
      inputLines_[20].get<3>()->text().toFloat();

    input.from_ = tmp;
    input.white_ = r;
    input.sourceM_ = srgb.GetM_Adapted(r, ada);
    input.sourceGamma_ = srgb.GetGamma();

    out[0] = Convert::From_RGB_To_XYZ(input);
    out[1] = Convert::From_RGB_To_xyY(input);
    out[2] = Convert::From_RGB_To_Lab(input);
    out[3] = Convert::From_RGB_To_LCHab(input);
    out[4] = Convert::From_RGB_To_Luv(input);
    out[5] = Convert::From_RGB_To_LCHuv(input);

    input.targetM_1_ = adobe.GetM_1_Adapted(r, ada);
    input.targetGamma_ = adobe.GetGamma();
    out[6] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = apple.GetM_1_Adapted(r, ada);
    input.targetGamma_ = apple.GetGamma();
    out[7] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = best.GetM_1_Adapted(r, ada);
    input.targetGamma_ = best.GetGamma();
    out[8] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = beta.GetM_1_Adapted(r, ada);
    input.targetGamma_ = beta.GetGamma();
    out[9] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = bruce.GetM_1_Adapted(r, ada);
    input.targetGamma_ = bruce.GetGamma();
    out[10] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = cie.GetM_1_Adapted(r, ada);
    input.targetGamma_ = cie.GetGamma();
    out[11] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = colormatch.GetM_1_Adapted(r, ada);
    input.targetGamma_ = colormatch.GetGamma();
    out[12] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = don4.GetM_1_Adapted(r, ada);
    input.targetGamma_ = don4.GetGamma();
    out[13] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = eci.GetM_1_Adapted(r, ada);
    input.targetGamma_ = eci.GetGamma();
    out[14] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = ektaspaceps5.GetM_1_Adapted(r, ada);
    input.targetGamma_ = ektaspaceps5.GetGamma();
    out[15] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = ntsc.GetM_1_Adapted(r, ada);
    input.targetGamma_ = ntsc.GetGamma();
    out[16] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = pal_secam.GetM_1_Adapted(r, ada);
    input.targetGamma_ = pal_secam.GetGamma();
    out[17] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = prophoto.GetM_1_Adapted(r, ada);
    input.targetGamma_ = prophoto.GetGamma();
    out[18] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = smpte_c.GetM_1_Adapted(r, ada);
    input.targetGamma_ = smpte_c.GetGamma();
    out[19] = Convert::From_RGB_To_RGB(input);
    out[20] = tmp;

    input.targetM_1_ = widegamut.GetM_1_Adapted(r, ada);
    input.targetGamma_ = widegamut.GetGamma();
    out[21] = Convert::From_RGB_To_RGB(input);
    break;

  case Manager::CSWideGamutRGB:
    tmp <<
      inputLines_[21].get<1>()->text().toFloat(),
      inputLines_[21].get<2>()->text().toFloat(),
      inputLines_[21].get<3>()->text().toFloat();

    input.from_ = tmp;
    input.white_ = r;
    input.sourceM_ = widegamut.GetM_Adapted(r, ada);
    input.sourceGamma_ = widegamut.GetGamma();

    out[0] = Convert::From_RGB_To_XYZ(input);
    out[1] = Convert::From_RGB_To_xyY(input);
    out[2] = Convert::From_RGB_To_Lab(input);
    out[3] = Convert::From_RGB_To_LCHab(input);
    out[4] = Convert::From_RGB_To_Luv(input);
    out[5] = Convert::From_RGB_To_LCHuv(input);

    input.targetM_1_ = adobe.GetM_1_Adapted(r, ada);
    input.targetGamma_ = adobe.GetGamma();
    out[6] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = apple.GetM_1_Adapted(r, ada);
    input.targetGamma_ = apple.GetGamma();
    out[7] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = best.GetM_1_Adapted(r, ada);
    input.targetGamma_ = best.GetGamma();
    out[8] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = beta.GetM_1_Adapted(r, ada);
    input.targetGamma_ = beta.GetGamma();
    out[9] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = bruce.GetM_1_Adapted(r, ada);
    input.targetGamma_ = bruce.GetGamma();
    out[10] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = cie.GetM_1_Adapted(r, ada);
    input.targetGamma_ = cie.GetGamma();
    out[11] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = colormatch.GetM_1_Adapted(r, ada);
    input.targetGamma_ = colormatch.GetGamma();
    out[12] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = don4.GetM_1_Adapted(r, ada);
    input.targetGamma_ = don4.GetGamma();
    out[13] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = eci.GetM_1_Adapted(r, ada);
    input.targetGamma_ = eci.GetGamma();
    out[14] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = ektaspaceps5.GetM_1_Adapted(r, ada);
    input.targetGamma_ = ektaspaceps5.GetGamma();
    out[15] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = ntsc.GetM_1_Adapted(r, ada);
    input.targetGamma_ = ntsc.GetGamma();
    out[16] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = pal_secam.GetM_1_Adapted(r, ada);
    input.targetGamma_ = pal_secam.GetGamma();
    out[17] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = prophoto.GetM_1_Adapted(r, ada);
    input.targetGamma_ = prophoto.GetGamma();
    out[18] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = smpte_c.GetM_1_Adapted(r, ada);
    input.targetGamma_ = smpte_c.GetGamma();
    out[19] = Convert::From_RGB_To_RGB(input);

    input.targetM_1_ = srgb.GetM_1_Adapted(r, ada);
    input.targetGamma_ = srgb.GetGamma();
    out[20] = Convert::From_RGB_To_RGB(input);
    out[21] = tmp;
    break;

  }

  int i = 0;
  BOOST_FOREACH(InputLine& rL, inputLines_){
    rL.get<1>()->setText(QString::number(out[i](0, 0), 'f', 10));
    rL.get<2>()->setText(QString::number(out[i](1, 0), 'f', 10));
    rL.get<3>()->setText(QString::number(out[i](2, 0), 'f', 10));
    i++;
  }
}
