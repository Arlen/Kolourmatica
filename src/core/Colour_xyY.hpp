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


namespace km
{
    inline auto convert_to_xyY(const XYZ& col) -> xyY
    {
        xyY result;
        const f64 sum = col.tri.sum();
        result.tri[0] = col.tri[0] / sum;
        result.tri[1] = col.tri[1] / sum;
        result.tri[2] = col.tri[1];

        return result;
    }

    inline auto convert_to_xyY(const Illuminant& rw) -> xyY
    {
        return convert_to_xyY(static_cast<XYZ>(rw));
    }

    inline auto convert_to_xyY(const Lab& col) -> xyY
    {
        return convert_to_xyY(convert_to_XYZ(col));
    }

    inline auto convert_to_xyY(const LCHab& col) -> xyY
    {
        return convert_to_xyY(convert_to_Lab(col));
    }

    inline auto convert_to_xyY(const Luv& col) -> xyY
    {
        return convert_to_xyY(convert_to_XYZ(col));
    }

    inline auto convert_to_xyY(const LCHuv& col) -> xyY
    {
        return convert_to_xyY(convert_to_Luv(col));
    }

    inline auto convert_to_xyY(const RGB& col) -> xyY
    {
        return convert_to_xyY(convert_to_XYZ(col));
    }
}