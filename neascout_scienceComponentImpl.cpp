// ====================================================================== 
// \title  neascout_scienceImpl.cpp
// \author Jack Lightholder
// \brief  cpp file for neascout_science component implementation class
//
// \copyright
// Copyright 2009-2015, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged. Any commercial use must be negotiated with the Office
// of Technology Transfer at the California Institute of Technology.
// 
// This software may be subject to U.S. export control laws and
// regulations.  By accepting this document, the user agrees to comply
// with all U.S. export laws and regulations.  User has the
// responsibility to obtain export licenses, or other export authority
// as may be required before exporting such information to foreign
// countries or providing access to foreign persons.
// ====================================================================== 


#include <LfNeasc/Neasc/neascout_science/neascout_scienceComponentImpl.hpp>
#include <Os/Log.hpp>
#include <Os/FileSystem.hpp>

namespace Neasc {

  // ----------------------------------------------------------------------
  // Construction, initialization, and destruction 
  // ----------------------------------------------------------------------

  neascout_scienceComponentImpl ::
#if FW_OBJECT_NAMES == 1
    neascout_scienceComponentImpl(
        const char *const compName
    ) :
      neascout_scienceComponentBase(compName)
#else
    neascout_scienceImpl(void)
#endif
  {
    reset_error_buffer();
    reset_log_buffer();
    initialize_data_buffers();
  }

  void neascout_scienceComponentImpl ::
    init(
        const NATIVE_INT_TYPE queueDepth,
        const NATIVE_INT_TYPE instance
    ) 
  {
    neascout_scienceComponentBase::init(queueDepth, instance);
  }

  neascout_scienceComponentImpl ::
    ~neascout_scienceComponentImpl(void)
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void neascout_scienceComponentImpl ::
    PingRecv_handler(
        const NATIVE_INT_TYPE portNum,
        U32 key
    )
  {
      if (this-> isConnected_PingResponse_OutputPort(0)){
          this->PingResponse_out(0, key);
      }
  }

  // ----------------------------------------------------------------------
  // Command handler implementations 
  // ----------------------------------------------------------------------

