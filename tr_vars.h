/*
 * tr_vars.h
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

#ifndef TR_VARS_H
#define TR_VARS_H

#include "tr.h"

//x,y,z,px,py,pz
double y[7];
//vx,vy,vz,Fx,Fy,Fz
double dydt[7];
//mover particle from t to ti in RK4
double t, ti ;
//physical constants
double e, m0, m, c, csqr, vsqr, psqr, m0csqr, m0sqrc4;
//Electric and magnetic fields
struct POINT E, B, extE, extB, extP, dE, dB;
//starting coordinates of trajectory
struct POINT raystart;
//max number of points in trajectory
int max_points_in_traj;
//string to hold message from RK4
char err_string[50];
//0 or 1
bool bxxch, bxych, bxzch;
//Trajectory calculated upto this (user supplied) end time
double t2;
//computed trajectory data
struct TRAJECTORY trajectory;
//dimensions of the onscreen box
double xmax, ymax, zmax;
double xmin, ymin, zmin;
//0 or 1
bool trajcomputed;

//clear trajectory data prior to computing it
void erase_trajectories (void);
//compute the trajectory
void compute_trajectory(void);
//default values at start of program
void initialise_default_system();
//get the E and B fields acting on the electron
void get_field (const double *y);
//prints error message from RK4
void error_message(char *error_text);
//saves trajectory data to file
void savetrajfile (void);
//computes the velocity vx,vy,vz, of the electron and the force Fx,Fy,Fz on the electron
int derivs (double t, const double *, double *, void *p);

#endif // TR_VARS_H
