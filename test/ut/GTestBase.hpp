// ======================================================================
// \title  neascout_science/test/ut/GTestBase.hpp
// \author Auto-generated
// \brief  hpp file for neascout_science component Google Test harness base class
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

#ifndef neascout_science_GTEST_BASE_HPP
#define neascout_science_GTEST_BASE_HPP

#include "TesterBase.hpp"
#include "gtest/gtest.h"

// ----------------------------------------------------------------------
// Macros for command history assertions
// ----------------------------------------------------------------------

#define ASSERT_CMD_RESPONSE_SIZE(size) \
  this->assertCmdResponse_size(__FILE__, __LINE__, size)

#define ASSERT_CMD_RESPONSE(index, opCode, cmdSeq, response) \
  this->assertCmdResponse(__FILE__, __LINE__, index, opCode, cmdSeq, response)

// ----------------------------------------------------------------------
// Macros for event history assertions 
// ----------------------------------------------------------------------

#define ASSERT_EVENTS_SIZE(size) \
  this->assertEvents_size(__FILE__, __LINE__, size)

#define ASSERT_EVENTS_L1Complete_SIZE(size) \
  this->assertEvents_L1Complete_size(__FILE__, __LINE__, size)

#define ASSERT_EVENTS_CropCompressComplete_SIZE(size) \
  this->assertEvents_CropCompressComplete_size(__FILE__, __LINE__, size)

#define ASSERT_EVENTS_CoadditionComplete_SIZE(size) \
  this->assertEvents_CoadditionComplete_size(__FILE__, __LINE__, size)

#define ASSERT_EVENTS_DetectionComplete_SIZE(size) \
  this->assertEvents_DetectionComplete_size(__FILE__, __LINE__, size)

#define ASSERT_EVENTS_ImgStatComplete_SIZE(size) \
  this->assertEvents_ImgStatComplete_size(__FILE__, __LINE__, size)

#define ASSERT_EVENTS_asp_error_SIZE(size) \
  this->assertEvents_asp_error_size(__FILE__, __LINE__, size)

#define ASSERT_EVENTS_asp_error(index, _error_code) \
  this->assertEvents_asp_error(__FILE__, __LINE__, index, _error_code)

#define ASSERT_EVENTS_asp_log_SIZE(size) \
  this->assertEvents_asp_log_size(__FILE__, __LINE__, size)

#define ASSERT_EVENTS_asp_log(index, _info_string) \
  this->assertEvents_asp_log(__FILE__, __LINE__, index, _info_string)

#define ASSERT_EVENTS_MaskUpdateComplete_SIZE(size) \
  this->assertEvents_MaskUpdateComplete_size(__FILE__, __LINE__, size)

// ----------------------------------------------------------------------
// Macros for typed user from port history assertions
// ----------------------------------------------------------------------

#define ASSERT_FROM_PORT_HISTORY_SIZE(size) \
  this->assertFromPortHistory_size(__FILE__, __LINE__, size)

#define ASSERT_from_bufferGetCaller_SIZE(size) \
  this->assert_from_bufferGetCaller_size(__FILE__, __LINE__, size)

#define ASSERT_from_bufferGetCaller(index, _size) \
  { \
    ASSERT_GT(this->fromPortHistory_bufferGetCaller->size(), static_cast<U32>(index)) \
    << "\n" \
    << "  File:     " << __FILE__ << "\n" \
    << "  Line:     " << __LINE__ << "\n" \
    << "  Value:    Index into history of from_bufferGetCaller\n" \
    << "  Expected: Less than size of history (" \
    << this->fromPortHistory_bufferGetCaller->size() << ")\n" \
    << "  Actual:   " << index << "\n"; \
    const FromPortEntry_bufferGetCaller& _e = \
      this->fromPortHistory_bufferGetCaller->at(index); \
    ASSERT_EQ(_size, _e.size) \
    << "\n" \
    << "  File:     " << __FILE__ << "\n" \
    << "  Line:     " << __LINE__ << "\n" \
    << "  Value:    Value of argument size at index " \
    << index \
    << " in history of from_bufferGetCaller\n" \
    << "  Expected: " << _size << "\n" \
    << "  Actual:   " << _e.size << "\n"; \
  }

#define ASSERT_from_PingResponse_SIZE(size) \
  this->assert_from_PingResponse_size(__FILE__, __LINE__, size)

