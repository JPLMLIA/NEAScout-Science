/** 
 * \file    asp_common.h
 * \brief   General ASP types, declarations, and macros
 * \author  Brian Bue
 *
 * Copyright 2013 California Institute of Technology.  ALL RIGHTS RESERVED.
 * U.S. Government Sponsorship acknowledged.
 */

#ifndef asp_common_h
#define asp_common_h

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "asp_image2d.h"

#define DEBUG_LEVEL 1
#if DEBUG_LEVEL > 0
#  define WRITE_DEBUG_IMAGES
#endif

#define ASP_FAILURE -1
#define ASP_SUCCESS  0

#define ASP_NONZERO(x) ((x)==0 ? 1 : (x))
#define ASP_ROUND(a)   ((a)  >  0  ? (int)((a) + 0.5) : (int)((a) - 0.5))
#define ASP_ABS(a)     ((a)  >  0  ? (a) : -(a))
#define ASP_MIN(a,b)   ((a)  < (b) ? (a) :  (b))
#define ASP_MAX(a,b)   ((a) >= (b) ? (a) :  (b))

#define ASP_MAX_LOGSTRING (110)
#define ASP_EPS       1e-9
#define NSSI_FILENAME_LENGTH 14
#define MAX_FILEPATH_LENGTH 80

/* Recommended method to support C standard __func__ macro with older
 * versions of GCC.  See
 * http://gcc.gnu.org/onlinedocs/gcc-4.0.4/gcc/Function-Names.html.
 */
#if __STDC_VERSION__ < 199901L
#  if __GNUC__ >= 2
#    define __func__ __FUNCTION__
#  else
#    define __func__ "<unknown>"
#  endif
#endif

void 
swap2(void *v);

void 
swap4(void *v);

#endif  /* asp_h */


/* Indentation settings for Vim and Emacs.  Please do not modify.
 *
 * Local Variables:
 * c-basic-offset: 2
 * indent-tabs-mode: nil
 * End:
 *
 * vim: et sts=2 sw=2
 */
