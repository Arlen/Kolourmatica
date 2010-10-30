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
#include <QtCore/QStringList>

#include <boost/array.hpp>
#include <boost/tuple/tuple.hpp>


class QGridLayout;
class QPushButton;
class QLineEdit;
class QComboBox;
class QLabel;


class ConversionTab : public QWidget{

public:
  ConversionTab();

private:
  typedef boost::tuple<QPushButton*, QLineEdit*, QLineEdit*, QLineEdit*,
  		       QPushButton*> InputLine;
  typedef boost::array<InputLine, 22> InputLines;

  void InitWidgets();
  void ClearInputs();
  void SetDoubleValidator(QLineEdit* const pLineEdit);

  QStringList colorSpaces_;
  QStringList referenceWhites_;

  QLabel* pColorSpace_;
  QComboBox* pColorSpaces_;
  QLabel* pSystemColorSpace_;
  QComboBox* pSystemColorSpaces_;
  QLabel* pReferenceWhite_;
  QComboBox* pReferenceWhites_;
  QLabel* pChromaticAdaptation_;
  QComboBox* pChromaticAdaptations_;

  InputLines inputLines_;

  QGridLayout* pMainLayout_;
};