#define ASSERT_from_PingResponse(index, _key) \
  { \
    ASSERT_GT(this->fromPortHistory_PingResponse->size(), static_cast<U32>(index)) \
    << "\n" \
    << "  File:     " << __FILE__ << "\n" \
    << "  Line:     " << __LINE__ << "\n" \
    << "  Value:    Index into history of from_PingResponse\n" \
    << "  Expected: Less than size of history (" \
    << this->fromPortHistory_PingResponse->size() << ")\n" \
    << "  Actual:   " << index << "\n"; \
    const FromPortEntry_PingResponse& _e = \
      this->fromPortHistory_PingResponse->at(index); \
    ASSERT_EQ(_key, _e.key) \
    << "\n" \
    << "  File:     " << __FILE__ << "\n" \
    << "  Line:     " << __LINE__ << "\n" \
    << "  Value:    Value of argument key at index " \
    << index \
    << " in history of from_PingResponse\n" \
    << "  Expected: " << _key << "\n" \
    << "  Actual:   " << _e.key << "\n"; \
  }

#define ASSERT_from_bufferReturn_SIZE(size) \
  this->assert_from_bufferReturn_size(__FILE__, __LINE__, size)

#define ASSERT_from_bufferReturn(index, _fwBuffer) \
  { \
    ASSERT_GT(this->fromPortHistory_bufferReturn->size(), static_cast<U32>(index)) \
    << "\n" \
    << "  File:     " << __FILE__ << "\n" \
    << "  Line:     " << __LINE__ << "\n" \
    << "  Value:    Index into history of from_bufferReturn\n" \
    << "  Expected: Less than size of history (" \
    << this->fromPortHistory_bufferReturn->size() << ")\n" \
    << "  Actual:   " << index << "\n"; \
    const FromPortEntry_bufferReturn& _e = \
      this->fromPortHistory_bufferReturn->at(index); \
    ASSERT_EQ(_fwBuffer, _e.fwBuffer) \
    << "\n" \
    << "  File:     " << __FILE__ << "\n" \
    << "  Line:     " << __LINE__ << "\n" \
    << "  Value:    Value of argument fwBuffer at index " \
    << index \
    << " in history of from_bufferReturn\n" \
    << "  Expected: " << _fwBuffer << "\n" \
    << "  Actual:   " << _e.fwBuffer << "\n"; \
  }

namespace Neasc {

