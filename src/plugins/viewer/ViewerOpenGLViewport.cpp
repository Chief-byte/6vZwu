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

/** @file ViewerOpenGLViewport.cpp */

#include <Editor.hpp>
#include <Time.hpp>

#include <ViewerOpenGL.hpp>
#include <ViewerOpenGLViewport.hpp>
#include <ViewerViewports.hpp>

#include <QDebug>
#include <QMouseEvent>
#include <QWheelEvent>

#define LOG_MODULE_NAME "ViewerOpenGLViewport"
#include <Log.hpp>

ViewerOpenGLViewport::ViewerOpenGLViewport(QWidget *parent)
    : QOpenGLWidget(parent),
      windowViewports_(nullptr),
      viewportId_(0),
      selected_(false),
      editor_(nullptr)

{
    resetCamera();
}

ViewerOpenGLViewport::~ViewerOpenGLViewport()
{
}

void ViewerOpenGLViewport::setWindowViewports(ViewerViewports *viewer,
                                              size_t viewportId)
{
    windowViewports_ = viewer;
    viewportId_ = viewportId;
}

size_t ViewerOpenGLViewport::viewportId() const
{
    return viewportId_;
}

void ViewerOpenGLViewport::setSelected(bool selected)
{
    selected_ = selected;
}

bool ViewerOpenGLViewport::isSelected() const
{
    return selected_;
}

void ViewerOpenGLViewport::updateScene(Editor *editor)
{
    LOG_TRACE_UPDATE_VIEW(<< "Update viewport <" << viewportId_ << ">.");
    editor_ = editor;
}

void ViewerOpenGLViewport::resetScene(Editor *editor, bool resetView)
{
    LOG_TRACE_UPDATE_VIEW(<< "Reset viewport <" << viewportId_ << ">.");
    aabb_.set(editor->datasets().boundary());
    if (resetView)
    {
        resetCamera();
    }
}

Camera ViewerOpenGLViewport::camera() const
{
    return camera_.toCamera();
}

void ViewerOpenGLViewport::setViewOrthographic()
{
    camera_.setOrthographic();
}

void ViewerOpenGLViewport::setViewPerspective()
{
    camera_.setPerspective();
}

void ViewerOpenGLViewport::setViewDirection(const QVector3D &dir,
                                            const QVector3D &up)
{
    QVector3D center = camera_.getCenter();
    float distance = camera_.getDistance();

    QVector3D eye = (dir * distance) + center;
    camera_.setLookAt(eye, center, up);
}

void ViewerOpenGLViewport::setViewTop()
{
    QVector3D dir(0.0F, 0.0F, 1.0F);
    QVector3D up(0.0F, 1.0F, 0.0F);
    setViewDirection(dir, up);
}

void ViewerOpenGLViewport::setViewFront()
{
    QVector3D dir(0.0F, -1.0F, 0.0F);
    QVector3D up(0.0F, 0.0F, 1.0F);
    setViewDirection(dir, up);
}

void ViewerOpenGLViewport::setViewRight()
{
    QVector3D dir(1.0F, 0.0F, 0.0F);
    QVector3D up(0.0F, 0.0F, 1.0F);
    setViewDirection(dir, up);
}

void ViewerOpenGLViewport::setView3d()
{
    QVector3D dir(1.0F, -1.0F, 1.0F);
    QVector3D up(-1.065F, 1.0F, 1.0F);
    dir.normalize();
    up.normalize();
    setViewDirection(dir, up);
}

void ViewerOpenGLViewport::resetCamera()
{
    QVector3D center(0.0F, 0.0F, 0.0F);
    float distance = 1.0F;

    if (aabb_.isValid())
    {
        center = aabb_.getCenter();
        // center[2] = aabb_.getMin().z();
        distance = aabb_.getRadius() * 2.0F;
    }

    QVector3D eye(1.0F, -1.0F, 1.0F);
    QVector3D up(-1.065F, 1.0F, 1.0F);
    eye.normalize();
    up.normalize();

    eye = (eye * distance) + center;
    camera_.setLookAt(eye, center, up);
}

