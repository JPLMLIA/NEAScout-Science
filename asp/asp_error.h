/**
 * \file    asp_error.h
 * \brief   Decls for error reporting methods used in asp.
 * \author  Steve Schaffer, Jack Lightholder
 *
 * Copyright 2011 California Institute of Technology.  ALL RIGHTS RESERVED.
 * U.S. Government Sponsorship acknowledged.
 */

#ifndef ASP_ERROR_H
#define ASP_ERROR_H

#include "asp_common.h" //for __func__


#define BUFFER_DEPTH 30

/* Stores errors/warnings created by ASP library. Become EVRs in ComponentImpl */
extern char errorEventBuffer[BUFFER_DEPTH][ASP_MAX_LOGSTRING];
extern char logEventBuffer[BUFFER_DEPTH][ASP_MAX_LOGSTRING];

/* Counters to track how full log/error EventBuffers are */
extern int errorBufferIndex;
extern int logBufferIndex;

void 
reset_log_buffer(void);

void 
reset_error_buffer(void);

/*
 * error codes enumeration
 */
typedef enum
{
  asp_error_none                                        = 0x0000,
  
  /* hardware errors prefixed 0x000 */
  asp_error_hw_nullParameter                            = 0x0001,

  asp_error_log_fileOpenFailed                          = 0x0101,
  
  asp_error_memory_blocksExhausted                      = 0x0201,
  asp_error_memory_addressNotFound                      = 0x0202,
  asp_error_memory_blockRange                           = 0x0203,
  asp_error_memory_bytesExhausted                       = 0x0204,
  asp_error_memory_allocatorFailed                      = 0x0205,
  
  asp_error_image_imageAllocatorFailed                  = 0x0301,
  asp_error_image_nullImage                             = 0x0302,
  asp_error_image_sampleIndexRange                      = 0x0303,
  asp_error_image_lineIndexRange                        = 0x0304,
  asp_error_image_getIndexFailed                        = 0x0305,

  asp_error_image2d_imageAllocatorFailed                = 0x0401,
  asp_error_image2d_nullImage                           = 0x0402,
  asp_error_image2d_sampleIndexRange                    = 0x0403,
  asp_error_image2d_lineIndexRange                      = 0x0404,
  asp_error_image2d_getIndexFailed                      = 0x0405,
  asp_error_image2d_skipCommentsMalformedHeader         = 0x0406,
  asp_error_image2d_cannotOpenFile                      = 0x0407,
  asp_error_image2d_writeImageFailed                    = 0x0408,
  asp_error_image2d_readImageCannotOpenFile             = 0x0409,
  asp_error_image2d_readImageWrongFileType              = 0x0410,
  asp_error_image2d_readImagePixelOverflow              = 0x0411,
  asp_error_image2d_readImageMalformedHeader            = 0x0412,
  asp_error_image2d_readImageBadImageParameters         = 0x0413,
  asp_error_image2d_readImageCannotAllocateImage        = 0x0414,
  asp_error_image2d_readImageInsufficientData           = 0x0415,
  asp_error_image2d_unsupportedProduct                  = 0x0416,
  asp_error_image2d_comparisonNullImage                 = 0x0417,
  asp_error_image2d_comparisonNullParameter             = 0x0418,
  asp_error_image2d_checkNullImage                      = 0x0419,
  asp_error_image2d_checkDimensionsInconsistent         = 0x0420,
  asp_error_image2d_checkDimensionsTooBig               = 0x0421,
  asp_error_image2d_checkDimensionsZero                 = 0x0422,
  asp_error_image2d_checkBadTimestamp                   = 0x0423,
  asp_error_image2d_checkBadTemperature                 = 0x0424,
  asp_error_image2d_checkBadExposure                    = 0x0425,
  asp_error_image2d_checkBadGain                        = 0x0426,
  asp_error_image2d_checkBadFormatType                  = 0x0427,
  asp_error_image2d_checkFileTooLarge                   = 0x0428, 
  asp_error_image2d_compare_mismatch_npix               = 0x0429,
  asp_error_image2d_insufficient_npix                   = 0x0430,
  asp_error_image2d_bad_bit_depth                       = 0x0431,
                                                        
  asp_error_conncomp_nullParameter                      = 0x0501, 
  asp_error_conncomp_zeroimage                          = 0x0502,      
  asp_error_conncomp_badDimensions                      = 0x0503, 
  asp_error_conncomp_outofmemory                        = 0x0504,
  asp_error_conncomp_outoflabels                        = 0x0505,

  asp_error_hull_nullParameter                          = 0x0601,
  asp_error_hull_arrayAllocatorFailed                   = 0x0602,
  asp_error_hull_noInputPoints                          = 0x0603,

  asp_error_rockster_nullParameter                      = 0x0701,
  asp_error_rockster_arrayAllocatorFailed               = 0x0702,
  asp_error_rockster_imageDataRange                     = 0x0703,
  asp_error_rockster_edgeDetectionFailed                = 0x0704,
  asp_error_rockster_noEdgesDetected                    = 0x0705,
  asp_error_rockster_CsetToEdgeFailed                   = 0x0706,
  asp_error_rockster_gSmoothFailed                      = 0x0707,
  asp_error_rockster_gradSupFailed                      = 0x0708,
  asp_error_rockster_edgeLinkFailed                     = 0x0709,
  asp_error_rockster_followSegmentFailed                = 0x0710,
  asp_error_rockster_CsetAllocatorFailed                = 0x0711,
  asp_error_rockster_CsetMaxSize                        = 0x0712,
  asp_error_rockster_connectNeighbor                    = 0x0713,

  asp_error_remove_cr_imageAllocatorFailed              = 0x0800,
  asp_error_remove_cr_arrayAllocatorFailed              = 0x0801,
  asp_error_remove_cr_connCompFailed                    = 0x0803,
  asp_error_remove_cr_filterCompFailed                  = 0x0804,

  asp_error_segment_surfaces_arrayAllocatorFailed       = 0x0900,
  asp_error_segment_surfaces_rocksterFailed             = 0x0901,
  asp_error_segment_surfaces_hullFailed                 = 0x0902,
  asp_error_segment_surfaces_rasterizeFailed            = 0x0903,

  asp_error_detect_plumes_noValidLimbPixels             = 0x1000,

  asp_error_medfilt_memory                              = 0x1100,

  asp_error_meanshift_memory                            = 0x1200,

  asp_error_detect_brightspots_image_invalid            = 0x1300,
  asp_error_detect_brightspots_undef_rf_file            = 0x1301,
  asp_error_detect_brightspots_memory                   = 0x1302,
  asp_error_detect_brightspots_loadforest               = 0x1303,
  asp_error_detect_brightspots_nclasses                 = 0x1304,
  asp_error_detect_brightspots_classify                 = 0x1305,
  asp_error_detect_brightspots_init_tc                  = 0x1306,
  asp_error_detect_brightspots_medfilt                  = 0x1307,
  asp_error_detect_brightspots_meanshift                = 0x1308,
  asp_error_detect_brightspots_extract_feats            = 0x1309,
  asp_error_detect_brightspots_free_forest_null         = 0x1310,
  asp_error_detect_brightspots_free_image_null          = 0x1311,

  asp_error_calib_image_invalid                         = 0x1400,
  asp_error_calib_darkimage_invalid                     = 0x1401,
  asp_error_calib_flatfield_invalid                     = 0x1402,
  asp_error_calib_badDimensions                         = 0x1403,
  asp_error_calib_rcc_invalid                           = 0x1404,
  asp_error_calib_findBrightPxNullImage                 = 0x1405,

  asp_error_crop_cropImageNullInput                     = 0x1500,
  asp_error_crop_cropImageTopValueOutOfBounds           = 0x1501,
  asp_error_crop_cropImageBottomValueOutOfBounds        = 0x1502,
  asp_error_crop_cropImageDestinationHeightOutOfBounds  = 0x1503,
  asp_error_crop_cropImageLeftValueOutOfBounds          = 0x1504,
  asp_error_crop_cropImageRightValueOutOfBounds         = 0x1505,
  asp_error_crop_cropImageDestinationWidthOutOfBounds   = 0x1506,
  asp_error_crop_cropImageCouldNotAllocateImage         = 0x1507,
  asp_error_crop_subsampleImageNullInput                = 0x1508, 
  asp_error_crop_subsampleImageFactorOutOfBounds        = 0x1509,
  asp_error_crop_subsampleImageCouldNotAllocateImage    = 0x1510,
  asp_error_crop_searchImageNullInput                   = 0x1510,
  asp_error_crop_searchImageTopValueOutOfBounds         = 0x1511, 
  asp_error_crop_searchImageBottomValueOutOfBounds      = 0x1512, 
  asp_error_crop_searchImageLeftValueOutOfBounds        = 0x1513, 
  asp_error_crop_searchImageRightValueOutOfBounds       = 0x1514, 
  asp_error_crop_subtractImageNullInput                 = 0x1515, 
  asp_error_crop_subtractImageBadDimensions             = 0x1516, 
  asp_error_crop_subtractImageCouldNotAllocateImage     = 0x1517,
  asp_error_crop_searchImage_boundsError                = 0x1518, 
                                                          
  asp_error_icerread_outOfMemory                        = 0x1600,
  asp_error_icerread_badHdr                             = 0x1601,
  asp_error_icerread_unsupportedProduct                 = 0x1602,
  asp_error_icerread_malformedheader                    = 0x1603,
  asp_error_icerread_endoffile                          = 0x1604,
  asp_error_icerread_dimensionsdisagree                 = 0x1605,
  asp_error_icerread_decompressfailed                   = 0x1606,
  asp_error_icerread_error_opening_file                 = 0x1607,
  asp_error_icerread_error_reading_file                 = 0x1608,
  asp_error_icerread_fseek_error                        = 0x1609,
  asp_error_icerread_malloc_error                       = 0x1610,
  asp_error_icerread_calloc_error                       = 0x1611,

  asp_error_icerwrite_unsupportedProduct                = 0x1700,
  asp_error_icerwrite_error_writing_file                = 0x1701,
  asp_error_icer_status                                 = 0x1702,
  asp_error_icer_bad_block_count                        = 0x1703,

  asp_error_coadd_null_input                            = 0x1800,
  asp_error_coadd_nonpositive_stack_parameters          = 0x1801,
  asp_error_coadd_radius_oversize                       = 0x1802,
  asp_error_coadd_stack_malloc_failure                  = 0x1803,   
  asp_error_coadd_ref_coords_x_malloc_failure           = 0x1804,
  asp_error_coadd_ref_coords_y_malloc_failure           = 0x1805,
  asp_error_coadd_base_malloc_failure                   = 0x1806,
  asp_error_coadd_base_size_mismatch                    = 0x1807,
  asp_error_coadd_reference_stars                       = 0x1808,
  asp_error_coadd_telemetrycopy                         = 0x1809,
  asp_error_coadd_inconsistent_state                    = 0x1810,
  asp_error_coadd_low_scaling                           = 0x1811,
  asp_error_coadd_shift_calculation                     = 0x1812,
  asp_error_coadd_insufficient_frames                   = 0x1813,
  asp_error_coadd_filename_increment_failed             = 0x1814,
  asp_error_coadd_read_image_failed                     = 0x1815,
  asp_error_coadd_stack_allocation_failed               = 0x1816,
  asp_error_coadd_stack_donation_failure                = 0x1817,
  asp_error_coadd_open_image_failure                    = 0x1818,
  asp_error_coadd_write_output_failure                  = 0x1819,
  asp_error_coadd_excessive_ref_stars                   = 0x1820,
  asp_error_coadd_excessive_frames                      = 0x1821,
  asp_error_coadd_excessive_stars                       = 0x1822,
  asp_error_coadd_temporal_median                       = 0x1823,

  asp_error_bbox_malloc_failure                         = 0x1900,
  asp_error_bbox_bad                                    = 0x1901,

  asp_error_smooth_smoothImageNullInput                 = 0x2000,
  asp_error_smooth_smoothImageWidthOutOfBounds          = 0x2001,
  asp_error_smooth_smoothImageSigmaOutOfBounds          = 0x2002,
  asp_error_smooth_smoothImageCouldNotAllocateImage     = 0x2003,
  asp_error_smooth_smoothImageCouldNotAllocateKernel    = 0x2004,
  asp_error_smooth_smoothKernelLessThanOne              = 0x2005,
  asp_error_smooth_inputImageDimsMismatch               = 0x2006,

  asp_error_level_one_invalid_dark_frame                = 0x2100,
  asp_error_level_one_read_dark_failed                  = 0x2101,
  asp_error_level_one_invalid_dark_filetype             = 0x2102,
  asp_error_level_one_invalid_flat_frame                = 0x2103,
  asp_error_level_one_read_flat_failed                  = 0x2104,
  asp_error_level_one_invalid_flat_filetype             = 0x2105,
  asp_error_level_one_invalid_bad_px_frame              = 0x2106,
  asp_error_level_one_read_bad_px_failed                = 0x2107,
  asp_error_level_one_invalid_bad_px_filetype           = 0x2108,
  asp_error_level_one_read_input_image_failed           = 0x2109,
  asp_error_level_one_input_image_wrong_filetype        = 0x2110,
  asp_error_level_one_open_output_file_failed           = 0x2111,
  asp_error_level_one_calibration_failed                = 0x2112,
  asp_error_level_one_write_output_failed               = 0x2113,

  asp_error_detect_open_image_failure                   = 0x2200,
  asp_error_detect_read_image_failure                   = 0x2201,
  asp_error_detect_write_image_failure                  = 0x2202,
  asp_error_detect_blurring_failure                     = 0x2203,
  asp_error_detect_image_subtract_failed                = 0x2204,

  asp_error_crop_open_image_failure                     = 0x2400,
  asp_error_crop_read_image_failure                     = 0x2401,
  asp_error_crop_failure_to_crop                        = 0x2402,
  asp_error_crop_write_image_failure                    = 0x2403,
  asp_error_crop_dyanmic_range                          = 0x2404,
  asp_error_crop_search_failure                         = 0x2405,
  asp_error_crop_height_width_bounds                    = 0x2406,

  asp_error_imgstat_open_image_failure                  = 0x2500,
  asp_error_imgstat_read_image_failure                  = 0x2501,
  asp_error_imgstat_integrity_check_failure             = 0x2502,
  asp_error_imgstat_comparison_failure                  = 0x2503,

  asp_error_dynamic_range_row                           = 0x2600,
  asp_error_dynamic_range_col                           = 0x2601,
  asp_error_dynamic_range_null                          = 0x2602,

  asp_error_open_image_failure                          = 0x2700,

  asp_error_buffer_unknown                              = 0x2800,
  asp_error_buffer_in_use                               = 0x2801,

  asp_error_image2d_bad_mask_parms                      = 0x2901,

  asp_error_os_file_object                              = 0x5500,
  asp_error_os_file_write                               = 0x5501,

  asp_error_crc_failed                                  = 0x5600,

  asp_error_end                                         = 0xffff

} asp_error_code_t;


