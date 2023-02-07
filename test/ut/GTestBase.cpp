// ======================================================================
// \title  neascout_science/test/ut/GTestBase.cpp
// \author Auto-generated
// \brief  cpp file for neascout_science component Google Test harness base class
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

#include "GTestBase.hpp"

namespace Neasc {

  // ----------------------------------------------------------------------
  // Construction and destruction
  // ----------------------------------------------------------------------

  neascout_scienceGTestBase ::
    neascout_scienceGTestBase(
#if FW_OBJECT_NAMES == 1
        const char *const compName,
        const U32 maxHistorySize
#else
        const U32 maxHistorySize
#endif
    ) : 
        neascout_scienceTesterBase (
#if FW_OBJECT_NAMES == 1
            compName,
#endif
            maxHistorySize
        )
  {

  }

  neascout_scienceGTestBase ::
    ~neascout_scienceGTestBase(void)
  {

  }

  // ----------------------------------------------------------------------
  // Commands
  // ----------------------------------------------------------------------

  void neascout_scienceGTestBase ::
    assertCmdResponse_size(
        const char *const __ISF_callSiteFileName,
        const U32 __ISF_callSiteLineNumber,
        const U32 size
    ) const
  {
    ASSERT_EQ((unsigned long) size, this->cmdResponseHistory->size())
      << "\n"
      << "  File:     " << __ISF_callSiteFileName << "\n"
      << "  Line:     " << __ISF_callSiteLineNumber << "\n"
      << "  Value:    Size of command response history\n"
      << "  Expected: " << size << "\n"
      << "  Actual:   " << this->cmdResponseHistory->size() << "\n";
  }

  void neascout_scienceGTestBase ::
    assertCmdResponse(
        const char *const __ISF_callSiteFileName,
        const U32 __ISF_callSiteLineNumber,
        const U32 index,
        const FwOpcodeType opCode,
        const U32 cmdSeq,
        const Fw::CommandResponse response
    )
    const
  {
    ASSERT_LT(index, this->cmdResponseHistory->size())
      << "\n"
      << "  File:     " << __ISF_callSiteFileName << "\n"
      << "  Line:     " << __ISF_callSiteLineNumber << "\n"
      << "  Value:    Index into command response history\n"
      << "  Expected: Less than size of command response history (" 
      << this->cmdResponseHistory->size() << ")\n"
      << "  Actual:   " << index << "\n";
    const CmdResponse& e = this->cmdResponseHistory->at(index);
    ASSERT_EQ(opCode, e.opCode)
      << "\n"
      << "  File:     " << __ISF_callSiteFileName << "\n"
      << "  Line:     " << __ISF_callSiteLineNumber << "\n"
      << "  Value:    Opcode at index "
      << index
      << " in command response history\n"
      << "  Expected: " << opCode << "\n"
      << "  Actual:   " << e.opCode << "\n";
    ASSERT_EQ(cmdSeq, e.cmdSeq)
      << "\n"
      << "  File:     " << __ISF_callSiteFileName << "\n"
      << "  Line:     " << __ISF_callSiteLineNumber << "\n"
      << "  Value:    Command sequence number at index "
      << index
      << " in command response history\n"
      << "  Expected: " << cmdSeq << "\n"
      << "  Actual:   " << e.cmdSeq << "\n";
    ASSERT_EQ(response, e.response)
      << "\n"
      << "  File:     " << __ISF_callSiteFileName << "\n"
      << "  Line:     " << __ISF_callSiteLineNumber << "\n"
      << "  Value:    Command response at index "
      << index
      << " in command resopnse history\n"
      << "  Expected: " << response << "\n"
      << "  Actual:   " << e.response << "\n";
  }

  // ----------------------------------------------------------------------
  // Events
  // ----------------------------------------------------------------------