  //! \class neascout_scienceGTestBase
  //! \brief Auto-generated base class for neascout_science component Google Test harness
  //!
  class neascout_scienceGTestBase :
    public neascout_scienceTesterBase
  {

    protected:

      // ----------------------------------------------------------------------
      // Construction and destruction
      // ----------------------------------------------------------------------

      //! Construct object neascout_scienceGTestBase
      //!
      neascout_scienceGTestBase(
#if FW_OBJECT_NAMES == 1
          const char *const compName, /*!< The component name*/
          const U32 maxHistorySize /*!< The maximum size of each history*/
#else
          const U32 maxHistorySize /*!< The maximum size of each history*/
#endif
      );

      //! Destroy object neascout_scienceGTestBase
      //!
      virtual ~neascout_scienceGTestBase(void);

    protected:

      // ----------------------------------------------------------------------
      // Commands
      // ----------------------------------------------------------------------

      //! Assert size of command response history
      //!
      void assertCmdResponse_size(
          const char *const __ISF_callSiteFileName, /*!< The name of the file containing the call site*/
          const U32 __ISF_callSiteLineNumber, /*!< The line number of the call site*/
          const U32 size /*!< The asserted size*/
      ) const;

      //! Assert command response in history at index
      //!
      void assertCmdResponse(
          const char *const __ISF_callSiteFileName, /*!< The name of the file containing the call site*/
          const U32 __ISF_callSiteLineNumber, /*!< The line number of the call site*/
          const U32 index, /*!< The index*/
          const FwOpcodeType opCode, /*!< The opcode*/
          const U32 cmdSeq, /*!< The command sequence number*/
          const Fw::CommandResponse response /*!< The command response*/
      ) const;

    protected:

      // ----------------------------------------------------------------------
      // Events
      // ----------------------------------------------------------------------

      void assertEvents_size(
          const char *const __ISF_callSiteFileName, /*!< The name of the file containing the call site*/
          const U32 __ISF_callSiteLineNumber, /*!< The line number of the call site*/
          const U32 size /*!< The asserted size*/
      ) const;

    protected:

      // ----------------------------------------------------------------------
      // Event: L1Complete
      // ----------------------------------------------------------------------

      void assertEvents_L1Complete_size(
          const char *const __ISF_callSiteFileName, /*!< The name of the file containing the call site*/
          const U32 __ISF_callSiteLineNumber, /*!< The line number of the call site*/
          const U32 size /*!< The asserted size*/
      ) const;

    protected:

      // ----------------------------------------------------------------------
      // Event: CropCompressComplete
      // ----------------------------------------------------------------------

      void assertEvents_CropCompressComplete_size(
          const char *const __ISF_callSiteFileName, /*!< The name of the file containing the call site*/
          const U32 __ISF_callSiteLineNumber, /*!< The line number of the call site*/
          const U32 size /*!< The asserted size*/
      ) const;

    protected:

      // ----------------------------------------------------------------------
      // Event: CoadditionComplete
      // ----------------------------------------------------------------------

      void assertEvents_CoadditionComplete_size(
          const char *const __ISF_callSiteFileName, /*!< The name of the file containing the call site*/
          const U32 __ISF_callSiteLineNumber, /*!< The line number of the call site*/
          const U32 size /*!< The asserted size*/
      ) const;

    protected:

      // ----------------------------------------------------------------------
      // Event: DetectionComplete
      // ----------------------------------------------------------------------

      void assertEvents_DetectionComplete_size(
          const char *const __ISF_callSiteFileName, /*!< The name of the file containing the call site*/
          const U32 __ISF_callSiteLineNumber, /*!< The line number of the call site*/
          const U32 size /*!< The asserted size*/
      ) const;

    protected:

      // ----------------------------------------------------------------------
      // Event: ImgStatComplete
      // ----------------------------------------------------------------------

      void assertEvents_ImgStatComplete_size(
          const char *const __ISF_callSiteFileName, /*!< The name of the file containing the call site*/
          const U32 __ISF_callSiteLineNumber, /*!< The line number of the call site*/
          const U32 size /*!< The asserted size*/
      ) const;

    protected:

      // ----------------------------------------------------------------------
      // Event: asp_error
      // ----------------------------------------------------------------------

      void assertEvents_asp_error_size(
          const char *const __ISF_callSiteFileName, /*!< The name of the file containing the call site*/
          const U32 __ISF_callSiteLineNumber, /*!< The line number of the call site*/
          const U32 size /*!< The asserted size*/
      ) const;

      void assertEvents_asp_error(
          const char *const __ISF_callSiteFileName, /*!< The name of the file containing the call site*/
          const U32 __ISF_callSiteLineNumber, /*!< The line number of the call site*/
          const U32 index, /*!< The index*/
          const char *const error_code 
      ) const;

    protected:

      // ----------------------------------------------------------------------
      // Event: asp_log
      // ----------------------------------------------------------------------

      void assertEvents_asp_log_size(
          const char *const __ISF_callSiteFileName, /*!< The name of the file containing the call site*/
          const U32 __ISF_callSiteLineNumber, /*!< The line number of the call site*/
          const U32 size /*!< The asserted size*/
      ) const;

      void assertEvents_asp_log(
          const char *const __ISF_callSiteFileName, /*!< The name of the file containing the call site*/
          const U32 __ISF_callSiteLineNumber, /*!< The line number of the call site*/
          const U32 index, /*!< The index*/
          const char *const info_string 
      ) const;

    protected:

      // ----------------------------------------------------------------------
      // Event: MaskUpdateComplete
      // ----------------------------------------------------------------------

      void assertEvents_MaskUpdateComplete_size(
          const char *const __ISF_callSiteFileName, /*!< The name of the file containing the call site*/
          const U32 __ISF_callSiteLineNumber, /*!< The line number of the call site*/
          const U32 size /*!< The asserted size*/
      ) const;

    protected:

      // ----------------------------------------------------------------------
      // From ports 
      // ----------------------------------------------------------------------

      void assertFromPortHistory_size(
          const char *const __ISF_callSiteFileName, /*!< The name of the file containing the call site*/
          const U32 __ISF_callSiteLineNumber, /*!< The line number of the call site*/
          const U32 size /*!< The asserted size*/
      ) const;

    protected:

      // ----------------------------------------------------------------------
      // From port: bufferGetCaller 
      // ----------------------------------------------------------------------

      void assert_from_bufferGetCaller_size(
          const char *const __ISF_callSiteFileName, /*!< The name of the file containing the call site*/
          const U32 __ISF_callSiteLineNumber, /*!< The line number of the call site*/
          const U32 size /*!< The asserted size*/
      ) const;

    protected:

      // ----------------------------------------------------------------------
      // From port: PingResponse 
      // ----------------------------------------------------------------------

      void assert_from_PingResponse_size(
          const char *const __ISF_callSiteFileName, /*!< The name of the file containing the call site*/
          const U32 __ISF_callSiteLineNumber, /*!< The line number of the call site*/
          const U32 size /*!< The asserted size*/
      ) const;

    protected:

      // ----------------------------------------------------------------------
      // From port: bufferReturn 
      // ----------------------------------------------------------------------

      void assert_from_bufferReturn_size(
          const char *const __ISF_callSiteFileName, /*!< The name of the file containing the call site*/
          const U32 __ISF_callSiteLineNumber, /*!< The line number of the call site*/
          const U32 size /*!< The asserted size*/
      ) const;

  };

} // end namespace Neasc

#endif