typedef enum
{
  asp_log_none                                          = 0x0000,
  asp_log_debug                                         = 0x0001,
  asp_log_params_used                                   = 0x0002,

  asp_log_l1                                            = 0x1500,

  asp_log_ref_stars_found                               = 0x1600,
  asp_log_ref_star_coords                               = 0x1601,

  asp_log_search_crop_brightest_pixel                   = 0x1700,
  
  asp_log_coadd_nextimage                               = 0x1800,

  asp_log_search_top_left                               = 0x1900,

  asp_neascout_science_component_constructed            = 0x2000,
  asp_neascout_science_component_destructed             = 0x2001,
  asp_named_neascout_science_component_constructed      = 0x2002,

  asp_log_read_image                                    = 0x2100,

  asp_detect_alignment_search_radius                    = 0x2200,

  asp_log_command_status                                = 0x2300,

  asp_log_compression_chosen                            = 0x2400,

  asp_log_imgstat_data                                  = 0x2500,
  asp_log_imgstat_similarity                            = 0x2501,

  asp_log_coadd_active_image                            = 0x2600, 
  asp_log_coadd_box_exceeded                            = 0x2601,

  asp_log_detect_blurring_status                        = 0x2700,
  asp_log_detect_brightest_star_search                  = 0x2701,

  asp_log_icer_bit_depth                                = 0x2800, 
  asp_log_icer_remapped_bits                            = 0x2801,
  asp_log_icer_original_image_size                      = 0x2802, 
  asp_log_icer_compression_status_word                  = 0x2803, 
  asp_log_icer_find_bytes_needed_info                   = 0x2804,
  asp_log_icer_segment_bits                             = 0x2805,
  asp_log_icer_total_compressed_bytes                   = 0x2806,
  asp_log_icer_segment_bytes                            = 0x2807,       
  asp_log_icer_number_segments                          = 0x2808,
  asp_log_icer_byte_quota                               = 0x2809,
  asp_log_icer_decompression_status_word                = 0x2810,
  asp_log_icer_decompression_image_size                 = 0x2811,
  asp_log_icer_decompression_info                       = 0x2812,
  asp_log_icer_offset                                   = 0x2813,

  asp_log_dynamic_range                                 = 0x2900,
  asp_log_dynamic_range_exceeded                        = 0x2901,

  asp_log_buffer_allocate                               = 0x3000,
  asp_log_buffer_return                                 = 0x3001,

  asp_log_interp_edge                                   = 0x3100,

  asp_log_end                                           = 0xffff

} asp_log_code_t;



void 
asp_error(asp_error_code_t errorCode, 
          const char* logMessage);

void 
asp_log(asp_log_code_t logCode, 
        const char* logMessage);

void 
asp_imgstat(const char* logMessage);

#endif//__ASE__science__algorithms__asp__asp_error_h__
