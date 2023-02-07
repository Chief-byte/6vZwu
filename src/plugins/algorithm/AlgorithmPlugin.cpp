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

/** @file AlgorithmPlugin.cpp */

#include <AlgorithmPlugin.hpp>
#include <AlgorithmWindow.hpp>
#include <MainWindow.hpp>
#include <ThemeIcon.hpp>

#define ICON(name) (ThemeIcon(":/algorithm/", name))

AlgorithmPlugin::AlgorithmPlugin()
    : mainWindow_(nullptr),
      pluginWindow_(nullptr)
{
}

void AlgorithmPlugin::initialize(MainWindow *mainWindow)
{
    mainWindow_ = mainWindow;

    mainWindow_->createAction(nullptr,
                              "File",
                              "File Properties",
                              tr("算法"),
                              tr("运行算法"),
                              ICON("algorithm"),
                              this,
                              SLOT(slotPlugin()));
}

void AlgorithmPlugin::slotPlugin()
{
    if (!mainWindow_)
    {
        return;
    }

    if (!pluginWindow_)
    {
        pluginWindow_ = new AlgorithmWindow(mainWindow_);
    }

    pluginWindow_->show();
    pluginWindow_->raise();
    pluginWindow_->activateWindow();
}
