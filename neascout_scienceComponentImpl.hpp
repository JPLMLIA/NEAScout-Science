// ====================================================================== 
// \title  neascout_scienceImpl.hpp
// \author Jack Lightholder
// \brief  hpp file for neascout_science component implementation class
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

#ifndef neascout_science_HPP
#define neascout_science_HPP

#define TRUE 1
#define FALSE 0

#define TOTAL_BUFFERS 7
#define M_IMAGE_BUFF1 1
#define M_IMAGE_BUFF2 2
#define M_IMAGE_BUFF3 3
#define M_IMAGE_BUFF4 4
#define M_IMAGE_BUFF5 5
#define M_IMAGE_BUFF6 6
#define M_IMAGE_BUFF7 7
#define M_IMAGE_BUFF_NULL 0

#define BUFFER_SIZE (16*1024*1024)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "LfNeasc/Neasc/neascout_science/neascout_scienceComponentAc.hpp"
#include "LfNeasc/Common/Util/CRCChecker.hpp"

#include "Fw/Types/BasicTypes.hpp"
#include "Os/File.hpp"
#include <Os/Task.hpp>
#include "Fw/Com/ComBuffer.hpp"
#include "Fw/FilePacket/FilePacket.hpp"
#include "Fw/Types/Assert.hpp"
#include "Fw/Types/SerialBuffer.hpp"
#include "Fw/Com/ComPacket.hpp"
#include "Fw/Types/Serializable.hpp"
#include "Fw/Port/InputSerializePort.hpp"

// Include files from ASP and ICER libraries
 extern "C" {
 #include <LfNeasc/Neasc/neascout_science/asp/asp_image2d.h>
 #include <LfNeasc/Neasc/neascout_science/asp/asp_bbox.h>
 #include <LfNeasc/Neasc/neascout_science/asp/asp_common.h>
 #include <LfNeasc/Neasc/neascout_science/asp/asp_comedi.h>
 #include <LfNeasc/Neasc/neascout_science/asp/asp_crop.h>
 #include <LfNeasc/Neasc/neascout_science/asp/asp_error.h>
 #include <LfNeasc/Neasc/neascout_science/asp/asp_calib.h>
 #include <LfNeasc/Neasc/neascout_science/asp/asp_gsmooth.h>
 #include <LfNeasc/Neasc/neascout_science/asp/asp_coshift.h>
 #include <LfNeasc/Neasc/neascout_science/asp/asp_coref.h>
 #include <LfNeasc/Neasc/neascout_science/icer/icer_compress-flushy.h>
 #include <LfNeasc/Neasc/neascout_science/icer/icer_pub.h>
 }


namespace Neasc {

  class neascout_scienceComponentImpl :
    public neascout_scienceComponentBase
  {

     I16 bufferTrack[TOTAL_BUFFERS];
     I16 buffOrderCount;

     Fw::Buffer m_imageBuff1;
     Fw::Buffer m_imageBuff2;
     Fw::Buffer m_imageBuff3;
     Fw::Buffer m_imageBuff4;
     Fw::Buffer m_imageBuff5;
     Fw::Buffer m_imageBuff6;
     Fw::Buffer m_imageBuff7;

    public:

      // ----------------------------------------------------------------------
      // Construction, initialization, and destruction
      // ----------------------------------------------------------------------

      //! Construct object neascout_science
      //!
      neascout_scienceComponentImpl(
#if FW_OBJECT_NAMES == 1
          const char *const compName /*!< The component name*/
#else
          void
#endif
      );

      //! Initialize object neascout_science
      //!
      void init(
          const NATIVE_INT_TYPE queueDepth, /*!< The queue depth*/
          const NATIVE_INT_TYPE instance = 0 /*!< The instance number*/
      );

      //! Destroy object neascout_science
      //!
      ~neascout_scienceComponentImpl(void);

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for PingRecv
      //!
      void PingRecv_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          U32 key /*!< Value to return to pinger*/
      );