  void neascout_scienceComponentImpl ::
    NEAS_SCIENCE_CMD_L1_cmdHandler(
        const FwOpcodeType opCode,
        const U32 cmdSeq,
        const Fw::CmdStringArg& rawFrame,
        const Fw::CmdStringArg& outFrame,
        const Fw::CmdStringArg& ffFrame1,
        const Fw::CmdStringArg& ffFrame2,
        const Fw::CmdStringArg& badFrame,
        const Fw::CmdStringArg& darkFrame,
        U32 fixPt,
        U32 unpack_only,
        I16 temperature
    )
  {
    /*
     * Image calibration command. 
     *
     * @param opCode flight software generated parameter
     * @param cmdSeq flight software generated parameter
     * @param rawFrame file system path to the raw needing calibration
     * @param outFrame file system path to the frame to be written. Cannot already exist.
     * @param ffFrame1 file system path to the first flat field calibration file (/sci/m_array.nsi notionally)
     * @param ffFrame2 file system path to the second flat field calibration file (/sci/b_array.nsi notionally)
     * @param badFrame file system path to the bad pixel mask (/sci/scmwl.nsi | /sci/lcmwl.nsi | /sci/empty.nsi)
     * @param darkFrame file system path to the dark current image to be used in calibration
     * @param fixPt scaling factor to be applied to resulting calibrated image
     * @param unpack_only flag to specify 12->16 bit unpacking only, no calibration. 0 for calibration
     * @param temperature if -99 use the image header temperature. Otherwise, use the value specified for flat field 
     *
     */
    char msg[ASP_MAX_LOGSTRING];
    asp_image2d_t *darkimg = NULL;
    asp_image2d_t *badpximg = NULL;
    asp_image2d_t *flatimg1 = NULL;
    asp_image2d_t *flatimg2 = NULL;
    asp_image2d_t *inimg = NULL;
    int status;
    int darkAuto = 0;
    double rcc = 1.0;
    double refTemp = 0.0;
    Fw::LogStringArg error_code;
    LfNeasc::crc_stat_t crc_stat;
    char autoCheck[12];
    (void)strncpy(autoCheck, "AUTO", sizeof(autoCheck));
    char newDark[MAX_FILE_LENGTH];

    // Initialize buffers for EVRs and FSW data buffer tracking
    reset_error_buffer();
    reset_log_buffer();
    initialize_data_buffers();

    // Read raw image
    if (asp_image2d_read(&inimg, rawFrame, M_IMAGE_BUFF1, M_IMAGE_BUFF6) == ASP_FAILURE) {
        clean_exit(opCode, cmdSeq, ASP_FAILURE);
        return;
    }

    (void)snprintf(msg, ASP_MAX_LOGSTRING, "Read a NSSI image: %s", rawFrame.toChar());
    asp_log(asp_log_read_image, msg);
    report_log_events();

    // Allow 3 seconds of sleep for memory scrubbing after initial data reading
    (void)Os::Task::delay(3000);

    // if unpack_only set to 0, apply calibration
    if (unpack_only == 0){

        // If dark field is set to "AUTO" - use raw image temperature to interpolate between two closest dark fields
        darkAuto = strncmp(darkFrame.toChar(), autoCheck, MAX_FILE_LENGTH);
        if (darkAuto == 0) {

            if(temperature == -99){
                // Use temperature of raw image being calibrated.
                refTemp = inimg->temperature;
            }else{
                // Use temperature passed in as temperature argument.
                refTemp = (double)temperature;   
            }

            if(refTemp <= -17.5){
                (void)strncpy(newDark, "/sci/dark_n20.nsi", MAX_FILE_LENGTH);
            } else if(refTemp > -17.5 && refTemp <= -12.5) {
                (void)strncpy(newDark, "/sci/dark_n15.nsi", MAX_FILE_LENGTH);
            } else if(refTemp > -12.5 && refTemp <= -7.5) {
                (void)strncpy(newDark, "/sci/dark_n10.nsi", MAX_FILE_LENGTH);
            } else if(refTemp > -7.5 && refTemp <= -2.5) {
                (void)strncpy(newDark, "/sci/dark_n5.nsi", MAX_FILE_LENGTH);
            } else if(refTemp > -2.5 && refTemp <= 2.5) {
                (void)strncpy(newDark, "/sci/dark_0.nsi", MAX_FILE_LENGTH);
            } else if(refTemp > 2.5 && refTemp <= 7.5) {
                (void)strncpy(newDark, "/sci/dark_5.nsi", MAX_FILE_LENGTH);
            } else if(refTemp > 7.5 && refTemp <= 12.5) {
                (void)strncpy(newDark, "/sci/dark_10.nsi", MAX_FILE_LENGTH);
            } else if(refTemp > 12.5 && refTemp <= 17.5) {
                (void)strncpy(newDark, "/sci/dark_15.nsi", MAX_FILE_LENGTH);
            } else if(refTemp > 17.5 && refTemp <= 22.5) {
                (void)strncpy(newDark, "/sci/dark_20.nsi", MAX_FILE_LENGTH);
            } else if(refTemp > 22.5 && refTemp <= 27.5) {
                (void)strncpy(newDark, "/sci/dark_25.nsi", MAX_FILE_LENGTH);
            } else {
                (void)strncpy(newDark, "/sci/dark_30.nsi", MAX_FILE_LENGTH);
            }

            (void)snprintf(msg, ASP_MAX_LOGSTRING, "Using dark current: %s", newDark);
            asp_log(asp_log_read_image, msg);

            // Read dark image with temperature optimal dark current image.
            if (asp_image2d_read(&darkimg, newDark, M_IMAGE_BUFF2, M_IMAGE_BUFF7) == ASP_FAILURE) {
                clean_exit(opCode, cmdSeq, ASP_FAILURE);
                return;
            } 


        } 
        else {

            // Read dark image with given file path.
            if (asp_image2d_read(&darkimg, darkFrame, M_IMAGE_BUFF2, M_IMAGE_BUFF7) == ASP_FAILURE) {
                clean_exit(opCode, cmdSeq, ASP_FAILURE);
                return;
            }       
        }

        // Read flat field image with given full path.
        if (asp_image2d_read(&flatimg1, ffFrame1, M_IMAGE_BUFF3, M_IMAGE_BUFF_NULL) == ASP_FAILURE) {
            clean_exit(opCode, cmdSeq, ASP_FAILURE);
            return;
        }

        // Allow 3 seconds of sleep for memory scrubbing after reading in calibration files
        (void)Os::Task::delay(3000);

        // Read flat field image with given full path.
        if (asp_image2d_read(&flatimg2, ffFrame2, M_IMAGE_BUFF4, M_IMAGE_BUFF_NULL) == ASP_FAILURE) {
            clean_exit(opCode, cmdSeq, ASP_FAILURE);
            return;
        }

        // Read bad pixel image.
        if (asp_image2d_read(&badpximg, badFrame, M_IMAGE_BUFF5, M_IMAGE_BUFF_NULL) == ASP_FAILURE) {
            clean_exit(opCode, cmdSeq, ASP_FAILURE);
            return;
        }

        // Allow 3 seconds of sleep for memory scrubbing after reading in calibration files
        (void)Os::Task::delay(3000);

        // calibration
        if(asp_calib(inimg, flatimg1, flatimg2, darkimg, badpximg, rcc, fixPt, temperature) != ASP_SUCCESS) {
            clean_exit(opCode, cmdSeq, ASP_FAILURE);
            return;
        }

        // Save calibrated image with fixed point scaled at end of calibration routine
        inimg->fixedPoint = (uint16_t)fixPt;

    }

    // Allow 3 seconds of sleep for memory scrubbing after reading in calibration files
    (void)Os::Task::delay(3000);

    // write output image
    status = asp_image2d_write(inimg, outFrame);
    if (status == ASP_FAILURE) {
        asp_error(asp_error_level_one_write_output_failed, "Failed to write output image");
        clean_exit(opCode, cmdSeq, ASP_FAILURE);
        return;
    }

    crc_stat = LfNeasc::create_checksum_file(outFrame.toChar());
    if (crc_stat != LfNeasc::PASSED_FILE_CRC_WRITE) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "Failed to write CRC: %d", crc_stat);
        asp_error(asp_error_crc_failed, msg);
        clean_exit(opCode, cmdSeq, ASP_FAILURE);
        return;
    }

    // Tell FSW we're done, clean up memory buffers and send lingering EVRs
    report_log_events();
    report_error_events();
    this->log_ACTIVITY_LO_L1Complete();
    clean_exit(opCode, cmdSeq, ASP_SUCCESS);
    return;
  }

  void neascout_scienceComponentImpl ::
    NEAS_SCIENCE_CMD_COADD_cmdHandler(
        const FwOpcodeType opCode,
        const U32 cmdSeq,
        I16 nFrames,
        const Fw::CmdStringArg& baseFrame,
        const Fw::CmdStringArg& outFrame,
        U32 numStars,
        U32 searchRadius,
        F64 scaling,
        U32 fixPt,
        U32 calcShift
    )
  {
    /*
     * Image coaddition command. 
     *
     * @param opCode flight software generated parameter
     * @param cmdSeq flight software generated parameter
     * @param nFrames number of frames to coadd.  Must be >= 3
     * @param baseFrame file system path to the first frame to be coadded.  Subsequent frames must be n+1, n+2, n+nFrames
     * @param outFrame file system path to the frame to be written. Cannot already exist.
     * @param numStars number of stars to use for shift reference calculation
     * @param searchRadius radius around each star to use in normalized cross correlation calculation
     * @param scaling Scaling value, to be applied to image 1, to adjust for know DN scale differences.
     * @param fixPt new fixed point to apply to resulting coadded image.  Input images can have different fixed point values
     * @param calcShift flag to calculate shift between images. Calculate shift if 1, otherwise, do not calculate shift
     *
     */
    asp_image2d_t *inimg = NULL;
    asp_costack_t *stack = NULL;
    asp_bbox_t* bbox1 = NULL;
    asp_bbox_t* bbox2 = NULL;
    int i;
    int j;
    int x;
    int y;
    int rowSize;
    int colSize;
    int bbox_diam;
    char newname[ASP_MAX_LOGSTRING];
    char msg[ASP_MAX_LOGSTRING];
    Fw::LogStringArg error_code;
    asp_pixel_t pixel;
    LfNeasc::crc_stat_t crc_stat;

    // Initialize buffers for EVRs and FSW data buffer tracking
    reset_error_buffer();
    reset_log_buffer();
    initialize_data_buffers();

    // Confirm number of reference stars < 25
    if(numStars > ASP_MAX_STARS){
        asp_error(asp_error_coadd_excessive_stars, "Max 25 stars for coaddition");
        clean_exit(opCode, cmdSeq, ASP_FAILURE);
        return;
    }

    // Confirm number of frames < 25
    if(nFrames > ASP_MAX_COADD_FRAMES){
        asp_error(asp_error_coadd_excessive_frames, "Max 25 frames for coaddition");
        clean_exit(opCode, cmdSeq, ASP_FAILURE);
        return;
    }

    // Need at least 3 frames to perform temporal median
    if (nFrames < 3) {
        asp_error(asp_error_coadd_insufficient_frames, "Minimum three frames required");
        clean_exit(opCode, cmdSeq, ASP_FAILURE);
        return;
    }

    // Read the base frame
    if (asp_image2d_read(&inimg, baseFrame, M_IMAGE_BUFF3, M_IMAGE_BUFF_NULL) == ASP_FAILURE) {
        clean_exit(opCode, cmdSeq, ASP_FAILURE);
        return;
    }

    // Get base row/col and give back first FSW buffer.  Taken back during donation.
    rowSize = inimg->rows;
    colSize = inimg->cols;
    asp_free_buffer(M_IMAGE_BUFF3);

    // Allocate costack memory space
    stack = asp_costack_alloc(rowSize, colSize, numStars, searchRadius);
    if (stack == NULL) {
        clean_exit(opCode, cmdSeq, ASP_FAILURE);
        return;
    }

    bbox_diam = 2 * stack->radius + 1;

    // Allocate memory for bounding box 1
    bbox1 = asp_bbox_init(0, 0, bbox_diam, bbox_diam, M_IMAGE_BUFF6);
    if(bbox1 == NULL){
        asp_error(asp_error_bbox_malloc_failure, "failed to allocate COADD bbox1");
        clean_exit(opCode, cmdSeq, ASP_FAILURE);
        return;
    } 

    // Allocate memory for bounding box 2
    bbox2 = asp_bbox_init(0, 0, bbox_diam, bbox_diam, M_IMAGE_BUFF7);
    if(bbox2 == NULL){
        asp_error(asp_error_bbox_malloc_failure, "failed to allocate COADD bbox2");
        clean_exit(opCode, cmdSeq, ASP_FAILURE);
        return;
    }

    // Allow 3 seconds of sleep for memory scrubbing after initial memory allocations
    (void)Os::Task::delay(3000);

    // For each frame, incremement filename and add to increasing temporal stack
    for (i = 0; i < nFrames; i++) {

        if (coadd_increment_filename(baseFrame.toChar(), newname, i) != ASP_SUCCESS) {
            asp_error(asp_error_coadd_filename_increment_failed, "Failed to increment filename");
            clean_exit(opCode, cmdSeq, ASP_FAILURE);
            return;
        }

        (void)snprintf(msg, ASP_MAX_LOGSTRING, "Processing: %s", newname);
        asp_log(asp_log_coadd_active_image, msg);
        report_log_events();

        // Add frame to growing coadded image base
        if (asp_donate(newname, stack, bbox1, bbox2, scaling, fixPt, calcShift) == ASP_FAILURE) {
            clean_exit(opCode, cmdSeq, ASP_FAILURE);
            return;
        }

        // Allow 15 seconds of memory scrubbing for each file iteration
        (void)Os::Task::delay(15000);

    }
  
    // Report reference star locations through EVR
    for (j = 0; j < stack->numrefs; j++) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "Ref Star:(r:%i,c:%i)",stack->ref_coords_y[j], stack->ref_coords_x[j]);
        asp_log(asp_log_ref_star_coords, msg);
        report_log_events();
    } 

    stack->base->overflow = 0;
    stack->base->underflow = 0;

    // Assume anything at min/max is overflow/underflow.  Not done in asp_donate, so no double counting
    for(x=0; x < stack->base->cols; x++)
    {
      for(y=0; y < stack->base->rows; y++)
      {
        pixel = *asp_image2d_get_pixel(stack->base,x,y);  

        if(pixel == ASP_PIXEL_MAX){
            stack->base->overflow += 1;
        }

        if(pixel == ASP_PIXEL_MIN){
            stack->base->underflow += 1;
        }

      }
    }

    // Allow 3 seconds of memory scrubbing before writing output file
    (void)Os::Task::delay(3000);

    // Write resulting image
    if (asp_image2d_write(stack->base, outFrame) == ASP_FAILURE) {
        asp_error(asp_error_coadd_write_output_failure, "Write coadded image failed");
        clean_exit(opCode, cmdSeq, ASP_FAILURE);
        return;
    }

    crc_stat = LfNeasc::create_checksum_file(outFrame.toChar());
    if (crc_stat != LfNeasc::PASSED_FILE_CRC_WRITE) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "Failed to write CRC: %d", crc_stat);
        asp_error(asp_error_crc_failed, msg);
        clean_exit(opCode, cmdSeq, ASP_FAILURE);
        return;
    }

    // Tell FSW we're done, clean up memory buffers and send lingering EVRs
    report_log_events();
    report_error_events();
    this->log_ACTIVITY_LO_CoadditionComplete();
    clean_exit(opCode, cmdSeq, ASP_SUCCESS);
    return;
  }

  void neascout_scienceComponentImpl ::
    NEAS_SCIENCE_CMD_CROP_cmdHandler(
        const FwOpcodeType opCode,
        const U32 cmdSeq,
        const Fw::CmdStringArg& inFrame,
        const Fw::CmdStringArg& outFrame,
        I16 cropLeft,
        I16 cropTop,
        I16 cropWidth,
        I16 cropHeight,
        I16 search,
        F32 compressionRate,
        I16 downsampleFactor,
        I16 min_loss,
        I16 filtype,
        I16 ndecomps,
        I16 nseg
    )
  {
    /*
     * Image cropping and compression command. 
     *
     * @param opCode flight software generated parameter
     * @param cmdSeq flight software generated parameter
     * @param inFrame file system path to the image which needs cropping/compression
     * @param outFrame file system path to the frame to be written. Cannot already exist.
     * @param cropLeft first column to be included in crop. 0 based.
     * @param cropTop first row to be included in crop. 0 based.
     * @param cropWidth number of columns to be included in crop.
     * @param cropHeight number of rows to be included in crop.
     * @param search radius to search around the brightest pixel in the image.
     *           searches within the space defined by cropLeft, cropTop, cropWidth and cropHeight
     * @param compressionRate ICER compression rate for image compression. Rates defined below.
     *                        <= 0:    No compression, write raw format
     *                        0 - 16:  Apply lossy compression
     *                        >= 16:   Apply lossless compression
     * @param downsampling factor.  If > 0, overrides cropping arguments, 0 for no downsampling or if cropping is desired.
     * @param min_loss minimum loss compression rate. See ICER guide for more information.
     * @param filtype ICER wavelet controller.  See ICER guide for more information.
     * @param ndecomps ICER decompositions controller.  See ICER guide for more information.
     * @param nseg ICER segments per image.  See ICER guide for more information.
     *
     */
    int status = ASP_FAILURE;
    asp_image2d_t *inimg = NULL;
    asp_image2d_t *outimg = NULL;
    asp_image2d_t *counts = NULL;
    char msg[ASP_MAX_LOGSTRING];
    Fw::LogStringArg error_code;
    int height;
    int width;
    int new_height;
    int new_width;
    int bottom = cropTop + cropHeight - 1;  // inclusive, zero-indexed
    int right = cropLeft + cropWidth - 1;
    int top = cropTop;
    int left = cropLeft;
    LfNeasc::crc_stat_t crc_stat;

    // Initialize buffers for EVRs and FSW data buffer tracking
    reset_error_buffer();
    reset_log_buffer();
    initialize_data_buffers();

    // Read in the raw image
    if (asp_image2d_read(&inimg, inFrame, M_IMAGE_BUFF1, M_IMAGE_BUFF_NULL) == ASP_FAILURE) {
        clean_exit(opCode, cmdSeq, ASP_FAILURE);
        return;
    }

    (void)snprintf(msg, ASP_MAX_LOGSTRING, "Read image: %s", inFrame.toChar());
    asp_log(asp_log_read_image, msg);

    // If downsampling is requested, cropping will not occur.
    if(downsampleFactor > 0){

        /* Guarded above against divide by zero */
        new_height = (int)ceil((double)inimg->rows / (double)ASP_NONZERO(downsampleFactor));
        new_width = (int)ceil((double)inimg->cols / (double)ASP_NONZERO(downsampleFactor));

        // Allocate memory to store output image
        outimg =  asp_image2d_alloc(new_height, new_width, M_IMAGE_BUFF2);
        if(outimg == NULL) {
            asp_error(asp_error_image2d_imageAllocatorFailed, "downsample outimg memory allocation failed" );
            clean_exit(opCode, cmdSeq, ASP_FAILURE);
            return;
        }

        // Allow 3 seconds of sleep for memory scrubbing after downsample memory setup
        (void)Os::Task::delay(3000);

        // Allocate memory to store output image
        counts =  asp_image2d_alloc(new_height, new_width, M_IMAGE_BUFF7);
        if(counts == NULL) {
            asp_error(asp_error_image2d_imageAllocatorFailed, "downsample counts memory allocation failed" );
            clean_exit(opCode, cmdSeq, ASP_FAILURE);
            return;
        }

        // Downsample image
        if (asp_subsample_image(inimg, &outimg, counts, downsampleFactor) != ASP_SUCCESS) {
            asp_error(asp_error_crop_failure_to_crop, "Downsample image failed");
            clean_exit(opCode, cmdSeq, ASP_FAILURE);
            return;
        }

        // Allow 3 seconds of sleep for memory scrubbing after downsampling
        (void)Os::Task::delay(3000);

        outimg->rows = new_height;
        outimg->cols = new_width;
        outimg->otop = inimg->otop;
        outimg->oleft = inimg->oleft;
        outimg->format = ASP_PRODTYPE_DOWNSAMPLE;


    }else{

        // Search for the brightest pixel in the image
        if (search > 0) {

            asp_log(asp_log_detect_brightest_star_search, "Searching for brightest pixel in image");

            // Center the subwindow on the brightest pixel in the subframe
            int box[4] = {0,0,0,0};
            if(asp_search_crop_image(inimg, box, search, search, top,
                    bottom, left, right) != ASP_SUCCESS) {
                asp_error(asp_error_crop_search_failure, "Failed to find brightest pixel");
                clean_exit(opCode, cmdSeq, ASP_FAILURE);
                return;
            }

            // Allow 3 seconds of sleep for memory scrubbing after star searching
            (void)Os::Task::delay(3000);

            top = box[0];
            bottom = box[1];
            left = box[2];
            right = box[3];
        } 

        height = bottom - top + 1;
        width = right - left + 1;

        if((height <= 0) || (width <= 0)){
            (void)snprintf(msg, ASP_MAX_LOGSTRING, "Height (%d) width (%d) <= 0", height, width);
            asp_error(asp_error_crop_height_width_bounds,msg);
            clean_exit(opCode, cmdSeq, ASP_FAILURE);
            return;
        }

        // Allocate memory to store output image
        outimg =  asp_image2d_alloc(height, width, M_IMAGE_BUFF2);
        if(outimg == NULL) {
            asp_error(asp_error_image2d_imageAllocatorFailed, "crop outimg memory allocation failed" );
            clean_exit(opCode, cmdSeq, ASP_FAILURE);
            return;
        }

        // Crop image
        if (asp_crop_image(inimg, &outimg, top, bottom, left, right) != ASP_SUCCESS) {
            asp_error(asp_error_crop_failure_to_crop, "Crop image failed");
            clean_exit(opCode, cmdSeq, ASP_FAILURE);
            return;
        }

        // Allow 3 seconds of sleep for memory scrubbing after cropping
        (void)Os::Task::delay(3000);

    }

    // Now write the file.  rate controls format.
    //    <= 0:    No compression, write raw format
    //  0 - 16:    Apply lossy compression
    //   >= 16:    Apply lossless compression
    if (compressionRate <= 0.0) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "Input rate: %f, skipping compression", compressionRate);
        status = asp_image2d_write(outimg, outFrame);
    }
    else if (compressionRate < 16.0) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "Input rate: %f, lossy compression", compressionRate);
        status = icerwrite_lossy(outimg, outFrame, compressionRate, min_loss, filtype, ndecomps, nseg);
    } else {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "Input rate: %f, lossless compression", compressionRate);
        status = icerwrite(outimg, outFrame, min_loss, filtype, ndecomps, nseg);
    }

    asp_log(asp_log_compression_chosen, msg);

    if (status == ASP_FAILURE) {
        asp_error(asp_error_crop_write_image_failure, "Write output image failed");
        clean_exit(opCode, cmdSeq, ASP_FAILURE);
        return;
    }

    crc_stat = LfNeasc::create_checksum_file(outFrame.toChar());
    if (crc_stat != LfNeasc::PASSED_FILE_CRC_WRITE) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "Failed to write CRC: %d", crc_stat);
        asp_error(asp_error_crc_failed, msg);
        clean_exit(opCode, cmdSeq, ASP_FAILURE);
        return;
    }

    // Tell FSW we're done, clean up memory buffers and send lingering EVRs
    report_log_events();
    report_error_events();
    this->log_ACTIVITY_LO_CropCompressComplete();
    clean_exit(opCode, cmdSeq, ASP_SUCCESS);
    return;
  }

  void neascout_scienceComponentImpl ::
    NEAS_SCIENCE_CMD_DETECT_cmdHandler(
        const FwOpcodeType opCode,
        const U32 cmdSeq,
        const Fw::CmdStringArg& inFrameA,
        const Fw::CmdStringArg& inFrameB,
        const Fw::CmdStringArg& outFrame,
        F32 sigma,
        I16 aligned,
        U32 searchRadius,
        U32 numStars,
        F64 scaling,
        I32 gaussKernelWidth,
        F64 gaussKernelScale,
        U32 fixPt
    )
  {
    /*
     * Image blurring and subtraction command.
     *
     * @param opCode flight software generated parameter
     * @param cmdSeq flight software generated parameter
     * @param inFrameA file system path to the image which will have inFrameB subtracted from it.
     * @param inFrameB file system path to the image which will be subtracted from inFrameA.
     * @param outFrame file system path to the frame to be written. Cannot already exist.
     * @param sigma sigma for Gaussian kernel creation. Blurring applied if > 0 (blurring argument)
     * @param aligned flag to factor in shift calculation when subtracting images. Apply shift calculation if aligned == 1 (shift argument)
     * @param searchRadius radius around each star to use in normalized cross correlation calculation (shift argument)
     * @param numStars number of stars to use for shift reference calculation (shift argument)
     * @param scaling Scaling value, to be applied to image 1, to adjust for know DN scale differences (shift argument)
     * @param gaussKernelWidth width to be applied when generating Gaussian kernel (blurring argument)
     * @param gaussKernelScale scale to be applied when generating Gaussian kernel (blurring argument)
     * @param fixPt fixed point scaler to be applied to resulting image
     *
     * This command provides the capability to do optional blurring and image subtraction, with shift optionally being factored in.  
     *    Note that all command arguments must be supplied every time, but shift arguments will be ignored if aligned is not set to 1
     *    and blurring arguments will be ignored if sigma is not larger than 0.
     *
     */
    int status = ASP_FAILURE;
    asp_image2d_t *inimgA = NULL;
    asp_image2d_t *inimgB = NULL;
    asp_image2d_t *outimg = NULL;
    asp_image2d_t *scratch = NULL;
    double *kernel = NULL;
    asp_bbox_t *bbox1 = NULL;
    asp_bbox_t *bbox2 = NULL;
    int shift[2] = {0,0};
    int j;
    int bbox_diam;
    int new_height;
    int new_width;
    asp_costack_t *stack = NULL;
    int starsfound = 0;
    char msg[ASP_MAX_LOGSTRING];
    Fw::LogStringArg error_code;
    LfNeasc::crc_stat_t crc_stat;

    // Initialize buffers for EVRs and FSW data buffer tracking
    reset_error_buffer();
    reset_log_buffer();
    initialize_data_buffers();

    // If sigma > 0, apply Gaussian blur to image
    if(sigma > 0.0){

        (void)snprintf(msg, ASP_MAX_LOGSTRING, "Blurring requested, sigma value of %f", sigma);
        asp_log(asp_log_detect_blurring_status, msg);

        // Using sizeof(double)*gaussKernelWidth*2+1
        kernel = (double*)asp_malloc(BUFFER_SIZE, M_IMAGE_BUFF1);
        if (kernel == NULL) {
            asp_error(asp_error_image2d_imageAllocatorFailed, "detect kernel memory allocation failed" );
            clean_exit(opCode, cmdSeq, ASP_FAILURE);
            return;
        }

        // we use outimg as a scratch space until subtraction
        if (asp_image2d_read(&outimg, inFrameA, M_IMAGE_BUFF2, M_IMAGE_BUFF_NULL) == ASP_FAILURE) {
            clean_exit(opCode, cmdSeq, ASP_FAILURE);
            return;
        }

        // Allow 3 seconds of sleep for memory scrubbing after initial data reading
        (void)Os::Task::delay(3000);

        (void)snprintf(msg, ASP_MAX_LOGSTRING, "Read a NSSI image: %s", inFrameA.toChar());
        asp_log(asp_log_read_image, msg);

        inimgA = asp_image2d_alloc(outimg->rows, outimg->cols, M_IMAGE_BUFF5);
        if (inimgA == NULL) {
            asp_error(asp_error_image2d_imageAllocatorFailed, "detect inimgA memory allocation failed");
            clean_exit(opCode, cmdSeq, ASP_FAILURE);
            return;
        }

        scratch = asp_image2d_alloc(outimg->rows, outimg->cols, M_IMAGE_BUFF4);
        if (scratch == NULL) {
            asp_error(asp_error_image2d_imageAllocatorFailed, "detect scratch memory allocation failed");
            clean_exit(opCode, cmdSeq, ASP_FAILURE);
            return;
        }

        // Add Gaussian smoothing to first image
        status = asp_smooth_image(outimg, &inimgA, scratch, gaussKernelWidth, sigma, gaussKernelScale, kernel);
        if (status == ASP_FAILURE) {             
           asp_error(asp_error_detect_blurring_failure, "Blurring image A failed");
           clean_exit(opCode, cmdSeq, ASP_FAILURE);
           return;
        }

        // Allow 2 minutes of sleep for memory scrubbing after Gaussian blurring
        (void)Os::Task::delay(120000);

        FW_ASSERT(outimg != NULL);
        FW_ASSERT(inimgA != NULL);

        // Copy header to blurred image A
        asp_image2d_cptelem(outimg, inimgA);
        
        if (asp_image2d_read(&outimg, inFrameB, M_IMAGE_BUFF3, M_IMAGE_BUFF_NULL) == ASP_FAILURE) {
            clean_exit(opCode, cmdSeq, ASP_FAILURE);
            return;
        }

        (void)snprintf(msg, ASP_MAX_LOGSTRING, "Read image: %s", inFrameB.toChar());
        asp_log(asp_log_read_image, msg);
        report_log_events();

        inimgB = asp_image2d_alloc(outimg->rows, outimg->cols, M_IMAGE_BUFF6);
        if (inimgB == NULL) {
            asp_error(asp_error_image2d_imageAllocatorFailed, "detect inimgB memory allocation failed");
            clean_exit(opCode, cmdSeq, ASP_FAILURE);
            return;
        }

        // Add Gaussian smoothing to second image
        status = asp_smooth_image(outimg, &inimgB, scratch, gaussKernelWidth, sigma, gaussKernelScale, kernel);
        if (status == ASP_FAILURE) {
            asp_error(asp_error_detect_blurring_failure, "Blurring image B failed");
            clean_exit(opCode, cmdSeq, ASP_FAILURE);
            return;
        }

        // Allow 2 minutes of sleep for memory scrubbing after Gaussian blurring
        (void)Os::Task::delay(120000);

        FW_ASSERT(outimg != NULL);
        FW_ASSERT(inimgB > NULL);

        // Copy header to blurred image B
        asp_image2d_cptelem(outimg, inimgB);

    }
    else{

        // No blurring requested.  Simply read in images.
        asp_log(asp_log_detect_blurring_status, "No blurring");

        if (asp_image2d_read(&inimgA, inFrameA, M_IMAGE_BUFF5, M_IMAGE_BUFF_NULL) == ASP_FAILURE) {
            clean_exit(opCode, cmdSeq, ASP_FAILURE);
            return;
        }

        (void)snprintf(msg, ASP_MAX_LOGSTRING, "Read image: %s", inFrameA.toChar());
        asp_log(asp_log_read_image, msg);

        if (asp_image2d_read(&inimgB, inFrameB, M_IMAGE_BUFF6, M_IMAGE_BUFF_NULL) == ASP_FAILURE) {
            clean_exit(opCode, cmdSeq, ASP_FAILURE);
            return;
        }

        (void)snprintf(msg, ASP_MAX_LOGSTRING, "Read image: %s", inFrameB.toChar());
        asp_log(asp_log_read_image, msg);

        // Allow 3 seconds of sleep for memory scrubbing after initial data reading
        (void)Os::Task::delay(3000);

    }

    // We now have imgaes in inimgA and inimgB.  Blurred or unblurred depending on conditional sigma
    if(aligned == 1){

        // Confirm number of stars less than 25.
        if(numStars > ASP_MAX_STARS){
            asp_error(asp_error_coadd_excessive_stars, "Max 25 stars for reference alignment");
            clean_exit(opCode, cmdSeq, ASP_FAILURE);
            return;
        }

        stack = asp_costack_alloc(inimgA->rows, inimgA->cols, numStars, searchRadius);
        if (stack == NULL) {
            clean_exit(opCode, cmdSeq, ASP_FAILURE);
            return;
        }

        stack->offx1 = 0;
        stack->offy1 = 0;

        // Calculate reference stars
        if(asp_ref_stars(stack, inimgA, &starsfound) != ASP_SUCCESS) {
            asp_error(asp_error_coadd_reference_stars, "Finding reference stars failed");
            clean_exit(opCode, cmdSeq, ASP_FAILURE);
            return;
        }

        // Allow 2 seconds of sleep for memory scrubbing after reference star collection
        (void)Os::Task::delay(2000);

        // Log coordinates of reference stars in base image
        for (j = 0; j < stack->numrefs; j++) {
            (void)snprintf(msg, ASP_MAX_LOGSTRING, "Ref Star:(r:%i,c:%i)",stack->ref_coords_y[j], stack->ref_coords_x[j]);
            asp_log(asp_log_ref_star_coords, msg);
            report_log_events();
        }

        bbox_diam = 2 * stack->radius + 1;

        // Initialize memory for bounding box 1
        bbox1 = asp_bbox_init(0, 0, bbox_diam, bbox_diam, M_IMAGE_BUFF3);
        if(bbox1 == NULL){
           asp_error(asp_error_bbox_malloc_failure, "failed to allocate DETECT bbox1");
           clean_exit(opCode, cmdSeq, ASP_FAILURE);
           return;
        }

        // Initialize memory for bounding box 2
        bbox2 = asp_bbox_init(0, 0, bbox_diam, bbox_diam, M_IMAGE_BUFF4);
        if(bbox2 == NULL){
           asp_error(asp_error_bbox_malloc_failure, "failed to allocate DETECT bbox2");
           clean_exit(opCode, cmdSeq, ASP_FAILURE);
           return;
        }

        // Calculate the shift between the two images
        if(asp_find_shift_image2d(shift, inimgA, inimgB, stack, bbox1, bbox2, scaling) != ASP_SUCCESS) {
            asp_error(asp_error_coadd_shift_calculation, "detection shift calculation failed");
            clean_exit(opCode, cmdSeq, ASP_FAILURE);
            return;
        }

        // Allow 2 seconds of sleep for memory scrubbing after shift calculation
        (void)Os::Task::delay(2000);

        // Log the shift between two images
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "Aligned Image shift: c:%i r:%i", shift[0], shift[1]);
        asp_log(asp_log_coadd_nextimage, msg);

        // Calculate new image dimensions 
        new_height = (inimgA->rows < inimgB->rows)? inimgA->rows : inimgB->rows;
        new_width = (inimgA->cols < inimgB->cols)? inimgA->cols : inimgB->cols;
        outimg = asp_image2d_alloc(new_height, new_width, M_IMAGE_BUFF7);
        if (outimg == NULL) {
            asp_error(asp_error_image2d_imageAllocatorFailed, "detect outimg memory allocation failed");
            clean_exit(opCode, cmdSeq, ASP_FAILURE);
            return;
        }

        FW_ASSERT(inimgA != NULL);
        FW_ASSERT(outimg != NULL);

        // Copy telemetry to newly allocated out image
        asp_image2d_cptelem(inimgA, outimg);

        // Subtract frame B from A, using shift calculated above.
        status = asp_subtract_image_offset(inimgA, inimgB, &outimg, shift, fixPt);
        if (status == ASP_FAILURE) {
            asp_error(asp_error_detect_image_subtract_failed, "Subtract image, with offset, failed");
            clean_exit(opCode, cmdSeq, ASP_FAILURE);
            return;
        }

        // Allow 2 seconds of sleep for memory scrubbing after shifted subtraction
        (void)Os::Task::delay(2000);

    } 
    else{

        // Do not find shift.  Simply subtract image B from A.
        new_height = (inimgA->rows < inimgB->rows)? inimgA->rows : inimgB->rows;
        new_width = (inimgA->cols < inimgB->cols)? inimgA->cols : inimgB->cols;
        outimg = asp_image2d_alloc(new_height, new_width, M_IMAGE_BUFF7);
        if (outimg == NULL) {
            asp_error(asp_error_image2d_imageAllocatorFailed, "detect outimg memory allocation failed");
            clean_exit(opCode, cmdSeq, ASP_FAILURE);
            return;
        }

        FW_ASSERT(inimgA != NULL);
        FW_ASSERT(outimg != NULL);

        // Copy image A header to newly allocated out image
        asp_image2d_cptelem(inimgA, outimg);

        // Allow 2 seconds of sleep for memory scrubbing before subtraction
        (void)Os::Task::delay(2000);

        // Subtract frame B from A, with no pixel-shift
        status = asp_subtract_image(inimgA, inimgB, &outimg, fixPt);
        if (status == ASP_FAILURE) {
            asp_error(asp_error_detect_image_subtract_failed, "Subtract image, no offset, failed");
            clean_exit(opCode, cmdSeq, ASP_FAILURE);
            return;
        }
    }
    
    // Track fixed point reference value in out image
    outimg->fixedPoint = fixPt;
    outimg->format = ASP_PRODTYPE_SUBTRACT;

    // Allow 2 second of sleep for memory scrubbing before final file write
    (void)Os::Task::delay(2000);

    // Write output image
    if (asp_image2d_write(outimg, outFrame) == ASP_FAILURE) {
        asp_error(asp_error_detect_write_image_failure, "Write output image failed");
        clean_exit(opCode, cmdSeq, ASP_FAILURE);
        return;
    }

    crc_stat = LfNeasc::create_checksum_file(outFrame.toChar());
    if (crc_stat != LfNeasc::PASSED_FILE_CRC_WRITE) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "Failed to write CRC: %d", crc_stat);
        asp_error(asp_error_crc_failed, msg);
        clean_exit(opCode, cmdSeq, ASP_FAILURE);
        return;
    }

    // Tell FSW we're done, clean up memory buffers and send lingering EVRs
    report_log_events();
    report_error_events();
    this->log_ACTIVITY_LO_DetectionComplete();
    clean_exit(opCode, cmdSeq, ASP_SUCCESS);
    return;
  }

  void neascout_scienceComponentImpl ::
    NEAS_SCIENCE_CMD_IMGSTAT_cmdHandler(
        const FwOpcodeType opCode,
        const U32 cmdSeq,
        const Fw::CmdStringArg& frameA,
        const Fw::CmdStringArg& frameB,
        I32 bin0HistMax,
        I32 bin1HistMax,
        I32 bin4HistMax,
        I32 bin5HistMax
    )
  {
    /*
     * Image statistics command
     *
     * @param opCode flight software generated parameter
     * @param cmdSeq flight software generated parameter
     * @param FrameA file system path to the first image for statistics
     * @param FrameB file system path to the optimal second image for statistics.  
     *                 If provided, will also compare A & B. If not desired, provide empty string.
     * @param bin0HistMax max DN value in bin 0.
     * @param bin1HistMax max DN value in bin 1
     * @param bin4HistMax max DN value in bin 4
     * @param bin5HistMax max DN value in bin 5
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
     *
     */
    int sameHeader;
    int sameData;
    int status = ASP_FAILURE;
    asp_image2d_t *inimgA = NULL;
    asp_image2d_t *inimgB = NULL;
    char msg[ASP_MAX_LOGSTRING];
    Fw::LogStringArg error_code;

    // Initialize buffers for EVRs and FSW data buffer tracking
    reset_error_buffer();
    reset_log_buffer();
    initialize_data_buffers();

    // Read first image for which we'll calculate statistics
    if (asp_image2d_read(&inimgA, frameA, M_IMAGE_BUFF1, M_IMAGE_BUFF3) == ASP_FAILURE) {
        clean_exit(opCode, cmdSeq, ASP_FAILURE);
        return;
    }

    // Allow 2 second of sleep for memory scrubbing after file read
    (void)Os::Task::delay(2000);

    (void)snprintf(msg, ASP_MAX_LOGSTRING, "Read a NSSI image: %s", frameA.toChar());
    asp_log(asp_log_read_image, msg);

    (void)snprintf(msg, ASP_MAX_LOGSTRING, "%s details:", frameA.toChar());
    asp_log(asp_log_imgstat_data, msg);
    report_log_events();

    // Claculate image 1 statistics, send as EVRs to groun
    if(asp_image2d_check(frameA.toChar(), inimgA, bin0HistMax, bin1HistMax, bin4HistMax, bin5HistMax) == ASP_FAILURE) {
        clean_exit(opCode, cmdSeq, ASP_FAILURE);
        return;
    }
    
    // Allow 2 second of sleep for memory scrubbing after file checking
    (void)Os::Task::delay(2000);

    report_log_events();
    
    #ifdef TGT_OS_TYPE_VXWORKS
        // If length of frameB < 3, no second image was specified.  No comparison to be done, exit.
        if (strnlen(frameB.toChar(), MAX_FILE_LENGTH) < 3) {
            report_log_events();
            report_error_events();
            this->log_ACTIVITY_LO_ImgStatComplete();
            clean_exit(opCode, cmdSeq, ASP_SUCCESS);
            return;
        }
    #else
         // If length of frameB < 2, no second image was specified.  No comparison to be done, exit.
        if (strnlen(frameB.toChar(), MAX_FILE_LENGTH) < 2) {
            report_log_events();
            report_error_events();
            this->log_ACTIVITY_LO_ImgStatComplete();
            clean_exit(opCode, cmdSeq, ASP_SUCCESS);
            return;
        }
    #endif       

    // If valid image file path provided, read in second image. 
    if (asp_image2d_read(&inimgB, frameB, M_IMAGE_BUFF2, M_IMAGE_BUFF4) == ASP_FAILURE) {
        clean_exit(opCode, cmdSeq, ASP_FAILURE);
        return;
    }

    // Allow 2 second of sleep for memory scrubbing after file read
    (void)Os::Task::delay(2000);

    (void)snprintf(msg, ASP_MAX_LOGSTRING, "Read a NSSI image: %s", frameB.toChar());
    asp_log(asp_log_read_image, msg);

    (void)snprintf(msg, ASP_MAX_LOGSTRING, "%s details:", frameB.toChar());
    asp_log(asp_log_imgstat_data, msg);
    report_log_events();
    
    // Second image was specified, run statistics
    if(asp_image2d_check(frameB.toChar(), inimgB, bin0HistMax, bin1HistMax, bin4HistMax, bin5HistMax) == ASP_FAILURE) {
        clean_exit(opCode, cmdSeq, ASP_FAILURE);
        return;
    }

    // Allow 2 second of sleep for memory scrubbing after file checking
    (void)Os::Task::delay(2000);

    report_log_events();

    // compare imgA and imgB, both header and image data.  Binary results of equivalence for both
    status = asp_image2d_compare(inimgA, inimgB, &sameHeader, &sameData);
    if (status == ASP_FAILURE) {
        asp_error(asp_error_imgstat_comparison_failure, "Compare images failed");
        clean_exit(opCode, cmdSeq, ASP_FAILURE);
        return;
    }

    // Report if headers, or data, of imgA and imgB are the same
    if (sameHeader) {
        if(sameData){
            (void)snprintf(msg, ASP_MAX_LOGSTRING, "%s and %s same", frameA.toChar(), frameB.toChar());
        } else {
            (void)snprintf(msg, ASP_MAX_LOGSTRING, "%s and %s sh, dd", frameA.toChar(), frameB.toChar());
        }
    } 
    else {
        if(sameData) {
            (void)snprintf(msg, ASP_MAX_LOGSTRING, "%s and %s dh, sd", frameA.toChar(), frameB.toChar());
        } else {
            (void)snprintf(msg, ASP_MAX_LOGSTRING, "%s and %s dh, dd", frameA.toChar(), frameB.toChar());
        }
    }

    asp_log(asp_log_imgstat_similarity, msg);

    // Tell FSW we're done, clean up memory buffers and send lingering EVRs
    report_log_events();
    report_error_events();
    this->log_ACTIVITY_LO_ImgStatComplete();
    clean_exit(opCode, cmdSeq, ASP_SUCCESS);
    return;
  }

  void neascout_scienceComponentImpl ::
    NEAS_SCIENCE_CMD_MASK_UPDATE_cmdHandler(
        const FwOpcodeType opCode,
        const U32 cmdSeq,
        const Fw::CmdStringArg& inFrame,
        const Fw::CmdStringArg& outFrame,
        I16 row,
        I16 col,
        I16 value
    )
  {
    /*
     * Pixel update command.  Intended for updating bad pixels masks in flight.
     *
     * @param opCode flight software generated parameter
     * @param cmdSeq flight software generated parameter
     * @param inFrame file system path to the image to be edited
     * @param outFrame file system path to the resulting adjusted image 
     * @param row row index to be edited. 0 based.
     * @param col column index to be edited. 0 based.
     * @param value new pixel value inserted.
     *
     */
    char msg[ASP_MAX_LOGSTRING];
    asp_image2d_t *img = NULL;
    int status = ASP_FAILURE;
    int ind = 0;
    int original_value = 0;
    LfNeasc::crc_stat_t crc_stat;

    // Initialize buffers for EVRs and FSW data buffer tracking
    reset_error_buffer();
    reset_log_buffer();
    initialize_data_buffers();

    // Image index bounds checking
    if ((col < 0) ||
        (row < 0) ||
        (row >= ASP_MAX_ROWS) ||
        (col >= ASP_MAX_COLS))
    {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "bad mask parms - row:%i col:%i", row, col);
        asp_error(asp_error_image2d_bad_mask_parms, msg);
        clean_exit(opCode, cmdSeq, ASP_FAILURE);
        return;
    }

    // Read existing image
    if (asp_image2d_read(&img, inFrame, M_IMAGE_BUFF1, M_IMAGE_BUFF2) == ASP_FAILURE) {
        clean_exit(opCode, cmdSeq, ASP_FAILURE);
        return;
    } 

    // Allow 2 second of sleep for memory scrubbing after file reading
    (void)Os::Task::delay(2000);

    if ((row >= img->rows) ||
        (col >= img->cols))
    {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "bad mask parms - req: row:%i col:%i act: row:%i col:%i", row, col, img->rows, img->cols);
        asp_error(asp_error_image2d_bad_mask_parms, msg);
        clean_exit(opCode, cmdSeq, ASP_FAILURE);
        return;
    }

    // Get flat index of pixel from row/col space.
    ind = asp_image2d_get_index(img, col, row);

    // Get original pixel value for logging.
    original_value = *asp_image2d_get_pixel(img, col, row);

    // Insert new value at index.
    img->data[ind] = (asp_pixel_t)value;

    (void)snprintf(msg, ASP_MAX_LOGSTRING, "Pixel value at r:%i c:%i set from %i to %i", row, col, original_value, value);
    asp_log(asp_log_ref_star_coords, msg);

    // Allow 2 second of sleep for memory scrubbing before final file write
    (void)Os::Task::delay(2000);

    // write output
    status = asp_image2d_write(img, outFrame);
    if (status == ASP_FAILURE) {
        asp_error(asp_error_level_one_write_output_failed, "Failed to write output image");
        clean_exit(opCode, cmdSeq, ASP_FAILURE);
        return;
    }

    crc_stat = LfNeasc::create_checksum_file(outFrame.toChar());
    if (crc_stat != LfNeasc::PASSED_FILE_CRC_WRITE) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "Failed to write CRC: %d", crc_stat);
        asp_error(asp_error_crc_failed, msg);
        clean_exit(opCode, cmdSeq, ASP_FAILURE);
        return;
    }

    // Tell FSW we're done, clean up memory buffers and send lingering EVRs
    report_log_events();
    report_error_events();
    this->log_ACTIVITY_LO_MaskUpdateComplete();
    clean_exit(opCode, cmdSeq, ASP_SUCCESS);
    return;
  }


  ///////////////////////////////////////////////
  //                                           //
  //              Helper Functions             //
  //                                           //
  ///////////////////////////////////////////////

  void  neascout_scienceComponentImpl ::
    report_log_events() 
  {
    /*
     * Dump logEventBuffer to FSW EVRs.  1 per second. Reset buffer.
     *
     */
    Fw::LogStringArg logString;
    int reportIndex = 0;

    // For each log message in buffer, report as ACTIVITY_LO
    for(int i=0; i < logBufferIndex; i++){
        logString = logEventBuffer[reportIndex];   
        log_ACTIVITY_LO_asp_log(logString);
fprintf(stderr, "%s", logEventBuffer[reportIndex]);  // TODO - remove
        // Delay 1 second between EVR messages to not overflow FSW buffers
        (void)Os::Task::delay(1000);

        reportIndex += 1;
    }

    reset_log_buffer();
  }

  void  neascout_scienceComponentImpl ::
    report_error_events() 
  {
    /*
     * Dump errorEventBuffer to FSW EVRs.  1 per second. Reset buffer.
     *
     */
    Fw::LogStringArg errorString;
    int reportIndex = 0;

    // For each error in buffer, report as WARNING_LO
    for(int i=0; i < errorBufferIndex; i++){
       errorString = errorEventBuffer[reportIndex];
       log_WARNING_LO_asp_error(errorString);
fprintf(stderr, "%s", errorEventBuffer[reportIndex]); // TODO - remove
       // Delay 1 second between EVR messages to not overflow FSW buffers
       (void)Os::Task::delay(1000);

       reportIndex += 1;
    }

    reset_error_buffer();
  }



  int  neascout_scienceComponentImpl ::
    asp_image2d_write(asp_image2d_t *image, const Fw::CmdStringArg& frame) 
  {
    /*
     * Write NSI image to file system.
     * 
     * @param image NSI image structure to be written.
     * @param frame file system path to write file.
     *
     */
    Os::File::Status stat;
    Os::File obj;
    char msg[ASP_MAX_LOGSTRING];
    NATIVE_INT_TYPE writeLen;
    NATIVE_INT_TYPE writtenAmount = 0;
    int32_t tmpIt;
    int16_t tmpPx;
    int i;

    // NULL data check
    if (image == NULL) {
        asp_error( asp_error_image2d_nullImage, "asp_image2d_write NULL image" );
        return ASP_FAILURE;
    }

    // Open file pointer at frame specified location.
    stat = obj.open(frame.toChar(), Os::File::OPEN_WRITE);
    if (stat != Os::File::OP_OK) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "Couldn't open %s for writing", frame.toChar());
        asp_error(asp_error_open_image_failure, msg);
        return ASP_FAILURE;
    }

    // Write out camera header.  Never editted or used, just passed through to ground.
    for(i=0; i<CAMERA_HEADER_BLOCKS; i++) {

        writeLen = sizeof(image->cameraHeader[i]);
        tmpIt = image->cameraHeader[i];

        // Swap camera header blocks if endianess requires it
        if (ASP_USES_BIGENDIAN) {
            swap4(&(tmpIt));
        }

        // Write each camera header block to memory. 
        stat = obj.write((void*)&(tmpIt), writeLen);
        if(stat != Os::File::OP_OK) {
            (void)snprintf(msg, ASP_MAX_LOGSTRING, "write fail camera header block %d: %s", i, obj.getLastErrorString());
            asp_error( asp_error_os_file_object, msg );
            obj.close();
            return ASP_FAILURE;
        }
        writtenAmount += writeLen;
    }

    // Swap science header fields if endianess requires it
    if (ASP_USES_BIGENDIAN) {
        swap2(&(image->format));
        swap4(&(image->productid));
        swap4(&(image->timestamp_whole_second));
        swap4(&(image->timestamp_sub_second));
        swap4(&(image->temperature));
        swap4(&(image->exposure));
        swap4(&(image->gain));
        swap2(&(image->rows));
        swap2(&(image->cols));
        swap2(&(image->otop));
        swap2(&(image->oleft));
        swap4(&(image->underflow));
        swap4(&(image->overflow));
        swap2(&(image->fixedPoint));
    }

    // Write science image type to disk. Confirm success
    writeLen = sizeof(image->format);
    stat = obj.write((void*)&(image->format), writeLen);
    if(stat != Os::File::OP_OK) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "write fail format: %s", obj.getLastErrorString());
        asp_error( asp_error_os_file_object, msg );
        obj.close();
        return ASP_FAILURE;
    }
    writtenAmount += writeLen;

    // Write science product ID to disk. Confirm success
    writeLen = sizeof(image->productid);
    stat = obj.write((void*)&(image->productid), writeLen);
    if(stat != Os::File::OP_OK) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "write fail product ID: %s", obj.getLastErrorString());
        asp_error( asp_error_os_file_object, msg );
        obj.close();
        return ASP_FAILURE;
    }
    writtenAmount += writeLen;

    // Write science time whole seconds to disk. Confirm success
    writeLen = sizeof(image->timestamp_whole_second);
    stat = obj.write((void*)&(image->timestamp_whole_second), writeLen);
    if(stat != Os::File::OP_OK) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "write fail whole seconds: %s", obj.getLastErrorString());
        asp_error( asp_error_os_file_object, msg );
        obj.close();
        return ASP_FAILURE;
    }
    writtenAmount += writeLen;

    // Write science time sub-second to disk. Confirm success
    writeLen = sizeof(image->timestamp_sub_second);
    stat = obj.write((void*)&(image->timestamp_sub_second), writeLen);
    if(stat != Os::File::OP_OK) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "write fail sub seconds: %s", obj.getLastErrorString());
        asp_error( asp_error_os_file_object, msg );
        obj.close();
        return ASP_FAILURE;
    }
    writtenAmount += writeLen;

    // Write science camera temperature to disk. Confirm success
    writeLen = sizeof(image->temperature);
    stat = obj.write((void*)&(image->temperature), writeLen);
    if(stat != Os::File::OP_OK) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "write fail temperature: %s", obj.getLastErrorString());
        asp_error( asp_error_os_file_object, msg );
        obj.close();
        return ASP_FAILURE;
    }
    writtenAmount += writeLen;

    // Write science exposure time to disk. Confirm success
    writeLen = sizeof(image->exposure);
    stat = obj.write((void*)&(image->exposure), writeLen);
    if(stat != Os::File::OP_OK) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "write fail exposure: %s", obj.getLastErrorString());
        asp_error( asp_error_os_file_object, msg );
        obj.close();
        return ASP_FAILURE;
    }
    writtenAmount += writeLen;

    // Write science camera gain to disk. Confirm success
    writeLen = sizeof(image->gain);
    stat = obj.write((void*)&(image->gain), writeLen);
    if(stat != Os::File::OP_OK) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "write fail gain: %s", obj.getLastErrorString());
        asp_error( asp_error_os_file_object, msg );
        obj.close();
        return ASP_FAILURE;
    }
    writtenAmount += writeLen;

    // Write science image rows to disk. Confirm success
    writeLen = sizeof(image->rows);
    stat = obj.write((void*)&(image->rows), writeLen);
    if(stat != Os::File::OP_OK) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "write fail rows: %s", obj.getLastErrorString());
        asp_error( asp_error_os_file_object, msg );
        obj.close();
        return ASP_FAILURE;
    }
    writtenAmount += writeLen;

    // Write science image columns to disk. Confirm success
    writeLen = sizeof(image->cols);
    stat = obj.write((void*)&(image->cols), writeLen);
    if(stat != Os::File::OP_OK) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "write fail cols: %s", obj.getLastErrorString());
        asp_error( asp_error_os_file_object, msg );
        obj.close();
        return ASP_FAILURE;
    }
    writtenAmount += writeLen;

    // Write science image top index to disk. Confirm success
    writeLen = sizeof(image->otop);
    stat = obj.write((void*)&(image->otop), writeLen);
    if(stat != Os::File::OP_OK) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "write fail otop: %s", obj.getLastErrorString());
        asp_error( asp_error_os_file_object, msg );
        obj.close();
        return ASP_FAILURE;
    }
    writtenAmount += writeLen;

    // Write science image left index to disk. Confirm success
    writeLen = sizeof(image->oleft);
    stat = obj.write((void*)&(image->oleft), writeLen);
    if(stat != Os::File::OP_OK) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "write fail oleft: %s", obj.getLastErrorString());
        asp_error( asp_error_os_file_object, msg );
        obj.close();
        return ASP_FAILURE;
    }
    writtenAmount += writeLen;

    // Write science underflow counter to disk. Confirm success
    writeLen = sizeof(image->underflow);
    stat = obj.write((void*)&(image->underflow), writeLen);
    if(stat != Os::File::OP_OK) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "write fail underflow: %s", obj.getLastErrorString());
        asp_error( asp_error_os_file_object, msg );
        obj.close();
        return ASP_FAILURE;
    }
    writtenAmount += writeLen;

    // Write science overflow counter to disk. Confirm success
    writeLen = sizeof(image->overflow);
    stat = obj.write((void*)&(image->overflow), writeLen);
    if(stat != Os::File::OP_OK) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "write fail overflow: %s", obj.getLastErrorString());
        asp_error( asp_error_os_file_object, msg );
        obj.close();
        return ASP_FAILURE;
    }
    writtenAmount += writeLen;

    // Write science fixed point scaler to disk. Confirm success
    writeLen = sizeof(image->fixedPoint);
    stat = obj.write((void*)&(image->fixedPoint), writeLen);
    if(stat != Os::File::OP_OK) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "write fail fixed point: %s", obj.getLastErrorString());
        asp_error( asp_error_os_file_object, msg );
        obj.close();
        return ASP_FAILURE;
    }
    writtenAmount += writeLen;

    // Confirm we wrote the correct amount of header data to the file system
    if(writtenAmount != IMAGE_DATA_OFFSET){
        asp_error(asp_error_image2d_writeImageFailed,
          "asp_write_image2d malformed header value");
        obj.close();
        return ASP_FAILURE;
    }
    
    // Convert pixel data endianess if needed.
    if (ASP_USES_BIGENDIAN) {
        for (uint32_t i=0; i<image->npix; i++){
            tmpPx = image->data[i];
            swap2(&(tmpPx));
            image->data[i] = tmpPx;
        }
    }

    // Write out pixel data.  Always 16 bit.
    writeLen = sizeof(asp_pixel_t) * image->npix;
    stat = obj.write(&(image->data[0]), writeLen);
    if(stat != Os::File::OP_OK) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "data write failed: %s", obj.getLastErrorString());
        asp_error(asp_error_os_file_object, msg);
        obj.close();
        return ASP_FAILURE;
    }

    obj.close();

    return ASP_SUCCESS;
  }


  asp_image2d_t * neascout_scienceComponentImpl ::
    asp_image2d_alloc(int rows, int cols, int buffer)
  {
    /*
     * Allocate memory for generic NSI image structure
     * 
     * @param rows number of image rows to allocate memory for
     * @param cols number of image columns to allocate memory for
     * @param buffer global buffer index for storing data
     *
     */
    asp_image2d_t *image = NULL;
    int npix = rows*cols;
    int i;
    char msg[ASP_MAX_LOGSTRING];

    // Row/Col bounds checking
    if ((cols <= 0) ||
        (rows <= 0) ||
        (rows > ASP_MAX_ROWS) ||
        (cols > ASP_MAX_COLS))
    {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "bad alloc parms - rows:%d cols:%d", rows, cols);
        asp_error(asp_error_image2d_readImageBadImageParameters, msg);
        return NULL;
    }

    if( npix < 1 ) {
        asp_error(asp_error_image2d_insufficient_npix, "asp_image2d_alloc npix < 1");
        return NULL;
    }

    // Request buffer from flight software
    image = (asp_image2d_t*) asp_malloc(BUFFER_SIZE, buffer);
    if (image == NULL) {
        asp_error(asp_error_image2d_imageAllocatorFailed, "asp_image2d_alloc malloc failed");
        return NULL;
    }

    // Insert empty camera header
    for(i=0; i<CAMERA_HEADER_BLOCKS; i++){
        image->cameraHeader[i] = 0;
    }

    // Allocate basic header for new image. Details set in caller
    image->format                 = ASP_PRODTYPE_RAW;
    image->productid              = 0;
    image->timestamp_whole_second = 0;
    image->timestamp_sub_second   = 0;
    image->temperature            = 0.0;
    image->exposure               = 0.0;
    image->gain                   = 0.0;
    image->rows                   = rows;
    image->cols                   = cols;
    image->npix                   = npix;
    image->otop                   = 0;
    image->oleft                  = 0;
    image->underflow              = 0;
    image->overflow               = 0;
    image->fixedPoint             = 1;
    image->buffer                 = buffer;

    // Memory pointer for pixel data inside image structure
    image->data = (asp_pixel_t *)((long int)image+sizeof(asp_image2d_t));
    if( image->data == NULL ) {
        asp_error( asp_error_image2d_imageAllocatorFailed, "asp_image2d_alloc data malloc failed" );
        return NULL;
    }
  
    return image;
  }

  int  neascout_scienceComponentImpl ::
     asp_image2d_readhdr(asp_image2d_t *image, const Fw::CmdStringArg& frame) 
  {
    /*
     * Read NSI header data
     * 
     * @param image NSI image structure to read from
     * @param frame file system path to read file.
     *
     */
    Os::File::Status stat;
    Os::File obj;
    NATIVE_INT_TYPE readLen;
    NATIVE_INT_TYPE readAmount = 0;
    char msg[ASP_MAX_LOGSTRING];
    int tmpIt;
    int i;

    // NULL check on incoming image structure
    if (image == NULL) {
        asp_error(asp_error_image2d_imageAllocatorFailed, "asp_image2d_readhdr NULL image" );
        return ASP_FAILURE;
    }

    // Open read pointer to file
    stat = obj.open(frame.toChar(), Os::File::OPEN_READ);
    if (stat != Os::File::OP_OK) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "%s:%s", frame.toChar(),obj.getLastErrorString());
        asp_error(asp_error_os_file_object, msg);
        return ASP_FAILURE;
    }

    // Read camera header blocks, swap endian if necessary
    for(i=0; i<CAMERA_HEADER_BLOCKS; i++){
        readLen = sizeof(image->cameraHeader[i]);
        stat = obj.read((void*)&(tmpIt), readLen);
        if(stat != Os::File::OP_OK) {
            (void)snprintf(msg, ASP_MAX_LOGSTRING, "read fail camera header block %d: %s", i, obj.getLastErrorString());
            asp_error( asp_error_os_file_object, msg );
            obj.close();
            return ASP_FAILURE;
        }

        readAmount += readLen;
        if (ASP_USES_BIGENDIAN) {
           swap4(&(tmpIt));
        }
        image->cameraHeader[i] = tmpIt;
    }

    // Read science image format. Confirm success
    readLen = sizeof(image->format);
    stat = obj.read((void*)&(image->format), readLen);
    if(stat != Os::File::OP_OK) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "read fail format: %s", obj.getLastErrorString());
        asp_error( asp_error_os_file_object, msg );
        obj.close();
        return ASP_FAILURE;
    }
    readAmount += readLen;

    // Read science product ID. Confirm success
    readLen = sizeof(image->productid);
    stat = obj.read((void*)&(image->productid), readLen);
    if(stat != Os::File::OP_OK) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "read fail product ID: %s", obj.getLastErrorString());
        asp_error( asp_error_os_file_object, msg );
        obj.close();
        return ASP_FAILURE;
    }
    readAmount += readLen;

    // Read science time in whole seconds. Confirm success
    readLen = sizeof(image->timestamp_whole_second);
    stat = obj.read((void*)&(image->timestamp_whole_second), readLen);
    if(stat != Os::File::OP_OK) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "read fail whole second: %s", obj.getLastErrorString());
        asp_error( asp_error_os_file_object, msg );
        obj.close();
        return ASP_FAILURE;
    }
    readAmount += readLen;

    // Read science time in sub-seconds. Confirm success
    readLen = sizeof(image->timestamp_sub_second);
    stat = obj.read((void*)&(image->timestamp_sub_second), readLen);
    if(stat != Os::File::OP_OK) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "read fail sub second: %s", obj.getLastErrorString());
        asp_error( asp_error_os_file_object, msg );
        obj.close();
        return ASP_FAILURE;
    }
    readAmount += readLen;

    // Read science image temperature. Confirm success
    readLen = sizeof(image->temperature);
    stat = obj.read((void*)&(image->temperature), readLen);
    if(stat != Os::File::OP_OK) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "read fail temperature: %s", obj.getLastErrorString());
        asp_error( asp_error_os_file_object, msg );
        obj.close();
        return ASP_FAILURE;
    }
    readAmount += readLen;

    // Read science image exposure. Confirm success
    readLen = sizeof(image->exposure);
    stat = obj.read((void*)&(image->exposure), readLen);
    if(stat != Os::File::OP_OK) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "read fail exposure: %s", obj.getLastErrorString());
        asp_error( asp_error_os_file_object, msg );
        obj.close();
        return ASP_FAILURE;
    }
    readAmount += readLen;

    // Read science image gain. Confirm success
    readLen = sizeof(image->gain);
    stat = obj.read((void*)&(image->gain), readLen);
    if(stat != Os::File::OP_OK) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "read fail gain: %s", obj.getLastErrorString());
        asp_error( asp_error_os_file_object, msg );
        obj.close();
        return ASP_FAILURE;
    }
    readAmount += readLen;

    // Read science image rows. Confirm success
    readLen = sizeof(image->rows);
    stat = obj.read((void*)&(image->rows), readLen);
    if(stat != Os::File::OP_OK) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "read fail rows: %s", obj.getLastErrorString());
        asp_error( asp_error_os_file_object, msg );
        obj.close();
        return ASP_FAILURE;
    }
    readAmount += readLen;

    // Read science image columns. Confirm success
    readLen = sizeof(image->cols);
    stat = obj.read((void*)&(image->cols), readLen);
    if(stat != Os::File::OP_OK) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "read fail cols: %s", obj.getLastErrorString());
        asp_error( asp_error_os_file_object, msg );
        obj.close();
        return ASP_FAILURE;
    }
    readAmount += readLen;

    // Read science image top row index. Confirm success
    readLen = sizeof(image->otop);
    stat = obj.read((void*)&(image->otop), readLen);
    if(stat != Os::File::OP_OK) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "read fail otop: %s", obj.getLastErrorString());
        asp_error( asp_error_os_file_object, msg );
        obj.close();
        return ASP_FAILURE;
    }
    readAmount += readLen;

    // Read science image left column index. Confirm success
    readLen = sizeof(image->oleft);
    stat = obj.read((void*)&(image->oleft), readLen);
    if(stat != Os::File::OP_OK) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "read fail oleft: %s", obj.getLastErrorString());
        asp_error( asp_error_os_file_object, msg );
        obj.close();
        return ASP_FAILURE;
    }
    readAmount += readLen;

    // Read science image underflow counter. Confirm success
    readLen = sizeof(image->underflow);
    stat = obj.read((void*)&(image->underflow), readLen);
    if(stat != Os::File::OP_OK) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "read fail underflow: %s", obj.getLastErrorString());
        asp_error( asp_error_os_file_object, msg );
        obj.close();
        return ASP_FAILURE;
    }
    readAmount += readLen;

    // Read science image overflow counter. Confirm success
    readLen = sizeof(image->overflow);
    stat = obj.read((void*)&(image->overflow), readLen);
    if(stat != Os::File::OP_OK) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "read fail overflow: %s", obj.getLastErrorString());
        asp_error( asp_error_os_file_object, msg );
        obj.close();
        return ASP_FAILURE;
    }
    readAmount += readLen;

    // Read science image fixed point scaler. Confirm success
    readLen = sizeof(image->fixedPoint);
    stat = obj.read((void*)&(image->fixedPoint), readLen);
    if(stat != Os::File::OP_OK) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "read fail fixed point: %s", obj.getLastErrorString());
        asp_error( asp_error_os_file_object, msg );
        obj.close();
        return ASP_FAILURE;
    }
    readAmount += readLen;

    // Swap endianness if necessary
    if (ASP_USES_BIGENDIAN) {
        swap2(&(image->format));
        swap4(&(image->productid));
        swap4(&(image->timestamp_whole_second));
        swap4(&(image->timestamp_sub_second));
        swap4(&(image->temperature));
        swap4(&(image->exposure));
        swap4(&(image->gain));
        swap2(&(image->rows));
        swap2(&(image->cols));
        swap2(&(image->otop));
        swap2(&(image->oleft));
        swap4(&(image->underflow));
        swap4(&(image->overflow));
        swap2(&(image->fixedPoint));
    }

    // Confirm the correct amount of header data was read
    if(readAmount != IMAGE_DATA_OFFSET){
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "insuff hdr data %d/%d bytes", readAmount, IMAGE_DATA_OFFSET);
        asp_error(asp_error_image2d_readImageMalformedHeader, msg);
        obj.close();
        return ASP_FAILURE;
    }

    // Bounds checking on row/col/otop/oleft header fields
    if ((image->cols <= 0) ||
        (image->rows <= 0) ||
        (image->rows > ASP_MAX_ROWS) ||
        (image->cols > ASP_MAX_COLS) ||
        (image->fixedPoint <= 0) ||
        ((image->cols + image->oleft) > ASP_MAX_COLS) ||
        ((image->rows + image->otop) > ASP_MAX_ROWS))
    {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "%s bad parms r:%d c:%d fp:%d t:%d l:%d", 
        frame.toChar(), image->rows, image->cols, image->fixedPoint, image->otop, image->oleft);
        asp_error( asp_error_image2d_readImageBadImageParameters, msg);
        obj.close();
        return ASP_FAILURE;
    }

    obj.close();
    return ASP_SUCCESS;
}



  asp_costack_t* neascout_scienceComponentImpl ::
    asp_costack_alloc(int rows, int cols,
                      int numrefs, int radius)
  {
    /*
     * Allocate memory for a coaddition stack structure
     * 
     * @param rows number of rows in each image
     * @param cols number of columns in each image
     * @param numrefs number of stars being tracked in the coaddition
     * @param radius search radius around each star in the coaddition
     *
     * @return stack pointer to stack structure for coaddition
     *
     */
    asp_costack_t* stack;
    char msg[ASP_MAX_LOGSTRING];
    int totalStars;
    int ref_coord_space;
    U64 offset;

    // Bounds checking on rows/cols/numrefs and radius
    if(rows <= 0 || cols <= 0 || numrefs <= 0 || radius <= 0) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "costack alloc failed, bad parms: rows:%d cols:%d numrefs:%d radius:%d", rows, cols, numrefs, radius);
        asp_error(asp_error_coadd_nonpositive_stack_parameters, msg);
        return NULL;
    }

    // Radius cannot be larger than half the width/height of the image
    if((2*radius+1) > rows || (2*radius+1) > cols) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "costack alloc failed, radius too large: %d", radius);
        asp_error(asp_error_coadd_radius_oversize, msg);
        return NULL;
    }

    // Allocate memory for image stack
    stack = (asp_costack_t*)asp_malloc(BUFFER_SIZE, M_IMAGE_BUFF1);
    if(stack == NULL) {
        asp_error(asp_error_coadd_stack_malloc_failure, "out of memory");
        return NULL;
    }

    // Set stack values based on arguments, NULL pointers for internal images and set unknown values to 0
    stack->numrefs = numrefs;
    stack->radius = radius;

    stack->img1 = NULL;
    stack->img2 = NULL;
    stack->img3 = NULL;

    stack->offx1 = 0;
    stack->offy1 = 0;
    stack->offx2 = 0;
    stack->offy2 = 0;
    stack->offx3 = 0;
    stack->offy3 = 0;
    stack->nframes = 0;

    stack->ref_coords_x = NULL;
    stack->ref_coords_y = NULL;

    // Calculate maximum number of x/y coords which can be tracked, based on buffer size
    totalStars = (BUFFER_SIZE - sizeof(asp_costack_t)) / ASP_NONZERO((sizeof(int) * 2));
    if(totalStars > (rows * cols)){
        totalStars = rows * cols;
    }

    // Ensure number of stars being requested fits inside buffer space
    if(numrefs > totalStars) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "stack ref stars exceeds allowable (%d/%d)", numrefs, totalStars);
        asp_error(asp_error_coadd_excessive_ref_stars, msg);
        return NULL;
    }
    
    // Pointer arithmetic to x coord storage pointers in stack buffer
    ref_coord_space = sizeof(int) * numrefs;
    offset = U64(stack + sizeof(asp_costack_t));
    stack->ref_coords_x = (int*)offset;
    if(stack->ref_coords_x == NULL) {
        asp_error(asp_error_coadd_ref_coords_x_malloc_failure,
          "asp_costack_alloc failed to malloc int* ref_coords_x");
        return NULL;
    }

    // Pointer arithmetic to y coord storage pointers in stack buffer
    offset += U64(ref_coord_space);
    stack->ref_coords_y = (int*)(offset);
    if(stack->ref_coords_y == NULL) {
        asp_error(asp_error_coadd_ref_coords_y_malloc_failure,
          "asp_costack_alloc failed to malloc int* ref_coords_y");
        return NULL;
    }

    // Allocate memory for base image
    stack->base = asp_image2d_alloc(rows, cols, M_IMAGE_BUFF2);
    if(stack->base == NULL) {
        asp_error(asp_error_coadd_base_malloc_failure,
          "asp_costack_alloc failed to malloc asp_image2d_t* base");
        return NULL;
    }

    // Set pixel data frame to all 0s.
    (void)memset(stack->base->data, 0,
      (unsigned long)stack->base->npix * sizeof(asp_pixel_t));
    stack->base->format = ASP_PRODTYPE_COADD;

    return stack;
}


  void   neascout_scienceComponentImpl ::
    asp_free_buffer(int buffer) 
  {
    /*
     * Return data buffer to FSW buffer manager
     *
     * Note that buffers must be returned in the order they're requested
     * 
     * @param buffer index of the buffer to be returned
     *
     */
    if(buffer == M_IMAGE_BUFF1) {
        asp_log(asp_log_buffer_return, "Returned buffer 1");
        this->bufferReturn_out(0, this->m_imageBuff1);
        this->bufferTrack[0] = 0;
    }
    else if(buffer == M_IMAGE_BUFF2) {
        asp_log(asp_log_buffer_return, "Returned buffer 2");
        this->bufferReturn_out(0, this->m_imageBuff2);
        this->bufferTrack[1] = 0;
    }
    else if(buffer == M_IMAGE_BUFF3) {
        asp_log(asp_log_buffer_return, "Returned buffer 3");
        this->bufferReturn_out(0, this->m_imageBuff3);
        this->bufferTrack[2] = 0;
    }
    else if(buffer == M_IMAGE_BUFF4) {
        asp_log(asp_log_buffer_return, "Returned buffer 4");
        this->bufferReturn_out(0, this->m_imageBuff4);
        this->bufferTrack[3] = 0;
    }
    else if(buffer == M_IMAGE_BUFF5) {
        asp_log(asp_log_buffer_return, "Returned buffer 5");
        this->bufferReturn_out(0, this->m_imageBuff5);
        this->bufferTrack[4] = 0;
    }
    else if(buffer == M_IMAGE_BUFF6) {
        asp_log(asp_log_buffer_return, "Returned buffer 6");
        this->bufferReturn_out(0, this->m_imageBuff6);
        this->bufferTrack[5] = 0;
    }
    else if(buffer == M_IMAGE_BUFF7) {
        asp_log(asp_log_buffer_return, "Returned buffer 7");
        this->bufferReturn_out(0, this->m_imageBuff7);
        this->bufferTrack[6] = 0;
    }
    else {
        asp_error(asp_error_buffer_unknown,
           "asp_free_buffer: unknown buffer");
        FW_ASSERT(0);
        return;
    }

    // Update tracker of how many buffers are currently checked out
    this->buffOrderCount -= 1;
    return;
  }

  void   neascout_scienceComponentImpl ::
    clean_exit(const FwOpcodeType opCode,
               const U32 cmdSeq,
               int status) 
  {
    /*
     * Generic clean up command called at the end of every command.
     *   Last thing called, regardless of success, or exit condition.
     * 
     * @param opCode flight software generated parameter needed for reporting
     * @param cmdSeq flight software generated parameter needed for reporting
     * @param status flag specifying nominal or off-nominal exit for EVR reporting
     *
     */
    int current = 0; 
    int lowest = 0;
    int deleteBuffer = 0; 
    int deleteIndex = 0;
    int i = 0;
    int j = 0;

    // In-order return of requested buffers
    for(i=0; i < TOTAL_BUFFERS; i++) {

        lowest = TOTAL_BUFFERS + 1;
        deleteBuffer = 0;

        for(j=0; j < TOTAL_BUFFERS; j++){

            current = this->bufferTrack[j];

            if(current < lowest) {
                if(current != 0) {
                    lowest = current;
                    deleteBuffer = j+1;
                    deleteIndex = j;
                }
            }
        }

        if(deleteBuffer != 0) {
            this->bufferTrack[deleteIndex] = 0;
            asp_free_buffer(deleteBuffer);        
        }

    }
    
    #ifdef TGT_OS_TYPE_VXWORKS
        // Dump logging EVRs
        if(strnlen(logEventBuffer[0], ASP_MAX_LOGSTRING) != 1) {
            report_log_events();
        }
    #else
        // Dump logging EVRs
        if(strnlen(logEventBuffer[0], ASP_MAX_LOGSTRING) != 0) {
            report_log_events();
        }
    #endif

    #ifdef TGT_OS_TYPE_VXWORKS
        // Dump error EVRs
        if(strnlen(errorEventBuffer[0], ASP_MAX_LOGSTRING) != 1) {
            report_error_events();
        }
    #else
        // Dump error EVRs
        if(strnlen(errorEventBuffer[0], ASP_MAX_LOGSTRING) != 0) {
            report_error_events();
        }
    #endif

    // Use status variable to determine success/failure of command.
    if(status == ASP_SUCCESS){
        this->cmdResponse_out(opCode, cmdSeq, Fw::COMMAND_OK);
    } else {
        this->cmdResponse_out(opCode, cmdSeq, Fw::COMMAND_EXECUTION_ERROR);
    }

    return;
 }

  void*  neascout_scienceComponentImpl ::
    asp_access(int buffer) 
  {
    /*
     * Get memory pointer from a given buffer.
     * 
     * @param buffer index to buffer for which we want memory location
     *
     * @return pointer to memory location of requested buffer
     *
     */
    char msg[ASP_MAX_LOGSTRING];

    // Assert it is a valid buffer index.
    FW_ASSERT(buffer > 0);
    FW_ASSERT(buffer <= TOTAL_BUFFERS);

    // Confirm buffer has already been checked out from FSW
    if(this->bufferTrack[buffer -1] == 0) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "Buffer %d not declared in use", buffer);
        asp_error(asp_error_buffer_in_use, msg);
        return NULL;
    }

    if(buffer == M_IMAGE_BUFF1) {
        return (void*)this->m_imageBuff1.getdata();   
    }
    else if(buffer == M_IMAGE_BUFF2) {
        return (void*)this->m_imageBuff2.getdata();
    }
    else if(buffer == M_IMAGE_BUFF3) {
        return (void*)this->m_imageBuff3.getdata();
    }
    else if(buffer == M_IMAGE_BUFF4) {
        return (void*)this->m_imageBuff4.getdata();
    }
    else if(buffer == M_IMAGE_BUFF5) {
        return (void*)this->m_imageBuff5.getdata();
    }
    else if(buffer == M_IMAGE_BUFF6) {
        return (void*)this->m_imageBuff6.getdata();
    }
    else if(buffer == M_IMAGE_BUFF7) {
        return (void*)this->m_imageBuff7.getdata();
    }
    else{
        FW_ASSERT(0);
        return NULL;
    }

}


  void*  neascout_scienceComponentImpl ::
    asp_malloc(size_t size, int buffer) 
  {
    /*
     * Get memory buffer from FSW buffer manager
     * 
     * @param size amount of data to request (Mb)
     * @param buffer index of buffer to track it in
     *
     * @return pointer to memory location of requested buffer
     *
     */
    Fw::Buffer imageBuff;
    NATIVE_INT_TYPE portBuff = 0;
    U32 bufferSize = size;

    // Assert the buffer index is valid
    FW_ASSERT(buffer > 0);
    FW_ASSERT(buffer <= TOTAL_BUFFERS);

    // If you've already requested the buffer, but want to re-use it, wipe old data first
    if(this->bufferTrack[buffer -1] != 0) {
        (void)memset(asp_access(buffer), 0, BUFFER_SIZE);
        return asp_access(buffer);
    }

    // Request the appropriate buffer and store it in correct neascout_science buffer variable
    if(buffer == M_IMAGE_BUFF1) {
        this->m_imageBuff1 = bufferGetCaller_out(portBuff, bufferSize);
        if(((void*)this->m_imageBuff1.getdata() == NULL) ||
           (this->m_imageBuff1.getdata() == 0) || 
           (this->m_imageBuff1.getsize() != bufferSize)) {
              asp_error(asp_error_memory_allocatorFailed, 
                "memory buffer 1 allocation failed" );
              return NULL;
        }
        if(claimBuffer(buffer) != ASP_FAILURE) {
            asp_log(asp_log_buffer_allocate, "Allocated buffer 1");
            return (void*)this->m_imageBuff1.getdata();
        } else {
            FW_ASSERT(0);
            return NULL;
        }

    }
    else if(buffer == M_IMAGE_BUFF2) {
        this->m_imageBuff2 = bufferGetCaller_out(portBuff, bufferSize);
        if(((void*)this->m_imageBuff2.getdata() == NULL) ||
           (this->m_imageBuff2.getdata() == 0) ||
           (this->m_imageBuff2.getsize() != bufferSize)) {
              asp_error(asp_error_memory_allocatorFailed,
                "memory buffer 2 allocation failed" );
              return NULL;
        }
        if(claimBuffer(buffer) != ASP_FAILURE) {
            asp_log(asp_log_buffer_allocate, "Allocated buffer 2");
            return (void*)this->m_imageBuff2.getdata();
        } else {
            FW_ASSERT(0);
            return NULL;
        }
    }
    else if(buffer == M_IMAGE_BUFF3) {
        this->m_imageBuff3 = bufferGetCaller_out(portBuff, bufferSize);
        if(((void*)this->m_imageBuff3.getdata() == NULL) ||
           (this->m_imageBuff3.getdata() == 0) ||
           (this->m_imageBuff3.getsize() != bufferSize)) {
              asp_error(asp_error_memory_allocatorFailed,
                "memory buffer 3 allocation failed" );
              return NULL;
        }
        if(claimBuffer(buffer) != ASP_FAILURE) {
            asp_log(asp_log_buffer_allocate, "Allocated buffer 3");
            return (void*)this->m_imageBuff3.getdata();
        } else {
            FW_ASSERT(0);
            return NULL;
        }
    }
    else if(buffer == M_IMAGE_BUFF4) {
        this->m_imageBuff4 = bufferGetCaller_out(portBuff, bufferSize);
        if(((void*)this->m_imageBuff4.getdata() == NULL) ||
           (this->m_imageBuff4.getdata() == 0) ||
           (this->m_imageBuff4.getsize() != bufferSize)) {
              asp_error(asp_error_memory_allocatorFailed,
                "memory buffer 4 allocation failed" );
              return NULL;
        }
        if(claimBuffer(buffer) != ASP_FAILURE) {
            asp_log(asp_log_buffer_allocate, "Allocated buffer 4");
            return (void*)this->m_imageBuff4.getdata();
        } else {
            FW_ASSERT(0);
            return NULL;
        }
    }
    else if(buffer == M_IMAGE_BUFF5) {
        this->m_imageBuff5 = bufferGetCaller_out(portBuff, bufferSize);
        if(((void*)this->m_imageBuff5.getdata() == NULL) ||
           (this->m_imageBuff5.getdata() == 0) ||
           (this->m_imageBuff5.getsize() != bufferSize)) {
              asp_error(asp_error_memory_allocatorFailed,
                "memory buffer 5 allocation failed" );
              return NULL;
        }
        if(claimBuffer(buffer) != ASP_FAILURE) {
            asp_log(asp_log_buffer_allocate, "Allocated buffer 5");
            return (void*)this->m_imageBuff5.getdata();
        } else {
            FW_ASSERT(0);
            return NULL;
        }
    }
    else if(buffer == M_IMAGE_BUFF6) {
        this->m_imageBuff6 = bufferGetCaller_out(portBuff, bufferSize);
        if(((void*)this->m_imageBuff6.getdata() == NULL) ||
           (this->m_imageBuff6.getdata() == 0) ||
           (this->m_imageBuff6.getsize() != bufferSize)) {
              asp_error(asp_error_memory_allocatorFailed,
                "memory buffer 6 allocation failed" );
              return NULL;
        }
        if(claimBuffer(buffer) != ASP_FAILURE) {
            asp_log(asp_log_buffer_allocate, "Allocated buffer 6");
            return (void*)this->m_imageBuff6.getdata();
        } else {
            FW_ASSERT(0);
            return NULL;
        }
    }
    else if(buffer == M_IMAGE_BUFF7) {
        this->m_imageBuff7 = bufferGetCaller_out(portBuff, bufferSize);
        if(((void*)this->m_imageBuff7.getdata() == NULL) ||
           (this->m_imageBuff7.getdata() == 0) ||
           (this->m_imageBuff7.getsize() != bufferSize)) {
              asp_error(asp_error_memory_allocatorFailed,
                "memory buffer 7 allocation failed" );
              return NULL;
        }
        if(claimBuffer(buffer) != ASP_FAILURE) {
            asp_log(asp_log_buffer_allocate, "Allocated buffer 7");
            return (void*)this->m_imageBuff7.getdata();
        } else {
            FW_ASSERT(0);
            return NULL;
        }
    } else {
        FW_ASSERT(0);
        return NULL;
    }

}


  int  neascout_scienceComponentImpl ::
    asp_image2d_read(asp_image2d_t **image, const Fw::CmdStringArg& frame, int buffer, int buffer2) 
  {

    /*
     * Read an NSI image from the file system
     * 
     * @param image memory for storing image
     * @param frame path to the image on the file system
     * @param buffer primary buffer to track the image in
     * @param buffer2 secondary buffer for unpacking 12-bit data quickly
     *
     * @return flag indicating success or failure of the functional operation
     *
     */
    asp_image2d_t *img = NULL;
    char *dataTmp;
    int len;
    int pix_i = 0;
    int i_idx = 0;
    int iterations = 0;

    if (image == NULL){
        asp_error(asp_error_image2d_imageAllocatorFailed, "asp_image2d_read image pointer NULL");
        return ASP_FAILURE;    
    }

    // Allocate memory to store the image
    img = (asp_image2d_t*) asp_malloc(BUFFER_SIZE, buffer);
    if (img == NULL) {
        asp_error(asp_error_image2d_imageAllocatorFailed, "asp_image2d_read image memory allocate failed");
        return ASP_FAILURE;
    }

    // Track which buffer we're using to store the image
    img->buffer = buffer;
 
    // Read the image header, both camera and science
    if(asp_image2d_readhdr(img, frame) != ASP_SUCCESS) {
        return ASP_FAILURE;
    }

    // Test for special-case file formats designed to be read 
    //   with a different routine
    if ((img->format == ASP_PRODTYPE_ICER_RAW)        ||
        (img->format == ASP_PRODTYPE_ICER_FLAT)       ||
        (img->format == ASP_PRODTYPE_ICER_DARK)       ||
        (img->format == ASP_PRODTYPE_ICER_MASK)       ||
        (img->format == ASP_PRODTYPE_ICER_CAL)        ||
        (img->format == ASP_PRODTYPE_ICER_COADD)      ||
        (img->format == ASP_PRODTYPE_ICER_DOWNSAMPLE) ||
        (img->format == ASP_PRODTYPE_ICER_SUBTRACT))
    {
        asp_error(asp_error_image2d_unsupportedProduct, "asp_image2d_read cannot read compressed data.");
        return ASP_FAILURE;
    }
 
    img->npix = img->rows * img->cols;
    img->buffer = buffer;
    img->data = (asp_pixel_t*)(((long int)img) + sizeof(asp_image2d_t));
    if (img->data == NULL) {
        asp_error(asp_error_image2d_imageAllocatorFailed, "asp_image2d_read image data memory allocate failed");
        return ASP_FAILURE;
    }

    // If FF00, must unpack from 12->16 bit data
    if(img->format == ASP_PRODTYPE_PACKED) {

        // Places where 12->16 bit unpacking is done requires a second buffer.
        //  Only available in L1, IMGSTAT and mask updater, must unpack in those commands
        if(buffer2 != M_IMAGE_BUFF_NULL){

            dataTmp = (char *)asp_malloc(BUFFER_SIZE, buffer2);
            if(dataTmp == NULL){
                asp_error(asp_error_image2d_imageAllocatorFailed, "asp_image2d_read unpack buffer alloc failed");
                return ASP_FAILURE;
            }

            len = (int)((double)img->npix * 1.5);
            if (freade(&(dataTmp[0]),sizeof(char), len, frame, IMAGE_DATA_OFFSET, TRUE) != ASP_SUCCESS) {
                return ASP_FAILURE;
            }

            // unpack images: 3 bytes => 2 pixels
            iterations = (static_cast<int>(img->npix * 1.5));
            for (i_idx=0; i_idx < iterations; i_idx+=3)
            {

              // Unpack formula provided by Christophe
              img->data[pix_i] = (asp_pixel_t)(((dataTmp[i_idx+0] & 0x00FF)) << 4 ) | ((dataTmp[i_idx+1] & 0x00F0) >> 4);
              img->data[pix_i+1] = (asp_pixel_t)((dataTmp[i_idx+1] & 0x000F) << 8) | (dataTmp[i_idx+2] & 0x00FF);
             
              // Count satured pixels at 12-bit max dynamic range
              if(img->data[pix_i] == 4095){
                  img->overflow += 1;
              }

              if(img->data[pix_i+1] == 4095){
                  img->overflow += 1;
              }

              pix_i += 2;

            }

            // Set to 16-bit format specifier
            img->format = ASP_PRODTYPE_RAW;

        } else {
            asp_error(asp_error_image2d_bad_bit_depth, "asp_image2d_read data must be unpacked in L1 first");
            return ASP_FAILURE;
        }
        
    }
    else {

        // Read 16 bit data if the format is not FF00 camera 12 bit raw
        if (freade(&(img->data[0]),sizeof(asp_pixel_t),img->npix,frame,IMAGE_DATA_OFFSET, TRUE) != ASP_SUCCESS) {
            return ASP_FAILURE;
        }

    }

    *image = img;
    return ASP_SUCCESS;
  }


  size_t neascout_scienceComponentImpl ::
    freade(void *ptr, size_t size, size_t nitems, const Fw::CmdStringArg& frame, int offset, int peak)
  {
    /*
     * Helper function to read image data chunks
     * 
     * @param ptr pointer with location to store data
     * @param size size of each data object
     * @param nitems number of data objects to read
     * @param file system path to read data from
     * @param offset offset into the file to start reading data from
     * @param boolean flag to peak beyond the amount of data requested to be read, plus offset, to see if more data is in file
     *
     * @return flag indicating success or failure of functional process
     *
     */
    Os::File::Status stat;
    Os::File obj;
    char msg[ASP_MAX_LOGSTRING];
    size_t nread = nitems;
    NATIVE_INT_TYPE readLen = size * nitems;
    NATIVE_INT_TYPE originalReadLen = readLen;
    U64 fileSize;
    U64 expectedSize;

    // Assert we're reading a positive amount of data and location to store data is not NULL
    FW_ASSERT(size > 0);
    FW_ASSERT(ptr != NULL);

    unsigned char c[8];
    unsigned char *x = (unsigned char *) ptr;
    
    // Open file
    stat = obj.open(frame.toChar(), Os::File::OPEN_READ);
    if (stat != Os::File::OP_OK) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "Os::File Error: %s", obj.getLastErrorString());
        asp_error(asp_error_os_file_object, msg);
        return ASP_FAILURE;
    }
 
    // Seek to the read location in the file
    stat = obj.seek(offset);
    if(stat != Os::File::OP_OK) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "freade data seek status: %s", obj.getLastErrorString());
        asp_error( asp_error_image2d_readImageInsufficientData, msg );
        obj.close();
        return ASP_FAILURE;
    }

    // Read data from file
    stat = obj.read(ptr, readLen);
    if(stat != Os::File::OP_OK) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "freade data read status: %s", obj.getLastErrorString());
        asp_error( asp_error_image2d_readImageInsufficientData, msg );
        obj.close();
        return ASP_FAILURE;
    }

    // Confirm we read the correct amount of data
    if(readLen != originalReadLen) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "%s insufficient data", frame.toChar());
        asp_error( asp_error_image2d_readImageInsufficientData, msg);
        obj.close();     
        return ASP_FAILURE;
    }

    // If peak is true, see if there is more data beyond offset + (nitems * size)
    if(peak == TRUE) {

        Os::FileSystem::Status fsStatus = Os::FileSystem::getFileSize(frame.toChar(), fileSize);
        if (fsStatus != Os::FileSystem::OP_OK) {
            (void)snprintf(msg, ASP_MAX_LOGSTRING, "%s getFileSize Failed", frame.toChar());
            asp_error(asp_error_image2d_checkFileTooLarge, msg);
            obj.close();
            return ASP_FAILURE;
        }

        expectedSize = (size * nitems) + IMAGE_DATA_OFFSET;

        if(fileSize != expectedSize) {
            (void)snprintf(msg, ASP_MAX_LOGSTRING, "%s bad size:%llu", frame.toChar(), fileSize);
            asp_error(asp_error_image2d_checkFileTooLarge, msg);
            obj.close();
            return ASP_FAILURE;
        }

    }

    // Swap endianess if necessary
    if (ASP_USES_BIGENDIAN) {

        for (size_t i=0; i<nread*size; i+=size) {
            for (size_t j=0; j<size; j++) {
                c[j] = x[i+j];
            }
            for (size_t j=0; j<size; j++) {
                x[i+j] = c[size-1-j];
            }
        }
    }

    obj.close();
    return ASP_SUCCESS;
  }


  void** neascout_scienceComponentImpl ::
    array2(long d1, long d2, long size, int buffer1, int buffer2)

  {
    /*
     * Adaptation of ICER data structure, using NEASC buffers. Two buffers are
     *    needed since ICER casts I16 data to I32 for compression.  
     * 
     * @param d1 number of rows to store data for
     * @param d2 number of cols to store data for
     * @param size size of each pixel
     * @param buffer1 first neascout science buffer to store rows in
     * @param buffer2 second neascout science buffer to store cols in
     *
     * @return pointer to memory location of pixel data
     *
     */
    char **a;
    long i;

    // Assert the number of rows is within our acceptable thresholds
    FW_ASSERT(d1 > 0);
    FW_ASSERT(d1 <= ASP_MAX_ROWS);

    // Allocate first image stucture
    a = (char **) asp_malloc(BUFFER_SIZE, buffer1);
    if(a == NULL){
        asp_error(asp_error_image2d_imageAllocatorFailed,
          "array2: first memory allocate failed" );
        return NULL;
    }

    // Allocate second structure
    a[0] = (char*)   asp_malloc(BUFFER_SIZE, buffer2);
    if(a[0] == NULL){
        asp_error(asp_error_image2d_imageAllocatorFailed,
          "array2: second memory allocate failed" );
        return NULL;
    }

    for(i=1;i<d1;i++){
        a[i] = a[i-1] + size*d2;
    }

    return (void**)a;
  }

  int neascout_scienceComponentImpl ::
     icerwrite(asp_image2d_t *img, const Fw::CmdStringArg& frame, const int min_loss, 
                const int filtype, const int ndecomps, const int nseg)
  {
    /*
     * Wrapper around ICER compression functionality 
     * 
     * @param img image data to be compressed
     * @param frame location on file path to store compressed data
     * @param min_loss minimum loss compression rate. See ICER guide for more information.
     * @param filtype ICER wavelet controller.  See ICER guide for more information.
     * @param ndecomps ICER decompositions controller.  See ICER guide for more information.
     * @param nseg ICER segments per image.  See ICER guide for more information.
     *
     * @return status indicating success or failure of functional processing
     *
     */
    if (img == NULL) {
        asp_error(asp_error_image2d_imageAllocatorFailed, "icerwrite img memory NULL" );
        return ASP_FAILURE;
    }

    // lossless compression, full bit rate.
    double targetrate = 8.0 * sizeof(asp_pixel_t);
    return icerwrite_lossy(img, frame, targetrate, min_loss, filtype, ndecomps, nseg);

  }


  asp_bbox_t* neascout_scienceComponentImpl ::
    asp_bbox_init(int top, int left, int height, int width, int buffer)
  {
    /*
     * Initalize bounding box coordinates used in normalized cross correlation search 
     * 
     * @param top index for the top row of the bounding box
     * @param left index for the left column of the bounding box
     * @param height number of rows to be included in the bounding box
     * @param width number of columns to be included in the bounding box
     * @param buffer index of the buffer to store the bounding box information
     *
     * @return pointer to memory location of bounding box
     *
     */
    asp_bbox_t* bbox;
    char msg[ASP_MAX_LOGSTRING];

    // Bounds checking on top left corner
    if(top < 0 || left < 0) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "asp_bbox_init failed bounds check t:%d l:%d", top, left);
        asp_error(asp_error_bbox_bad, msg);
        return NULL;
    }

    // Bounding box on the overall size of the box
    if(height <= 0 || width <= 0) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "asp_bbox_init failed bounds check h:%d w:%d", height, width);
        asp_error(asp_error_bbox_bad, msg);
        return NULL;
    }

    // Allocate memory, ensure it gets allocated
    bbox = asp_bbox_alloc(buffer);
    if(bbox == NULL) {
        return NULL;
    }

    // Fill box with functional arguments
    bbox->top = top;
    bbox->left = left;
    bbox->height = height;
    bbox->width = width;

    return bbox;
  }

  asp_bbox_t* neascout_scienceComponentImpl ::
    asp_bbox_alloc(int buffer)
  {
    /*
     * Allocate memory for bounding box used in normalized cross correlation
     * 
     * @param buffer index of buffer to store bounding box in
     *
     * @return pointer to memory location of empty bounding box
     *
     */
    asp_bbox_t* bbox;

    // Allocate buffer, the size of a bounding box
    bbox = (asp_bbox_t*) asp_malloc(sizeof(asp_bbox_t), buffer);
    if(bbox == NULL) {
        asp_error(asp_error_bbox_malloc_failure, "asp_bbox_alloc failed");
        return NULL;
    }

    // Placeholder values for structure contents
    bbox->top = 0;
    bbox->left = 0;
    bbox->height = 0;
    bbox->width = 0;

    return bbox;
  }


  int  neascout_scienceComponentImpl ::
    coadd_increment_filename(const char *base, char *inc, int index) 
  {
    /*
     * Increments a file name, for sequential image reading in image coaddition.
     *   Can have preamble, like /sci/ before NSI filename.
     *
     * 
     * @param base base string of the coaddition process.  
     * @param inc resulting filename after increment
     * @param index index count after increment process.  
     *
     * @return flag indicating the success or failure of the functional process
     *
     */
    long long int old;
    int preamble;

    // Isolate file path preamble, before the NSI file name
    #ifdef TGT_OS_TYPE_VXWORKS
        preamble = strnlen(base, MAX_FILE_LENGTH) - NSSI_FILENAME_LENGTH - 1;
    #else
        preamble = strnlen(base, MAX_FILE_LENGTH) - NSSI_FILENAME_LENGTH;
    #endif

    FW_ASSERT(preamble < MAX_FILE_LENGTH);

    char front[preamble + 1];
    char filename[NSSI_FILENAME_LENGTH + 1];

    (void)strncpy(front, base, preamble);
    (void)strncpy(filename, &base[preamble], NSSI_FILENAME_LENGTH);

    front[preamble] = '\0';
    filename[NSSI_FILENAME_LENGTH] = '\0';

    // Use index argument to increment the filename, in the NSI filename format
    if (sscanf(filename, "nssi%010lld", &old) != 1) {
        return ASP_FAILURE;
    }

    if (snprintf(inc, ASP_MAX_LOGSTRING, "%snssi%010lld", front, old + index) > 0) {
        return ASP_SUCCESS;
    }

    return ASP_FAILURE;
  }

  int neascout_scienceComponentImpl ::
    icerwrite_lossy(asp_image2d_t *img, const Fw::CmdStringArg& frame, 
                    const double targetrate, const int min_loss, const int filtype,
                    const int ndecomps, const int nseg) 
  {
    /*
     * Apply ICER compression to an NSSI image and write it to disk
     *
     * @param img image data to be compressed
     * @param frame file path location to write the resulting data. Folder must exist, image cannot.
     * @param targetrate target compression rate of the compressed image - bits per pixel.
     * @param min_loss minimum loss compression rate. See ICER guide for more information.
     * @param filtype ICER wavelet controller.  See ICER guide for more information.
     * @param ndecomps ICER decompositions controller.  See ICER guide for more information.
     * @param nseg ICER segments per image.  See ICER guide for more information.
     *
     * @return flag indicating the success or failure of the functional process
     *
     */

    // Image arrays
    IcerPixelType  **im;      // image array (32-bit dynamic range)

    // Image parameters
    unsigned int wd;       // image width
    unsigned int ht;       // image height

    // ICER compression parameters
    int    bytequota;         // ICER byte quota

    // Variables and working memory for ICER compression function, 
    // and information about the compressed image
    IcerMemorySpaceType *resultspace = NULL;
    IcerMemorySpaceType *workspace = NULL;
    IcerBitstreamType **blockboundary = NULL;
    int **blockstart;
    int *nblocks;
    int *lastwordbits;
    int totalnblocks;
    int32_t nbits[ICER_MAX_N_SEGS];
    unsigned char *dataseg[ICER_MAX_N_SEGS];
    I32 resultbytes;
    I32 workbytes;
    IcerBitstreamType *thedata;
    IcerBitstreamType *pdata;
    IcerBitstreamType *pword;
    int totalcompbytes;  // total number of compressed bytes
    int totalbits;       // total  number of compressed bits

    char msg[ASP_MAX_LOGSTRING];  // asp_error and asp_log string

    // misc. indices, iteration variables
    unsigned int rowindex;
    unsigned int colindex;
    int i;
    int seg;  // segment index
    int blk;  // block index

    int status;  // status of image file read, compression, decompression
    int px_min;  // minimum DN value in original image
    int px_max;  // maximum DN value in original image
    int dr;      // dynamic range of the original image
    int iterations = 0;
    unsigned int ht2 = 0;
    unsigned int ht1 = 0;
    double blocks;
    uint16_t prev_type;
    int32_t iterations_swap;
    int tmpIt;
    int z;       // Numbber of compression interations to fit in memory requirement
    int ICER_HEADER_ELEMENTS = 3;  // Number of nested ICER header elements in science header

    // This collection of variables stores header values which are endian swapped after in-place
    //    swap.  We do not store them back directly in their originating memory since the endian
    //    correct can/are used after the swap for additional logic decisions.
    int32_t nbits_swap[ICER_MAX_N_SEGS];  // Hold endian swapped nbits array for writing
    int32_t nseg_swap;                    // Hold endian swapped nseg for writing
    int32_t max_val_swap;                 // Hold endian swapped max value for writing
    int32_t min_val_swap;                 // Hold endian swapped min value for writing
    int32_t tmp_nbit;                     // Hold endian swapped nbit value for writing

    long int count;
    long int block2;

    Os::File::Status stat;
    Fw::Buffer resultBuff;
    Fw::Buffer workspaceBuff;
    Fw::Buffer compBuff;
    Os::File obj;
    NATIVE_INT_TYPE writeLen;
    NATIVE_INT_TYPE writtenAmount = 0;

    // Ensure incoming image data is not NULL
    if (img == NULL) {
        asp_error(asp_error_image2d_imageAllocatorFailed, "icerwrite_lossy img memory NULL" );
        return ASP_FAILURE;
    }

    // Bounds checking on incoming image header data used in compression
    if ((img->cols <= 0) ||
        (img->rows <= 0) ||
        (img->rows > ASP_MAX_ROWS) ||
        (img->cols > ASP_MAX_COLS))
    {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "bad icerwrite image parms - rows:%d cols:%d", img->rows, img->cols);
        asp_error(asp_error_image2d_readImageBadImageParameters, msg);
        return ASP_FAILURE;
    }

    // Calculate the dynamic range of the incoming image
    status = asp_dynamic_range(img, &px_min, &px_max);
    if (status == ASP_FAILURE) {
        asp_error(asp_error_crop_dyanmic_range, "Dynamic Range Computation Failed");
        return ASP_FAILURE;
    }

    dr = px_max - px_min;

    (void)snprintf(msg, ASP_MAX_LOGSTRING, "Dynamic Range: %d", dr);
    asp_log(asp_log_dynamic_range, msg);
    
    // Make sure this is not already ICER data
    //   Also, modify format information to reflect new compressed content
    prev_type = img->format;
    switch(prev_type) {
        case ASP_PRODTYPE_RAW:
            img->format = ASP_PRODTYPE_ICER_RAW;
            break;
        case ASP_PRODTYPE_FLAT:
            img->format = ASP_PRODTYPE_ICER_FLAT;
            break;
        case ASP_PRODTYPE_DARK:
            img->format = ASP_PRODTYPE_ICER_DARK;
            break; 
        case ASP_PRODTYPE_MASK:
            img->format = ASP_PRODTYPE_ICER_MASK;
            break; 
        case ASP_PRODTYPE_CAL:
            img->format = ASP_PRODTYPE_ICER_CAL;
            break; 
        case ASP_PRODTYPE_COADD:
            img->format = ASP_PRODTYPE_ICER_COADD;
            break; 
        case ASP_PRODTYPE_DOWNSAMPLE:
            img->format = ASP_PRODTYPE_ICER_DOWNSAMPLE;
            break; 
        case ASP_PRODTYPE_SUBTRACT:
            img->format = ASP_PRODTYPE_ICER_SUBTRACT;
            break; 
        default:
            img->format = ASP_PRODTYPE_ICER_RAW;
            break; 
    }

    // Acquire width and height
    (void)snprintf(msg, ASP_MAX_LOGSTRING, "Compressing an image of size %i x %i", img->rows, img->cols);
    asp_log(asp_log_icer_original_image_size, msg);

    // Since ICER needs I32, we can only compress half an NSI image at a time, if it is full size.  
    //   Determine size of incoming image and determine if we need one or two compression iterations to compress the whole input image.
    wd = img->cols;
    ht = img->rows;
    blocks = (double)((double)ht * (double)wd * (double)sizeof(IcerPixelType)) / (double)ASP_NONZERO(BUFFER_SIZE);
    if(blocks <= 1){
        iterations = 1;
        ht1 = ht;
    } else if((blocks > 1.0) && (blocks < 2.0)){
        iterations = 2;
        ht1 = (unsigned int) ceil((double)ht / 2.0);
        ht2 = (unsigned int) floor((double)ht / 2.0);
    } else {
         (void)snprintf(msg, ASP_MAX_LOGSTRING, "ASP_ICER block definition - %f blocks not supported", blocks);
         asp_error(asp_error_icer_bad_block_count, msg);
         FW_ASSERT(0);
         return ASP_FAILURE;
    }

    // Open file pointer to write compressed image
    stat = obj.open(frame.toChar(), Os::File::OPEN_WRITE);
    if (stat != Os::File::OP_OK) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "Couldn't open %s for writing", frame.toChar());
        asp_error(asp_error_open_image_failure, msg);
        return ASP_FAILURE;
    }

    // Swap endian for writing
    iterations_swap = (int32_t)iterations;
    if (ASP_USES_BIGENDIAN) {
        swap2(&(img->format));
        swap4(&(img->productid));
        swap4(&(img->timestamp_whole_second));
        swap4(&(img->timestamp_sub_second));
        swap4(&(img->temperature));
        swap4(&(img->exposure));
        swap4(&(img->gain));
        swap2(&(img->rows));
        swap2(&(img->cols));
        swap2(&(img->otop));
        swap2(&(img->oleft));
        swap4(&(img->underflow));
        swap4(&(img->overflow));
        swap2(&(img->fixedPoint));
        swap4(&iterations_swap);
    }

    // Write camera header blocks to disk, swap endian if necessary
    for(i=0; i<CAMERA_HEADER_BLOCKS; i++) {

        writeLen = sizeof(img->cameraHeader[i]);
        tmpIt = img->cameraHeader[i];

        if (ASP_USES_BIGENDIAN) {
            swap4(&(tmpIt));
        }

        stat = obj.write((void*)&(tmpIt), writeLen);
        if(stat != Os::File::OP_OK) {
            (void)snprintf(msg, ASP_MAX_LOGSTRING, "ICER write fail camera header block %d: %s", i, obj.getLastErrorString());
            asp_error( asp_error_os_file_object, msg );
            obj.close();
            return ASP_FAILURE;
        }
        writtenAmount += writeLen;
    }

    // Write science image format to disk.  Confirm success
    writeLen = sizeof(img->format);
    stat = obj.write((void*)&(img->format), writeLen);
    if(stat != Os::File::OP_OK) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "ICER write fail format: %s", obj.getLastErrorString());
        asp_error(asp_error_os_file_object, msg);
        obj.close();
        return ASP_FAILURE;
    }
    writtenAmount += writeLen;

    // Write science image product ID to disk.  Confirm success
    writeLen = sizeof(img->productid);
    stat = obj.write((void*)&(img->productid), writeLen);
    if(stat != Os::File::OP_OK) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "ICER write fail product ID: %s", obj.getLastErrorString());
        asp_error(asp_error_os_file_object, msg);
        obj.close();
        return ASP_FAILURE;
    }
    writtenAmount += writeLen;

    // Write science image time whole seconds.  Confirm success
    writeLen = sizeof(img->timestamp_whole_second);
    stat = obj.write((void*)&(img->timestamp_whole_second), writeLen);
    if(stat != Os::File::OP_OK) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "ICER write fail whole second: %s", obj.getLastErrorString());
        asp_error(asp_error_os_file_object, msg);
        obj.close();
        return ASP_FAILURE;
    }
    writtenAmount += writeLen;

    // Write science image time sub seconds  Confirm success
    writeLen = sizeof(img->timestamp_sub_second);
    stat = obj.write((void*)&(img->timestamp_sub_second), writeLen);
    if(stat != Os::File::OP_OK) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "ICER write fail sub second: %s", obj.getLastErrorString());
        asp_error(asp_error_os_file_object, msg);
        obj.close();
        return ASP_FAILURE;
    }
    writtenAmount += writeLen;

    // Write science image temperature.  Confirm success
    writeLen = sizeof(img->temperature);
    stat = obj.write((void*)&(img->temperature), writeLen);
    if(stat != Os::File::OP_OK) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "ICER write fail temperature: %s", obj.getLastErrorString());
        asp_error(asp_error_os_file_object, msg);
        obj.close();
        return ASP_FAILURE;
    }
    writtenAmount += writeLen;

    // Write science image exposure.  Confirm success
    writeLen = sizeof(img->exposure);
    stat = obj.write((void*)&(img->exposure), writeLen);
    if(stat != Os::File::OP_OK) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "ICER write fail exposure: %s", obj.getLastErrorString());
        asp_error(asp_error_os_file_object, msg);
        obj.close();
        return ASP_FAILURE;
    }
    writtenAmount += writeLen;

    // Write science image gain.  Confirm success
    writeLen = sizeof(img->gain);
    stat = obj.write((void*)&(img->gain), writeLen);
    if(stat != Os::File::OP_OK) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "ICER write fail gain: %s", obj.getLastErrorString());
        asp_error(asp_error_os_file_object, msg);
        obj.close();
        return ASP_FAILURE;
    }
    writtenAmount += writeLen;

    // Write science image rows.  Confirm success
    writeLen = sizeof(img->rows);
    stat = obj.write((void*)&(img->rows), writeLen);
    if(stat != Os::File::OP_OK) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "ICER write fail rows: %s", obj.getLastErrorString());
        asp_error(asp_error_os_file_object, msg);
        obj.close();
        return ASP_FAILURE;
    }
    writtenAmount += writeLen;

    // Write science image cols.  Confirm success
    writeLen = sizeof(img->cols);
    stat = obj.write((void*)&(img->cols), writeLen);
    if(stat != Os::File::OP_OK) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "ICER write fail cols: %s", obj.getLastErrorString());
        asp_error(asp_error_os_file_object, msg);
        obj.close();
        return ASP_FAILURE;
    }
    writtenAmount += writeLen;

    // Write science image top row index.  Confirm success
    writeLen = sizeof(img->otop);
    stat = obj.write((void*)&(img->otop), writeLen);
    if(stat != Os::File::OP_OK) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "ICER write fail otop: %s", obj.getLastErrorString());
        asp_error(asp_error_os_file_object, msg);
        obj.close();
        return ASP_FAILURE;
    }
    writtenAmount += writeLen;

    // Write science image left column index.  Confirm success
    writeLen = sizeof(img->oleft);
    stat = obj.write((void*)&(img->oleft), writeLen);
    if(stat != Os::File::OP_OK) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "ICER write fail oleft: %s", obj.getLastErrorString());
        asp_error(asp_error_os_file_object, msg);
        obj.close();
        return ASP_FAILURE;
    }
    writtenAmount += writeLen;

    // Write science image underflow counter.  Confirm success
    writeLen = sizeof(img->underflow);
    stat = obj.write((void*)&(img->underflow), writeLen);
    if(stat != Os::File::OP_OK) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "ICER write fail underflow: %s", obj.getLastErrorString());
        asp_error(asp_error_os_file_object, msg);
        obj.close();
        return ASP_FAILURE;
    }
    writtenAmount += writeLen;

    // Write science image overflow counter.  Confirm success
    writeLen = sizeof(img->overflow);
    stat = obj.write((void*)&(img->overflow), writeLen);
    if(stat != Os::File::OP_OK) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "ICER write fail overflow: %s", obj.getLastErrorString());
        asp_error(asp_error_os_file_object, msg);
        obj.close();
        return ASP_FAILURE;
    }
    writtenAmount += writeLen;

    // Write science image fixed point scaler.  Confirm success
    writeLen = sizeof(img->fixedPoint);
    stat = obj.write((void*)&(img->fixedPoint), writeLen);
    if(stat != Os::File::OP_OK) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "ICER write fail fixed point: %s", obj.getLastErrorString());
        asp_error(asp_error_os_file_object, msg);
        obj.close();
        return ASP_FAILURE;
    }
    writtenAmount += writeLen;

    // Write science image iterations count (ICER specific header field).  Confirm success
    writeLen = sizeof(iterations_swap);
    stat = obj.write(&iterations_swap, writeLen);
    if(stat != Os::File::OP_OK) {
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "ICER write fail iterations: %s", obj.getLastErrorString());
        asp_error(asp_error_os_file_object, msg);
        obj.close();
        return ASP_FAILURE;
    }
    writtenAmount += writeLen;

    // Confirm we wrote the correct amount of header data to disk.
    if(writtenAmount != IMAGE_DATA_OFFSET+sizeof(iterations_swap)){
        asp_error(asp_error_image2d_writeImageFailed, "asp_write_image2d malformed header value");
        obj.close();
        return ASP_FAILURE;
    }

    // Allow 3 seconds of sleep for memory scrubbing after file setup
    (void)Os::Task::delay(3000);

    for(z=0; z<iterations; z++) {

        if(z == 1) {
            ht = ht2;
        } else if (z == 0){
            ht = ht1;
        }else{
            asp_error(asp_error_image2d_writeImageFailed, "asp_write_image2d malformed iterations");
            obj.close();
            return ASP_FAILURE;
        }

        // Allocate im array.  Uses ICER array2 function for storing at I32
        im = (IcerPixelType **) array2((long)ht, (long)wd, (long)sizeof(IcerPixelType), M_IMAGE_BUFF6, M_IMAGE_BUFF5);
        if(im == NULL) {
             asp_error(asp_error_image2d_imageAllocatorFailed,"icerwrite_lossy im is NULL" );
             obj.close();
             return ASP_FAILURE;
        }

        // Move the correct data into the ICER data structure.  Either top half of image, or bottom, depending on iteration.
        count = 0;
        block2 = (long int)((double)ht1 * (double)wd);
        for (rowindex=0; rowindex<ht; rowindex++) {
            for (colindex=0; colindex<wd; colindex++) {
                if(z == 0) {
                    im[rowindex][colindex] = img->data[count];
                }
                else if(z == 1) {
                    im[rowindex][colindex] = img->data[count+block2];
                }
                else{
                    (void)snprintf(msg, ASP_MAX_LOGSTRING, "ASP_ICER block definition - %f blocks not supported", blocks);
                    asp_error(asp_error_icer_bad_block_count, msg);
                    obj.close();
                    FW_ASSERT(0);
                    return ASP_FAILURE;
               }
               count += 1;
            }
        }

        bytequota = (int)(0.5 + ((float)(wd*ht)*(targetrate/8.0)));
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "Byte quota = %d", bytequota);
        asp_log(asp_log_icer_byte_quota, msg);

        // Find the number of bytes needed to store the compressed image
        status = icer_find_bytes_needed(wd,
                                        ht,
                                        wd,
                                        bytequota,
                                        filtype,
                                        ndecomps,
                                        nseg,
                                        min_loss,
                                        &resultbytes,
                                        &workbytes);
    
        // Allow 3 seconds of sleep for memory scrubbing after byte space calculation
        (void)Os::Task::delay(3000);

        // Exit here if number of bytes needed could not be found
        if (status != 0) {
            (void)snprintf(msg, ASP_MAX_LOGSTRING, "icer_find_bytes_needed status: %x", status);
            asp_error(asp_error_icer_status, msg);
            obj.close();
            return ASP_FAILURE;
        }

        // Allocate ICER resultspace data structure
        resultspace = (IcerMemorySpaceType*)asp_malloc(BUFFER_SIZE, M_IMAGE_BUFF3);
        if (resultspace == NULL) {
            asp_error(asp_error_image2d_imageAllocatorFailed, "icerwrite_lossy resultspace is NULL" );
            obj.close();
            return ASP_FAILURE;
        }
    
        // Allocate ICER workspace data structure
        workspace = (IcerMemorySpaceType*)asp_malloc(BUFFER_SIZE, M_IMAGE_BUFF4);
        if (workspace == NULL) {
            asp_error(asp_error_image2d_imageAllocatorFailed, "icerwrite_lossy workspace is NULL" );
            obj.close();
            return ASP_FAILURE;
        }

        // Compress the image chunk
        status = icer_compress(resultbytes,
                               resultspace,
                               workbytes,
                               workspace,
                               im[0],
                               wd,
                               ht,
                               wd,
                               bytequota,
                               filtype,
                               ndecomps,
                               nseg,
                               min_loss,
                               &blockboundary,
                               &blockstart,
                               &nblocks,
                               &lastwordbits,
                               &totalnblocks);

        // Allow 5 seconds of sleep for memory scrubbing after compression
        (void)Os::Task::delay(5000);

        // exit here if no compression was done
        if (status != 0) {
            (void)snprintf(msg, ASP_MAX_LOGSTRING, "icer_compress status: %x", status);
            asp_error(asp_error_icer_status, msg);
            obj.close();
            return ASP_FAILURE;
        }
        
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "ICER_compress status word: %x",status);
        asp_log(asp_log_icer_compression_status_word, msg);
    
        // number of compressed bytes
        totalcompbytes = (blockboundary[totalnblocks] -
                          blockboundary[0]) * sizeof(IcerBitstreamType);

        // allocate to exact number of bytes needed rather than just reserving a big array
        thedata = (IcerBitstreamType *)asp_malloc(totalcompbytes, M_IMAGE_BUFF5);
        if (thedata == NULL) {    
            asp_error(asp_error_image2d_imageAllocatorFailed, "icerwrite_lossy thedata is NULL" );
            obj.close();
            return ASP_FAILURE;
        }

        // Prepare compressed data for writing - nsegs
        pdata = thedata;
        for (seg=0; seg<nseg; seg++) {
            dataseg[seg] = (unsigned char *) pdata;
            nbits[seg] = 0;
            for (i=0; i<nblocks[seg]; i++) {
                blk = blockstart[seg][i];
                pword=blockboundary[blk];
                while (pword<blockboundary[blk+1]) {
                    *pdata++ = *pword++;
                    nbits[seg] += 8*sizeof(IcerBitstreamType);
                }
            }
        }

        // Calculate totalbits used in compression
        totalbits = 0;
        for (seg=0; seg<nseg; seg++) {
            totalbits += nbits[seg];
        }

        pdata = thedata;
        for (seg=0; seg<nseg; seg++) {
            dataseg[seg] = (unsigned char *) pdata;
            nbits[seg] = 0;
            for (i=0; i<nblocks[seg]; i++) {
                blk = blockstart[seg][i];
                pword=blockboundary[blk];
                while (pword<blockboundary[blk+1]
                         && (pword-blockboundary[0])*sizeof(IcerBitstreamType)<static_cast<U32>(bytequota))
                {
                    *pdata++ = *pword++;
                    nbits[seg] += 8*sizeof(IcerBitstreamType);
                }
            }
        }
        totalbits = 0;
        for (seg=0; seg<nseg; seg++) {
            totalbits += nbits[seg];
        }

        (void)snprintf(msg, ASP_MAX_LOGSTRING, "Total compressed bytes: %d of %d (%f bits/pixel)",
          totalbits/8, totalcompbytes ,(double)totalbits/((double)(wd*ht)));
        asp_log(asp_log_icer_total_compressed_bytes, msg);
  
        for (seg=0; seg<nseg; seg++) {
            (void)snprintf(msg, ASP_MAX_LOGSTRING, "%d total bytes in segment of %d total",
              nbits[seg]/8, totalcompbytes);
            asp_log(asp_log_icer_segment_bytes, msg);
            report_log_events();
        }

        // Make secondary variable with swapped ICER header values for writing to disk. Don't swap in place, since we're still using them
        nseg_swap = nseg;
        max_val_swap = px_max;
        min_val_swap = px_min;
        if (ASP_USES_BIGENDIAN) {
            swap4(&max_val_swap);
            swap4(&min_val_swap);
            swap4(&nseg_swap);
        }
        for (seg=0; seg<nseg; seg++) {
              tmp_nbit = nbits[seg];
              if ASP_USES_BIGENDIAN{
                  swap4(&tmp_nbit);
              }
              nbits_swap[seg] = tmp_nbit;
        }
        writtenAmount = 0;
        writeLen = sizeof(int32_t);
        
        // Write max DN value to disk.  Confirm success.
        stat = obj.write(&max_val_swap, writeLen);
        if(stat != Os::File::OP_OK) {
            (void)snprintf(msg, ASP_MAX_LOGSTRING, "ICER write fail max_val: %s", obj.getLastErrorString());
            asp_error( asp_error_os_file_object, msg );
            obj.close();
            return ASP_FAILURE;
        }
        writtenAmount += writeLen;
        
        // Write min DN value to disk.  Confirm success.
        stat = obj.write(&min_val_swap, writeLen);
        if(stat != Os::File::OP_OK) {
            (void)snprintf(msg, ASP_MAX_LOGSTRING, "ICER write fail min_val: %s", obj.getLastErrorString());
            asp_error( asp_error_os_file_object, msg );
            obj.close();
            return ASP_FAILURE;
        }
        writtenAmount += writeLen;
        
        // Write nseg count to disk.  Confirm success.
        stat = obj.write(&nseg_swap, writeLen);
        if(stat != Os::File::OP_OK) {
            (void)snprintf(msg, ASP_MAX_LOGSTRING, "ICER write fail nseg: %s", obj.getLastErrorString());
            asp_error( asp_error_os_file_object, msg );
            obj.close();
            return ASP_FAILURE;
        }
        writtenAmount += writeLen;

        // Write nseg array to disk. Confirm success.
        writeLen = sizeof(int32_t) * nseg;
        NATIVE_INT_TYPE nbits_goal = writeLen;
        stat = obj.write((void*)nbits_swap, writeLen);
        if(stat != Os::File::OP_OK) {
            (void)snprintf(msg, ASP_MAX_LOGSTRING, "ICER write nbits failed: %s", obj.getLastErrorString());
            asp_error(asp_error_os_file_object, msg);
            obj.close();
            return ASP_FAILURE;
        }

        // Ensure we wrote the correct amount of nseg array fields
        if(writeLen != nbits_goal) {
            (void)snprintf(msg, ASP_MAX_LOGSTRING, "Nbits data written: %d of %d", writeLen, nbits_goal);
            asp_error(asp_error_os_file_write, msg);
            obj.close();
            return ASP_FAILURE;
        }

        // Ensure we wrote the correct amount of header data to disk.
        NATIVE_INT_TYPE newHeaderSize = (ICER_HEADER_ELEMENTS * sizeof(int32_t));
        if(writtenAmount != newHeaderSize){
            asp_error(asp_error_image2d_writeImageFailed,
              "asp_write_image2d malformed ICER header value");
            obj.close();
            return ASP_FAILURE;
        }

        for (seg=0; seg<nseg; seg++) {

          // Write the image data
          writeLen = sizeof(I8) * (nbits[seg]/8);
          NATIVE_INT_TYPE goal = writeLen;
          stat = obj.write((void*)dataseg[seg], writeLen);
          if(stat != Os::File::OP_OK) {
              (void)snprintf(msg, ASP_MAX_LOGSTRING, "compresed data write failed: %s", obj.getLastErrorString());
              asp_error(asp_error_os_file_object, msg);
              obj.close();
              return ASP_FAILURE;
          }

          // Ensure we wrote the correct amount of compressed image data to disk
          if(writeLen != goal) {
              (void)snprintf(msg, ASP_MAX_LOGSTRING, "Insufficient data written: %d of %d", writeLen, goal);
              asp_error(asp_error_os_file_write, msg);
              obj.close();
              return ASP_FAILURE;
          }
      }

    }

    obj.close();
    return ASP_SUCCESS;
    
}

  void neascout_scienceComponentImpl ::
    initialize_data_buffers()
  {
    /*
     * Resets buffer tracker to 0s.
     *
     * Used during buffer requests/returns to track order.  Buffers must be returned in the order they're requsted.
     *
     */
    (void)memset(this->bufferTrack, 0, sizeof(bufferTrack));
    this->buffOrderCount = 1;

    return;
  }

  int neascout_scienceComponentImpl ::
    claimBuffer(int buffer) 
  {
    /*
     * Claim a buffer from flight software buffer manager
     *
     * @param buffer index of buffer being requested.
     *
     * @return flag indicating the success or failure of the functional process
     *
     */
    char msg[ASP_MAX_LOGSTRING];

    // Array is 0 based, buffers are 1 based
    int bufferIndex = buffer - 1;

    // Ensure requested buffer index is within expected bounds
    if((bufferIndex < 0) || (bufferIndex >= TOTAL_BUFFERS)){
        (void)snprintf(msg, ASP_MAX_LOGSTRING, "buffer %d requested out of bounds", buffer);
        asp_error(asp_error_buffer_unknown, msg);
        FW_ASSERT(0);
        return ASP_FAILURE;
    }

    // Set the index of the buffer being requested to the counter of the order it was requested in.
    this->bufferTrack[bufferIndex] = this->buffOrderCount;
    this->buffOrderCount += 1;
    return ASP_SUCCESS;
  }

  int neascout_scienceComponentImpl ::
    asp_donate(char* newname, asp_costack_t* stack, asp_bbox_t* bbox1,
                           asp_bbox_t* bbox2, double scaling, const int newFixPt, const int calcShift)
  {
    /*
     * Add to the coaddition stack.  Manages temporal median calculations and data structure management
     *
     * @param stack image coaddition stack being modified
     * @param bbox1 allocated bounding box 1 for use in normalized cross correlation calculations (base image)
     * @param bbox2 allocated bounding box 2 for use in normalized cross correlation calculations (incoming image)
     * @param scaling Scaling value, to be applied to image 1, to adjust for know DN scale differences.
     * @param newFixPt new fixed point scaler to be applied to the resulting image
     * @param calcShift flag to calculate shift between images. Calculate shift if 1, otherwise, do not calculate shift
     *
     * @return flag indicating the success or failure of the functional process
     *
     */
    int shift[2] = {0,0};
    int starsfound = 0;
    int newBuffer;
    char msg[ASP_MAX_LOGSTRING];
    asp_image2d_t* img;

    // Ensure new filename was specified and is not NULL
    if(newname == NULL) {
        asp_error(asp_error_coadd_null_input,
          "asp_donate failed: new filename was NULL");
        return ASP_FAILURE;        
    }

    // Ensure coaddition image stack has been allocated
    if(stack == NULL) {
        asp_error(asp_error_coadd_null_input, "asp_donate failed: stack was NULL");
        return ASP_FAILURE;
    }

    switch (stack->nframes) {
  
        case 0:

            // First image.  Check the slot is empty
            if (stack->img1 != NULL ||
                stack->base == NULL) {
              asp_error(asp_error_coadd_inconsistent_state, "The frame counter and memory do not agree - 0.");
              return ASP_FAILURE;
            }

            // Fail if new image cannot be read
            if (asp_image2d_read(&img, newname, M_IMAGE_BUFF3, M_IMAGE_BUFF_NULL) == ASP_FAILURE) {
                return ASP_FAILURE;
            }

            stack->img1 = img;
            stack->offx1 = 0;
            stack->offy1 = 0;

            // Confirm enough stars were found
            if(asp_ref_stars(stack, img, &starsfound) != ASP_SUCCESS) {
                asp_error(asp_error_coadd_reference_stars, "asp_start_costack_first_image did not find enough stars");
                return ASP_FAILURE;
            }

            FW_ASSERT(img != NULL);
            FW_ASSERT(stack->base != NULL);

            // Use telemetry from "base" for first image.  
            //   But it becomes a coadded product.
            asp_image2d_cptelem(img, stack->base);

            // Adjust header values for fixed point and image format
            stack->base->format = ASP_PRODTYPE_COADD;
            stack->base->fixedPoint = newFixPt;
            break;

        case 1:

            // Second image.  Check the slot is empty
            if (stack->img2 != NULL ||
                stack->img1 == NULL ||
                stack->base == NULL) {
                asp_error(asp_error_coadd_inconsistent_state, "The frame counter and memory do not agree - 1.");
                return ASP_FAILURE;
            }

            // Fail if second image could not be read, otherwise put in img
            if (asp_image2d_read(&img, newname, M_IMAGE_BUFF4, M_IMAGE_BUFF_NULL) == ASP_FAILURE) {
                return ASP_FAILURE;
            }

            // Ensure image sizes are consistent
            if(img->rows != stack->base->rows ||
                img->cols != stack->base->cols) {
                (void)snprintf(msg, ASP_MAX_LOGSTRING, "donate-1 (base r:%d c:%d) (new r:%d c:%d)", 
                stack->base->rows, stack->base->cols, img->rows, img->cols);
                asp_error(asp_error_coadd_base_size_mismatch, msg);
                return ASP_FAILURE;
            }

            // If calculating shift between images, find the shift and store in x/y offsets
            if(calcShift == 1){

                // Take custody.  Compare img to stack->img1
                if(asp_find_shift_image2d(shift, stack->img1, img, stack,
                    bbox1, bbox2, scaling) != ASP_SUCCESS)
                {
                    return ASP_FAILURE;
                }

                stack->offx2 = shift[0];
                stack->offy2 = shift[1];

            } 
            else {
                stack->offx2 = 0;
                stack->offy2 = 0;
            }

            stack->img2 = img;
            break;

        case 2:

            // Third image.  Check the slot is empty
            if(stack->img3 != NULL ||
               stack->img1 == NULL ||
               stack->img2 == NULL ||
               stack->base == NULL) {
                asp_error(asp_error_coadd_inconsistent_state, "The frame counter and memory do not agree - 2.");
                return ASP_FAILURE;
            }

            // Read the image
            if (asp_image2d_read(&img, newname, M_IMAGE_BUFF5, M_IMAGE_BUFF_NULL) == ASP_FAILURE) {
                return ASP_FAILURE;
            }

            // Ensure the image sizes are consistent
            if(img->rows != stack->base->rows ||
               img->cols != stack->base->cols) {

                (void)snprintf(msg, ASP_MAX_LOGSTRING, "donate-2 (base r:%d c:%d) (new r:%d c:%d)", 
                stack->base->rows, stack->base->cols, img->rows, img->cols);
                asp_error(asp_error_coadd_base_size_mismatch, msg);
                return ASP_FAILURE;
            }

            // If calculating shift between images, find the shift and store in x/y offsets
            if(calcShift == 1) {

                // Take custody and add to base
                if(asp_find_shift_image2d(shift, stack->img1, img, stack,
                    bbox1, bbox2, scaling) != ASP_SUCCESS) {
                    return ASP_FAILURE;
                }

                stack->offx3 = shift[0];
                stack->offy3 = shift[1];

            }
            else{
                stack->offx3 = 0;
                stack->offy3 = 0;
            }

            // Now have three images, calculate temporal median and apply resulting data to structure base field
            stack->img3 = img;
            if(asp_add_temporal_median(stack) != ASP_SUCCESS){
                asp_error(asp_error_coadd_temporal_median, "Failed to add case 2 temporal median");
                return ASP_FAILURE;
            }
            break;

        default:

            // Image after the third. Check consistency
            if(stack->img1 == NULL ||
               stack->img2 == NULL ||
               stack->img3 == NULL ||
               stack->base == NULL ||
               stack->nframes < 0) {
                asp_error(asp_error_coadd_inconsistent_state,
                  "The frame counter and memory do not agree.");
                return ASP_FAILURE;
            }

            // Cycle old images to make room
            newBuffer = stack->img1->buffer;
            (void)memset((void*)stack->img1, 0, (size_t)BUFFER_SIZE);      
            stack->img1 = stack->img2;
            stack->img2 = stack->img3;

            if(asp_image2d_read(&img, newname, newBuffer, M_IMAGE_BUFF_NULL) == ASP_FAILURE) {
                return ASP_FAILURE;
            }

            // Ensure image sizes are consistent
            if(img->rows != stack->base->rows ||
                img->cols != stack->base->cols) {
                (void)snprintf(msg, ASP_MAX_LOGSTRING, "donate-d (base r:%d c:%d) (new r:%d c:%d)", 
                stack->base->rows, stack->base->cols, img->rows, img->cols);
                asp_error(asp_error_coadd_base_size_mismatch, msg);
                return ASP_FAILURE;
            }

            // Shuffle structure metrics
            stack->offx1 = stack->offx2;
            stack->offy1 = stack->offy2;
            stack->offx2 = stack->offx3;
            stack->offy2 = stack->offy3;
            stack->offx3 = 0;
            stack->offy3 = 0;
            stack->img3 = NULL;

            // If calculating shift between images, find the shift and store in x/y offsets
            if(calcShift == 1){

                // Now find the shift relative to the "base"
                scaling = (double)(stack->nframes) - 2.0;
                if(asp_find_shift_image2d(shift, stack->base, img, stack,
                     bbox1, bbox2, scaling) != ASP_SUCCESS) {
                     return ASP_FAILURE;
                }

                stack->offx3 = shift[0];
                stack->offy3 = shift[1];
            } 
            else{
                stack->offx3 = 0;
                stack->offy3 = 0;
            }

            // Now have three images, calculate temporal median and apply resulting data to structure base field
            stack->img3 = img;
            if(asp_add_temporal_median(stack) != ASP_SUCCESS) {
                asp_error(asp_error_coadd_temporal_median, "Failed to add default case temporal median");
                return ASP_FAILURE;      
            }

            break;
    }

    // Increment the frame counter and log the event
    stack->nframes = stack->nframes+1;
    (void)snprintf(msg, ASP_MAX_LOGSTRING, "Image %i shift: c:%i r:%i",
      stack->nframes, shift[0],shift[1]);
    asp_log(asp_log_coadd_nextimage, msg);

    return ASP_SUCCESS;
  }


} // end namespace Neasc
