/***********************************************************************
|*  Copyright (C) 2024 Arlen Avakian
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


#pragma once

#include <QtCore/QString>

#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/push_back.hpp>

#include <enums_seq.hpp>


#define KM_DEFINE_ENUM(name, SEQ)                                \
enum class name : i32 {                                          \
    BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_PUSH_BACK(SEQ, INVALID))      \
};                                                               \


namespace enums
{
    KM_DEFINE_ENUM(RefWhite, KM_REF_WHITES_SEQ)
    KM_DEFINE_ENUM(RefWhiteFov, KM_REF_WHITE_FOV_SEQ)
    KM_DEFINE_ENUM(ColourSpace, KM_COLOUR_SPACES_SEQ)

    static RefWhite RefWhiteArray[] = {
        RefWhite::A,
        RefWhite::B,
        RefWhite::C,
        RefWhite::D50,
        RefWhite::D55,
        RefWhite::D65,
        RefWhite::D75,
        RefWhite::E,
        RefWhite::F1,
        RefWhite::F2,
        RefWhite::F3,
        RefWhite::F4,
        RefWhite::F5,
        RefWhite::F6,
        RefWhite::F7,
        RefWhite::F8,
        RefWhite::F9,
        RefWhite::F10,
        RefWhite::F11,
        RefWhite::F12
    };

    static RefWhiteFov RefWhiteFovArray[] = {
        RefWhiteFov::_1931_2,
        RefWhiteFov::_1964_10
    };

    static ColourSpace ColourSpaceArray[] = {
        ColourSpace::XYZ,
        ColourSpace::xyY,
        ColourSpace::Luv,
        ColourSpace::LCHuv,
        ColourSpace::Lab,
        ColourSpace::LCHab,
        ColourSpace::Adobe_RGB,
        ColourSpace::Apple_RGB,
        ColourSpace::Best_RGB,
        ColourSpace::Beta_RGB,
        ColourSpace::Bruce_RGB,
        ColourSpace::CIE_RGB,
        ColourSpace::ColorMatch_RGB,
        ColourSpace::Don_RGB_4,
        ColourSpace::ECI_RGB,
        ColourSpace::Ekta_Space_PS5,
        ColourSpace::NTSC_RGB,
        ColourSpace::PALSECAM_RGB,
        ColourSpace::ProPhoto_RGB,
        ColourSpace::SMPTEC_RGB,
        ColourSpace::sRGB,
        ColourSpace::Wide_Gamut_RGB
    };

    inline QString to_string(RefWhite rw)
    {
        switch (rw) {
            case RefWhite::A:       return {"A"};
            case RefWhite::B:       return {"B"};
            case RefWhite::C:       return {"C"};
            case RefWhite::D50:     return {"D50"};
            case RefWhite::D55:     return {"D55"};
            case RefWhite::D65:     return {"D65"};
            case RefWhite::D75:     return {"D75"};
            case RefWhite::E:       return {"E"};
            case RefWhite::F1:      return {"F1"};
            case RefWhite::F2:      return {"F2"};
            case RefWhite::F3:      return {"F3"};
            case RefWhite::F4:      return {"F4"};
            case RefWhite::F5:      return {"F5"};
            case RefWhite::F6:      return {"F6"};
            case RefWhite::F7:      return {"F7"};
            case RefWhite::F8:      return {"F8"};
            case RefWhite::F9:      return {"F9"};
            case RefWhite::F10:     return {"F10"};
            case RefWhite::F11:     return {"F11"};
            case RefWhite::F12:     return {"F12"};
            default:                return {"INVALID"};
        }
    }

    inline QString to_string(ColourSpace cs)
    {
        switch (cs) {
            case ColourSpace::XYZ:            return {"XYZ"};
            case ColourSpace::xyY:            return {"xyY"};
            case ColourSpace::Luv:            return {"Luv"};
            case ColourSpace::LCHuv:          return {"LCHuv"};
            case ColourSpace::Lab:            return {"Lab"};
            case ColourSpace::LCHab:          return {"LCHab"};
            case ColourSpace::Adobe_RGB:      return {"Adobe RGB"};
            case ColourSpace::Apple_RGB:      return {"Apple RGB"};
            case ColourSpace::Best_RGB:       return {"Best RGB"};
            case ColourSpace::Beta_RGB:       return {"Beta RGB"};
            case ColourSpace::Bruce_RGB:      return {"Bruce RGB"};
            case ColourSpace::CIE_RGB:        return {"CIE RGB"};
            case ColourSpace::ColorMatch_RGB: return {"ColorMatch RGB"};
            case ColourSpace::Don_RGB_4:      return {"Don RGB 4"};
            case ColourSpace::ECI_RGB:        return {"ECI RGB"};
            case ColourSpace::Ekta_Space_PS5: return {"Ekta Space PS5"};
            case ColourSpace::NTSC_RGB:       return {"NTSC RGB"};
            case ColourSpace::PALSECAM_RGB:   return {"PAL/SECAM RGB"};
            case ColourSpace::ProPhoto_RGB:   return {"ProPhoto RGB"};
            case ColourSpace::SMPTEC_RGB:     return {"SMPTE-C RGB"};
            case ColourSpace::sRGB:           return {"sRGB"};
            case ColourSpace::Wide_Gamut_RGB: return {"Wide Gamut RGB"};
            default:                          return {"INVALID"};
        }
    }
}