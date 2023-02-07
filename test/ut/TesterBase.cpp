// ======================================================================
// \title  neascout_science/test/ut/TesterBase.cpp
// \author Auto-generated
// \brief  cpp file for neascout_science component test harness base class
//
// \copyright
// Copyright 2009-2016, by the California Institute of Technology.
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

#include <stdlib.h>
#include <string.h>
#include "TesterBase.hpp"

namespace Neasc {

  // ----------------------------------------------------------------------
  // Construction, initialization, and destruction
  // ----------------------------------------------------------------------

  neascout_scienceTesterBase ::
    neascout_scienceTesterBase(
#if FW_OBJECT_NAMES == 1
        const char *const compName,
        const U32 maxHistorySize
#else
        const U32 maxHistorySize
#endif
    ) :
#if FW_OBJECT_NAMES == 1
      Fw::PassiveComponentBase(compName)
#else
      Fw::PassiveComponentBase()
#endif
  {
    // Initialize command history
    this->cmdResponseHistory = new History<CmdResponse>(maxHistorySize);
    // Initialize event histories
#if FW_ENABLE_TEXT_LOGGING
    this->textLogHistory = new History<TextLogEntry>(maxHistorySize);
#endif
    this->eventHistory_asp_error =
      new History<EventEntry_asp_error>(maxHistorySize);
    this->eventHistory_asp_log =
      new History<EventEntry_asp_log>(maxHistorySize);
    // Initialize histories for typed user output ports
    this->fromPortHistory_bufferGetCaller =
      new History<FromPortEntry_bufferGetCaller>(maxHistorySize);
    this->fromPortHistory_PingResponse =
      new History<FromPortEntry_PingResponse>(maxHistorySize);
    this->fromPortHistory_bufferReturn =
      new History<FromPortEntry_bufferReturn>(maxHistorySize);
    // Clear history
    this->clearHistory();
  }

  neascout_scienceTesterBase ::
    ~neascout_scienceTesterBase(void) 
  {
    // Destroy command history
    delete this->cmdResponseHistory;
    // Destroy event histories
#if FW_ENABLE_TEXT_LOGGING
    delete this->textLogHistory;
#endif
    delete this->eventHistory_asp_error;
    delete this->eventHistory_asp_log;
  }

  void neascout_scienceTesterBase ::
    init(
        const NATIVE_INT_TYPE instance
    )
  {

    // Initialize base class

		Fw::PassiveComponentBase::init(instance);

    // Attach input port timeCaller

    for (
        NATIVE_INT_TYPE _port = 0;
        _port < this->getNum_from_timeCaller();
        ++_port
    ) {

      this->m_from_timeCaller[_port].init();
      this->m_from_timeCaller[_port].addCallComp(
          this,
          from_timeCaller_static
      );
      this->m_from_timeCaller[_port].setPortNum(_port);

#if FW_OBJECT_NAMES == 1
      char _portName[80];
      (void) snprintf(
          _portName,
          sizeof(_portName),
          "%s_from_timeCaller[%d]",
          this->m_objName,
          _port
      );
      this->m_from_timeCaller[_port].setObjName(_portName);
#endif

    }

    // Attach input port cmdRegOut

    for (
        NATIVE_INT_TYPE _port = 0;
        _port < this->getNum_from_cmdRegOut();
        ++_port
    ) {

      this->m_from_cmdRegOut[_port].init();
      this->m_from_cmdRegOut[_port].addCallComp(
          this,
          from_cmdRegOut_static
      );
      this->m_from_cmdRegOut[_port].setPortNum(_port);

#if FW_OBJECT_NAMES == 1
      char _portName[80];
      (void) snprintf(
          _portName,
          sizeof(_portName),
          "%s_from_cmdRegOut[%d]",
          this->m_objName,
          _port
      );
      this->m_from_cmdRegOut[_port].setObjName(_portName);
#endif

    }

    // Attach input port eventOut

    for (
        NATIVE_INT_TYPE _port = 0;
        _port < this->getNum_from_eventOut();
        ++_port
    ) {

      this->m_from_eventOut[_port].init();
      this->m_from_eventOut[_port].addCallComp(
          this,
          from_eventOut_static
      );
      this->m_from_eventOut[_port].setPortNum(_port);

#if FW_OBJECT_NAMES == 1
      char _portName[80];
      (void) snprintf(
          _portName,
          sizeof(_portName),
          "%s_from_eventOut[%d]",
          this->m_objName,
          _port
      );
      this->m_from_eventOut[_port].setObjName(_portName);
#endif

    }

    // Attach input port bufferGetCaller

    for (
        NATIVE_INT_TYPE _port = 0;
        _port < this->getNum_from_bufferGetCaller();
        ++_port
    ) {

      this->m_from_bufferGetCaller[_port].init();
      this->m_from_bufferGetCaller[_port].addCallComp(
          this,
          from_bufferGetCaller_static
      );
      this->m_from_bufferGetCaller[_port].setPortNum(_port);

#if FW_OBJECT_NAMES == 1
      char _portName[80];
      (void) snprintf(
          _portName,
          sizeof(_portName),
          "%s_from_bufferGetCaller[%d]",
          this->m_objName,
          _port
      );
      this->m_from_bufferGetCaller[_port].setObjName(_portName);
#endif

    }

    // Attach input port PingResponse

    for (
        NATIVE_INT_TYPE _port = 0;
        _port < this->getNum_from_PingResponse();
        ++_port
    ) {

      this->m_from_PingResponse[_port].init();
      this->m_from_PingResponse[_port].addCallComp(
          this,
          from_PingResponse_static
      );
      this->m_from_PingResponse[_port].setPortNum(_port);

#if FW_OBJECT_NAMES == 1
      char _portName[80];
      (void) snprintf(
          _portName,
          sizeof(_portName),
          "%s_from_PingResponse[%d]",
          this->m_objName,
          _port
      );
      this->m_from_PingResponse[_port].setObjName(_portName);
#endif

    }

    // Attach input port bufferReturn

    for (
        NATIVE_INT_TYPE _port = 0;
        _port < this->getNum_from_bufferReturn();
        ++_port
    ) {

      this->m_from_bufferReturn[_port].init();
      this->m_from_bufferReturn[_port].addCallComp(
          this,
          from_bufferReturn_static
      );
      this->m_from_bufferReturn[_port].setPortNum(_port);

#if FW_OBJECT_NAMES == 1
      char _portName[80];
      (void) snprintf(
          _portName,
          sizeof(_portName),
          "%s_from_bufferReturn[%d]",
          this->m_objName,
          _port
      );
      this->m_from_bufferReturn[_port].setObjName(_portName);
#endif

    }

    // Attach input port cmdResponseOut

    for (
        NATIVE_INT_TYPE _port = 0;
        _port < this->getNum_from_cmdResponseOut();
        ++_port
    ) {

      this->m_from_cmdResponseOut[_port].init();
      this->m_from_cmdResponseOut[_port].addCallComp(
          this,
          from_cmdResponseOut_static
      );
      this->m_from_cmdResponseOut[_port].setPortNum(_port);

#if FW_OBJECT_NAMES == 1
      char _portName[80];
      (void) snprintf(
          _portName,
          sizeof(_portName),
          "%s_from_cmdResponseOut[%d]",
          this->m_objName,
          _port
      );
      this->m_from_cmdResponseOut[_port].setObjName(_portName);
#endif

    }

    // Attach input port LogText

#if FW_ENABLE_TEXT_LOGGING == 1
    for (
        NATIVE_INT_TYPE _port = 0;
        _port < this->getNum_from_LogText();
        ++_port
    ) {

      this->m_from_LogText[_port].init();
      this->m_from_LogText[_port].addCallComp(
          this,
          from_LogText_static
      );
      this->m_from_LogText[_port].setPortNum(_port);

#if FW_OBJECT_NAMES == 1
      char _portName[80];
      (void) snprintf(
          _portName,
          sizeof(_portName),
          "%s_from_LogText[%d]",
          this->m_objName,
          _port
      );
      this->m_from_LogText[_port].setObjName(_portName);
#endif

    }
#endif

    // Initialize output port PingRecv

    for (
        NATIVE_INT_TYPE _port = 0;
        _port < this->getNum_to_PingRecv();
        ++_port
    ) {
      this->m_to_PingRecv[_port].init();

#if FW_OBJECT_NAMES == 1
      char _portName[80];
      snprintf(
          _portName,
          sizeof(_portName),
          "%s_to_PingRecv[%d]",
          this->m_objName,
          _port
      );
      this->m_to_PingRecv[_port].setObjName(_portName);
#endif

    }

  }

