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


#include "View.hpp"
#include "Viewport.hpp"

#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QComboBox>
#include <QtGui/QLabel>
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsView>
#include <QtGui/QDoubleValidator>


View::View(){

  configured_ = false;
  initWidgets();
  connectOptionsWidgets();
}

void View::configure(const ViewConfig& vc){

  if(!configured_){
    ViewportConfig config;
    config.workingColourSpace_ = vc.workingColourSpace_;
    config.systemColourSpace_ = vc.systemColourSpace_;
    config.referenceWhite_ = vc.referenceWhite_;
    config.adaptationMethod_ = vc.adaptationMethod_;
    config.precision_ = pPrecisionOptions_->currentIndex();
    config.accuracy_ = pAccuracyOptions_->currentText().toDouble();
    config.camera_ = pCameraOptions_->currentIndex();
    pViewport_->configure(config);
    configured_ = true;
  }
}

/* public slots */
void View::setWorkingColourSpace(int wcs){ pViewport_->setWCS(wcs); }
void View::setSystemColourSpace(int scs){ pViewport_->setSCS(scs); }
void View::setReferenceWhite(int rw){ pViewport_->setRW(rw); }
void View::setAdaptationMethod(int am){ pViewport_->setAM(am); }


/* private */
void View::initWidgets(){

  /* create the layouts. */
  pLayoutA_ = new QVBoxLayout(this);
  pLayoutA_->setContentsMargins(0, 0, 0, 0);
  pLayoutA_->setSpacing(0);

  pLayoutB1_ = new QHBoxLayout;
  pLayoutB1_->setContentsMargins(0, 0, 0, 0);
  pLayoutB1_->setSpacing(1);

  /* create the widgets. */
  pPrecision_ = new QLabel(" precision ");
  pAccuracy_ = new QLabel(" accuracy ");
  pCamera_ = new QLabel(" camera ");

  pPrecisionOptions_ = new QComboBox;
  pAccuracyOptions_ = new QComboBox;
  pCameraOptions_ = new QComboBox;

  /* set up the widgets. */
  pPrecisionOptions_->addItem("single");
  pPrecisionOptions_->addItem("double");

  pAccuracyOptions_->addItem("0.25");
  pAccuracyOptions_->setEditable(true);
  QDoubleValidator* pValidator = new QDoubleValidator(pAccuracyOptions_);
  pValidator->setNotation(QDoubleValidator::StandardNotation);
  pValidator->setDecimals(10);
  pAccuracyOptions_->setValidator(pValidator);

  pCameraOptions_->addItem("front");
  pCameraOptions_->addItem("left");
  pCameraOptions_->addItem("right");
  pCameraOptions_->addItem("top");
  pCameraOptions_->addItem("bottom");

  /* organize the options widgets. */
  pLayoutB1_->addWidget(pPrecision_);
  pLayoutB1_->addWidget(pPrecisionOptions_);
  pLayoutB1_->addItem(new QSpacerItem(8, 1,
				      QSizePolicy::Fixed,
				      QSizePolicy::Fixed));
  pLayoutB1_->addWidget(pAccuracy_);
  pLayoutB1_->addWidget(pAccuracyOptions_);
  pLayoutB1_->addItem(new QSpacerItem(8, 1,
				      QSizePolicy::Fixed,
				      QSizePolicy::Fixed));
  pLayoutB1_->addWidget(pCamera_);
  pLayoutB1_->addWidget(pCameraOptions_);
  pLayoutB1_->addItem(new QSpacerItem(1, 1,
  				      QSizePolicy::MinimumExpanding,
  				      QSizePolicy::Ignored));

  pScene_ = new QGraphicsScene;
  pView_ = new QGraphicsView(pScene_);
  pView_->setFrameShape(QFrame::NoFrame);
  pView_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  pView_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  pViewport_ = new Viewport;
  pScene_->addItem(pViewport_);

  pLayoutA_->addLayout(pLayoutB1_);
  pLayoutA_->addWidget(pView_);
}

void View::connectOptionsWidgets(){

  QObject::connect(pPrecisionOptions_, SIGNAL(currentIndexChanged(int)),
		   this, SLOT(setPrecision(int)));
  QObject::connect(pAccuracyOptions_, SIGNAL(activated(const QString&)),
		   this, SLOT(setAccuracy(const QString&)));
  QObject::connect(pCameraOptions_, SIGNAL(currentIndexChanged(int)),
		   this, SLOT(setCamera(int)));
}


/*private slots*/
void View::setPrecision(int precision){ pViewport_->setPrecision(precision); }

void View::setAccuracy(const QString& accuracy){

  pViewport_->setAccuracy(accuracy.toDouble());
}

void View::setCamera(int camera){ pViewport_->setCamera(camera); }

