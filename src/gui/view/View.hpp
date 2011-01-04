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


#ifndef VIEW_HPP
#define VIEW_HPP

#include <QtGui/QWidget>


class Viewport;
class ViewConfig;
class ViewportConfig;

class QHBoxLayout;
class QVBoxLayout;
class QComboBox;
class QLabel;
class QGraphicsScene;
class QGraphicsView;


class View : public QWidget{

Q_OBJECT

public:
  View();
  void configure(const ViewConfig& vc);

public slots:
  void setWorkingColourSpace(int wcs);
  void setSystemColourSpace(int scs);
  void setReferenceWhite(int rw);
  void setAdaptationMethod(int am);

private:
  void initWidgets();
  void connectOptionsWidgets();

  QVBoxLayout* pLayoutA_;
  QHBoxLayout* pLayoutB1_;

  QLabel* pPrecision_;
  QComboBox* pPrecisionOptions_;
  QLabel* pAccuracy_;
  QComboBox* pAccuracyOptions_;
  QLabel* pCamera_;
  QComboBox* pCameraOptions_;

  QGraphicsScene* pScene_;
  QGraphicsView* pView_;

  Viewport* pViewport_;

  bool configured_;

private slots:
  void setPrecision(int precision);
  void setAccuracy(const QString& accuracy);
  void setCamera(int camera);
};


struct ViewConfig{

  int workingColourSpace_;
  int systemColourSpace_;
  int referenceWhite_;
  int adaptationMethod_;
};

#endif
