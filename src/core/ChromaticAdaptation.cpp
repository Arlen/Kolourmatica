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


#include <ChromaticAdaptation.hpp>


using namespace km;

Eigen::Matrix3d
internal::chromaticAdaptation(const Illuminant &src, const Illuminant &tar, const Eigen::Matrix3d &m)
{
    using Eigen::Matrix3d;

    Point3d S = m * src.tri;
    Point3d D = m * tar.tri;
    Matrix3d tmp = Matrix3d::Zero();
    tmp(0, 0) = D(0) / S(0);
    tmp(1, 1) = D(1) / S(1);
    tmp(2, 2) = D(2) / S(2);

    return m.inverse() * tmp * m;
}

Eigen::Matrix3d XYZScalingCA::apply(const Illuminant& source, const Illuminant& target)
{
    return internal::chromaticAdaptation(source, target, Eigen::Matrix3d::Identity());
}

Eigen::Matrix3d VonKriesCA::apply(const Illuminant& source, const Illuminant& target)
{
    using Eigen::Matrix3d;
    const Matrix3d method =
        (Matrix3d() <<
            0.40024, 0.7076, -0.08081,
            -0.2263, 1.16532, 0.0457,
            0.0, 0.0, 0.91822)
        .finished();

    return internal::chromaticAdaptation(source, target, method);
}

Eigen::Matrix3d BradfordCA::apply(const Illuminant& source, const Illuminant& target)
{
    using Eigen::Matrix3d;
    const Matrix3d method =
        (Matrix3d() <<
            0.8951, 0.2664, -0.1614,
            -0.7502, 1.7135, 0.0367,
            0.0389, -0.0685, 1.0296)
        .finished();

    return internal::chromaticAdaptation(source, target, method);
}

Eigen::Matrix3d BypassCA::apply(const Illuminant& source, const Illuminant& target)
{
    return Eigen::Matrix3d::Identity();
}