  // ----------------------------------------------------------------------
  // Getters for port counts
  // ----------------------------------------------------------------------

  NATIVE_INT_TYPE neascout_scienceTesterBase ::
    getNum_from_timeCaller(void) const
  {
    return (NATIVE_INT_TYPE) FW_NUM_ARRAY_ELEMENTS(this->m_from_timeCaller);
  }

  NATIVE_INT_TYPE neascout_scienceTesterBase ::
    getNum_from_cmdRegOut(void) const
  {
    return (NATIVE_INT_TYPE) FW_NUM_ARRAY_ELEMENTS(this->m_from_cmdRegOut);
  }

  NATIVE_INT_TYPE neascout_scienceTesterBase ::
    getNum_from_eventOut(void) const
  {
    return (NATIVE_INT_TYPE) FW_NUM_ARRAY_ELEMENTS(this->m_from_eventOut);
  }

  NATIVE_INT_TYPE neascout_scienceTesterBase ::
    getNum_from_bufferGetCaller(void) const
  {
    return (NATIVE_INT_TYPE) FW_NUM_ARRAY_ELEMENTS(this->m_from_bufferGetCaller);
  }

  NATIVE_INT_TYPE neascout_scienceTesterBase ::
    getNum_to_cmdIn(void) const
  {
    return (NATIVE_INT_TYPE) FW_NUM_ARRAY_ELEMENTS(this->m_to_cmdIn);
  }

  NATIVE_INT_TYPE neascout_scienceTesterBase ::
    getNum_from_PingResponse(void) const
  {
    return (NATIVE_INT_TYPE) FW_NUM_ARRAY_ELEMENTS(this->m_from_PingResponse);
  }

  NATIVE_INT_TYPE neascout_scienceTesterBase ::
    getNum_to_PingRecv(void) const
  {
    return (NATIVE_INT_TYPE) FW_NUM_ARRAY_ELEMENTS(this->m_to_PingRecv);
  }

  NATIVE_INT_TYPE neascout_scienceTesterBase ::
    getNum_from_bufferReturn(void) const
  {
    return (NATIVE_INT_TYPE) FW_NUM_ARRAY_ELEMENTS(this->m_from_bufferReturn);
  }

  NATIVE_INT_TYPE neascout_scienceTesterBase ::
    getNum_from_cmdResponseOut(void) const
  {
    return (NATIVE_INT_TYPE) FW_NUM_ARRAY_ELEMENTS(this->m_from_cmdResponseOut);
  }

#if FW_ENABLE_TEXT_LOGGING == 1
  NATIVE_INT_TYPE neascout_scienceTesterBase ::
    getNum_from_LogText(void) const
  {
    return (NATIVE_INT_TYPE) FW_NUM_ARRAY_ELEMENTS(this->m_from_LogText);
  }
#endif

