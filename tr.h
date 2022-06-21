/*
 * tr.h
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

#define MAXTRAJPOINTS 20000

#ifndef WINDOW_H
#define WINDOW_H

#include <QApplication>
#include <QtGui>
#include <QtOpenGL>

struct POINT {
     double x;
     double y;
     double z;
};

struct TRAJECTORY {
     struct POINT trajpoint[MAXTRAJPOINTS];
     double time[MAXTRAJPOINTS];
     double totaltime;
     double initmom, finalmom;
     double initen, finalen;
};

class GLWidget;
class Window;

class GLWidget : public QGLWidget
{
     Q_OBJECT

public:
     GLWidget();

public slots:

     void set_reset (void);

     void setXRotation(int angle);
     void setYRotation(int angle);
     void setZRotation(int angle);

     void setXShift(int x);
     void setYShift(int y);
     void setZShift(int z);

     void setZoom(int angle);

     void replot();
     void change_background_color();
     void change_traj_color();

signals:
     void xRotationChanged(int angle);
     void yRotationChanged(int angle);
     void zRotationChanged(int angle);

     void zoomChanged(int z);

     void update_result();

protected:
     void initializeGL();
     void paintGL();
     void mousePressEvent(QMouseEvent *event);
     void mouseMoveEvent(QMouseEvent *event);
     void wheelEvent(QWheelEvent *event);

     void draw_the_axes (void);
     void draw_the_walls(void);
     void draw_trajectory();

private:
     QColor clearcolor, trajcolor;
     int xRot;
     int yRot;
     int zRot;

     QPoint lastPos;

     double xShift;
     double yShift;
     double zShift;

     float zoom;

};

class Window : public QWidget
{
     Q_OBJECT

public:
     Window(QMainWindow *parent);
     GLWidget *glWidget;
     //this has to be public because it is referenced in MainWindow
     QPushButton *b1, *b2;

protected:
     void keyPressEvent (QKeyEvent *event);

public slots:
     void set_gui();

     void setmaxtime(QString);

     void setx(QString);
     void sety(QString);
     void setz(QString);

     void setEx(QString);
     void setEy(QString);
     void setEz(QString);

     void setdEx(QString);
     void setdEy(QString);
     void setdEz(QString);

     void setBx(QString);
     void setBy(QString);
     void setBz(QString);

     void setdBx(QString);
     void setdBy(QString);
     void setdBz(QString);

     void setdbxx (int);
     void setdbxy (int);
     void setdbxz (int);

     void setPx(QString);
     void setPy(QString);
     void setPz(QString);

     void update_res();

private:
     QTabWidget *tabcontainer;
     QWidget *OptionTab;
     QWidget *SliderTab;

     //rotation sliders along xyz
     QSlider *xSlider;
     QSlider *ySlider;
     QSlider *zSlider;
     //shifting sliders along xyz
     QSlider *xSh;
     QSlider *ySh;
     QSlider *zSh;
     //zoom slider
     QSlider *zoomSlider;

     QLabel *rotlb, *shftlb, *zoomlb;

     //contained on the Optiontab
     //b1 is public above
     QPushButton *b3, *b4;

     QLabel *t1lb, *t2lb;
     QLineEdit *t1a, *t1b, *t1c;
     QLineEdit *t2a;

     QLabel *t4lb, *t5lb, *t6lb, *t7lb, *t8lb;
     QLineEdit *t4;
     QLineEdit *t5a, *t5b, *t5c;
     QLineEdit *t6a, *t6b, *t6c;
     QLineEdit *t7a, *t7b, *t7c;
     QLineEdit *t8a, *t8b, *t8c;

     QCheckBox *bxx, *bxy, *bxz;
     QTextEdit *result;

     //functions
     void creatAllOptions(void);
     void creatAllSliders (void);
     QSlider *createRotSlider();
     QSlider *createZoomSlider();
     QSlider *createShiftSlider();

     void set_connections (void);

};

class MainWindow : public QMainWindow
{
     Q_OBJECT

     Window *window;
     //menubar
     QMenu *filemenu, *computemenu, *helpmenu;

     //toolbar and statusbar
     QToolBar *tb1;

public:
     MainWindow(QWidget *parent = 0);
     ~MainWindow ();

public slots:
     void save();
     void quit();
     void help();
     void about();


signals:
     void set_gui();
     void replot();

protected:
     void setup_menu_and_toolbar (void);
     void initialise_default_system();
};

#endif
