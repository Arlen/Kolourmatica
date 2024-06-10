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


namespace km
{
    inline auto convert_to_Lab(const XYZ& col, const Illuminant& rw) -> Lab
    {
        const f64 third = f64(1) / f64(3);
        const f64 xr = col.tri[0] / rw.tri[0];
        const f64 yr = col.tri[1] / rw.tri[1];
        const f64 zr = col.tri[2] / rw.tri[2];

        f64 fx, fy, fz;
        if (xr > constants::_cie_epsilon)
            fx = pow(xr, third);
        else
            fx = (constants::_cie_kappa * xr + 16.0) / 116.0;

        if (yr > constants::_cie_epsilon)
            fy = pow(yr, third);
        else
            fy = (constants::_cie_kappa * yr + 16.0) / 116.0;

        if (zr > constants::_cie_epsilon)
            fz = pow(zr, third);
        else
            fz = (constants::_cie_kappa * zr + 16.0) / 116.0;

        Lab result;
        result.tri[0] = 116.0 * fy - 16.0;
        result.tri[1] = 500.0 * (fx - fy);
        result.tri[2] = 200.0 * (fy - fz);
        result.rw     = rw;

        return result;
    }

    inline auto convert_to_Lab(const xyY& col, const Illuminant& rw) -> Lab
    {
        return convert_to_Lab(convert_to_XYZ(col), rw);
    }

    inline auto convert_to_Lab(const LCHab& col) -> Lab
    {
        Lab result;

        result.tri[0] = col.tri[0];
        result.tri[1] = col.tri[1] * cos(col.tri[2] * constants::_radian);
        result.tri[2] = col.tri[1] * sin(col.tri[2] * constants::_radian);
        result.rw     = col.rw;

        return result;
    }

    inline auto convert_to_Lab(const Luv& col, const Illuminant& rw) -> Lab
    {
        return convert_to_Lab(convert_to_XYZ(col), rw);
    }

    inline auto convert_to_Lab(const LCHuv& col, const Illuminant& rw) -> Lab
    {
        return convert_to_Lab(convert_to_XYZ(col), rw);
    }

    inline auto convert_to_Lab(const RGB& col, const Illuminant& rw) -> Lab
    {
        return convert_to_Lab(convert_to_XYZ(col), rw);
    }
}