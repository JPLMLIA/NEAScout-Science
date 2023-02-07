/** 
 * \file    asp_crop.c
 * \brief   subwindowing and subsampling
 * \author  Brian Bue, and David R Thompson
 *
 * Copyright 2014 California Institute of Technology.  ALL RIGHTS RESERVED.
 * U.S. Government Sponsorship acknowledged.
 */


#include "asp_common.h"
#include "asp_error.h"
#include "asp_image2d.h"
#include "asp_crop.h"

#include <stdlib.h>
#include <math.h>
#include <stdio.h>


int
asp_subsample_image(asp_image2d_t *orig, 
                    asp_image2d_t **subs,
                    asp_image2d_t *counts,
                    const int factor)
{
/* 
 *
 * Take the image "orig" and subsample it, returning a new image "subs"
 * which holds the result.  Subsampling involves a simple average of pixels 
 * in the original image.  Divide all DNs by ((factor*factor)+2) to fight 
 * overflow.  Re-scale by that value again after averaging.  Some precision loss
 * may occur, but protects us from needing larger data buffers.
 *
 * @param orig - original image for downsampling
 * @param subs - resulting downsampled image
 * @param counts - image array for storing downsample pixel neighbor count
 * @param factor - ratio to downsample by.  4 gives an image 1/4 original size.
 *
 * @return ASP_SUCCESS flag indicating success or failure of functional processing
 */
 int r = 0;
 int c = 0;
 int ind = 0;
 int nr = 0;
 int nc = 0;
 int old = 0;
 int new = 0;
 int scaler = 1;
 int nzcounts = 1;
 int32_t testPx = 0;
 asp_pixel_t subTmp = 0;

  if (orig == NULL)
  {
    asp_error(asp_error_crop_subsampleImageNullInput, "asp_subsample orig null input");
    return ASP_FAILURE;
  }

  if (counts == NULL)
  {
    asp_error(asp_error_crop_subsampleImageNullInput, "asp_subsample counts null input");
    return ASP_FAILURE;
  }

  if (subs == NULL)
  {
    asp_error(asp_error_crop_subsampleImageNullInput, "asp_subsample subs null input");
    return ASP_FAILURE;
  }

  if ((*subs) == NULL)
  {
    asp_error(asp_error_crop_subsampleImageNullInput, "asp_subsample *subs null input");
    return ASP_FAILURE;
  }

  /* Calculate new image dimensions */
  if (factor <= 0 || factor > orig->rows || factor > orig->cols)
  {
    asp_error(asp_error_crop_subsampleImageFactorOutOfBounds, "subsample factor out of bounds");
    return ASP_FAILURE; 
  }

  /* copy incidental telemetry parameters */
  asp_image2d_cptelem(orig, *subs);

  for (uint32_t i=0; i < counts->npix; i++)
  {
    counts->data[i] = 0;
    (*subs)->data[i] = 0;
  }

  // Amount to divide original DNs to fight overflow
  scaler = (factor*factor)+2;

  // Divide DNs by factor to protect against overflow
  for (r=0; r < orig->rows; r++)
  {
    for (c=0; c < orig->cols; c++)
    {
      ind = asp_image2d_get_index(orig, c, r);
      orig->data[ind] = (asp_pixel_t)ASP_ROUND((double)orig->data[ind] / (double)ASP_NONZERO(scaler));
    }
  }

  // Accumulate signal in each big pixel
  for (r=0; r < orig->rows; r++)
  {
    for (c=0; c < orig->cols; c++)
    {
      nr = (int)((double)r / (double)ASP_NONZERO(factor));
      nc = (int)((double)c / (double)ASP_NONZERO(factor));
      old = asp_image2d_get_index(orig, c, r);
      new = asp_image2d_get_index((*subs), nc, nr);

      subTmp = (asp_pixel_t)((*subs)->data[new] + orig->data[old]);
      testPx = (int32_t)((*subs)->data[new] + orig->data[old]);

      // Underflow / overflow protection
      if(subTmp == testPx) {
          (*subs)->data[new] = (asp_pixel_t)subTmp;
      }

      if(testPx > ASP_PIXEL_MAX) {
         (*subs)->data[new] = (asp_pixel_t)ASP_PIXEL_MAX;
      }

      if(testPx < ASP_PIXEL_MIN) {
          (*subs)->data[new] = (asp_pixel_t)ASP_PIXEL_MIN;
      }

      counts->data[new]++;

    }
  }

  // Divide by counts to get an average
  for (r=0; r < counts->rows; r++)
  {
    for (c=0; c < counts->cols; c++)
    {
      ind = asp_image2d_get_index((*subs), c, r);
      nzcounts = ASP_NONZERO(counts->data[ind]);

      subTmp = (asp_pixel_t)ASP_ROUND(((double)(*subs)->data[ind] / (double)ASP_NONZERO(nzcounts)) * (double)scaler);
      testPx = (int32_t)    ASP_ROUND(((double)(*subs)->data[ind] / (double)ASP_NONZERO(nzcounts)) * (double)scaler);

      // Underflow / overflow protection
      if(subTmp == testPx) {
          (*subs)->data[ind] = (asp_pixel_t)subTmp;
      }

      if(testPx >= ASP_PIXEL_MAX) {
         (*subs)->data[ind] = (asp_pixel_t)ASP_PIXEL_MAX;
         (*subs)->overflow += 1;
      }

      if(testPx <= ASP_PIXEL_MIN) {
         (*subs)->data[ind] = (asp_pixel_t)ASP_PIXEL_MIN;
         (*subs)->underflow += 1;
      }

    }
  }

  return ASP_SUCCESS; 
}



