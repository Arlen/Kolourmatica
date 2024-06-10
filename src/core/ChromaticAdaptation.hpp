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

#include <Illuminant.hpp>


namespace km::internal
{
    Eigen::Matrix3d
    chromaticAdaptation(const Illuminant &src, const Illuminant &tar, const Eigen::Matrix3d &m);
}

namespace km
{
    struct ChromaticAdaptation
    {
        virtual ~ChromaticAdaptation() = default;
        virtual Eigen::Matrix3d apply(const Illuminant& source, const Illuminant& target) = 0;
    };

    struct XYZScalingCA final : ChromaticAdaptation
    {
        Eigen::Matrix3d apply(const Illuminant& source, const Illuminant& target) override;
    };

    struct VonKriesCA final : ChromaticAdaptation
    {
        Eigen::Matrix3d apply(const Illuminant& source, const Illuminant& target) override;
    };

    struct BradfordCA final : ChromaticAdaptation
    {
        Eigen::Matrix3d apply(const Illuminant& source, const Illuminant& target) override;
    };

    struct BypassCA final : ChromaticAdaptation
    {
        Eigen::Matrix3d apply(const Illuminant& source, const Illuminant& target) override;
    };
}