  // ----------------------------------------------------------------------
  // Connectors for to ports 
  // ----------------------------------------------------------------------

  void neascout_scienceTesterBase ::
    connect_to_cmdIn(
        const NATIVE_INT_TYPE portNum,
        Fw::InputCmdPort *const cmdIn
    ) 
  {
    FW_ASSERT(portNum < this->getNum_to_cmdIn(),static_cast<AssertArg>(portNum));
    this->m_to_cmdIn[portNum].addCallPort(cmdIn);
  }

  void neascout_scienceTesterBase ::
    connect_to_PingRecv(
        const NATIVE_INT_TYPE portNum,
        Svc::InputPingPort *const PingRecv
    ) 
  {
    FW_ASSERT(portNum < this->getNum_to_PingRecv(),static_cast<AssertArg>(portNum));
    this->m_to_PingRecv[portNum].addCallPort(PingRecv);
  }


  // ----------------------------------------------------------------------
  // Invocation functions for to ports
  // ----------------------------------------------------------------------

  void neascout_scienceTesterBase ::
    invoke_to_PingRecv(
        const NATIVE_INT_TYPE portNum,
        U32 key
    )
  {
    FW_ASSERT(portNum < this->getNum_to_PingRecv(),static_cast<AssertArg>(portNum));
    FW_ASSERT(portNum < this->getNum_to_PingRecv(),static_cast<AssertArg>(portNum));
    this->m_to_PingRecv[portNum].invoke(
        key
    );
  }

  // ----------------------------------------------------------------------
  // Connection status for to ports
  // ----------------------------------------------------------------------

  bool neascout_scienceTesterBase ::
    isConnected_to_cmdIn(const NATIVE_INT_TYPE portNum)
  {
    FW_ASSERT(portNum < this->getNum_to_cmdIn(), static_cast<AssertArg>(portNum));
    return this->m_to_cmdIn[portNum].isConnected();
  }

  bool neascout_scienceTesterBase ::
    isConnected_to_PingRecv(const NATIVE_INT_TYPE portNum)
  {
    FW_ASSERT(portNum < this->getNum_to_PingRecv(), static_cast<AssertArg>(portNum));
    return this->m_to_PingRecv[portNum].isConnected();
  }

  // ----------------------------------------------------------------------
  // Getters for from ports
  // ----------------------------------------------------------------------
 
  Fw::InputTimePort *neascout_scienceTesterBase ::
    get_from_timeCaller(const NATIVE_INT_TYPE portNum)
  {
    FW_ASSERT(portNum < this->getNum_from_timeCaller(),static_cast<AssertArg>(portNum));
    return &this->m_from_timeCaller[portNum];
  }

  Fw::InputCmdRegPort *neascout_scienceTesterBase ::
    get_from_cmdRegOut(const NATIVE_INT_TYPE portNum)
  {
    FW_ASSERT(portNum < this->getNum_from_cmdRegOut(),static_cast<AssertArg>(portNum));
    return &this->m_from_cmdRegOut[portNum];
  }

  Fw::InputLogPort *neascout_scienceTesterBase ::
    get_from_eventOut(const NATIVE_INT_TYPE portNum)
  {
    FW_ASSERT(portNum < this->getNum_from_eventOut(),static_cast<AssertArg>(portNum));
    return &this->m_from_eventOut[portNum];
  }

  Fw::InputBufferGetPort *neascout_scienceTesterBase ::
    get_from_bufferGetCaller(const NATIVE_INT_TYPE portNum)
  {
    FW_ASSERT(portNum < this->getNum_from_bufferGetCaller(),static_cast<AssertArg>(portNum));
    return &this->m_from_bufferGetCaller[portNum];
  }

  Svc::InputPingPort *neascout_scienceTesterBase ::
    get_from_PingResponse(const NATIVE_INT_TYPE portNum)
  {
    FW_ASSERT(portNum < this->getNum_from_PingResponse(),static_cast<AssertArg>(portNum));
    return &this->m_from_PingResponse[portNum];
  }

  Fw::InputBufferSendPort *neascout_scienceTesterBase ::
    get_from_bufferReturn(const NATIVE_INT_TYPE portNum)
  {
    FW_ASSERT(portNum < this->getNum_from_bufferReturn(),static_cast<AssertArg>(portNum));
    return &this->m_from_bufferReturn[portNum];
  }

  Fw::InputCmdResponsePort *neascout_scienceTesterBase ::
    get_from_cmdResponseOut(const NATIVE_INT_TYPE portNum)
  {
    FW_ASSERT(portNum < this->getNum_from_cmdResponseOut(),static_cast<AssertArg>(portNum));
    return &this->m_from_cmdResponseOut[portNum];
  }

#if FW_ENABLE_TEXT_LOGGING == 1
  Fw::InputLogTextPort *neascout_scienceTesterBase ::
    get_from_LogText(const NATIVE_INT_TYPE portNum)
  {
    FW_ASSERT(portNum < this->getNum_from_LogText(),static_cast<AssertArg>(portNum));
    return &this->m_from_LogText[portNum];
  }
#endif

  // ----------------------------------------------------------------------
  // Static functions for from ports
  // ----------------------------------------------------------------------

