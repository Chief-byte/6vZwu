/*
    Copyright 2020 VUKOZ

    This file is part of 3D Forest.

    3D Forest is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    3D Forest is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 3D Forest.  If not, see <https://www.gnu.org/licenses/>.
*/

/** @file LayerData.hpp */

#ifndef LAYER_DATA_HPP
#define LAYER_DATA_HPP

#include <ExportEditor.hpp>
#include <Json.hpp>
#include <Vector3.hpp>

/** LayerData. */
class EXPORT_EDITOR LayerData
{
public:
    LayerData();

    const Vector3<double> &position() const { return position_; }
    void setPosition(const Vector3<double> &p) { position_ = p; };

    double radius() const { return radius_; }
    void setRadius(double r) { radius_ = r; }

protected:
    Vector3<double> position_;
    double radius_;
};

#endif /* LAYER_DATA_HPP */