int asp_crop_image(asp_image2d_t *orig, 
                   asp_image2d_t **crop,
                   const int top, const int bottom,
                   const int left, const int right)
{
 /*
  *  
  * Crop the image "orig," allocate a new image "crop" to hold the result.
  * all indices are zero-based and inclusive.
  *
  * @param orig    Incoming image to be cropped 
  * @param crop    Resulting cropped image
  * @param top     Index of top-most pixel
  * @param bottom  Index of bottom-most pixel
  * @param left    Index of left-most pixel
  * @param right   Index of right-most pixel
  *
  * @return ASP_SUCCESS flag indicating success or failure of functional processing
  *
  */
  int height = bottom-top+1;
  int width = right-left+1;
  int old = 0;
  int new = 0;
  char msg[ASP_MAX_LOGSTRING];

  /* If memory has not yet been allocated, return */
  if (orig == NULL) {
      asp_error(asp_error_crop_cropImageNullInput, "asp_crop_image null input");
      return ASP_FAILURE;
  }

  /* If provided height related parameters
       are out of bounds, return */
  if (height <= 0               || 
      height > ASP_MAX_ROWS     || 
      height+top > orig->rows   || 
      top < 0                   || 
      top > orig->rows          || 
      bottom < 0                || 
      bottom > orig->rows)
  {
      (void)snprintf(msg, ASP_MAX_LOGSTRING, 
        "asp_crop_image height value error: T:%d B:%d H:%d", top, bottom, height);
      asp_error( asp_error_crop_cropImageDestinationHeightOutOfBounds, msg);
      return ASP_FAILURE; 
  }

  /* If provided width related parameters
       are out of bounds, return */
  if (width <= 0                || 
      width > ASP_MAX_COLS      || 
      width+left > orig->cols   || 
      left < 0                  || 
      left > orig->cols         || 
      right < 0                 || 
      right > orig->cols)
  {
      (void)snprintf(msg, ASP_MAX_LOGSTRING, "asp_crop_image width value error: L:%d R:%d W:%d", 
                 left, right, width);
      asp_error( asp_error_crop_cropImageDestinationWidthOutOfBounds, msg);
      return ASP_FAILURE; 
  }

  /* If memory has not yet been allocated, return */
  if (crop == NULL) {
      asp_error(asp_error_crop_cropImageCouldNotAllocateImage, "asp_crop crop NULL");
      return ASP_FAILURE; 
  } 

  /* If memory has not yet been allocated, return */
  if ((*crop) == NULL) {
      asp_error(asp_error_crop_cropImageCouldNotAllocateImage, "asp_crop *crop NULL");
      return ASP_FAILURE; 
  } 

  /* copy telemetry parameters */
  asp_image2d_cptelem(orig, *crop);

  /* Reset overflow/underflow counter */
  (*crop)->underflow = 0;
  (*crop)->overflow = 0;

  for (int r=0; r < height; r++) {
      for (int c=0; c < width; c++) {

          old = asp_image2d_get_index( orig, left+c, top+r );
          new = asp_image2d_get_index( (*crop), c, r );
          (*crop)->data[new] = orig->data[old];

          /* Recalculate underflow/overflow pixels for subregion */
          if((*crop)->data[new] == ASP_PIXEL_MAX) {
              (*crop)->overflow += 1;
          }

          if((*crop)->data[new] == ASP_PIXEL_MIN) {
              (*crop)->underflow += 1;
          }

      }
  }

  /* record bounding box in ORIGINAL frame */
  (*crop)->otop  = orig->otop  + top;
  (*crop)->oleft = orig->oleft + left;

  return ASP_SUCCESS; 
}


