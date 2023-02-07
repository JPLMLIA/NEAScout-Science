/**
 * \file    asp_comedi.c
 * \brief   Temporal median processing for NEASCOUT
 * \author  William Huffman, Brian Bue, and David R Thompson, Jack Lightholder
 *
 * Copyright 2015 California Institute of Technology.  ALL RIGHTS RESERVED.
 * U.S. Government Sponsorship acknowledged.
 */

#include "asp_comedi.h"
#include "asp_image2d.h"
#include "asp_error.h"

#include <stdlib.h>

  asp_pixel_t offsetval(asp_image2d_t* img, int xoff, int yoff, int x, int y)
  {
   /**
    *
    * Get the pixel value from an image, given an (x,y) coordinate and (x,y) offset.  
    *
    * @param img  Image to get pixel values from
    * @param xoff Offset in x (columns)
    * @param yoff Offset in y (rows)
    * @param x    Initial X, to take offset from
    * @param y    Initial Y, to take offset from
    *
    * @return pixel return the pixel value, calculated from a base coordinate and offset
    * 
    * Return 0 if the offset coordinate is out of bounds
    */

    /* Bounds check, return 0 if requested offset is out of bounds */
    if(x+xoff < 0          || 
       x+xoff >= img->cols ||
       y+yoff < 0          ||
       y+yoff >= img->rows)
          return (asp_pixel_t) 0;

    return *asp_image2d_get_pixel(img, (x+xoff), (y+yoff));
  }


  asp_pixel_t get_median(asp_pixel_t a, asp_pixel_t b, asp_pixel_t c) 
{ 
   /**
    * Take the median of the three input parameters and return that median
    *
    * @param a  First value to be factored into the median calculation
    * @param b  Second value to be factored into the median calculation
    * @param c  Third value to be factored into median calculation
    *
    * @return median median value of the three input points
    */
    if (a > b)  
    { 
        if (b > c) 
            return b; 
        else if (a > c) 
            return c; 
        else
            return a; 
    } 
    else 
    { 
        if (a > c) 
            return a; 
        else if (b > c) 
            return c; 
        else
            return b; 
    } 
} 


  int asp_add_temporal_median(asp_costack_t* stack)
  {
   /*
    *
    * This function takes a coadded stack, computes the temporal median 
    *   of the three offset images it currently holds, and adds that to the stack
    *
    * @param stack  The coadded stack for which the images are being coregistered, 
    *                   which controls the reference window size through <stack->radius>, 
    *                   the number of reference stars through <stack->numrefs>, and the 
    *                   locations of the reference stars through <stack->ref_coords_x> 
    *                   and <stack->ref_coords_y>
    *
    * @return ASP_SUCCESS success or failure flag for the function computation
    *
    */    
    asp_pixel_t p1;
    asp_pixel_t p2;
    asp_pixel_t p3;
    asp_pixel_t median;
    asp_pixel_t baseVal;
    asp_pixel_t tmpSum;
    int32_t testPx;

    /* If memory is not yet allocated, return */
    if(stack == NULL)
    {
      asp_error(asp_error_coadd_null_input, "median failed: NULL input");
      return ASP_FAILURE;
    }

    /* If memory is not yet allocated, return */
    if(stack->img1 == NULL || 
       stack->img2 == NULL || 
       stack->img3 == NULL ||
       stack->base == NULL)
    {
      asp_error(asp_error_coadd_null_input, "incomplete stack init");
      return ASP_FAILURE;
    }

    /* For each pixel coordinate, get pixel values from 3 reference images, take mean and store in base*/
    for(int x=0; x < stack->base->cols; x++)
    {
      for(int y=0; y < stack->base->rows; y++)
      {

        /* Get current positon pixel value from each stack image */
        p1 = (asp_pixel_t)ASP_ROUND((double)offsetval(stack->img1, stack->offx1, stack->offy1, x, y) / 
		            (double)ASP_NONZERO(stack->img1->fixedPoint));
        p2 = (asp_pixel_t)ASP_ROUND((double)offsetval(stack->img2, stack->offx2, stack->offy2, x, y) / 
 	            	(double)ASP_NONZERO(stack->img2->fixedPoint));
        p3 = (asp_pixel_t)ASP_ROUND((double)offsetval(stack->img3, stack->offx3, stack->offy3, x, y) / 
		            (double)ASP_NONZERO(stack->img3->fixedPoint));

        /* Calculate median, base value and check for overflow/underflow */
        median = get_median(p1, p2, p3);
        baseVal = *asp_image2d_get_pixel(stack->base,x,y);
        tmpSum = (asp_pixel_t)(baseVal + (asp_pixel_t)ASP_ROUND((double)median * (double)stack->base->fixedPoint));
        testPx = (int32_t)(baseVal + (asp_pixel_t)ASP_ROUND((double)median * (double)stack->base->fixedPoint));

        /* Underflow/Overflow protection */
        if(tmpSum == testPx){
           *asp_image2d_get_pixel(stack->base,x,y) = (asp_pixel_t)tmpSum;
        }

        if(testPx > ASP_PIXEL_MAX){
           *asp_image2d_get_pixel(stack->base,x,y) = (asp_pixel_t)ASP_PIXEL_MAX;
        }

        if(testPx < ASP_PIXEL_MIN) {
           *asp_image2d_get_pixel(stack->base,x,y) = (asp_pixel_t)ASP_PIXEL_MIN;
        }

      }
    }
    return ASP_SUCCESS;
  }





