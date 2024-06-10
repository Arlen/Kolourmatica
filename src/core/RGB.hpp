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


#pragma once

#include <types.hpp>
#include <ColourSpace.hpp>

#include <functional>


namespace km
{
    /// used when converting XYZ to RGB
    void gammaCompanding(f64 gamma, Point3d &coords);

    /// used when converting RGB to XYZ
    Point3d inverseGammaCompanding(f64 gamma, const Point3d &coords);

    /// used when converting XYZ to sRGB
    void sRGB_Companding(f64 gamma, Point3d &coords);

    /// used when converting sRGB to XYZ
    Point3d inverse_sRGB_Companding(f64 gamma, const Point3d &coords);
}


namespace km
{
    struct RGB
    {
        using Companding        = std::function<void (f64, Point3d &)>;
        using InverseCompanding = std::function<Point3d (f64, const Point3d &)>;

        Point3d tri;

        [[nodiscard]] const f64 &gamma() const;

        [[nodiscard]] const Eigen::Matrix3d &m() const;

        [[nodiscard]] const Eigen::Matrix3d &m_1() const;

        [[nodiscard]] const Illuminant &referenceWhite() const;

        [[nodiscard]] Point3d inverseCompanding(f64 gamma, const Point3d &coords) const;

        RGB &from(const XYZ &col);

        RGB &from(const xyY &col);

        RGB &from(const Lab &col);

        RGB &from(const LCHab &col);

        RGB &from(const Luv &col);

        RGB &from(const LCHuv &col);

        RGB(const RGB&) = default;

    protected:
        RGB(const Illuminant &rw,
            const f64 gamma,
            const xyY &redPrimary,
            const xyY &greenPrimary,
            const xyY &bluePrimary,
            const Point3d &tri)
            : tri(tri)
            , _gamma(gamma)
            , _m(computeConversionMatrix(redPrimary, greenPrimary, bluePrimary, rw))
            , _m_1(_m.inverse())
            , _companding(Companding(gammaCompanding))
            , _inv_companding(InverseCompanding(inverseGammaCompanding))
        {
            _default_rw = rw;
        }

        /// so far only used by sRGB to initialize the companding functions.
        RGB(const Illuminant &rw,
            const f64 gamma,
            const xyY &redPrimary,
            const xyY &greenPrimary,
            const xyY &bluePrimary,
            const Companding &c,
            const InverseCompanding &ic,
            const Point3d &tri)
            : tri(tri)
            , _gamma(gamma)
            , _m(computeConversionMatrix(redPrimary, greenPrimary, bluePrimary, rw))
            , _m_1(_m.inverse())
            , _companding(c)
            , _inv_companding(ic)
        {
            _default_rw = rw;
        }

    private:
        /// computes the conversion matrix from RGB to XYZ
        static Eigen::Matrix3d
        computeConversionMatrix(const xyY &redPrimary,
                                const xyY &greenPrimary,
                                const xyY &bluePrimary,
                                const Illuminant &rw)
        {
            Eigen::Matrix3<f64> xyzs;
            Eigen::Matrix3<f64> M;
            Point3d S;
            xyzs.col(0) = convert_to_XYZ(redPrimary).tri;
            xyzs.col(1) = convert_to_XYZ(greenPrimary).tri;
            xyzs.col(2) = convert_to_XYZ(bluePrimary).tri;
            S = xyzs.inverse() * rw.tri;
            M.col(0) = S(0) * xyzs.col(0);
            M.col(1) = S(1) * xyzs.col(1);
            M.col(2) = S(2) * xyzs.col(2);
            return M;
        }

    protected:
        Illuminant _default_rw;
        f64 _gamma;
        const Eigen::Matrix3d _m;
        const Eigen::Matrix3d _m_1;
        const Companding _companding;
        const InverseCompanding _inv_companding;
    };

    inline auto as_xyY(f64 x, f64 y) -> xyY
    {
        xyY result;
        result.tri = {x, y, 1};

        return result;
    }
}


#define DECLARE_RGB_TYPE(NAME)                                      \
    struct Colour_##NAME : public RGB {                             \
        explicit Colour_##NAME(f64 r = 1, f64 g = 1, f64 b = 1);    \
        explicit Colour_##NAME(const Point3d& tri);                 \
        Colour_##NAME& operator=(const Colour_##NAME& other);};     \

namespace km
{
    DECLARE_RGB_TYPE(AdobeRGB)
    DECLARE_RGB_TYPE(AppleRGB)
    DECLARE_RGB_TYPE(BestRGB)
    DECLARE_RGB_TYPE(BetaRGB)
    DECLARE_RGB_TYPE(BruceRGB)
    DECLARE_RGB_TYPE(CIERGB)
    DECLARE_RGB_TYPE(ColorMatchRGB)
    DECLARE_RGB_TYPE(DonRGB4)
    DECLARE_RGB_TYPE(ECIRGB)
    DECLARE_RGB_TYPE(EktaSpacePS5)
    DECLARE_RGB_TYPE(NTSCRGB)
    DECLARE_RGB_TYPE(PAL_SECAMRGB)
    DECLARE_RGB_TYPE(ProPhotoRGB)
    DECLARE_RGB_TYPE(SMPTE_CRGB)
    DECLARE_RGB_TYPE(WideGamutRGB)
}

#undef DECLARE_RGB_TYPE


namespace km
{
    struct Colour_sRGB : RGB
    {
        explicit Colour_sRGB(f64 r = 1, f64 g = 1, f64 b = 1);
        explicit Colour_sRGB(const Point3d& tri);
        Colour_sRGB& operator=(const Colour_sRGB& other);
    };
}