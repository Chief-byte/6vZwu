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

/**
    @file EditorTile.hpp
*/

#ifndef EDITOR_TILE_HPP
#define EDITOR_TILE_HPP

#include <Aabb.hpp>
#include <OctreeIndex.hpp>
#include <Vector3.hpp>
#include <cstdint>
#include <vector>

class EditorBase;

/** Editor Tile. */
class EditorTile
{
public:
    /** Editor Tile Attributes. */
    struct Attributes
    {
        uint16_t intensity;
        int16_t scanAngle;
        uint8_t returnNumber;
        uint8_t numberOfReturns;
        uint8_t classification;
        uint8_t userData;
    };

    /** @name Point Data */
    /**@{*/
    /** Point coordinates.
        The data are stored as [x0, y0, z0, x1, y1, ...].
        These are actual X, Y, or Z coordinates after scaling and offset.
     */
    std::vector<double> xyz;

    /** Red, Green, and Blue image channels.
        The data are stored as [r0, g0, b0, r1, g1, ...].
        Color values are in range from 0 (zero intensity) to 1 (full intensity).
        When the input data set has no colors, then this vector is empty.
    */
    std::vector<float> rgb;

    /** Red, Green, and Blue output colors.
        The data are stored as [r0, g0, b0, r1, g1, ...].
        Color values are in range from 0 (zero intensity) to 1 (full intensity).
        This value is stored in Point Data Record extra bytes.
    */
    std::vector<float> rgbOutput;

    /** Point attributes. */
    std::vector<Attributes> attrib;

    /** GPS time. */
    std::vector<double> gps;

    /** Layer identification numbers.
        This value is stored in Point Data Record extra bytes.
    */
    std::vector<uint32_t> layer;
    /**@}*/

    // Bounding box
    Aabb<double> boundary;

    // Index
    OctreeIndex index;
    std::vector<unsigned int> indices;

    // Tile
    size_t dataSetId;
    size_t tileId;

    // State
    bool loaded;
    bool modified;

    /** Editor Tile Visualization. */
    class View
    {
    public:
        std::vector<float> xyz;
        std::vector<float> rgb;

        Aabb<float> boundary;

        View();
        ~View();

        void resetFrame();
        void nextFrame();
        bool isStarted() const;
        bool isFinished() const;

    protected:
        size_t renderStep;
        size_t renderStepCount;
    };

    View view;

    EditorTile();
    ~EditorTile();

    void read(const EditorBase *editor);

protected:
    void readFilter(const EditorBase *editor);
};

#endif /* EDITOR_TILE_HPP */
