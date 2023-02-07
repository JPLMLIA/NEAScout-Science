/** 
 * \file    asp_gsmooth.c
 * \brief   approximate Gaussian smoothing
 * \author  David R Thompson, based on gsmooth_border_ring.c by Mike Burl, 
 *          Steve Schaeffer, and others on the AEGIS team.
 *
 * Copyright 2015 California Institute of Technology.  ALL RIGHTS RESERVED.
 * U.S. Government Sponsorship acknowledged.
 */

#ifndef ASP_GSMOOTH_C
#define ASP_GSMOOTH_C

#include "asp_common.h"
#include "asp_error.h"
#include "asp_gsmooth.h"

#include <stdio.h>
#include <string.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

// Doesn't exist in VXWORKS build.  Must self-define.
#ifndef M_PI
    #define M_PI 3.14159265358979323846264338327950288
#endif


  int asp_smooth_image(asp_image2d_t *orig, asp_image2d_t **smoothed, asp_image2d_t *scratch,
                 const int width, const double sigma, const double scale, double *kernel)
  {  
   /**
    *
    * Apply a Gaussian filter to an image.  Requires four image buffers.  First 
    *   stores the incoming image, second stores the horizontally blurred image (scratch),
    *   the third stores the final output, and the fourth stores the Gaussian kernel. 
    *
    * @param orig      original image
    * @param smoothed  resulting image, with Gaussian filter applied 
    * @param scratch   scratch image for storing intermediary results
    * @param width     width of the Gaussian filter
    * @param sigma     sigma of the Gaussian filter
    * @param scale     scale of the Gaussian filter
    * @param kernel    memory to store the Gaussian filter
    *
    * @param ASP_SUCCESS flag indicating success or failure of the functional processing
    *
    */
    char msg[ASP_MAX_LOGSTRING];
    int rows = 1;
    int cols = 1;
    int i = 0;
    int r = 0;
    int c = 0;
    int ind = 0;
    int indB = 0;
    double fixPt = 1.0;
    double s = 0.0;
    double sum = 0.0;
    double pxVal = 0.0;

    /* If memory has not yet been allocated, return */
    if (orig == NULL) {
      asp_error( asp_error_smooth_smoothImageNullInput, "asp_smooth orig NULL" );
      return ASP_FAILURE;
    }
 
    rows = orig->rows;
    cols = orig->cols;
    fixPt = (double)orig->fixedPoint;

    if (width < 2 || (width*2+1 > cols) || 
                   (width*2+1 > rows))
    {
      (void)snprintf(msg, ASP_MAX_LOGSTRING, "asp_smooth_image width out of bounds: %d", width);
      asp_error( asp_error_smooth_smoothImageWidthOutOfBounds, msg);
      return ASP_FAILURE; 
    }

    /* Gaussian sigma must be larger than 0 and <= width */
    if (sigma < 0.0 || sigma >= width) {
      (void)snprintf(msg, ASP_MAX_LOGSTRING, "asp_smooth_image sigma out of bounds: %f", sigma);
      asp_error( asp_error_smooth_smoothImageSigmaOutOfBounds, msg);
      return ASP_FAILURE; 
    }

    /* If memory has not yet been allocated, return */
    if (smoothed == NULL) {
      asp_error( asp_error_smooth_smoothImageCouldNotAllocateImage, "asp_smooth smoothed NULL" );
      return ASP_FAILURE; 
    } 

    /* If memory has not yet been allocated, return */
    if ((*smoothed) == NULL) {
      asp_error( asp_error_smooth_smoothImageCouldNotAllocateImage, "asp_smooth *smoothed NULL" );
      return ASP_FAILURE; 
    } 

    /* If memory has not yet been allocated, return */
    if (scratch == NULL) {
      asp_error( asp_error_smooth_smoothImageCouldNotAllocateImage, "asp_smooth scratch NULL" );
      return ASP_FAILURE; 
    } 

    // Ensure orig and scratch have the same dimensions for downstream indexing
    if (asp_image2d_cmp_dims(orig, scratch)) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "mismatched dims (orig r:%d c:%d) (scratch r:%d c:%d)", orig->rows, orig->cols, scratch->rows, scratch->cols);
        asp_error(asp_error_smooth_inputImageDimsMismatch, msg);
        return ASP_FAILURE;
    }

    // Ensure scratch and smoothed image have same dimensions for downstream indexing
    if (asp_image2d_cmp_dims(scratch, (*smoothed))) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "mismatched dims (scratch r:%d c:%d) (smoothed r:%d c:%d)", scratch->rows, scratch->cols, (*smoothed)->rows, (*smoothed)->cols);
        asp_error(asp_error_smooth_inputImageDimsMismatch, msg);
        return ASP_FAILURE;
    }

    /* set standard deviation to 1.0 */
    s = scale * sigma;

    /* If memory has not yet been allocated, return */
    if (kernel == NULL) {
      asp_error( asp_error_smooth_smoothImageCouldNotAllocateKernel, "asp_smooth kernel NULL" );
      return ASP_FAILURE; 
    }
 
    for (i = -width; i <= width; i++) {
       kernel[i + width] = exp(-((double)(i*i)/(double)ASP_NONZERO(s)))/(double)ASP_NONZERO((M_PI * s));
       sum += kernel[i + width];
    }

    // Avoid division by zero
    if (sum==0) sum=1e-6;
 
    // Normalize kernel, create checksum to verify normalization
    for(int i = 0; i < width*2+1; i++) {
      kernel[i] /= (double)ASP_NONZERO(sum);
    }

    // Initialize smoothed and scratch images to zero
    for (c=0; c < cols; c++)
    {
      for (r=0; r < rows; r++)
      {
        ind = asp_image2d_get_index(orig, c, r );
        (*smoothed)->data[ind] = 0;
        scratch->data[ind] = 0;
      }
    }

    // Perform smoothing horizontally across the image
    for (r=0; r < rows; r++)
    {

      // Near the left edge, keep original data (bounds protection)
      for (c=0; c < width; c++)
      {
        ind = asp_image2d_get_index(orig, c, r );
        scratch->data[ind] = orig->data[ind];
      }

      // Apply Gaussian filter horizontally
      for (c=width; c < cols-width; c++)
      {
        ind = asp_image2d_get_index(orig, c, r );
        pxVal = 0.0;
        for(int i = -width; i <= width; ++i)
        {
          indB = asp_image2d_get_index(orig, c+i, r );
          pxVal += ((double)orig->data[indB] / (double)ASP_NONZERO(fixPt)) * kernel[i+width];
        }
        scratch->data[ind] = (asp_pixel_t)ASP_ROUND(pxVal * fixPt);
      }

      // Near the right edge, keep original data (bounds protection)
      for (c=cols-width; c < cols; c++)
      {
        ind = asp_image2d_get_index(orig, c, r );
        scratch->data[ind] = orig->data[ind];
      }
      
    }

    // Perform vertical smoothing across the image
    for (c=0; c < cols; c++)
    {

      // Near the top, keep original data (bounds protection)
      for (r=0; r < width; r++)
      {
        ind = asp_image2d_get_index(orig, c, r );
        (*smoothed)->data[ind] = scratch->data[ind];
      }

      // Apply Gaussian filter vertically
      for (r=width; r < rows-width; r++)
      {
        ind = asp_image2d_get_index(orig, c, r );
        pxVal = 0.0;
        for(int i = -width; i <= width; ++i)
        {
          indB = asp_image2d_get_index(orig, c, r+i);
          pxVal += ((double)scratch->data[indB] / (double)ASP_NONZERO(fixPt)) * kernel[i+width];
        }
        (*smoothed)->data[ind] = (asp_pixel_t)ASP_ROUND(pxVal * fixPt);
      }

      // Near the bottom, keep the original data (bounds protection)
      for (r=rows-width; r < rows; r++)
      {
        ind = asp_image2d_get_index(orig, c, r );
        (*smoothed)->data[ind] = scratch->data[ind];
      }
    }

    return ASP_SUCCESS;
  }
 


#endif