void ViewerOpenGLViewport::setViewResetDistance()
{
    QVector3D center = camera_.getCenter();
    QVector3D up = camera_.getUp();
    QVector3D dir = camera_.getDirection();

    float distance = 1.0F;
    if (aabb_.isValid())
    {
        distance = aabb_.getRadius() * 2.0F;
    }

    QVector3D eye = (dir * distance) + center;
    camera_.setLookAt(eye, center, up);
}

void ViewerOpenGLViewport::setViewResetCenter()
{
    QVector3D dir = camera_.getDirection();
    QVector3D up = camera_.getUp();
    float distance = camera_.getDistance();

    QVector3D center = camera_.getCenter();
    if (aabb_.isValid())
    {
        center = aabb_.getCenter();
        // center[2] = aabb_.getMin().z();
    }

    QVector3D eye = (dir * distance) + center;
    camera_.setLookAt(eye, center, up);
}

void ViewerOpenGLViewport::initializeGL()
{
    initializeOpenGLFunctions();

    setUpdateBehavior(QOpenGLWidget::PartialUpdate);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glClearDepth(1.0F);
}

void ViewerOpenGLViewport::paintGL()
{
    LOG_TRACE_UNKNOWN(<< "Paint width <" << camera_.width() << "> height <"
                      << camera_.height() << ">.");

    // Setup camera
    glViewport(0, 0, camera_.width(), camera_.height());

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(camera_.getProjection().data());

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(camera_.getModelView().data());

    // Render
    bool firstFrame = renderScene();

    if (firstFrame)
    {
        renderGuides();
    }
}

void ViewerOpenGLViewport::renderGuides()
{
    // Bounding box
    glColor3f(0.25F, 0.25F, 0.25F);
    ViewerOpenGL::renderAabb(aabb_);

    // Overlay
    QMatrix4x4 m;
    float w = static_cast<float>(camera_.width());
    float h = static_cast<float>(camera_.height());

    m.ortho(-w * 0.5F, w * 0.5F, -h * 0.5F, h * 0.5F, -50.0F, 50.0F);
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(m.data());

    // Overlay: rotated
    float axesSize = 30.0F;
    m.setToIdentity();
    m.translate(-w * 0.5F + axesSize + 2.0F, -h * 0.5F + axesSize + 2.0F);
    m.scale(axesSize);
    m = m * camera_.rotation();

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(m.data());

    glDisable(GL_DEPTH_TEST);
    glLineWidth(2.0F);
    ViewerOpenGL::renderAxis();
    glLineWidth(1.0F);
    glEnable(GL_DEPTH_TEST);
}

void ViewerOpenGLViewport::clearScreen()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool ViewerOpenGLViewport::renderScene()
{
    if (!editor_)
    {
        return true;
    }

    bool firstFrame = false;

    editor_->lock();

    renderSceneSettingsEnable();

    double t1 = Time::realTime();

    size_t pageSize = editor_->viewports().pageSize(viewportId_);

    if (pageSize == 0)
    {
        clearScreen();
        firstFrame = true;
    }

    LOG_TRACE_UNKNOWN(<< "Render viewport <" << viewportId_ << "> pageSize <"
                      << pageSize << ">.");

    for (size_t pageIndex = 0; pageIndex < pageSize; pageIndex++)
    {
        Page &page = editor_->viewports().page(viewportId_, pageIndex);

        if (page.state() == Page::STATE_RENDER)
        {
            LOG_TRACE_UNKNOWN(<< "Render pageId <" << page.pageId() << ">.");

            if (pageIndex == 0)
            {
                clearScreen();
                firstFrame = true;
            }

            ViewerOpenGL::render(ViewerOpenGL::POINTS,
                                 page.renderPosition,
                                 page.renderColor,
                                 page.selection,
                                 page.selectionSize);

            glFlush();

            page.setState(Page::STATE_RENDERED);

            double t2 = Time::realTime();
            if (t2 - t1 > 0.02)
            {
                break;
            }
        }
    }

    renderSceneSettingsDisable();

    if (firstFrame)
    {
        renderFirstFrame();
    }

    editor_->unlock();

    return firstFrame;
}

