/** 
 * \file    asp_calib.c
 * \brief   Level 1 processing for NEASCOUT
 * \author  Brian Bue, David R Thompson, Jack Lightholder
 *
 * Copyright 2014 California Institute of Technology.  ALL RIGHTS RESERVED.
 * U.S. Government Sponsorship acknowledged.
 */

#include "asp_common.h"
#include "asp_error.h"
#include "asp_image2d.h"
#include "asp_calib.h"

#include <stdio.h>
#include <math.h>


  int asp_calib(asp_image2d_t *img, asp_image2d_t *m_array, asp_image2d_t *b_array, asp_image2d_t *dk, 
                asp_image2d_t *bad, const float rcc, const int fixPt, int temperature)
  {
    /*
     * Perform L1 calibration on a raw image.
     *
     * @param img pointer to image with raw pixel data
     * @param m_array flat field slope value for each pixel (y=mx+b linear where x is temperature)
     * @param b_array flat field intercept value for each pixel (y=mx+b linear where x is temperature)
     * @param dk dark current map to be subtracted from raw image
     * @param bad bad pixel mask.  1 signifies bad pixel, 0 signifies good
     * @param rcc radiometric calibration coefficient
     * @param fixPt fixed point scaler to be applied to resulting image
     * @param temperature if -99 use the image header temperature. Otherwise, use the value specified for flat field 
     *
     * @return ASP_SUCCESS flag indicating success or failure of the function.
     *
     * Perform flat-field and dark-current subtraction (in-place) on img 
     * flat and dk must be the same dimensions as img, and by convention 
     * the reserved value of ASP_IGNORE_PIXEL signifies bad data
     * that should be set to zero.  Finally, multiply by the radiometric
     * calibration coefficient rcc.
     *
     * If either raw image, flat field, dark currrents or bad pixel mask are NULL, 
     * we skip the associated operation.
     *
     * Flat field scaling is determined by two images, m_array and b_array.  These data frames
     * take the form of a standard NSI image, but capture the linear flat field response per-pixel
     * across the allowable flight temperature. m_array has 10,000,000 scaling hard coded, b_array has
     * 10,000.  These are hard-coded because the fixed point header field in the imagine is not large enough
     * for images of these size.  This calibration model was adapted late in development when it was realized
     * we could not upload as many calibration files to the flight system as originally intended. Instead of 
     * adjusting the NSI file format in all parts of the flight system, the values were hard-coded.  Since these
     * calibration files are static, their precision should never need to be changed.  Any new files which were to 
     * uploaded would be made with these levels of precision.
     *
     * y = mx+b is calculated where m is taken from m_array, b from b_array and x is the temperature of the raw image.
     * y will be a value between 0 and 1, representing the responsivness of that pixel at a given temperature.
     * The raw image will have the dark current subtracted, and then be divided by y. In the case where y=1, the pixel
     * was optimally responsive, and the resulting calibrated image will not change DN.  Anything with less than full response
     * will be scaled up to adjust for limited pixel response.  More information about this calibration process can be found
     * in the calibration report.
     *
     *
     */
    int calIndex;
    int rawIndex;
    int c;
    int r; 
    char msg[ASP_MAX_LOGSTRING];
    double x; 
    double df;
    double ff;
    double m;
    double b;
    double refTemp;
    int32_t testPx;
    asp_pixel_t subTmp;
    
    // Scales built into onboard m_array and b_array files. 
    double m_array_scale = 10000000;
    double b_array_scale = 10000;

    // null image pointer check for raw image
    if (img == NULL) {
        asp_error( asp_error_calib_image_invalid, "asp_calib raw image is null" );
        return ASP_FAILURE;
    }

    // null image pointer check for flat field m_array
    if (m_array == NULL) {
        asp_error( asp_error_calib_image_invalid, "asp_calib m_array is null" );
        return ASP_FAILURE;
    }

    // null image pointer check for flat field b_array
    if (b_array == NULL) {
        asp_error( asp_error_calib_image_invalid, "asp_calib b_array is null" );
        return ASP_FAILURE;
    }

    // null image pointer check for dark image
    if (dk == NULL) {
        asp_error( asp_error_calib_image_invalid, "asp_calib dark image is null" );
        return ASP_FAILURE;
    }

    // null image pointer check for bad pixel mask
    if (bad == NULL) {
        asp_error( asp_error_calib_image_invalid, "asp_calib bad image is null" );
        return ASP_FAILURE;   
    }

    // Ensure flat field m_array dimensions match those of b_array
    if (asp_image2d_cmp_dims(b_array, m_array)) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "mismatched dims (m_array r:%d c:%d) (b_array r:%d c:%d)",
            m_array->rows, m_array->cols, b_array->rows, b_array->cols);
        asp_error(asp_error_calib_badDimensions, msg);
        return ASP_FAILURE;
    }

    // Ensure bad pixel mask dimensions match those of dark frame
    if (asp_image2d_cmp_dims(bad, dk)) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "mismatched dims (bad r:%d c:%d) (dark r:%d c:%d)",
            bad->rows, bad->cols, dk->rows, dk->cols);
        asp_error(asp_error_calib_badDimensions, msg);
        return ASP_FAILURE;
    }

    // Ensure b_array dimensions match those of bad pixel mask
    if (asp_image2d_cmp_dims(b_array, bad)) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "mismatched dims (b_array r:%d c:%d) (bad r:%d c:%d)",
            b_array->rows, b_array->cols, bad->rows, bad->cols);
        asp_error(asp_error_calib_badDimensions, msg);
        return ASP_FAILURE;
    }

    // Ensure input images which are sections of the full frame do not run out of bounds
    if(((img->cols + img->oleft) > ASP_MAX_COLS) ||
       ((img->rows + img->otop) > ASP_MAX_ROWS)) {
            (void)snprintf(msg, ASP_MAX_LOGSTRING, "bad parms r:%d c:%d t:%d l:%d",
            img->rows, img->cols, img->otop, img->oleft);
            asp_error(asp_error_calib_badDimensions, msg);
            return ASP_FAILURE;
    }

    // Ensure raw image or dark don't have negative fixed point values
    if((img->fixedPoint <= 0) ||
       (dk->fixedPoint <= 0)) {
            (void)snprintf(msg, ASP_MAX_LOGSTRING, "bad fp img:%d dark:%d",
            img->fixedPoint, dk->fixedPoint);
            asp_error(asp_error_calib_badDimensions, msg);
            return ASP_FAILURE;       
    }

    // Ensure rcc is larger than or equal to 0.
    if (rcc <= 0) {
        asp_error( asp_error_calib_rcc_invalid, "asp_calib invalid rcc" );
        return ASP_FAILURE;
    }

    img->overflow = 0;
    img->underflow = 0;

    if(temperature == -99){
        // Use temperature of raw image being calibrated.
        refTemp = (double)img->temperature;
    }else{
        // Use temperature passed in as temperature argument.
        refTemp = (double)temperature;   
    }

    for (c=0; c<img->cols; c++) {
        for (r=0; r<img->rows; r++) {

            rawIndex = asp_image2d_get_index(img, c, r );
            calIndex = asp_image2d_get_index(bad, (c+img->oleft), (r+img->otop));

            // Is the pixel flagged as bad data? Set it to zero.
            if ((bad != NULL) && (bad->data[calIndex] == ASP_IGNORE_PIXEL)) {
                img->data[rawIndex] = 0;
            }
            else {

                // convert all to floating point, divide by fixed point, subtract dark
                x = (double)img->data[rawIndex] / (double)ASP_NONZERO(img->fixedPoint);
                df = (double)dk->data[calIndex] / (double)ASP_NONZERO(dk->fixedPoint);

                // calculate m and b values, divide by known scaling factors.
                m = (double)m_array->data[calIndex] / (double)ASP_NONZERO(m_array_scale);
                b = (double)b_array->data[calIndex] / (double)ASP_NONZERO(b_array_scale);
                ff = (m * refTemp) + b;

                // Do not fit line beyond allowable flight temperature (high temp)
                if(ff > 1.0){
                    ff = 1.0;
                }

                // Do not fit line beyond allowable flight temperature (low temp)
                if(ff <= 0.0){
                    ff = 1.0 / (double)ASP_NONZERO(m_array_scale);
                }

                // dark-subtracted, flat scaled and radiometrically corrected pixel.
                x = ((x-df) * rcc) / (double)ASP_NONZERO(ff);

                subTmp = (asp_pixel_t)ASP_ROUND((x * (double)ASP_NONZERO(fixPt)));
                testPx = (int32_t)ASP_ROUND((x * (double)ASP_NONZERO(fixPt)));

                // underflow / overflow protection
                if(subTmp == testPx) {
                    img->data[rawIndex] = (asp_pixel_t)subTmp;
                }

                if(testPx > ASP_PIXEL_MAX) {
                    img->data[rawIndex] = (asp_pixel_t)ASP_PIXEL_MAX;
                    img->overflow += 1;
                }

                if(testPx < ASP_PIXEL_MIN) {
                    img->data[rawIndex] = (asp_pixel_t)ASP_PIXEL_MIN;
                    img->underflow += 1;
                }

            }
        }
    }

    img->format = ASP_PRODTYPE_CAL;
    return ASP_SUCCESS;
  }


  int asp_dynamic_range(asp_image2d_t *img, int *px_min, int *px_max)
  {
   /**
    * Calculate dynamic range of an image. 
    *
    * @param img image for which dynamic range will be calculated
    * @param px_min return reference for minimum pixel value in image
    * @param px_max return reference for maximum pixel value in image
    *
    * @return ASP_SUCCESS if the procedure succeeds, ASP_FAILURE if 
                            an error condition was hit.
    *
    */
    char msg[ASP_MAX_LOGSTRING];
    asp_pixel_t px;
    asp_pixel_t max_px = ASP_PIXEL_MIN;
    asp_pixel_t min_px = ASP_PIXEL_MAX;

    if (img == NULL) {
        asp_error(asp_error_dynamic_range_null, "asp_dynamic_range null input");
        return ASP_FAILURE;
    }

    if(img->rows <= 0 || img->rows > ASP_MAX_ROWS) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "asp_dynamic_range: Rows out of rage at %d", img->rows);
        asp_error(asp_error_dynamic_range_row, msg);
        return ASP_FAILURE;
    }

    if(img->cols <= 0 || img->cols > ASP_MAX_COLS) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "asp_dynamic_range: Columns out of rage at %d", img->cols);
        asp_error(asp_error_dynamic_range_col, msg);
        return ASP_FAILURE;
    }

    for(int r=0; r<img->rows; r++) {
        for(int c=0; c<img->cols; c++) {

            px =  *asp_image2d_get_pixel(img, c, r);

           if(px > max_px){
               max_px = px;
           }

           if(px < min_px){
               min_px = px;
           }

        }
    }

    *px_max = max_px;
    *px_min = min_px;

    return ASP_SUCCESS;
  }


