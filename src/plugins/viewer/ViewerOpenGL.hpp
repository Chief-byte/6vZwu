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

/** @file ViewerOpenGL.hpp */

#ifndef VIEWER_OPENGL_HPP
#define VIEWER_OPENGL_HPP

#include <vector>

#include <Region.hpp>

#include <ViewerAabb.hpp>

#include <QVector3D>

/** Viewer OpenGL. */
class ViewerOpenGL
{
public:
    enum Mode
    {
        POINTS,
        LINES,
        QUADS
    };

    ViewerOpenGL();
    ~ViewerOpenGL();

    static void render(Mode mode,
                       const std::vector<float> &xyz,
                       const std::vector<float> &rgb);

    static void render(Mode mode,
                       const std::vector<float> &xyz,
                       const std::vector<float> &rgb,
                       const std::vector<unsigned int> &indices,
                       size_t count);

    static void renderClipFilter(const Region &clipFilter);
    static void renderAabb(const ViewerAabb &box);
    static void renderAxis();

protected:
};

#endif /* VIEWER_OPENGL_HPP */