    PRIVATE:

      // ----------------------------------------------------------------------
      // Command handler implementations 
      // ----------------------------------------------------------------------

      //! Implementation for NEAS_SCIENCE_CMD_L1 command handler
      //! Run the L1 pipeline on an image
      void NEAS_SCIENCE_CMD_L1_cmdHandler(
          const FwOpcodeType opCode, /*!< The opcode*/
          const U32 cmdSeq, /*!< The command sequence number*/
          const Fw::CmdStringArg& rawFrame, /*!< The raw instrument data from the camera*/
          const Fw::CmdStringArg& outFrame, /*!< File path for radiometrically-calibrated output*/
          const Fw::CmdStringArg& ffFrame1, /*!< File path for flat field m_array file*/
          const Fw::CmdStringArg& ffFrame2, /*!< File path for flat field b_array file*/
          const Fw::CmdStringArg& badFrame, /*!< File path for bad pixel corection mask file*/
          const Fw::CmdStringArg& darkFrame, /*!< File path for dark current mask file*/
          U32 fixPt, /*!< Fixed point conversion value.  Nominally 1.*/
          U32 unpack_only, /*!< If >0 just unpack the data without applying calibration*/
          I16 temperature /*!< If -99 use the image header temperature. Otherwise, use the value specified for flat field interpolation*/
      );

      //! Implementation for NEAS_SCIENCE_CMD_COADD command handler
      //! Coalign and coadd many frames
      void NEAS_SCIENCE_CMD_COADD_cmdHandler(
          const FwOpcodeType opCode, /*!< The opcode*/
          const U32 cmdSeq, /*!< The command sequence number*/
          I16 nFrames, /*!< Number of frames to coalign*/
          const Fw::CmdStringArg& baseFrame, /*!< base file path for input frames*/
          const Fw::CmdStringArg& outFrame, /*!< output file path*/
          U32 numStars, /*!< number of stars to search for in coaddition.  Nominally 5-10*/
          U32 searchRadius, /*!< radius of search area of reference star shift (pixels).*/
          F64 scaling, /*!< Scaling value utilized in coaddition xcorr/shift calculation. Nominally 1.*/
          U32 fixPt, /*!< Fixed point conversion value.  Nominally 1.*/
          U32 calcShift /*!< 1 for consider shift, 0 for no shift*/
      );

      //! Implementation for NEAS_SCIENCE_CMD_CROP command handler
      //! Crop an image and/or compress it with ICER
      void NEAS_SCIENCE_CMD_CROP_cmdHandler(
          const FwOpcodeType opCode, /*!< The opcode*/
          const U32 cmdSeq, /*!< The command sequence number*/
          const Fw::CmdStringArg& inFrame, /*!< input frame filepath*/
          const Fw::CmdStringArg& outFrame, /*!< output data filepath*/
          I16 cropLeft, /*!< left of subwindow bounding box, skip cropping if less than or equal to 0*/
          I16 cropTop, /*!< top of subwindow bounding box, skip cropping if less than or equal to 0*/
          I16 cropWidth, /*!< width of subwindow, skip cropping if less than or equal to 0 */
          I16 cropHeight, /*!< height of subwindow, skip cropping if less than or equal to 0*/
          I16 search, /*!< if less than or equal to zero, search for the brightest pixel in the subwindow and apply further cropping with this diameter*/
          F32 compressionRate, /*!< bits per pixel. greater than 12 = lossless, less than 0 = no compression */
          I16 downsampleFactor, /*!< If larger than zero, will downsample image at rate requested.  4 will give an image 1/4 the size of the original.  Overrides cropping, but not compression.*/
          I16 min_loss, /*!< ICER minimum loss parameter.*/
          I16 filtype, /*!< ICER wavelet fill type.*/
          I16 ndecomps, /*!< Number of ICER decompositions.*/
          I16 nseg /*!< Number of ICER segments.*/
      );

