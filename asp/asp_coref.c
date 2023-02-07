/**
 * \file    asp_coref.h
 * \brief   Reference stars for NEASCOUT
 * \author  William Huffman, Brian Bue, and David R Thompson, Jack Lightholder
 *
 * Copyright 2015 California Institute of Technology.  ALL RIGHTS RESERVED.
 * U.S. Government Sponsorship acknowledged.
 */

#include "asp_coref.h"
#include "asp_image2d.h"
#include "asp_error.h"

#include <stdlib.h>


  int asp_ref_stars(asp_costack_t* stack, asp_image2d_t* image, int* starsfound)
  {
   /**
    *
    * @param stack      The coadded stack for which the images are being coregistered, 
    *                      which controls the reference window size through <stack->radius>, 
    *                      the number of reference stars through <stack->numrefs>, and the 
    *                      locations of the reference stars through <stack->ref_coords_x> 
    *                      and <stack->ref_coords_y>
    * @param image      Image to be searched for reference stars
    * @param starsfound Return value counting the number of stars logged
    * 
    * @return ASP_SUCCESS flag indicating success or failure of function
    *
    * This function finds a number of reference stars (determined by 
    * <stack->numrefs>) in the initial image <image>.  It sets <*starsfound> 
    * to zero, and increments it whenever another reference star is added to 
    * <stack>'s internal list.  The function returns ASP_SUCCESS only if it 
    * finds exactly <stack->numrefs> reference stars, but the calling function
    * can look at <*starsfound> and determine how to proceed.  All entries in 
    * <stack->ref_coords_x> and <stack->ref_coords_y> correspond to the x and y
    * coordinates of the reference stars (if not enough reference stars are 
    * found, the remaining entries are zero).  Reference stars are chosen as the 
    * brightest pixels in the image, under the conditions that they are not 
    * within 2*<stack->radius> pixels from the edge of the image (in order to 
    * do template matching in the area surrounding the star) and they are not 
    * within <stack->radius> pixels of another reference star (to prevent 
    * clustering references about a single bright star)
    * 
    */
    int* xcoords;
    int* ycoords;
    int numrefs;
    int radius;
    int twice_radius;
    int found_one;
    int valid;
    int c_valid;
    int r_valid;
    asp_pixel_t current_value;
    char msg[ASP_MAX_LOGSTRING];

    /* If memory is not yet allocated, return */
    if(stack == NULL || 
       image == NULL || 
       starsfound == NULL)
    {
      asp_error(asp_error_coadd_null_input,
        "asp_ref_stars failed: NULL input");
      return ASP_FAILURE;
    }

    xcoords = stack->ref_coords_x;
    ycoords = stack->ref_coords_y;
    numrefs = stack->numrefs;
    radius  = stack->radius;
    (*starsfound) = 0;

    /* Set x/y coordinate vectors to zeros */
    for(int i = 0; i < numrefs; i++) {
      xcoords[i] = 0;
      ycoords[i] = 0;
    }

    // For each desired reference star, sweep image and find brightest pixel.
    twice_radius = 2 * radius;
    for(int refind = 0; refind < numrefs; refind++)
    {
      current_value = ASP_PIXEL_MIN;
      found_one = 0;
      for(int r = twice_radius; r < (image->rows - twice_radius); r++)
      {
        for(int c = twice_radius; c < (image->cols - twice_radius); c++)
        {
          if(*asp_image2d_get_pixel(image, c, r) >= current_value)
          {
            
            // Confirm this pixel is not part of an already found star
            valid = 1;
            for(int i = 0; i < refind; i++) {
   	            c_valid = ((c > xcoords[i]+radius) || (c < xcoords[i]-radius));
	              r_valid = ((r > ycoords[i]+radius) || (r < ycoords[i]-radius));
                valid = valid && c_valid && r_valid;
            }

            // Add to star coordinate frames xcoords and ycoords.
            if(valid) {
              xcoords[refind] = c;
              ycoords[refind] = r;
              current_value = *asp_image2d_get_pixel(image, c, r);
              found_one = 1;
            }
          }
        }
      }

      if(found_one != 0) {
        (*starsfound)++;
      }

    }

    (void)snprintf(msg, ASP_MAX_LOGSTRING, "Stars found: %i", *starsfound);
    asp_log(asp_log_ref_stars_found, msg);

    /* Returns a failure status if insufficient stars found.
         Unlikely since in the absence of additional stars, 
         noise will begin to be identified as stars.
         Only reachable with an abnormally high bounding box
         search size and very large number of stars.
         In that case, no image space would be available to 
         identify as an additional star and this would be triggered
         This would break compute availability and should halt */
    if(*starsfound != numrefs) {
      return ASP_FAILURE;
    }

    return ASP_SUCCESS;
  }



