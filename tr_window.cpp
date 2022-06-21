/*
 * tr_window.cpp
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

//Last modified 21 June 2022

#include "tr.h"
#include "tr_externs.h"

Window::Window(QMainWindow *parent) : QWidget(parent)
{
     creatAllSliders ();
     creatAllOptions();

     //this is the splitter containing the two components - tab and gl window
     QSplitter *splitter = new QSplitter(this);

     //this is a container for the tabs to the left of the GL window
     tabcontainer = new QTabWidget;
     tabcontainer->setMaximumWidth(250);
     //this is the GL window on the right
     glWidget = new GLWidget ();

     //this is the view tab (rotation, translation, zoom and section sliders)
     SliderTab = new QWidget (this);
     //this is the slider tab layout
     QVBoxLayout *vLayout = new QVBoxLayout;
          //reset button
          vLayout->addWidget(b1);
          //rotate
          vLayout->addWidget(rotlb);
          vLayout->addWidget(xSlider);
          vLayout->addWidget(ySlider);
          vLayout->addWidget(zSlider);
          //translate
          vLayout->addWidget(shftlb);
          vLayout->addWidget(xSh);
          vLayout->addWidget(ySh);
          vLayout->addWidget(zSh);
          //zoom
          vLayout->addWidget(zoomlb);
          vLayout->addWidget(zoomSlider);

          //background color button
          vLayout->addWidget(b3);
          //traj color button
          vLayout->addWidget(b4);

          vLayout->addStretch();
     SliderTab->setLayout(vLayout);

     //this is the options tab
     //replot button, time
     //Exyz, dExyz, Bxyz, dBx, checkboxes, Pxyz
     OptionTab = new QWidget ();
     //this is the options tab layout
     QVBoxLayout *oLayout = new QVBoxLayout;
          //Replot button
          oLayout->addWidget(b2);

          //trajectory time text entry
          QHBoxLayout *oh4 = new QHBoxLayout;
          //oh4->addWidget(new QLabel ("Time"));
          oh4->addWidget(t4lb);
          oh4->addWidget(t4);

          //electron xyz position
          QHBoxLayout *oh3 = new QHBoxLayout;
          oh3->addWidget(t8a);//x
          oh3->addWidget(t8b);//y
          oh3->addWidget(t8c);//z

          //Exyz
          QHBoxLayout *oh5 = new QHBoxLayout;
          oh5->addWidget(t5a);//Ex
          oh5->addWidget(t5b);//Ey
          oh5->addWidget(t5c);//Ez

          //dExyz
          QHBoxLayout *oh1 = new QHBoxLayout;
          oh1->addWidget(t1a);//dEx
          oh1->addWidget(t1b);//dEy
          oh1->addWidget(t1c);//dEz

          //Bxyz
          QHBoxLayout *oh6 = new QHBoxLayout;
          oh6->addWidget(t6a);//Bx
          oh6->addWidget(t6b);//By
          oh6->addWidget(t6c);//Bz

          //dBx
          QHBoxLayout *oh2 = new QHBoxLayout;
          oh2->addWidget(t2lb);//dBx
          oh2->addWidget(t2a);

          QButtonGroup *bxg = new QButtonGroup;
          bxg->addButton(bxx);
          bxg->addButton(bxy);
          bxg->addButton(bxz);

          oh2->addWidget(bxx);
          oh2->addWidget(bxy);
          oh2->addWidget(bxz);

          //Pxyz
          QHBoxLayout *oh7 = new QHBoxLayout;
          oh7->addWidget(t7a);//px
          oh7->addWidget(t7b);//py
          oh7->addWidget(t7c);//pz

          //now add all the items to the layout
          oLayout->addItem(oh4);
          oLayout->addWidget(t8lb);//xyz label
          oLayout->addItem(oh3);
          oLayout->addWidget(t5lb);//Exyz label
          oLayout->addItem(oh5);
          oLayout->addWidget(t1lb);//dExyz label
          oLayout->addItem(oh1);

          oLayout->addWidget(t6lb);//Bxyz label
          oLayout->addItem(oh6);
          oLayout->addItem(oh2);

          oLayout->addWidget(t7lb);//Pxyz label
          oLayout->addItem(oh7);

          oLayout->addWidget(new QLabel("Trajectory Data"));
          oLayout->addWidget(result);//shows trajectory data

          oLayout->addStretch();
     OptionTab->setLayout(oLayout);

     //add the tabs to the tabcontainer
     tabcontainer->addTab (OptionTab, tr("&Options"));
     tabcontainer->addTab (SliderTab, tr("&View"));

     //add the tabcontainer and the GL window to the splitter
     splitter->addWidget(tabcontainer);
     splitter->addWidget(glWidget);

     //put the splitter as the main layout of the gui
     QGridLayout *mainLayout = new QGridLayout;
     mainLayout->addWidget(splitter, 0, 0);
     setLayout(mainLayout);

     //attach the signals and slots
     set_connections();
}

void Window::keyPressEvent(QKeyEvent *e)
{
     if (e->key() == Qt::Key_Escape)
          ;//trap the ESC key
     else
          QWidget::keyPressEvent(e);
}

void Window::creatAllSliders (void)
{

     b1 = new QPushButton (tr("&Reset"));
     b1->setToolTip(tr("Click to reset the view to its default orientation"));

     rotlb = new QLabel (tr("Rotate"));
     xSlider = createRotSlider();
     xSlider->setToolTip(tr("Rotate view around the x-axis"));
     xSlider->setValue(0);

     ySlider = createRotSlider();
     ySlider->setToolTip(tr("Rotate view around the y-axis"));
     ySlider->setValue(0);

     zSlider = createRotSlider();
     zSlider->setToolTip(tr("Rotate view around the z-axis"));
     zSlider->setValue(0);

     shftlb = new QLabel (tr("Translate"));
     xSh = createShiftSlider();
     xSh->setToolTip(tr("Shift view along the x-axis"));
     xSh->setValue(0);

     ySh = createShiftSlider();
     ySh->setToolTip(tr("Shift view along the y-axis"));
     ySh->setValue(0);

     zSh = createShiftSlider();
     zSh->setToolTip(tr("Shift view along the z-axis"));
     zSh->setValue(0);

     zoomlb = new QLabel (tr("Zoom"));
     zoomSlider = createZoomSlider();
     zoomSlider->setToolTip(tr("Adjust the magnification"));
     zoomSlider->setValue(1.0);

}

QSlider *Window::createRotSlider()
{
     QSlider *slider = new QSlider(Qt::Horizontal);
     slider->setRange(0, 360);
     slider->setSingleStep(1);
     slider->setPageStep(15);
     slider->setTickInterval(10);
     return slider;
}

QSlider *Window::createZoomSlider()
{
     QSlider *slider = new QSlider(Qt::Horizontal);
     slider->setRange(1, 100000);
     slider->setSingleStep(100);
     slider->setPageStep(1000);
     slider->setTickInterval(1000);
     return slider;
}

QSlider *Window::createShiftSlider()
{
     QSlider *slider = new QSlider(Qt::Horizontal);
     slider->setRange(-100, 100);
     slider->setSingleStep(1);
     slider->setPageStep(5);
     slider->setTickInterval(5);
     return slider;
}

void Window::creatAllOptions()
{
     b2 = new QPushButton (tr("&Replot"));
     b2->setToolTip(tr("Click to redraw the trajectory after modifying any physical quantitiy"));

     t1lb = new QLabel ("dEx, dEy, dEz");

     t1a = new QLineEdit();
     t1a->setPlaceholderText("0");
     t1a->setToolTip(tr("Change in the X-component of the external electric field\nper cm across the x-direction."));

     t1b = new QLineEdit();
     t1b->setPlaceholderText("0");
     t1b->setToolTip(tr("Change in the y-component of the external electric field\nper cm across the y-direction."));

     t1c = new QLineEdit();
     t1c->setPlaceholderText("0");
     t1c->setToolTip(tr("Change in the z-component of the external electric field\nper cm across the z-direction."));

     t2lb = new QLabel ("dBx");
     t2a = new QLineEdit();
     t2a->setPlaceholderText("0");
     t2a->setToolTip(tr("Change in the X-component of the external magnetic field\nper cm across the checked direction."));

     bxx = new QCheckBox ("x");
     bxx->setChecked(true);
     bxx->setToolTip("Bx varies along the x-direction");
     bxy = new QCheckBox ("y");
     bxy->setChecked(false);
     bxy->setToolTip("Bx varies along the y-direction");
     bxz = new QCheckBox ("z");
     bxz->setChecked(false);
     bxz->setToolTip("Bx varies along the z-direction");

     t4 = new QLineEdit();
     t4lb  = new QLabel ("&Time");
     t4lb->setBuddy(t4);
     t4->setToolTip(tr("Time upto which the trajectory is computed (seconds)."));
     t4->setMaxLength(10);
     t4->setPlaceholderText("0");

     t5lb = new QLabel ("Ex, Ey, Ez");
     t5a = new QLineEdit();
     t5a->setPlaceholderText("0");
     t5a->setToolTip(tr("X-component of the external electric field (volts/cm)."));

     t5b = new QLineEdit();
     t5b->setPlaceholderText("0");
     t5b->setToolTip(tr("Y-component of the external electric field (volts/cm)."));

     t5c = new QLineEdit();
     t5c->setPlaceholderText("0");
     t5c->setToolTip(tr("Z-component of the external electric field (volts/cm)."));

     t6lb = new QLabel ("Bx, By, Bz");
     t6a = new QLineEdit();
     t6a->setPlaceholderText("0");
     t6a->setToolTip(tr("X-component of the external magnetic field (gauss)."));

     t6b = new QLineEdit();
     t6b->setPlaceholderText("0");
     t6b->setToolTip(tr("Y-component of the external magnetic field (gauss)."));

     t6c = new QLineEdit();
     t6c->setPlaceholderText("0");
     t6c->setToolTip(tr("Z-component of the external magnetic field (gauss)."));

     t7lb = new QLabel ("Px, Py, Pz");
     t7a = new QLineEdit();
     t7a->setPlaceholderText("0");
     t7a->setToolTip(tr("X-component of the initial electron momentum (gm. cm. per sec)."));

     t7b = new QLineEdit();
     t7b->setPlaceholderText("0");
     t7b->setToolTip(tr("Y-component of the initial electron momentum (gm. cm. per sec)."));

     t7c = new QLineEdit();
     t7c->setPlaceholderText("0");
     t7c->setToolTip(tr("Z-component of the initial electron momentum (gm. cm. per sec)."));

     t8lb = new QLabel ("x, y, z");
     t8a = new QLineEdit();
     t8a->setPlaceholderText("0.0");
     t8a->setToolTip(tr("X-position of the electron (cm)."));

     t8b = new QLineEdit();
     t8b->setPlaceholderText("5.0");
     t8b->setToolTip(tr("Y-position of the electron (cm)."));

     t8c = new QLineEdit();
     t8c->setPlaceholderText("0.0");
     t8c->setToolTip(tr("Z-position of the electron (cm)."));

     result = new QTextEdit ();
     result->setReadOnly (true);
     result->setToolTip(tr("Trajectory data is displayed here"));

     b3 = new QPushButton (tr("Back&ground color"));
     b3->setToolTip(tr("Click to change the background color"));

     b4 = new QPushButton (tr("Tra&jectory color"));
     b4->setToolTip(tr("Click to change the color of the trajectory"));
}

void Window::set_connections (void)
{
     connect(xSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setXRotation(int)));
     connect(glWidget, SIGNAL(xRotationChanged(int)), xSlider, SLOT(setValue(int)));

     connect(ySlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setYRotation(int)));
     connect(glWidget, SIGNAL(yRotationChanged(int)), ySlider, SLOT(setValue(int)));

     connect(zSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setZRotation(int)));
     connect(glWidget, SIGNAL(zRotationChanged(int)), zSlider, SLOT(setValue(int)));

     connect(xSh, SIGNAL(valueChanged(int)), glWidget, SLOT(setXShift(int)));
     connect(ySh, SIGNAL(valueChanged(int)), glWidget, SLOT(setYShift(int)));
     connect(zSh, SIGNAL(valueChanged(int)), glWidget, SLOT(setZShift(int)));

     connect(zoomSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setZoom(int)));
     connect(glWidget, SIGNAL(zoomChanged(int)), zoomSlider, SLOT(setValue(int)));

     connect(b1, SIGNAL(clicked(bool)), glWidget, SLOT(set_reset()));
     connect(b2, SIGNAL(clicked(bool)), glWidget, SLOT(replot()));
     connect(b3, SIGNAL(clicked(bool)), glWidget, SLOT(change_background_color()));
     connect(b4, SIGNAL(clicked(bool)), glWidget, SLOT(change_traj_color()));

     connect (t4, SIGNAL(textChanged(QString)), this, SLOT(setmaxtime(QString)));

     connect (t8a, SIGNAL(textChanged(QString)), this, SLOT(setx(QString)));
     connect (t8b, SIGNAL(textChanged(QString)), this, SLOT(sety(QString)));
     connect (t8c, SIGNAL(textChanged(QString)), this, SLOT(setz(QString)));

     connect (t5a, SIGNAL(textChanged(QString)), this, SLOT(setEx(QString)));
     connect (t5b, SIGNAL(textChanged(QString)), this, SLOT(setEy(QString)));
     connect (t5c, SIGNAL(textChanged(QString)), this, SLOT(setEz(QString)));

     connect (t1a, SIGNAL(textChanged(QString)), this, SLOT(setdEx(QString)));
     connect (t1b, SIGNAL(textChanged(QString)), this, SLOT(setdEy(QString)));
     connect (t1c, SIGNAL(textChanged(QString)), this, SLOT(setdEz(QString)));

     connect (t6a, SIGNAL(textChanged(QString)), this, SLOT(setBx(QString)));
     connect (t6b, SIGNAL(textChanged(QString)), this, SLOT(setBy(QString)));
     connect (t6c, SIGNAL(textChanged(QString)), this, SLOT(setBz(QString)));

     connect (bxx, SIGNAL(stateChanged(int)), this, SLOT(setdbxx(int)));
     connect (bxy, SIGNAL(stateChanged(int)), this, SLOT(setdbxy(int)));
     connect (bxz, SIGNAL(stateChanged(int)), this, SLOT(setdbxz(int)));

     connect (t2a, SIGNAL(textChanged(QString)), this, SLOT(setdBx(QString)));

     connect (t7a, SIGNAL(textChanged(QString)), this, SLOT(setPx(QString)));
     connect (t7b, SIGNAL(textChanged(QString)), this, SLOT(setPy(QString)));
     connect (t7c, SIGNAL(textChanged(QString)), this, SLOT(setPz(QString)));

     connect(glWidget, SIGNAL(update_result()), this, SLOT(update_res()));
}

void Window::set_gui()
{
     t4->setText(QString("%1").arg(t2));
     xSh->setValue(0);
     ySh->setValue(0);
     zSh->setValue(0);

     t8a->setText(QString ("%1").arg(raystart.x));
     t8b->setText(QString ("%1").arg(raystart.y));
     t8c->setText(QString ("%1").arg(raystart.z));

     t7a->setText(QString ("%1").arg(extP.x));
     t7b->setText(QString ("%1").arg(extP.y));
     t7c->setText(QString ("%1").arg(extP.z));

     t6a->setText(QString ("%1").arg(extB.x));
     t6b->setText(QString ("%1").arg(extB.y));
     t6c->setText(QString ("%1").arg(extB.z));

     t5a->setText(QString ("%1").arg(extE.x));
     t5b->setText(QString ("%1").arg(extE.y));
     t5c->setText(QString ("%1").arg(extE.z));



}

void Window::update_res()
{
QString s;
int i;

    result->clear();

    s = QString ("Total time=%1 sec").arg(t);
    result->append(s);

    s = QString ("KE in ergs:");
    result->append(s);

    s = QString ("Start=%1").arg(trajectory.initen);
    result->append(s);

    s = QString ("End=%1").arg(trajectory.finalen);
    result->append(s);

    s = QString ("Momentum in g-cm/sec:");
    result->append(s);

    s = QString ("Start=%1").arg(trajectory.initmom);
    result->append(s);

    s = QString ("End=%1").arg(trajectory.finalmom);
    result->append(s);

    s = QString ("Coordinates in cm:");
    result->append(s);

    s = QString ("Start=%1, %2, %3").
            arg(trajectory.trajpoint[0].x).
            arg(trajectory.trajpoint[0].y).
            arg(trajectory.trajpoint[0].z);
    result->append(s);

    i = max_points_in_traj - 1;
    s = QString ("End=%1, %2, %3").
            arg(trajectory.trajpoint[i].x).
            arg(trajectory.trajpoint[i].y).
            arg(trajectory.trajpoint[i].z);
    result->append(s);

}

void Window::setmaxtime(QString s)
{
     t2 = s.toDouble();
}

void Window::setx(QString s)
{
     raystart.x = s.toDouble();
}

void Window::sety(QString s)
{
     raystart.y = s.toDouble();
}

void Window::setz(QString s)
{
     raystart.z = s.toDouble();
}

void Window::setEx(QString s)
{
     extE.x = s.toDouble();
     //convert to statvolts per cm
     extE.x = extE.x / 300.0;
}

void Window::setEy(QString s)
{
     extE.y = s.toDouble();
     //convert to statvolts per cm
     extE.y = extE.y / 300.0;
}

void Window::setEz(QString s)
{
     extE.z = s.toDouble();
     //convert to statvolts per cm
     extE.z = extE.z / 300.0;
}

void Window::setdEx(QString s)
{
     dE.x = s.toDouble();
     //convert to statvolts per cm
     dE.x = dE.x / 300.0;
}

void Window::setdEy(QString s)
{
     dE.y = s.toDouble();
     //convert to statvolts per cm
     dE.y = dE.y / 300.0;
}

void Window::setdEz(QString s)
{
     dE.z = s.toDouble();
     //convert to statvolts per cm
     dE.z = dE.z / 300.0;
}

void Window::setBx(QString s)
{
     extB.x = s.toDouble();
}

void Window::setBy(QString s)
{
     extB.y = s.toDouble();
}

void Window::setBz(QString s)
{
     extB.z = s.toDouble();
}

void Window::setdBx(QString s)
{
     dB.x = s.toDouble();
}

void Window::setdBy(QString s)
{
     dB.y = s.toDouble();
}

void Window::setdBz(QString s)
{
     dB.z = s.toDouble();
}

void Window::setdbxx(int ch)
{
     if (ch == Qt::Checked)
          bxxch = true;
     else
          bxxch = false;
}

void Window::setdbxy(int ch)
{
     if (ch == Qt::Checked)
          bxych = true;
     else
          bxych = false;
}

void Window::setdbxz(int ch)
{
     if (ch == Qt::Checked)
          bxzch = true;
     else
          bxzch = false;
}

void Window::setPx(QString s)
{
     extP.x = s.toDouble();
}

void Window::setPy(QString s)
{
     extP.y = s.toDouble();
}

void Window::setPz(QString s)
{
     extP.z = s.toDouble();
}
