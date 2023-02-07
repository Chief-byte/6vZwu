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

/** @file ImportFileDialog.cpp */

#include <Log.hpp>

#include <ImportFileDialog.hpp>
#include <MainWindow.hpp>
#include <ThemeIcon.hpp>

#include <QCheckBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#define ICON(name) (ThemeIcon(":/importfile/", name))

ImportFileDialog::ImportFileDialog(MainWindow *mainWindow) : QDialog(mainWindow)
{
    // Widgets
    QLabel *description = new QLabel(tr("此操作可能会修改"
                                        " 输入文件。"));

    description->setToolTip(tr("文件大小可能会变大。\n"
                               "未知用户数据将丢失。\n"
                               "一些值将被归一化。"));

    QGroupBox *options = new QGroupBox(tr("选项"));

    centerCheckBox_ = new QCheckBox;
    centerCheckBox_->setChecked(true);

    convertCheckBox_ = new QCheckBox;
    convertCheckBox_->setChecked(true);
    convertCheckBox_->setEnabled(false);

    acceptButton_ = new QPushButton(tr("导入"));
    connect(acceptButton_, SIGNAL(clicked()), this, SLOT(slotAccept()));

    rejectButton_ = new QPushButton(tr("取消"));
    connect(rejectButton_, SIGNAL(clicked()), this, SLOT(slotReject()));

    // Layout
    QGridLayout *optionsLayout = new QGridLayout;
    int row = 0;
    optionsLayout->addWidget(new QLabel(tr("中心偏移")), row, 0);
    optionsLayout->addWidget(centerCheckBox_, row, 1);
    row++;
    optionsLayout->addWidget(new QLabel(tr("转换为 v1.4+")), row, 0);
    optionsLayout->addWidget(convertCheckBox_, row, 1);
    row++;
    options->setLayout(optionsLayout);

    QHBoxLayout *dialogButtons = new QHBoxLayout;
    dialogButtons->addStretch();
    dialogButtons->addWidget(acceptButton_);
    dialogButtons->addWidget(rejectButton_);

    QVBoxLayout *dialogLayout = new QVBoxLayout;
    dialogLayout->addWidget(options);
    dialogLayout->addSpacing(10);
    dialogLayout->addWidget(description);
    dialogLayout->addSpacing(10);
    dialogLayout->addLayout(dialogButtons);
    dialogLayout->addStretch();

    setLayout(dialogLayout);

    // Window
    setWindowTitle(tr("导入文件"));
    setWindowIcon(ICON("import_file"));
    setMaximumWidth(width());
    setMaximumHeight(height());
}

void ImportFileDialog::slotAccept()
{
    close();
    setResult(QDialog::Accepted);
}

void ImportFileDialog::slotReject()
{
    close();
    setResult(QDialog::Rejected);
}

SettingsImport ImportFileDialog::getSettings() const
{
    SettingsImport settings;

    settings.setCenterEnabled(centerCheckBox_->isChecked());

    return settings;
}
