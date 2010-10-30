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


#include <QtGui/QDoubleValidator>
#include <QtGui/QGridLayout>
#include <QtGui/QPushButton>
#include <QtGui/QLineEdit>
#include <QtGui/QComboBox>
#include <QtGui/QLabel>

#include <boost/foreach.hpp>

#include "ConversionTab.hpp"


ConversionTab::ConversionTab() : QWidget(){

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
}

void ConversionTab::InitWidgets(){

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
  pChromaticAdaptations_->addItem("Bradford");
  pChromaticAdaptations_->addItem("Von Kries");

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
    rL.get<4>() = new QPushButton("Display");
    rL.get<4>()->setFixedSize(80, 24);

    pMainLayout_->addItem(new QSpacerItem(128, 1,
    					  QSizePolicy::MinimumExpanding,
    					  QSizePolicy::Minimum), col, 0);
    pMainLayout_->addWidget(rL.get<0>(), col, 1);
    pMainLayout_->addWidget(rL.get<1>(), col, 2);
    pMainLayout_->addWidget(rL.get<2>(), col, 3);
    pMainLayout_->addWidget(rL.get<3>(), col, 4);
    pMainLayout_->addWidget(rL.get<4>(), col, 5);
    pMainLayout_->addItem(new QSpacerItem(128, 1,
    					  QSizePolicy::MinimumExpanding,
    					  QSizePolicy::Minimum), col, 6);
    ++col;
  }
  ClearInputs();
  pColorSpaces_->setCurrentIndex(1);
  pSystemColorSpaces_->setCurrentIndex(20);
  pReferenceWhites_->setCurrentIndex(5);
  pChromaticAdaptations_->setCurrentIndex(1);
}

void ConversionTab::ClearInputs(){

  BOOST_FOREACH(InputLine& rL, inputLines_){
    rL.get<1>()->setText(QString::number(0.0, 'f', 12));
    rL.get<2>()->setText(QString::number(0.0, 'f', 12));
    rL.get<3>()->setText(QString::number(0.0, 'f', 12));
  }
}

void ConversionTab::SetDoubleValidator(QLineEdit* const pLineEdit){

  QDoubleValidator *pDoubleValidator = new QDoubleValidator(pLineEdit);
  pDoubleValidator->setNotation(QDoubleValidator::StandardNotation);
  pDoubleValidator->setDecimals(10);
  pLineEdit->setValidator(pDoubleValidator);
}
