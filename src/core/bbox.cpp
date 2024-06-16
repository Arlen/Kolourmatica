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


#include <types.hpp>


namespace km
{
    bool contains(const BBox& bb, const Point3d& p)
    {
        const bool contains_x = p[0] <= bb.max[0] && p[0] >= bb.min[0];
        const bool contains_y = p[1] <= bb.max[1] && p[1] >= bb.min[1];
        const bool contains_z = p[2] <= bb.max[2] && p[2] >= bb.min[2];

        return contains_x && contains_y && contains_z;
    }

    void expand(BBox& bb, const Point3d& p)
    {
        if (p[0] > bb.max[0]) { bb.max[0] = p[0]; }
        if (p[1] > bb.max[1]) { bb.max[1] = p[1]; }
        if (p[2] > bb.max[2]) { bb.max[2] = p[2]; }

        if (p[0] < bb.min[0]) { bb.min[0] = p[0]; }
        if (p[1] < bb.min[1]) { bb.min[1] = p[1]; }
        if (p[2] < bb.min[2]) { bb.min[2] = p[2]; }
    }
}