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

#include "Utility.hpp"
#include "AdaptationMethod.hpp"
#include "ReferenceWhite.hpp"
#include "Space_XYZ.hpp"
#include "Space_xyY.hpp"
#include "Space_Lab.hpp"
#include "Space_LCHab.hpp"
#include "Space_Luv.hpp"
#include "Space_LCHuv.hpp"
#include "Space_AdobeRGB.hpp"
#include "Space_AppleRGB.hpp"
#include "Space_BestRGB.hpp"
#include "Space_BetaRGB.hpp"
#include "Space_BruceRGB.hpp"
#include "Space_CIERGB.hpp"
#include "Space_ColorMatchRGB.hpp"
#include "Space_DonRGB4.hpp"
#include "Space_ECIRGB.hpp"
#include "Space_EktaSpacePS5.hpp"
#include "Space_NTSCRGB.hpp"
#include "Space_PAL_SECAMRGB.hpp"
#include "Space_ProPhotoRGB.hpp"
#include "Space_SMPTE_CRGB.hpp"
#include "Space_sRGB.hpp"
#include "Space_WideGamutRGB.hpp"

#include <Eigen/Core>

#include <QtGui/QWidget>

#include <boost/array.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/fusion/container/list/cons.hpp>

using boost::tuple;
using boost::array;
using boost::fusion::cons;


class View;

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QScrollArea;
class QStringList;
class QPushButton;
class QLineEdit;
class QComboBox;
class QLabel;


class ConversionConsole : public QWidget{

  Q_OBJECT

public:
  ConversionConsole();
  void setViewer(View* const pView);

private:
  typedef tuple<QPushButton*, QLineEdit*, QLineEdit*, QLineEdit*> InputLine;
  typedef array<InputLine, 22> InputLines;
  typedef double Real;

  typedef GlobalReferenceWhite<Real> GRW;
  typedef Space_XYZ<Real> XYZ;
  typedef Space_xyY<Real> xyY;
  typedef Space_Lab<Real> Lab;
  typedef Space_LCHab<Real> LCHab;
  typedef Space_Luv<Real> Luv;
  typedef Space_LCHuv<Real> LCHuv;
  typedef Space_AdobeRGB<Real> AdobeRGB;
  typedef Space_AppleRGB<Real> AppleRGB;
  typedef Space_BestRGB<Real> BestRGB;
  typedef Space_BetaRGB<Real> BetaRGB;
  typedef Space_BruceRGB<Real> BruceRGB;
  typedef Space_CIERGB<Real> CIERGB;
  typedef Space_ColorMatchRGB<Real> ColorMatchRGB;
  typedef Space_DonRGB4<Real> DonRGB4;
  typedef Space_ECIRGB<Real> ECIRGB;
  typedef Space_EktaSpacePS5<Real> EktaSpacePS5;
  typedef Space_NTSCRGB<Real> NTSCRGB;
  typedef Space_PAL_SECAMRGB<Real> PAL_SECAMRGB;
  typedef Space_ProPhotoRGB<Real> ProPhotoRGB;
  typedef Space_SMPTE_CRGB<Real> SMPTE_CRGB;
  typedef Space_sRGB<Real> sRGB;
  typedef Space_WideGamutRGB<Real> WideGamutRGB;

  typedef cons<XYZ, cons<xyY, cons<Lab, cons<LCHab, cons<Luv, cons<LCHuv,
          cons<AdobeRGB, cons<AppleRGB, cons<BestRGB, cons<BetaRGB,
  	  cons<BruceRGB, cons<CIERGB, cons<ColorMatchRGB, cons<DonRGB4,
  	  cons<ECIRGB, cons<EktaSpacePS5, cons<NTSCRGB, cons<PAL_SECAMRGB,
  	  cons<ProPhotoRGB, cons<SMPTE_CRGB, cons<sRGB, cons<WideGamutRGB
  	  > > > > > > > > > > > > > > > > > > > > > > ColourSpaces;

  void initWidgets();
  void clearInputs();
  void connectConversionButtons();
  Vector3d readSource(int index);
  void writeResults(std::vector<Eigen::Vector3d>& results);
  void connectRefWhiteButton();
  void connectAdaptationButton();
  void setDoubleValidator(QLineEdit* const pLineEdit);

  QStringList colourSpaces_;
  QStringList referenceWhites_;

  QLabel* pWorkingColourSpace_;
  QComboBox* pWorkingColourSpaces_;
  QLabel* pSystemColourSpace_;
  QComboBox* pSystemColourSpaces_;
  QLabel* pReferenceWhite_;
  QComboBox* pReferenceWhites_;
  QLabel* pChromaticAdaptation_;
  QComboBox* pChromaticAdaptations_;

  QVBoxLayout* pLayoutA_;
  QHBoxLayout* pLayoutB1_;
  QGridLayout* pLayoutC1_;
  QScrollArea* pScrollArea_;
  View* pView_;

  ReferenceWhite<Real> refWhite_;

  int convertingFrom_;
  int workingColourSpace_;
  int systemColourSpace_;

  Matrix<Real, 3, 3> adaptationMethod_;
  Vector3d input_;

  InputLines inputLines_;
  ColourSpaces allColourSpaces_;
  
private slots:
  void convertFrom_XYZ_To_all();
  void convertFrom_xyY_To_all();
  void convertFrom_Lab_To_all();
  void convertFrom_LCHab_To_all();
  void convertFrom_Luv_To_all();
  void convertFrom_LCHuv_To_all();
  void convertFrom_Adobe_To_all();
  void convertFrom_Apple_To_all();
  void convertFrom_Best_To_all();
  void convertFrom_Beta_To_all();
  void convertFrom_Bruce_To_all();
  void convertFrom_CIE_To_all();
  void convertFrom_ColorMatch_To_all();
  void convertFrom_Don4_To_all();
  void convertFrom_ECI_To_all();
  void convertFrom_EktaSpacePS5_To_all();
  void convertFrom_NTSC_To_all();
  void convertFrom_PALSECAM_To_all();
  void convertFrom_ProPhoto_To_all();
  void convertFrom_SMPTEC_To_all();
  void convertFrom_sRGB_To_all();
  void convertFrom_WideGamut_To_all();
  void setRefWhite(int);
  void setAdaptationMethod(int);
};

#endif
