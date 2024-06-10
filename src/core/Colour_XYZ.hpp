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
#include <RGB.hpp>


namespace km
{
    inline auto convert_to_XYZ(const xyY &col) -> XYZ
    {
        /// X = (x * Y) / y
        /// Y = Y
        /// Z = ((1 - x - y) * Y) / y

        const f64 y_inv = f64(1) / col.tri[1];
        XYZ result;
        result.tri[0] = col.tri[0] * col.tri[2] * y_inv;
        result.tri[1] = col.tri[2];
        result.tri[2] = (f64(1) - col.tri[0] - col.tri[1]) * col.tri[2] * y_inv;

        return result;
    }

    inline auto convert_to_XYZ(const Lab &col) -> XYZ
    {
        f64 fx, fy, fz, fxCube, fzCube, xr, yr, zr;
        fy = (col.tri[0] + 16.0) * constants::_116_inv;
        fx = (col.tri[1] / 500.0) + fy;
        fz = fy - (col.tri[2] / 200.0);
        fxCube = fx * fx * fx;
        fzCube = fz * fz * fz;

        if (fxCube > constants::_cie_epsilon)
            xr = fxCube;
        else
            xr = ((116.0 * fx) - 16.0) / constants::_cie_kappa;

        if (col.tri[0] > constants::_cie_ke) {
            f64 tmp = (col.tri[0] + 16.0) * constants::_116_inv;
            yr = tmp * tmp * tmp;
        } else {
            yr = col.tri[0] / constants::_cie_kappa;
        }

        if (fzCube > constants::_cie_epsilon)
            zr = fzCube;
        else
            zr = ((116.0 * fz) - 16.0) / constants::_cie_kappa;

        XYZ result;
        result.tri[0] = xr * col.rw.tri[0];
        result.tri[1] = yr * col.rw.tri[1];
        result.tri[2] = zr * col.rw.tri[2];

        return result;
    }

    inline auto convert_to_XYZ(const LCHab& col) -> XYZ
    {
        return convert_to_XYZ(convert_to_Lab(col));
    }

    inline auto convert_to_XYZ(const Luv& col) -> XYZ
    {
        f64 a, b, c, d, uo, vo, x, y, z;
        ComputeUoVo(uo, vo, col.rw.tri);

        c = -1.0 / 3.0;
        a = (((52.0 * col.tri[0]) /
          (col.tri[1] + 13.0 * col.tri[0] * uo)) - 1.0) / 3.0;
        if( col.tri[0] > constants::_cie_ke )
            y = pow( (col.tri[0] + 16.0) * constants::_116_inv, 3.0 );
        else
            y = col.tri[0] * constants::_cie_kappa_inv;

        b = -5.0 * y;
        d = (((39.0 * col.tri[0]) /
          (col.tri[2] + 13.0 * col.tri[0] * vo)) - 5.0) * y;
        x = (d - b) / (a - c);
        z = x * a + b;

        XYZ result;
        result.tri[0] = x;
        result.tri[1] = y;
        result.tri[2] = z;

        return result;
    }

    inline auto convert_to_XYZ(const LCHuv& col) -> XYZ
    {
        return convert_to_XYZ(convert_to_Luv(col));
    }

    inline auto convert_to_XYZ(const RGB& col) -> XYZ
    {
        Point3d tri = col.inverseCompanding(col.gamma(), col.tri);
        XYZ result;
        result.tri = col.m() * tri;

        return result;
    }
}