  void neascout_scienceGTestBase ::
    assertEvents_size(
        const char *const __ISF_callSiteFileName,
        const U32 __ISF_callSiteLineNumber,
        const U32 size
    ) const
  {
    ASSERT_EQ(size, this->eventsSize)
      << "\n"
      << "  File:     " << __ISF_callSiteFileName << "\n"
      << "  Line:     " << __ISF_callSiteLineNumber << "\n"
      << "  Value:    Total size of all event histories\n"
      << "  Expected: " << size << "\n"
      << "  Actual:   " << this->eventsSize << "\n";
  }

  // ----------------------------------------------------------------------
  // Event: L1Complete
  // ----------------------------------------------------------------------

  void neascout_scienceGTestBase ::
    assertEvents_L1Complete_size(
        const char *const __ISF_callSiteFileName,
        const U32 __ISF_callSiteLineNumber,
        const U32 size
    ) const
  {
    ASSERT_EQ(size, this->eventsSize_L1Complete)
      << "\n"
      << "  File:     " << __ISF_callSiteFileName << "\n"
      << "  Line:     " << __ISF_callSiteLineNumber << "\n"
      << "  Value:    Size of history for event L1Complete\n"
      << "  Expected: " << size << "\n"
      << "  Actual:   " << this->eventsSize_L1Complete << "\n";
  }

  // ----------------------------------------------------------------------
  // Event: CropCompressComplete
  // ----------------------------------------------------------------------

  void neascout_scienceGTestBase ::
    assertEvents_CropCompressComplete_size(
        const char *const __ISF_callSiteFileName,
        const U32 __ISF_callSiteLineNumber,
        const U32 size
    ) const
  {
    ASSERT_EQ(size, this->eventsSize_CropCompressComplete)
      << "\n"
      << "  File:     " << __ISF_callSiteFileName << "\n"
      << "  Line:     " << __ISF_callSiteLineNumber << "\n"
      << "  Value:    Size of history for event CropCompressComplete\n"
      << "  Expected: " << size << "\n"
      << "  Actual:   " << this->eventsSize_CropCompressComplete << "\n";
  }

  // ----------------------------------------------------------------------
  // Event: CoadditionComplete
  // ----------------------------------------------------------------------

  void neascout_scienceGTestBase ::
    assertEvents_CoadditionComplete_size(
        const char *const __ISF_callSiteFileName,
        const U32 __ISF_callSiteLineNumber,
        const U32 size
    ) const
  {
    ASSERT_EQ(size, this->eventsSize_CoadditionComplete)
      << "\n"
      << "  File:     " << __ISF_callSiteFileName << "\n"
      << "  Line:     " << __ISF_callSiteLineNumber << "\n"
      << "  Value:    Size of history for event CoadditionComplete\n"
      << "  Expected: " << size << "\n"
      << "  Actual:   " << this->eventsSize_CoadditionComplete << "\n";
  }

  // ----------------------------------------------------------------------
  // Event: DetectionComplete
  // ----------------------------------------------------------------------

  void neascout_scienceGTestBase ::
    assertEvents_DetectionComplete_size(
        const char *const __ISF_callSiteFileName,
        const U32 __ISF_callSiteLineNumber,
        const U32 size
    ) const
  {
    ASSERT_EQ(size, this->eventsSize_DetectionComplete)
      << "\n"
      << "  File:     " << __ISF_callSiteFileName << "\n"
      << "  Line:     " << __ISF_callSiteLineNumber << "\n"
      << "  Value:    Size of history for event DetectionComplete\n"
      << "  Expected: " << size << "\n"
      << "  Actual:   " << this->eventsSize_DetectionComplete << "\n";
  }

  // ----------------------------------------------------------------------
  // Event: ImgStatComplete
  // ----------------------------------------------------------------------

  void neascout_scienceGTestBase ::
    assertEvents_ImgStatComplete_size(
        const char *const __ISF_callSiteFileName,
        const U32 __ISF_callSiteLineNumber,
        const U32 size
    ) const
  {
    ASSERT_EQ(size, this->eventsSize_ImgStatComplete)
      << "\n"
      << "  File:     " << __ISF_callSiteFileName << "\n"
      << "  Line:     " << __ISF_callSiteLineNumber << "\n"
      << "  Value:    Size of history for event ImgStatComplete\n"
      << "  Expected: " << size << "\n"
      << "  Actual:   " << this->eventsSize_ImgStatComplete << "\n";
  }

