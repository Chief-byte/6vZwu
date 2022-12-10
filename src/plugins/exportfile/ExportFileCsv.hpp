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

/** @file ExportFileCsv.hpp */

#ifndef EXPORT_FILE_CSV_HPP
#define EXPORT_FILE_CSV_HPP

#include <ExportFileInterface.hpp>
#include <File.hpp>
#include <Query.hpp>

/** Export File in Comma Separated Values File Format. */
class ExportFileCsv : public ExportFileInterface
{
public:
    ExportFileCsv();
    virtual ~ExportFileCsv();

    virtual bool isOpen() { return file_.isOpen(); }
    virtual void create(const std::string &path,
                        uint64_t nPoints,
                        const Box<double> &region);
    virtual void write(Query &query);
    virtual void close();

private:
    File file_;
};

#endif /* EXPORT_FILE_CSV_HPP */
