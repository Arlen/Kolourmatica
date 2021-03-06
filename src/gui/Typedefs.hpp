/***********************************************************************
|*  Copyright (C) 2011 Arlen Avakian
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


#ifndef TYPEDEFS_HPP
#define TYPEDEFS_HPP

#include "ColourSpace.hpp"
#include "Colour_XYZ.hpp"
#include "Colour_xyY.hpp"
#include "Colour_Lab.hpp"
#include "Colour_LCHab.hpp"
#include "Colour_Luv.hpp"
#include "Colour_LCHuv.hpp"
#include "Colour_RGB.hpp"
#include "Illuminant.hpp"


typedef double Real;
typedef Colour_XYZ<Real> XYZ;
typedef Colour_xyY<Real> xyY;
typedef Colour_Lab<Real> Lab;
typedef Colour_LCHab<Real> LCHab;
typedef Colour_Luv<Real> Luv;
typedef Colour_LCHuv<Real> LCHuv;
typedef Colour_AdobeRGB<Real> AdobeRGB;
typedef Colour_AppleRGB<Real> AppleRGB;
typedef Colour_BestRGB<Real> BestRGB;
typedef Colour_BetaRGB<Real> BetaRGB;
typedef Colour_BruceRGB<Real> BruceRGB;
typedef Colour_CIERGB<Real> CIERGB;
typedef Colour_ColorMatchRGB<Real> ColorMatchRGB;
typedef Colour_DonRGB4<Real> DonRGB4;
typedef Colour_ECIRGB<Real> ECIRGB;
typedef Colour_EktaSpacePS5<Real> EktaSpacePS5;
typedef Colour_NTSCRGB<Real> NTSCRGB;
typedef Colour_PAL_SECAMRGB<Real> PAL_SECAMRGB;
typedef Colour_ProPhotoRGB<Real> ProPhotoRGB;
typedef Colour_SMPTE_CRGB<Real> SMPTE_CRGB;
typedef Colour_sRGB<Real> sRGB;
typedef Colour_WideGamutRGB<Real> WideGamutRGB;

typedef BaseIlluminant<Real> Illuminant;
typedef IlluminantA<Real, _1931_2> A_1931_2;
typedef IlluminantA<Real, _1964_10> A_1964_10;
typedef IlluminantB<Real, _1931_2> B_1931_2;
typedef IlluminantB<Real, _1964_10> B_1964_10;
typedef IlluminantC<Real, _1931_2> C_1931_2;
typedef IlluminantC<Real, _1964_10> C_1964_10;
typedef IlluminantD50<Real, _1931_2> D50_1931_2;
typedef IlluminantD50<Real, _1964_10> D50_1964_10;
typedef IlluminantD55<Real, _1931_2> D55_1931_2;
typedef IlluminantD55<Real, _1964_10> D55_1964_10;
typedef IlluminantD65<Real, _1931_2> D65_1931_2;
typedef IlluminantD65<Real, _1964_10> D65_1964_10;
typedef IlluminantD75<Real, _1931_2> D75_1931_2;
typedef IlluminantD75<Real, _1964_10> D75_1964_10;
typedef IlluminantE<Real, _1931_2> E_1931_2;
typedef IlluminantE<Real, _1964_10> E_1964_10;
typedef IlluminantF1<Real, _1931_2> F1_1931_2;
typedef IlluminantF1<Real, _1964_10> F1_1964_10;
typedef IlluminantF2<Real, _1931_2> F2_1931_2;
typedef IlluminantF2<Real, _1964_10> F2_1964_10;
typedef IlluminantF3<Real, _1931_2> F3_1931_2;
typedef IlluminantF3<Real, _1964_10> F3_1964_10;
typedef IlluminantF4<Real, _1931_2> F4_1931_2;
typedef IlluminantF4<Real, _1964_10> F4_1964_10;
typedef IlluminantF5<Real, _1931_2> F5_1931_2;
typedef IlluminantF5<Real, _1964_10> F5_1964_10;
typedef IlluminantF6<Real, _1931_2> F6_1931_2;
typedef IlluminantF6<Real, _1964_10> F6_1964_10;
typedef IlluminantF7<Real, _1931_2> F7_1931_2;
typedef IlluminantF7<Real, _1964_10> F7_1964_10;
typedef IlluminantF8<Real, _1931_2> F8_1931_2;
typedef IlluminantF8<Real, _1964_10> F8_1964_10;
typedef IlluminantF9<Real, _1931_2> F9_1931_2;
typedef IlluminantF9<Real, _1964_10> F9_1964_10;
typedef IlluminantF10<Real, _1931_2> F10_1931_2;
typedef IlluminantF10<Real, _1964_10> F10_1964_10;
typedef IlluminantF11<Real, _1931_2> F11_1931_2;
typedef IlluminantF11<Real, _1964_10> F11_1964_10;
typedef IlluminantF12<Real, _1931_2> F12_1931_2;
typedef IlluminantF12<Real, _1964_10> F12_1964_10;

#endif