  // ----------------------------------------------------------------------
  // Event: asp_error
  // ----------------------------------------------------------------------

  void neascout_scienceGTestBase ::
    assertEvents_asp_error_size(
        const char *const __ISF_callSiteFileName,
        const U32 __ISF_callSiteLineNumber,
        const U32 size
    ) const
  {
    ASSERT_EQ(size, this->eventHistory_asp_error->size())
      << "\n"
      << "  File:     " << __ISF_callSiteFileName << "\n"
      << "  Line:     " << __ISF_callSiteLineNumber << "\n"
      << "  Value:    Size of history for event asp_error\n"
      << "  Expected: " << size << "\n"
      << "  Actual:   " << this->eventHistory_asp_error->size() << "\n";
  }

  void neascout_scienceGTestBase ::
    assertEvents_asp_error(
        const char *const __ISF_callSiteFileName,
        const U32 __ISF_callSiteLineNumber,
        const U32 index,
        const char *const error_code
    ) const
  {
    ASSERT_GT(this->eventHistory_asp_error->size(), index)
      << "\n"
      << "  File:     " << __ISF_callSiteFileName << "\n"
      << "  Line:     " << __ISF_callSiteLineNumber << "\n"
      << "  Value:    Index into history of event asp_error\n"
      << "  Expected: Less than size of history (" 
      << this->eventHistory_asp_error->size() << ")\n"
      << "  Actual:   " << index << "\n";
    const EventEntry_asp_error& e =
      this->eventHistory_asp_error->at(index);
    ASSERT_STREQ(error_code, e.error_code.toChar())
      << "\n"
      << "  File:     " << __ISF_callSiteFileName << "\n"
      << "  Line:     " << __ISF_callSiteLineNumber << "\n"
      << "  Value:    Value of argument error_code at index "
      << index
      << " in history of event asp_error\n"
      << "  Expected: " << error_code << "\n"
      << "  Actual:   " << e.error_code.toChar() << "\n";
  }

  // ----------------------------------------------------------------------
  // Event: asp_log
  // ----------------------------------------------------------------------

  void neascout_scienceGTestBase ::
    assertEvents_asp_log_size(
        const char *const __ISF_callSiteFileName,
        const U32 __ISF_callSiteLineNumber,
        const U32 size
    ) const
  {
    ASSERT_EQ(size, this->eventHistory_asp_log->size())
      << "\n"
      << "  File:     " << __ISF_callSiteFileName << "\n"
      << "  Line:     " << __ISF_callSiteLineNumber << "\n"
      << "  Value:    Size of history for event asp_log\n"
      << "  Expected: " << size << "\n"
      << "  Actual:   " << this->eventHistory_asp_log->size() << "\n";
  }

  void neascout_scienceGTestBase ::
    assertEvents_asp_log(
        const char *const __ISF_callSiteFileName,
        const U32 __ISF_callSiteLineNumber,
        const U32 index,
        const char *const info_string
    ) const
  {
    ASSERT_GT(this->eventHistory_asp_log->size(), index)
      << "\n"
      << "  File:     " << __ISF_callSiteFileName << "\n"
      << "  Line:     " << __ISF_callSiteLineNumber << "\n"
      << "  Value:    Index into history of event asp_log\n"
      << "  Expected: Less than size of history (" 
      << this->eventHistory_asp_log->size() << ")\n"
      << "  Actual:   " << index << "\n";
    const EventEntry_asp_log& e =
      this->eventHistory_asp_log->at(index);
    ASSERT_STREQ(info_string, e.info_string.toChar())
      << "\n"
      << "  File:     " << __ISF_callSiteFileName << "\n"
      << "  Line:     " << __ISF_callSiteLineNumber << "\n"
      << "  Value:    Value of argument info_string at index "
      << index
      << " in history of event asp_log\n"
      << "  Expected: " << info_string << "\n"
      << "  Actual:   " << e.info_string.toChar() << "\n";
  }

