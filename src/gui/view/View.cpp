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
#include "Render.hpp"

#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QComboBox>
#include <QtGui/QLabel>
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsView>
#include <QtGui/QDoubleValidator>


View::View() : QWidget(){

  initWidgets();
}

void View::initialize(int wcs, int scs, int rw, int am){

}

/* public slots */
#include <iostream>
using namespace std;

void View::setWorkingColourSpace(int wcs){

  cout << "View::wcs -> " << wcs << endl;
}

void View::setSystemColourSpace(int scs){

  cout << "View::scs -> " << scs << endl;
}

void View::setReferenceWhite(int rw){

  cout << "View::rw -> " << rw << endl;
}

void View::setAdaptationMethod(int am){

  cout << "View::am -> " << am << endl;
}


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
  pPrecisionOptions_->addItem("float");
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

  connectOptionsWidgets();

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
  pRenderer_ = new Render;
  pScene_->addItem(pRenderer_);

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
  //initialize the values
}


/*private slots*/
void View::setPrecision(int precision){

  cout << "View::precision -> " << precision << endl;
}

void View::setAccuracy(const QString& accuracy){

  cout << "View::accuracy -> " << accuracy.toStdString() << endl;
}

void View::setCamera(int camera){

  cout << "View::camera -> " << camera << endl;
}
