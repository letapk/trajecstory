/*
 * tr_traj.cpp
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

#include <gsl/gsl_errno.h>
#include <gsl/gsl_odeiv2.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void compute_trajectory(void)
{
    int i;
    double crudestep = 0;
    int status;

    //initialise physical quantities
    e = -4.8e-10;//statcoul
    c = 2.997925e10;//speed of light, cm per sec
    csqr = c * c;
    m0 =  9.1e-28;//rest mass of electron in grams
    m0csqr = m0 * csqr;
    m0sqrc4 = m0csqr * m0csqr;

    max_points_in_traj = MAXTRAJPOINTS;
    //units are cm, all
    y[1] = raystart.x;//x
    y[2] = raystart.y;//y
    y[3] = raystart.z;//z

    //units g-cm/sec, all
    //p=2.6998e-20 gm-cm/sec is equal to 0.25 eV
    y[4] = extP.x;//px
    y[5] = extP.y;//py
    y[6] = extP.z;//pz

    crudestep = t2 / max_points_in_traj;

    gsl_odeiv2_system sys = {derivs, NULL, 7, &m0};
    gsl_odeiv2_driver *d = gsl_odeiv2_driver_alloc_y_new (&sys, gsl_odeiv2_step_rk4, crudestep, crudestep, 0.0);

    psqr = y[4] * y[4] + y[5] * y[5] + y[6] * y[6];
    trajectory.initmom = sqrt (psqr);
    trajectory.initen = sqrt(psqr * csqr + m0sqrc4) - m0csqr;

    for (i = 0; i < max_points_in_traj; i++) {
        t = i * crudestep;
        ti = t + crudestep;

        (trajectory.trajpoint[i]).x = y[1];
        (trajectory.trajpoint[i]).y = y[2];
        (trajectory.trajpoint[i]).z = y[3];
        trajectory.time[i] = t;

        //printf ("%i %.5e %.5e %.5e %.5e %.5e %.5e %.5e\n", i, t, y[1], y[2], y[3], y[4], y[5], y[6]);

        status = gsl_odeiv2_driver_apply (d, &t, ti, y);
        if (status != GSL_SUCCESS){
            sprintf (err_string, "Error in RK4, return value=%d\n", status);
            error_message(err_string);
            break;
        }
    }

    trajectory.totaltime = ti;
    psqr = y[4] * y[4] + y[5] * y[5] + y[6] * y[6];
    trajectory.finalmom = sqrt (psqr);
    trajectory.finalen = sqrt(psqr * csqr + m0sqrc4) - m0csqr;

    trajcomputed = true;

    gsl_odeiv2_driver_free (d);

    return;
}

//end compute_trajectory

int derivs (double t, const double *y, double *dydt, void *p)
{
double totalenergysqr;
double m0;

    (void)(t); //avoid unused parameter warning for t
    m0 = *(double *)p; //pass rest mass as a parameter to avoid unused parameter warning for p

    get_field (y);

    //square of momentum
    psqr = y[4] * y[4] + y[5] * y[5] + y[6] * y[6];
    //E^2 = p^2.c^2+m0^2.c^4
    totalenergysqr = psqr * csqr + m0sqrc4;
    //using p.c^2 = E.v, we have velocity of particle
    vsqr = csqr - (m0sqrc4 * csqr) / totalenergysqr;
    //all this to get relativistic mass from velocity
    m = m0 / sqrt(1 - (vsqr / csqr));


    dydt[1] = y[4] / m;//(d/dt)x=vx=px/m
    dydt[2] = y[5] / m;//(d/dt)y=vy=py/m
    dydt[3] = y[6] / m;//(d/dt)z=vz=pz/m
    dydt[4] = e * E.x + e * (dydt[2] * B.z - dydt[3] * B.y) / c;//(d/dt)px
    dydt[5] = e * E.y - e * (dydt[1] * B.z - dydt[3] * B.x) / c;//(d/dt)py
    dydt[6] = e * E.z + e * (dydt[1] * B.y - dydt[2] * B.x) / c;//(d/dt)pz

    return GSL_SUCCESS;
}

//end derivs

void get_field (const double *y)
{
double intBx = 0.0;

     E.x = extE.x + dE.x * y[1];
     E.y = extE.y + dE.y * y[2];
     E.z = extE.z + dE.z * y[3];

     if (bxxch == true) {
          intBx = dB.x * y[1];
     }
     if (bxych == true) {
          intBx = dB.x * y[2];
     }
     if (bxzch == true) {
          intBx = dB.x * y[3];
     }

     B.x = extB.x + intBx;
     B.y = extB.y;
     B.z = extB.z;

}

//end get_field

void erase_trajectories (void)
{
    int i;

    for (i = 1; i < max_points_in_traj-1; i++) {
        (trajectory.trajpoint[i]).x = 0.0;
        (trajectory.trajpoint[i]).y = 0.0;
        (trajectory.trajpoint[i]).z = 0.0;
        (trajectory.time[i]) = 0.0;
    }
    trajectory.totaltime = 0;

    trajcomputed = false;
}

//end erase_trajectories

void error_message(char *error_text)
{
QMessageBox msgBox;

     msgBox.setText(error_text);
     msgBox.exec();
}

//end error_message

//end file
