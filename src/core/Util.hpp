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

/** @file Util.hpp */

#ifndef UTIL_HPP
#define UTIL_HPP

#include <string>

inline std::string tolower(const std::string &str)
{
    std::string ret = str;
    for (auto &c : ret)
    {
        c = static_cast<char>(std::tolower(c));
    }
    return ret;
}

#endif /* UTIL_HPP */
