// ======================================================================
// \title  neascout_science/test/ut/TesterBase.hpp
// \author Auto-generated
// \brief  hpp file for neascout_science component test harness base class
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

#ifndef neascout_science_TESTER_BASE_HPP
#define neascout_science_TESTER_BASE_HPP

#include <LfNeasc/Neasc/neascout_science/neascout_scienceComponentAc.hpp>
#include <Fw/Types/Assert.hpp>
#include <Fw/Comp/PassiveComponentBase.hpp>
#include <stdio.h>
#include <Fw/Port/InputSerializePort.hpp>

namespace Neasc {

  //! \class neascout_scienceTesterBase
  //! \brief Auto-generated base class for neascout_science component test harness
  //!
  class neascout_scienceTesterBase :
    public Fw::PassiveComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Initialization
      // ----------------------------------------------------------------------

      //! Initialize object neascout_scienceTesterBase
      //!
      virtual void init(
          const NATIVE_INT_TYPE instance = 0 /*!< The instance number*/
      );

    public:

      // ----------------------------------------------------------------------
      // Connectors for 'to' ports
      // Connect these output ports to the input ports under test
      // ----------------------------------------------------------------------

      //! Connect cmdIn to to_cmdIn[portNum]
      //!
      void connect_to_cmdIn(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          Fw::InputCmdPort *const cmdIn /*!< The port*/
      );

      //! Connect PingRecv to to_PingRecv[portNum]
      //!
      void connect_to_PingRecv(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          Svc::InputPingPort *const PingRecv /*!< The port*/
      );

    public:

      // ----------------------------------------------------------------------
      // Getters for 'from' ports
      // Connect these input ports to the output ports under test
      // ----------------------------------------------------------------------

      //! Get the port that receives input from timeCaller
      //!
      //! \return from_timeCaller[portNum]
      //!
      Fw::InputTimePort* get_from_timeCaller(
          const NATIVE_INT_TYPE portNum /*!< The port number*/
      );

      //! Get the port that receives input from cmdRegOut
      //!
      //! \return from_cmdRegOut[portNum]
      //!
      Fw::InputCmdRegPort* get_from_cmdRegOut(
          const NATIVE_INT_TYPE portNum /*!< The port number*/
      );

      //! Get the port that receives input from eventOut
      //!
      //! \return from_eventOut[portNum]
      //!
      Fw::InputLogPort* get_from_eventOut(
          const NATIVE_INT_TYPE portNum /*!< The port number*/
      );

      //! Get the port that receives input from bufferGetCaller
      //!
      //! \return from_bufferGetCaller[portNum]
      //!
      Fw::InputBufferGetPort* get_from_bufferGetCaller(
          const NATIVE_INT_TYPE portNum /*!< The port number*/
      );

      //! Get the port that receives input from PingResponse
      //!
      //! \return from_PingResponse[portNum]
      //!
      Svc::InputPingPort* get_from_PingResponse(
          const NATIVE_INT_TYPE portNum /*!< The port number*/
      );

      //! Get the port that receives input from bufferReturn
      //!
      //! \return from_bufferReturn[portNum]
      //!
      Fw::InputBufferSendPort* get_from_bufferReturn(
          const NATIVE_INT_TYPE portNum /*!< The port number*/
      );

      //! Get the port that receives input from cmdResponseOut
      //!
      //! \return from_cmdResponseOut[portNum]
      //!
      Fw::InputCmdResponsePort* get_from_cmdResponseOut(
          const NATIVE_INT_TYPE portNum /*!< The port number*/
      );

#if FW_ENABLE_TEXT_LOGGING == 1
      //! Get the port that receives input from LogText
      //!
      //! \return from_LogText[portNum]
      //!
      Fw::InputLogTextPort* get_from_LogText(
          const NATIVE_INT_TYPE portNum /*!< The port number*/
      );
#endif

    protected:

      // ----------------------------------------------------------------------
      // Construction and destruction
      // ----------------------------------------------------------------------

