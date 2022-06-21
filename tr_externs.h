/*
 * tr_externs.h
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

//Last modified 25 June 2017

#ifndef TR_EXTERNS_H
#define TR_EXTERNS_H

#include "tr.h"

extern double y[];
extern double dydt[];
extern double t, ti ;
extern double e, m0, m, c, csqr, vsqr, psqr, m0csqr, m0sqrc4;
extern struct POINT E, B, extE, extB, extP, dE, dB;
extern struct POINT raystart;
extern int max_points_in_traj;
extern char err_string[];
extern bool bxxch, bxych, bxzch;
extern double t2;
extern struct TRAJECTORY trajectory;
extern double xmax, ymax, zmax;
extern double xmin, ymin, zmin;
extern bool trajcomputed;

extern void erase_trajectories (void);
extern void compute_trajectory(void);
extern void initialise_default_system();
extern void get_field (const double *y);
extern void error_message(char *error_text);
extern void savetrajfile (void);
extern int derivs (double t, const double *, double *, void *p);

#endif // TR_EXTERNS_H
