/**
 * \file    asp_bbox.h
 * \brief   Reference stars for NEASCOUT
 * \author  William Huffman, Brian Bue, and David R Thompson
 *
 * Copyright 2015 California Institute of Technology.  ALL RIGHTS RESERVED.
 * U.S. Government Sponsorship acknowledged.
 */

#ifndef asp_bbox_h
#define asp_bbox_h

#include "asp_image2d.h"

/**
 * This structure represents a rectangular bbox of an image2d.  The struct 
 * does *not* own the underlying image data, but is merely a light wrapper 
 * around it to designate a rectangular bbox.
 * 
 * Members:
 *  top: the first row of the bbox
 *  left: the first column of the bbox
 *  height: the number of rows in the bbox
 *  width: the number of columns in the bbox
 *  size: the number of pixels in the bbox, height*width
 */
typedef struct {
  int top;
  int left;
  int height;
  int width;
} asp_bbox_t;


double 
asp_bbox_mean(asp_image2d_t* img, asp_bbox_t* bbox); 

double 
asp_bbox_std(asp_image2d_t* img, double mean, asp_bbox_t* bbox);

#endif