      //! Construct object neascout_scienceTesterBase
      //!
      neascout_scienceTesterBase(
#if FW_OBJECT_NAMES == 1
          const char *const compName, /*!< The component name*/
          const U32 maxHistorySize /*!< The maximum size of each history*/
#else
          const U32 maxHistorySize /*!< The maximum size of each history*/
#endif
      );

      //! Destroy object neascout_scienceTesterBase
      //!
      virtual ~neascout_scienceTesterBase(void);

      // ----------------------------------------------------------------------
      // Test history
      // ----------------------------------------------------------------------

    protected:

      //! \class History
      //! \brief A history of port inputs
      //!
      template <typename T> class History {

        public:

          //! Create a History
          //!
          History(
              const U32 maxSize /*!< The maximum history size*/
          ) : 
              numEntries(0), 
              maxSize(maxSize) 
          { 
            this->entries = new T[maxSize];
          }

          //! Destroy a History
          //!
          ~History() {
            delete[] this->entries;
          }

          //! Clear the history
          //!
          void clear() { this->numEntries = 0; }

          //! Push an item onto the history
          //!
          void push_back(
              T entry /*!< The item*/
          ) {
            FW_ASSERT(this->numEntries < this->maxSize);
            entries[this->numEntries++] = entry;
          }

          //! Get an item at an index
          //!
          //! \return The item at index i
          //!
          T at(
              const U32 i /*!< The index*/
          ) const {
            FW_ASSERT(i < this->numEntries);
            return entries[i];
          }

          //! Get the number of entries in the history
          //!
          //! \return The number of entries in the history
          //!
          U32 size(void) const { return this->numEntries; }

        private:

          //! The number of entries in the history
          //!
          U32 numEntries;

          //! The maximum history size
          //!
          const U32 maxSize;

          //! The entries
          //!
          T *entries;

      };

      //! Clear all history
      //!
      void clearHistory(void);

    protected:

      // ----------------------------------------------------------------------
      // Handler prototypes for typed from ports
      // ----------------------------------------------------------------------

