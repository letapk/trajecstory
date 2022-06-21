/*
 * tr_gl.cpp
 *
 * Copyright (c) Kartik Patel
 * E-mail: letapk@gmail.com
 * Download from: https://letapk.wordpress.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

//Last modified 26 June 2017

#include "tr.h"
#include "tr_externs.h"

#include <GL/glu.h>

GLenum mode = GL_LINE_LOOP;

GLWidget::GLWidget() : QGLWidget()
{
    clearcolor = Qt::black;
    trajcolor = Qt::yellow;

    xRot = yRot = zRot = 0;
    xShift = yShift = zShift = 0;
    zoom = 0;
}


void GLWidget::set_reset()
{
    setXRotation (10);
    setYRotation (15);
    setZRotation (0);

    setXShift(0);
    setYShift(0);
    setZShift(0);

    setZoom(50000);

    updateGL();
}

void GLWidget::setXRotation(int angle)
{
    xRot = angle;
    emit xRotationChanged(angle);
    updateGL();
}

void GLWidget::setYRotation(int angle)
{
    yRot = angle;
    emit yRotationChanged(angle);
    updateGL();
}

void GLWidget::setZRotation(int angle)
{
    zRot = angle;
    emit zRotationChanged(angle);
    updateGL();
}

void GLWidget::setXShift(int i)
{
    xShift = (i * (xmax-xmin) * 0.01);// + xmin;
    updateGL();
}

void GLWidget::setYShift(int i)
{
    yShift = (i * (ymax-ymin) * 0.01);// + ymin;
    updateGL();
}

void GLWidget::setZShift(int i)
{
    zShift = (i * (zmax-zmin) * 0.01);// + zmin;
    updateGL();
}

void GLWidget::setZoom(int i)
{
    zoom = i/10.0;
    emit zoomChanged(i);
    updateGL();
}

void GLWidget::replot()
{
    compute_trajectory();
    updateGL();
    emit update_result();
}

void GLWidget::change_background_color()
{
    QColor color;

    color = QColorDialog::getColor(clearcolor, this, "Select Color");

    if (color.isValid()) {
        clearcolor = color;
    }
}

void GLWidget::change_traj_color()
{
    QColor color;

    color = QColorDialog::getColor(trajcolor, this, "Select Color");

    if (color.isValid()) {
        trajcolor = color;
    }
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot + 0.5 * dy);
        setYRotation(yRot + 0.5 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(xRot + 0.5 * dy);
        setZRotation(zRot + 0.5 * dx);
    }
    lastPos = event->pos();
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->delta() / 8;
    int numSteps = numDegrees / 15;

    zoom += 5 * numSteps;

    emit zoomChanged(zoom*10.0);
}

void GLWidget::initializeGL()
{
    qglClearColor(clearcolor);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
}

void GLWidget::paintGL()
{
    double scale;
    int w, h;

    w = this->width();
    h = this->height();

    qglClearColor(clearcolor);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    scale = (double)h/(double)w;
    glViewport(0, 0, w, h);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glScalef(scale, 1.0, 1.0);
    glOrtho(-xmax*5000.0/zoom, xmax*5000.0/zoom, -xmax*5000.0/zoom, xmax*5000.0/zoom, -xmax*5000/zoom, xmax*5000/zoom);

    glRotatef(xRot, 1.0, 0.0, 0.0);
    glRotatef(yRot, 0.0, 1.0, 0.0);
    glRotatef(zRot, 0.0, 0.0, 1.0);

    glTranslatef (-((xmax+xmin)/2.0)+xShift, -((ymax+ymin)/2.0)+yShift, -((zmax+zmin)/2.0)+zShift);

    renderText(10, 15, QString("Zoom=%1").arg(zoom));

    qglColor(Qt::red);
    draw_the_axes();

    qglColor(Qt::green);
    draw_the_walls();

    if (trajcomputed == true) {
        qglColor(trajcolor);
        draw_trajectory();
    }
    glFlush ();
}

void GLWidget::draw_the_axes (void)
{
    //draw the axes
    //xaxis
    glBegin (GL_LINES);
    glVertex3d (xmin, 0, 0);
    glVertex3d (0, 0, 0);
    glEnd();
    glBegin (GL_LINES);
    glVertex3d (0, 0, 0);
    glVertex3d (xmax, 0, 0);
    glEnd();

    //yaxis
    glBegin (GL_LINES);
    glVertex3d (0, ymin, 0);
    glVertex3d (0, 0, 0);
    glEnd();
    glBegin (GL_LINES);
    glVertex3d (0, 0, 0);
    glVertex3d (0, ymax, 0);
    glEnd();

    //zaxis
    glBegin (GL_LINES);
    glVertex3d (0, 0, zmin);
    glVertex3d (0, 0, 0);
    glEnd();
    glBegin (GL_LINES);
    glVertex3d (0, 0, 0);
    glVertex3d (0, 0, zmax);
    glEnd();

    renderText(xmax-xmax/10.0, 0, 0, "x");
    renderText(0, ymax-ymax/10.0, 0, "y");
    renderText(0, 0, zmax-zmax/10.0, "z");
}

//end draw_the_axes

void GLWidget::draw_the_walls(void)
{
    //the computational box
    double x, y, z, dx, dy, dz;

    x = 0.0;
    y = 0.0;
    z = 0.0;
    dx = xmax;
    dy = ymax;
    dz = zmax;

    //left x-face
    glBegin(GL_LINE_LOOP);
    glVertex3f(x, y, z);
    glVertex3f(x, y, z + dz);
    glVertex3f(x, y + dy, z + dz);
    glVertex3f(x, y + dy, z);
    glEnd();
    //right x-face
    glBegin(GL_LINE_LOOP);
    glVertex3f(x + dx, y, z);
    glVertex3f(x + dx, y, z + dz);
    glVertex3f(x + dx, y + dy, z + dz);
    glVertex3f(x + dx, y + dy, z);
    glEnd();

    //lower y-face
    glBegin(GL_LINE_LOOP);
    glVertex3f(x, y, z);
    glVertex3f(x + dx, y, z);
    glVertex3f(x + dx, y, z + dz);
    glVertex3f(x, y, z + dz);
    glEnd();
    //upper y-face
    glBegin(GL_LINE_LOOP);
    glVertex3f(x, y + dy, z);
    glVertex3f(x + dx, y + dy, z);
    glVertex3f(x + dx, y + dy, z + dz);
    glVertex3f(x, y + dy, z + dz);
    glEnd();

    //rear z-face
    glBegin(GL_LINE_LOOP);
    glVertex3f(x, y, z);
    glVertex3f(x + dx, y, z);
    glVertex3f(x + dx, y + dy, z);
    glVertex3f(x, y + dy, z);
    glEnd();
    //front z-face
    glBegin(GL_LINE_LOOP);
    glVertex3f(x, y, z + dz);
    glVertex3f(x, y + dy, z + dz);
    glVertex3f(x + dx, y + dy, z + dz);
    glVertex3f(x + dx, y, z + dz);
    glEnd();

    //cross on the x=0 face
    glBegin (GL_LINES);
    glVertex3d (0, dy/2, 0);
    glVertex3d (0, dy/2, dz);
    glEnd();
    glBegin (GL_LINES);
    glVertex3d (0, 0, dz/2);
    glVertex3d (0, dy, dz/2);
    glEnd();
    //cross on the y=0 face
    glBegin (GL_LINES);
    glVertex3d (dx/2, 0, 0);
    glVertex3d (dx/2, 0, dz);
    glEnd();
    glBegin (GL_LINES);
    glVertex3d (0, 0, dz/2);
    glVertex3d (dx, 0, dz/2);
    glEnd();
    //cross on the z=0 face
    glBegin (GL_LINES);
    glVertex3d (dx/2, 0, 0);
    glVertex3d (dx/2, dy, 0);
    glEnd();
    glBegin (GL_LINES);
    glVertex3d (0, dy/2, 0);
    glVertex3d (dx, dy/2, 0);
    glEnd();

}

//end draw_the_walls

void GLWidget::draw_trajectory()
{
    int i;

    glBegin(GL_LINE_STRIP);

    for (i = 1; i < max_points_in_traj; i++) {
        glVertex3f((trajectory.trajpoint[i]).x,
                   (trajectory.trajpoint[i]).y,
                   (trajectory.trajpoint[i]).z);
    }

    glEnd();

}

//end draw_trajectory

//end file
