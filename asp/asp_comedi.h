/**
 * \file    asp_comedi.h
 * \brief   Temporal median processing for NEASCOUT
 * \author  William Huffman, Brian Bue, and David R Thompson
 *
 * Copyright 2014 California Institute of Technology.  ALL RIGHTS RESERVED.
 * U.S. Government Sponsorship acknowledged.
 */

#ifndef asp_comedi_h
#define asp_comedi_h

#include "asp_image2d.h"

asp_pixel_t 
get_median(asp_pixel_t a, asp_pixel_t b, asp_pixel_t c);

int 
asp_add_temporal_median(asp_costack_t* stack);

#endif