int asp_search_image(asp_image2d_t *img, 
             const int top, const int bottom,
             const int left, const int right,
             float *row, float *col)
{
 /* 
  *
  * Find the brightest pixel in this subwindow. 
  *
  * @param img image to search
  * @param top     Index of top-most pixel to consider
  * @param bottom  Index of bottom-most pixel to consider
  * @param left    Index of left-most pixel to consider
  * @param right   Index of right-most pixel to consider
  * @param row     Return pointer for row of brightest pixel
  * @param col     Return pointer for col of brightest pixel
  *
  * @return ASP_SUCCESS flag indicating success or failure of functional processing
  *
  */
  char msg[ASP_MAX_LOGSTRING];
  int irow = 0;
  int icol = 0;
  int index = 0;
  float x = 0.0;
  float total = 0.0;
  asp_pixel_t max = ASP_PIXEL_MIN;

  /* If memory has not yet been allocated, return */
  if (row == NULL) {
      asp_error( asp_error_crop_searchImageNullInput, "asp_search_image null row input" );
      return ASP_FAILURE;
  } 

  /* If memory has not yet been allocated, return */
  if (col == NULL) {
      asp_error( asp_error_crop_searchImageNullInput, "asp_search_image null col input" );
      return ASP_FAILURE;
  } 

  /* If memory has not yet been allocated, return */
  if (img == NULL) {
      asp_error( asp_error_crop_searchImageNullInput, "asp_search_image null input" );
      return ASP_FAILURE;
  } 

  /* Ensure top < bottom */
  if (bottom <= top) {
      (void)snprintf(msg, ASP_MAX_LOGSTRING, "asp_search_image bottom(%d) <= top(%d)", bottom, top);
      asp_error( asp_error_crop_searchImage_boundsError, msg);
      return ASP_FAILURE; 
  }

  /* Ensure left < right */
  if (right <= left) {
      (void)snprintf(msg, ASP_MAX_LOGSTRING, "asp_search_image right(%d) <= left(%d)", right, left);
      asp_error( asp_error_crop_searchImage_boundsError, msg);
      return ASP_FAILURE; 
  }

  /* Bounds check on top pixel index */
  if (top < 0 || top >= img->rows) {
      (void)snprintf(msg, ASP_MAX_LOGSTRING, "asp_search_image top value out of bounds: %d", top);
      asp_error( asp_error_crop_searchImageTopValueOutOfBounds, msg);
      return ASP_FAILURE; 
  }

  /* Bounds check on bottom pixel index */
  if (bottom < 0 || bottom >= img->rows) {
      (void)snprintf(msg, ASP_MAX_LOGSTRING, "asp_search_image bottom value out of bounds: %d", bottom);
      asp_error( asp_error_crop_searchImageBottomValueOutOfBounds, msg);
      return ASP_FAILURE; 
  }

  /* Bounds check on left pixel index */
  if (left < 0 || left >= img->cols) {
      (void)snprintf(msg, ASP_MAX_LOGSTRING, "asp_search_image left value out of bounds: %d", left);
      asp_error( asp_error_crop_searchImageLeftValueOutOfBounds, msg);
      return ASP_FAILURE; 
  }

  /* Bounds check on right pixel index */
  if (right < 0 || right >= img->cols) {
      (void)snprintf(msg, ASP_MAX_LOGSTRING, "asp_search_image right value out of bounds: %d", right);
      asp_error( asp_error_crop_searchImageRightValueOutOfBounds, msg);
      return ASP_FAILURE; 
  }

  (void)snprintf(msg, ASP_MAX_LOGSTRING,  "top left : %d %d", top, left);
  asp_log(asp_log_search_top_left, msg);

  /* Find brightest pixel */
  for (int r=top; r <= bottom; r++) {
      for (int c=left; c <= right; c++) {
          index = asp_image2d_get_index(img, c, r);
          asp_pixel_t x = img->data[index];
          if(x > max) {
              irow = r;  
              icol = c;  
              max = x;
          }
      }
  }

  /* Do not index image outside bounding box */
  irow = (irow<=top)? top+1 : irow;
  irow = (irow>=bottom)? bottom-1 : irow;
  icol = (icol<=left)? left+1 : icol;
  icol = (irow>=right)? right-1 : icol;

  /* Find grey level center of mass, 3x3 window */
  *row = 0.0;
  *col = 0.0;
  for (int r=irow-1; r <= irow+1; r++) {
      for (int c=icol-1; c <= icol+1; c++) {
          index = asp_image2d_get_index( img, c, r );
          x = img->data[index];

          total  += x;
          (*row) += x * (float)r;
          (*col) += x * (float)c;
      }
  }

  (*row) = (*row)/ASP_NONZERO(total);
  (*col) = (*col)/ASP_NONZERO(total);

  return ASP_SUCCESS; 
}


