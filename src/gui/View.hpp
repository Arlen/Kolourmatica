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


#include <QWidget>

#include <RGB.hpp>
#include <converter.hpp>
#include <enums.hpp>
#include <types.hpp>

#include <functional>
#include <vector>


namespace km
{
    auto canvas_to_viewport(const QPoint& p, const QSize& size) -> Point2d;
    auto get_rgb_corners() -> std::vector<Point3d>;
    auto dist_to_closest_side(const Point3d& p) -> f64;
}


class View : public QWidget
{
    using SegmentFn = std::function<km::Segment (const Point2d&)>;

    Q_OBJECT

public:
    explicit View(QWidget* parent = nullptr);

    void setFrom(enums::ColourSpace index);

    void setTo(enums::ColourSpace index);

    void setSrcRefWhite(const km::Illuminant& rw);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    km::BBox getBB();

    static km::Segment getSegment_Lab(const Point2d& p);
    static km::Segment getSegment_LCHab(const Point2d& p);
    static km::Segment getSegment_Luv(const Point2d& p);
    static km::Segment getSegment_LCHuv(const Point2d& p);
    static km::Segment getSegment_xyY(const Point2d& p);
    static km::Segment getSegment_default(const Point2d& p);

    static bool isValidSource(enums::ColourSpace index);
    static bool isValidTarget(enums::ColourSpace index);

    static i32 getRayIndex(enums::ColourSpace);

    enums::ColourSpace fromIndex;
    enums::ColourSpace toIndex;

    km::Illuminant srcRefWhite;
    SegmentFn getSegment;

    km::Converter conv;
};
