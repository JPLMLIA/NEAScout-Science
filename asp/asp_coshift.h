/**
 * \file    asp_coshift.c
 * \brief  Find the optimal shift 
 * \author  William Huffman, Brian Bue, and David R Thompson
 *
 * Copyright 2015 California Institute of Technology.  ALL RIGHTS RESERVED.
 * U.S. Government Sponsorship acknowledged.
 */

#ifndef asp_coshift_h
#define asp_coshift_h

#include "asp_image2d.h"
#include "asp_bbox.h"

double 
asp_xcorr_image2d(asp_image2d_t* img1,
                  asp_image2d_t* img2,
                  asp_bbox_t* bbox1,
                  asp_bbox_t* bbox2,
                  const double scaling);


int 
asp_find_shift_image2d(int* shift, 
                       asp_image2d_t* img1, 
                       asp_image2d_t* img2,
                       asp_costack_t* stack, 
                       asp_bbox_t* bbox1,
                       asp_bbox_t* bbox2, 
                       const double scaling);

#endif