  Fw::Buffer neascout_scienceTesterBase ::
    from_bufferGetCaller_static(
        Fw::PassiveComponentBase *const callComp,
        const NATIVE_INT_TYPE portNum,
        U32 size
    )
  {
    FW_ASSERT(callComp);
    neascout_scienceTesterBase* _testerBase = 
      static_cast<neascout_scienceTesterBase*>(callComp);
    return _testerBase->from_bufferGetCaller_handlerBase(
        portNum,
        size
    );
  }

  void neascout_scienceTesterBase ::
    from_PingResponse_static(
        Fw::PassiveComponentBase *const callComp,
        const NATIVE_INT_TYPE portNum,
        U32 key
    )
  {
    FW_ASSERT(callComp);
    neascout_scienceTesterBase* _testerBase = 
      static_cast<neascout_scienceTesterBase*>(callComp);
    _testerBase->from_PingResponse_handlerBase(
        portNum,
        key
    );
  }

  void neascout_scienceTesterBase ::
    from_bufferReturn_static(
        Fw::PassiveComponentBase *const callComp,
        const NATIVE_INT_TYPE portNum,
        Fw::Buffer fwBuffer
    )
  {
    FW_ASSERT(callComp);
    neascout_scienceTesterBase* _testerBase = 
      static_cast<neascout_scienceTesterBase*>(callComp);
    _testerBase->from_bufferReturn_handlerBase(
        portNum,
        fwBuffer
    );
  }

  void neascout_scienceTesterBase ::
    from_cmdResponseOut_static(
        Fw::PassiveComponentBase *const component,
        const NATIVE_INT_TYPE portNum,
        const FwOpcodeType opCode,
        const U32 cmdSeq,
        const Fw::CommandResponse response
    )
  {
    neascout_scienceTesterBase* _testerBase =
      static_cast<neascout_scienceTesterBase*>(component);
    _testerBase->cmdResponseIn(opCode, cmdSeq, response);
  }

  void neascout_scienceTesterBase ::
    from_cmdRegOut_static(
        Fw::PassiveComponentBase *const component,
        const NATIVE_INT_TYPE portNum,
        const FwOpcodeType opCode
    )
  {

  }

  void neascout_scienceTesterBase ::
    from_eventOut_static(
        Fw::PassiveComponentBase *const component,
        const NATIVE_INT_TYPE portNum,
        FwEventIdType id,
        Fw::Time &timeTag,
        Fw::LogSeverity severity,
        Fw::LogBuffer &args
    )
  {
    neascout_scienceTesterBase* _testerBase =
      static_cast<neascout_scienceTesterBase*>(component);
    _testerBase->dispatchEvents(id, timeTag, severity, args);
  }

#if FW_ENABLE_TEXT_LOGGING == 1
  void neascout_scienceTesterBase ::
    from_LogText_static(
        Fw::PassiveComponentBase *const component,
        const NATIVE_INT_TYPE portNum,
        FwEventIdType id,
        Fw::Time &timeTag,
        Fw::TextLogSeverity severity,
        Fw::TextLogString &text
    )
  {
    neascout_scienceTesterBase* _testerBase =
      static_cast<neascout_scienceTesterBase*>(component);
    _testerBase->textLogIn(id,timeTag,severity,text);
  }
#endif

  void neascout_scienceTesterBase ::
    from_timeCaller_static(
        Fw::PassiveComponentBase *const component,
        const NATIVE_INT_TYPE portNum,
        Fw::Time& time
    )
  {
    neascout_scienceTesterBase* _testerBase =
      static_cast<neascout_scienceTesterBase*>(component);
    time = _testerBase->m_testTime;
  }

  // ----------------------------------------------------------------------
  // Histories for typed from ports
  // ----------------------------------------------------------------------

  void neascout_scienceTesterBase ::
    clearFromPortHistory(void)
  {
    this->fromPortHistorySize = 0;
    this->fromPortHistory_bufferGetCaller->clear();
    this->fromPortHistory_PingResponse->clear();
    this->fromPortHistory_bufferReturn->clear();
  }

  // ---------------------------------------------------------------------- 
  // From port: bufferGetCaller
  // ---------------------------------------------------------------------- 

  void neascout_scienceTesterBase ::
    pushFromPortEntry_bufferGetCaller(
        U32 size
    )
  {
    FromPortEntry_bufferGetCaller _e = {
      size
    };
    this->fromPortHistory_bufferGetCaller->push_back(_e);
    ++this->fromPortHistorySize;
  }

  // ---------------------------------------------------------------------- 
  // From port: PingResponse
  // ---------------------------------------------------------------------- 

  void neascout_scienceTesterBase ::
    pushFromPortEntry_PingResponse(
        U32 key
    )
  {
    FromPortEntry_PingResponse _e = {
      key
    };
    this->fromPortHistory_PingResponse->push_back(_e);
    ++this->fromPortHistorySize;
  }

  // ---------------------------------------------------------------------- 
  // From port: bufferReturn
  // ---------------------------------------------------------------------- 

  void neascout_scienceTesterBase ::
    pushFromPortEntry_bufferReturn(
        Fw::Buffer fwBuffer
    )
  {
    FromPortEntry_bufferReturn _e = {
      fwBuffer
    };
    this->fromPortHistory_bufferReturn->push_back(_e);
    ++this->fromPortHistorySize;
  }

  // ----------------------------------------------------------------------
  // Handler base functions for from ports
  // ----------------------------------------------------------------------

  Fw::Buffer neascout_scienceTesterBase ::
    from_bufferGetCaller_handlerBase(
        const NATIVE_INT_TYPE portNum,
        U32 size
    )
  {
    FW_ASSERT(portNum < this->getNum_from_bufferGetCaller(),static_cast<AssertArg>(portNum));
    return this->from_bufferGetCaller_handler(
        portNum,
        size
    );
  }