int asp_search_crop_image(asp_image2d_t *orig, int *box_dims, 
                          const int crop_width, const int crop_height,
                          const int top, const int bottom,
                          const int left, const int right)
{
 /*
  * Search the image for the brightest pixel,
  *   crop around that location, based on crop_width
  *   and crop_height.
  *
  * @param orig         Incoming image to be searched/cropped 
  * @param box_dims 
  * @param crop_width   Width to crop around brightest pixel
  * @param crop_height  Height to crop around brightest pixel
  * @param top          Index of the top-most pixel
  * @param bottom       Index of the bottom-most pixel
  * @param left         Index of left-most pixel
  * @param right        Index of right-most pixel
  * 
  * @return ASP_SUCCESS flag indicating success or failure of functional processing
  *
  */
  float frow=0;
  float fcol=0;
  int row=0;
  int col=0;
  int status=0;
  int ctop=0;
  int cleft=0;
  int cbottom=0;
  int cright=0;
  char msg[ASP_MAX_LOGSTRING];

  if (orig == NULL) {
      asp_error(asp_error_crop_searchImageNullInput, "asp_search_crop_image null orig input");
      return ASP_FAILURE;
  } 

  if (box_dims == NULL) {
      asp_error(asp_error_crop_searchImageNullInput, "asp_search_crop_image null box_dims input");
      return ASP_FAILURE;
  } 

  status = asp_search_image(orig,top,bottom,left,right,&frow,&fcol);
  if (status == ASP_FAILURE) {
      return ASP_FAILURE; 
  }

  col = ASP_ROUND(fcol);
  row = ASP_ROUND(frow);

  (void)snprintf(msg, ASP_MAX_LOGSTRING, "Bright pixel in the image: row:%d col:%d",row,col);
  asp_log(asp_log_search_crop_brightest_pixel, msg);

  /* set crop parameters based on bright pixel */
  ctop    = row-(crop_height/2);
  cleft   = col-(crop_width/2);
  cbottom = row+(crop_height/2); 
  cright  = col+(crop_width/2);

  /* enforce image edges */
  ctop    = (ctop >= 0)?            ctop    : 0;
  cbottom = (cbottom < orig->rows)? cbottom : orig->rows-1;
  cleft   = (cleft >= 0)?           cleft   : 0;
  cright  = (cright  < orig->cols)? cright  : orig->cols-1;

  box_dims[0] = ctop;
  box_dims[1] = cbottom;
  box_dims[2] = cleft;
  box_dims[3] = cright;

  return ASP_SUCCESS;
}