void ViewerOpenGLViewport::renderFirstFrame()
{
    LOG_TRACE_UPDATE_VIEW(<< "Rendered first frame in viewport <" << viewportId_
                          << ">.");

    ViewerOpenGL::renderClipFilter(editor_->clipFilter());
    renderLayers();
}

void ViewerOpenGLViewport::renderLayers()
{
    const Layers &layers = editor_->layers();
    for (size_t i = 0; i < layers.size(); i++)
    {
        const Layer &layer = layers.at(i);
        for (size_t m = 0; m < layer.meshSize(); m++)
        {
            const Mesh &mesh = layer.mesh(m);

            ViewerOpenGL::Mode mode;
            if (mesh.mode == Mesh::MODE_POINTS)
            {
                mode = ViewerOpenGL::POINTS;
                glPointSize(3.0F);
            }
            else
            {
                mode = ViewerOpenGL::LINES;
            }

            ViewerOpenGL::render(mode, mesh.xyz, mesh.rgb);

            glPointSize(1.0F);
        }
    }
}

void ViewerOpenGLViewport::renderSceneSettingsEnable()
{
    const Settings &settings = editor_->settings();
    const SettingsView &opt = settings.view();

    // Background
    const Vector3<float> &rgb = opt.backgroundColor();
    glClearColor(rgb[0], rgb[1], rgb[2], 0.0F);

    // Point size
    glPointSize(opt.pointSize());

    // Fog
    if (opt.isFogEnabled())
    {
        QVector3D eye = camera_.getEye();
        QVector3D direction = -camera_.getDirection();
        direction.normalize();

        float min;
        float max;
        aabb_.getRange(eye, direction, &min, &max);
        float d = max - min;

        GLfloat colorFog[4]{0.0F, 0.0F, 0.0F, 0.0F};
        glFogi(GL_FOG_MODE, GL_LINEAR);
        glFogfv(GL_FOG_COLOR, colorFog);
        glHint(GL_FOG_HINT, GL_DONT_CARE);
        glFogf(GL_FOG_START, min);
        glFogf(GL_FOG_END, max + d * 0.1F);
        glEnable(GL_FOG);
    }
}

void ViewerOpenGLViewport::renderSceneSettingsDisable()
{
    const Settings &settings = editor_->settings();
    glPointSize(1.0F);

    if (settings.view().isFogEnabled())
    {
        glDisable(GL_FOG);
    }
}

void ViewerOpenGLViewport::resizeGL(int w, int h)
{
    w = static_cast<int>(parentWidget()->devicePixelRatio() * w);
    h = static_cast<int>(parentWidget()->devicePixelRatio() * h);
    camera_.setViewport(0, 0, w, h);
    camera_.updateProjection();
    cameraChanged();
}

void ViewerOpenGLViewport::mouseReleaseEvent(QMouseEvent *event)
{
    (void)event;
}

void ViewerOpenGLViewport::mousePressEvent(QMouseEvent *event)
{
    camera_.mousePressEvent(event);
    setFocus();
}

void ViewerOpenGLViewport::mouseMoveEvent(QMouseEvent *event)
{
    camera_.mouseMoveEvent(event);
    cameraChanged();
}

void ViewerOpenGLViewport::wheelEvent(QWheelEvent *event)
{
    camera_.wheelEvent(event);
    setFocus();
    cameraChanged();
}

void ViewerOpenGLViewport::setFocus()
{
    if (!isSelected())
    {
        if (windowViewports_)
        {
            windowViewports_->selectViewport(this);
        }
    }
}

void ViewerOpenGLViewport::cameraChanged()
{
    if (windowViewports_)
    {
        emit windowViewports_->cameraChanged(viewportId_);
    }
}
