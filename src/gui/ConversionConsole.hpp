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


#ifndef CONVERSIONCONSOLE_HPP
#define CONVERSIONCONSOLE_HPP

#include "../core/Manager.hpp"

#include "../../../eigen/Eigen/Core"

#include <QtGui/QWidget>
#include <QtCore/QStringList>

#include <boost/array.hpp>
#include <boost/tuple/tuple.hpp>

class QGridLayout;
class QPushButton;
class QLineEdit;
class QComboBox;
class QLabel;


class ConversionConsole : public QWidget{

Q_OBJECT

public:
  ConversionConsole();

private:
  typedef boost::tuple<QPushButton*, QLineEdit*, QLineEdit*, QLineEdit*>
  InputLine;
  typedef boost::array<InputLine, 22> InputLines;

  void InitWidgets();
  void ClearInputs();
  void ConnectConversionButtons();
  void ConvertAll();
  void ConnectWorkingColorSpaceButton();
  void ConnectSystemColorSpaceButton();
  void ConnectRefWhiteButton();
  void ConnectAdaptationButton();
  void SetDoubleValidator(QLineEdit* const pLineEdit);

  QStringList colorSpaces_;
  QStringList referenceWhites_;

  QLabel* pWorkingColorSpace_;
  QComboBox* pWorkingColorSpaces_;
  QLabel* pSystemColorSpace_;
  QComboBox* pSystemColorSpaces_;
  QLabel* pReferenceWhite_;
  QComboBox* pReferenceWhites_;
  QLabel* pChromaticAdaptation_;
  QComboBox* pChromaticAdaptations_;

  InputLines inputLines_;
  Manager::CSType convertingFrom_;
  Manager::CSType workingColorSpace_;
  Manager::CSType systemColorSpace_;
  Eigen::Vector3f refWhite_;
  Eigen::Matrix3f adaptationMethod_;

  QGridLayout* pMainLayout_;

private slots:
  void ConvertFrom_XYZ_To_all();
  void ConvertFrom_xyY_To_all();
  void ConvertFrom_Lab_To_all();
  void ConvertFrom_LCHab_To_all();
  void ConvertFrom_Luv_To_all();
  void ConvertFrom_LCHuv_To_all();
  void ConvertFrom_Adobe_To_all();
  void ConvertFrom_Apple_To_all();
  void ConvertFrom_Best_To_all();
  void ConvertFrom_Beta_To_all();
  void ConvertFrom_Bruce_To_all();
  void ConvertFrom_CIE_To_all();
  void ConvertFrom_ColorMatch_To_all();
  void ConvertFrom_Don4_To_all();
  void ConvertFrom_ECI_To_all();
  void ConvertFrom_EktaSpacePS5_To_all();
  void ConvertFrom_NTSC_To_all();
  void ConvertFrom_PALSECAM_To_all();
  void ConvertFrom_ProPhoto_To_all();
  void ConvertFrom_SMPTEC_To_all();
  void ConvertFrom_sRGB_To_all();
  void ConvertFrom_WideGamut_To_all();
  void SetWorkingColorSpace(int);
  void SetSystemColorSpace(int);
  void SetRefWhite(int);
  void SetAdaptationMethod(int);
};

#endif
