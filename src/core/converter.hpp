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


#include <enums.hpp>
#include <ColourSpace.hpp>
#include <RGB.hpp>

#include <boost/mp11.hpp>

#include <tuple>


namespace km
{
    struct Converter
    {
        std::tuple<
            std::tuple<enums::ColourSpace, XYZ>,
            std::tuple<enums::ColourSpace, xyY>,
            std::tuple<enums::ColourSpace, Luv>,
            std::tuple<enums::ColourSpace, LCHuv>,
            std::tuple<enums::ColourSpace, Lab>,
            std::tuple<enums::ColourSpace, LCHab>,
            std::tuple<enums::ColourSpace, Colour_AdobeRGB>,
            std::tuple<enums::ColourSpace, Colour_AppleRGB>,
            std::tuple<enums::ColourSpace, Colour_BestRGB>,
            std::tuple<enums::ColourSpace, Colour_BetaRGB>,
            std::tuple<enums::ColourSpace, Colour_BruceRGB>,
            std::tuple<enums::ColourSpace, Colour_CIERGB>,
            std::tuple<enums::ColourSpace, Colour_ColorMatchRGB>,
            std::tuple<enums::ColourSpace, Colour_DonRGB4>,
            std::tuple<enums::ColourSpace, Colour_ECIRGB>,
            std::tuple<enums::ColourSpace, Colour_EktaSpacePS5>,
            std::tuple<enums::ColourSpace, Colour_NTSCRGB>,
            std::tuple<enums::ColourSpace, Colour_PAL_SECAMRGB>,
            std::tuple<enums::ColourSpace, Colour_ProPhotoRGB>,
            std::tuple<enums::ColourSpace, Colour_SMPTE_CRGB>,
            std::tuple<enums::ColourSpace, Colour_sRGB>,
            std::tuple<enums::ColourSpace, Colour_WideGamutRGB>
            > lut;

        Converter()
        {
            lut = std::make_tuple(
                std::make_tuple(enums::ColourSpace::XYZ, XYZ()),
                std::make_tuple(enums::ColourSpace::xyY, xyY()),
                std::make_tuple(enums::ColourSpace::Luv, Luv()),
                std::make_tuple(enums::ColourSpace::LCHuv, LCHuv()),
                std::make_tuple(enums::ColourSpace::Lab, Lab()),
                std::make_tuple(enums::ColourSpace::LCHab, LCHab()),
                std::make_tuple(enums::ColourSpace::Adobe_RGB, Colour_AdobeRGB()),
                std::make_tuple(enums::ColourSpace::Apple_RGB, Colour_AppleRGB()),
                std::make_tuple(enums::ColourSpace::Best_RGB, Colour_BestRGB()),
                std::make_tuple(enums::ColourSpace::Beta_RGB, Colour_BetaRGB()),
                std::make_tuple(enums::ColourSpace::Bruce_RGB, Colour_BruceRGB()),
                std::make_tuple(enums::ColourSpace::CIE_RGB, Colour_CIERGB()),
                std::make_tuple(enums::ColourSpace::ColorMatch_RGB, Colour_ColorMatchRGB()),
                std::make_tuple(enums::ColourSpace::Don_RGB_4, Colour_DonRGB4()),
                std::make_tuple(enums::ColourSpace::ECI_RGB, Colour_ECIRGB()),
                std::make_tuple(enums::ColourSpace::Ekta_Space_PS5, Colour_EktaSpacePS5()),
                std::make_tuple(enums::ColourSpace::NTSC_RGB, Colour_NTSCRGB()),
                std::make_tuple(enums::ColourSpace::PALSECAM_RGB, Colour_PAL_SECAMRGB()),
                std::make_tuple(enums::ColourSpace::ProPhoto_RGB, Colour_ProPhotoRGB()),
                std::make_tuple(enums::ColourSpace::SMPTEC_RGB, Colour_SMPTE_CRGB()),
                std::make_tuple(enums::ColourSpace::sRGB, Colour_sRGB()),
                std::make_tuple(enums::ColourSpace::Wide_Gamut_RGB, Colour_WideGamutRGB()));
        }

        void set_colour_value(enums::ColourSpace cs_index, const Point3d &tri, const Illuminant &rw)
        {
            using namespace boost::mp11;

            /// TODO: 'template for' in C++26 has nicer syntax than mp11::tuple_for_each
            tuple_for_each(lut, [cs_index, &tri, &rw](auto &&x) {
                if (std::get<0>(x) == cs_index) {
                    std::get<1>(x).tri = tri;
                    if constexpr (requires { &std::get<1>(x).rw; }) {
                        std::get<1>(x).rw = rw;
                    }
                }
            });
        }

        XYZ to_XYZ(enums::ColourSpace source_index)
        {
            using namespace boost::mp11;
            XYZ result{nan_tri()};

            tuple_for_each(lut, [source_index, &result](auto &&x) {
                if (std::get<0>(x) == source_index) {
                    result.tri = convert_to_XYZ(std::get<1>(x)).tri;
                }
            });

            return result;
        }

        Point3d from_XYZ(enums::ColourSpace dest_index, const XYZ& xyz, const Illuminant& rw)
        {
            using namespace boost::mp11;

            if (dest_index == enums::ColourSpace::XYZ)   { return xyz.tri; }
            if (dest_index == enums::ColourSpace::xyY)   { return convert_to_xyY(xyz).tri; }
            if (dest_index == enums::ColourSpace::Luv)   { return convert_to_Luv(xyz, rw).tri; }
            if (dest_index == enums::ColourSpace::LCHuv) { return convert_to_LCHuv(xyz, rw).tri; }
            if (dest_index == enums::ColourSpace::Lab)   { return convert_to_Lab(xyz, rw).tri; }
            if (dest_index == enums::ColourSpace::LCHab) { return convert_to_LCHab(xyz, rw).tri; }

            Point3d result{nan_tri()};

            tuple_for_each(lut, [dest_index, &result, &xyz](auto &&x) {
                if (std::get<0>(x) == dest_index) {
                    if constexpr (requires { std::get<1>(x).from(xyz); }) {
                        result = std::get<1>(x).from(xyz).tri;
                    }
                }
            });

            return result;
        }

        /// returns the defined illuminant of an RGB colour space.
        auto get_rgb_illuminant(enums::ColourSpace index) -> Illuminant
        {
            using namespace boost::mp11;

            Illuminant result{nan_tri()};

            switch (index) {
                using namespace enums;
                /// only RGB colour spaces have reference whites in their spec.
                case ColourSpace::XYZ:
                case ColourSpace::xyY:
                case ColourSpace::Luv:
                case ColourSpace::LCHuv:
                case ColourSpace::Lab:
                case ColourSpace::LCHab:
                    return result;
                default: {
                    tuple_for_each(lut, [index, &result](auto &&x) {
                        if (std::get<0>(x) == index) {
                            if constexpr (requires { &std::get<1>(x).rw; }) {
                                result = std::get<1>(x).rw;
                            }
                        }
                    });
                    return result;
                }
            }
        }
    };

}


























