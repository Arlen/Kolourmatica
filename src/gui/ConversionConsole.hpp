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

#include "../core/Utility.hpp"
#include "../core/AdaptationMethod.hpp"
#include "../core/ReferenceWhite.hpp"
#include "../core/Space_XYZ.hpp"
#include "../core/Space_xyY.hpp"
#include "../core/Space_Lab.hpp"
#include "../core/Space_LCHab.hpp"
#include "../core/Space_Luv.hpp"
#include "../core/Space_LCHuv.hpp"
#include "../core/Space_AdobeRGB.hpp"
#include "../core/Space_AppleRGB.hpp"
#include "../core/Space_BestRGB.hpp"
#include "../core/Space_BetaRGB.hpp"
#include "../core/Space_BruceRGB.hpp"
#include "../core/Space_CIERGB.hpp"
#include "../core/Space_ColorMatchRGB.hpp"
#include "../core/Space_DonRGB4.hpp"
#include "../core/Space_ECIRGB.hpp"
#include "../core/Space_EktaSpacePS5.hpp"
#include "../core/Space_NTSCRGB.hpp"
#include "../core/Space_PAL_SECAMRGB.hpp"
#include "../core/Space_ProPhotoRGB.hpp"
#include "../core/Space_SMPTE_CRGB.hpp"
#include "../core/Space_sRGB.hpp"
#include "../core/Space_WideGamutRGB.hpp"

#include "../../../eigen/Eigen/Core"

#include <QtGui/QWidget>

#include <boost/array.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/fusion/container/list/cons.hpp>

using boost::tuple;
using boost::array;
using boost::fusion::cons;


class Viewer;

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
  
  enum{
    CSXYZ = 0, CSxyY, CSLab, CSLCHab, CSLuv, CSLCHuv, CSAdobeRGB, CSAppleRGB,
    CSBestRGB, CSBetaRGB, CSBruceRGB, CSCIERGB, CSColorMatchRGB, CSDonRGB4,
    CSECIRGB, CSEktaSpacePS5, CSNTSCRGB, CSPALSECAMRGB, CSProPhotoRGB,
    CSSMPTECRGB, CSsRGB, CSWideGamutRGB
  };
  
  ConversionConsole();
  void setViewer(Viewer* pViewer);

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
  void connectWorkingColourSpaceButton();
  void connectSystemColourSpaceButton();
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

  InputLines inputLines_;
  int convertingFrom_;
  int workingColourSpace_;
  int systemColourSpace_;
  ReferenceWhite<double> refWhite_;
  Matrix<double, 3, 3> adaptationMethod_;

  QVBoxLayout* pLayout0_;
  QHBoxLayout* pLayout1A_;
  QGridLayout* pLayout1B_;
  QScrollArea* pScrollArea_;
  //Viewer* pViewer_;
 
  Vector3d input_;
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
  void setWorkingColourSpace(int);
  void setSystemColourSpace(int);
  void setRefWhite(int);
  void setAdaptationMethod(int);
};

#endif
