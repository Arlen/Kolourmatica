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


#include <ColourSpace.hpp>


auto km::convert_to_Luv(const XYZ& col, const Illuminant& rw) -> Luv
{
    const f64 yr = col.tri[1] / rw.tri[1];
    f64 up, vp, urp, vrp, L;
    ComputeUoVo(up, vp, col.tri);
    ComputeUoVo(urp, vrp, rw.tri);

    if (yr > constants::_cie_epsilon)
        L = 116.0 * pow(yr, 1.0/3.0) - 16.0;
    else
        L = constants::_cie_kappa * yr;

    const f64 u = 13.0 * L * (up - urp);
    const f64 v = 13.0 * L * (vp - vrp);

    Luv result;
    result.tri[0] = L;
    result.tri[1] = u;
    result.tri[2] = v;
    result.rw     = rw;

    return result;
}

auto km::convert_to_Luv(const xyY& col, const Illuminant& rw) -> Luv
{
    return convert_to_Luv(convert_to_XYZ(col), rw);
}

auto km::convert_to_Luv(const Lab& col, const Illuminant& rw) -> Luv
{
    return convert_to_Luv(convert_to_XYZ(col), rw);
}

auto km::convert_to_Luv(const LCHab& col, const Illuminant& rw) -> Luv
{
    return convert_to_Luv(convert_to_XYZ(col), rw);
}

auto km::convert_to_Luv(const LCHuv& col) -> Luv
{
    Luv result;
    result.tri[0] = col.tri[0];
    result.tri[1] = col.tri[1] * cos(col.tri[2] * constants::_radian);
    result.tri[2] = col.tri[1] * sin(col.tri[2] * constants::_radian);
    result.rw     = col.rw;

    return result;
}

auto km::convert_to_Luv(const RGB& col, const Illuminant& rw) -> Luv
{
    return convert_to_Luv(convert_to_XYZ(col), rw);
}