  void neascout_scienceTesterBase ::
    from_PingResponse_handlerBase(
        const NATIVE_INT_TYPE portNum,
        U32 key
    )
  {
    FW_ASSERT(portNum < this->getNum_from_PingResponse(),static_cast<AssertArg>(portNum));
    this->from_PingResponse_handler(
        portNum,
        key
    );
  }

  void neascout_scienceTesterBase ::
    from_bufferReturn_handlerBase(
        const NATIVE_INT_TYPE portNum,
        Fw::Buffer fwBuffer
    )
  {
    FW_ASSERT(portNum < this->getNum_from_bufferReturn(),static_cast<AssertArg>(portNum));
    this->from_bufferReturn_handler(
        portNum,
        fwBuffer
    );
  }

  // ----------------------------------------------------------------------
  // Command response handling
  // ----------------------------------------------------------------------

  void neascout_scienceTesterBase ::
    cmdResponseIn(
        const FwOpcodeType opCode,
        const U32 seq,
        const Fw::CommandResponse response
    )
  {
    CmdResponse e = { opCode, seq, response };
    this->cmdResponseHistory->push_back(e);
  }

  // ---------------------------------------------------------------------- 
  // Command: NEAS_SCIENCE_CMD_L1
  // ---------------------------------------------------------------------- 