  // ----------------------------------------------------------------------
  // Event: MaskUpdateComplete
  // ----------------------------------------------------------------------

  void neascout_scienceGTestBase ::
    assertEvents_MaskUpdateComplete_size(
        const char *const __ISF_callSiteFileName,
        const U32 __ISF_callSiteLineNumber,
        const U32 size
    ) const
  {
    ASSERT_EQ(size, this->eventsSize_MaskUpdateComplete)
      << "\n"
      << "  File:     " << __ISF_callSiteFileName << "\n"
      << "  Line:     " << __ISF_callSiteLineNumber << "\n"
      << "  Value:    Size of history for event MaskUpdateComplete\n"
      << "  Expected: " << size << "\n"
      << "  Actual:   " << this->eventsSize_MaskUpdateComplete << "\n";
  }

  // ----------------------------------------------------------------------
  // From ports
  // ----------------------------------------------------------------------

  void neascout_scienceGTestBase ::
    assertFromPortHistory_size(
        const char *const __ISF_callSiteFileName,
        const U32 __ISF_callSiteLineNumber,
        const U32 size
    ) const
  {
    ASSERT_EQ(size, this->fromPortHistorySize)
      << "\n"
      << "  File:     " << __ISF_callSiteFileName << "\n"
      << "  Line:     " << __ISF_callSiteLineNumber << "\n"
      << "  Value:    Total size of all from port histories\n"
      << "  Expected: " << size << "\n"
      << "  Actual:   " << this->fromPortHistorySize << "\n";
  }

  // ----------------------------------------------------------------------
  // From port: bufferGetCaller
  // ----------------------------------------------------------------------

  void neascout_scienceGTestBase ::
    assert_from_bufferGetCaller_size(
        const char *const __ISF_callSiteFileName,
        const U32 __ISF_callSiteLineNumber,
        const U32 size
    ) const
  {
    ASSERT_EQ(size, this->fromPortHistory_bufferGetCaller->size())
      << "\n"
      << "  File:     " << __ISF_callSiteFileName << "\n"
      << "  Line:     " << __ISF_callSiteLineNumber << "\n"
      << "  Value:    Size of history for from_bufferGetCaller\n"
      << "  Expected: " << size << "\n"
      << "  Actual:   " << this->fromPortHistory_bufferGetCaller->size() << "\n";
  }

  // ----------------------------------------------------------------------
  // From port: PingResponse
  // ----------------------------------------------------------------------

  void neascout_scienceGTestBase ::
    assert_from_PingResponse_size(
        const char *const __ISF_callSiteFileName,
        const U32 __ISF_callSiteLineNumber,
        const U32 size
    ) const
  {
    ASSERT_EQ(size, this->fromPortHistory_PingResponse->size())
      << "\n"
      << "  File:     " << __ISF_callSiteFileName << "\n"
      << "  Line:     " << __ISF_callSiteLineNumber << "\n"
      << "  Value:    Size of history for from_PingResponse\n"
      << "  Expected: " << size << "\n"
      << "  Actual:   " << this->fromPortHistory_PingResponse->size() << "\n";
  }

  // ----------------------------------------------------------------------
  // From port: bufferReturn
  // ----------------------------------------------------------------------

  void neascout_scienceGTestBase ::
    assert_from_bufferReturn_size(
        const char *const __ISF_callSiteFileName,
        const U32 __ISF_callSiteLineNumber,
        const U32 size
    ) const
  {
    ASSERT_EQ(size, this->fromPortHistory_bufferReturn->size())
      << "\n"
      << "  File:     " << __ISF_callSiteFileName << "\n"
      << "  Line:     " << __ISF_callSiteLineNumber << "\n"
      << "  Value:    Size of history for from_bufferReturn\n"
      << "  Expected: " << size << "\n"
      << "  Actual:   " << this->fromPortHistory_bufferReturn->size() << "\n";
  }

} // end namespace Neasc