      //! Implementation for NEAS_SCIENCE_CMD_DETECT command handler
      //! Blur two frames and subtract them.
      void NEAS_SCIENCE_CMD_DETECT_cmdHandler(
          const FwOpcodeType opCode, /*!< The opcode*/
          const U32 cmdSeq, /*!< The command sequence number*/
          const Fw::CmdStringArg& inFrameA, /*!< input frame*/
          const Fw::CmdStringArg& inFrameB, /*!< input frame*/
          const Fw::CmdStringArg& outFrame, /*!< output file path*/
          F32 sigma, /*!< width of Gaussian kernel*/
          I16 aligned, /*!< True (1) if images should be aligned before subtraction, else false (0).*/
          U32 searchRadius, /*!< radius of search area of reference star shift (pixels).*/
          U32 numStars, /*!< number of stars to use as reference when optionally determining shift.  Nominally 5-10*/
          F64 scaling, /*!< Scaling value utilized in coaddition xcorr/shift calculation. Nominally 1.*/
          I32 gaussKernelWidth, /*!< Width of the Gaussian kernel.  Nominally 2, or 4, times the sigma value*/
          F64 gaussKernelScale, /*!< Scaling factor of the Gaussian kernel used in the detection sequence.  Nominally one, or two.*/
          U32 fixPt /*!< Fixed point conversion value.  Nominally 1.*/
      );

      //! Implementation for NEAS_SCIENCE_CMD_IMGSTAT command handler
      //! Calculate the statistics and integrity of an image, and possibly compare it to another.
      void NEAS_SCIENCE_CMD_IMGSTAT_cmdHandler(
          const FwOpcodeType opCode, /*!< The opcode*/
          const U32 cmdSeq, /*!< The command sequence number*/
          const Fw::CmdStringArg& frameA, /*!< primary frame to analyze*/
          const Fw::CmdStringArg& frameB, /*!< if length larger than 1, secondary frame for comparison*/
          I32 bin0HistMax, /*!< Defines bin 0 max value for ImgStat histogram.  See ICD.*/
          I32 bin1HistMax, /*!< Defines bin 1 max value for ImgStat histogram.  See ICD.*/
          I32 bin4HistMax, /*!< Defines bin 4 max value for ImgStat histogram.  See ICD.*/
          I32 bin5HistMax /*!< Defines bin 5 max value for ImgStat histogram.  See ICD.*/
      );

      //! Implementation for NEAS_SCIENCE_CMD_MASK_UPDATE command handler
      //! Update flight bad pixel mask
      void NEAS_SCIENCE_CMD_MASK_UPDATE_cmdHandler(
          const FwOpcodeType opCode, /*!< The opcode*/
          const U32 cmdSeq, /*!< The command sequence number*/
          const Fw::CmdStringArg& inFrame, /*!< original bad pixel mask*/
          const Fw::CmdStringArg& outFrame, /*!< result bad pixel mask*/
          I16 row, /*!< row to be adjusted (zero based)*/
          I16 col, /*!< col to be adjusted (zero based)*/
          I16 value /*!< new value. 0 for good pixel, 1 for bad.*/
      );


      void report_log_events();
      void report_error_events();

      void clean_exit(
          const FwOpcodeType opCode,
          const U32 cmdSeq,
          int status);

      int asp_image2d_read(
          asp_image2d_t **image, 
          const Fw::CmdStringArg& frame,
          int buffer, 
          int buffer2);

      int asp_image2d_readhdr(
          asp_image2d_t *image, 
          const Fw::CmdStringArg& frame);

      int asp_image2d_write(
          asp_image2d_t *image, 
          const Fw::CmdStringArg& frame);

      size_t freade(
          void *ptr, 
          size_t size, 
          size_t nitems, 
          const Fw::CmdStringArg& frame, 
          int offset,
          int peak);

      void asp_free_buffer(int buffer);

