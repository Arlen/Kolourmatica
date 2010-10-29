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


#include <QtGui/QWidget>

class QGridLayout;
class QPushButton;
class QLineEdit;
class QLabel;


class ConversionTab : public QWidget{

public:
  ConversionTab();

private:
  void InitWidgets();
  void SetDoubleValidator(QLineEdit* const pLineEdit);

  QPushButton* pFromXYZ_;
  QLineEdit* pEditX_;
  QLineEdit* pEditY_;
  QLineEdit* pEditZ_;
  QPushButton* pDisplayXYZ_;

  QPushButton* pFromLuv_;
  QLineEdit* pEditL_;
  QLineEdit* pEditu_;
  QLineEdit* pEditv_;
  QPushButton* pDisplayLuv_;

  QPushButton* pFromRGB_;
  QLineEdit* pEditR_;
  QLineEdit* pEditG_;
  QLineEdit* pEditB_;
  QPushButton* pDisplayRGB_;

  QGridLayout* pMainLayout_;
};
