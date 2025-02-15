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

/** @file ProjectNavigatorItemClipping.cpp */

#include <Region.hpp>

#include <DoubleRangeSliderWidget.hpp>
#include <MainWindow.hpp>
#include <ProjectNavigatorItemClipping.hpp>
#include <ThemeIcon.hpp>

#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>

#define LOG_MODULE_NAME "ProjectNavigatorItemClipping"
#include <Log.hpp>

#define ICON(name) (ThemeIcon(":/projectnavigator/", name))

ProjectNavigatorItemClipping::ProjectNavigatorItemClipping(
    MainWindow *mainWindow,
    const QIcon &icon,
    const QString &text)
    : ProjectNavigatorItem(mainWindow, icon, text)
{
    LOG_DEBUG(<< "Called.");

    // Input widgets
    DoubleRangeSliderWidget::create(
        rangeInput_[0],
        this,
        SLOT(slotRangeIntermediateMinimumValue()),
        SLOT(slotRangeIntermediateMaximumValue()),
        tr("X range"),
        tr("Min-max clipping range filter along X axis"),
        tr("pt"),
        1,
        0,
        100,
        0,
        100);

    DoubleRangeSliderWidget::create(
        rangeInput_[1],
        this,
        SLOT(slotRangeIntermediateMinimumValue()),
        SLOT(slotRangeIntermediateMaximumValue()),
        tr("Y range"),
        tr("Min-max clipping range filter along Y axis"),
        tr("pt"),
        1,
        0,
        100,
        0,
        100);

    DoubleRangeSliderWidget::create(
        rangeInput_[2],
        this,
        SLOT(slotRangeIntermediateMinimumValue()),
        SLOT(slotRangeIntermediateMaximumValue()),
        tr("Z range"),
        tr("Min-max clipping range filter along Z axis"),
        tr("pt"),
        1,
        0,
        100,
        0,
        100);

    resetButton_ = new QPushButton(tr("&Reset"), this);
    connect(resetButton_, SIGNAL(clicked()), this, SLOT(reset()));

    // Layout
    QHBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->addStretch();
    controlLayout->addWidget(resetButton_);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(rangeInput_[0]);
    mainLayout->addWidget(rangeInput_[1]);
    mainLayout->addWidget(rangeInput_[2]);
    mainLayout->addLayout(controlLayout);
    mainLayout->addStretch();

    mainLayout_->addLayout(mainLayout);
    setLayout(mainLayout_);

    // Data
    connect(mainWindow_,
            SIGNAL(signalUpdate(void *, const QSet<Editor::Type> &)),
            this,
            SLOT(slotUpdate(void *, const QSet<Editor::Type> &)));
}

void ProjectNavigatorItemClipping::slotUpdate(void *sender,
                                              const QSet<Editor::Type> &target)
{
    if (sender == this)
    {
        return;
    }

    if (!target.empty() && !target.contains(Editor::TYPE_CLIP_FILTER))
    {
        return;
    }

    LOG_DEBUG(<< "Called.");

    const Box<double> &boundary = mainWindow_->editor().datasets().boundary();
    LOG_DEBUG(<< "Update boundary <" << boundary << ">.");

    const Region &region = mainWindow_->editor().clipFilter();
    LOG_DEBUG(<< "Update region <" << region << ">.");

    for (size_t i = 0; i < 3; i++)
    {
        clipRange_[i].setMinimum(boundary.min(i));
        clipRange_[i].setMaximum(boundary.max(i));
        clipRange_[i].setMinimumValue(region.box.min(i));
        clipRange_[i].setMaximumValue(region.box.max(i));

        rangeInput_[i]->blockSignals(true);
        rangeInput_[i]->setMinimum(clipRange_[i].minimum());
        rangeInput_[i]->setMaximum(clipRange_[i].maximum());
        rangeInput_[i]->setMinimumValue(clipRange_[i].minimumValue());
        rangeInput_[i]->setMaximumValue(clipRange_[i].maximumValue());
        rangeInput_[i]->blockSignals(false);
    }
}

void ProjectNavigatorItemClipping::slotRangeIntermediateMinimumValue()
{
    LOG_DEBUG(<< "Called.");
    QObject *obj = sender();
    for (int i = 0; i < 3; i++)
    {
        if (obj == rangeInput_[i])
        {
            double v = rangeInput_[i]->minimumValue();
            LOG_DEBUG(<< "Input minimumValue <" << v << ">.");
            clipRange_[i].setMinimumValue(v);
        }
    }

    filterChanged();
}

void ProjectNavigatorItemClipping::slotRangeIntermediateMaximumValue()
{
    LOG_DEBUG(<< "Called.");
    QObject *obj = sender();
    for (int i = 0; i < 3; i++)
    {
        if (obj == rangeInput_[i])
        {
            double v = rangeInput_[i]->maximumValue();
            LOG_DEBUG(<< "Input maximumValue <" << v << ">.");
            clipRange_[i].setMaximumValue(v);
        }
    }

    filterChanged();
}

void ProjectNavigatorItemClipping::filterChanged()
{
    LOG_DEBUG(<< "Called.");

    Region region;

    double x1 = clipRange_[0].minimumValue();
    double y1 = clipRange_[1].minimumValue();
    double z1 = clipRange_[2].minimumValue();
    double x2 = clipRange_[0].maximumValue();
    double y2 = clipRange_[1].maximumValue();
    double z2 = clipRange_[2].maximumValue();

    region.box.set(x1, y1, z1, x2, y2, z2);

    if (isFilterEnabled())
    {
        region.enabled = Region::TYPE_BOX;
    }
    else
    {
        region.enabled = Region::TYPE_NONE;
    }

    mainWindow_->suspendThreads();
    mainWindow_->editor().setClipFilter(region);
    mainWindow_->updateFilter();
}

void ProjectNavigatorItemClipping::setFilterEnabled(bool b)
{
    ProjectNavigatorItem::setFilterEnabled(b);
    filterChanged();
}

void ProjectNavigatorItemClipping::reset()
{
    for (size_t i = 0; i < 3; i++)
    {
        clipRange_[i].setMinimumValue(clipRange_[i].minimum());
        clipRange_[i].setMaximumValue(clipRange_[i].maximum());

        rangeInput_[i]->blockSignals(true);
        rangeInput_[i]->setMinimumValue(clipRange_[i].minimumValue());
        rangeInput_[i]->setMaximumValue(clipRange_[i].maximumValue());
        rangeInput_[i]->blockSignals(false);
    }

    filterChanged();
}