  void neascout_scienceTesterBase ::
    sendCmd_NEAS_SCIENCE_CMD_L1(
        const NATIVE_INT_TYPE instance,
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

    // Serialize arguments

    Fw::CmdArgBuffer buff;
    Fw::SerializeStatus _status;
    _status = buff.serialize(rawFrame);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));
    _status = buff.serialize(outFrame);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));
    _status = buff.serialize(ffFrame1);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));
    _status = buff.serialize(ffFrame2);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));
    _status = buff.serialize(badFrame);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));
    _status = buff.serialize(darkFrame);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));
    _status = buff.serialize(fixPt);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));
    _status = buff.serialize(unpack_only);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));
    _status = buff.serialize(temperature);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));

    // Call output command port
    
    FwOpcodeType _opcode;
    const U32 idBase = this->getIdBase();
    _opcode = neascout_scienceComponentBase::OPCODE_NEAS_SCIENCE_CMD_L1 + idBase;

    if (this->m_to_cmdIn[0].isConnected()) {
      this->m_to_cmdIn[0].invoke(
          _opcode,
          cmdSeq,
          buff
      );
    }
    else {
      printf("Test Command Output port not connected!\n");
    }

  }

  // ---------------------------------------------------------------------- 
  // Command: NEAS_SCIENCE_CMD_COADD
  // ---------------------------------------------------------------------- 

  void neascout_scienceTesterBase ::
    sendCmd_NEAS_SCIENCE_CMD_COADD(
        const NATIVE_INT_TYPE instance,
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

    // Serialize arguments

    Fw::CmdArgBuffer buff;
    Fw::SerializeStatus _status;
    _status = buff.serialize(nFrames);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));
    _status = buff.serialize(baseFrame);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));
    _status = buff.serialize(outFrame);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));
    _status = buff.serialize(numStars);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));
    _status = buff.serialize(searchRadius);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));
    _status = buff.serialize(scaling);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));
    _status = buff.serialize(fixPt);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));
    _status = buff.serialize(calcShift);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));

    // Call output command port
    
    FwOpcodeType _opcode;
    const U32 idBase = this->getIdBase();
    _opcode = neascout_scienceComponentBase::OPCODE_NEAS_SCIENCE_CMD_COADD + idBase;

    if (this->m_to_cmdIn[0].isConnected()) {
      this->m_to_cmdIn[0].invoke(
          _opcode,
          cmdSeq,
          buff
      );
    }
    else {
      printf("Test Command Output port not connected!\n");
    }

  }

  // ---------------------------------------------------------------------- 
  // Command: NEAS_SCIENCE_CMD_CROP
  // ---------------------------------------------------------------------- 

  void neascout_scienceTesterBase ::
    sendCmd_NEAS_SCIENCE_CMD_CROP(
        const NATIVE_INT_TYPE instance,
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

    // Serialize arguments

    Fw::CmdArgBuffer buff;
    Fw::SerializeStatus _status;
    _status = buff.serialize(inFrame);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));
    _status = buff.serialize(outFrame);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));
    _status = buff.serialize(cropLeft);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));
    _status = buff.serialize(cropTop);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));
    _status = buff.serialize(cropWidth);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));
    _status = buff.serialize(cropHeight);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));
    _status = buff.serialize(search);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));
    _status = buff.serialize(compressionRate);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));
    _status = buff.serialize(downsampleFactor);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));
    _status = buff.serialize(min_loss);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));
    _status = buff.serialize(filtype);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));
    _status = buff.serialize(ndecomps);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));
    _status = buff.serialize(nseg);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));

    // Call output command port
    
    FwOpcodeType _opcode;
    const U32 idBase = this->getIdBase();
    _opcode = neascout_scienceComponentBase::OPCODE_NEAS_SCIENCE_CMD_CROP + idBase;

    if (this->m_to_cmdIn[0].isConnected()) {
      this->m_to_cmdIn[0].invoke(
          _opcode,
          cmdSeq,
          buff
      );
    }
    else {
      printf("Test Command Output port not connected!\n");
    }

  }

  // ---------------------------------------------------------------------- 
  // Command: NEAS_SCIENCE_CMD_DETECT
  // ---------------------------------------------------------------------- 

  void neascout_scienceTesterBase ::
    sendCmd_NEAS_SCIENCE_CMD_DETECT(
        const NATIVE_INT_TYPE instance,
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

    // Serialize arguments

    Fw::CmdArgBuffer buff;
    Fw::SerializeStatus _status;
    _status = buff.serialize(inFrameA);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));
    _status = buff.serialize(inFrameB);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));
    _status = buff.serialize(outFrame);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));
    _status = buff.serialize(sigma);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));
    _status = buff.serialize(aligned);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));
    _status = buff.serialize(searchRadius);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));
    _status = buff.serialize(numStars);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));
    _status = buff.serialize(scaling);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));
    _status = buff.serialize(gaussKernelWidth);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));
    _status = buff.serialize(gaussKernelScale);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));
    _status = buff.serialize(fixPt);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));

    // Call output command port
    
    FwOpcodeType _opcode;
    const U32 idBase = this->getIdBase();
    _opcode = neascout_scienceComponentBase::OPCODE_NEAS_SCIENCE_CMD_DETECT + idBase;

    if (this->m_to_cmdIn[0].isConnected()) {
      this->m_to_cmdIn[0].invoke(
          _opcode,
          cmdSeq,
          buff
      );
    }
    else {
      printf("Test Command Output port not connected!\n");
    }

  }

  // ---------------------------------------------------------------------- 
  // Command: NEAS_SCIENCE_CMD_IMGSTAT
  // ---------------------------------------------------------------------- 

  void neascout_scienceTesterBase ::
    sendCmd_NEAS_SCIENCE_CMD_IMGSTAT(
        const NATIVE_INT_TYPE instance,
        const U32 cmdSeq,
        const Fw::CmdStringArg& frameA,
        const Fw::CmdStringArg& frameB,
        I32 bin0HistMax,
        I32 bin1HistMax,
        I32 bin4HistMax,
        I32 bin5HistMax
    )
  {

    // Serialize arguments

    Fw::CmdArgBuffer buff;
    Fw::SerializeStatus _status;
    _status = buff.serialize(frameA);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));
    _status = buff.serialize(frameB);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));
    _status = buff.serialize(bin0HistMax);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));
    _status = buff.serialize(bin1HistMax);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));
    _status = buff.serialize(bin4HistMax);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));
    _status = buff.serialize(bin5HistMax);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));

    // Call output command port
    
    FwOpcodeType _opcode;
    const U32 idBase = this->getIdBase();
    _opcode = neascout_scienceComponentBase::OPCODE_NEAS_SCIENCE_CMD_IMGSTAT + idBase;

    if (this->m_to_cmdIn[0].isConnected()) {
      this->m_to_cmdIn[0].invoke(
          _opcode,
          cmdSeq,
          buff
      );
    }
    else {
      printf("Test Command Output port not connected!\n");
    }

  }

  // ---------------------------------------------------------------------- 
  // Command: NEAS_SCIENCE_CMD_MASK_UPDATE
  // ---------------------------------------------------------------------- 

  void neascout_scienceTesterBase ::
    sendCmd_NEAS_SCIENCE_CMD_MASK_UPDATE(
        const NATIVE_INT_TYPE instance,
        const U32 cmdSeq,
        const Fw::CmdStringArg& inFrame,
        const Fw::CmdStringArg& outFrame,
        I16 row,
        I16 col,
        I16 value
    )
  {

    // Serialize arguments

    Fw::CmdArgBuffer buff;
    Fw::SerializeStatus _status;
    _status = buff.serialize(inFrame);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));
    _status = buff.serialize(outFrame);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));
    _status = buff.serialize(row);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));
    _status = buff.serialize(col);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));
    _status = buff.serialize(value);
    FW_ASSERT(_status == Fw::FW_SERIALIZE_OK,static_cast<AssertArg>(_status));

    // Call output command port
    
    FwOpcodeType _opcode;
    const U32 idBase = this->getIdBase();
    _opcode = neascout_scienceComponentBase::OPCODE_NEAS_SCIENCE_CMD_MASK_UPDATE + idBase;

    if (this->m_to_cmdIn[0].isConnected()) {
      this->m_to_cmdIn[0].invoke(
          _opcode,
          cmdSeq,
          buff
      );
    }
    else {
      printf("Test Command Output port not connected!\n");
    }

  }

  
  void neascout_scienceTesterBase ::
    sendRawCmd(FwOpcodeType opcode, U32 cmdSeq, Fw::CmdArgBuffer& args) {
       
    const U32 idBase = this->getIdBase();   
    FwOpcodeType _opcode = opcode + idBase;
    if (this->m_to_cmdIn[0].isConnected()) {
      this->m_to_cmdIn[0].invoke(
          _opcode,
          cmdSeq,
          args
      );
    }
    else {
      printf("Test Command Output port not connected!\n");
    }
        
  }
  
  // ----------------------------------------------------------------------
  // History 
  // ----------------------------------------------------------------------

  void neascout_scienceTesterBase ::
    clearHistory()
  {
    this->cmdResponseHistory->clear();
    this->textLogHistory->clear();
    this->clearEvents();
    this->clearFromPortHistory();
  }

  // ----------------------------------------------------------------------
  // Time
  // ----------------------------------------------------------------------

  void neascout_scienceTesterBase ::
    setTestTime(const Fw::Time& time)
  {
    this->m_testTime = time;
  }

  // ----------------------------------------------------------------------
  // Event dispatch
  // ----------------------------------------------------------------------

  void neascout_scienceTesterBase ::
    dispatchEvents(
        const FwEventIdType id,
        Fw::Time &timeTag,
        const Fw::LogSeverity severity,
        Fw::LogBuffer &args
    )
  {

    args.resetDeser();

    const U32 idBase = this->getIdBase();
    FW_ASSERT(id >= idBase, id, idBase);
    switch (id - idBase) {

      case neascout_scienceComponentBase::EVENTID_L1COMPLETE: 
      {

#if FW_AMPCS_COMPATIBLE
        // For AMPCS, decode zero arguments
        Fw::SerializeStatus _zero_status = Fw::FW_SERIALIZE_OK;
        U8 _noArgs;
        _zero_status = args.deserialize(_noArgs);
        FW_ASSERT(
            _zero_status == Fw::FW_SERIALIZE_OK,
            static_cast<AssertArg>(_zero_status)
        );
#endif    
        this->logIn_ACTIVITY_LO_L1Complete();

        break;

      }

      case neascout_scienceComponentBase::EVENTID_CROPCOMPRESSCOMPLETE: 
      {

#if FW_AMPCS_COMPATIBLE
        // For AMPCS, decode zero arguments
        Fw::SerializeStatus _zero_status = Fw::FW_SERIALIZE_OK;
        U8 _noArgs;
        _zero_status = args.deserialize(_noArgs);
        FW_ASSERT(
            _zero_status == Fw::FW_SERIALIZE_OK,
            static_cast<AssertArg>(_zero_status)
        );
#endif    
        this->logIn_ACTIVITY_LO_CropCompressComplete();

        break;

      }

      case neascout_scienceComponentBase::EVENTID_COADDITIONCOMPLETE: 
      {

#if FW_AMPCS_COMPATIBLE
        // For AMPCS, decode zero arguments
        Fw::SerializeStatus _zero_status = Fw::FW_SERIALIZE_OK;
        U8 _noArgs;
        _zero_status = args.deserialize(_noArgs);
        FW_ASSERT(
            _zero_status == Fw::FW_SERIALIZE_OK,
            static_cast<AssertArg>(_zero_status)
        );
#endif    
        this->logIn_ACTIVITY_LO_CoadditionComplete();

        break;

      }

      case neascout_scienceComponentBase::EVENTID_DETECTIONCOMPLETE: 
      {

#if FW_AMPCS_COMPATIBLE
        // For AMPCS, decode zero arguments
        Fw::SerializeStatus _zero_status = Fw::FW_SERIALIZE_OK;
        U8 _noArgs;
        _zero_status = args.deserialize(_noArgs);
        FW_ASSERT(
            _zero_status == Fw::FW_SERIALIZE_OK,
            static_cast<AssertArg>(_zero_status)
        );
#endif    
        this->logIn_ACTIVITY_LO_DetectionComplete();

        break;

      }

      case neascout_scienceComponentBase::EVENTID_IMGSTATCOMPLETE: 
      {

#if FW_AMPCS_COMPATIBLE
        // For AMPCS, decode zero arguments
        Fw::SerializeStatus _zero_status = Fw::FW_SERIALIZE_OK;
        U8 _noArgs;
        _zero_status = args.deserialize(_noArgs);
        FW_ASSERT(
            _zero_status == Fw::FW_SERIALIZE_OK,
            static_cast<AssertArg>(_zero_status)
        );
#endif    
        this->logIn_ACTIVITY_LO_ImgStatComplete();

        break;

      }

      case neascout_scienceComponentBase::EVENTID_ASP_ERROR: 
      {

        Fw::SerializeStatus _status = Fw::FW_SERIALIZE_OK;
#if FW_AMPCS_COMPATIBLE
        // Deserialize the number of arguments.
        U8 _numArgs;
        _status = args.deserialize(_numArgs);
        FW_ASSERT(
          _status == Fw::FW_SERIALIZE_OK,
          static_cast<AssertArg>(_status)
        );
        // verify they match expected.
        FW_ASSERT(_numArgs == 1,_numArgs,1);
        
#endif    
        Fw::LogStringArg error_code;
        _status = args.deserialize(error_code);
        FW_ASSERT(
            _status == Fw::FW_SERIALIZE_OK,
            static_cast<AssertArg>(_status)
        );

        this->logIn_WARNING_LO_asp_error(error_code);

        break;

      }

      case neascout_scienceComponentBase::EVENTID_ASP_LOG: 
      {

        Fw::SerializeStatus _status = Fw::FW_SERIALIZE_OK;
#if FW_AMPCS_COMPATIBLE
        // Deserialize the number of arguments.
        U8 _numArgs;
        _status = args.deserialize(_numArgs);
        FW_ASSERT(
          _status == Fw::FW_SERIALIZE_OK,
          static_cast<AssertArg>(_status)
        );
        // verify they match expected.
        FW_ASSERT(_numArgs == 1,_numArgs,1);
        
#endif    
        Fw::LogStringArg info_string;
        _status = args.deserialize(info_string);
        FW_ASSERT(
            _status == Fw::FW_SERIALIZE_OK,
            static_cast<AssertArg>(_status)
        );

        this->logIn_ACTIVITY_LO_asp_log(info_string);

        break;

      }

      case neascout_scienceComponentBase::EVENTID_MASKUPDATECOMPLETE: 
      {

#if FW_AMPCS_COMPATIBLE
        // For AMPCS, decode zero arguments
        Fw::SerializeStatus _zero_status = Fw::FW_SERIALIZE_OK;
        U8 _noArgs;
        _zero_status = args.deserialize(_noArgs);
        FW_ASSERT(
            _zero_status == Fw::FW_SERIALIZE_OK,
            static_cast<AssertArg>(_zero_status)
        );
#endif    
        this->logIn_ACTIVITY_LO_MaskUpdateComplete();

        break;

      }

      default: {
        FW_ASSERT(0, id);
        break;
      }

    }

  }

  void neascout_scienceTesterBase ::
    clearEvents(void)
  {
    this->eventsSize = 0;
    this->eventsSize_L1Complete = 0;
    this->eventsSize_CropCompressComplete = 0;
    this->eventsSize_CoadditionComplete = 0;
    this->eventsSize_DetectionComplete = 0;
    this->eventsSize_ImgStatComplete = 0;
    this->eventHistory_asp_error->clear();
    this->eventHistory_asp_log->clear();
    this->eventsSize_MaskUpdateComplete = 0;
  }

