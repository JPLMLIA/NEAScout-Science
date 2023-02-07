/** 
 * \file    asp_calib.h
 * \brief   L1 Pipeline for NEASCOUT
 * \author  Brian Bue, David R Thompson, Jack Lightholder
 *
 * Copyright 2014 California Institute of Technology.  ALL RIGHTS RESERVED.
 * U.S. Government Sponsorship acknowledged.
 */

#ifndef ASP_CALIB_H
#define ASP_CALIB_H

#include "asp_image2d.h"

int
asp_calib(asp_image2d_t *img,
	      asp_image2d_t *m_array,
          asp_image2d_t *b_array, 
          asp_image2d_t *dk,
          asp_image2d_t *bad, 
          const float rcc, 
          const int fixPt,
          int temperature);


int
asp_dynamic_range(asp_image2d_t *img, 
                  int *px_min, 
                  int *px_max);


#endif // ASP_CALIB_H
