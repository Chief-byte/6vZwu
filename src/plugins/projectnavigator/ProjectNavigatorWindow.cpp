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

/** @file ProjectNavigatorWindow.cpp */

#include <MainWindow.hpp>
#include <ProjectNavigatorItemClassifications.hpp>
#include <ProjectNavigatorItemClipping.hpp>
#include <ProjectNavigatorItemColor.hpp>
#include <ProjectNavigatorItemDensity.hpp>
#include <ProjectNavigatorItemDescriptor.hpp>
#include <ProjectNavigatorItemElevation.hpp>
#include <ProjectNavigatorItemFiles.hpp>
#include <ProjectNavigatorItemIntensity.hpp>
#include <ProjectNavigatorItemLayers.hpp>
#include <ProjectNavigatorItemReturnNumber.hpp>
#include <ProjectNavigatorTree.hpp>
#include <ProjectNavigatorWindow.hpp>
#include <ThemeIcon.hpp>

#define ICON(name) (ThemeIcon(":/projectnavigator/", name))

ProjectNavigatorWindow::ProjectNavigatorWindow(MainWindow *mainWindow)
    : QDockWidget(mainWindow),
      mainWindow_(mainWindow)
{
    // Tab
    items_.push_back(
        new ProjectNavigatorItemFiles(mainWindow_, ICON("file"), tr("文件")));

    items_.push_back(new ProjectNavigatorItemLayers(mainWindow_,
                                                    ICON("layers"),
                                                    tr("图层")));

    items_.push_back(
        new ProjectNavigatorItemClassifications(mainWindow_,
                                                ICON("classification"),
                                                tr("分类")));

    items_.push_back(
        new ProjectNavigatorItemColor(mainWindow_, ICON("color"), tr("颜色")));

    items_.push_back(new ProjectNavigatorItemIntensity(mainWindow_,
                                                       ICON("intensity"),
                                                       tr("强度")));

    items_.push_back(new ProjectNavigatorItemReturnNumber(mainWindow_,
                                                          ICON("return_number"),
                                                          tr("返回号码")));

    items_.push_back(new ProjectNavigatorItemElevation(mainWindow_,
                                                       ICON("elevation_filter"),
                                                       tr("高度")));

    items_.push_back(new ProjectNavigatorItemDensity(mainWindow_,
                                                     ICON("descriptor_filter"),
                                                     tr("密度")));

    items_.push_back(
        new ProjectNavigatorItemDescriptor(mainWindow_,
                                           ICON("descriptor_filter"),
                                           tr("描述符")));

    items_.push_back(new ProjectNavigatorItemClipping(mainWindow_,
                                                      ICON("clip_filter"),
                                                      tr("剪辑过滤器")));

    // Tabs
    menu_ = new ProjectNavigatorTree(mainWindow_);
    for (size_t i = 0; i < items_.size(); i++)
    {
        menu_->addItem(items_[i]);
    }

    // Dock
    setWidget(menu_);
    setWindowTitle(tr("数据浏览器"));
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    mainWindow_->addDockWidget(Qt::RightDockWidgetArea, this);
}
