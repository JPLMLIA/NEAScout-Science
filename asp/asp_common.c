/** 
 * \file    asp_common.c
 * \brief   Basic ASP types and functions
 * \author  Brian Bue, based on asp_image.c by Ben Bornstein
 *
 * Copyright 2011 California Institute of Technology.  ALL RIGHTS RESERVED.
 * U.S. Government Sponsorship acknowledged.
 */

#ifndef ASP_COMMON_C
#define ASP_COMMON_C

#include "asp_common.h"
#include <stdio.h>

 /**
  *
  * @param v pointer to 4 byte memory chunk to be endian-swapped
  *
  *   Utility functions for swapping a specific endianness to
  *    accomodate the machine's native format.
  *
  */
void swap4(void *v)
{

    char in[4];
    char out[4];
    (void)memcpy(in, v, 4);
    out[0] = in[3];
    out[1] = in[2];
    out[2] = in[1];
    out[3] = in[0];
    (void)memcpy(v, out, 4);
}


 /**
  *
  * @param v pointer to 2 byte memory chunk to be endian-swapped
  *
  * Utility functions for swapping  a specific endianness to
  *   accomodate the machine's native format.
  *
  */
void swap2(void *v)
{

    char in[2];
    char out[2];
    (void)memcpy(in, v, 2);
    out[0] = in[1];
    out[1] = in[0];
    (void)memcpy(v, out, 2);
}


#endif


