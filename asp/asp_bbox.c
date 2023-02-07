/**
 * \file    asp_bbox.c
 * \brief   Reference stars for NEASCOUT
 * \author  William Huffman, Brian Bue, David R Thompson, Jack Lightholder
 *
 * Copyright 2015 California Institute of Technology.  ALL RIGHTS RESERVED.
 * U.S. Government Sponsorship acknowledged.
 */

#include <math.h>

#include "asp_bbox.h"
#include "asp_image2d.h"
#include "asp_error.h"


  double asp_bbox_mean(asp_image2d_t* img, asp_bbox_t* bbox) 
  {
    /*
     * Calculate mean value inside bounding box of an image.
     *
     *
     * @param img pointer to image with pixel data
     * @param bbox bounding box dimensions for where to calculate mean
     *
     * @return mean mean value of the pixels in the bounding box
     *
     */
    double accumulator = 0.0;
    double bbox_size = (double)bbox->height * (double)bbox->width;
    int r;
    int c;

    // Sum each pixel in bounding box.  Divide each by image fixed point scaler before summation.
    for(r = bbox->top; r < bbox->top + bbox->height; r++) {
        for(c = bbox->left; c < bbox->left + bbox->width; c++) {
            accumulator += (double)(*asp_image2d_get_pixel(img, c, r)) / (double)ASP_NONZERO(img->fixedPoint);
        }
    }

    // Divide by number of pixels for mean calculation. Returns mean of pixels.
    return accumulator / (double)ASP_NONZERO(bbox_size);
  }



  double asp_bbox_std(asp_image2d_t* img, double mean, asp_bbox_t* bbox) 
  {
    /*
     * Calculate standard deviation value inside bounding box of an image.
     *
     *
     * @param img pointer to image with pixel data
     * @param bbox bounding box dimensions for where to calculate standard deviation
     *
     * @return standard_deviation standard deviation value of the pixels in the bounding box
     *
     */
    double accumulator = 0.0;
    double diff = 0.0; 
    double result = 1.0;
    double bbox_size = (double)bbox->height * (double)bbox->width;
    int r;
    int c;

    // Sum each pixel in bounding box. Divide each by image fixed point scalar before mean subtraction. Square and accumulate.
    for(r = bbox->top; r < bbox->top + bbox->height; r++) {
        for(c = bbox->left; c < bbox->left + bbox->width; c++) {
            diff =  ((double)(*asp_image2d_get_pixel(img, c, r)) / (double)ASP_NONZERO(img->fixedPoint)) - mean;
            accumulator += diff * diff;
        }
    }

    // Square root of accumulated value and divide by number of pixels. Returns std of pixels.
    accumulator /= (double)ASP_NONZERO(bbox_size);
    if(accumulator > 0){
        result = (double)sqrt(accumulator);
    }

    return result;
  }