      //! Handler prototype for from_bufferGetCaller
      //!
      virtual Fw::Buffer from_bufferGetCaller_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          U32 size 
      ) = 0;

      //! Handler base function for from_bufferGetCaller
      //!
      Fw::Buffer from_bufferGetCaller_handlerBase(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          U32 size 
      );

      //! Handler prototype for from_PingResponse
      //!
      virtual void from_PingResponse_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          U32 key /*!< Value to return to pinger*/
      ) = 0;

      //! Handler base function for from_PingResponse
      //!
      void from_PingResponse_handlerBase(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          U32 key /*!< Value to return to pinger*/
      );

      //! Handler prototype for from_bufferReturn
      //!
      virtual void from_bufferReturn_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          Fw::Buffer fwBuffer 
      ) = 0;

      //! Handler base function for from_bufferReturn
      //!
      void from_bufferReturn_handlerBase(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          Fw::Buffer fwBuffer 
      );

    protected:

      // ----------------------------------------------------------------------
      // Histories for typed from ports
      // ----------------------------------------------------------------------

      //! Clear from port history
      //!
      void clearFromPortHistory(void);

      //! The total number of from port entries
      //!
      U32 fromPortHistorySize;

      //! Push an entry on the history for from_bufferGetCaller
      void pushFromPortEntry_bufferGetCaller(
          U32 size 
      );

      //! A history entry for from_bufferGetCaller
      //!
      typedef struct {
        U32 size;
      } FromPortEntry_bufferGetCaller;

      //! The history for from_bufferGetCaller
      //!
      History<FromPortEntry_bufferGetCaller> 
        *fromPortHistory_bufferGetCaller;

      //! Push an entry on the history for from_PingResponse
      void pushFromPortEntry_PingResponse(
          U32 key /*!< Value to return to pinger*/
      );

      //! A history entry for from_PingResponse
      //!
      typedef struct {
        U32 key;
      } FromPortEntry_PingResponse;

      //! The history for from_PingResponse
      //!
      History<FromPortEntry_PingResponse> 
        *fromPortHistory_PingResponse;

      //! Push an entry on the history for from_bufferReturn
      void pushFromPortEntry_bufferReturn(
          Fw::Buffer fwBuffer 
      );

      //! A history entry for from_bufferReturn
      //!
      typedef struct {
        Fw::Buffer fwBuffer;
      } FromPortEntry_bufferReturn;

      //! The history for from_bufferReturn
      //!
      History<FromPortEntry_bufferReturn> 
        *fromPortHistory_bufferReturn;

    protected:

      // ----------------------------------------------------------------------
      // Invocation functions for to ports
      // ----------------------------------------------------------------------

      //! Invoke the to port connected to PingRecv
      //!
      void invoke_to_PingRecv(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          U32 key /*!< Value to return to pinger*/
      );

    public:

      // ----------------------------------------------------------------------
      // Getters for port counts
      // ----------------------------------------------------------------------

      //! Get the number of from_timeCaller ports
      //!
      //! \return The number of from_timeCaller ports
      //!
      NATIVE_INT_TYPE getNum_from_timeCaller(void) const;

      //! Get the number of from_cmdRegOut ports
      //!
      //! \return The number of from_cmdRegOut ports
      //!
      NATIVE_INT_TYPE getNum_from_cmdRegOut(void) const;

      //! Get the number of from_eventOut ports
      //!
      //! \return The number of from_eventOut ports
      //!
      NATIVE_INT_TYPE getNum_from_eventOut(void) const;

      //! Get the number of from_bufferGetCaller ports
      //!
      //! \return The number of from_bufferGetCaller ports
      //!
      NATIVE_INT_TYPE getNum_from_bufferGetCaller(void) const;

      //! Get the number of to_cmdIn ports
      //!
      //! \return The number of to_cmdIn ports
      //!
      NATIVE_INT_TYPE getNum_to_cmdIn(void) const;

      //! Get the number of from_PingResponse ports
      //!
      //! \return The number of from_PingResponse ports
      //!
      NATIVE_INT_TYPE getNum_from_PingResponse(void) const;

      //! Get the number of to_PingRecv ports
      //!
      //! \return The number of to_PingRecv ports
      //!
      NATIVE_INT_TYPE getNum_to_PingRecv(void) const;

      //! Get the number of from_bufferReturn ports
      //!
      //! \return The number of from_bufferReturn ports
      //!
      NATIVE_INT_TYPE getNum_from_bufferReturn(void) const;

      //! Get the number of from_cmdResponseOut ports
      //!
      //! \return The number of from_cmdResponseOut ports
      //!
      NATIVE_INT_TYPE getNum_from_cmdResponseOut(void) const;

#if FW_ENABLE_TEXT_LOGGING == 1
      //! Get the number of from_LogText ports
      //!
      //! \return The number of from_LogText ports
      //!
      NATIVE_INT_TYPE getNum_from_LogText(void) const;
