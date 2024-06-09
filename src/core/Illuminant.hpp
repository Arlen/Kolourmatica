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

#include <types.hpp>
#include <Colour_XYZ.hpp>


namespace km::internal
{
    inline auto as_RefWhite(f64 x, f64 y) -> Illuminant
    {
        xyY rw;
        rw.tri = {x, y, 1};

        return static_cast<Illuminant>(to_XYZ(rw));
    }
}

static const Illuminant A_1964_10   = km::internal::as_RefWhite(0.45117, 0.40594);
static const Illuminant A_1931_2    = km::internal::as_RefWhite(0.44757, 0.40745);

static const Illuminant B_1964_10   = km::internal::as_RefWhite(0.3498, 0.3527);
static const Illuminant B_1931_2    = km::internal::as_RefWhite(0.34842, 0.35161);

static const Illuminant C_1964_10   = km::internal::as_RefWhite(0.31039, 0.31905);
static const Illuminant C_1931_2    = km::internal::as_RefWhite(0.31006, 0.31616);

static const Illuminant D50_1964_10 = km::internal::as_RefWhite(0.34773, 0.35952);
static const Illuminant D50_1931_2  = km::internal::as_RefWhite(0.34567, 0.3585);

static const Illuminant D55_1964_10 = km::internal::as_RefWhite(0.33411, 0.34877);
static const Illuminant D55_1931_2  = km::internal::as_RefWhite(0.33242, 0.34743);

static const Illuminant D65_1964_10 = km::internal::as_RefWhite(0.31382, 0.331);
static const Illuminant D65_1931_2  = km::internal::as_RefWhite(0.31271, 0.32902);

static const Illuminant D75_1964_10 = km::internal::as_RefWhite(0.29968, 0.3174);
static const Illuminant D75_1931_2  = km::internal::as_RefWhite(0.29902, 0.31485);

static const Illuminant E_1964_10   = km::internal::as_RefWhite(f64(1) / f64(3), f64(1) / f64(3));
static const Illuminant E_1931_2    = km::internal::as_RefWhite(f64(1) / f64(3), f64(1) / f64(3));

static const Illuminant F1_1964_10  = km::internal::as_RefWhite(0.31811, 0.33559);
static const Illuminant F1_1931_2   = km::internal::as_RefWhite(0.3131, 0.33727);

static const Illuminant F2_1964_10  = km::internal::as_RefWhite(0.37925, 0.36733);
static const Illuminant F2_1931_2   = km::internal::as_RefWhite(0.37208, 0.37529);

static const Illuminant F3_1964_10  = km::internal::as_RefWhite(0.41761, 0.38324);
static const Illuminant F3_1931_2   = km::internal::as_RefWhite(0.4091, 0.3943);

static const Illuminant F4_1964_10  = km::internal::as_RefWhite(0.4492, 0.39074);
static const Illuminant F4_1931_2   = km::internal::as_RefWhite(0.44018, 0.40329);

static const Illuminant F5_1964_10  = km::internal::as_RefWhite(0.31975, 0.34246);
static const Illuminant F5_1931_2   = km::internal::as_RefWhite(0.31379, 0.34531);

static const Illuminant F6_1964_10  = km::internal::as_RefWhite(0.3866, 0.37847);
static const Illuminant F6_1931_2   = km::internal::as_RefWhite(0.3779, 0.38835);

static const Illuminant F7_1964_10  = km::internal::as_RefWhite(0.31569, 0.3296);
static const Illuminant F7_1931_2   = km::internal::as_RefWhite(0.31292, 0.32933);

static const Illuminant F8_1964_10  = km::internal::as_RefWhite(0.34902, 0.35939);
static const Illuminant F8_1931_2   = km::internal::as_RefWhite(0.34588, 0.35875);

static const Illuminant F9_1964_10  = km::internal::as_RefWhite(0.37829, 0.37045);
static const Illuminant F9_1931_2   = km::internal::as_RefWhite(0.37417, 0.37281);

static const Illuminant F10_1964_10 = km::internal::as_RefWhite(0.3509, 0.35444);
static const Illuminant F10_1931_2  = km::internal::as_RefWhite(0.34609, 0.35986);

static const Illuminant F11_1964_10 = km::internal::as_RefWhite(0.38541, 0.37123);
static const Illuminant F11_1931_2  = km::internal::as_RefWhite(0.38052, 0.37713);

static const Illuminant F12_1964_10 = km::internal::as_RefWhite(0.44256, 0.39717);
static const Illuminant F12_1931_2  = km::internal::as_RefWhite(0.43695, 0.40441);
