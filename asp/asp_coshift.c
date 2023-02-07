/**
 * \file    asp_coshift.c
 * \brief  Find the optimal shift 
 * \author  William Huffman, Brian Bue, and David R Thompson, Jack Lightholder
 *
 * Copyright 2015 California Institute of Technology.  ALL RIGHTS RESERVED.
 * U.S. Government Sponsorship acknowledged.
 */

#include "asp_coshift.h"
#include "asp_image2d.h"
#include "asp_bbox.h"
#include "asp_error.h"

#include <stdlib.h>
#include <math.h>

  double asp_xcorr_image2d(asp_image2d_t* img1, 
                           asp_image2d_t* img2,
                           asp_bbox_t* bbox1, 
                           asp_bbox_t* bbox2,
                           const double scaling)
  {
  /**
   *
   * This function computes normalized cross-correlation between two regions 
   *   of two asp_image2ds. As this is currently only meant to be used in 
   *   asp_find_shift_image2d, parameter checking is done in that function and 
   *   omitted here.
   *
   * @param img1    Image 1 data to be correlated
   * @param img2    Image 2 data to be correlated
   * @param bbox1   Bounding box, defines region of image 1 to be correlated
   * @param bbox2   Bounding box, defines region of image 2 to be correlated
   * @param scaling Scaling value, to be applied to image 1, to adjust for know
   *                   DN scale differences.
   *
   * @return cross normalized cross correlation value
   */
    if(scaling < ASP_EPS) {
      return 0;
    }

    double m1 = 0.0;
    double m2 = 0.0;
    double sd1 = 0.0;
    double sd2 = 0.0;
    double accum = 0.0;
    double v1 = 0.0;
    double v2 = 0.0;
    double bbox_size = 1.0;
    int row1 = 0;
    int row2 = 0;
    int col1 = 0;
    int col2 = 0;
    int r = 0;
    int c = 0;

    if(img1 == NULL  || 
       img2 == NULL  ||
       bbox1 == NULL ||
       bbox2 == NULL)
    {
        return 0;
    }


    bbox_size = (double)bbox1->height * (double)bbox1->width;

    // Calculate mean and standard deviation for both images
    m1 = asp_bbox_mean(img1, bbox1) / (double)ASP_NONZERO(scaling);
    m2 = asp_bbox_mean(img2, bbox2);
    sd1 = asp_bbox_std(img1, m1, bbox1) / (double)ASP_NONZERO(scaling);
    sd2 = asp_bbox_std(img2, m2, bbox2);

    // Normalized cross correlation metric 
    for(r = 0; r < bbox1->height; r++)
    {
        row1 = bbox1->top + r;
        row2 = bbox2->top + r;

        if (row1>=img1->rows || row1<0 || row2>=img2->rows || row2<0) {
          continue;
        }

        for(c = 0; c < bbox1->width; c++)
        {
            col1 = bbox1->left + c;
            col2 = bbox2->left + c;

            // Remain within image bounds
            if (col1>=img1->cols || col1<0 || col2>=img2->cols || col2<0) {
              break;
            }

            v1 = ((double)(*asp_image2d_get_pixel(img1,col1,row1)) / (double)ASP_NONZERO(img1->fixedPoint)) / (double)ASP_NONZERO(scaling);

            v2 = (double)(*asp_image2d_get_pixel(img2,col2,row2)) / (double)ASP_NONZERO(img2->fixedPoint);

            accum += (v1 - m1) * (v2 - m2);
        }
    }

    return accum / (double)ASP_NONZERO(bbox_size * sd1 * sd2);
  }


  int asp_find_shift_image2d(int* shift, 
                             asp_image2d_t* img1, 
                             asp_image2d_t* img2,
                             asp_costack_t* stack,
                             asp_bbox_t* bbox1,
                             asp_bbox_t* bbox2,
                             const double scaling)
  {
  /**
   *
   * This function computes the row-column shift that matches img2 to img1 
   * (such that that a location on img2 + shift = the corresponding location 
   * on img1) based on the normalized cross-correlation between windows around 
   * the reference stars
   *
   *
   * @param img1     Image 1 data to be correlated
   * @param img2     Image 2 data to be correlated
   * @param stack    The coadded stack for which the images are being coregistered, 
   *                   which controls the reference window size through <stack->radius>, 
   *                   the number of reference stars through <stack->numrefs>, and the 
   *                   locations of the reference stars through <stack->ref_coords_x> 
   *                   and <stack->ref_coords_y>
   * @param bbox1    Bounding box, defines region of image 1 to be correlated
   * @param bbox2    Bounding box, defines region of image 2 to be correlated
   * @param scaling  Scaling value, to be applied to image 1, to adjust for know
   *                   DN scale differences.
   *
   * @return ASP_SUCCESS flag indicating success or failure of the functional processing
   */    
    double maxcorr;
    double thiscorr;
    int ro;
    int co;
    int iter;
    char msg[ASP_MAX_LOGSTRING];

    if(shift == NULL || 
       img1 == NULL  || 
       img2 == NULL  ||
       stack == NULL ||
       bbox1 == NULL ||
       bbox2 == NULL)
    {
        asp_error(asp_error_coadd_null_input, "asp_find_shift_image2d failed: NULL inputs");
        return ASP_FAILURE;
    }

    if (scaling < ASP_EPS) {
	      (void)snprintf(msg, ASP_MAX_LOGSTRING, "asp_find_shift failed: scaling (%f) < ASP_EPS (%f)", scaling, ASP_EPS);
        asp_error(asp_error_coadd_low_scaling, msg);
        return ASP_FAILURE;
    }

    shift[0] = 0;
    shift[1] = 0;

    maxcorr = -1;

    // Sweep bounding box exploration region.  Calculate cross correlation for each, around each star.
    for(ro = -(stack->radius); ro <= stack->radius; ro++)
    {
        for(co = -(stack->radius); co <= stack->radius; co++)
        {
            thiscorr = 0;

            for(iter = 0; iter < stack->numrefs && 
                          stack->ref_coords_x[iter] > 0 &&
                          stack->ref_coords_y[iter] > 0; iter++)
            {
                // Get bounding region for base image
                bbox1->top = stack->ref_coords_y[iter] - stack->radius;
                bbox1->left  = stack->ref_coords_x[iter] - stack->radius;

                // Get bounding region for new image (sliding window)
                bbox2->top = bbox1->top + ro;
                bbox2->left  = bbox1->left + co;

                // Add in correlation from this reference star
                thiscorr += asp_xcorr_image2d(img1,img2,bbox1,bbox2,scaling);
            }
 
            // If correlation higher than previous, set new x/y shift for given sweep coords.
            if(thiscorr > maxcorr)
            {
                shift[0] = co;
                shift[1] = ro;
                maxcorr = thiscorr;
            }
        }
    }

    
    // If the last bounding box search is considered best, we've likely hit 
    //  a non-descriptive region and the shift cannot be trusted.
    if((shift[0] == -(stack->radius)) ||
	     (shift[0] == stack->radius)    ||
	     (shift[1] == -(stack->radius)) ||
	     (shift[1] == stack->radius)){
          asp_log(asp_log_coadd_box_exceeded, "shift exceeded bounding box");
		      shift[0] = 0;
		      shift[1] = 0;
    }
    
    return ASP_SUCCESS;
}
