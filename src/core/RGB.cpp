/***********************************************************************
|*  Copyright (C) 2010, 2011, 2024 Arlen Avakian
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


#include <RGB.hpp>
#include <Illuminant.hpp>


using namespace km;

void km::gammaCompanding(f64 gamma, Point3d &coords)
{
    f64 p = f64(1) / gamma;

    if (coords(0) < 0.0)
        coords(0) = pow(-coords(0), p) * -1.0;
    else
        coords(0) = pow(coords(0), p);

    if (coords(1) < 0.0)
        coords(1) = pow(-coords(1), p) * -1.0;
    else
        coords(1) = pow(coords(1), p);

    if (coords(2) < 0.0)
        coords(2) = pow(-coords(2), p) * -1.0;
    else
        coords(2) = pow(coords(2), p);
}

Point3d km::inverseGammaCompanding(f64 gamma, const Point3d &coords)
{
    Point3d tri;

    if (coords(0) < 0.0)
        tri(0) = pow(-coords(0), gamma) * -1.0;
    else
        tri(0) = pow(coords(0), gamma);

    if (coords(1) < 0.0)
        tri(1) = pow(-coords(1), gamma) * -1.0;
    else
        tri(1) = pow(coords(1), gamma);

    if (coords(2) < 0.0)
        tri(2) = pow(-coords(2), gamma) * -1.0;
    else
        tri(2) = pow(coords(2), gamma);

    return tri;
}

void km::sRGB_Companding(f64 gamma, Point3d& coords)
{
    const f64 p = f64(1) / gamma;

    if (coords(0) > 0.0031308)
        coords(0) = 1.055 * pow(coords(0), p) - 0.055;
    else
        coords(0) = 12.92 * coords(0);

    if (coords(1) > 0.0031308)
        coords(1) = 1.055 * pow(coords(1), p) - 0.055;
    else
        coords(1) = 12.92 * coords(1);

    if (coords(2) > 0.0031308)
        coords(2) = 1.055 * pow(coords(2), p) - 0.055;
    else
        coords(2) = 12.92 * coords(2);
}

Point3d km::inverse_sRGB_Companding(f64 gamma, const Point3d& coords)
{
    Point3d tri;

    if (coords(0) > 0.04045)
        tri(0) = pow( ((coords(0) + 0.055) / 1.055), gamma );
    else
        tri(0) = coords(0) / 12.92;

    if (coords(1) > 0.04045)
        tri(1) = pow( ((coords(1) + 0.055) / 1.055), gamma );
    else
        tri(1) = coords(1) / 12.92;

    if (coords(2) > 0.04045)
        tri(2) = pow( ((coords(2) + 0.055) / 1.055), gamma );
    else
        tri(2) = coords(2) / 12.92;

    return tri;
}


/// RGB colour model.
[[nodiscard]] const f64 &RGB::gamma() const { return _gamma; }

[[nodiscard]] const Eigen::Matrix3d &RGB::m() const { return _m; }

[[nodiscard]] const Eigen::Matrix3d &RGB::m_1() const { return _m_1; }

[[nodiscard]] const Illuminant &RGB::referenceWhite() const { return _default_rw; }

[[nodiscard]] Point3d RGB::inverseCompanding(f64 gamma, const Point3d &coords) const
{
    return _inv_companding(gamma, coords);
}

RGB &RGB::from(const XYZ &col)
{
    tri = _m_1 * col.tri;
    _companding(_gamma, tri);

    return *this;
}

RGB &RGB::from(const xyY &col)
{
    return from(convert_to_XYZ(col));
}

RGB &RGB::from(const Lab &col)
{
    return from(convert_to_XYZ(col));
}

RGB &RGB::from(const LCHab &col)
{
    return from(convert_to_XYZ(col));
}

RGB &RGB::from(const Luv &col)
{
    return from(convert_to_XYZ(col));
}

RGB &RGB::from(const LCHuv &col)
{
    return from(convert_to_XYZ(col));
}


/// sRGB colour space.
Colour_sRGB::Colour_sRGB(f64 r, f64 g, f64 b)
    : RGB(D65_1931_2()
        , 2.4
        , as_xyY(0.64, 0.33)
        , as_xyY(0.30, 0.60)
        , as_xyY(0.15, 0.06)
        , Companding(sRGB_Companding)
        , InverseCompanding(inverse_sRGB_Companding)
        , {r, g, b})
{ }

Colour_sRGB::Colour_sRGB(const Point3d& tri)
    : Colour_sRGB(tri[0], tri[1], tri[2])
{ }

Colour_sRGB& Colour_sRGB::operator=(const Colour_sRGB& other)
{
    tri = other.tri;
    return *this;
}


/// other RGB colour spaces.
#define GET_X_(x, y) x
#define GET_X(P) GET_X_ P
#define GET_Y_(x, y) y
#define GET_Y(P) GET_Y_ P

#define RED_PRIMARY(x, y) (x, y)
#define GREEN_PRIMARY(x, y) (x, y)
#define BLUE_PRIMARY(x, y) (x, y)

#define DEFINE_RGB_TYPE(NAME, RW, GAMMA, RED, GREEN, BLUE)                      \
        namespace km {                                                          \
        Colour_##NAME::Colour_##NAME (f64 r, f64 g, f64 b )                     \
            : RGB( RW()                                                         \
                 , GAMMA                                                        \
                 , as_xyY(GET_X(RED),   GET_Y(RED))                             \
                 , as_xyY(GET_X(GREEN), GET_Y(GREEN))                           \
                 , as_xyY(GET_X(BLUE),  GET_Y(BLUE))                            \
                 , {r, g, b}) {}                                                \
        Colour_##NAME::Colour_##NAME(const Point3d& tri)                        \
            : Colour_##NAME(tri[0], tri[1], tri[2]) {}                          \
         Colour_##NAME& Colour_##NAME::operator=(const Colour_##NAME& other) {  \
            tri = other.tri;                                                    \
            return *this; }}                                                    \

DEFINE_RGB_TYPE(AdobeRGB,      D65_1931_2, 2.2, RED_PRIMARY(0.64, 0.33),     GREEN_PRIMARY(0.21, 0.71),     BLUE_PRIMARY(0.15, 0.06))
DEFINE_RGB_TYPE(AppleRGB,      D65_1931_2, 1.8, RED_PRIMARY(0.625, 0.340),   GREEN_PRIMARY(0.280, 0.595),   BLUE_PRIMARY(0.155, 0.070))
DEFINE_RGB_TYPE(BestRGB,       D50_1931_2, 2.2, RED_PRIMARY(0.7347, 0.2653), GREEN_PRIMARY(0.2150, 0.7750), BLUE_PRIMARY(0.1300, 0.0350))
DEFINE_RGB_TYPE(BetaRGB,       D50_1931_2, 2.2, RED_PRIMARY(0.6888, 0.3112), GREEN_PRIMARY(0.1986, 0.7551), BLUE_PRIMARY(0.1265, 0.0352))
DEFINE_RGB_TYPE(BruceRGB,      D65_1931_2, 2.2, RED_PRIMARY(0.64, 0.33),     GREEN_PRIMARY(0.28, 0.65),     BLUE_PRIMARY(0.15, 0.06))
DEFINE_RGB_TYPE(CIERGB,        E_1931_2,   2.2, RED_PRIMARY(0.735, 0.265),   GREEN_PRIMARY(0.274, 0.717),   BLUE_PRIMARY(0.167, 0.009))
DEFINE_RGB_TYPE(ColorMatchRGB, D50_1931_2, 1.8, RED_PRIMARY(0.630, 0.340),   GREEN_PRIMARY(0.295, 0.605),   BLUE_PRIMARY(0.150, 0.075))
DEFINE_RGB_TYPE(DonRGB4,       D50_1931_2, 2.2, RED_PRIMARY(0.696, 0.300),   GREEN_PRIMARY(0.215, 0.765),   BLUE_PRIMARY(0.130, 0.035))
DEFINE_RGB_TYPE(ECIRGB,        D50_1931_2, 1.8, RED_PRIMARY(0.67, 0.33),     GREEN_PRIMARY(0.21, 0.71),     BLUE_PRIMARY(0.14, 0.08))
DEFINE_RGB_TYPE(EktaSpacePS5,  D50_1931_2, 2.2, RED_PRIMARY(0.695, 0.305),   GREEN_PRIMARY(0.260, 0.700),   BLUE_PRIMARY(0.110, 0.005))
DEFINE_RGB_TYPE(NTSCRGB,       C_1931_2,   2.2, RED_PRIMARY(0.67, 0.33),     GREEN_PRIMARY(0.21, 0.71),     BLUE_PRIMARY(0.14, 0.08))
DEFINE_RGB_TYPE(PAL_SECAMRGB,  D65_1931_2, 2.2, RED_PRIMARY(0.64, 0.33),     GREEN_PRIMARY(0.29, 0.60),     BLUE_PRIMARY(0.15, 0.06))
DEFINE_RGB_TYPE(ProPhotoRGB,   D50_1931_2, 1.8, RED_PRIMARY(0.7347, 0.2653), GREEN_PRIMARY(0.1596, 0.8404), BLUE_PRIMARY(0.0366, 0.0001))
DEFINE_RGB_TYPE(SMPTE_CRGB,    D65_1931_2, 2.2, RED_PRIMARY(0.630, 0.340),   GREEN_PRIMARY(0.310, 0.595),   BLUE_PRIMARY(0.155, 0.070))
DEFINE_RGB_TYPE(WideGamutRGB,  D50_1931_2, 2.2, RED_PRIMARY(0.735, 0.265),   GREEN_PRIMARY(0.115, 0.826),   BLUE_PRIMARY(0.157, 0.018))

#undef DEFINE_RGB_TYPE
#undef BLUE_PRIMARY
#undef GREEN_PRIMARY
#undef RED_PRIMARY
#undef GET_Y
#undef GET_Y_
#undef GET_X
#undef GET_X_