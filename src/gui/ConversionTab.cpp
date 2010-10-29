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
#include <QtGui/QLabel>

#include "ConversionTab.hpp"


ConversionTab::ConversionTab() : QWidget(){

  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  InitWidgets();
}

void ConversionTab::InitWidgets(){

  pMainLayout_ = new QGridLayout(this);
  pMainLayout_->setContentsMargins(0, 2, 0, 2);
  pMainLayout_->setSpacing(2);

  pFromXYZ_ = new QPushButton("XYZ");
  pFromXYZ_->setFixedSize(48, 24);
  pEditX_ = new QLineEdit;
  pEditX_->setFixedSize(128, 24);
  SetDoubleValidator(pEditX_);
  pEditY_ = new QLineEdit;
  pEditY_->setFixedSize(128, 24);
  SetDoubleValidator(pEditY_);
  pEditZ_ = new QLineEdit();
  pEditZ_->setFixedSize(128, 24);
  SetDoubleValidator(pEditZ_);
  pDisplayXYZ_ = new QPushButton("Display");
  pDisplayXYZ_->setFixedSize(80, 24);

  pFromLuv_ = new QPushButton("Luv");
  pFromLuv_->setFixedSize(48, 24);
  pEditL_ = new QLineEdit;
  pEditL_->setFixedSize(128, 24);
  SetDoubleValidator(pEditL_);
  pEditu_ = new QLineEdit;
  pEditu_->setFixedSize(128, 24);
  SetDoubleValidator(pEditu_);
  pEditv_ = new QLineEdit;
  pEditv_->setFixedSize(128, 24);
  SetDoubleValidator(pEditv_);
  pDisplayLuv_ = new QPushButton("Display");
  pDisplayLuv_->setFixedSize(80, 24);

  pFromRGB_ = new QPushButton("RGB");
  pFromRGB_->setFixedSize(48, 24);
  pEditR_ = new QLineEdit;
  pEditR_->setFixedSize(128, 24);
  SetDoubleValidator(pEditR_);
  pEditG_ = new QLineEdit;
  pEditG_->setFixedSize(128, 24);
  SetDoubleValidator(pEditG_);
  pEditB_ = new QLineEdit;
  pEditB_->setFixedSize(128, 24);
  SetDoubleValidator(pEditB_);
  pDisplayRGB_ = new QPushButton("Display");
  pDisplayRGB_->setFixedSize(80, 24);

  pMainLayout_->addItem(new QSpacerItem(128, 1,
					QSizePolicy::MinimumExpanding,
					QSizePolicy::Minimum), 0, 0);
  pMainLayout_->addWidget(pFromXYZ_, 0, 1);
  pMainLayout_->addWidget(pEditX_, 0, 2);
  pMainLayout_->addWidget(pEditY_, 0, 3);
  pMainLayout_->addWidget(pEditZ_, 0, 4);
  pMainLayout_->addWidget(pDisplayXYZ_, 0, 5);
  pMainLayout_->addItem(new QSpacerItem(128, 1,
					QSizePolicy::MinimumExpanding,
					QSizePolicy::Minimum), 0, 6);

  pMainLayout_->addItem(new QSpacerItem(128, 1,
					QSizePolicy::MinimumExpanding,
					QSizePolicy::Minimum), 1, 0);
  pMainLayout_->addWidget(pFromLuv_, 1, 1);
  pMainLayout_->addWidget(pEditL_, 1, 2);
  pMainLayout_->addWidget(pEditu_, 1, 3);
  pMainLayout_->addWidget(pEditv_, 1, 4);
  pMainLayout_->addWidget(pDisplayLuv_, 1, 5);
  pMainLayout_->addItem(new QSpacerItem(128, 1,
					QSizePolicy::MinimumExpanding,
					QSizePolicy::Minimum), 1, 6);

  pMainLayout_->addItem(new QSpacerItem(128, 1,
					QSizePolicy::MinimumExpanding,
					QSizePolicy::Minimum), 2, 0);
  pMainLayout_->addWidget(pFromRGB_, 2, 1);
  pMainLayout_->addWidget(pEditR_, 2, 2);
  pMainLayout_->addWidget(pEditG_, 2, 3);
  pMainLayout_->addWidget(pEditB_, 2, 4);
  pMainLayout_->addWidget(pDisplayRGB_, 2, 5);
  pMainLayout_->addItem(new QSpacerItem(128, 1,
					QSizePolicy::MinimumExpanding,
					QSizePolicy::Minimum), 2, 6);
}

void ConversionTab::SetDoubleValidator(QLineEdit* const pLineEdit){

  QDoubleValidator *pDoubleValidator = new QDoubleValidator(pLineEdit);
  pDoubleValidator->setNotation(QDoubleValidator::StandardNotation);
  pDoubleValidator->setDecimals(10);
  pLineEdit->setValidator(pDoubleValidator);
}