#if FW_ENABLE_TEXT_LOGGING

  // ----------------------------------------------------------------------
  // Text events 
  // ----------------------------------------------------------------------

  void neascout_scienceTesterBase ::
    textLogIn(
        const U32 id,
        Fw::Time &timeTag,
        const Fw::TextLogSeverity severity,
        const Fw::TextLogString &text
    )
  {
    TextLogEntry e = { id, timeTag, severity, text };
    textLogHistory->push_back(e);
  }

  void neascout_scienceTesterBase ::
    printTextLogHistoryEntry(
        const TextLogEntry& e,
        FILE* file
    )
  {
    const char *severityString = "UNKNOWN";
    switch (e.severity) {
      case Fw::LOG_FATAL:
        severityString = "FATAL";
        break;
      case Fw::LOG_WARNING_HI:
        severityString = "WARNING_HI";
        break;
      case Fw::LOG_WARNING_LO:
        severityString = "WARNING_LO";
        break;
      case Fw::LOG_COMMAND:
        severityString = "COMMAND";
        break;
      case Fw::LOG_ACTIVITY_HI:
        severityString = "ACTIVITY_HI";
        break;
      case Fw::LOG_ACTIVITY_LO:
        severityString = "ACTIVITY_LO";
        break;
      case Fw::LOG_DIAGNOSTIC:
       severityString = "DIAGNOSTIC";
        break;
      default:
        severityString = "SEVERITY ERROR";
        break;
    }

    fprintf(
        file,
        "EVENT: (%d) (%d:%d,%d) %s: %s\n",
        e.id,
        const_cast<TextLogEntry&>(e).timeTag.getTimeBase(),
        const_cast<TextLogEntry&>(e).timeTag.getSeconds(),
        const_cast<TextLogEntry&>(e).timeTag.getUSeconds(),
        severityString,
        e.text.toChar()
    );

  }

  void neascout_scienceTesterBase ::
    printTextLogHistory(FILE *file) 
  {
    for (U32 i = 0; i < this->textLogHistory->size(); ++i) {
      this->printTextLogHistoryEntry(
          this->textLogHistory->at(i), 
          file
      );
    }
  }

