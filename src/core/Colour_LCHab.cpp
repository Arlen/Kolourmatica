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


auto km::convert_to_LCHab(const XYZ& col, const Illuminant& rw) -> LCHab
{
    return convert_to_LCHab(convert_to_Lab(col, rw));
}

auto km::convert_to_LCHab(const xyY& col, const Illuminant& rw) -> LCHab
{
    return convert_to_LCHab(convert_to_Lab(col, rw));
}

auto km::convert_to_LCHab(const Lab& col) -> LCHab
{
    const f64 L = col.tri[0];
    const f64 C = sqrt(col.tri[1] * col.tri[1] + col.tri[2] * col.tri[2]);
    const f64 h = atan2(col.tri[2], col.tri[1]) * constants::_angle;
    const f64 H = h < f64(0) ? h + f64(360) : h - f64(360);

    LCHab result;
    result.tri[0] = L;
    result.tri[1] = C;
    result.tri[2] = H;
    result.rw     = col.rw;

    return result;
}

auto km::convert_to_LCHab(const Luv& col, const Illuminant& rw) -> LCHab
{
    return convert_to_LCHab(convert_to_Lab(col, rw));
}

auto km::convert_to_LCHab(const LCHuv& col, const Illuminant& rw) -> LCHab
{
    return convert_to_LCHab(convert_to_Lab(col, rw));
}

auto km::convert_to_LCHab(const RGB& col, const Illuminant& rw) -> LCHab
{
    return convert_to_LCHab(convert_to_Lab(col, rw));
}