int asp_subtract_image(asp_image2d_t *origA, 
                       asp_image2d_t *origB, 
                       asp_image2d_t **result,
	                     int newFixPt)
{
 /* 
  *
  * Subtract image B from image A.
  * 
  * @param origA    Image A, base image for subtraction
  * @param origB    Image B, image to be subtracted from A
  * @param result   Resulting image, after subtraction
  * @param newFixPt Fixed point scalar to apply to resulting image
  * 
  * @return ASP_SUCCESS flag indicating success or failure of functional processing
  *
  */
  asp_pixel_t subTmp;
  int32_t testPx;
  double subDouble;
  int indA;
  int indB;
  int indR;
  int new_height;
  int new_width;

  if (origA == NULL || origB == NULL) {
      asp_error( asp_error_crop_subtractImageNullInput, 
        "asp_subtract_image null input" );
      return ASP_FAILURE;
  }

  /* Calculate new image dimensions */
  new_height = (origA->rows < origB->rows)? origA->rows : origB->rows;
  new_width = (origA->cols < origB->cols)? origA->cols : origB->cols;

  if (new_height <= 0 || new_width <= 0) {
      asp_error( asp_error_crop_subtractImageBadDimensions, 
        "asp_subtract_image bad dimensions" );
      return ASP_FAILURE; 
  }

  /* If memory not yet allocated, return */
  if (result == NULL) {
      asp_error( asp_error_crop_subtractImageCouldNotAllocateImage, 
        "asp_subtract_image result null input" );
      return ASP_FAILURE; 
  } 

  /* If memory not yet allocated, return */
  if ((*result) == NULL) {
      asp_error( asp_error_crop_subtractImageCouldNotAllocateImage, 
        "asp_subtract_image *result null input" );
      return ASP_FAILURE; 
  } 

  /* Set underflow/overflow counters to zero */
  (*result)->overflow = 0;
  (*result)->underflow = 0;

  /* Subtract */
  for (int r=0; r < new_height; r++) {
      for (int c=0; c < new_width; c++) {

          indA = asp_image2d_get_index( origA, c, r );
          indB = asp_image2d_get_index( origB, c, r );
          indR = asp_image2d_get_index( (*result), c, r );

          subDouble = ((double)origA->data[indA] / (double)ASP_NONZERO(origA->fixedPoint)) 
                        - ((double)origB->data[indB] / (double)ASP_NONZERO(origB->fixedPoint));

          subDouble = subDouble * (double)newFixPt;

          testPx = (int32_t)ASP_ROUND(subDouble);
          subTmp = (asp_pixel_t)ASP_ROUND(subDouble);

          /* Underflow/overflow protection */
          if(subTmp == testPx) {
              (*result)->data[indR] = (asp_pixel_t)subTmp;
          }

          if(testPx > ASP_PIXEL_MAX){
              (*result)->data[indR] = (asp_pixel_t)ASP_PIXEL_MAX;
              (*result)->overflow += 1;
          }

          if(testPx < ASP_PIXEL_MIN){
              (*result)->data[indR] = (asp_pixel_t)ASP_PIXEL_MIN;
              (*result)->underflow += 1;
          }

      }
  }

  return ASP_SUCCESS; 
}


