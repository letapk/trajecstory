/*
 * tr_main.cpp
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
#include "tr_vars.h"

int main(int argc, char *argv[])
{
     QApplication app(argc, argv);

     MainWindow mainwindow;
     mainwindow.show();
     mainwindow.setWindowTitle(QObject::tr("Trajecstory 0.4"));

     return app.exec();
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow (parent)
{
     //initial window size
     this->setGeometry(0, 0, 1024, 600);
     window = new Window(this);
     setCentralWidget(window);
     setup_menu_and_toolbar ();
     statusBar()->showMessage(tr("Ready"));

     initialise_default_system();
}

MainWindow::~MainWindow()
{

}

void MainWindow::setup_menu_and_toolbar (void)
{
     //main menu
     filemenu = menuBar()->addMenu(tr("&File"));
     computemenu = menuBar()->addMenu(tr("&Compute"));
     helpmenu = menuBar()->addMenu(tr("&Help"));

     //toolbar
     tb1 = addToolBar(tr("File"));
     tb1->setFloatable (false);
     tb1->setMovable(false);

     //file menu items
     QAction *save = new QAction(tr("&Save"), this);
     filemenu->addAction(save);
     connect(save, SIGNAL(triggered()), this, SLOT(save()));

     QAction *quit = new QAction(tr("E&xit"), this);
     filemenu->addAction(quit);
     connect(quit, SIGNAL(triggered()), this, SLOT(quit()));

     //compute menu items
     QIcon computetrajIcon = QIcon::fromTheme("", QIcon(":/images/tr.png"));
     QAction *computetraj= new QAction(tr("&Trajectory"), this);
     computetraj->setToolTip(tr("Compute trajectory"));
     computemenu->addAction(computetraj);
     computetraj->setIcon(computetrajIcon);
     connect(computetraj, SIGNAL(triggered()), window->glWidget, SLOT(replot()));

     //help menu
     QAction *helpitem = new QAction(tr("&Help"), this);
     helpmenu->addAction(helpitem);
     connect(helpitem, SIGNAL(triggered()), this, SLOT(help()));
     QAction *aboutitem = new QAction(tr("&About"), this);
     helpmenu->addAction(aboutitem);
     connect(aboutitem, SIGNAL(triggered()), this, SLOT(about()));

     helpmenu->addSeparator();

     QAction *aboutQtitem = new QAction(tr("About &Qt"), this);
     helpmenu->addAction(aboutQtitem);
     connect(aboutQtitem, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

     tb1->addAction (computetraj);

     connect (this, SIGNAL(set_gui()), window, SLOT(set_gui()));
     connect (this, SIGNAL(set_gui()), window->b1, SLOT(click()));

     connect (this, SIGNAL(replot()), window->glWidget, SLOT(replot()));
}

void MainWindow::save()
{
int i;
FILE *outfile;
char tempstring[128];

    if (trajcomputed == false) {
        statusBar()->showMessage(tr("Nothing to save"));
        return;
    }

    strcpy (tempstring, "default.dat.trajectories");

    outfile = fopen (tempstring, "w");
    fprintf (outfile, "%.5e %.5e %.5e %.5e %.5e %.5e %.5e %.5e %.5e %.5e %i %i %i\n",
             extE.x*300.0, extE.y*300.0, extE.z*300.0,
             dE.x*300.0, dE.y*300.0, dE.z*300.0,
             extB.x, extB.y, extB.z,
             dB.x, bxxch, bxych, bxzch);

    for (i = 0; i < max_points_in_traj; i++){
        fprintf (outfile, "%i %.5e %.5e %.5e %.5e\n",
                 i,
                 (trajectory.time[i]),
                 (trajectory.trajpoint[i]).x,
                 (trajectory.trajpoint[i]).y,
                 (trajectory.trajpoint[i]).z
                 );
    }
    fprintf (outfile, "\n");
    fclose (outfile);

    statusBar()->showMessage(tr("Trajectory saved in file \"default.dat.trajectories\""));
}

void MainWindow::quit()
{
     exit(0);
}

void MainWindow::help()
{
     QFile helpfile ("./trhelp.pdf");
     if (helpfile.exists() == false) {
          QMessageBox msgBox;
          msgBox.setIcon(QMessageBox::Information);
          msgBox.setText(tr("The help file was not found"));
          msgBox.exec();
     }
     else {
          QDesktopServices::openUrl (QUrl ("./trhelp.pdf"));
     }
}

void MainWindow::about()
//open a window to show program information and copyright license
{
     QFile file("COPYING");
     QTextStream in(&file);
     QTextBrowser *gnugpl;
     bool ok;

     ok = file.open(QFile::ReadOnly);
     if (ok == false)
          return;

     gnugpl = new QTextBrowser ();
     gnugpl->setGeometry(100, 100, 800, 600);
     gnugpl->setWindowTitle (QObject::tr("About Trajecstory"));
     gnugpl->setPlainText(in.readAll());
     gnugpl->setAlignment(Qt::AlignLeft);
     gnugpl->show();
}

void MainWindow::initialise_default_system()
{
    raystart.x = 0.0;
    raystart.y = 5.0;
    raystart.z = 0.0;

    extP.x = 1e-19;
    extP.y = 1e-19;
    extP.z = 1e-19;


    extE.x = extE.y = extE.z = 0.0;

    E.x = extE.x;
    E.y = extE.y;
    E.z = extE.z;

    extB.x = B.x = 5.0;
    extB.y = extB.z = B.y = B.z = 0.0;

    dE.x = dE.y = dE.z = 0.0;
    dB.x = dB.y = dB.z = 0.0;

    xmin = ymin = zmin = 0.0;
    xmax = 10.0;
    ymax = 10.0;
    zmax = 10.0;

    t2 = 5e-07;

    emit set_gui();
}


