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

/** @file ClassificationWindow.cpp */
#include <TochmWindow.hpp>
#include <MainWindow.hpp>
#include <ProgressDialog.hpp>
#include <SliderWidget.hpp>
#include <ThemeIcon.hpp>
#include <ImportFilePlugin.cpp>
#include<windows.h>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>

#define LOG_MODULE_NAME "ClassificationWindow"
#include <Log.hpp>

#define ICON(name) (ThemeIcon(":/tochm/", name))

TochmWindow::TochmWindow(MainWindow *mainWindow)
    : QDialog(mainWindow),
      mainWindow_(mainWindow),
      tochm_(&mainWindow->editor())
{
    LOG_DEBUG(<< "Window created.");

    // Widgets
    SliderWidget::create(nPointsSlider_,
                         this,
                         nullptr,
                         nullptr,
                         tr("每个单元格的点数"),
                         tr("每个单元格的点数"),
                         tr("点"),
                         1,
                         1000,
                         1000000,
                         10000);

    SliderWidget::create(lengthSlider_,
                         this,
                         nullptr,
                         nullptr,
                         tr("单元最小长度"),
                         tr("单元最小长度"),
                         tr("%"),
                         1,
                         1,
                         100,
                         5);

    SliderWidget::create(rangeSlider_,
                         this,
                         nullptr,
                         nullptr,
                         tr("地面层"),
                         tr("地平面最大值"),
                         tr("%"),
                         1,
                         1,
                         100,
                         15);

    SliderWidget::create(angleSlider_,
                         this,
                         nullptr,
                         nullptr,
                         tr("地面角度"),
                         tr("地面角度"),
                         tr("度"),
                         1,
                         1,
                         89,
                         60);

    // Settings layout
    QVBoxLayout *settingsLayout = new QVBoxLayout;
    settingsLayout->addWidget(nPointsSlider_);
    settingsLayout->addWidget(lengthSlider_);
    settingsLayout->addWidget(rangeSlider_);
    settingsLayout->addWidget(angleSlider_);
    settingsLayout->addStretch();

    // Buttons
    applyButton_ = new QPushButton(tr("应用"));
    applyButton_->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    connect(applyButton_, SIGNAL(clicked()), this, SLOT(slotApply()));

    // Buttons layout
    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(applyButton_);

    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(settingsLayout);
    mainLayout->addSpacing(10);
    mainLayout->addLayout(buttonsLayout);
    mainLayout->addStretch();

    // Dialog
    setLayout(mainLayout);
    setWindowTitle(tr("冠层高度模型"));
    setWindowIcon(ICON("chm"));
    setMaximumHeight(height());
    setModal(true);
}

void TochmWindow::slotApply()
{
    LOG_DEBUG(<< "Compute classification.");

    mainWindow_->suspendThreads();

    //size_t pointsPerCell = static_cast<size_t>(nPointsSlider_->value());
    //double cellLengthMinPercent = static_cast<double>(lengthSlider_->value());
    //double groundErrorPercent = static_cast<double>(rangeSlider_->value());
    //double angleDeg = static_cast<double>(angleSlider_->value());

    try
    {
        // tochm_.initialize(pointsPerCell,
        //                            cellLengthMinPercent,
        //                            groundErrorPercent,
        //                            angleDeg);
        // ProgressDialog::run(mainWindow_,
        //                     "Computing Classification", &tochm_);
        WinExec("lasground_new -cpu64 -i \"C:\\Users\\Chief\\3DForest-Fork\\base_cloud.las\" -wilderness -hyper_fine -compute_height -replace_z -odir \"..\\test\" -o \"123.las\"",SW_HIDE );
        
        importPluginFile("..\\test\\123.las",mainWindow_);
    }
    catch (std::exception &e)
    {
        mainWindow_->showError(e.what());
    }
    catch (...)
    {
        mainWindow_->showError("Unknown error");
    }

    tochm_.clear();

    mainWindow_->editor().viewports().setState(Page::STATE_READ);

    mainWindow_->resumeThreads();
}
