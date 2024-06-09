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


#pragma once

#include <cstdint>

#include <Eigen/Core>


using i32  = std::int32_t;
using f32  = float;
using f64  = double;
using real = f64;

using Point3d = Eigen::Vector3<f64>;


struct XYZ
{
    Point3d tri;
};

struct xyY
{
    Point3d tri;
};

struct Lab
{
    Point3d tri;
};

struct LCHab
{
    Point3d tri;
};

struct Luv
{
    Point3d tri;
};

struct LCHuv
{
    Point3d tri;
};

struct Illuminant : XYZ
{
};

struct RGB;

