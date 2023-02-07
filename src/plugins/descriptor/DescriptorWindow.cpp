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

/** @file DescriptorWindow.cpp */

#include <DescriptorWindow.hpp>
#include <MainWindow.hpp>
#include <ProgressDialog.hpp>
#include <SliderWidget.hpp>
#include <ThemeIcon.hpp>

#include <QGroupBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QVBoxLayout>

#define ICON(name) (ThemeIcon(":/descriptor/", name))

#define LOG_MODULE_NAME "DescriptorWindow"
#include <Log.hpp>

DescriptorWindow::DescriptorWindow(MainWindow *mainWindow)
    : QDialog(mainWindow),
      mainWindow_(mainWindow),
      descriptor_(&mainWindow->editor())
{
    LOG_DEBUG(<< "Called.");

    // Widgets
    SliderWidget::create(radius_,
                         this,
                         nullptr,
                         nullptr,
                         tr("半径"),
                         tr("半径"),
                         tr("点"),
                         1,
                         10,
                         1000,
                         1000);

    SliderWidget::create(voxelSize_,
                         this,
                         nullptr,
                         nullptr,
                         tr("体素大小"),
                         tr("体素大小"),
                         tr("点"),
                         1,
                         1,
                         1000,
                         1);
    voxelSize_->setEnabled(false);

    // Method
    methodRadioButton_[DescriptorAction::METHOD_PCA] =
        new QRadioButton(tr("PCA"));
    methodRadioButton_[DescriptorAction::METHOD_DISTRIBUTION] =
        new QRadioButton(tr("分布"));
    methodRadioButton_[DescriptorAction::METHOD_PCA]->setChecked(true);
    QVBoxLayout *methodVBoxLayout = new QVBoxLayout;
    methodVBoxLayout->addWidget(methodRadioButton_[0]);
    methodVBoxLayout->addWidget(methodRadioButton_[1]);
    // methodVBoxLayout->addStretch(1);
    QGroupBox *methodGroupBox = new QGroupBox(tr("方法"));
    methodGroupBox->setLayout(methodVBoxLayout);

    // Settings layout
    QVBoxLayout *settingsLayout = new QVBoxLayout;
    settingsLayout->addWidget(radius_);
    settingsLayout->addWidget(voxelSize_);
    settingsLayout->addWidget(methodGroupBox);
    settingsLayout->addStretch();

    // Buttons
    applyButton_ = new QPushButton(tr("计算"));
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
    setWindowTitle(tr("描述符"));
    setWindowIcon(ICON("descriptor"));
    setMaximumHeight(height());
    setModal(true);
}

void DescriptorWindow::slotApply()
{
    LOG_DEBUG(<< "Called.");

    mainWindow_->suspendThreads();

    double radius = static_cast<double>(radius_->value());
    double voxelSize = static_cast<double>(voxelSize_->value());

    DescriptorAction::Method method = DescriptorAction::METHOD_PCA;
    if (methodRadioButton_[DescriptorAction::METHOD_DISTRIBUTION]->isChecked())
    {
        method = DescriptorAction::METHOD_DISTRIBUTION;
    }

    try
    {
        descriptor_.initialize(radius, voxelSize, method);
        ProgressDialog::run(mainWindow_, "Computing Descriptors", &descriptor_);
    }
    catch (std::exception &e)
    {
        mainWindow_->showError(e.what());
    }
    catch (...)
    {
        mainWindow_->showError("Unknown error");
    }

    descriptor_.clear();

    mainWindow_->update({Editor::TYPE_DESCRIPTOR});
}
