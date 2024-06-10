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


namespace constants
{
    constexpr static f64 _116_inv       = f64(1) / f64(116);
    constexpr static f64 _200_inv       = f64(1) / f64(200);
    constexpr static f64 _500_inv       = f64(1) / f64(500);
    constexpr static f64 _angle         = f64(180) / std::numbers::pi;
    constexpr static f64 _cie_epsilon   = f64(216) / f64(24389);
    constexpr static f64 _cie_kappa     = f64(24389) / f64(27);
    constexpr static f64 _cie_kappa_inv = f64(1) / _cie_kappa;
    constexpr static f64 _cie_ke        = _cie_kappa * _cie_epsilon;
    constexpr static f64 _pi            = std::numbers::pi;
    constexpr static f64 _radian        = std::numbers::pi / f64(180);
};

namespace km
{
    auto convert_to_XYZ(const xyY&) -> XYZ;
    auto convert_to_XYZ(const Lab&) -> XYZ;
    auto convert_to_XYZ(const LCHab&) -> XYZ;
    auto convert_to_XYZ(const Luv&) -> XYZ;
    auto convert_to_XYZ(const LCHuv&) -> XYZ;
    auto convert_to_XYZ(const RGB&) -> XYZ;


    auto convert_to_xyY(const XYZ& col) -> xyY;
    auto convert_to_xyY(const Illuminant& rw) -> xyY;
    auto convert_to_xyY(const Lab& col) -> xyY;
    auto convert_to_xyY(const LCHab& col) -> xyY;
    auto convert_to_xyY(const Luv& col) -> xyY;
    auto convert_to_xyY(const LCHuv& col) -> xyY;
    auto convert_to_xyY(const RGB& col) -> xyY;


    auto convert_to_Lab(const XYZ& col, const Illuminant& rw) -> Lab;
    auto convert_to_Lab(const xyY& col, const Illuminant& rw) -> Lab;
    auto convert_to_Lab(const LCHab& col) -> Lab;
    auto convert_to_Lab(const Luv& col, const Illuminant& rw) -> Lab;
    auto convert_to_Lab(const LCHuv& col, const Illuminant& rw) -> Lab;
    auto convert_to_Lab(const RGB& col, const Illuminant& rw) -> Lab;


    auto convert_to_LCHab(const XYZ& col, const Illuminant& rw) -> LCHab;
    auto convert_to_LCHab(const xyY& col, const Illuminant& rw) -> LCHab;
    auto convert_to_LCHab(const Lab& col) -> LCHab;
    auto convert_to_LCHab(const Luv& col, const Illuminant& rw) -> LCHab;
    auto convert_to_LCHab(const LCHuv& col, const Illuminant& rw) -> LCHab;
    auto convert_to_LCHab(const RGB& col, const Illuminant& rw) -> LCHab;


    auto convert_to_Luv(const XYZ& col, const Illuminant& rw) -> Luv;
    auto convert_to_Luv(const xyY& col, const Illuminant& rw) -> Luv;
    auto convert_to_Luv(const Lab& col, const Illuminant& rw) -> Luv;
    auto convert_to_Luv(const LCHab& col, const Illuminant& rw) -> Luv;
    auto convert_to_Luv(const LCHuv& col) -> Luv;
    auto convert_to_Luv(const RGB& col, const Illuminant& rw) -> Luv;


    auto convert_to_LCHuv(const XYZ& col, const Illuminant& rw) -> LCHuv;
    auto convert_to_LCHuv(const xyY& col, const Illuminant& rw) -> LCHuv;
    auto convert_to_LCHuv(const Lab& col, const Illuminant& rw) -> LCHuv;
    auto convert_to_LCHuv(const LCHab& col, const Illuminant& rw) -> LCHuv;
    auto convert_to_LCHuv(const Luv& col) -> LCHuv;
    auto convert_to_LCHuv(const RGB& col, const Illuminant& rw) -> LCHuv;
}


inline void ComputeUoVo(f64 &uo, f64 &vo, const Point3d &rw)
{
    f64 d = f64(1) / (rw(0) + f64(15) * rw(1) + f64(3) * rw(2));
    uo = f64(4) * rw(0) * d;
    vo = f64(9) * rw(1) * d;
}
