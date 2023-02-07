// ====================================================================== 
// \title  neascout_scienceComponentAc.hpp
// \author Auto-generated
// \brief  hpp file for neascout_science component base class
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

#ifndef NEASCOUT_SCIENCE_COMP_HPP_
#define NEASCOUT_SCIENCE_COMP_HPP_

#include <Fw/Cfg/Config.hpp>
#include <Fw/Port/InputSerializePort.hpp>
#include <Fw/Port/OutputSerializePort.hpp>
#include <Fw/Comp/ActiveComponentBase.hpp>
#include <Fw/Cmd/CmdString.hpp>
#include <Fw/Time/TimePortAc.hpp>
#include <Fw/Log/LogString.hpp>
#include <Fw/Time/Time.hpp>
#include <Fw/Log/LogBuffer.hpp>
#include <Fw/Buffer/BufferSerializableAc.hpp>
#include <Fw/Cmd/CmdArgBuffer.hpp>
#include <Fw/Log/TextLogString.hpp>
#include <Fw/Time/TimePortAc.hpp>
#include <Fw/Cmd/CmdRegPortAc.hpp>
#include <Fw/Log/LogPortAc.hpp>
#include <Fw/Buffer/BufferGetPortAc.hpp>
#include <Fw/Cmd/CmdPortAc.hpp>
#include <Svc/Ping/PingPortAc.hpp>
#include <Fw/Buffer/BufferSendPortAc.hpp>
#include <Fw/Cmd/CmdResponsePortAc.hpp>
#if FW_ENABLE_TEXT_LOGGING == 1
#include <Fw/Log/LogTextPortAc.hpp>
#endif

namespace Neasc {

