/***********************************************************************
|*  Copyright (C) 2011, 2024 Arlen Avakian
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

#include <ColourSpace.hpp>
#include <enums.hpp>
#include <types.hpp>


namespace km::internal
{
    /// convert reference white xyY(x,y,1) to type km::Illuminant.
    inline auto as_illuminant(f64 x, f64 y) -> Illuminant
    {
        xyY rw;
        rw.tri = {x, y, 1};

        return static_cast<Illuminant>(convert_to_XYZ(rw));
    }
}

namespace km
{
    auto A_1931_2() -> Illuminant;
    auto A_1964_10() -> Illuminant;
    
    auto B_1931_2() -> Illuminant;
    auto B_1964_10() -> Illuminant;
    
    auto C_1931_2() -> Illuminant;
    auto C_1964_10() -> Illuminant;
    
    auto D50_1931_2() -> Illuminant;
    auto D50_1964_10() -> Illuminant;
    
    auto D55_1931_2() -> Illuminant;
    auto D55_1964_10() -> Illuminant;
    
    auto D65_1931_2() -> Illuminant;
    auto D65_1964_10() -> Illuminant;
    
    auto D75_1931_2() -> Illuminant;
    auto D75_1964_10() -> Illuminant;
    
    auto E_1931_2() -> Illuminant;
    auto E_1964_10() -> Illuminant;
    
    auto F1_1931_2() -> Illuminant;
    auto F1_1964_10() -> Illuminant;
    
    auto F2_1931_2() -> Illuminant;
    auto F2_1964_10() -> Illuminant;
    
    auto F3_1931_2() -> Illuminant;
    auto F3_1964_10() -> Illuminant;
    
    auto F4_1931_2() -> Illuminant;
    auto F4_1964_10() -> Illuminant;
    
    auto F5_1931_2() -> Illuminant;
    auto F5_1964_10() -> Illuminant;
    
    auto F6_1931_2() -> Illuminant;
    auto F6_1964_10() -> Illuminant;
    
    auto F7_1931_2() -> Illuminant;
    auto F7_1964_10() -> Illuminant;
    
    auto F8_1931_2() -> Illuminant;
    auto F8_1964_10() -> Illuminant;
    
    auto F9_1931_2() -> Illuminant;
    auto F9_1964_10() -> Illuminant;
    
    auto F10_1931_2() -> Illuminant;
    auto F10_1964_10() -> Illuminant;
    
    auto F11_1931_2() -> Illuminant;
    auto F11_1964_10() -> Illuminant;
    
    auto F12_1931_2() -> Illuminant;
    auto F12_1964_10() -> Illuminant;

    auto get_ref_white(enums::RefWhite rw, enums::RefWhiteFov fov) -> Illuminant;
}