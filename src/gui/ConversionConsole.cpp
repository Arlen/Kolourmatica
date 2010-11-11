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

  switch(convertingFrom_){

  case Manager::CSXYZ:
    tmp <<
      inputLines_[0].get<1>()->text().toFloat(),
      inputLines_[0].get<2>()->text().toFloat(),
      inputLines_[0].get<3>()->text().toFloat();

    out[0] = tmp;
    out[1] = Convert::From_XYZ_To_xyY(tmp);
    out[2] = Convert::From_XYZ_To_Lab(tmp, r);
    out[3] = Convert::From_XYZ_To_LCHab(tmp, r);
    out[4] = Convert::From_XYZ_To_Luv(tmp, r);
    out[5] = Convert::From_XYZ_To_LCHuv(tmp, r);
    out[6] = Convert::From_XYZ_To_RGB(tmp, r, adobe.GetM_1_Adapted(r, ada),
				      adobe.GetGamma());
    out[7] = Convert::From_XYZ_To_RGB(tmp, r, apple.GetM_1_Adapted(r, ada),
				      apple.GetGamma());
    out[8] = Convert::From_XYZ_To_RGB(tmp, r, best.GetM_1_Adapted(r, ada),
				      best.GetGamma());
    out[9] = Convert::From_XYZ_To_RGB(tmp, r, beta.GetM_1_Adapted(r, ada),
				      beta.GetGamma());
    out[10] = Convert::From_XYZ_To_RGB(tmp, r,
				       bruce.GetM_1_Adapted(r, ada),
				       bruce.GetGamma());
    out[11] = Convert::From_XYZ_To_RGB(tmp, r, cie.GetM_1_Adapted(r, ada),
				       cie.GetGamma());
    out[12] = Convert::From_XYZ_To_RGB(tmp, r,
				       colormatch.GetM_1_Adapted(r, ada),
				       colormatch.GetGamma());
    out[13] = Convert::From_XYZ_To_RGB(tmp, r, don4.GetM_1_Adapted(r, ada),
				       don4.GetGamma());
    out[14] = Convert::From_XYZ_To_RGB(tmp, r, eci.GetM_1_Adapted(r, ada),
				       eci.GetGamma());
    out[15] = Convert::From_XYZ_To_RGB(tmp, r,
				       ektaspaceps5.GetM_1_Adapted(r, ada),
				       ektaspaceps5.GetGamma());
    out[16] = Convert::From_XYZ_To_RGB(tmp, r, ntsc.GetM_1_Adapted(r, ada),
				       ntsc.GetGamma());
    out[17] = Convert::From_XYZ_To_RGB(tmp, r,
				       pal_secam.GetM_1_Adapted(r, ada),
				       pal_secam.GetGamma());
    out[18] = Convert::From_XYZ_To_RGB(tmp, r,
				       prophoto.GetM_1_Adapted(r, ada),
				       prophoto.GetGamma());
    out[19] = Convert::From_XYZ_To_RGB(tmp, r,
				       smpte_c.GetM_1_Adapted(r, ada),
				       smpte_c.GetGamma());
    out[20] = Convert::From_XYZ_To_sRGB(tmp, r,
					srgb.GetM_1_Adapted(r, ada),
					srgb.GetGamma());
    out[21] = Convert::From_XYZ_To_RGB(tmp, r,
				       widegamut.GetM_1_Adapted(r, ada),
				       widegamut.GetGamma());
    break;

  case Manager::CSxyY:
    tmp <<
      inputLines_[1].get<1>()->text().toFloat(),
      inputLines_[1].get<2>()->text().toFloat(),
      inputLines_[1].get<3>()->text().toFloat();

    out[0] = Convert::From_xyY_To_XYZ(tmp);
    out[1] = tmp;
    out[2] = Convert::From_xyY_To_Lab(tmp, r);
    out[3] = Convert::From_xyY_To_LCHab(tmp, r);
    out[4] = Convert::From_xyY_To_Luv(tmp, r);
    out[5] = Convert::From_xyY_To_LCHuv(tmp, r);
    out[6] = Convert::From_xyY_To_RGB(tmp, r, adobe.GetM_1_Adapted(r, ada),
				      adobe.GetGamma());
    out[7] = Convert::From_xyY_To_RGB(tmp, r, apple.GetM_1_Adapted(r, ada),
				      apple.GetGamma());
    out[8] = Convert::From_xyY_To_RGB(tmp, r, best.GetM_1_Adapted(r, ada),
				      best.GetGamma());
    out[9] = Convert::From_xyY_To_RGB(tmp, r, beta.GetM_1_Adapted(r, ada),
				      beta.GetGamma());
    out[10] = Convert::From_xyY_To_RGB(tmp, r,
				       bruce.GetM_1_Adapted(r, ada),
				       bruce.GetGamma());
    out[11] = Convert::From_xyY_To_RGB(tmp, r, cie.GetM_1_Adapted(r, ada),
				       cie.GetGamma());
    out[12] = Convert::From_xyY_To_RGB(tmp, r,
				       colormatch.GetM_1_Adapted(r, ada),
				       colormatch.GetGamma());
    out[13] = Convert::From_xyY_To_RGB(tmp, r, don4.GetM_1_Adapted(r, ada),
				       don4.GetGamma());
    out[14] = Convert::From_xyY_To_RGB(tmp, r, eci.GetM_1_Adapted(r, ada),
				       eci.GetGamma());
    out[15] = Convert::From_xyY_To_RGB(tmp, r,
				       ektaspaceps5.GetM_1_Adapted(r, ada),
				       ektaspaceps5.GetGamma());
    out[16] = Convert::From_xyY_To_RGB(tmp, r, ntsc.GetM_1_Adapted(r, ada),
				       ntsc.GetGamma());
    out[17] = Convert::From_xyY_To_RGB(tmp, r,
				       pal_secam.GetM_1_Adapted(r, ada),
				       pal_secam.GetGamma());
    out[18] = Convert::From_xyY_To_RGB(tmp, r,
				       prophoto.GetM_1_Adapted(r, ada),
				       prophoto.GetGamma());
    out[19] = Convert::From_xyY_To_RGB(tmp, r,
				       smpte_c.GetM_1_Adapted(r, ada),
				       smpte_c.GetGamma());
    out[20] = Convert::From_xyY_To_sRGB(tmp, r,
					srgb.GetM_1_Adapted(r, ada),
					srgb.GetGamma());
    out[21] = Convert::From_xyY_To_RGB(tmp, r,
				       widegamut.GetM_1_Adapted(r, ada),
				       widegamut.GetGamma());
    break;

  case Manager::CSLab:
    tmp <<
      inputLines_[2].get<1>()->text().toFloat(),
      inputLines_[2].get<2>()->text().toFloat(),
      inputLines_[2].get<3>()->text().toFloat();

    out[0] = Convert::From_Lab_To_XYZ(tmp, r);
    out[1] = Convert::From_Lab_To_xyY(tmp, r);
    out[2] = tmp;
    out[3] = Convert::From_Lab_To_LCHab(tmp, r);
    out[4] = Convert::From_Lab_To_Luv(tmp, r);
    out[5] = Convert::From_Lab_To_LCHuv(tmp, r);
    out[6] = Convert::From_Lab_To_RGB(tmp, r, adobe.GetM_1_Adapted(r, ada),
				      adobe.GetGamma());
    out[7] = Convert::From_Lab_To_RGB(tmp, r, apple.GetM_1_Adapted(r, ada),
				      apple.GetGamma());
    out[8] = Convert::From_Lab_To_RGB(tmp, r, best.GetM_1_Adapted(r, ada),
				      best.GetGamma());
    out[9] = Convert::From_Lab_To_RGB(tmp, r, beta.GetM_1_Adapted(r, ada),
				      beta.GetGamma());
    out[10] = Convert::From_Lab_To_RGB(tmp, r,
				       bruce.GetM_1_Adapted(r, ada),
				       bruce.GetGamma());
    out[11] = Convert::From_Lab_To_RGB(tmp, r, cie.GetM_1_Adapted(r, ada),
				       cie.GetGamma());
    out[12] = Convert::From_Lab_To_RGB(tmp, r,
				       colormatch.GetM_1_Adapted(r, ada),
				       colormatch.GetGamma());
    out[13] = Convert::From_Lab_To_RGB(tmp, r, don4.GetM_1_Adapted(r, ada),
				       don4.GetGamma());
    out[14] = Convert::From_Lab_To_RGB(tmp, r, eci.GetM_1_Adapted(r, ada),
				       eci.GetGamma());
    out[15] = Convert::From_Lab_To_RGB(tmp, r,
				       ektaspaceps5.GetM_1_Adapted(r, ada),
				       ektaspaceps5.GetGamma());
    out[16] = Convert::From_Lab_To_RGB(tmp, r, ntsc.GetM_1_Adapted(r, ada),
				       ntsc.GetGamma());
    out[17] = Convert::From_Lab_To_RGB(tmp, r,
				       pal_secam.GetM_1_Adapted(r, ada),
				       pal_secam.GetGamma());
    out[18] = Convert::From_Lab_To_RGB(tmp, r,
				       prophoto.GetM_1_Adapted(r, ada),
				       prophoto.GetGamma());
    out[19] = Convert::From_Lab_To_RGB(tmp, r,
				       smpte_c.GetM_1_Adapted(r, ada),
				       smpte_c.GetGamma());
    out[20] = Convert::From_Lab_To_sRGB(tmp, r,
					srgb.GetM_1_Adapted(r, ada),
					srgb.GetGamma());
    out[21] = Convert::From_Lab_To_RGB(tmp, r,
				       widegamut.GetM_1_Adapted(r, ada),
				       widegamut.GetGamma());
    break;

  case Manager::CSLCHab:
    tmp <<
      inputLines_[3].get<1>()->text().toFloat(),
      inputLines_[3].get<2>()->text().toFloat(),
      inputLines_[3].get<3>()->text().toFloat();

    out[0] = Convert::From_LCHab_To_XYZ(tmp, r);
    out[1] = Convert::From_LCHab_To_xyY(tmp, r);
    out[2] = Convert::From_LCHab_To_Lab(tmp, r);
    out[3] = tmp;
    out[4] = Convert::From_LCHab_To_Luv(tmp, r);
    out[5] = Convert::From_LCHab_To_LCHuv(tmp, r);
    out[6] = Convert::From_LCHab_To_RGB(tmp, r,
					adobe.GetM_1_Adapted(r, ada),
					adobe.GetGamma());
    out[7] = Convert::From_LCHab_To_RGB(tmp, r,
					apple.GetM_1_Adapted(r, ada),
					apple.GetGamma());
    out[8] = Convert::From_LCHab_To_RGB(tmp, r,
					best.GetM_1_Adapted(r, ada),
					best.GetGamma());
    out[9] = Convert::From_LCHab_To_RGB(tmp, r,
					beta.GetM_1_Adapted(r, ada),
					beta.GetGamma());
    out[10] = Convert::From_LCHab_To_RGB(tmp, r,
					 bruce.GetM_1_Adapted(r, ada),
					 bruce.GetGamma());
    out[11] = Convert::From_LCHab_To_RGB(tmp, r,
					 cie.GetM_1_Adapted(r, ada),
					 cie.GetGamma());
    out[12] = Convert::From_LCHab_To_RGB(tmp, r,
					 colormatch.GetM_1_Adapted(r, ada),
					 colormatch.GetGamma());
    out[13] = Convert::From_LCHab_To_RGB(tmp, r,
					 don4.GetM_1_Adapted(r, ada),
					 don4.GetGamma());
    out[14] = Convert::From_LCHab_To_RGB(tmp, r,
					 eci.GetM_1_Adapted(r, ada),
					 eci.GetGamma());
    out[15] = Convert::From_LCHab_To_RGB(tmp, r,
					 ektaspaceps5.GetM_1_Adapted(r,
								     ada),
					 ektaspaceps5.GetGamma());
    out[16] = Convert::From_LCHab_To_RGB(tmp, r,
					 ntsc.GetM_1_Adapted(r, ada),
					 ntsc.GetGamma());
    out[17] = Convert::From_LCHab_To_RGB(tmp, r,
					 pal_secam.GetM_1_Adapted(r, ada),
					 pal_secam.GetGamma());
    out[18] = Convert::From_LCHab_To_RGB(tmp, r,
					 prophoto.GetM_1_Adapted(r, ada),
					 prophoto.GetGamma());
    out[19] = Convert::From_LCHab_To_RGB(tmp, r,
					 smpte_c.GetM_1_Adapted(r, ada),
					 smpte_c.GetGamma());
    out[20] = Convert::From_LCHab_To_sRGB(tmp, r,
					  srgb.GetM_1_Adapted(r, ada),
					  srgb.GetGamma());
    out[21] = Convert::From_LCHab_To_RGB(tmp, r,
					 widegamut.GetM_1_Adapted(r, ada),
					 widegamut.GetGamma());
    break;

  case Manager::CSLuv:
    tmp <<
      inputLines_[4].get<1>()->text().toFloat(),
      inputLines_[4].get<2>()->text().toFloat(),
      inputLines_[4].get<3>()->text().toFloat();

    out[0] = Convert::From_Luv_To_XYZ(tmp, r);
    out[1] = Convert::From_Luv_To_xyY(tmp, r);
    out[2] = Convert::From_Luv_To_Lab(tmp, r);
    out[3] = Convert::From_Luv_To_LCHab(tmp, r);
    out[4] = tmp;
    out[5] = Convert::From_Luv_To_LCHuv(tmp, r);
    out[6] = Convert::From_Luv_To_RGB(tmp, r, adobe.GetM_1_Adapted(r, ada),
				      adobe.GetGamma());
    out[7] = Convert::From_Luv_To_RGB(tmp, r, apple.GetM_1_Adapted(r, ada),
				      apple.GetGamma());
    out[8] = Convert::From_Luv_To_RGB(tmp, r, best.GetM_1_Adapted(r, ada),
				      best.GetGamma());
    out[9] = Convert::From_Luv_To_RGB(tmp, r, beta.GetM_1_Adapted(r, ada),
				      beta.GetGamma());
    out[10] = Convert::From_Luv_To_RGB(tmp, r,
				       bruce.GetM_1_Adapted(r, ada),
				       bruce.GetGamma());
    out[11] = Convert::From_Luv_To_RGB(tmp, r, cie.GetM_1_Adapted(r, ada),
				       cie.GetGamma());
    out[12] = Convert::From_Luv_To_RGB(tmp, r,
				       colormatch.GetM_1_Adapted(r, ada),
				       colormatch.GetGamma());
    out[13] = Convert::From_Luv_To_RGB(tmp, r, don4.GetM_1_Adapted(r, ada),
				       don4.GetGamma());
    out[14] = Convert::From_Luv_To_RGB(tmp, r, eci.GetM_1_Adapted(r, ada),
				       eci.GetGamma());
    out[15] = Convert::From_Luv_To_RGB(tmp, r,
				       ektaspaceps5.GetM_1_Adapted(r, ada),
				       ektaspaceps5.GetGamma());
    out[16] = Convert::From_Luv_To_RGB(tmp, r, ntsc.GetM_1_Adapted(r, ada),
				       ntsc.GetGamma());
    out[17] = Convert::From_Luv_To_RGB(tmp, r,
				       pal_secam.GetM_1_Adapted(r, ada),
				       pal_secam.GetGamma());
    out[18] = Convert::From_Luv_To_RGB(tmp, r,
				       prophoto.GetM_1_Adapted(r, ada),
				       prophoto.GetGamma());
    out[19] = Convert::From_Luv_To_RGB(tmp, r,
				       smpte_c.GetM_1_Adapted(r, ada),
				       smpte_c.GetGamma());
    out[20] = Convert::From_Luv_To_sRGB(tmp, r,
					srgb.GetM_1_Adapted(r, ada),
					srgb.GetGamma());
    out[21] = Convert::From_Luv_To_RGB(tmp, r,
				       widegamut.GetM_1_Adapted(r, ada),
				       widegamut.GetGamma());
    break;

  case Manager::CSLCHuv:
    tmp <<
      inputLines_[5].get<1>()->text().toFloat(),
      inputLines_[5].get<2>()->text().toFloat(),
      inputLines_[5].get<3>()->text().toFloat();

    out[0] = Convert::From_LCHuv_To_XYZ(tmp, r);
    out[1] = Convert::From_LCHuv_To_xyY(tmp, r);
    out[2] = Convert::From_LCHuv_To_Lab(tmp, r);
    out[3] = Convert::From_LCHuv_To_LCHab(tmp, r);
    out[4] = Convert::From_LCHuv_To_Luv(tmp, r);;
    out[5] = tmp;
    out[6] = Convert::From_LCHuv_To_RGB(tmp, r,
					adobe.GetM_1_Adapted(r, ada),
					adobe.GetGamma());
    out[7] = Convert::From_LCHuv_To_RGB(tmp, r,
					apple.GetM_1_Adapted(r, ada),
					apple.GetGamma());
    out[8] = Convert::From_LCHuv_To_RGB(tmp, r,best.GetM_1_Adapted(r, ada),
					best.GetGamma());
    out[9] = Convert::From_LCHuv_To_RGB(tmp, r,
					beta.GetM_1_Adapted(r, ada),
					beta.GetGamma());
    out[10] = Convert::From_LCHuv_To_RGB(tmp, r,
					 bruce.GetM_1_Adapted(r, ada),
					 bruce.GetGamma());
    out[11] = Convert::From_LCHuv_To_RGB(tmp, r,
					 cie.GetM_1_Adapted(r, ada),
					 cie.GetGamma());
    out[12] = Convert::From_LCHuv_To_RGB(tmp, r,
					 colormatch.GetM_1_Adapted(r, ada),
					 colormatch.GetGamma());
    out[13] = Convert::From_LCHuv_To_RGB(tmp, r,
					 don4.GetM_1_Adapted(r, ada),
					 don4.GetGamma());
    out[14] = Convert::From_LCHuv_To_RGB(tmp, r,
					 eci.GetM_1_Adapted(r, ada),
					 eci.GetGamma());
    out[15] = Convert::From_LCHuv_To_RGB(tmp, r,
					 ektaspaceps5.GetM_1_Adapted(r,
								     ada),
					 ektaspaceps5.GetGamma());
    out[16] = Convert::From_LCHuv_To_RGB(tmp, r,
					 ntsc.GetM_1_Adapted(r, ada),
					 ntsc.GetGamma());
    out[17] = Convert::From_LCHuv_To_RGB(tmp, r,
					 pal_secam.GetM_1_Adapted(r, ada),
					 pal_secam.GetGamma());
    out[18] = Convert::From_LCHuv_To_RGB(tmp, r,
					 prophoto.GetM_1_Adapted(r, ada),
					 prophoto.GetGamma());
    out[19] = Convert::From_LCHuv_To_RGB(tmp, r,
					 smpte_c.GetM_1_Adapted(r, ada),
					 smpte_c.GetGamma());
    out[20] = Convert::From_LCHuv_To_sRGB(tmp, r,
					  srgb.GetM_1_Adapted(r, ada),
					  srgb.GetGamma());
    out[21] = Convert::From_LCHuv_To_RGB(tmp, r,
					 widegamut.GetM_1_Adapted(r, ada),
					 widegamut.GetGamma());
    break;


  case Manager::CSAdobeRGB:
    tmp <<
      inputLines_[6].get<1>()->text().toFloat(),
      inputLines_[6].get<2>()->text().toFloat(),
      inputLines_[6].get<3>()->text().toFloat();

    out[0] = Convert::From_RGB_To_XYZ(tmp, r,
				      adobe.GetM_Adapted(r, ada),
				      adobe.GetGamma());
    out[1] = Convert::From_RGB_To_xyY(tmp, r,
				      adobe.GetM_Adapted(r, ada),
				      adobe.GetGamma());
    out[2] = Convert::From_RGB_To_Lab(tmp, r,
				      adobe.GetM_Adapted(r, ada),
				      adobe.GetGamma());
    out[3] = Convert::From_RGB_To_LCHab(tmp, r,
					adobe.GetM_Adapted(r, ada),
					adobe.GetGamma());
    out[4] = Convert::From_RGB_To_Luv(tmp, r,
				      adobe.GetM_Adapted(r, ada),
				      adobe.GetGamma());
    out[5] = Convert::From_RGB_To_LCHuv(tmp, r,
					adobe.GetM_Adapted(r, ada),
					adobe.GetGamma());
    out[6] = tmp;
    out[7] = Convert::From_RGB_To_RGB(tmp, r, apple.GetM_1_Adapted(r, ada),
				      apple.GetGamma(),
				      adobe.GetM_Adapted(r, ada),
				      adobe.GetGamma());
    out[8] = Convert::From_RGB_To_RGB(tmp, r, best.GetM_1_Adapted(r, ada),
				      best.GetGamma(),
				      adobe.GetM_Adapted(r, ada),
				      adobe.GetGamma());
    out[9] = Convert::From_RGB_To_RGB(tmp, r, beta.GetM_1_Adapted(r, ada),
				      beta.GetGamma(),
				      adobe.GetM_Adapted(r, ada),
				      adobe.GetGamma());
    out[10] = Convert::From_RGB_To_RGB(tmp, r,
				       bruce.GetM_1_Adapted(r, ada),
				       bruce.GetGamma(),
				       adobe.GetM_Adapted(r, ada),
				       adobe.GetGamma());
    out[11] = Convert::From_RGB_To_RGB(tmp, r, cie.GetM_1_Adapted(r, ada),
				       cie.GetGamma(),
				       adobe.GetM_Adapted(r, ada),
				       adobe.GetGamma());
    out[12] = Convert::From_RGB_To_RGB(tmp, r,
				       colormatch.GetM_1_Adapted(r, ada),
				       colormatch.GetGamma(),
				       adobe.GetM_Adapted(r, ada),
				       adobe.GetGamma());
    out[13] = Convert::From_RGB_To_RGB(tmp, r, don4.GetM_1_Adapted(r, ada),
				       don4.GetGamma(),
				       adobe.GetM_Adapted(r, ada),
				       adobe.GetGamma());
    out[14] = Convert::From_RGB_To_RGB(tmp, r, eci.GetM_1_Adapted(r, ada),
				       eci.GetGamma(),
				       adobe.GetM_Adapted(r, ada),
				       adobe.GetGamma());
    out[15] = Convert::From_RGB_To_RGB(tmp, r,
				       ektaspaceps5.GetM_1_Adapted(r, ada),
				       ektaspaceps5.GetGamma(),
				       adobe.GetM_Adapted(r, ada),
				       adobe.GetGamma());
    out[16] = Convert::From_RGB_To_RGB(tmp, r, ntsc.GetM_1_Adapted(r, ada),
				       ntsc.GetGamma(),
				       adobe.GetM_Adapted(r, ada),
				       adobe.GetGamma());
    out[17] = Convert::From_RGB_To_RGB(tmp, r,
				       pal_secam.GetM_1_Adapted(r, ada),
				       pal_secam.GetGamma(),
				       adobe.GetM_Adapted(r, ada),
				       adobe.GetGamma());
    out[18] = Convert::From_RGB_To_RGB(tmp, r,
				       prophoto.GetM_1_Adapted(r, ada),
				       prophoto.GetGamma(),
				       adobe.GetM_Adapted(r, ada),
				       adobe.GetGamma());
    out[19] = Convert::From_RGB_To_RGB(tmp, r,
				       smpte_c.GetM_1_Adapted(r, ada),
				       smpte_c.GetGamma(),
				       adobe.GetM_Adapted(r, ada),
				       adobe.GetGamma());
    out[20] = Convert::From_RGB_To_RGB(tmp, r, srgb.GetM_1_Adapted(r, ada),
				       srgb.GetGamma(),
				       adobe.GetM_Adapted(r, ada),
				       adobe.GetGamma());
    out[21] = Convert::From_RGB_To_RGB(tmp, r,
				       widegamut.GetM_1_Adapted(r, ada),
				       widegamut.GetGamma(),
				       adobe.GetM_Adapted(r, ada),
				       adobe.GetGamma());
    break;


  case Manager::CSAppleRGB:
    tmp <<
      inputLines_[7].get<1>()->text().toFloat(),
      inputLines_[7].get<2>()->text().toFloat(),
      inputLines_[7].get<3>()->text().toFloat();

    out[0] = Convert::From_RGB_To_XYZ(tmp, r,
				      apple.GetM_Adapted(r, ada),
				      apple.GetGamma());
    out[1] = Convert::From_RGB_To_xyY(tmp, r,
				      apple.GetM_Adapted(r, ada),
				      apple.GetGamma());
    out[2] = Convert::From_RGB_To_Lab(tmp, r,
				      apple.GetM_Adapted(r, ada),
				      apple.GetGamma());
    out[3] = Convert::From_RGB_To_LCHab(tmp, r,
					apple.GetM_Adapted(r, ada),
					apple.GetGamma());
    out[4] = Convert::From_RGB_To_Luv(tmp, r,
				      apple.GetM_Adapted(r, ada),
				      apple.GetGamma());
    out[5] = Convert::From_RGB_To_LCHuv(tmp, r,
					apple.GetM_Adapted(r, ada),
					apple.GetGamma());
    out[6] = Convert::From_RGB_To_RGB(tmp, r, adobe.GetM_1_Adapted(r, ada),
				      adobe.GetGamma(),
				      apple.GetM_Adapted(r, ada),
				      apple.GetGamma());
    out[7] = tmp;
    out[8] = Convert::From_RGB_To_RGB(tmp, r, best.GetM_1_Adapted(r, ada),
				      best.GetGamma(),
				      apple.GetM_Adapted(r, ada),
				      apple.GetGamma());
    out[9] = Convert::From_RGB_To_RGB(tmp, r, beta.GetM_1_Adapted(r, ada),
				      beta.GetGamma(),
				      apple.GetM_Adapted(r, ada),
				      apple.GetGamma());
    out[10] = Convert::From_RGB_To_RGB(tmp, r,
				       bruce.GetM_1_Adapted(r, ada),
				       bruce.GetGamma(),
				       apple.GetM_Adapted(r, ada),
				       apple.GetGamma());
    out[11] = Convert::From_RGB_To_RGB(tmp, r, cie.GetM_1_Adapted(r, ada),
				       cie.GetGamma(),
				       apple.GetM_Adapted(r, ada),
				       apple.GetGamma());
    out[12] = Convert::From_RGB_To_RGB(tmp, r,
				       colormatch.GetM_1_Adapted(r, ada),
				       colormatch.GetGamma(),
				       apple.GetM_Adapted(r, ada),
				       apple.GetGamma());
    out[13] = Convert::From_RGB_To_RGB(tmp, r, don4.GetM_1_Adapted(r, ada),
				       don4.GetGamma(),
				       apple.GetM_Adapted(r, ada),
				       apple.GetGamma());
    out[14] = Convert::From_RGB_To_RGB(tmp, r, eci.GetM_1_Adapted(r, ada),
				       eci.GetGamma(),
				       apple.GetM_Adapted(r, ada),
				       apple.GetGamma());
    out[15] = Convert::From_RGB_To_RGB(tmp, r,
				       ektaspaceps5.GetM_1_Adapted(r, ada),
				       ektaspaceps5.GetGamma(),
				       apple.GetM_Adapted(r, ada),
				       apple.GetGamma());
    out[16] = Convert::From_RGB_To_RGB(tmp, r, ntsc.GetM_1_Adapted(r, ada),
				       ntsc.GetGamma(),
				       apple.GetM_Adapted(r, ada),
				       apple.GetGamma());
    out[17] = Convert::From_RGB_To_RGB(tmp, r,
				       pal_secam.GetM_1_Adapted(r, ada),
				       pal_secam.GetGamma(),
				       apple.GetM_Adapted(r, ada),
				       apple.GetGamma());
    out[18] = Convert::From_RGB_To_RGB(tmp, r,
				       prophoto.GetM_1_Adapted(r, ada),
				       prophoto.GetGamma(),
				       apple.GetM_Adapted(r, ada),
				       apple.GetGamma());
    out[19] = Convert::From_RGB_To_RGB(tmp, r,
				       smpte_c.GetM_1_Adapted(r, ada),
				       smpte_c.GetGamma(),
				       apple.GetM_Adapted(r, ada),
				       apple.GetGamma());
    out[20] = Convert::From_RGB_To_RGB(tmp, r, srgb.GetM_1_Adapted(r, ada),
				       srgb.GetGamma(),
				       apple.GetM_Adapted(r, ada),
				       apple.GetGamma());
    out[21] = Convert::From_RGB_To_RGB(tmp, r,
				       widegamut.GetM_1_Adapted(r, ada),
				       widegamut.GetGamma(),
				       apple.GetM_Adapted(r, ada),
				       apple.GetGamma());
    break;

  case Manager::CSBestRGB:
    tmp <<
      inputLines_[8].get<1>()->text().toFloat(),
      inputLines_[8].get<2>()->text().toFloat(),
      inputLines_[8].get<3>()->text().toFloat();

    out[0] = Convert::From_RGB_To_XYZ(tmp, r,
				      best.GetM_Adapted(r, ada),
				      best.GetGamma());
    out[1] = Convert::From_RGB_To_xyY(tmp, r,
				      best.GetM_Adapted(r, ada),
				      best.GetGamma());
    out[2] = Convert::From_RGB_To_Lab(tmp, r,
				      best.GetM_Adapted(r, ada),
				      best.GetGamma());
    out[3] = Convert::From_RGB_To_LCHab(tmp, r,
					best.GetM_Adapted(r, ada),
					best.GetGamma());
    out[4] = Convert::From_RGB_To_Luv(tmp, r,
				      best.GetM_Adapted(r, ada),
				      best.GetGamma());
    out[5] = Convert::From_RGB_To_LCHuv(tmp, r,
					best.GetM_Adapted(r, ada),
					best.GetGamma());
    out[6] = Convert::From_RGB_To_RGB(tmp, r, adobe.GetM_1_Adapted(r, ada),
				      adobe.GetGamma(),
				      best.GetM_Adapted(r, ada),
				      best.GetGamma());
    out[7] = Convert::From_RGB_To_RGB(tmp, r, apple.GetM_1_Adapted(r, ada),
				      apple.GetGamma(),
				      best.GetM_Adapted(r, ada),
				      best.GetGamma()); 
    out[8] = tmp;
    out[9] = Convert::From_RGB_To_RGB(tmp, r, beta.GetM_1_Adapted(r, ada),
				      beta.GetGamma(),
				      best.GetM_Adapted(r, ada),
				      best.GetGamma());
    out[10] = Convert::From_RGB_To_RGB(tmp, r,
				       bruce.GetM_1_Adapted(r, ada),
				       bruce.GetGamma(),
				       best.GetM_Adapted(r, ada),
				       best.GetGamma());
    out[11] = Convert::From_RGB_To_RGB(tmp, r, cie.GetM_1_Adapted(r, ada),
				       cie.GetGamma(),
				       best.GetM_Adapted(r, ada),
				       best.GetGamma());
    out[12] = Convert::From_RGB_To_RGB(tmp, r,
				       colormatch.GetM_1_Adapted(r, ada),
				       colormatch.GetGamma(),
				       best.GetM_Adapted(r, ada),
				       best.GetGamma());
    out[13] = Convert::From_RGB_To_RGB(tmp, r, don4.GetM_1_Adapted(r, ada),
				       don4.GetGamma(),
				       best.GetM_Adapted(r, ada),
				       best.GetGamma());
    out[14] = Convert::From_RGB_To_RGB(tmp, r, eci.GetM_1_Adapted(r, ada),
				       eci.GetGamma(),
				       best.GetM_Adapted(r, ada),
				       best.GetGamma());
    out[15] = Convert::From_RGB_To_RGB(tmp, r,
				       ektaspaceps5.GetM_1_Adapted(r, ada),
				       ektaspaceps5.GetGamma(),
				       best.GetM_Adapted(r, ada),
				       best.GetGamma());
    out[16] = Convert::From_RGB_To_RGB(tmp, r, ntsc.GetM_1_Adapted(r, ada),
				       ntsc.GetGamma(),
				       best.GetM_Adapted(r, ada),
				       best.GetGamma());
    out[17] = Convert::From_RGB_To_RGB(tmp, r,
				       pal_secam.GetM_1_Adapted(r, ada),
				       pal_secam.GetGamma(),
				       best.GetM_Adapted(r, ada),
				       best.GetGamma());
    out[18] = Convert::From_RGB_To_RGB(tmp, r,
				       prophoto.GetM_1_Adapted(r, ada),
				       prophoto.GetGamma(),
				       best.GetM_Adapted(r, ada),
				       best.GetGamma());
    out[19] = Convert::From_RGB_To_RGB(tmp, r,
				       smpte_c.GetM_1_Adapted(r, ada),
				       smpte_c.GetGamma(),
				       best.GetM_Adapted(r, ada),
				       best.GetGamma());
    out[20] = Convert::From_RGB_To_RGB(tmp, r, srgb.GetM_1_Adapted(r, ada),
				       srgb.GetGamma(),
				       best.GetM_Adapted(r, ada),
				       best.GetGamma());
    out[21] = Convert::From_RGB_To_RGB(tmp, r,
				       widegamut.GetM_1_Adapted(r, ada),
				       widegamut.GetGamma(),
				       best.GetM_Adapted(r, ada),
				       best.GetGamma());
    break;

  case Manager::CSBetaRGB:
    tmp <<
      inputLines_[9].get<1>()->text().toFloat(),
      inputLines_[9].get<2>()->text().toFloat(),
      inputLines_[9].get<3>()->text().toFloat();

    out[0] = Convert::From_RGB_To_XYZ(tmp, r,
				      beta.GetM_Adapted(r, ada),
				      beta.GetGamma());
    out[1] = Convert::From_RGB_To_xyY(tmp, r,
				      beta.GetM_Adapted(r, ada),
				      beta.GetGamma());
    out[2] = Convert::From_RGB_To_Lab(tmp, r,
				      beta.GetM_Adapted(r, ada),
				      beta.GetGamma());
    out[3] = Convert::From_RGB_To_LCHab(tmp, r,
					beta.GetM_Adapted(r, ada),
					beta.GetGamma());
    out[4] = Convert::From_RGB_To_Luv(tmp, r,
				      beta.GetM_Adapted(r, ada),
				      beta.GetGamma());
    out[5] = Convert::From_RGB_To_LCHuv(tmp, r,
					beta.GetM_Adapted(r, ada),
					beta.GetGamma());
    out[6] = Convert::From_RGB_To_RGB(tmp, r, adobe.GetM_1_Adapted(r, ada),
				      adobe.GetGamma(),
				      beta.GetM_Adapted(r, ada),
				      beta.GetGamma());
    out[7] = Convert::From_RGB_To_RGB(tmp, r, apple.GetM_1_Adapted(r, ada),
				      apple.GetGamma(),
				      beta.GetM_Adapted(r, ada),
				      beta.GetGamma()); 
    out[8] = Convert::From_RGB_To_RGB(tmp, r, best.GetM_1_Adapted(r, ada),
				      best.GetGamma(),
				      beta.GetM_Adapted(r, ada),
				      beta.GetGamma());  
    out[9] = tmp;
    out[10] = Convert::From_RGB_To_RGB(tmp, r,
				       bruce.GetM_1_Adapted(r, ada),
				       bruce.GetGamma(),
				       beta.GetM_Adapted(r, ada),
				       beta.GetGamma());
    out[11] = Convert::From_RGB_To_RGB(tmp, r, cie.GetM_1_Adapted(r, ada),
				       cie.GetGamma(),
				       beta.GetM_Adapted(r, ada),
				       beta.GetGamma());
    out[12] = Convert::From_RGB_To_RGB(tmp, r,
				       colormatch.GetM_1_Adapted(r, ada),
				       colormatch.GetGamma(),
				       beta.GetM_Adapted(r, ada),
				       beta.GetGamma());
    out[13] = Convert::From_RGB_To_RGB(tmp, r, don4.GetM_1_Adapted(r, ada),
				       don4.GetGamma(),
				       beta.GetM_Adapted(r, ada),
				       beta.GetGamma());
    out[14] = Convert::From_RGB_To_RGB(tmp, r, eci.GetM_1_Adapted(r, ada),
				       eci.GetGamma(),
				       beta.GetM_Adapted(r, ada),
				       beta.GetGamma());
    out[15] = Convert::From_RGB_To_RGB(tmp, r,
				       ektaspaceps5.GetM_1_Adapted(r, ada),
				       ektaspaceps5.GetGamma(),
				       beta.GetM_Adapted(r, ada),
				       beta.GetGamma());
    out[16] = Convert::From_RGB_To_RGB(tmp, r, ntsc.GetM_1_Adapted(r, ada),
				       ntsc.GetGamma(),
				       beta.GetM_Adapted(r, ada),
				       beta.GetGamma());
    out[17] = Convert::From_RGB_To_RGB(tmp, r,
				       pal_secam.GetM_1_Adapted(r, ada),
				       pal_secam.GetGamma(),
				       beta.GetM_Adapted(r, ada),
				       beta.GetGamma());
    out[18] = Convert::From_RGB_To_RGB(tmp, r,
				       prophoto.GetM_1_Adapted(r, ada),
				       prophoto.GetGamma(),
				       beta.GetM_Adapted(r, ada),
				       beta.GetGamma());
    out[19] = Convert::From_RGB_To_RGB(tmp, r,
				       smpte_c.GetM_1_Adapted(r, ada),
				       smpte_c.GetGamma(),
				       beta.GetM_Adapted(r, ada),
				       beta.GetGamma());
    out[20] = Convert::From_RGB_To_RGB(tmp, r, srgb.GetM_1_Adapted(r, ada),
				       srgb.GetGamma(),
				       beta.GetM_Adapted(r, ada),
				       beta.GetGamma());
    out[21] = Convert::From_RGB_To_RGB(tmp, r,
				       widegamut.GetM_1_Adapted(r, ada),
				       widegamut.GetGamma(),
				       beta.GetM_Adapted(r, ada),
				       beta.GetGamma());
    break;

  case Manager::CSBruceRGB:
    tmp <<
      inputLines_[10].get<1>()->text().toFloat(),
      inputLines_[10].get<2>()->text().toFloat(),
      inputLines_[10].get<3>()->text().toFloat();

    out[0] = Convert::From_RGB_To_XYZ(tmp, r,
				      bruce.GetM_Adapted(r, ada),
				      bruce.GetGamma());
    out[1] = Convert::From_RGB_To_xyY(tmp, r,
				      bruce.GetM_Adapted(r, ada),
				      bruce.GetGamma());
    out[2] = Convert::From_RGB_To_Lab(tmp, r,
				      bruce.GetM_Adapted(r, ada),
				      bruce.GetGamma());
    out[3] = Convert::From_RGB_To_LCHab(tmp, r,
					bruce.GetM_Adapted(r, ada),
					bruce.GetGamma());
    out[4] = Convert::From_RGB_To_Luv(tmp, r,
				      bruce.GetM_Adapted(r, ada),
				      bruce.GetGamma());
    out[5] = Convert::From_RGB_To_LCHuv(tmp, r,
					bruce.GetM_Adapted(r, ada),
					bruce.GetGamma());
    out[6] = Convert::From_RGB_To_RGB(tmp, r, adobe.GetM_1_Adapted(r, ada),
				      adobe.GetGamma(),
				      bruce.GetM_Adapted(r, ada),
				      bruce.GetGamma());
    out[7] = Convert::From_RGB_To_RGB(tmp, r, apple.GetM_1_Adapted(r, ada),
				      apple.GetGamma(),
				      bruce.GetM_Adapted(r, ada),
				      bruce.GetGamma()); 
    out[8] = Convert::From_RGB_To_RGB(tmp, r, best.GetM_1_Adapted(r, ada),
				      best.GetGamma(),
				      bruce.GetM_Adapted(r, ada),
				      bruce.GetGamma());  
    out[9] = Convert::From_RGB_To_RGB(tmp, r, beta.GetM_1_Adapted(r, ada),
				      beta.GetGamma(),
				      bruce.GetM_Adapted(r, ada),
				      bruce.GetGamma());   
    out[10] = tmp;
    out[11] = Convert::From_RGB_To_RGB(tmp, r, cie.GetM_1_Adapted(r, ada),
				       cie.GetGamma(),
				       bruce.GetM_Adapted(r, ada),
				       bruce.GetGamma());
    out[12] = Convert::From_RGB_To_RGB(tmp, r,
				       colormatch.GetM_1_Adapted(r, ada),
				       colormatch.GetGamma(),
				       bruce.GetM_Adapted(r, ada),
				       bruce.GetGamma());
    out[13] = Convert::From_RGB_To_RGB(tmp, r, don4.GetM_1_Adapted(r, ada),
				       don4.GetGamma(),
				       bruce.GetM_Adapted(r, ada),
				       bruce.GetGamma());
    out[14] = Convert::From_RGB_To_RGB(tmp, r, eci.GetM_1_Adapted(r, ada),
				       eci.GetGamma(),
				       bruce.GetM_Adapted(r, ada),
				       bruce.GetGamma());
    out[15] = Convert::From_RGB_To_RGB(tmp, r,
				       ektaspaceps5.GetM_1_Adapted(r, ada),
				       ektaspaceps5.GetGamma(),
				       bruce.GetM_Adapted(r, ada),
				       bruce.GetGamma());
    out[16] = Convert::From_RGB_To_RGB(tmp, r, ntsc.GetM_1_Adapted(r, ada),
				       ntsc.GetGamma(),
				       bruce.GetM_Adapted(r, ada),
				       bruce.GetGamma());
    out[17] = Convert::From_RGB_To_RGB(tmp, r,
				       pal_secam.GetM_1_Adapted(r, ada),
				       pal_secam.GetGamma(),
				       bruce.GetM_Adapted(r, ada),
				       bruce.GetGamma());
    out[18] = Convert::From_RGB_To_RGB(tmp, r,
				       prophoto.GetM_1_Adapted(r, ada),
				       prophoto.GetGamma(),
				       bruce.GetM_Adapted(r, ada),
				       bruce.GetGamma());
    out[19] = Convert::From_RGB_To_RGB(tmp, r,
				       smpte_c.GetM_1_Adapted(r, ada),
				       smpte_c.GetGamma(),
				       bruce.GetM_Adapted(r, ada),
				       bruce.GetGamma());
    out[20] = Convert::From_RGB_To_RGB(tmp, r, srgb.GetM_1_Adapted(r, ada),
				       srgb.GetGamma(),
				       bruce.GetM_Adapted(r, ada),
				       bruce.GetGamma());
    out[21] = Convert::From_RGB_To_RGB(tmp, r,
				       widegamut.GetM_1_Adapted(r, ada),
				       widegamut.GetGamma(),
				       bruce.GetM_Adapted(r, ada),
				       bruce.GetGamma());
    break;

  case Manager::CSCIERGB:
    tmp <<
      inputLines_[11].get<1>()->text().toFloat(),
      inputLines_[11].get<2>()->text().toFloat(),
      inputLines_[11].get<3>()->text().toFloat();

    out[0] = Convert::From_RGB_To_XYZ(tmp, r,
				      cie.GetM_Adapted(r, ada),
				      cie.GetGamma());
    out[1] = Convert::From_RGB_To_xyY(tmp, r,
				      cie.GetM_Adapted(r, ada),
				      cie.GetGamma());
    out[2] = Convert::From_RGB_To_Lab(tmp, r,
				      cie.GetM_Adapted(r, ada),
				      cie.GetGamma());
    out[3] = Convert::From_RGB_To_LCHab(tmp, r,
					cie.GetM_Adapted(r, ada),
					cie.GetGamma());
    out[4] = Convert::From_RGB_To_Luv(tmp, r,
				      cie.GetM_Adapted(r, ada),
				      cie.GetGamma());
    out[5] = Convert::From_RGB_To_LCHuv(tmp, r,
					cie.GetM_Adapted(r, ada),
					cie.GetGamma());
    out[6] = Convert::From_RGB_To_RGB(tmp, r, adobe.GetM_1_Adapted(r, ada),
				      adobe.GetGamma(),
				      cie.GetM_Adapted(r, ada),
				      cie.GetGamma());
    out[7] = Convert::From_RGB_To_RGB(tmp, r, apple.GetM_1_Adapted(r, ada),
				      apple.GetGamma(),
				      cie.GetM_Adapted(r, ada),
				      cie.GetGamma()); 
    out[8] = Convert::From_RGB_To_RGB(tmp, r, best.GetM_1_Adapted(r, ada),
				      best.GetGamma(),
				      cie.GetM_Adapted(r, ada),
				      cie.GetGamma());  
    out[9] = Convert::From_RGB_To_RGB(tmp, r, beta.GetM_1_Adapted(r, ada),
				      beta.GetGamma(),
				      cie.GetM_Adapted(r, ada),
				      cie.GetGamma());   
    out[10] = Convert::From_RGB_To_RGB(tmp, r,
				       bruce.GetM_1_Adapted(r, ada),
				       bruce.GetGamma(),
				       cie.GetM_Adapted(r, ada),
				       cie.GetGamma());   
    out[11] = tmp;
    out[12] = Convert::From_RGB_To_RGB(tmp, r,
				       colormatch.GetM_1_Adapted(r, ada),
				       colormatch.GetGamma(),
				       cie.GetM_Adapted(r, ada),
				       cie.GetGamma());
    out[13] = Convert::From_RGB_To_RGB(tmp, r, don4.GetM_1_Adapted(r, ada),
				       don4.GetGamma(),
				       cie.GetM_Adapted(r, ada),
				       cie.GetGamma());
    out[14] = Convert::From_RGB_To_RGB(tmp, r, eci.GetM_1_Adapted(r, ada),
				       eci.GetGamma(),
				       cie.GetM_Adapted(r, ada),
				       cie.GetGamma());
    out[15] = Convert::From_RGB_To_RGB(tmp, r,
				       ektaspaceps5.GetM_1_Adapted(r, ada),
				       ektaspaceps5.GetGamma(),
				       cie.GetM_Adapted(r, ada),
				       cie.GetGamma());
    out[16] = Convert::From_RGB_To_RGB(tmp, r, ntsc.GetM_1_Adapted(r, ada),
				       ntsc.GetGamma(),
				       cie.GetM_Adapted(r, ada),
				       cie.GetGamma());
    out[17] = Convert::From_RGB_To_RGB(tmp, r,
				       pal_secam.GetM_1_Adapted(r, ada),
				       pal_secam.GetGamma(),
				       cie.GetM_Adapted(r, ada),
				       cie.GetGamma());
    out[18] = Convert::From_RGB_To_RGB(tmp, r,
				       prophoto.GetM_1_Adapted(r, ada),
				       prophoto.GetGamma(),
				       cie.GetM_Adapted(r, ada),
				       cie.GetGamma());
    out[19] = Convert::From_RGB_To_RGB(tmp, r,
				       smpte_c.GetM_1_Adapted(r, ada),
				       smpte_c.GetGamma(),
				       cie.GetM_Adapted(r, ada),
				       cie.GetGamma());
    out[20] = Convert::From_RGB_To_RGB(tmp, r, srgb.GetM_1_Adapted(r, ada),
				       srgb.GetGamma(),
				       cie.GetM_Adapted(r, ada),
				       cie.GetGamma());
    out[21] = Convert::From_RGB_To_RGB(tmp, r,
				       widegamut.GetM_1_Adapted(r, ada),
				       widegamut.GetGamma(),
				       cie.GetM_Adapted(r, ada),
				       cie.GetGamma());
    break;

  case Manager::CSColorMatchRGB:
    tmp <<
      inputLines_[12].get<1>()->text().toFloat(),
      inputLines_[12].get<2>()->text().toFloat(),
      inputLines_[12].get<3>()->text().toFloat();

    out[0] = Convert::From_RGB_To_XYZ(tmp, r,
				      colormatch.GetM_Adapted(r, ada),
				      colormatch.GetGamma());
    out[1] = Convert::From_RGB_To_xyY(tmp, r,
				      colormatch.GetM_Adapted(r, ada),
				      colormatch.GetGamma());
    out[2] = Convert::From_RGB_To_Lab(tmp, r,
				      colormatch.GetM_Adapted(r, ada),
				      colormatch.GetGamma());
    out[3] = Convert::From_RGB_To_LCHab(tmp, r,
					colormatch.GetM_Adapted(r, ada),
					colormatch.GetGamma());
    out[4] = Convert::From_RGB_To_Luv(tmp, r,
				      colormatch.GetM_Adapted(r, ada),
				      colormatch.GetGamma());
    out[5] = Convert::From_RGB_To_LCHuv(tmp, r,
					colormatch.GetM_Adapted(r, ada),
					colormatch.GetGamma());
    out[6] = Convert::From_RGB_To_RGB(tmp, r, adobe.GetM_1_Adapted(r, ada),
				      adobe.GetGamma(),
				      colormatch.GetM_Adapted(r, ada),
				      colormatch.GetGamma());
    out[7] = Convert::From_RGB_To_RGB(tmp, r, apple.GetM_1_Adapted(r, ada),
				      apple.GetGamma(),
				      colormatch.GetM_Adapted(r, ada),
				      colormatch.GetGamma()); 
    out[8] = Convert::From_RGB_To_RGB(tmp, r, best.GetM_1_Adapted(r, ada),
				      best.GetGamma(),
				      colormatch.GetM_Adapted(r, ada),
				      colormatch.GetGamma());  
    out[9] = Convert::From_RGB_To_RGB(tmp, r, beta.GetM_1_Adapted(r, ada),
				      beta.GetGamma(),
				      colormatch.GetM_Adapted(r, ada),
				      colormatch.GetGamma());   
    out[10] = Convert::From_RGB_To_RGB(tmp, r,
				       bruce.GetM_1_Adapted(r, ada),
				       bruce.GetGamma(),
				       colormatch.GetM_Adapted(r, ada),
				       colormatch.GetGamma());   
    out[11] = Convert::From_RGB_To_RGB(tmp, r, cie.GetM_1_Adapted(r, ada),
				       cie.GetGamma(),
				       colormatch.GetM_Adapted(r, ada),
				       colormatch.GetGamma());   
    out[12] = tmp;
    out[13] = Convert::From_RGB_To_RGB(tmp, r, don4.GetM_1_Adapted(r, ada),
				       don4.GetGamma(),
				       colormatch.GetM_Adapted(r, ada),
				       colormatch.GetGamma());
    out[14] = Convert::From_RGB_To_RGB(tmp, r, eci.GetM_1_Adapted(r, ada),
				       eci.GetGamma(),
				       colormatch.GetM_Adapted(r, ada),
				       colormatch.GetGamma());
    out[15] = Convert::From_RGB_To_RGB(tmp, r,
				       ektaspaceps5.GetM_1_Adapted(r, ada),
				       ektaspaceps5.GetGamma(),
				       colormatch.GetM_Adapted(r, ada),
				       colormatch.GetGamma());
    out[16] = Convert::From_RGB_To_RGB(tmp, r, ntsc.GetM_1_Adapted(r, ada),
				       ntsc.GetGamma(),
				       colormatch.GetM_Adapted(r, ada),
				       colormatch.GetGamma());
    out[17] = Convert::From_RGB_To_RGB(tmp, r,
				       pal_secam.GetM_1_Adapted(r, ada),
				       pal_secam.GetGamma(),
				       colormatch.GetM_Adapted(r, ada),
				       colormatch.GetGamma());
    out[18] = Convert::From_RGB_To_RGB(tmp, r,
				       prophoto.GetM_1_Adapted(r, ada),
				       prophoto.GetGamma(),
				       colormatch.GetM_Adapted(r, ada),
				       colormatch.GetGamma());
    out[19] = Convert::From_RGB_To_RGB(tmp, r,
				       smpte_c.GetM_1_Adapted(r, ada),
				       smpte_c.GetGamma(),
				       colormatch.GetM_Adapted(r, ada),
				       colormatch.GetGamma());
    out[20] = Convert::From_RGB_To_RGB(tmp, r, srgb.GetM_1_Adapted(r, ada),
				       srgb.GetGamma(),
				       colormatch.GetM_Adapted(r, ada),
				       colormatch.GetGamma());
    out[21] = Convert::From_RGB_To_RGB(tmp, r,
				       widegamut.GetM_1_Adapted(r, ada),
				       widegamut.GetGamma(),
				       colormatch.GetM_Adapted(r, ada),
				       colormatch.GetGamma());
    break;

  case Manager::CSDonRGB4:
    tmp <<
      inputLines_[13].get<1>()->text().toFloat(),
      inputLines_[13].get<2>()->text().toFloat(),
      inputLines_[13].get<3>()->text().toFloat();

    out[0] = Convert::From_RGB_To_XYZ(tmp, r, don4.GetM_Adapted(r, ada),
				      don4.GetGamma());
    out[1] = Convert::From_RGB_To_xyY(tmp, r, don4.GetM_Adapted(r, ada),
				      don4.GetGamma());
    out[2] = Convert::From_RGB_To_Lab(tmp, r, don4.GetM_Adapted(r, ada),
				      don4.GetGamma());
    out[3] = Convert::From_RGB_To_LCHab(tmp, r, don4.GetM_Adapted(r, ada),
					don4.GetGamma());
    out[4] = Convert::From_RGB_To_Luv(tmp, r, don4.GetM_Adapted(r, ada),
				      don4.GetGamma());
    out[5] = Convert::From_RGB_To_LCHuv(tmp, r, don4.GetM_Adapted(r, ada),
					don4.GetGamma());
    out[6] = Convert::From_RGB_To_RGB(tmp, r, adobe.GetM_1_Adapted(r, ada),
				      adobe.GetGamma(),
				      don4.GetM_Adapted(r, ada),
				      don4.GetGamma());
    out[7] = Convert::From_RGB_To_RGB(tmp, r, apple.GetM_1_Adapted(r, ada),
				      apple.GetGamma(),
				      don4.GetM_Adapted(r, ada),
				      don4.GetGamma()); 
    out[8] = Convert::From_RGB_To_RGB(tmp, r, best.GetM_1_Adapted(r, ada),
				      best.GetGamma(),
				      don4.GetM_Adapted(r, ada),
				      don4.GetGamma());  
    out[9] = Convert::From_RGB_To_RGB(tmp, r, beta.GetM_1_Adapted(r, ada),
				      beta.GetGamma(),
				      don4.GetM_Adapted(r, ada),
				      don4.GetGamma());   
    out[10] = Convert::From_RGB_To_RGB(tmp, r,
				       bruce.GetM_1_Adapted(r, ada),
				       bruce.GetGamma(),
				       don4.GetM_Adapted(r, ada),
				       don4.GetGamma());   
    out[11] = Convert::From_RGB_To_RGB(tmp, r, cie.GetM_1_Adapted(r, ada),
				       cie.GetGamma(),
				       don4.GetM_Adapted(r, ada),
				       don4.GetGamma());   
    out[12] = Convert::From_RGB_To_RGB(tmp, r,
				       colormatch.GetM_1_Adapted(r, ada),
				       colormatch.GetGamma(),
				       don4.GetM_Adapted(r, ada),
				       don4.GetGamma());
    out[13] = tmp;
    out[14] = Convert::From_RGB_To_RGB(tmp, r, eci.GetM_1_Adapted(r, ada),
				       eci.GetGamma(),
				       don4.GetM_Adapted(r, ada),
				       don4.GetGamma());
    out[15] = Convert::From_RGB_To_RGB(tmp, r,
				       ektaspaceps5.GetM_1_Adapted(r, ada),
				       ektaspaceps5.GetGamma(),
				       don4.GetM_Adapted(r, ada),
				       don4.GetGamma());
    out[16] = Convert::From_RGB_To_RGB(tmp, r, ntsc.GetM_1_Adapted(r, ada),
				       ntsc.GetGamma(),
				       don4.GetM_Adapted(r, ada),
				       don4.GetGamma());
    out[17] = Convert::From_RGB_To_RGB(tmp, r,
				       pal_secam.GetM_1_Adapted(r, ada),
				       pal_secam.GetGamma(),
				       don4.GetM_Adapted(r, ada),
				       don4.GetGamma());
    out[18] = Convert::From_RGB_To_RGB(tmp, r,
				       prophoto.GetM_1_Adapted(r, ada),
				       prophoto.GetGamma(),
				       don4.GetM_Adapted(r, ada),
				       don4.GetGamma());
    out[19] = Convert::From_RGB_To_RGB(tmp, r,
				       smpte_c.GetM_1_Adapted(r, ada),
				       smpte_c.GetGamma(),
				       don4.GetM_Adapted(r, ada),
				       don4.GetGamma());
    out[20] = Convert::From_RGB_To_RGB(tmp, r, srgb.GetM_1_Adapted(r, ada),
				       srgb.GetGamma(),
				       don4.GetM_Adapted(r, ada),
				       don4.GetGamma());
    out[21] = Convert::From_RGB_To_RGB(tmp, r,
				       widegamut.GetM_1_Adapted(r, ada),
				       widegamut.GetGamma(),
				       don4.GetM_Adapted(r, ada),
				       don4.GetGamma());
    break;

  case Manager::CSECIRGB:
    tmp <<
      inputLines_[14].get<1>()->text().toFloat(),
      inputLines_[14].get<2>()->text().toFloat(),
      inputLines_[14].get<3>()->text().toFloat();

    out[0] = Convert::From_RGB_To_XYZ(tmp, r, eci.GetM_Adapted(r, ada),
				      eci.GetGamma());
    out[1] = Convert::From_RGB_To_xyY(tmp, r, eci.GetM_Adapted(r, ada),
				      eci.GetGamma());
    out[2] = Convert::From_RGB_To_Lab(tmp, r, eci.GetM_Adapted(r, ada),
				      eci.GetGamma());
    out[3] = Convert::From_RGB_To_LCHab(tmp, r, eci.GetM_Adapted(r, ada),
					eci.GetGamma());
    out[4] = Convert::From_RGB_To_Luv(tmp, r, eci.GetM_Adapted(r, ada),
				      eci.GetGamma());
    out[5] = Convert::From_RGB_To_LCHuv(tmp, r, eci.GetM_Adapted(r, ada),
					eci.GetGamma());
    out[6] = Convert::From_RGB_To_RGB(tmp, r, adobe.GetM_1_Adapted(r, ada),
				      adobe.GetGamma(),
				      eci.GetM_Adapted(r, ada),
				      eci.GetGamma());
    out[7] = Convert::From_RGB_To_RGB(tmp, r, apple.GetM_1_Adapted(r, ada),
				      apple.GetGamma(),
				      eci.GetM_Adapted(r, ada),
				      eci.GetGamma()); 
    out[8] = Convert::From_RGB_To_RGB(tmp, r, best.GetM_1_Adapted(r, ada),
				      best.GetGamma(),
				      eci.GetM_Adapted(r, ada),
				      eci.GetGamma());  
    out[9] = Convert::From_RGB_To_RGB(tmp, r, beta.GetM_1_Adapted(r, ada),
				      beta.GetGamma(),
				      eci.GetM_Adapted(r, ada),
				      eci.GetGamma());   
    out[10] = Convert::From_RGB_To_RGB(tmp, r,
				       bruce.GetM_1_Adapted(r, ada),
				       bruce.GetGamma(),
				       eci.GetM_Adapted(r, ada),
				       eci.GetGamma());   
    out[11] = Convert::From_RGB_To_RGB(tmp, r, cie.GetM_1_Adapted(r, ada),
				       cie.GetGamma(),
				       eci.GetM_Adapted(r, ada),
				       eci.GetGamma());   
    out[12] = Convert::From_RGB_To_RGB(tmp, r,
				       colormatch.GetM_1_Adapted(r, ada),
				       colormatch.GetGamma(),
				       eci.GetM_Adapted(r, ada),
				       eci.GetGamma());
    out[13] = Convert::From_RGB_To_RGB(tmp, r, don4.GetM_1_Adapted(r, ada),
				       don4.GetGamma(),
				       eci.GetM_Adapted(r, ada),
				       eci.GetGamma());
    out[14] = tmp;
    out[15] = Convert::From_RGB_To_RGB(tmp, r,
				       ektaspaceps5.GetM_1_Adapted(r, ada),
				       ektaspaceps5.GetGamma(),
				       eci.GetM_Adapted(r, ada),
				       eci.GetGamma());
    out[16] = Convert::From_RGB_To_RGB(tmp, r, ntsc.GetM_1_Adapted(r, ada),
				       ntsc.GetGamma(),
				       eci.GetM_Adapted(r, ada),
				       eci.GetGamma());
    out[17] = Convert::From_RGB_To_RGB(tmp, r,
				       pal_secam.GetM_1_Adapted(r, ada),
				       pal_secam.GetGamma(),
				       eci.GetM_Adapted(r, ada),
				       eci.GetGamma());
    out[18] = Convert::From_RGB_To_RGB(tmp, r,
				       prophoto.GetM_1_Adapted(r, ada),
				       prophoto.GetGamma(),
				       eci.GetM_Adapted(r, ada),
				       eci.GetGamma());
    out[19] = Convert::From_RGB_To_RGB(tmp, r,
				       smpte_c.GetM_1_Adapted(r, ada),
				       smpte_c.GetGamma(),
				       eci.GetM_Adapted(r, ada),
				       eci.GetGamma());
    out[20] = Convert::From_RGB_To_RGB(tmp, r, srgb.GetM_1_Adapted(r, ada),
				       srgb.GetGamma(),
				       eci.GetM_Adapted(r, ada),
				       eci.GetGamma());
    out[21] = Convert::From_RGB_To_RGB(tmp, r,
				       widegamut.GetM_1_Adapted(r, ada),
				       widegamut.GetGamma(),
				       eci.GetM_Adapted(r, ada),
				       eci.GetGamma());
    break;

  case Manager::CSEktaSpacePS5:
    tmp <<
      inputLines_[15].get<1>()->text().toFloat(),
      inputLines_[15].get<2>()->text().toFloat(),
      inputLines_[15].get<3>()->text().toFloat();

    out[0] = Convert::From_RGB_To_XYZ(tmp, r,
				      ektaspaceps5.GetM_Adapted(r, ada),
				      ektaspaceps5.GetGamma());
    out[1] = Convert::From_RGB_To_xyY(tmp, r,
				      ektaspaceps5.GetM_Adapted(r, ada),
				      ektaspaceps5.GetGamma());
    out[2] = Convert::From_RGB_To_Lab(tmp, r,
				      ektaspaceps5.GetM_Adapted(r, ada),
				      ektaspaceps5.GetGamma());
    out[3] = Convert::From_RGB_To_LCHab(tmp, r,
					ektaspaceps5.GetM_Adapted(r, ada),
					ektaspaceps5.GetGamma());
    out[4] = Convert::From_RGB_To_Luv(tmp, r,
				      ektaspaceps5.GetM_Adapted(r, ada),
				      ektaspaceps5.GetGamma());
    out[5] = Convert::From_RGB_To_LCHuv(tmp, r,
					ektaspaceps5.GetM_Adapted(r, ada),
					ektaspaceps5.GetGamma());
    out[6] = Convert::From_RGB_To_RGB(tmp, r, adobe.GetM_1_Adapted(r, ada),
				      adobe.GetGamma(),
				      ektaspaceps5.GetM_Adapted(r, ada),
				      ektaspaceps5.GetGamma());
    out[7] = Convert::From_RGB_To_RGB(tmp, r, apple.GetM_1_Adapted(r, ada),
				      apple.GetGamma(),
				      ektaspaceps5.GetM_Adapted(r, ada),
				      ektaspaceps5.GetGamma()); 
    out[8] = Convert::From_RGB_To_RGB(tmp, r, best.GetM_1_Adapted(r, ada),
				      best.GetGamma(),
				      ektaspaceps5.GetM_Adapted(r, ada),
				      ektaspaceps5.GetGamma());  
    out[9] = Convert::From_RGB_To_RGB(tmp, r, beta.GetM_1_Adapted(r, ada),
				      beta.GetGamma(),
				      ektaspaceps5.GetM_Adapted(r, ada),
				      ektaspaceps5.GetGamma());   
    out[10] = Convert::From_RGB_To_RGB(tmp, r,
				       bruce.GetM_1_Adapted(r, ada),
				       bruce.GetGamma(),
				       ektaspaceps5.GetM_Adapted(r, ada),
				       ektaspaceps5.GetGamma());   
    out[11] = Convert::From_RGB_To_RGB(tmp, r, cie.GetM_1_Adapted(r, ada),
				       cie.GetGamma(),
				       ektaspaceps5.GetM_Adapted(r, ada),
				       ektaspaceps5.GetGamma());   
    out[12] = Convert::From_RGB_To_RGB(tmp, r,
				       colormatch.GetM_1_Adapted(r, ada),
				       colormatch.GetGamma(),
				       ektaspaceps5.GetM_Adapted(r, ada),
				       ektaspaceps5.GetGamma());
    out[13] = Convert::From_RGB_To_RGB(tmp, r, don4.GetM_1_Adapted(r, ada),
				       don4.GetGamma(),
				       ektaspaceps5.GetM_Adapted(r, ada),
				       ektaspaceps5.GetGamma());
    out[14] = Convert::From_RGB_To_RGB(tmp, r, eci.GetM_1_Adapted(r, ada),
				       eci.GetGamma(),
				       ektaspaceps5.GetM_Adapted(r, ada),
				       ektaspaceps5.GetGamma());
    out[15] = tmp;
    out[16] = Convert::From_RGB_To_RGB(tmp, r, ntsc.GetM_1_Adapted(r, ada),
				       ntsc.GetGamma(),
				       ektaspaceps5.GetM_Adapted(r, ada),
				       ektaspaceps5.GetGamma());
    out[17] = Convert::From_RGB_To_RGB(tmp, r,
				       pal_secam.GetM_1_Adapted(r, ada),
				       pal_secam.GetGamma(),
				       ektaspaceps5.GetM_Adapted(r, ada),
				       ektaspaceps5.GetGamma());
    out[18] = Convert::From_RGB_To_RGB(tmp, r,
				       prophoto.GetM_1_Adapted(r, ada),
				       prophoto.GetGamma(),
				       ektaspaceps5.GetM_Adapted(r, ada),
				       ektaspaceps5.GetGamma());
    out[19] = Convert::From_RGB_To_RGB(tmp, r,
				       smpte_c.GetM_1_Adapted(r, ada),
				       smpte_c.GetGamma(),
				       ektaspaceps5.GetM_Adapted(r, ada),
				       ektaspaceps5.GetGamma());
    out[20] = Convert::From_RGB_To_RGB(tmp, r, srgb.GetM_1_Adapted(r, ada),
				       srgb.GetGamma(),
				       ektaspaceps5.GetM_Adapted(r, ada),
				       ektaspaceps5.GetGamma());
    out[21] = Convert::From_RGB_To_RGB(tmp, r,
				       widegamut.GetM_1_Adapted(r, ada),
				       widegamut.GetGamma(),
				       ektaspaceps5.GetM_Adapted(r, ada),
				       ektaspaceps5.GetGamma());
    break;

  case Manager::CSNTSCRGB:
    tmp <<
      inputLines_[16].get<1>()->text().toFloat(),
      inputLines_[16].get<2>()->text().toFloat(),
      inputLines_[16].get<3>()->text().toFloat();

    out[0] = Convert::From_RGB_To_XYZ(tmp, r, ntsc.GetM_Adapted(r, ada),
				      ntsc.GetGamma());
    out[1] = Convert::From_RGB_To_xyY(tmp, r, ntsc.GetM_Adapted(r, ada),
				      ntsc.GetGamma());
    out[2] = Convert::From_RGB_To_Lab(tmp, r, ntsc.GetM_Adapted(r, ada),
				      ntsc.GetGamma());
    out[3] = Convert::From_RGB_To_LCHab(tmp, r, ntsc.GetM_Adapted(r, ada),
					ntsc.GetGamma());
    out[4] = Convert::From_RGB_To_Luv(tmp, r, ntsc.GetM_Adapted(r, ada),
				      ntsc.GetGamma());
    out[5] = Convert::From_RGB_To_LCHuv(tmp, r, ntsc.GetM_Adapted(r, ada),
					ntsc.GetGamma());
    out[6] = Convert::From_RGB_To_RGB(tmp, r, adobe.GetM_1_Adapted(r, ada),
				      adobe.GetGamma(),
				      ntsc.GetM_Adapted(r, ada),
				      ntsc.GetGamma());
    out[7] = Convert::From_RGB_To_RGB(tmp, r, apple.GetM_1_Adapted(r, ada),
				      apple.GetGamma(),
				      ntsc.GetM_Adapted(r, ada),
				      ntsc.GetGamma()); 
    out[8] = Convert::From_RGB_To_RGB(tmp, r, best.GetM_1_Adapted(r, ada),
				      best.GetGamma(),
				      ntsc.GetM_Adapted(r, ada),
				      ntsc.GetGamma());  
    out[9] = Convert::From_RGB_To_RGB(tmp, r, beta.GetM_1_Adapted(r, ada),
				      beta.GetGamma(),
				      ntsc.GetM_Adapted(r, ada),
				      ntsc.GetGamma());   
    out[10] = Convert::From_RGB_To_RGB(tmp, r,
				       bruce.GetM_1_Adapted(r, ada),
				       bruce.GetGamma(),
				       ntsc.GetM_Adapted(r, ada),
				       ntsc.GetGamma());   
    out[11] = Convert::From_RGB_To_RGB(tmp, r, cie.GetM_1_Adapted(r, ada),
				       cie.GetGamma(),
				       ntsc.GetM_Adapted(r, ada),
				       ntsc.GetGamma());   
    out[12] = Convert::From_RGB_To_RGB(tmp, r,
				       colormatch.GetM_1_Adapted(r, ada),
				       colormatch.GetGamma(),
				       ntsc.GetM_Adapted(r, ada),
				       ntsc.GetGamma());
    out[13] = Convert::From_RGB_To_RGB(tmp, r, don4.GetM_1_Adapted(r, ada),
				       don4.GetGamma(),
				       ntsc.GetM_Adapted(r, ada),
				       ntsc.GetGamma());
    out[14] = Convert::From_RGB_To_RGB(tmp, r, eci.GetM_1_Adapted(r, ada),
				       eci.GetGamma(),
				       ntsc.GetM_Adapted(r, ada),
				       ntsc.GetGamma());
    out[15] = Convert::From_RGB_To_RGB(tmp, r,
				       ektaspaceps5.GetM_1_Adapted(r, ada),
				       ektaspaceps5.GetGamma(),
				       ntsc.GetM_Adapted(r, ada),
				       ntsc.GetGamma());
    out[16] = tmp;
    out[17] = Convert::From_RGB_To_RGB(tmp, r,
				       pal_secam.GetM_1_Adapted(r, ada),
				       pal_secam.GetGamma(),
				       ntsc.GetM_Adapted(r, ada),
				       ntsc.GetGamma());
    out[18] = Convert::From_RGB_To_RGB(tmp, r,
				       prophoto.GetM_1_Adapted(r, ada),
				       prophoto.GetGamma(),
				       ntsc.GetM_Adapted(r, ada),
				       ntsc.GetGamma());
    out[19] = Convert::From_RGB_To_RGB(tmp, r,
				       smpte_c.GetM_1_Adapted(r, ada),
				       smpte_c.GetGamma(),
				       ntsc.GetM_Adapted(r, ada),
				       ntsc.GetGamma());
    out[20] = Convert::From_RGB_To_RGB(tmp, r, srgb.GetM_1_Adapted(r, ada),
				       srgb.GetGamma(),
				       ntsc.GetM_Adapted(r, ada),
				       ntsc.GetGamma());
    out[21] = Convert::From_RGB_To_RGB(tmp, r,
				       widegamut.GetM_1_Adapted(r, ada),
				       widegamut.GetGamma(),
				       ntsc.GetM_Adapted(r, ada),
				       ntsc.GetGamma());
    break;

  case Manager::CSPALSECAMRGB:
    tmp <<
      inputLines_[17].get<1>()->text().toFloat(),
      inputLines_[17].get<2>()->text().toFloat(),
      inputLines_[17].get<3>()->text().toFloat();

    out[0] = Convert::From_RGB_To_XYZ(tmp, r,
				      pal_secam.GetM_Adapted(r, ada),
				      pal_secam.GetGamma());
    out[1] = Convert::From_RGB_To_xyY(tmp, r,
				      pal_secam.GetM_Adapted(r, ada),
				      pal_secam.GetGamma());
    out[2] = Convert::From_RGB_To_Lab(tmp, r,
				      pal_secam.GetM_Adapted(r, ada),
				      pal_secam.GetGamma());
    out[3] = Convert::From_RGB_To_LCHab(tmp, r,
					pal_secam.GetM_Adapted(r, ada),
					pal_secam.GetGamma());
    out[4] = Convert::From_RGB_To_Luv(tmp, r,
				      pal_secam.GetM_Adapted(r, ada),
				      pal_secam.GetGamma());
    out[5] = Convert::From_RGB_To_LCHuv(tmp, r,
					pal_secam.GetM_Adapted(r, ada),
					pal_secam.GetGamma());
    out[6] = Convert::From_RGB_To_RGB(tmp, r, adobe.GetM_1_Adapted(r, ada),
				      adobe.GetGamma(),
				      pal_secam.GetM_Adapted(r, ada),
				      pal_secam.GetGamma());
    out[7] = Convert::From_RGB_To_RGB(tmp, r, apple.GetM_1_Adapted(r, ada),
				      apple.GetGamma(),
				      pal_secam.GetM_Adapted(r, ada),
				      pal_secam.GetGamma()); 
    out[8] = Convert::From_RGB_To_RGB(tmp, r, best.GetM_1_Adapted(r, ada),
				      best.GetGamma(),
				      pal_secam.GetM_Adapted(r, ada),
				      pal_secam.GetGamma());  
    out[9] = Convert::From_RGB_To_RGB(tmp, r, beta.GetM_1_Adapted(r, ada),
				      beta.GetGamma(),
				      pal_secam.GetM_Adapted(r, ada),
				      pal_secam.GetGamma());   
    out[10] = Convert::From_RGB_To_RGB(tmp, r,
				       bruce.GetM_1_Adapted(r, ada),
				       bruce.GetGamma(),
				       pal_secam.GetM_Adapted(r, ada),
				       pal_secam.GetGamma());   
    out[11] = Convert::From_RGB_To_RGB(tmp, r, cie.GetM_1_Adapted(r, ada),
				       cie.GetGamma(),
				       pal_secam.GetM_Adapted(r, ada),
				       pal_secam.GetGamma());   
    out[12] = Convert::From_RGB_To_RGB(tmp, r,
				       colormatch.GetM_1_Adapted(r, ada),
				       colormatch.GetGamma(),
				       pal_secam.GetM_Adapted(r, ada),
				       pal_secam.GetGamma());
    out[13] = Convert::From_RGB_To_RGB(tmp, r, don4.GetM_1_Adapted(r, ada),
				       don4.GetGamma(),
				       pal_secam.GetM_Adapted(r, ada),
				       pal_secam.GetGamma());
    out[14] = Convert::From_RGB_To_RGB(tmp, r, eci.GetM_1_Adapted(r, ada),
				       eci.GetGamma(),
				       pal_secam.GetM_Adapted(r, ada),
				       pal_secam.GetGamma());
    out[15] = Convert::From_RGB_To_RGB(tmp, r,
				       ektaspaceps5.GetM_1_Adapted(r, ada),
				       ektaspaceps5.GetGamma(),
				       pal_secam.GetM_Adapted(r, ada),
				       pal_secam.GetGamma());
    out[16] = Convert::From_RGB_To_RGB(tmp, r, ntsc.GetM_1_Adapted(r, ada),
				       ntsc.GetGamma(),
				       pal_secam.GetM_Adapted(r, ada),
				       pal_secam.GetGamma());
    out[17] = tmp;
    out[18] = Convert::From_RGB_To_RGB(tmp, r,
				       prophoto.GetM_1_Adapted(r, ada),
				       prophoto.GetGamma(),
				       pal_secam.GetM_Adapted(r, ada),
				       pal_secam.GetGamma());
    out[19] = Convert::From_RGB_To_RGB(tmp, r,
				       smpte_c.GetM_1_Adapted(r, ada),
				       smpte_c.GetGamma(),
				       pal_secam.GetM_Adapted(r, ada),
				       pal_secam.GetGamma());
    out[20] = Convert::From_RGB_To_RGB(tmp, r, srgb.GetM_1_Adapted(r, ada),
				       srgb.GetGamma(),
				       pal_secam.GetM_Adapted(r, ada),
				       pal_secam.GetGamma());
    out[21] = Convert::From_RGB_To_RGB(tmp, r,
				       widegamut.GetM_1_Adapted(r, ada),
				       widegamut.GetGamma(),
				       pal_secam.GetM_Adapted(r, ada),
				       pal_secam.GetGamma());
    break;

  case Manager::CSProPhotoRGB:
    tmp <<
      inputLines_[18].get<1>()->text().toFloat(),
      inputLines_[18].get<2>()->text().toFloat(),
      inputLines_[18].get<3>()->text().toFloat();

    out[0] = Convert::From_RGB_To_XYZ(tmp, r,
				      prophoto.GetM_Adapted(r, ada),
				      prophoto.GetGamma());
    out[1] = Convert::From_RGB_To_xyY(tmp, r,
				      prophoto.GetM_Adapted(r, ada),
				      prophoto.GetGamma());
    out[2] = Convert::From_RGB_To_Lab(tmp, r,
				      prophoto.GetM_Adapted(r, ada),
				      prophoto.GetGamma());
    out[3] = Convert::From_RGB_To_LCHab(tmp, r,
					prophoto.GetM_Adapted(r, ada),
					prophoto.GetGamma());
    out[4] = Convert::From_RGB_To_Luv(tmp, r,
				      prophoto.GetM_Adapted(r, ada),
				      prophoto.GetGamma());
    out[5] = Convert::From_RGB_To_LCHuv(tmp, r,
					prophoto.GetM_Adapted(r, ada),
					prophoto.GetGamma());
    out[6] = Convert::From_RGB_To_RGB(tmp, r, adobe.GetM_1_Adapted(r, ada),
				      adobe.GetGamma(),
				      prophoto.GetM_Adapted(r, ada),
				      prophoto.GetGamma());
    out[7] = Convert::From_RGB_To_RGB(tmp, r, apple.GetM_1_Adapted(r, ada),
				      apple.GetGamma(),
				      prophoto.GetM_Adapted(r, ada),
				      prophoto.GetGamma()); 
    out[8] = Convert::From_RGB_To_RGB(tmp, r, best.GetM_1_Adapted(r, ada),
				      best.GetGamma(),
				      prophoto.GetM_Adapted(r, ada),
				      prophoto.GetGamma());  
    out[9] = Convert::From_RGB_To_RGB(tmp, r, beta.GetM_1_Adapted(r, ada),
				      beta.GetGamma(),
				      prophoto.GetM_Adapted(r, ada),
				      prophoto.GetGamma());   
    out[10] = Convert::From_RGB_To_RGB(tmp, r,
				       bruce.GetM_1_Adapted(r, ada),
				       bruce.GetGamma(),
				       prophoto.GetM_Adapted(r, ada),
				       prophoto.GetGamma());   
    out[11] = Convert::From_RGB_To_RGB(tmp, r, cie.GetM_1_Adapted(r, ada),
				       cie.GetGamma(),
				       prophoto.GetM_Adapted(r, ada),
				       prophoto.GetGamma());   
    out[12] = Convert::From_RGB_To_RGB(tmp, r,
				       colormatch.GetM_1_Adapted(r, ada),
				       colormatch.GetGamma(),
				       prophoto.GetM_Adapted(r, ada),
				       prophoto.GetGamma());
    out[13] = Convert::From_RGB_To_RGB(tmp, r, don4.GetM_1_Adapted(r, ada),
				       don4.GetGamma(),
				       prophoto.GetM_Adapted(r, ada),
				       prophoto.GetGamma());
    out[14] = Convert::From_RGB_To_RGB(tmp, r, eci.GetM_1_Adapted(r, ada),
				       eci.GetGamma(),
				       prophoto.GetM_Adapted(r, ada),
				       prophoto.GetGamma());
    out[15] = Convert::From_RGB_To_RGB(tmp, r,
				       ektaspaceps5.GetM_1_Adapted(r, ada),
				       ektaspaceps5.GetGamma(),
				       prophoto.GetM_Adapted(r, ada),
				       prophoto.GetGamma());
    out[16] = Convert::From_RGB_To_RGB(tmp, r, ntsc.GetM_1_Adapted(r, ada),
				       ntsc.GetGamma(),
				       prophoto.GetM_Adapted(r, ada),
				       prophoto.GetGamma());
    out[17] = Convert::From_RGB_To_RGB(tmp, r,
				       pal_secam.GetM_1_Adapted(r, ada),
				       pal_secam.GetGamma(),
				       prophoto.GetM_Adapted(r, ada),
				       prophoto.GetGamma());
    out[18] = tmp;
    out[19] = Convert::From_RGB_To_RGB(tmp, r,
				       smpte_c.GetM_1_Adapted(r, ada),
				       smpte_c.GetGamma(),
				       prophoto.GetM_Adapted(r, ada),
				       prophoto.GetGamma());
    out[20] = Convert::From_RGB_To_RGB(tmp, r, srgb.GetM_1_Adapted(r, ada),
				       srgb.GetGamma(),
				       prophoto.GetM_Adapted(r, ada),
				       prophoto.GetGamma());
    out[21] = Convert::From_RGB_To_RGB(tmp, r,
				       widegamut.GetM_1_Adapted(r, ada),
				       widegamut.GetGamma(),
				       prophoto.GetM_Adapted(r, ada),
				       prophoto.GetGamma());
    break;

  case Manager::CSSMPTECRGB:
    tmp <<
      inputLines_[19].get<1>()->text().toFloat(),
      inputLines_[19].get<2>()->text().toFloat(),
      inputLines_[19].get<3>()->text().toFloat();

    out[0] = Convert::From_RGB_To_XYZ(tmp, r, smpte_c.GetM_Adapted(r, ada),
				      smpte_c.GetGamma());
    out[1] = Convert::From_RGB_To_xyY(tmp, r, smpte_c.GetM_Adapted(r, ada),
				      smpte_c.GetGamma());
    out[2] = Convert::From_RGB_To_Lab(tmp, r, smpte_c.GetM_Adapted(r, ada),
				      smpte_c.GetGamma());
    out[3] = Convert::From_RGB_To_LCHab(tmp, r,
					smpte_c.GetM_Adapted(r, ada),
					smpte_c.GetGamma());
    out[4] = Convert::From_RGB_To_Luv(tmp, r, smpte_c.GetM_Adapted(r, ada),
				      smpte_c.GetGamma());
    out[5] = Convert::From_RGB_To_LCHuv(tmp, r,
					smpte_c.GetM_Adapted(r, ada),
					smpte_c.GetGamma());
    out[6] = Convert::From_RGB_To_RGB(tmp, r, adobe.GetM_1_Adapted(r, ada),
				      adobe.GetGamma(),
				      smpte_c.GetM_Adapted(r, ada),
				      smpte_c.GetGamma());
    out[7] = Convert::From_RGB_To_RGB(tmp, r, apple.GetM_1_Adapted(r, ada),
				      apple.GetGamma(),
				      smpte_c.GetM_Adapted(r, ada),
				      smpte_c.GetGamma()); 
    out[8] = Convert::From_RGB_To_RGB(tmp, r, best.GetM_1_Adapted(r, ada),
				      best.GetGamma(),
				      smpte_c.GetM_Adapted(r, ada),
				      smpte_c.GetGamma());  
    out[9] = Convert::From_RGB_To_RGB(tmp, r, beta.GetM_1_Adapted(r, ada),
				      beta.GetGamma(),
				      smpte_c.GetM_Adapted(r, ada),
				      smpte_c.GetGamma());   
    out[10] = Convert::From_RGB_To_RGB(tmp, r,
				       bruce.GetM_1_Adapted(r, ada),
				       bruce.GetGamma(),
				       smpte_c.GetM_Adapted(r, ada),
				       smpte_c.GetGamma());   
    out[11] = Convert::From_RGB_To_RGB(tmp, r, cie.GetM_1_Adapted(r, ada),
				       cie.GetGamma(),
				       smpte_c.GetM_Adapted(r, ada),
				       smpte_c.GetGamma());   
    out[12] = Convert::From_RGB_To_RGB(tmp, r,
				       colormatch.GetM_1_Adapted(r, ada),
				       colormatch.GetGamma(),
				       smpte_c.GetM_Adapted(r, ada),
				       smpte_c.GetGamma());
    out[13] = Convert::From_RGB_To_RGB(tmp, r, don4.GetM_1_Adapted(r, ada),
				       don4.GetGamma(),
				       smpte_c.GetM_Adapted(r, ada),
				       smpte_c.GetGamma());
    out[14] = Convert::From_RGB_To_RGB(tmp, r, eci.GetM_1_Adapted(r, ada),
				       eci.GetGamma(),
				       smpte_c.GetM_Adapted(r, ada),
				       smpte_c.GetGamma());
    out[15] = Convert::From_RGB_To_RGB(tmp, r,
				       ektaspaceps5.GetM_1_Adapted(r, ada),
				       ektaspaceps5.GetGamma(),
				       smpte_c.GetM_Adapted(r, ada),
				       smpte_c.GetGamma());
    out[16] = Convert::From_RGB_To_RGB(tmp, r, ntsc.GetM_1_Adapted(r, ada),
				       ntsc.GetGamma(),
				       smpte_c.GetM_Adapted(r, ada),
				       smpte_c.GetGamma());
    out[17] = Convert::From_RGB_To_RGB(tmp, r,
				       pal_secam.GetM_1_Adapted(r, ada),
				       pal_secam.GetGamma(),
				       smpte_c.GetM_Adapted(r, ada),
				       smpte_c.GetGamma());
    out[18] = Convert::From_RGB_To_RGB(tmp, r,
				       prophoto.GetM_1_Adapted(r, ada),
				       prophoto.GetGamma(),
				       smpte_c.GetM_Adapted(r, ada),
				       smpte_c.GetGamma());
    out[19] = tmp;
    out[20] = Convert::From_RGB_To_RGB(tmp, r, srgb.GetM_1_Adapted(r, ada),
				       srgb.GetGamma(),
				       smpte_c.GetM_Adapted(r, ada),
				       smpte_c.GetGamma());
    out[21] = Convert::From_RGB_To_RGB(tmp, r,
				       widegamut.GetM_1_Adapted(r, ada),
				       widegamut.GetGamma(),
				       smpte_c.GetM_Adapted(r, ada),
				       smpte_c.GetGamma());
    break;

  case Manager::CSsRGB:
    tmp <<
      inputLines_[20].get<1>()->text().toFloat(),
      inputLines_[20].get<2>()->text().toFloat(),
      inputLines_[20].get<3>()->text().toFloat();

    out[0] = Convert::From_RGB_To_XYZ(tmp, r, srgb.GetM_Adapted(r, ada),
				      srgb.GetGamma());
    out[1] = Convert::From_RGB_To_xyY(tmp, r, srgb.GetM_Adapted(r, ada),
				      srgb.GetGamma());
    out[2] = Convert::From_RGB_To_Lab(tmp, r, srgb.GetM_Adapted(r, ada),
				      srgb.GetGamma());
    out[3] = Convert::From_RGB_To_LCHab(tmp, r, srgb.GetM_Adapted(r, ada),
					srgb.GetGamma());
    out[4] = Convert::From_RGB_To_Luv(tmp, r, srgb.GetM_Adapted(r, ada),
				      srgb.GetGamma());
    out[5] = Convert::From_RGB_To_LCHuv(tmp, r, srgb.GetM_Adapted(r, ada),
					srgb.GetGamma());
    out[6] = Convert::From_RGB_To_RGB(tmp, r, adobe.GetM_1_Adapted(r, ada),
				      adobe.GetGamma(),
				      srgb.GetM_Adapted(r, ada),
				      srgb.GetGamma());
    out[7] = Convert::From_RGB_To_RGB(tmp, r, apple.GetM_1_Adapted(r, ada),
				      apple.GetGamma(),
				      srgb.GetM_Adapted(r, ada),
				      srgb.GetGamma()); 
    out[8] = Convert::From_RGB_To_RGB(tmp, r, best.GetM_1_Adapted(r, ada),
				      best.GetGamma(),
				      srgb.GetM_Adapted(r, ada),
				      srgb.GetGamma());  
    out[9] = Convert::From_RGB_To_RGB(tmp, r, beta.GetM_1_Adapted(r, ada),
				      beta.GetGamma(),
				      srgb.GetM_Adapted(r, ada),
				      srgb.GetGamma());   
    out[10] = Convert::From_RGB_To_RGB(tmp, r,
				       bruce.GetM_1_Adapted(r, ada),
				       bruce.GetGamma(),
				       srgb.GetM_Adapted(r, ada),
				       srgb.GetGamma());   
    out[11] = Convert::From_RGB_To_RGB(tmp, r, cie.GetM_1_Adapted(r, ada),
				       cie.GetGamma(),
				       srgb.GetM_Adapted(r, ada),
				       srgb.GetGamma());   
    out[12] = Convert::From_RGB_To_RGB(tmp, r,
				       colormatch.GetM_1_Adapted(r, ada),
				       colormatch.GetGamma(),
				       srgb.GetM_Adapted(r, ada),
				       srgb.GetGamma());
    out[13] = Convert::From_RGB_To_RGB(tmp, r, don4.GetM_1_Adapted(r, ada),
				       don4.GetGamma(),
				       srgb.GetM_Adapted(r, ada),
				       srgb.GetGamma());
    out[14] = Convert::From_RGB_To_RGB(tmp, r, eci.GetM_1_Adapted(r, ada),
				       eci.GetGamma(),
				       srgb.GetM_Adapted(r, ada),
				       srgb.GetGamma());
    out[15] = Convert::From_RGB_To_RGB(tmp, r,
				       ektaspaceps5.GetM_1_Adapted(r, ada),
				       ektaspaceps5.GetGamma(),
				       srgb.GetM_Adapted(r, ada),
				       srgb.GetGamma());
    out[16] = Convert::From_RGB_To_RGB(tmp, r, ntsc.GetM_1_Adapted(r, ada),
				       ntsc.GetGamma(),
				       srgb.GetM_Adapted(r, ada),
				       srgb.GetGamma());
    out[17] = Convert::From_RGB_To_RGB(tmp, r,
				       pal_secam.GetM_1_Adapted(r, ada),
				       pal_secam.GetGamma(),
				       srgb.GetM_Adapted(r, ada),
				       srgb.GetGamma());
    out[18] = Convert::From_RGB_To_RGB(tmp, r,
				       prophoto.GetM_1_Adapted(r, ada),
				       prophoto.GetGamma(),
				       srgb.GetM_Adapted(r, ada),
				       srgb.GetGamma());
    out[19] = Convert::From_RGB_To_RGB(tmp, r,
				       smpte_c.GetM_1_Adapted(r, ada),
				       smpte_c.GetGamma(),
				       srgb.GetM_Adapted(r, ada),
				       srgb.GetGamma());
    out[20] = tmp;
    out[21] = Convert::From_RGB_To_RGB(tmp, r,
				       widegamut.GetM_1_Adapted(r, ada),
				       widegamut.GetGamma(),
				       srgb.GetM_Adapted(r, ada),
				       srgb.GetGamma());
    break;

  case Manager::CSWideGamutRGB:
    tmp <<
      inputLines_[21].get<1>()->text().toFloat(),
      inputLines_[21].get<2>()->text().toFloat(),
      inputLines_[21].get<3>()->text().toFloat();

    out[0] = Convert::From_RGB_To_XYZ(tmp, r,
				      widegamut.GetM_Adapted(r, ada),
				      widegamut.GetGamma());
    out[1] = Convert::From_RGB_To_xyY(tmp, r,
				      widegamut.GetM_Adapted(r, ada),
				      widegamut.GetGamma());
    out[2] = Convert::From_RGB_To_Lab(tmp, r,
				      widegamut.GetM_Adapted(r, ada),
				      widegamut.GetGamma());
    out[3] = Convert::From_RGB_To_LCHab(tmp, r,
					widegamut.GetM_Adapted(r, ada),
					widegamut.GetGamma());
    out[4] = Convert::From_RGB_To_Luv(tmp, r,
				      widegamut.GetM_Adapted(r, ada),
				      widegamut.GetGamma());
    out[5] = Convert::From_RGB_To_LCHuv(tmp, r,
					widegamut.GetM_Adapted(r, ada),
					widegamut.GetGamma());
    out[6] = Convert::From_RGB_To_RGB(tmp, r, adobe.GetM_1_Adapted(r, ada),
				      adobe.GetGamma(),
				      widegamut.GetM_Adapted(r, ada),
				      widegamut.GetGamma());
    out[7] = Convert::From_RGB_To_RGB(tmp, r, apple.GetM_1_Adapted(r, ada),
				      apple.GetGamma(),
				      widegamut.GetM_Adapted(r, ada),
				      widegamut.GetGamma()); 
    out[8] = Convert::From_RGB_To_RGB(tmp, r, best.GetM_1_Adapted(r, ada),
				      best.GetGamma(),
				      widegamut.GetM_Adapted(r, ada),
				      widegamut.GetGamma());  
    out[9] = Convert::From_RGB_To_RGB(tmp, r, beta.GetM_1_Adapted(r, ada),
				      beta.GetGamma(),
				      widegamut.GetM_Adapted(r, ada),
				      widegamut.GetGamma());   
    out[10] = Convert::From_RGB_To_RGB(tmp, r,
				       bruce.GetM_1_Adapted(r, ada),
				       bruce.GetGamma(),
				       widegamut.GetM_Adapted(r, ada),
				       widegamut.GetGamma());   
    out[11] = Convert::From_RGB_To_RGB(tmp, r, cie.GetM_1_Adapted(r, ada),
				       cie.GetGamma(),
				       widegamut.GetM_Adapted(r, ada),
				       widegamut.GetGamma());   
    out[12] = Convert::From_RGB_To_RGB(tmp, r,
				       colormatch.GetM_1_Adapted(r, ada),
				       colormatch.GetGamma(),
				       widegamut.GetM_Adapted(r, ada),
				       widegamut.GetGamma());
    out[13] = Convert::From_RGB_To_RGB(tmp, r, don4.GetM_1_Adapted(r, ada),
				       don4.GetGamma(),
				       widegamut.GetM_Adapted(r, ada),
				       widegamut.GetGamma());
    out[14] = Convert::From_RGB_To_RGB(tmp, r, eci.GetM_1_Adapted(r, ada),
				       eci.GetGamma(),
				       widegamut.GetM_Adapted(r, ada),
				       widegamut.GetGamma());
    out[15] = Convert::From_RGB_To_RGB(tmp, r,
				       ektaspaceps5.GetM_1_Adapted(r, ada),
				       ektaspaceps5.GetGamma(),
				       widegamut.GetM_Adapted(r, ada),
				       widegamut.GetGamma());
    out[16] = Convert::From_RGB_To_RGB(tmp, r, ntsc.GetM_1_Adapted(r, ada),
				       ntsc.GetGamma(),
				       widegamut.GetM_Adapted(r, ada),
				       widegamut.GetGamma());
    out[17] = Convert::From_RGB_To_RGB(tmp, r,
				       pal_secam.GetM_1_Adapted(r, ada),
				       pal_secam.GetGamma(),
				       widegamut.GetM_Adapted(r, ada),
				       widegamut.GetGamma());
    out[18] = Convert::From_RGB_To_RGB(tmp, r,
				       prophoto.GetM_1_Adapted(r, ada),
				       prophoto.GetGamma(),
				       widegamut.GetM_Adapted(r, ada),
				       widegamut.GetGamma());
    out[19] = Convert::From_RGB_To_RGB(tmp, r,
				       smpte_c.GetM_1_Adapted(r, ada),
				       smpte_c.GetGamma(),
				       widegamut.GetM_Adapted(r, ada),
				       widegamut.GetGamma());
    out[20] = Convert::From_RGB_To_RGB(tmp, r, srgb.GetM_1_Adapted(r, ada),
				       srgb.GetGamma(),
				       widegamut.GetM_Adapted(r, ada),
				       widegamut.GetGamma());
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
