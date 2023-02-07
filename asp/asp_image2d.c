/** 
 * \file    asp_image2d.c
 * \brief   spectral image data structure
 * \author  Brian Bue, based on asp_image.c by Ben Bornstein
 *
 * Copyright 2011 California Institute of Technology.  ALL RIGHTS RESERVED.
 * U.S. Government Sponsorship acknowledged.
 */

#ifndef ASP_IMAGE2D_C
#define ASP_IMAGE2D_C

#include "asp_common.h"
#include "asp_error.h"
#include "asp_image2d.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string.h>
#include <math.h>
#include <ctype.h>


  int asp_image2d_cmp_dims(const void *a, const void *b) 
  { 
   /**
    * Image dimension comparison function. 
    *
    * @param a image A pointer
    * @param b image B pointer
    *
    * @return 0 if image A dimensions equal image B dimensions, 1 if one dimension differs or -1 if both dimensions differ (or don't exist)
    *
    */
    int lines = 0;
    int cols = 0;

    /* check for null pointer */
    if( a == NULL ) {
      asp_error(asp_error_image2d_comparisonNullImage, "asp_image2d_cmp_dims null a");
      return -1;
    }

    /* check for null pointer */
    if( b == NULL ) {
      asp_error(asp_error_image2d_comparisonNullImage, "asp_image2d_cmp_dims null b");
      return -1;
    }

    const asp_image2d_t *img1 = (const asp_image2d_t *)a; 
    const asp_image2d_t *img2 = (const asp_image2d_t *)b;

    lines = img1->rows - img2->rows;
    cols  = img1->cols - img2->cols;

    if (lines == 0 && cols == 0) {
        return 0;
    }
    else if (lines == 0 || cols == 0) {
        return 1;
    }

    return -1;
  } 


  void asp_image2d_cptelem(const asp_image2d_t *src, asp_image2d_t *dst)
  {
   /**
    *
    * @param src source image
    * @param dst destination image
    *
    *
    *  Copy telemetry (non-dimensioned) parameters from src to dst\n\n
    *  Header fields copied:\n
    *  Camera header\n
    *  data format\n
    *  timestamp (whole seconds)\n
    *  timestamp (sub-seconds)\n
    *  temperature\n
    *  exposure\n
    *  gain\n
    *  fixed point scalar\n
    *  product ID\n
    *  pixel overflow count\n
    *  pixel underflow count\n\n
    */
    int i;
    for(i=0; i<CAMERA_HEADER_BLOCKS; i++){
        dst->cameraHeader[i] = src->cameraHeader[i];
    }

    dst->format                    = src->format;
    dst->timestamp_whole_second    = src->timestamp_whole_second;
    dst->timestamp_sub_second      = src->timestamp_sub_second;
    dst->temperature               = src->temperature;
    dst->exposure                  = src->exposure;
    dst->gain                      = src->gain;
    dst->fixedPoint                = src->fixedPoint;
    dst->productid                 = src->productid; 
    dst->overflow                  = src->overflow;
    dst->underflow	               = src->underflow;
}



  int asp_image2d_compare(const asp_image2d_t *imgA, const asp_image2d_t *imgB, int *sameHeader, int *sameData)
  {
   /**
    *
    * Compare pixel data, and headers, of two images.
    *
    * @param imgA       First image to be compared
    * @param imgB       Second image to be compared         
    * @param sameHeader Used to return true/false of same header between images
    * @param sameData   Used to return true/false of same data between images
    *
    * @return ASP_SUCCESS flag indicating success or failure of functional processing.
    *
    */
    uint32_t i;

    /* check for null pointer */
    if( sameHeader == NULL ) {
      asp_error(asp_error_image2d_comparisonNullImage, "asp_image2d_compare null sameHeader");
      return ASP_FAILURE;
    }

    /* check for null pointer */
    if( sameData == NULL ) {
      asp_error(asp_error_image2d_comparisonNullImage, "asp_image2d_compare null sameData");
      return ASP_FAILURE;
    }


    (*sameHeader) = 0;
    (*sameData) = 0;

    /* check for null pointer */
    if( imgA == NULL ) {
      asp_error(asp_error_image2d_comparisonNullImage, "asp_image2d_compare null image A");
      return ASP_FAILURE;
    }

    /* check for null pointer */
    if( imgB == NULL ) {
      asp_error(asp_error_image2d_comparisonNullImage, "asp_image2d_compare null image B");
      return ASP_FAILURE;
    }

    /* check that the header data is the same */
     if((imgA->format      != imgB->format)      ||
        (imgA->temperature != imgB->temperature) ||
        (imgA->exposure    != imgB->exposure)    ||
        (imgA->gain        != imgB->gain)        ||
        (imgA->rows        != imgB->rows)        ||
        (imgA->cols        != imgB->cols)        ||
        (imgA->otop        != imgB->otop)        ||
        (imgA->oleft       != imgB->oleft)       ||
        (imgA->underflow   != imgB->underflow)   ||
        (imgA->overflow    != imgB->overflow)    ||
        (imgA->fixedPoint  != imgB->fixedPoint))       
    {
      (*sameHeader) = 0;
    } else {
      (*sameHeader) = 1;
    }

    // Ensure both images have the same numbers of pixels
    (*sameData) = 1;
    if(imgA->npix != imgB->npix){
      (*sameData) = 0;
      return ASP_SUCCESS;
    }

    // Check each pixel for equivalence between two images. Halt when different DN value identified.
    for (i=0; i<imgA->npix; i++)
    {
      if (imgA->data[i] != imgB->data[i])
      {
        (*sameData) = 0;
        return ASP_SUCCESS;
      }
    }

    return ASP_SUCCESS;
  }


  int asp_image2d_check(const char* name, const asp_image2d_t *a, 
                        const int bin0MaxVal, const int bin1MaxVal, 
                        const int bin4MaxVal, const int bin5MaxVal)
  {
   /**
    *
    * Main functionality of the IMGSTAT command.  Checks image for structure
    *   integrity, generates histogram of DN values, and basic statistics.  
    *   All are logged in an ACTIVITY_LO EVR for transmission to ground
    *
    * @param name       name of file being analyzed
    * @param a          image data
    * @param bin0MaxVal command defined histogram bin 0 max value
    * @param bin1MaxVal command defined histogram bin 1 max value
    * @param bin4MaxVal command defined histogram bin 4 max value
    * @param bin5MaxVal command defined histogram bin 5 max value
    * 
    * Histogram bin definition:
    *
    *    bin 0: (-max < value < bin0MaxVal)
    *    bin 1: (bin0MaxVal <= value < bin1Max)
    *    bin 2: (bin1Max <= value < 0)
    *    bin 3: (value == 0)
    *    bin 4: (0 < value <= bin4Max )
    *    bin 5: (bin4Max < value <= bin5Max)
    *    bin 6: (bin5Max < value <= max)
    *
    */
    int result = ASP_SUCCESS;
    int bin0Cum = 0;
    int bin1Cum = 0;
    int bin2Cum = 0;
    int bin3Cum = 0;
    int bin4Cum = 0;
    int bin5Cum = 0;
    int bin6Cum = 0;
    int minRow = 0;
    int minCol = 0;
    int maxRow = 0;
    int maxCol = 0;
    int fixPt = 1;
    uint32_t i = 0;
    char msg[ASP_MAX_LOGSTRING];
    double tmpPxVal = 0.0;
    double mean = 0;
    double meansq = 0;
    double stdev = 0;
    double minVal = ASP_PIXEL_MAX;
    double maxVal = ASP_PIXEL_MIN;
    double dnpix = 1;
    double var = 1;
    double contrast = 1;

    /* If name is not already allocated, return */
    if( name == NULL )
    {
      asp_error(asp_error_image2d_checkNullImage, "asp_image2d_check null name");
      return ASP_FAILURE;
    }

    /* If memory is not already allocated, return */
    if( a == NULL )
    {
      asp_error(asp_error_image2d_checkNullImage, "asp_image2d_check null image");
      return ASP_FAILURE;
    }

    /* If number of pixels != rows * cols, return */
    if (a->npix != (uint32_t)(a->rows * a->cols))    
    {
      asp_error(asp_error_image2d_checkDimensionsInconsistent, "asp_image2d_check inconsistent dimensions");
      return ASP_FAILURE;
    }

    /* If dims are larger than max allowable dims, return */
    if ((a->rows > ASP_MAX_ROWS) || (a->cols > ASP_MAX_COLS))    
    {
       asp_error(asp_error_image2d_checkDimensionsTooBig, "asp_image2d_check dimension(s) too big");
       return ASP_FAILURE;
    }

    /* if rows/cols < 0, return */
    if ((a->rows <= 0) || (a->cols <= 0))    
    {
      asp_error(asp_error_image2d_checkDimensionsZero, "asp_image2d_check dimension(s) of zero");
      return ASP_FAILURE;
    }

    /* height/width larger than allowed, return */
    if ((a->otop > ASP_MAX_ROWS) || (a->oleft > ASP_MAX_COLS))    
    {
      asp_error(asp_error_image2d_checkDimensionsTooBig, "asp_image2d_check bad location of crop corner");
      return ASP_FAILURE;
    }

    /* If exposure time < 0, return */
    if ((a->exposure < (float)0.0))    
    {
      (void)snprintf(msg, ASP_MAX_LOGSTRING, "%s bad exposure: %02.5e", name, a->exposure);
      asp_error( asp_error_image2d_checkBadExposure, msg);
      result = ASP_FAILURE;
    }

    /* If gain < 0, return */
    if ((a->gain < (float)0.0))    
    {
      (void)snprintf(msg, ASP_MAX_LOGSTRING, "%s bad gain: %04.7e", name, a->gain);
      asp_error( asp_error_image2d_checkBadGain, msg);
      result = ASP_FAILURE;
    }

    fixPt = a->fixedPoint;
    dnpix = (double)a->npix;

    // Sweep all pixels, calculate metrics like min/max, fill in histogram
    for(int r = 0; r < a->rows; r++) {
      for(int c= 0; c < a->cols; c++) {

        i = asp_image2d_get_index(a, c, r);
        tmpPxVal = (double)a->data[i] / (double)ASP_NONZERO(fixPt);
	
        mean += tmpPxVal / (double)ASP_NONZERO(dnpix); 
        meansq += (tmpPxVal * tmpPxVal) / (double)ASP_NONZERO(dnpix); 
	
        if(tmpPxVal >= maxVal){
            maxVal = tmpPxVal;
            maxRow = r;
            maxCol = c;
        }

        if(tmpPxVal <= minVal){
	          minVal = tmpPxVal;
              minRow = r;
	          minCol = c;
        }

        /* Determine histogram bin to incrememnt */
        if(tmpPxVal == 0.0) {
          bin3Cum += 1;
        }

        if(tmpPxVal > 0.0){

          if(tmpPxVal <= (double)bin4MaxVal) {
            bin4Cum += 1;
          }
          if((tmpPxVal > (double)bin4MaxVal) && (tmpPxVal <= (double)bin5MaxVal)) {
            bin5Cum += 1;
          }
          if((tmpPxVal > (double)bin5MaxVal) && (tmpPxVal <= (double)ASP_PIXEL_MAX)){
            bin6Cum += 1;
          }

        }

        if(tmpPxVal < 0.0){

          if(tmpPxVal >= (double)bin1MaxVal) {
            bin2Cum += 1;
          }

          if((tmpPxVal < (double)bin1MaxVal) && (tmpPxVal >= (double)bin0MaxVal)) {
            bin1Cum += 1;
          }

          if((tmpPxVal < (double)bin0MaxVal) && (tmpPxVal >= (double)ASP_PIXEL_MIN)) {
            bin0Cum += 1;
          }
        }
        
      }
    }

    // Finish calculating standard deviation
    var = (meansq - (mean*mean));  
    if (var>0)
    { 
      stdev = sqrt(var);
    }

    /* Root mean squared contrast calculation */
    contrast = (double)(maxVal - minVal) / (double)ASP_NONZERO(mean);

    /* Create strings to be stored in log buffer for EVR transmission */
    (void)snprintf(msg, ASP_MAX_LOGSTRING, "format:%x npix:%lu pid:%lu\n",
	            a->format, (unsigned long)a->npix, (unsigned long)a->productid);
    asp_imgstat(msg);

    (void)snprintf(msg, ASP_MAX_LOGSTRING, "time(w):%lu time(s):%lu\n",
              (unsigned long)a->timestamp_whole_second, (unsigned long)a->timestamp_sub_second);
    asp_imgstat(msg);

    (void)memset(&msg[0], 0, sizeof(msg));
    (void)snprintf(msg, ASP_MAX_LOGSTRING, "temp:%.5f exp:%.5f gain:%.5f r:%i\n",
	            a->temperature, a->exposure, a->gain, a->rows);
    asp_imgstat(msg);

    (void)memset(&msg[0], 0, sizeof(msg));
    (void)snprintf(msg, ASP_MAX_LOGSTRING, "c:%i t:%i l:%i uf:%lu of:%lu\n",
	            a->cols,a->otop, a->oleft, (unsigned long)a->underflow, (unsigned long)a->overflow);
    asp_imgstat(msg);

    (void)memset(&msg[0], 0, sizeof(msg));
    (void)snprintf(msg, ASP_MAX_LOGSTRING, "fp:%i mean:%.5f\n", a->fixedPoint, mean);
    asp_imgstat(msg);

    (void)memset(&msg[0], 0, sizeof(msg));
    (void)snprintf(msg, ASP_MAX_LOGSTRING, "std:%.5f cont:%.5f\n", stdev, contrast);
    asp_imgstat(msg);

    (void)memset(&msg[0], 0, sizeof(msg));
    (void)snprintf(msg, ASP_MAX_LOGSTRING, "min v:%.2f r:%i c:%i\n", minVal, minRow, minCol);
    asp_imgstat(msg);

    (void)memset(&msg[0], 0, sizeof(msg));
    (void)snprintf(msg, ASP_MAX_LOGSTRING, "max v:%.2f r:%i c:%i\n", maxVal, maxRow, maxCol);
    asp_imgstat(msg);

    (void)memset(&msg[0], 0, sizeof(msg));
    (void)snprintf(msg, ASP_MAX_LOGSTRING, "bin0(%d,%d):%i\n", ASP_PIXEL_MIN, bin0MaxVal, bin0Cum);
    asp_imgstat(msg);

    (void)memset(&msg[0], 0, sizeof(msg));
    (void)snprintf(msg, ASP_MAX_LOGSTRING, "bin1[%d,%d):%i\n", bin0MaxVal, bin1MaxVal, bin1Cum);
    asp_imgstat(msg);

    (void)memset(&msg[0], 0, sizeof(msg));
    (void)snprintf(msg, ASP_MAX_LOGSTRING, "bin2[%d,0):%i\n", bin1MaxVal, bin2Cum);
    asp_imgstat(msg);

    (void)memset(&msg[0], 0, sizeof(msg));
    (void)snprintf(msg, ASP_MAX_LOGSTRING, "bin3(px == 0):%i\n", bin3Cum);
    asp_imgstat(msg);

    (void)memset(&msg[0], 0, sizeof(msg));
    (void)snprintf(msg, ASP_MAX_LOGSTRING, "bin4(0,%d]:%i\n", bin4MaxVal, bin4Cum);
    asp_imgstat(msg);

    (void)memset(&msg[0], 0, sizeof(msg));
    (void)snprintf(msg, ASP_MAX_LOGSTRING, "bin5(%d,%d]:%i\n", bin4MaxVal, bin5MaxVal, bin5Cum);
    asp_imgstat(msg);

    (void)memset(&msg[0], 0, sizeof(msg));
    (void)snprintf(msg, ASP_MAX_LOGSTRING, "bin6(%d,%d]:%i\n", bin5MaxVal, ASP_PIXEL_MAX, bin6Cum);
    asp_imgstat(msg);

    /* Check for acceptable image format identifiers */
    if ((a->format != ASP_PRODTYPE_RAW             ) &&
        (a->format != ASP_PRODTYPE_FLAT            ) &&   
        (a->format != ASP_PRODTYPE_DARK            ) &&   
        (a->format != ASP_PRODTYPE_MASK            ) &&     
        (a->format != ASP_PRODTYPE_CAL             ) &&
        (a->format != ASP_PRODTYPE_COADD           ) &&
        (a->format != ASP_PRODTYPE_DOWNSAMPLE      ) &&
        (a->format != ASP_PRODTYPE_SUBTRACT        ) &&
        (a->format != ASP_PRODTYPE_ICER_RAW        ) &&
        (a->format != ASP_PRODTYPE_ICER_FLAT       ) &&   
        (a->format != ASP_PRODTYPE_ICER_DARK       ) &&   
        (a->format != ASP_PRODTYPE_ICER_MASK       ) &&     
        (a->format != ASP_PRODTYPE_ICER_CAL        ) &&
        (a->format != ASP_PRODTYPE_ICER_COADD      ) &&
        (a->format != ASP_PRODTYPE_ICER_DOWNSAMPLE ) &&
        (a->format != ASP_PRODTYPE_ICER_SUBTRACT   ))
    {
      (void)snprintf(msg, ASP_MAX_LOGSTRING, "%s bad format: %x", name, a->format);
      asp_error( asp_error_image2d_checkBadFormatType, msg);
      result = ASP_FAILURE;
    }

    return result;
  }

#endif

/* Indentation settings for Vim and Emacs.  Please do not modify.
 *
 * Local Variables:
 * c-basic-offset: 2
 * indent-tabs-mode: nil
 * End:
 *
 * vim: et sts=2 sw=2
 */

