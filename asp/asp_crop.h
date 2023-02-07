/** 
 * \file    asp_crop.h
 * \brief   Image subsampling and cropping operations
 * \author  Brian Bue, David R Thompson
 *
 * Copyright 2014 California Institute of Technology.  ALL RIGHTS RESERVED.
 * U.S. Government Sponsorship acknowledged.
 */

#ifndef ASP_CROP_H
#define ASP_CROP_H

#include "asp_image2d.h"


int
asp_subsample_image(asp_image2d_t *orig, 
                    asp_image2d_t **subs,
                    asp_image2d_t *counts,
                    const int factor);

int
asp_crop_image(asp_image2d_t *orig, 
               asp_image2d_t **crop,
               const int top, const int bottom,
               const int left, const int right);

 
int
asp_search_image(asp_image2d_t *img, 
                 const int top, const int bottom,
                 const int left, const int right,
                 float *row, float *col);

int
asp_search_crop_image(asp_image2d_t *orig, int *box_dims, 
                      const int crop_width, const int crop_height,
                      const int top, const int bottom,
                      const int left, const int right);


int
asp_subtract_image(asp_image2d_t *origA, 
                   asp_image2d_t *origB, 
                   asp_image2d_t **result,
		               int newFixPt);

int
asp_subtract_image_offset(asp_image2d_t *origA, 
                          asp_image2d_t *origB, 
                          asp_image2d_t **result,
                          int* shift,
			                    int newFixPt);


#endif
