/** 
 * \file    asp_gsmooth.h
 * \brief   approximate Gaussian smoothing
 * \author  David R Thompson, based on gsmooth_border_ring.c by Mike Burl, 
 *          Steve Schaeffer, and others on the AEGIS team.
 *
 * Copyright 2015 California Institute of Technology.  ALL RIGHTS RESERVED.
 * U.S. Government Sponsorship acknowledged.
 */

#ifndef ASP_GSMOOTH_H
#define ASP_GSMOOTH_H

#include "asp_common.h"
#include "asp_image2d.h"

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>


int 
asp_smooth_image(asp_image2d_t *orig, 
                 asp_image2d_t **smoothed, 
                 asp_image2d_t *scratch,
                 const int width, 
                 const double sigma, 
                 const double scale, 
                 double *kernel);


#endif
