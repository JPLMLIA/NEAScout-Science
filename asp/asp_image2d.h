/** 
 * \file    asp_image2d.h
 * \brief   2d image data structure
 * \author  Brian Bue, based on asp_image.h by Ben Bornstein
 *
 * Copyright 2013 California Institute of Technology.  ALL RIGHTS RESERVED.
 * U.S. Government Sponsorship acknowledged.
 */

#ifndef ASP_IMAGE2D_H
#define ASP_IMAGE2D_H

#include <stdio.h>
#include "Fw/Types/BasicTypes.hpp"

#define ASP_PIXEL_MAX                (32767)
#define ASP_PIXEL_MIN                (-32767)

#if defined(BUILD_SPHINX)
    #define ASP_USES_BIGENDIAN       (0)
#else
    #define ASP_USES_BIGENDIAN       (1)
#endif

#define ASP_IGNORE_PIXEL              (1) 
#define MAX_HEADER_CHARS              (10000)
#define ASP_MAX_ROWS                  (2184)
#define ASP_MAX_COLS                  (3840)
#define ASP_IMAGE_HEADER_BYTES        (44)
#define ASP_IMAGE_PIXEL_BYTES         (2)
#define MAX_FILE_LENGTH               (35)

#define ASP_PRODTYPE_PACKED           (0xFF00)
#define ASP_PRODTYPE_RAW              (0xFF01)
#define ASP_PRODTYPE_FLAT             (0xFF02)
#define ASP_PRODTYPE_DARK             (0xFF03)
#define ASP_PRODTYPE_MASK             (0xFF04)
#define ASP_PRODTYPE_CAL              (0xFF05)
#define ASP_PRODTYPE_COADD            (0xFF06)
#define ASP_PRODTYPE_DOWNSAMPLE       (0xFF07)
#define ASP_PRODTYPE_SUBTRACT         (0xFF08)

#define ASP_PRODTYPE_ICER_RAW         (0xFA01)
#define ASP_PRODTYPE_ICER_FLAT        (0xFA02)
#define ASP_PRODTYPE_ICER_DARK        (0xFA03)
#define ASP_PRODTYPE_ICER_MASK        (0xFA04)
#define ASP_PRODTYPE_ICER_CAL         (0xFA05)
#define ASP_PRODTYPE_ICER_COADD       (0xFA06)
#define ASP_PRODTYPE_ICER_DOWNSAMPLE  (0xFA07)
#define ASP_PRODTYPE_ICER_SUBTRACT    (0xFA08)

#define IMAGE_DATA_OFFSET             (876)
#define CAMERA_HEADER_BLOCKS          (208)

#define ASP_MAX_STARS                 (25)
#define ASP_MAX_COADD_FRAMES          (25)

typedef int16_t asp_pixel_t;

/**
 * Computes the 1d index of a pixel in the image.
 * 
 * NOTE: necessary to do bounds checking in the calling function!
 */
#define asp_image2d_get_index(image,x,y) ((y)*(image)->cols+(x))


/**
 * Computes the 1d index of a pixel given the x dimension of the image.
 * 
 * NOTE: necessary to do bounds checking in the calling function!
 */
#define asp_image2d_get_index_from_xdim(xdim,x,y) ((y)*(xdim)+(x))


/**
 * Retrieves the pointer to the pixel at location Image(x,y) in the image. 
 * 
 * NOTE: necessary to do bounds checking in the calling function!
 */
#define asp_image2d_get_pixel(image,x,y) ((image)->data+((y)*(image)->cols+(x)))


/**
 * Images are simply a small metadata wrapper around raw pixel
 * data.
 *
 * NOTE: IMAGES OWN THEIR UNDERLYING DATA.
 */
typedef struct {

  // Camera Header - 832 bytes 
  int32_t              cameraHeader[208];

  // NSI header - 44 bytes
  uint16_t             format;
  uint32_t             productid;
  uint32_t             timestamp_whole_second;
  uint32_t             timestamp_sub_second;
  float                temperature;
  float                exposure;
  float                gain;
  uint16_t             rows;   // rows of this raster
  uint16_t             cols;   // columns in this raster 
  uint16_t             otop;   // top relative to original frame  
  uint16_t             oleft;  // left relative to original frame
  uint32_t             underflow;
  uint32_t             overflow;
  uint16_t             fixedPoint;

  // Data Payload
  asp_pixel_t         *data;

  uint32_t             npix;   // cached in RAM but not stored in file
  uint16_t             buffer; // cached in RAM but not stored in file

} asp_image2d_t;


/**
 * This struct represents a coadded stack, including the reference star 
 * locations, numbers, and window sizes in the stack, the three most recent 
 * images to be added to the stack (kept for the purposes of temporal median 
 * filtering), and the coadded stack image itself.  The struct owns the 
 * coadded stack image (<base>) via the asp_image2d_real_t struct, as well as 
 * the underlying data to the most recent images. The images are allocated 
 * elsewhere and then "donated" to the coadded stack for filtering and 
 * accumulation.  The struct also owns the memory where the reference 
 * star coordinates are held (<ref_coords_x>, <ref_coords_y>).  It 
 * periodically frees image memory from the stack to make room for new frames.
 * On its destruction, all base and source images are freed.
 * 
 * Members:
 *   numrefs: the number of reference stars in the stack
 *   radius: a parameter controlling the size of the reference windows around 
 *           the reference stars (of size 2*<radius>+1)
 *   ref_coords_x: contains <numrefs> integers specifying the columns of the 
 *           reference stars.  The struct owns this memory
 *   ref_coords_y: contains <numrefs> integers specifying the rows of the 
 *           reference stars.  The struct owns this memory
 *   img1: The third most recent image to be added to the stack.  The struct 
 *           owns this memory after recieving it with "donate" 
 *   img2: The second most recent image to be added to the stack. The struct 
 *           owns this memory after recieving it with "donate"  
 *   img3: the most recent image to be added to the stack.   The struct 
 *           owns this memory after recieving it with "donate"  
 *    base: The underlying coadded stack image.  The struct owns this memory
 */
typedef struct
{
    int numrefs;
    int radius;
    int* ref_coords_x;
    int* ref_coords_y;
    asp_image2d_t* img1;
    asp_image2d_t* img2;
    asp_image2d_t* img3;
    asp_image2d_t* base;
    int offx1, offx2, offx3;
    int offy1, offy2, offy3;
    int nframes;
} asp_costack_t;

int 
asp_skip_comments(FILE *fp);

int 
asp_image2d_cmp_dims(const void *a, const void *b);

void 
asp_image2d_cptelem(const asp_image2d_t *src, asp_image2d_t *dst);

int 
asp_image2d_check(const char* name, const asp_image2d_t *A,  const int bin0MaxVal, 
                  const int bin1MaxVal, const int bin4MaxVal, const int bin5MaxVal);

int 
asp_image2d_compare(const asp_image2d_t *imgA, const asp_image2d_t *imgB, int *sameHeader, int *sameData);

#endif  /* asp_image2d_h */

/* Indentation settings for Vim and Emacs.  Please do not modify.
 *
 * Local Variables:
 * c-basic-offset: 2
 * indent-tabs-mode: nil
 * End:
 *
 * vim: et sts=2 sw=2
 */