  //! \class neascout_scienceComponentBase
  //! \brief Auto-generated base for neascout_science component
  //!
  class neascout_scienceComponentBase :
    public Fw::ActiveComponentBase
  {

    // ----------------------------------------------------------------------
    // Friend classes
    // ----------------------------------------------------------------------

    //! Friend class for white-box testing
    //!
    friend class neascout_scienceComponentBaseFriend;

  public:

    // ----------------------------------------------------------------------
    // Getters for typed input ports
    // ----------------------------------------------------------------------

    //! Get input port at index
    //!
    //! \return cmdIn[portNum]
    //!
    Fw::InputCmdPort* get_cmdIn_InputPort(
        NATIVE_INT_TYPE portNum /*!< The port number*/
    );

    //! Get input port at index
    //!
    //! \return PingRecv[portNum]
    //!
    Svc::InputPingPort* get_PingRecv_InputPort(
        NATIVE_INT_TYPE portNum /*!< The port number*/
    );

  public:

    // ----------------------------------------------------------------------
    // Connect typed input ports to typed output ports
    // ----------------------------------------------------------------------

    //! Connect port to timeCaller[portNum]
    //!
    void set_timeCaller_OutputPort(
        NATIVE_INT_TYPE portNum, /*!< The port number*/
        Fw::InputTimePort *port /*!< The port*/
    );

    //! Connect port to cmdRegOut[portNum]
    //!
    void set_cmdRegOut_OutputPort(
        NATIVE_INT_TYPE portNum, /*!< The port number*/
        Fw::InputCmdRegPort *port /*!< The port*/
    );

    //! Connect port to eventOut[portNum]
    //!
    void set_eventOut_OutputPort(
        NATIVE_INT_TYPE portNum, /*!< The port number*/
        Fw::InputLogPort *port /*!< The port*/
    );

    //! Connect port to bufferGetCaller[portNum]
    //!
    void set_bufferGetCaller_OutputPort(
        NATIVE_INT_TYPE portNum, /*!< The port number*/
        Fw::InputBufferGetPort *port /*!< The port*/
    );

    //! Connect port to PingResponse[portNum]
    //!
    void set_PingResponse_OutputPort(
        NATIVE_INT_TYPE portNum, /*!< The port number*/
        Svc::InputPingPort *port /*!< The port*/
    );

    //! Connect port to bufferReturn[portNum]
    //!
    void set_bufferReturn_OutputPort(
        NATIVE_INT_TYPE portNum, /*!< The port number*/
        Fw::InputBufferSendPort *port /*!< The port*/
    );

    //! Connect port to cmdResponseOut[portNum]
    //!
    void set_cmdResponseOut_OutputPort(
        NATIVE_INT_TYPE portNum, /*!< The port number*/
        Fw::InputCmdResponsePort *port /*!< The port*/
    );

#if FW_ENABLE_TEXT_LOGGING == 1
    //! Connect port to LogText[portNum]
    //!
    void set_LogText_OutputPort(
        NATIVE_INT_TYPE portNum, /*!< The port number*/
        Fw::InputLogTextPort *port /*!< The port*/
    );
#endif

#if FW_PORT_SERIALIZATION

  public:

    // ----------------------------------------------------------------------
    // Connect serialization input ports to typed output ports
    // ----------------------------------------------------------------------

    //! Connect port to timeCaller[portNum]
    //!
    void set_timeCaller_OutputPort(
        NATIVE_INT_TYPE portNum, /*!< The port number*/
        Fw::InputSerializePort *port /*!< The port*/
    );

    //! Connect port to cmdRegOut[portNum]
    //!
    void set_cmdRegOut_OutputPort(
        NATIVE_INT_TYPE portNum, /*!< The port number*/
        Fw::InputSerializePort *port /*!< The port*/
    );

    //! Connect port to eventOut[portNum]
    //!
    void set_eventOut_OutputPort(
        NATIVE_INT_TYPE portNum, /*!< The port number*/
        Fw::InputSerializePort *port /*!< The port*/
    );

    //! Connect port to bufferGetCaller[portNum]
    //!
    void set_bufferGetCaller_OutputPort(
        NATIVE_INT_TYPE portNum, /*!< The port number*/
        Fw::InputSerializePort *port /*!< The port*/
    );

    //! Connect port to PingResponse[portNum]
    //!
    void set_PingResponse_OutputPort(
        NATIVE_INT_TYPE portNum, /*!< The port number*/
        Fw::InputSerializePort *port /*!< The port*/
    );

    //! Connect port to bufferReturn[portNum]
    //!
    void set_bufferReturn_OutputPort(
        NATIVE_INT_TYPE portNum, /*!< The port number*/
        Fw::InputSerializePort *port /*!< The port*/
    );

    //! Connect port to cmdResponseOut[portNum]
    //!
    void set_cmdResponseOut_OutputPort(
        NATIVE_INT_TYPE portNum, /*!< The port number*/
        Fw::InputSerializePort *port /*!< The port*/
    );

#if FW_ENABLE_TEXT_LOGGING == 1
    //! Connect port to LogText[portNum]
    //!
    void set_LogText_OutputPort(
        NATIVE_INT_TYPE portNum, /*!< The port number*/
        Fw::InputSerializePort *port /*!< The port*/
    );
#endif

#endif

  public:

    // ----------------------------------------------------------------------
    // Command registration
    // ----------------------------------------------------------------------

    //! \brief Register commands with the Command Dispatcher
    //!
    //! Connect the dispatcher first
    //!
    void regCommands(void);

  PROTECTED:

    // ----------------------------------------------------------------------
    // Component construction, initialization, and destruction
    // ----------------------------------------------------------------------

#if FW_OBJECT_NAMES == 1
    //! Construct a neascout_scienceComponentBase object
    //!
    neascout_scienceComponentBase(
        const char* compName /*!< Component name*/
    );
#else
    //! Construct a neascout_scienceComponentBase object
    //!
    neascout_scienceComponentBase(void);
#endif

    //! Initialize a neascout_scienceComponentBase object
    //!
    void init(
        NATIVE_INT_TYPE queueDepth, /*!< The queue depth*/
        NATIVE_INT_TYPE instance = 0 /*!< The instance number*/
    );

    //! Destroy a neascout_scienceComponentBase object
    //!
    virtual ~neascout_scienceComponentBase(void);

  PROTECTED:

    // ----------------------------------------------------------------------
    // Handlers to implement for typed input ports
    // ----------------------------------------------------------------------

    //! Handler for input port PingRecv
    //
    virtual void PingRecv_handler(
        NATIVE_INT_TYPE portNum, /*!< The port number*/
        U32 key /*!< Value to return to pinger*/
    ) = 0;

  PROTECTED:

    // ----------------------------------------------------------------------
    // Port handler base-class functions for typed input ports.
    // ----------------------------------------------------------------------
    // Call these functions directly to bypass the corresponding ports.
    // ----------------------------------------------------------------------

    //! Handler base-class function for input port PingRecv
    //!
    void PingRecv_handlerBase(
        NATIVE_INT_TYPE portNum, /*!< The port number*/
        U32 key /*!< Value to return to pinger*/
    );

  PROTECTED:

    // ----------------------------------------------------------------------
    // Pre-message hooks for typed async input ports.
    // ----------------------------------------------------------------------
    // Each of these functions is invoked just before processing a message
    // on the corresponding port. By default they do nothing. You can
    // override them to provide specific pre-message behavior.
    // ----------------------------------------------------------------------

    //! Pre-message hook for async input port PingRecv
    //!
    virtual void PingRecv_preMsgHook(
        NATIVE_INT_TYPE portNum, /*!< The port number*/
        U32 key /*!< Value to return to pinger*/
    );

  PROTECTED:

    // ----------------------------------------------------------------------
    // Invocation functions for typed output ports
    // ----------------------------------------------------------------------

    //! Invoke output port bufferGetCaller
    //!
    Fw::Buffer bufferGetCaller_out(
        NATIVE_INT_TYPE portNum, /*!< The port number*/
        U32 size 
    );

    //! Invoke output port PingResponse
    //!
    void PingResponse_out(
        NATIVE_INT_TYPE portNum, /*!< The port number*/
        U32 key /*!< Value to return to pinger*/
    );

    //! Invoke output port bufferReturn
    //!
    void bufferReturn_out(
        NATIVE_INT_TYPE portNum, /*!< The port number*/
        Fw::Buffer fwBuffer 
    );

  PROTECTED:

    // ----------------------------------------------------------------------
    // Getters for numbers of input ports
    // ----------------------------------------------------------------------

    //! Get the number of cmdIn input ports
    //!
    //! \return The number of cmdIn input ports
    //!
    NATIVE_INT_TYPE getNum_cmdIn_InputPorts(void);

    //! Get the number of PingRecv input ports
    //!
    //! \return The number of PingRecv input ports
    //!
    NATIVE_INT_TYPE getNum_PingRecv_InputPorts(void);

  
    // ----------------------------------------------------------------------
    // Enumerations for number of ports
    // ----------------------------------------------------------------------
    
    enum {
       NUM_CMDIN_INPUT_PORTS = 1,
       NUM_PINGRECV_INPUT_PORTS = 1,
    };
  
  PROTECTED:

    // ----------------------------------------------------------------------
    // Getters for numbers of output ports
    // ----------------------------------------------------------------------

    //! Get the number of timeCaller output ports
    //!
    //! \return The number of timeCaller output ports
    //!
    NATIVE_INT_TYPE getNum_timeCaller_OutputPorts(void);

    //! Get the number of cmdRegOut output ports
    //!
    //! \return The number of cmdRegOut output ports
    //!
    NATIVE_INT_TYPE getNum_cmdRegOut_OutputPorts(void);

    //! Get the number of eventOut output ports
    //!
    //! \return The number of eventOut output ports
    //!
    NATIVE_INT_TYPE getNum_eventOut_OutputPorts(void);

    //! Get the number of bufferGetCaller output ports
    //!
    //! \return The number of bufferGetCaller output ports
    //!
    NATIVE_INT_TYPE getNum_bufferGetCaller_OutputPorts(void);

    //! Get the number of PingResponse output ports
    //!
    //! \return The number of PingResponse output ports
    //!
    NATIVE_INT_TYPE getNum_PingResponse_OutputPorts(void);

    //! Get the number of bufferReturn output ports
    //!
    //! \return The number of bufferReturn output ports
    //!
    NATIVE_INT_TYPE getNum_bufferReturn_OutputPorts(void);

    //! Get the number of cmdResponseOut output ports
    //!
    //! \return The number of cmdResponseOut output ports
    //!
    NATIVE_INT_TYPE getNum_cmdResponseOut_OutputPorts(void);

#if FW_ENABLE_TEXT_LOGGING == 1
    //! Get the number of LogText output ports
    //!
    //! \return The number of LogText output ports
    //!
    NATIVE_INT_TYPE getNum_LogText_OutputPorts(void);
#endif

  
    enum {
       NUM_TIMECALLER_OUTPUT_PORTS = 1,
       NUM_CMDREGOUT_OUTPUT_PORTS = 1,
       NUM_EVENTOUT_OUTPUT_PORTS = 1,
       NUM_BUFFERGETCALLER_OUTPUT_PORTS = 1,
       NUM_PINGRESPONSE_OUTPUT_PORTS = 1,
       NUM_BUFFERRETURN_OUTPUT_PORTS = 1,
       NUM_CMDRESPONSEOUT_OUTPUT_PORTS = 1,
       NUM_LOGTEXT_OUTPUT_PORTS = 1,
    };
  
  PROTECTED:

    // ----------------------------------------------------------------------
    // Connection status queries for output ports
    // ----------------------------------------------------------------------

    //! Check whether port timeCaller is connected
    //!
    //! \return Whether port timeCaller is connected
    //!
    bool isConnected_timeCaller_OutputPort(
        NATIVE_INT_TYPE portNum /*!< The port number*/
    );

    //! Check whether port cmdRegOut is connected
    //!
    //! \return Whether port cmdRegOut is connected
    //!
    bool isConnected_cmdRegOut_OutputPort(
        NATIVE_INT_TYPE portNum /*!< The port number*/
    );

    //! Check whether port eventOut is connected
    //!
    //! \return Whether port eventOut is connected
    //!
    bool isConnected_eventOut_OutputPort(
        NATIVE_INT_TYPE portNum /*!< The port number*/
    );

    //! Check whether port bufferGetCaller is connected
    //!
    //! \return Whether port bufferGetCaller is connected
    //!
    bool isConnected_bufferGetCaller_OutputPort(
        NATIVE_INT_TYPE portNum /*!< The port number*/
    );

    //! Check whether port PingResponse is connected
    //!
    //! \return Whether port PingResponse is connected
    //!
    bool isConnected_PingResponse_OutputPort(
        NATIVE_INT_TYPE portNum /*!< The port number*/
    );

    //! Check whether port bufferReturn is connected
    //!
    //! \return Whether port bufferReturn is connected
    //!
    bool isConnected_bufferReturn_OutputPort(
        NATIVE_INT_TYPE portNum /*!< The port number*/
    );

    //! Check whether port cmdResponseOut is connected
    //!
    //! \return Whether port cmdResponseOut is connected
    //!
    bool isConnected_cmdResponseOut_OutputPort(
        NATIVE_INT_TYPE portNum /*!< The port number*/
    );

#if FW_ENABLE_TEXT_LOGGING == 1
    //! Check whether port LogText is connected
    //!
    //! \return Whether port LogText is connected
    //!
    bool isConnected_LogText_OutputPort(
        NATIVE_INT_TYPE portNum /*!< The port number*/
    );
#endif

  PROTECTED:

    // ----------------------------------------------------------------------
    // Command opcodes
    // ----------------------------------------------------------------------

    enum {
      OPCODE_NEAS_SCIENCE_CMD_L1 = 0xFF00, /* Run the L1 pipeline on an image */
      OPCODE_NEAS_SCIENCE_CMD_COADD = 0xFF01, /* Coalign and coadd many frames */
      OPCODE_NEAS_SCIENCE_CMD_CROP = 0xFF02, /* Crop an image and/or compress it with ICER */
      OPCODE_NEAS_SCIENCE_CMD_DETECT = 0xFF03, /* Blur two frames and subtract them. */
      OPCODE_NEAS_SCIENCE_CMD_IMGSTAT = 0xFF04, /* Calculate the statistics and integrity of an image, and possibly compare it to another. */
      OPCODE_NEAS_SCIENCE_CMD_MASK_UPDATE = 0xFF05, /* Update flight bad pixel mask */
    };

  PROTECTED:

    // ----------------------------------------------------------------------
    // Command handlers to implement
    // ----------------------------------------------------------------------

    //! Handler for command NEAS_SCIENCE_CMD_L1
    /* Run the L1 pipeline on an image */
    virtual void NEAS_SCIENCE_CMD_L1_cmdHandler(
        FwOpcodeType opCode, /*!< The opcode*/
        U32 cmdSeq, /*!< The command sequence number*/
        const Fw::CmdStringArg& rawFrame, /*!< The raw instrument data from the camera*/
        const Fw::CmdStringArg& outFrame, /*!< File path for radiometrically-calibrated output*/
        const Fw::CmdStringArg& ffFrame1, /*!< File path for flat field m_array file*/
        const Fw::CmdStringArg& ffFrame2, /*!< File path for flat field b_array file*/
        const Fw::CmdStringArg& badFrame, /*!< File path for bad pixel corection mask file*/
        const Fw::CmdStringArg& darkFrame, /*!< File path for dark current mask file*/
        U32 fixPt, /*!< Fixed point conversion value.  Nominally 1.*/
        U32 unpack_only, /*!< If >0 just unpack the data without applying calibration*/
        I16 temperature /*!< If -99 use the image header temperature. Otherwise, use the value specified for flat field interpolation*/
    ) = 0;

    //! Handler for command NEAS_SCIENCE_CMD_COADD
    /* Coalign and coadd many frames */
    virtual void NEAS_SCIENCE_CMD_COADD_cmdHandler(
        FwOpcodeType opCode, /*!< The opcode*/
        U32 cmdSeq, /*!< The command sequence number*/
        I16 nFrames, /*!< Number of frames to coalign*/
        const Fw::CmdStringArg& baseFrame, /*!< base file path for input frames*/
        const Fw::CmdStringArg& outFrame, /*!< output file path*/
        U32 numStars, /*!< number of stars to search for in coaddition.  Nominally 5-10*/
        U32 searchRadius, /*!< radius of search area of reference star shift (pixels).*/
        F64 scaling, /*!< Scaling value utilized in coaddition xcorr/shift calculation. Nominally 1.*/
        U32 fixPt, /*!< Fixed point conversion value.  Nominally 1.*/
        U32 calcShift /*!< 1 for consider shift, 0 for no shift*/
    ) = 0;

    //! Handler for command NEAS_SCIENCE_CMD_CROP
    /* Crop an image and/or compress it with ICER */
    virtual void NEAS_SCIENCE_CMD_CROP_cmdHandler(
        FwOpcodeType opCode, /*!< The opcode*/
        U32 cmdSeq, /*!< The command sequence number*/
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
    ) = 0;

    //! Handler for command NEAS_SCIENCE_CMD_DETECT
    /* Blur two frames and subtract them. */
    virtual void NEAS_SCIENCE_CMD_DETECT_cmdHandler(
        FwOpcodeType opCode, /*!< The opcode*/
        U32 cmdSeq, /*!< The command sequence number*/
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
    ) = 0;

    //! Handler for command NEAS_SCIENCE_CMD_IMGSTAT
    /* Calculate the statistics and integrity of an image, and possibly compare it to another. */
    virtual void NEAS_SCIENCE_CMD_IMGSTAT_cmdHandler(
        FwOpcodeType opCode, /*!< The opcode*/
        U32 cmdSeq, /*!< The command sequence number*/
        const Fw::CmdStringArg& frameA, /*!< primary frame to analyze*/
        const Fw::CmdStringArg& frameB, /*!< if length larger than 1, secondary frame for comparison*/
        I32 bin0HistMax, /*!< Defines bin 0 max value for ImgStat histogram.  See ICD.*/
        I32 bin1HistMax, /*!< Defines bin 1 max value for ImgStat histogram.  See ICD.*/
        I32 bin4HistMax, /*!< Defines bin 4 max value for ImgStat histogram.  See ICD.*/
        I32 bin5HistMax /*!< Defines bin 5 max value for ImgStat histogram.  See ICD.*/
    ) = 0;

    //! Handler for command NEAS_SCIENCE_CMD_MASK_UPDATE
    /* Update flight bad pixel mask */
    virtual void NEAS_SCIENCE_CMD_MASK_UPDATE_cmdHandler(
        FwOpcodeType opCode, /*!< The opcode*/
        U32 cmdSeq, /*!< The command sequence number*/
        const Fw::CmdStringArg& inFrame, /*!< original bad pixel mask*/
        const Fw::CmdStringArg& outFrame, /*!< result bad pixel mask*/
        I16 row, /*!< row to be adjusted (zero based)*/
        I16 col, /*!< col to be adjusted (zero based)*/
        I16 value /*!< new value. 0 for good pixel, 1 for bad.*/
    ) = 0;

  PROTECTED:

    // ----------------------------------------------------------------------
    // Pre-message hooks for async commands.
    // ----------------------------------------------------------------------
    // Each of these functions is invoked just before processing the
    // corresponding command. By default they do nothing. You can
    // override them to provide specific pre-command behavior.
    // ----------------------------------------------------------------------

    //! Pre-message hook for command NEAS_SCIENCE_CMD_L1
    //!
    virtual void NEAS_SCIENCE_CMD_L1_preMsgHook(
        FwOpcodeType opCode, /*!< The opcode*/
        U32 cmdSeq /*!< The command sequence number*/
    );

    //! Pre-message hook for command NEAS_SCIENCE_CMD_COADD
    //!
    virtual void NEAS_SCIENCE_CMD_COADD_preMsgHook(
        FwOpcodeType opCode, /*!< The opcode*/
        U32 cmdSeq /*!< The command sequence number*/
    );

    //! Pre-message hook for command NEAS_SCIENCE_CMD_CROP
    //!
    virtual void NEAS_SCIENCE_CMD_CROP_preMsgHook(
        FwOpcodeType opCode, /*!< The opcode*/
        U32 cmdSeq /*!< The command sequence number*/
    );

    //! Pre-message hook for command NEAS_SCIENCE_CMD_DETECT
    //!
    virtual void NEAS_SCIENCE_CMD_DETECT_preMsgHook(
        FwOpcodeType opCode, /*!< The opcode*/
        U32 cmdSeq /*!< The command sequence number*/
    );

    //! Pre-message hook for command NEAS_SCIENCE_CMD_IMGSTAT
    //!
    virtual void NEAS_SCIENCE_CMD_IMGSTAT_preMsgHook(
        FwOpcodeType opCode, /*!< The opcode*/
        U32 cmdSeq /*!< The command sequence number*/
    );

    //! Pre-message hook for command NEAS_SCIENCE_CMD_MASK_UPDATE
    //!
    virtual void NEAS_SCIENCE_CMD_MASK_UPDATE_preMsgHook(
        FwOpcodeType opCode, /*!< The opcode*/
        U32 cmdSeq /*!< The command sequence number*/
    );

  PROTECTED:

    // ----------------------------------------------------------------------
    // Command handler base-class functions.
    // Call these functions directly to bypass the command input port.
    // ----------------------------------------------------------------------

    //! Base-class handler function for command NEAS_SCIENCE_CMD_L1
    //! 
    void NEAS_SCIENCE_CMD_L1_cmdHandlerBase( 
        FwOpcodeType opCode, /*!< The opcode*/
        U32 cmdSeq, /*!< The command sequence number*/
        Fw::CmdArgBuffer &args /*!< The command argument buffer*/
    );

    //! Base-class handler function for command NEAS_SCIENCE_CMD_COADD
    //! 
    void NEAS_SCIENCE_CMD_COADD_cmdHandlerBase( 
        FwOpcodeType opCode, /*!< The opcode*/
        U32 cmdSeq, /*!< The command sequence number*/
        Fw::CmdArgBuffer &args /*!< The command argument buffer*/
    );

    //! Base-class handler function for command NEAS_SCIENCE_CMD_CROP
    //! 
    void NEAS_SCIENCE_CMD_CROP_cmdHandlerBase( 
        FwOpcodeType opCode, /*!< The opcode*/
        U32 cmdSeq, /*!< The command sequence number*/
        Fw::CmdArgBuffer &args /*!< The command argument buffer*/
    );

    //! Base-class handler function for command NEAS_SCIENCE_CMD_DETECT
    //! 
    void NEAS_SCIENCE_CMD_DETECT_cmdHandlerBase( 
        FwOpcodeType opCode, /*!< The opcode*/
        U32 cmdSeq, /*!< The command sequence number*/
        Fw::CmdArgBuffer &args /*!< The command argument buffer*/
    );

    //! Base-class handler function for command NEAS_SCIENCE_CMD_IMGSTAT
    //! 
    void NEAS_SCIENCE_CMD_IMGSTAT_cmdHandlerBase( 
        FwOpcodeType opCode, /*!< The opcode*/
        U32 cmdSeq, /*!< The command sequence number*/
        Fw::CmdArgBuffer &args /*!< The command argument buffer*/
    );

    //! Base-class handler function for command NEAS_SCIENCE_CMD_MASK_UPDATE
    //! 
    void NEAS_SCIENCE_CMD_MASK_UPDATE_cmdHandlerBase( 
        FwOpcodeType opCode, /*!< The opcode*/
        U32 cmdSeq, /*!< The command sequence number*/
        Fw::CmdArgBuffer &args /*!< The command argument buffer*/
    );

  PROTECTED:

    // ----------------------------------------------------------------------
    // Command response
    // ----------------------------------------------------------------------

    //! Emit command response
    //!
    void cmdResponse_out(
        FwOpcodeType opCode, /*!< The opcode*/
        U32 cmdSeq, /*!< The command sequence number*/
        Fw::CommandResponse response /*!< The command response*/
    );

  PROTECTED:

    // ----------------------------------------------------------------------
    // Event IDs
    // ----------------------------------------------------------------------

    enum {
      EVENTID_L1COMPLETE = 0xFF00, /* L1 Command Complete */
      EVENTID_CROPCOMPRESSCOMPLETE = 0xFF01, /* Crop/Compress Command Complete */
      EVENTID_COADDITIONCOMPLETE = 0xFF02, /* Coaddition Command Complete */
      EVENTID_DETECTIONCOMPLETE = 0xFF03, /* Detection Command Complete */
      EVENTID_IMGSTATCOMPLETE = 0xFF04, /* ImgStat Command Complete */
      EVENTID_ASP_ERROR = 0xFF05, /*  */
      EVENTID_ASP_LOG = 0xFF06, /*  */
      EVENTID_MASKUPDATECOMPLETE = 0xFF07, /* Mask Update Command Complete */
    };
    
  PROTECTED:

    // ----------------------------------------------------------------------
    // Event logging functions
    // ----------------------------------------------------------------------

    //! Log event L1Complete
    //!
    /* L1 Command Complete */
    void log_ACTIVITY_LO_L1Complete(
        void
    );
    

    //! Log event CropCompressComplete
    //!
    /* Crop/Compress Command Complete */
    void log_ACTIVITY_LO_CropCompressComplete(
        void
    );
    

    //! Log event CoadditionComplete
    //!
    /* Coaddition Command Complete */
    void log_ACTIVITY_LO_CoadditionComplete(
        void
    );
    

    //! Log event DetectionComplete
    //!
    /* Detection Command Complete */
    void log_ACTIVITY_LO_DetectionComplete(
        void
    );
    

    //! Log event ImgStatComplete
    //!
    /* ImgStat Command Complete */
    void log_ACTIVITY_LO_ImgStatComplete(
        void
    );
    

    //! Log event asp_error
    //!
    void log_WARNING_LO_asp_error(
        Fw::LogStringArg& error_code 
    );
    

    //! Log event asp_log
    //!
    void log_ACTIVITY_LO_asp_log(
        Fw::LogStringArg& info_string 
    );
    

    //! Log event MaskUpdateComplete
    //!
    /* Mask Update Command Complete */
    void log_ACTIVITY_LO_MaskUpdateComplete(
        void
    );
    

  PROTECTED:

    // ----------------------------------------------------------------------
    // Time
    // ----------------------------------------------------------------------

    //! Get the time
    //!
    //! \return The current time
    //!
    Fw::Time getTime(void);


  PRIVATE:

    // ----------------------------------------------------------------------
    // Typed input ports
    // ----------------------------------------------------------------------

    //! Input port cmdIn
    //!
    Fw::InputCmdPort m_cmdIn_InputPort[NUM_CMDIN_INPUT_PORTS];

    //! Input port PingRecv
    //!
    Svc::InputPingPort m_PingRecv_InputPort[NUM_PINGRECV_INPUT_PORTS];

  PRIVATE:

    // ----------------------------------------------------------------------
    // Typed output ports
    // ----------------------------------------------------------------------

    //! Output port timeCaller
    //!
    Fw::OutputTimePort m_timeCaller_OutputPort[NUM_TIMECALLER_OUTPUT_PORTS];

    //! Output port cmdRegOut
    //!
    Fw::OutputCmdRegPort m_cmdRegOut_OutputPort[NUM_CMDREGOUT_OUTPUT_PORTS];

    //! Output port eventOut
    //!
    Fw::OutputLogPort m_eventOut_OutputPort[NUM_EVENTOUT_OUTPUT_PORTS];

    //! Output port bufferGetCaller
    //!
    Fw::OutputBufferGetPort m_bufferGetCaller_OutputPort[NUM_BUFFERGETCALLER_OUTPUT_PORTS];

    //! Output port PingResponse
    //!
    Svc::OutputPingPort m_PingResponse_OutputPort[NUM_PINGRESPONSE_OUTPUT_PORTS];

    //! Output port bufferReturn
    //!
    Fw::OutputBufferSendPort m_bufferReturn_OutputPort[NUM_BUFFERRETURN_OUTPUT_PORTS];

    //! Output port cmdResponseOut
    //!
    Fw::OutputCmdResponsePort m_cmdResponseOut_OutputPort[NUM_CMDRESPONSEOUT_OUTPUT_PORTS];

#if FW_ENABLE_TEXT_LOGGING == 1
    //! Output port LogText
    //!
    Fw::OutputLogTextPort m_LogText_OutputPort[NUM_LOGTEXT_OUTPUT_PORTS];
#endif

  PRIVATE:

    // ----------------------------------------------------------------------
    // Calls for messages received on typed input ports
    // ----------------------------------------------------------------------

    //! Callback for port cmdIn
    //!
    static void m_p_cmdIn_in(
        Fw::PassiveComponentBase* callComp, /*!< The component instance*/
        NATIVE_INT_TYPE portNum, /*!< The port number*/
        FwOpcodeType opCode, /*!< Command Op Code*/
        U32 cmdSeq, /*!< Command Sequence*/
        Fw::CmdArgBuffer &args /*!< Buffer containing arguments*/
    );

    //! Callback for port PingRecv
    //!
    static void m_p_PingRecv_in(
        Fw::PassiveComponentBase* callComp, /*!< The component instance*/
        NATIVE_INT_TYPE portNum, /*!< The port number*/
        U32 key /*!< Value to return to pinger*/
    );

  PRIVATE:

    // ----------------------------------------------------------------------
    // Message dispatch functions
    // ----------------------------------------------------------------------

    //! Called in the message loop to dispatch a message from the queue
    //!
    virtual MsgDispatchStatus doDispatch(void);


  PRIVATE:
    // ----------------------------------------------------------------------
    // Counter values for event throttling
    // ----------------------------------------------------------------------

  };  

} // end namespace Neasc
#endif