#endif

  // ----------------------------------------------------------------------
  // Event: L1Complete 
  // ----------------------------------------------------------------------

  void neascout_scienceTesterBase ::
    logIn_ACTIVITY_LO_L1Complete(
        void
    )
  {
    ++this->eventsSize_L1Complete;
    ++this->eventsSize;
  }

  // ----------------------------------------------------------------------
  // Event: CropCompressComplete 
  // ----------------------------------------------------------------------

  void neascout_scienceTesterBase ::
    logIn_ACTIVITY_LO_CropCompressComplete(
        void
    )
  {
    ++this->eventsSize_CropCompressComplete;
    ++this->eventsSize;
  }

  // ----------------------------------------------------------------------
  // Event: CoadditionComplete 
  // ----------------------------------------------------------------------

  void neascout_scienceTesterBase ::
    logIn_ACTIVITY_LO_CoadditionComplete(
        void
    )
  {
    ++this->eventsSize_CoadditionComplete;
    ++this->eventsSize;
  }

  // ----------------------------------------------------------------------
  // Event: DetectionComplete 
  // ----------------------------------------------------------------------

  void neascout_scienceTesterBase ::
    logIn_ACTIVITY_LO_DetectionComplete(
        void
    )
  {
    ++this->eventsSize_DetectionComplete;
    ++this->eventsSize;
  }

  // ----------------------------------------------------------------------
  // Event: ImgStatComplete 
  // ----------------------------------------------------------------------

  void neascout_scienceTesterBase ::
    logIn_ACTIVITY_LO_ImgStatComplete(
        void
    )
  {
    ++this->eventsSize_ImgStatComplete;
    ++this->eventsSize;
  }

  // ----------------------------------------------------------------------
  // Event: asp_error 
  // ----------------------------------------------------------------------

  void neascout_scienceTesterBase ::
    logIn_WARNING_LO_asp_error(
        Fw::LogStringArg& error_code
    )
  {
    EventEntry_asp_error e = {
      error_code
    };
    eventHistory_asp_error->push_back(e);
    ++this->eventsSize;
  }

  // ----------------------------------------------------------------------
  // Event: asp_log 
  // ----------------------------------------------------------------------

  void neascout_scienceTesterBase ::
    logIn_ACTIVITY_LO_asp_log(
        Fw::LogStringArg& info_string
    )
  {
    EventEntry_asp_log e = {
      info_string
    };
    eventHistory_asp_log->push_back(e);
    ++this->eventsSize;
  }

  // ----------------------------------------------------------------------
  // Event: MaskUpdateComplete 
  // ----------------------------------------------------------------------

  void neascout_scienceTesterBase ::
    logIn_ACTIVITY_LO_MaskUpdateComplete(
        void
    )
  {
    ++this->eventsSize_MaskUpdateComplete;
    ++this->eventsSize;
  }

} // end namespace Neasc
