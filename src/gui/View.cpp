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


#include "View.hpp"

#include <QtGui/QPainter>
#include <QtWidgets/QVBoxLayout>

#include <bbox.hpp>


// returns a point in [0,1]^2
auto km::canvas_to_viewport(const QPoint& pixel, const QSize& size) -> Point2d
{
    const f64 inv = f64(1) / qMin(size.width()-1, size.height()-1);

    return {pixel.x() * inv, (size.height() - pixel.y()) * inv};
}

auto km::get_rgb_corners() -> std::vector<Point3d>
{
    return {{0,0,0}, {0,0,1}, {0,1,0}, {1,0,0}, {0,1,1}, {1,1,0}, {1,0,1}, {1,1,1}};
}


View::View(QWidget* parent)
    : QWidget(parent)
{
    setMinimumHeight(256);
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Expanding);

    getSegment = [this](const Point2d& p) { return View::getSegment_default(p); };

    fromIndex = enums::ColourSpace::XYZ;
    toIndex = enums::ColourSpace::xyY;
}

void View::setFrom(enums::ColourSpace index)
{
    switch (index) {
        using namespace enums;
        case ColourSpace::Lab:
            getSegment = [this](const Point2d& p) { return View::getSegment_Lab(p); };
        break;
        case ColourSpace::LCHab:
            getSegment = [this](const Point2d& p) { return View::getSegment_LCHab(p); };
        break;
        case ColourSpace::Luv:
            getSegment = [this](const Point2d& p) { return View::getSegment_Luv(p); };
        break;
        case ColourSpace::LCHuv:
            getSegment = [this](const Point2d& p) { return View::getSegment_LCHuv(p); };
        break;
        case ColourSpace::xyY:
            getSegment = [this](const Point2d& p) { return View::getSegment_xyY(p); };
        break;
        default:
            getSegment = [this](const Point2d& p) { return View::getSegment_default(p); };
        break;
    }
    fromIndex = index;
    update();
}

void View::setTo(enums::ColourSpace index)
{
    toIndex = index;
    update();
}

void View::setSrcRefWhite(const km::Illuminant& rw)
{
    srcRefWhite = rw;

    update();
}

void View::paintEvent(QPaintEvent* event)
{
    QPainter p(this);

    p.fillRect(rect(), Qt::black);

    if (!(isValidSource(fromIndex) && isValidTarget(toIndex))) {
        return;
    }

    auto fcmp = [](f64 x, f64 y, f64 eps = 0.000001)  -> bool {
        return std::fabs(x - y) < eps;
    };

    const auto bbox = getBB();
    const auto ray_index = getRayIndex(fromIndex);

    if (ray_index == -1) { return; }

    km::XYZ xyz;
    Point3d outCurr;

    for (int x = 0; x < width(); ++x) {
        for (int y = 0; y < height(); ++y) {
            const auto vp = km::canvas_to_viewport({x,y}, rect().size());
            auto segment = getSegment(vp);
            if (!contains(bbox, segment.a) && !contains(bbox, segment.b)) {
                continue;
            }

            auto delta   = segment.a[ray_index] * 0.125;
            auto prevRay = segment.a[ray_index];

            do {
                conv.set_colour_value(fromIndex, segment.a, srcRefWhite);
                xyz.tri = conv.to_XYZ(fromIndex).tri;
                outCurr = conv.from_XYZ(toIndex, xyz, km::Illuminant{});

                if (fcmp(outCurr[0], 1) or fcmp(outCurr[1], 1) or fcmp(outCurr[2], 1)) {
                    break;
                }

                if ((outCurr.array() > 1.0).any()) {
                    prevRay = segment.a[ray_index];
                    segment.a[ray_index] -= delta;
                } else {
                    segment.a[ray_index] = prevRay;
                    delta *= 0.5;
                    segment.a[ray_index] -= delta;
                }

            } while (delta > 0.0001 && segment.a[ray_index] >= 0);

            p.setPen(QColor(outCurr[0] * 255, outCurr[1] * 255, outCurr[2] * 255));
            p.drawPoint(x, y);
        }
    }
}