int asp_subtract_image_offset(asp_image2d_t *origA, 
                              asp_image2d_t *origB, 
                              asp_image2d_t **result,
                              int* shift,
		                          int newFixPt)
{
 /* 
  *
  * Subtract image B from image A, with an (x,y) offset from shift. 
  *
  * @param origA    Image A, base image for subtraction
  * @param origB    Image B, image to be subtracted from A
  * @param result   Resulting image, after subtraction
  * @param shuft    X,Y vector indicating shift to apply in subtraction
  * @param newFixPt Fixed point scalar to apply to both images
  *
  * @return ASP_SUCCESS flag indicating success or failure of functional processing
  *
  */
  asp_pixel_t subTmp;
  int32_t testPx;
  double subDouble;
  int shiftedRow;
  int shiftedCol;
  int indA;
  int indB;
  int indR;
  int new_height;
  int new_width;

  if (shift == NULL) {
      asp_error( asp_error_crop_subtractImageNullInput, 
        "asp_subtract_image_offset null shift input" );
      return ASP_FAILURE;
  }

  if (origA == NULL || origB == NULL) {
      asp_error( asp_error_crop_subtractImageNullInput, 
        "asp_subtract_image_offset null input" );
      return ASP_FAILURE;
  }

  /* Calculate new image dimensions */
  new_height = (origA->rows < origB->rows)? origA->rows : origB->rows;
  new_width = (origA->cols < origB->cols)? origA->cols : origB->cols;

  if (new_height <= 0 || new_width <= 0) {
      asp_error( asp_error_crop_subtractImageBadDimensions, 
        "asp_subtract_image bad dimensions" );
      return ASP_FAILURE; 
  }

  if (result == NULL) {
      asp_error( asp_error_crop_subtractImageCouldNotAllocateImage, 
        "asp_subtract_image_offset result null input" );
      return ASP_FAILURE; 
  } 

  if ((*result) == NULL) {
      asp_error( asp_error_crop_subtractImageCouldNotAllocateImage, 
        "asp_subtract_image_offset *result null input" );
      return ASP_FAILURE; 
  } 

  (*result)->overflow = 0;
  (*result)->underflow = 0;

  /* Subtract */
  for (int r=0; r < new_height; r++) {
      for (int c=0; c < new_width; c++) {

          indA = asp_image2d_get_index( origA, c, r );
          indR = asp_image2d_get_index( (*result), c, r );

          shiftedCol = c + shift[0];
          shiftedRow = r + shift[1];

          if(shiftedCol < 0 || shiftedCol >= origB->cols || shiftedRow < 0 || shiftedRow >= origB->rows){
              (*result)->data[indR] = 0;

          } else {

              indB = asp_image2d_get_index( origB, shiftedCol, shiftedRow);

              subDouble = ((double)origA->data[indA] / (double)ASP_NONZERO(origA->fixedPoint)) 
                             - ((double)origB->data[indB] / (double)ASP_NONZERO(origB->fixedPoint));

              subDouble = subDouble * (double)newFixPt;

              testPx = (int32_t)ASP_ROUND(subDouble);
              subTmp = (asp_pixel_t)ASP_ROUND(subDouble);
   
              /* Underflow/overflow protection */
              if(subTmp == testPx) {
     	            (*result)->data[indR] = (asp_pixel_t)subTmp;
              }

   	          if(testPx > ASP_PIXEL_MAX) {
                  (*result)->data[indR] = (asp_pixel_t)ASP_PIXEL_MAX;
                  (*result)->overflow += 1;
              }

              if(testPx < ASP_PIXEL_MIN) {
                  (*result)->data[indR] = (asp_pixel_t)ASP_PIXEL_MIN;
                  (*result)->underflow += 1;
              }

          }
      }
  }

  return ASP_SUCCESS; 
}