#endif

    protected:

      // ----------------------------------------------------------------------
      // Connection status for to ports
      // ----------------------------------------------------------------------

      //! Check whether port is connected
      //!
      //! Whether to_cmdIn[portNum] is connected
      //!
      bool isConnected_to_cmdIn(
          const NATIVE_INT_TYPE portNum /*!< The port number*/
      );

      //! Check whether port is connected
      //!
      //! Whether to_PingRecv[portNum] is connected
      //!
      bool isConnected_to_PingRecv(
          const NATIVE_INT_TYPE portNum /*!< The port number*/
      );

      // ----------------------------------------------------------------------
      // Functions for sending commands
      // ----------------------------------------------------------------------

    protected:
    
      // send command buffers directly - used for intentional command encoding errors
      void sendRawCmd(FwOpcodeType opcode, U32 cmdSeq, Fw::CmdArgBuffer& args); 

      //! Send a NEAS_SCIENCE_CMD_L1 command
      //!
      void sendCmd_NEAS_SCIENCE_CMD_L1(
          const NATIVE_INT_TYPE instance, /*!< The instance number*/
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

      //! Send a NEAS_SCIENCE_CMD_COADD command
      //!
      void sendCmd_NEAS_SCIENCE_CMD_COADD(
          const NATIVE_INT_TYPE instance, /*!< The instance number*/
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

      //! Send a NEAS_SCIENCE_CMD_CROP command
      //!
      void sendCmd_NEAS_SCIENCE_CMD_CROP(
          const NATIVE_INT_TYPE instance, /*!< The instance number*/
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

      //! Send a NEAS_SCIENCE_CMD_DETECT command
      //!
      void sendCmd_NEAS_SCIENCE_CMD_DETECT(
          const NATIVE_INT_TYPE instance, /*!< The instance number*/
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

      //! Send a NEAS_SCIENCE_CMD_IMGSTAT command
      //!
      void sendCmd_NEAS_SCIENCE_CMD_IMGSTAT(
          const NATIVE_INT_TYPE instance, /*!< The instance number*/
          const U32 cmdSeq, /*!< The command sequence number*/
          const Fw::CmdStringArg& frameA, /*!< primary frame to analyze*/
          const Fw::CmdStringArg& frameB, /*!< if length larger than 1, secondary frame for comparison*/
          I32 bin0HistMax, /*!< Defines bin 0 max value for ImgStat histogram.  See ICD.*/
          I32 bin1HistMax, /*!< Defines bin 1 max value for ImgStat histogram.  See ICD.*/
          I32 bin4HistMax, /*!< Defines bin 4 max value for ImgStat histogram.  See ICD.*/
          I32 bin5HistMax /*!< Defines bin 5 max value for ImgStat histogram.  See ICD.*/
      );

      //! Send a NEAS_SCIENCE_CMD_MASK_UPDATE command
      //!
      void sendCmd_NEAS_SCIENCE_CMD_MASK_UPDATE(
          const NATIVE_INT_TYPE instance, /*!< The instance number*/
          const U32 cmdSeq, /*!< The command sequence number*/
          const Fw::CmdStringArg& inFrame, /*!< original bad pixel mask*/
          const Fw::CmdStringArg& outFrame, /*!< result bad pixel mask*/
          I16 row, /*!< row to be adjusted (zero based)*/
          I16 col, /*!< col to be adjusted (zero based)*/
          I16 value /*!< new value. 0 for good pixel, 1 for bad.*/
      );

    protected:

      // ----------------------------------------------------------------------
      // Command response handling
      // ----------------------------------------------------------------------

      //! Handle a command response
      //!
      virtual void cmdResponseIn(
          const FwOpcodeType opCode, /*!< The opcode*/
          const U32 cmdSeq, /*!< The command sequence number*/
          const Fw::CommandResponse response /*!< The command response*/
      );

      //! A type representing a command response
      //!
      typedef struct {
        FwOpcodeType opCode;
        U32 cmdSeq;
        Fw::CommandResponse response;
      } CmdResponse;

      //! The command response history
      //!
      History<CmdResponse> *cmdResponseHistory;

    protected:

      // ----------------------------------------------------------------------
      // Event dispatch
      // ----------------------------------------------------------------------

      //! Dispatch an event
      //!
      void dispatchEvents(
          const FwEventIdType id, /*!< The event ID*/
          Fw::Time& timeTag, /*!< The time*/
          const Fw::LogSeverity severity, /*!< The severity*/
          Fw::LogBuffer& args /*!< The serialized arguments*/
      );

      //! Clear event history
      //!
      void clearEvents(void);

      //! The total number of events seen
      //!
      U32 eventsSize;

#if FW_ENABLE_TEXT_LOGGING

    protected:

      // ----------------------------------------------------------------------
      // Text events
      // ----------------------------------------------------------------------

      //! Handle a text event
      //!
      virtual void textLogIn(
          const FwEventIdType id, /*!< The event ID*/
          Fw::Time& timeTag, /*!< The time*/
          const Fw::TextLogSeverity severity, /*!< The severity*/
          const Fw::TextLogString& text /*!< The event string*/
      );

      //! A history entry for the text log
      //!
      typedef struct {
        U32 id;
        Fw::Time timeTag;
        Fw::TextLogSeverity severity;
        Fw::TextLogString text;
      } TextLogEntry;

      //! The history of text log events
      //!
      History<TextLogEntry> *textLogHistory;

      //! Print a text log history entry
      //!
      static void printTextLogHistoryEntry(
          const TextLogEntry& e,
          FILE* file
      );

      //! Print the text log history
      //!
      void printTextLogHistory(FILE *const file);

#endif

    protected:

      // ----------------------------------------------------------------------
      // Event: L1Complete
      // ----------------------------------------------------------------------

      //! Handle event L1Complete
      //!
      virtual void logIn_ACTIVITY_LO_L1Complete(
          void
      );

      //! Size of history for event L1Complete
      //!
      U32 eventsSize_L1Complete;

    protected:

      // ----------------------------------------------------------------------
      // Event: CropCompressComplete
      // ----------------------------------------------------------------------

      //! Handle event CropCompressComplete
      //!
      virtual void logIn_ACTIVITY_LO_CropCompressComplete(
          void
      );

      //! Size of history for event CropCompressComplete
      //!
      U32 eventsSize_CropCompressComplete;

    protected:

      // ----------------------------------------------------------------------
      // Event: CoadditionComplete
      // ----------------------------------------------------------------------

      //! Handle event CoadditionComplete
      //!
      virtual void logIn_ACTIVITY_LO_CoadditionComplete(
          void
      );

      //! Size of history for event CoadditionComplete
      //!
      U32 eventsSize_CoadditionComplete;

    protected:

      // ----------------------------------------------------------------------
      // Event: DetectionComplete
      // ----------------------------------------------------------------------

      //! Handle event DetectionComplete
      //!
      virtual void logIn_ACTIVITY_LO_DetectionComplete(
          void
      );

      //! Size of history for event DetectionComplete
      //!
      U32 eventsSize_DetectionComplete;

    protected:

      // ----------------------------------------------------------------------
      // Event: ImgStatComplete
      // ----------------------------------------------------------------------

      //! Handle event ImgStatComplete
      //!
      virtual void logIn_ACTIVITY_LO_ImgStatComplete(
          void
      );

      //! Size of history for event ImgStatComplete
      //!
      U32 eventsSize_ImgStatComplete;

    protected:

      // ----------------------------------------------------------------------
      // Event: asp_error
      // ----------------------------------------------------------------------

      //! Handle event asp_error
      //!
      virtual void logIn_WARNING_LO_asp_error(
          Fw::LogStringArg& error_code 
      );

      //! A history entry for event asp_error
      //!
      typedef struct {
        Fw::LogStringArg error_code;
      } EventEntry_asp_error;

      //! The history of asp_error events
      //!
      History<EventEntry_asp_error> 
        *eventHistory_asp_error;

    protected:

      // ----------------------------------------------------------------------
      // Event: asp_log
      // ----------------------------------------------------------------------

      //! Handle event asp_log
      //!
      virtual void logIn_ACTIVITY_LO_asp_log(
          Fw::LogStringArg& info_string 
      );

      //! A history entry for event asp_log
      //!
      typedef struct {
        Fw::LogStringArg info_string;
      } EventEntry_asp_log;

      //! The history of asp_log events
      //!
      History<EventEntry_asp_log> 
        *eventHistory_asp_log;

    protected:

      // ----------------------------------------------------------------------
      // Event: MaskUpdateComplete
      // ----------------------------------------------------------------------

      //! Handle event MaskUpdateComplete
      //!
      virtual void logIn_ACTIVITY_LO_MaskUpdateComplete(
          void
      );

      //! Size of history for event MaskUpdateComplete
      //!
      U32 eventsSize_MaskUpdateComplete;

    protected:

      // ----------------------------------------------------------------------
      // Test time
      // ----------------------------------------------------------------------

      //! Set the test time for events and telemetry
      //!
      void setTestTime(
          const Fw::Time& timeTag /*!< The time*/
      );

    private:

      // ----------------------------------------------------------------------
      // To ports
      // ----------------------------------------------------------------------

      //! To port connected to cmdIn
      //!
      Fw::OutputCmdPort m_to_cmdIn[1];

      //! To port connected to PingRecv
      //!
      Svc::OutputPingPort m_to_PingRecv[1];

    private:

      // ----------------------------------------------------------------------
      // From ports
      // ----------------------------------------------------------------------

      //! From port connected to timeCaller
      //!
      Fw::InputTimePort m_from_timeCaller[1];

      //! From port connected to cmdRegOut
      //!
      Fw::InputCmdRegPort m_from_cmdRegOut[1];

      //! From port connected to eventOut
      //!
      Fw::InputLogPort m_from_eventOut[1];

      //! From port connected to bufferGetCaller
      //!
      Fw::InputBufferGetPort m_from_bufferGetCaller[1];

      //! From port connected to PingResponse
      //!
      Svc::InputPingPort m_from_PingResponse[1];

      //! From port connected to bufferReturn
      //!
      Fw::InputBufferSendPort m_from_bufferReturn[1];

      //! From port connected to cmdResponseOut
      //!
      Fw::InputCmdResponsePort m_from_cmdResponseOut[1];

#if FW_ENABLE_TEXT_LOGGING == 1
      //! From port connected to LogText
      //!
      Fw::InputLogTextPort m_from_LogText[1];
#endif

    private:

      // ----------------------------------------------------------------------
      // Static functions for output ports
      // ----------------------------------------------------------------------

      //! Static function for port from_timeCaller
      //!
      static void from_timeCaller_static(
          Fw::PassiveComponentBase *const callComp, /*!< The component instance*/
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          Fw::Time &time /*!< The U32 cmd argument*/
      );

      //! Static function for port from_cmdRegOut
      //!
      static void from_cmdRegOut_static(
          Fw::PassiveComponentBase *const callComp, /*!< The component instance*/
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          FwOpcodeType opCode /*!< Command Op Code*/
      );

      //! Static function for port from_eventOut
      //!
      static void from_eventOut_static(
          Fw::PassiveComponentBase *const callComp, /*!< The component instance*/
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          FwEventIdType id, /*!< Log ID*/
          Fw::Time &timeTag, /*!< Time Tag*/
          Fw::LogSeverity severity, /*!< The severity argument*/
          Fw::LogBuffer &args /*!< Buffer containing serialized log entry*/
      );

      //! Static function for port from_bufferGetCaller
      //!
      static Fw::Buffer from_bufferGetCaller_static(
          Fw::PassiveComponentBase *const callComp, /*!< The component instance*/
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          U32 size 
      );

      //! Static function for port from_PingResponse
      //!
      static void from_PingResponse_static(
          Fw::PassiveComponentBase *const callComp, /*!< The component instance*/
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          U32 key /*!< Value to return to pinger*/
      );

      //! Static function for port from_bufferReturn
      //!
      static void from_bufferReturn_static(
          Fw::PassiveComponentBase *const callComp, /*!< The component instance*/
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          Fw::Buffer fwBuffer 
      );

      //! Static function for port from_cmdResponseOut
      //!
      static void from_cmdResponseOut_static(
          Fw::PassiveComponentBase *const callComp, /*!< The component instance*/
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          FwOpcodeType opCode, /*!< Command Op Code*/
          U32 cmdSeq, /*!< Command Sequence*/
          Fw::CommandResponse response /*!< The command response argument*/
      );

#if FW_ENABLE_TEXT_LOGGING == 1
      //! Static function for port from_LogText
      //!
      static void from_LogText_static(
          Fw::PassiveComponentBase *const callComp, /*!< The component instance*/
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          FwEventIdType id, /*!< Log ID*/
          Fw::Time &timeTag, /*!< Time Tag*/
          Fw::TextLogSeverity severity, /*!< The severity argument*/
          Fw::TextLogString &text /*!< Text of log message*/
      );
#endif

    private:

      // ----------------------------------------------------------------------
      // Test time
      // ----------------------------------------------------------------------

      //! Test time stamp
      //!
      Fw::Time m_testTime;

  };

} // end namespace Neasc

#endif