km::BBox View::getBB()
{
    km::BBox result;
    km::XYZ xyz;

    for (const auto& corner : km::get_rgb_corners()) {
        /// flip from/to b/c we need the result in the 'From' colour space.
        conv.set_colour_value(toIndex, corner, km::Illuminant{}); /// rgb colour spaces
        xyz.tri = conv.to_XYZ(toIndex).tri;
        auto output = conv.from_XYZ(fromIndex, xyz, srcRefWhite);
        if (output.hasNaN()) {
            output = {0, 0, 0};
        }
        expand(result, output);
    }

    /// some colour spaces, such as Luv, have larger bounding box than the one
    /// produced using RGB corners, so expand it a bit.
    for (i32 i = 0; i < 3; ++i) { result.min[i] -= std::fabs(result.min[i]) * 0.1; }
    for (i32 i = 0; i < 3; ++i) { result.max[i] += std::fabs(result.max[i]) * 0.1; }

    return result;
}

/// map p in [0,1]^2 to a maximum segment in Lab space.
km::Segment View::getSegment_Lab(const Point2d& p)
{
    km::Segment result;

    result.a = {100, p[0] * 500.0 - 250, p[1] * 500.0 - 250};
    result.b = {0,   p[0] * 500.0 - 250, p[1] * 500.0 - 250};

    return result;
}

km::Segment View::getSegment_LCHab(const Point2d& p)
{
    km::Segment result;

    result.a = {100, p[0] * 500.0 - 250, p[1] * 500.0 - 250};
    result.b = {0,   p[0] * 500.0 - 250, p[1] * 500.0 - 250};

    return result;
}

km::Segment View::getSegment_Luv(const Point2d& p)
{
    km::Segment result;

    result.a = {100, p[0] * 500.0 - 250, p[1] * 500.0 - 250};
    result.b = {0,   p[0] * 500.0 - 250, p[1] * 500.0 - 250};

    return result;
}

km::Segment View::getSegment_LCHuv(const Point2d& p)
{
    km::Segment result;

    result.a = {100, p[0] * 500.0 - 250, p[1] * 500.0 - 250};
    result.b = {0,   p[0] * 500.0 - 250, p[1] * 500.0 - 250};

    return result;
}

km::Segment View::getSegment_xyY(const Point2d& p)
{
    km::Segment result;

    result.a = {p[0], p[1], 1};
    result.b = {p[0], p[1], 0};

    return result;
}

km::Segment View::getSegment_default(const Point2d& p)
{
    km::Segment result;

    result.a = {0, 0, 0};
    result.b = {0, 0, 0};

    return result;
}

i32 View::getRayIndex(enums::ColourSpace index)
{
    switch (index) {
        using enums::ColourSpace;

        case ColourSpace::Lab: return 0;
        case ColourSpace::LCHab: return 0;
        case ColourSpace::Luv: return 0;
        case ColourSpace::LCHuv: return 0;
        case ColourSpace::xyY: return 2;
        default:
            return -1;
    }
}

bool View::isValidSource(enums::ColourSpace index)
{
    switch (index) {
        using enums::ColourSpace;

        case ColourSpace::Lab:
        case ColourSpace::LCHab:
        case ColourSpace::Luv:
        case ColourSpace::LCHuv:
        case ColourSpace::xyY:
            return true;
        default:
            return false;
    }
}

bool View::isValidTarget(enums::ColourSpace index)
{
    switch (index) {
        using namespace enums;
        case ColourSpace::XYZ:
        case ColourSpace::xyY:
        case ColourSpace::Luv:
        case ColourSpace::LCHuv:
        case ColourSpace::Lab:
        case ColourSpace::LCHab:
            return false;
        default:
            return true;
    }
}