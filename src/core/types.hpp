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

#include <cstdint>

#include <Eigen/Core>
#include <Eigen/Dense>


using i32  = std::int32_t;
using u32  = std::uint32_t;
using f32  = float;
using f64  = double;
using real = f64;

using Point3d = Eigen::Vector3<f64>;
using Point2d = Eigen::Vector2<f64>;

inline Point3d nan_tri()
{
    return {NAN, NAN, NAN};
}


namespace km
{
    /// forward declarations
    struct XYZ;
    struct xyY;
    struct Lab;
    struct LCHab;
    struct Luv;
    struct LCHuv;
    struct Illuminant;
    struct RGB;
    struct Colour_AdobeRGB;
    struct Colour_AppleRGB;
    struct Colour_BestRGB;
    struct Colour_BetaRGB;
    struct Colour_BruceRGB;
    struct Colour_CIERGB;
    struct Colour_ColorMatchRGB;
    struct Colour_DonRGB4;
    struct Colour_ECIRGB;
    struct Colour_EktaSpacePS5;
    struct Colour_NTSCRGB;
    struct Colour_PAL_SECAMRGB;
    struct Colour_ProPhotoRGB;
    struct Colour_SMPTE_CRGB;
    struct Colour_sRGB;
    struct Colour_WideGamutRGB;

    /// types
    struct XYZ
    {
        Point3d tri;
    };

    struct xyY
    {
        Point3d tri;
    };

    struct Illuminant : XYZ
    {

    };

    struct Lab
    {
        Point3d tri;
        Illuminant rw;
    };

    struct LCHab
    {
        Point3d tri;
        Illuminant rw;
    };

    struct Luv
    {
        Point3d tri;
        Illuminant rw;
    };

    struct LCHuv
    {
        Point3d tri;
        Illuminant rw;
    };

    struct BBox
    {
        Point3d min{1,1,1};
        Point3d max{0,0,0};
    };

    struct Segment
    {
        Point3d a;
        Point3d b;
    };
}