     /**
      * An allocator/initializer for an asp_costack_t.  This creates and 
      * allocates a based coadded stack image (<base>) with <num_lines> lines and 
      * <num_samples> samples.  It allocates sizeof(int)*<numrefs> memory for each 
      * of <ref_coords_x> and <ref_coords_y>.  <numrefs> and <radius> are stored 
      * in the struct, and <img1>, <img2>, and <img3> are allocated empty 
      * asp_offset_image2d_ts with NULL in the underlying image pointer
      **/
      asp_costack_t* asp_costack_alloc(
          int rows, 
          int cols,
          int numrefs, 
          int radius);

      asp_image2d_t * asp_image2d_alloc(
          int rows, 
          int cols,
          int buffer);

      void* asp_malloc(size_t size, int buffer);
      void* asp_access(int buffer);


      int coadd_increment_filename(
           const char *base, 
           char *inc, 
           int index);

      int icerwrite(
           asp_image2d_t *img, 
           const Fw::CmdStringArg& frame,
           const int min_loss,
           const int filtype,
           const int ndecomps, 
           const int nseg);

      int icerwrite_lossy(
           asp_image2d_t *img, 
           const Fw::CmdStringArg& frame, 
           const double targetrate,
           const int min_loss, 
           const int filtype,
           const int ndecomps, 
           const int nseg);


      /**
       * An allocator which returns a pointer to the newly allocated 
       * asp_image2d_bbox_t.  Members are initialized as per the parameters, 
       * and <size> is set to <height>*<width>
       */
      asp_bbox_t* asp_bbox_init(int top, int left, int height, int width, int buffer);


      /**
       * An allocator returns a pointer to the newly allocated asp_bbox_t.
       * <top>, <left>, <height>, <width>, and <size> are all set to 0, 
       * and <img> is set to NULL
       */
       asp_bbox_t* asp_bbox_alloc(int buffer);
       
       asp_bbox_t* asp_bbox_reset(asp_bbox_t* bbox);

      /**
       * A function for donating new images to the coadded stack.  This coregisters 
       * the new images to the first one donated, and adds them to stack->base.  It 
       * stores the new image in stack->img1, stack->img2, and stack->img3.  It also
       * performs a temporal median filter prior to coaddition.  This means that it 
       * will only start to accumulate values in stack->base after three frames have
       * been registered.  It manages the size of its FILO queue, freeing old images
       * as needed to make room.  Note that, after an image has been "donated" to 
       * the stack, the stack functions are responsible for freeing it.  
       * 
       * Inputs:
       *   img: the new image to be added to the stack
       *   stack: the coadded stack
       * Outputs:
       *   ASP_FAILURE if either of the inputs are NULL or if the stack is in an 
       *     inconsistent state.
       *   ASP_SUCCESS otherwise
       **/
       int asp_donate(char* newname, 
          asp_costack_t* stack, 
          asp_bbox_t* bbox1,
          asp_bbox_t* bbox2, 
          double scaling, 
          const int newFixPt, 
          const int calcShift);

      /**
       *  The corresponding free command for an asp_image2d_bbox created by 
       *   asp_image2d_bbox_alloc() or asp_image2d_bbox_init().  <top>, <left>, 
       *   <height>, <width>, and <size> are all set to 0, and <img> is set to 
       *   NULL, before freeing.  However, <img> is *not* freed
       **/
       void asp_bbox_free(asp_bbox_t* bbox);

      /**
       * Frees all memory associated with the given Image.
       **/
       void asp_image2d_free (asp_image2d_t **image);

       void asp_free (void *ptr, int size);

       /**
        * The corresponding free for an asp_costack_t allocated by 
        * asp_costack_alloc().  It frees all of the memory which the struct owns, 
        * including any image data which has been "donated" to it. 
        **/
       void asp_costack_free(asp_costack_t* stack);

       void** array2(long d1, long d2, long size, int buffer1, int buffer2);

       void initialize_data_buffers();

       int claimBuffer(int buffer);

    };

} // end namespace Neasc

#endif
