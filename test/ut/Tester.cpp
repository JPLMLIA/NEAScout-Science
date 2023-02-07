// ====================================================================== 
// \title  neascout_science.hpp
// \author Jack Lightholder
// \brief  cpp file for neascout_science test harness implementation class
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

#include "Tester.hpp"
#include "../../asp/asp_image2d.h"
#include "../../neascout_scienceComponentImpl.hpp"

#define INSTANCE 0
#define MAX_HISTORY_SIZE 1000
#define QUEUE_DEPTH 10
#define FSW_MAX_ARG 40
#define IMAGEX 3840
#define IMAGEY 2184
#define MANAGER_ID 100
#define BUFFER_ID 200
static bool offnom = false;
static int badBufferIndex = -1;
static int releasingBufferIndex = 0;

namespace Neasc {

  // ----------------------------------------------------------------------
  // Construction and destruction 
  // ----------------------------------------------------------------------

  Tester ::
    Tester(void) : 
#if FW_OBJECT_NAMES == 1
      neascout_scienceGTestBase("Tester", MAX_HISTORY_SIZE),
      component("neascout_science")
#else
      neascout_scienceGTestBase(MAX_HISTORY_SIZE),
      component()
#endif
  {
    this->initComponents();
    this->connectPorts();
  }

  Tester ::
    ~Tester(void) 
  {
    
  }

  // ----------------------------------------------------------------------
  // Tests 
  // ----------------------------------------------------------------------


  void Tester ::
    imgstat(void)
  {

    this->clearHistory();

    Fw::CmdStringArg frame1("data/small_ut/rosetta.nsi");
    Fw::CmdStringArg frame2("data/small_ut/rosetta.nsi");

    this->sendCmd_NEAS_SCIENCE_CMD_IMGSTAT(0, 9, frame1, frame2, -32000, -5000, 5000, 32000);
    this->component.doDispatch();

    // Verify count of complete EVRs
    ASSERT_EQ(1, this->eventsSize_ImgStatComplete);

    // Verify count of asp_error EVRs
    ASSERT_EQ(0, this->eventHistory_asp_error->size());

    // Verify count of asp_log EVRs
    ASSERT_EQ(39, this->eventHistory_asp_log->size());

    const EventEntry_asp_log& a = this->eventHistory_asp_log->at(0);
    ASSERT_STREQ("3000:Allocated buffer 1\n", a.info_string.toChar());

    const EventEntry_asp_log& b = this->eventHistory_asp_log->at(1);
    ASSERT_STREQ("2100:Read a NSSI image: data/small_ut/rosetta.nsi\n", b.info_string.toChar());

    const EventEntry_asp_log& c = this->eventHistory_asp_log->at(2);
    ASSERT_STREQ("2500:data/small_ut/rosetta.nsi details:\n", c.info_string.toChar());

    const EventEntry_asp_log& d = this->eventHistory_asp_log->at(3);
    ASSERT_STREQ("format:ff01 npix:8386560 pid:105\n", d.info_string.toChar());

    const EventEntry_asp_log& e = this->eventHistory_asp_log->at(4);
    ASSERT_STREQ("time(w):100 time(s):254\n", e.info_string.toChar());

    const EventEntry_asp_log& f = this->eventHistory_asp_log->at(5);
    ASSERT_STREQ("temp:20.40000 exp:1.25000 gain:2.70000 r:2184\n", f.info_string.toChar());

    const EventEntry_asp_log& g = this->eventHistory_asp_log->at(6);
    ASSERT_STREQ("c:3840 t:0 l:0 uf:0 of:0\n", g.info_string.toChar());

    const EventEntry_asp_log& h = this->eventHistory_asp_log->at(7);
    ASSERT_STREQ("fp:1 mean:8143.02739\n", h.info_string.toChar());

    const EventEntry_asp_log& i = this->eventHistory_asp_log->at(8);
    ASSERT_STREQ("std:12803.14263 cont:4.02393\n", i.info_string.toChar());

    const EventEntry_asp_log& j = this->eventHistory_asp_log->at(9);
    ASSERT_STREQ("min v:0.00 r:2183 c:3839\n", j.info_string.toChar());

    const EventEntry_asp_log& k = this->eventHistory_asp_log->at(10);
    ASSERT_STREQ("max v:32767.00 r:2056 c:1183\n", k.info_string.toChar());

    const EventEntry_asp_log& l = this->eventHistory_asp_log->at(11);
    ASSERT_STREQ("bin0(-32767,-32000):0\n", l.info_string.toChar());

    const EventEntry_asp_log& m = this->eventHistory_asp_log->at(12);
    ASSERT_STREQ("bin1[-32000,-5000):0\n", m.info_string.toChar());

    const EventEntry_asp_log& n = this->eventHistory_asp_log->at(13);
    ASSERT_STREQ("bin2[-5000,0):0\n", n.info_string.toChar());

    const EventEntry_asp_log& o = this->eventHistory_asp_log->at(14);
    ASSERT_STREQ("bin3(px == 0):3683423\n", o.info_string.toChar());

    const EventEntry_asp_log& p = this->eventHistory_asp_log->at(15);
    ASSERT_STREQ("bin4(0,5000]:2214242\n", p.info_string.toChar());

    const EventEntry_asp_log& q = this->eventHistory_asp_log->at(16);
    ASSERT_STREQ("bin5(5000,32000]:1143027\n", q.info_string.toChar());

    const EventEntry_asp_log& r = this->eventHistory_asp_log->at(17);
    ASSERT_STREQ("bin6(32000,32767]:1345868\n", r.info_string.toChar());

    const EventEntry_asp_log& s = this->eventHistory_asp_log->at(18);
    ASSERT_STREQ("3000:Allocated buffer 2\n", s.info_string.toChar());

    const EventEntry_asp_log& t = this->eventHistory_asp_log->at(19);
    ASSERT_STREQ("2100:Read a NSSI image: data/small_ut/rosetta.nsi\n", t.info_string.toChar());

    const EventEntry_asp_log& u = this->eventHistory_asp_log->at(20);
    ASSERT_STREQ("2500:data/small_ut/rosetta.nsi details:\n", u.info_string.toChar());

    const EventEntry_asp_log& v = this->eventHistory_asp_log->at(21);
    ASSERT_STREQ("format:ff01 npix:8386560 pid:105\n", v.info_string.toChar());

    const EventEntry_asp_log& w = this->eventHistory_asp_log->at(22);
    ASSERT_STREQ("time(w):100 time(s):254\n", w.info_string.toChar());

    const EventEntry_asp_log& x = this->eventHistory_asp_log->at(23);
    ASSERT_STREQ("temp:20.40000 exp:1.25000 gain:2.70000 r:2184\n", x.info_string.toChar());

    const EventEntry_asp_log& y = this->eventHistory_asp_log->at(24);
    ASSERT_STREQ("c:3840 t:0 l:0 uf:0 of:0\n", y.info_string.toChar());

    const EventEntry_asp_log& z = this->eventHistory_asp_log->at(25);
    ASSERT_STREQ("fp:1 mean:8143.02739\n", z.info_string.toChar());

    const EventEntry_asp_log& aa = this->eventHistory_asp_log->at(26);
    ASSERT_STREQ("std:12803.14263 cont:4.02393\n", aa.info_string.toChar());

    const EventEntry_asp_log& ab = this->eventHistory_asp_log->at(27);
    ASSERT_STREQ("min v:0.00 r:2183 c:3839\n", ab.info_string.toChar());

    const EventEntry_asp_log& ac = this->eventHistory_asp_log->at(28);
    ASSERT_STREQ("max v:32767.00 r:2056 c:1183\n", ac.info_string.toChar());

    const EventEntry_asp_log& ad = this->eventHistory_asp_log->at(29);
    ASSERT_STREQ("bin0(-32767,-32000):0\n", ad.info_string.toChar());

    const EventEntry_asp_log& ae = this->eventHistory_asp_log->at(30);
    ASSERT_STREQ("bin1[-32000,-5000):0\n", ae.info_string.toChar());

    const EventEntry_asp_log& af = this->eventHistory_asp_log->at(31);
    ASSERT_STREQ("bin2[-5000,0):0\n", af.info_string.toChar());

    const EventEntry_asp_log& ag = this->eventHistory_asp_log->at(32);
    ASSERT_STREQ("bin3(px == 0):3683423\n", ag.info_string.toChar());

    const EventEntry_asp_log& ah = this->eventHistory_asp_log->at(33);
    ASSERT_STREQ("bin4(0,5000]:2214242\n", ah.info_string.toChar());

    const EventEntry_asp_log& ai = this->eventHistory_asp_log->at(34);
    ASSERT_STREQ("bin5(5000,32000]:1143027\n", ai.info_string.toChar());

    const EventEntry_asp_log& aj = this->eventHistory_asp_log->at(35);
    ASSERT_STREQ("bin6(32000,32767]:1345868\n", aj.info_string.toChar());

    const EventEntry_asp_log& ak = this->eventHistory_asp_log->at(36);
    ASSERT_STREQ("2501:data/small_ut/rosetta.nsi and data/small_ut/rosetta.nsi same\n", ak.info_string.toChar());

    const EventEntry_asp_log& al = this->eventHistory_asp_log->at(37);
    ASSERT_STREQ("3001:Returned buffer 1\n", al.info_string.toChar());

    const EventEntry_asp_log& am = this->eventHistory_asp_log->at(38);
    ASSERT_STREQ("3001:Returned buffer 2\n", am.info_string.toChar());

  }

  void Tester ::
    l1(void)
  {

    this->clearHistory();

    Fw::CmdStringArg flat1("data/small_ut/m_array.nsi");
    Fw::CmdStringArg flat2("data/small_ut/b_array.nsi");
    Fw::CmdStringArg dark("data/small_ut/dark.nsi");
    Fw::CmdStringArg bad("data/small_ut/bad.nsi");

    Fw::CmdStringArg frame1("data/small_ut/rosetta.nsi");
    Fw::CmdStringArg frame2("data/small_ut/l1.nsi");

    this->sendCmd_NEAS_SCIENCE_CMD_L1(0, 10, frame1, frame2, flat1, flat2, bad, dark, 1, 0, -99);
    this->component.doDispatch();

    // Verify count of complete EVRs
    ASSERT_EQ(1, this->eventsSize_L1Complete);

    // Verify count of asp_error EVRs
    ASSERT_EQ(0, this->eventHistory_asp_error->size());

    // Verify count of asp_log EVRs
    ASSERT_EQ(11, this->eventHistory_asp_log->size());

    const EventEntry_asp_log& a = this->eventHistory_asp_log->at(0);
    ASSERT_STREQ("3000:Allocated buffer 1\n", a.info_string.toChar());

    const EventEntry_asp_log& b = this->eventHistory_asp_log->at(1);
    ASSERT_STREQ("2100:Read a NSSI image: data/small_ut/rosetta.nsi\n", b.info_string.toChar());

    const EventEntry_asp_log& c = this->eventHistory_asp_log->at(2);
    ASSERT_STREQ("3000:Allocated buffer 2\n", c.info_string.toChar());

    const EventEntry_asp_log& d = this->eventHistory_asp_log->at(3);
    ASSERT_STREQ("3000:Allocated buffer 3\n", d.info_string.toChar());

    const EventEntry_asp_log& e = this->eventHistory_asp_log->at(4);
    ASSERT_STREQ("3000:Allocated buffer 4\n", e.info_string.toChar());

    const EventEntry_asp_log& f = this->eventHistory_asp_log->at(5);
    ASSERT_STREQ("3000:Allocated buffer 5\n", f.info_string.toChar());

    const EventEntry_asp_log& g = this->eventHistory_asp_log->at(6);
    ASSERT_STREQ("3001:Returned buffer 1\n", g.info_string.toChar());

    const EventEntry_asp_log& h = this->eventHistory_asp_log->at(7);
    ASSERT_STREQ("3001:Returned buffer 2\n", h.info_string.toChar());

    const EventEntry_asp_log& i = this->eventHistory_asp_log->at(8);
    ASSERT_STREQ("3001:Returned buffer 3\n", i.info_string.toChar());

    const EventEntry_asp_log& j = this->eventHistory_asp_log->at(9);
    ASSERT_STREQ("3001:Returned buffer 4\n", j.info_string.toChar());

    const EventEntry_asp_log& k = this->eventHistory_asp_log->at(10);
    ASSERT_STREQ("3001:Returned buffer 5\n", k.info_string.toChar());

    this->compare_equal("data/small_ut/l1.nsi", "data/small_ut/l1_ref.nsi");   

  }

  void Tester ::
    coadd(void)
  {

    this->clearHistory();

    Fw::CmdStringArg frame1("data/small_ut/nssi0000000130");
    Fw::CmdStringArg frame2("data/small_ut/nssi2000000000");

    this->sendCmd_NEAS_SCIENCE_CMD_COADD(0, 13, 7, frame1, frame2, 5, 15, 2, 10, 1);
    this->component.doDispatch();

    // Verify count of complete EVRs
    ASSERT_EQ(1, this->eventsSize_CoadditionComplete);

    // Verify count of asp_error EVRs
    ASSERT_EQ(0, this->eventHistory_asp_error->size());

    // Verify count of asp_log EVRs
    ASSERT_EQ(36, this->eventHistory_asp_log->size());

    const EventEntry_asp_log& a = this->eventHistory_asp_log->at(0);
    ASSERT_STREQ("3000:Allocated buffer 3\n", a.info_string.toChar());

    const EventEntry_asp_log& b = this->eventHistory_asp_log->at(1);
    ASSERT_STREQ("3001:Returned buffer 3\n", b.info_string.toChar());

    const EventEntry_asp_log& c = this->eventHistory_asp_log->at(2);
    ASSERT_STREQ("3000:Allocated buffer 1\n", c.info_string.toChar());

    const EventEntry_asp_log& d = this->eventHistory_asp_log->at(3);
    ASSERT_STREQ("3000:Allocated buffer 2\n", d.info_string.toChar());

    const EventEntry_asp_log& e = this->eventHistory_asp_log->at(4);
    ASSERT_STREQ("3000:Allocated buffer 6\n", e.info_string.toChar());

    const EventEntry_asp_log& f = this->eventHistory_asp_log->at(5);
    ASSERT_STREQ("3000:Allocated buffer 7\n", f.info_string.toChar());

    const EventEntry_asp_log& g = this->eventHistory_asp_log->at(6);
    ASSERT_STREQ("2600:Processing: data/small_ut/nssi0000000130\n", g.info_string.toChar());

    const EventEntry_asp_log& h = this->eventHistory_asp_log->at(7);
    ASSERT_STREQ("3000:Allocated buffer 3\n", h.info_string.toChar());

    const EventEntry_asp_log& i = this->eventHistory_asp_log->at(8);
    ASSERT_STREQ("1600:Stars found: 5\n", i.info_string.toChar());

    const EventEntry_asp_log& j = this->eventHistory_asp_log->at(9);
    ASSERT_STREQ("1800:Image 1 shift: c:0 r:0\n", j.info_string.toChar());

    const EventEntry_asp_log& k = this->eventHistory_asp_log->at(10);
    ASSERT_STREQ("2600:Processing: data/small_ut/nssi0000000131\n", k.info_string.toChar());

    const EventEntry_asp_log& l = this->eventHistory_asp_log->at(11);
    ASSERT_STREQ("3000:Allocated buffer 4\n", l.info_string.toChar());

    const EventEntry_asp_log& m = this->eventHistory_asp_log->at(12);
    ASSERT_STREQ("1800:Image 2 shift: c:-1 r:1\n", m.info_string.toChar());

    const EventEntry_asp_log& n = this->eventHistory_asp_log->at(13);
    ASSERT_STREQ("2600:Processing: data/small_ut/nssi0000000132\n", n.info_string.toChar());

    const EventEntry_asp_log& o = this->eventHistory_asp_log->at(14);
    ASSERT_STREQ("3000:Allocated buffer 5\n", o.info_string.toChar());

    const EventEntry_asp_log& p = this->eventHistory_asp_log->at(15);
    ASSERT_STREQ("1800:Image 3 shift: c:1 r:-1\n", p.info_string.toChar());

    const EventEntry_asp_log& q = this->eventHistory_asp_log->at(16);
    ASSERT_STREQ("2600:Processing: data/small_ut/nssi0000000133\n", q.info_string.toChar());

    const EventEntry_asp_log& r = this->eventHistory_asp_log->at(17);
    ASSERT_STREQ("1800:Image 4 shift: c:-1 r:1\n", r.info_string.toChar());

    const EventEntry_asp_log& s = this->eventHistory_asp_log->at(18);
    ASSERT_STREQ("2600:Processing: data/small_ut/nssi0000000134\n", s.info_string.toChar());

    const EventEntry_asp_log& t = this->eventHistory_asp_log->at(19);
    ASSERT_STREQ("1800:Image 5 shift: c:1 r:-1\n", t.info_string.toChar());

    const EventEntry_asp_log& u = this->eventHistory_asp_log->at(20);
    ASSERT_STREQ("2600:Processing: data/small_ut/nssi0000000135\n", u.info_string.toChar());

    const EventEntry_asp_log& v = this->eventHistory_asp_log->at(21);
    ASSERT_STREQ("1800:Image 6 shift: c:-1 r:1\n", v.info_string.toChar());

    const EventEntry_asp_log& w = this->eventHistory_asp_log->at(22);
    ASSERT_STREQ("2600:Processing: data/small_ut/nssi0000000136\n", w.info_string.toChar());

    const EventEntry_asp_log& x = this->eventHistory_asp_log->at(23);
    ASSERT_STREQ("1800:Image 7 shift: c:1 r:-1\n", x.info_string.toChar());

    const EventEntry_asp_log& y = this->eventHistory_asp_log->at(24);
    ASSERT_STREQ("1601:Ref Star:(r:899,c:899)\n", y.info_string.toChar());

    const EventEntry_asp_log& z = this->eventHistory_asp_log->at(25);
    ASSERT_STREQ("1601:Ref Star:(r:799,c:799)\n", z.info_string.toChar());

    const EventEntry_asp_log& aa = this->eventHistory_asp_log->at(26);
    ASSERT_STREQ("1601:Ref Star:(r:699,c:699)\n", aa.info_string.toChar());

    const EventEntry_asp_log& ab = this->eventHistory_asp_log->at(27);
    ASSERT_STREQ("1601:Ref Star:(r:599,c:599)\n", ab.info_string.toChar());

    const EventEntry_asp_log& ac = this->eventHistory_asp_log->at(28);
    ASSERT_STREQ("1601:Ref Star:(r:499,c:499)\n", ac.info_string.toChar());

    const EventEntry_asp_log& ad = this->eventHistory_asp_log->at(29);
    ASSERT_STREQ("3001:Returned buffer 1\n", ad.info_string.toChar());

    const EventEntry_asp_log& ae = this->eventHistory_asp_log->at(30);
    ASSERT_STREQ("3001:Returned buffer 2\n", ae.info_string.toChar());

    const EventEntry_asp_log& af = this->eventHistory_asp_log->at(31);
    ASSERT_STREQ("3001:Returned buffer 6\n", af.info_string.toChar());

    const EventEntry_asp_log& ag = this->eventHistory_asp_log->at(32);
    ASSERT_STREQ("3001:Returned buffer 7\n", ag.info_string.toChar());

    const EventEntry_asp_log& ah = this->eventHistory_asp_log->at(33);
    ASSERT_STREQ("3001:Returned buffer 3\n", ah.info_string.toChar());

    const EventEntry_asp_log& ai = this->eventHistory_asp_log->at(34);
    ASSERT_STREQ("3001:Returned buffer 4\n", ai.info_string.toChar());

    const EventEntry_asp_log& aj = this->eventHistory_asp_log->at(35);
    ASSERT_STREQ("3001:Returned buffer 5\n", aj.info_string.toChar());

    this->compare_equal("data/small_ut/nssi2000000000", "data/small_ut/nssi2000000000_ref");

  }

  void Tester ::
    detect(void)
  {

    this->clearHistory();

    Fw::CmdStringArg frame1("data/small_ut/nssi0000000130");
    Fw::CmdStringArg frame2("data/small_ut/nssi0000000131");
    Fw::CmdStringArg frame3("data/small_ut/detect_1.nsi");
    Fw::CmdStringArg frame4("data/small_ut/detect_2.nsi");

    this->sendCmd_NEAS_SCIENCE_CMD_DETECT(0, 11, frame1, frame2, frame3, 0, 1, 15, 5, 2, 20, 1, 10);
    this->component.doDispatch();

    this->sendCmd_NEAS_SCIENCE_CMD_DETECT(0, 11, frame1, frame2, frame4, 1, 0, 15, 5, 2, 20, 1, 10);
    this->component.doDispatch();

    // Verify count of complete EVRs
    ASSERT_EQ(2, this->eventsSize_DetectionComplete);

    // Verify count of asp_error EVRs
    ASSERT_EQ(0, this->eventHistory_asp_error->size());

    // Verify count of asp_log EVRs
    ASSERT_EQ(41, this->eventHistory_asp_log->size());

    const EventEntry_asp_log& a = this->eventHistory_asp_log->at(0);
    ASSERT_STREQ("2700:No blurring\n", a.info_string.toChar());

    const EventEntry_asp_log& b = this->eventHistory_asp_log->at(1);
    ASSERT_STREQ("3000:Allocated buffer 5\n", b.info_string.toChar());

    const EventEntry_asp_log& c = this->eventHistory_asp_log->at(2);
    ASSERT_STREQ("2100:Read image: data/small_ut/nssi0000000130\n", c.info_string.toChar());

    const EventEntry_asp_log& d = this->eventHistory_asp_log->at(3);
    ASSERT_STREQ("3000:Allocated buffer 6\n", d.info_string.toChar());

    const EventEntry_asp_log& e = this->eventHistory_asp_log->at(4);
    ASSERT_STREQ("2100:Read image: data/small_ut/nssi0000000131\n", e.info_string.toChar());

    const EventEntry_asp_log& f = this->eventHistory_asp_log->at(5);
    ASSERT_STREQ("3000:Allocated buffer 1\n", f.info_string.toChar());

    const EventEntry_asp_log& g = this->eventHistory_asp_log->at(6);
    ASSERT_STREQ("3000:Allocated buffer 2\n", g.info_string.toChar());

    const EventEntry_asp_log& h = this->eventHistory_asp_log->at(7);
    ASSERT_STREQ("1600:Stars found: 5\n", h.info_string.toChar());

    const EventEntry_asp_log& i = this->eventHistory_asp_log->at(8);
    ASSERT_STREQ("1601:Ref Star:(r:899,c:899)\n", i.info_string.toChar());

    const EventEntry_asp_log& j = this->eventHistory_asp_log->at(9);
    ASSERT_STREQ("1601:Ref Star:(r:799,c:799)\n", j.info_string.toChar());

    const EventEntry_asp_log& k = this->eventHistory_asp_log->at(10);
    ASSERT_STREQ("1601:Ref Star:(r:699,c:699)\n", k.info_string.toChar());

    const EventEntry_asp_log& l = this->eventHistory_asp_log->at(11);
    ASSERT_STREQ("1601:Ref Star:(r:599,c:599)\n", l.info_string.toChar());

    const EventEntry_asp_log& m = this->eventHistory_asp_log->at(12);
    ASSERT_STREQ("1601:Ref Star:(r:499,c:499)\n", m.info_string.toChar());

    const EventEntry_asp_log& n = this->eventHistory_asp_log->at(13);
    ASSERT_STREQ("3000:Allocated buffer 3\n", n.info_string.toChar());

    const EventEntry_asp_log& o = this->eventHistory_asp_log->at(14);
    ASSERT_STREQ("3000:Allocated buffer 4\n", o.info_string.toChar());

    const EventEntry_asp_log& p = this->eventHistory_asp_log->at(15);
    ASSERT_STREQ("1800:Aligned Image shift: c:-1 r:1\n", p.info_string.toChar());

    const EventEntry_asp_log& q = this->eventHistory_asp_log->at(16);
    ASSERT_STREQ("3000:Allocated buffer 7\n", q.info_string.toChar());

    const EventEntry_asp_log& r = this->eventHistory_asp_log->at(17);
    ASSERT_STREQ("3001:Returned buffer 5\n", r.info_string.toChar());

    const EventEntry_asp_log& s = this->eventHistory_asp_log->at(18);
    ASSERT_STREQ("3001:Returned buffer 6\n", s.info_string.toChar());

    const EventEntry_asp_log& t = this->eventHistory_asp_log->at(19);
    ASSERT_STREQ("3001:Returned buffer 1\n", t.info_string.toChar());

    const EventEntry_asp_log& u = this->eventHistory_asp_log->at(20);
    ASSERT_STREQ("3001:Returned buffer 2\n", u.info_string.toChar());

    const EventEntry_asp_log& v = this->eventHistory_asp_log->at(21);
    ASSERT_STREQ("3001:Returned buffer 3\n", v.info_string.toChar());

    const EventEntry_asp_log& w = this->eventHistory_asp_log->at(22);
    ASSERT_STREQ("3001:Returned buffer 4\n", w.info_string.toChar());

    const EventEntry_asp_log& x = this->eventHistory_asp_log->at(23);
    ASSERT_STREQ("3001:Returned buffer 7\n", x.info_string.toChar());

    const EventEntry_asp_log& y = this->eventHistory_asp_log->at(24);
    ASSERT_STREQ("2700:Blurring requested, sigma value of 1.000000\n", y.info_string.toChar());

    const EventEntry_asp_log& z = this->eventHistory_asp_log->at(25);
    ASSERT_STREQ("3000:Allocated buffer 1\n", z.info_string.toChar());

    const EventEntry_asp_log& aa = this->eventHistory_asp_log->at(26);
    ASSERT_STREQ("3000:Allocated buffer 2\n", aa.info_string.toChar());

    const EventEntry_asp_log& ab = this->eventHistory_asp_log->at(27);
    ASSERT_STREQ("2100:Read a NSSI image: data/small_ut/nssi0000000130\n", ab.info_string.toChar());

    const EventEntry_asp_log& ac = this->eventHistory_asp_log->at(28);
    ASSERT_STREQ("3000:Allocated buffer 5\n", ac.info_string.toChar());

    const EventEntry_asp_log& ad = this->eventHistory_asp_log->at(29);
    ASSERT_STREQ("3000:Allocated buffer 4\n", ad.info_string.toChar());

    const EventEntry_asp_log& ae = this->eventHistory_asp_log->at(30);
    ASSERT_STREQ("3000:Allocated buffer 3\n", ae.info_string.toChar());

    const EventEntry_asp_log& af = this->eventHistory_asp_log->at(31);
    ASSERT_STREQ("2100:Read image: data/small_ut/nssi0000000131\n", af.info_string.toChar());

    const EventEntry_asp_log& ag = this->eventHistory_asp_log->at(32);
    ASSERT_STREQ("3000:Allocated buffer 6\n", ag.info_string.toChar());

    const EventEntry_asp_log& ah = this->eventHistory_asp_log->at(33);
    ASSERT_STREQ("3000:Allocated buffer 7\n", ah.info_string.toChar());

    const EventEntry_asp_log& ai = this->eventHistory_asp_log->at(34);
    ASSERT_STREQ("3001:Returned buffer 1\n", ai.info_string.toChar());

    const EventEntry_asp_log& aj = this->eventHistory_asp_log->at(35);
    ASSERT_STREQ("3001:Returned buffer 2\n", aj.info_string.toChar());

    const EventEntry_asp_log& ak = this->eventHistory_asp_log->at(36);
    ASSERT_STREQ("3001:Returned buffer 5\n", ak.info_string.toChar());

    const EventEntry_asp_log& al = this->eventHistory_asp_log->at(37);
    ASSERT_STREQ("3001:Returned buffer 4\n", al.info_string.toChar());

    const EventEntry_asp_log& am = this->eventHistory_asp_log->at(38);
    ASSERT_STREQ("3001:Returned buffer 3\n", am.info_string.toChar());

    const EventEntry_asp_log& an = this->eventHistory_asp_log->at(39);
    ASSERT_STREQ("3001:Returned buffer 6\n", an.info_string.toChar());

    const EventEntry_asp_log& ao = this->eventHistory_asp_log->at(40);
    ASSERT_STREQ("3001:Returned buffer 7\n", ao.info_string.toChar());

    this->compare_equal("data/small_ut/detect_1.nsi", "data/small_ut/detect_1_ref.nsi");

    this->compare_equal("data/small_ut/detect_2.nsi", "data/small_ut/detect_2_ref.nsi");

  }

  void Tester ::
    crop(void) 
  {
    
    this->clearHistory(); 

    Fw::CmdStringArg frame1("data/small_ut/rosetta.nsi");
    Fw::CmdStringArg frame2("data/small_ut/cropped_1.nsi");
    Fw::CmdStringArg frame3("data/small_ut/cropped_2.nsi");
    Fw::CmdStringArg frame4("data/small_ut/cropped_3.nsi");

    this->sendCmd_NEAS_SCIENCE_CMD_CROP(0, 12, frame1, frame2, 0, 0, 500, 500, -20, 16, -4, 0, 0, 4, 1);
    this->component.doDispatch();

    this->sendCmd_NEAS_SCIENCE_CMD_CROP(0, 12, frame1, frame3, 0, 0, 500, 500, 250, 6, -4, 0, 0, 4, 1);
    this->component.doDispatch();

    this->sendCmd_NEAS_SCIENCE_CMD_CROP(0, 12, frame1, frame4, 0, 0, 500, 500, -20, -1, 4, 0, 0, 4, 1);
    this->component.doDispatch();

    // Verify count of complete EVRs
    ASSERT_EQ(3, this->eventsSize_CropCompressComplete);

    // Verify count of asp_error EVRs
    ASSERT_EQ(0, this->eventHistory_asp_error->size());

    // Verify count of asp_log EVRs
    ASSERT_EQ(51, this->eventHistory_asp_log->size());

    const EventEntry_asp_log& a = this->eventHistory_asp_log->at(0);
    ASSERT_STREQ("3000:Allocated buffer 1\n", a.info_string.toChar());

    const EventEntry_asp_log& b = this->eventHistory_asp_log->at(1);
    ASSERT_STREQ("2100:Read image: data/small_ut/rosetta.nsi\n", b.info_string.toChar());

    const EventEntry_asp_log& c = this->eventHistory_asp_log->at(2);
    ASSERT_STREQ("3000:Allocated buffer 2\n", c.info_string.toChar());

    const EventEntry_asp_log& d = this->eventHistory_asp_log->at(3);
    ASSERT_STREQ("2900:Dynamic Range: 32767\n", d.info_string.toChar());

    const EventEntry_asp_log& g = this->eventHistory_asp_log->at(4);
    ASSERT_STREQ("2802:Compressing an image of size 500 x 500\n", g.info_string.toChar());

    const EventEntry_asp_log& h = this->eventHistory_asp_log->at(5);
    ASSERT_STREQ("3000:Allocated buffer 6\n", h.info_string.toChar());

    const EventEntry_asp_log& i = this->eventHistory_asp_log->at(6);
    ASSERT_STREQ("3000:Allocated buffer 5\n", i.info_string.toChar());

    const EventEntry_asp_log& k = this->eventHistory_asp_log->at(7);
    ASSERT_STREQ("2809:Byte quota = 500000\n", k.info_string.toChar());

    const EventEntry_asp_log& l = this->eventHistory_asp_log->at(8);
    ASSERT_STREQ("3000:Allocated buffer 3\n", l.info_string.toChar());

    const EventEntry_asp_log& m = this->eventHistory_asp_log->at(9);
    ASSERT_STREQ("3000:Allocated buffer 4\n", m.info_string.toChar());

    const EventEntry_asp_log& n = this->eventHistory_asp_log->at(10);
    ASSERT_STREQ("2803:ICER_compress status word: 0\n", n.info_string.toChar());

    const EventEntry_asp_log& o = this->eventHistory_asp_log->at(11);
    ASSERT_STREQ("2806:Total compressed bytes: 85108 of 85108 (2.723456 bits/pixel)\n", o.info_string.toChar());

    const EventEntry_asp_log& p = this->eventHistory_asp_log->at(12);
    ASSERT_STREQ("2807:85108 total bytes in segment of 85108 total\n", p.info_string.toChar());

    const EventEntry_asp_log& q = this->eventHistory_asp_log->at(13);
    ASSERT_STREQ("2400:Input rate: 16.000000, lossless compression\n", q.info_string.toChar());

    const EventEntry_asp_log& r = this->eventHistory_asp_log->at(14);
    ASSERT_STREQ("3001:Returned buffer 1\n", r.info_string.toChar());

    const EventEntry_asp_log& s = this->eventHistory_asp_log->at(15);
    ASSERT_STREQ("3001:Returned buffer 2\n", s.info_string.toChar());

    const EventEntry_asp_log& t = this->eventHistory_asp_log->at(16);
    ASSERT_STREQ("3001:Returned buffer 6\n", t.info_string.toChar());

    const EventEntry_asp_log& u = this->eventHistory_asp_log->at(17);
    ASSERT_STREQ("3001:Returned buffer 5\n", u.info_string.toChar());

    const EventEntry_asp_log& v = this->eventHistory_asp_log->at(18);
    ASSERT_STREQ("3001:Returned buffer 3\n", v.info_string.toChar());

    const EventEntry_asp_log& w = this->eventHistory_asp_log->at(19);
    ASSERT_STREQ("3001:Returned buffer 4\n", w.info_string.toChar());

    const EventEntry_asp_log& x = this->eventHistory_asp_log->at(20);
    ASSERT_STREQ("3000:Allocated buffer 1\n", x.info_string.toChar());

    const EventEntry_asp_log& y = this->eventHistory_asp_log->at(21);
    ASSERT_STREQ("2100:Read image: data/small_ut/rosetta.nsi\n", y.info_string.toChar());

    const EventEntry_asp_log& z = this->eventHistory_asp_log->at(22);
    ASSERT_STREQ("2701:Searching for brightest pixel in image\n", z.info_string.toChar());

    const EventEntry_asp_log& aa = this->eventHistory_asp_log->at(23);
    ASSERT_STREQ("1900:top left : 0 0\n", aa.info_string.toChar());

    const EventEntry_asp_log& ab = this->eventHistory_asp_log->at(24);
    ASSERT_STREQ("1700:Bright pixel in the image: row:182 col:490\n", ab.info_string.toChar());

    const EventEntry_asp_log& ac = this->eventHistory_asp_log->at(25);
    ASSERT_STREQ("3000:Allocated buffer 2\n", ac.info_string.toChar());

    const EventEntry_asp_log& ad = this->eventHistory_asp_log->at(26);
    ASSERT_STREQ("2900:Dynamic Range: 32767\n", ad.info_string.toChar());

    const EventEntry_asp_log& ag = this->eventHistory_asp_log->at(27);
    ASSERT_STREQ("2802:Compressing an image of size 251 x 251\n", ag.info_string.toChar());

    const EventEntry_asp_log& ah = this->eventHistory_asp_log->at(28);
    ASSERT_STREQ("3000:Allocated buffer 6\n", ah.info_string.toChar());

    const EventEntry_asp_log& ai = this->eventHistory_asp_log->at(29);
    ASSERT_STREQ("3000:Allocated buffer 5\n", ai.info_string.toChar());

    const EventEntry_asp_log& ak = this->eventHistory_asp_log->at(30);
    ASSERT_STREQ("2809:Byte quota = 47251\n", ak.info_string.toChar());

    const EventEntry_asp_log& al = this->eventHistory_asp_log->at(31);
    ASSERT_STREQ("3000:Allocated buffer 3\n", al.info_string.toChar());

    const EventEntry_asp_log& am = this->eventHistory_asp_log->at(32);
    ASSERT_STREQ("3000:Allocated buffer 4\n", am.info_string.toChar());

    const EventEntry_asp_log& an = this->eventHistory_asp_log->at(33);
    ASSERT_STREQ("2803:ICER_compress status word: 0\n", an.info_string.toChar());

    const EventEntry_asp_log& ao = this->eventHistory_asp_log->at(34);
    ASSERT_STREQ("2806:Total compressed bytes: 41624 of 41624 (5.285503 bits/pixel)\n", ao.info_string.toChar());

    const EventEntry_asp_log& ap = this->eventHistory_asp_log->at(35);
    ASSERT_STREQ("2807:41624 total bytes in segment of 41624 total\n", ap.info_string.toChar());

    const EventEntry_asp_log& aq = this->eventHistory_asp_log->at(36);
    ASSERT_STREQ("2400:Input rate: 6.000000, lossy compression\n", aq.info_string.toChar());

    const EventEntry_asp_log& ar = this->eventHistory_asp_log->at(37);
    ASSERT_STREQ("3001:Returned buffer 1\n", ar.info_string.toChar());

    const EventEntry_asp_log& as = this->eventHistory_asp_log->at(38);
    ASSERT_STREQ("3001:Returned buffer 2\n", as.info_string.toChar());

    const EventEntry_asp_log& at = this->eventHistory_asp_log->at(39);
    ASSERT_STREQ("3001:Returned buffer 6\n", at.info_string.toChar());

    const EventEntry_asp_log& au = this->eventHistory_asp_log->at(40);
    ASSERT_STREQ("3001:Returned buffer 5\n", au.info_string.toChar());

    const EventEntry_asp_log& av = this->eventHistory_asp_log->at(41);
    ASSERT_STREQ("3001:Returned buffer 3\n", av.info_string.toChar());

    const EventEntry_asp_log& aw = this->eventHistory_asp_log->at(42);
    ASSERT_STREQ("3001:Returned buffer 4\n", aw.info_string.toChar());

    const EventEntry_asp_log& ax = this->eventHistory_asp_log->at(43);
    ASSERT_STREQ("3000:Allocated buffer 1\n", ax.info_string.toChar());

    const EventEntry_asp_log& ay = this->eventHistory_asp_log->at(44);
    ASSERT_STREQ("2100:Read image: data/small_ut/rosetta.nsi\n", ay.info_string.toChar());

    const EventEntry_asp_log& az = this->eventHistory_asp_log->at(45);
    ASSERT_STREQ("3000:Allocated buffer 2\n", az.info_string.toChar());

    const EventEntry_asp_log& ba = this->eventHistory_asp_log->at(46);
    ASSERT_STREQ("3000:Allocated buffer 7\n", ba.info_string.toChar());

    const EventEntry_asp_log& bb = this->eventHistory_asp_log->at(47);
    ASSERT_STREQ("2400:Input rate: -1.000000, skipping compression\n", bb.info_string.toChar());

    const EventEntry_asp_log& bc = this->eventHistory_asp_log->at(48);
    ASSERT_STREQ("3001:Returned buffer 1\n", bc.info_string.toChar());

    const EventEntry_asp_log& bd = this->eventHistory_asp_log->at(49);
    ASSERT_STREQ("3001:Returned buffer 2\n", bd.info_string.toChar());

    const EventEntry_asp_log& be = this->eventHistory_asp_log->at(50);
    ASSERT_STREQ("3001:Returned buffer 7\n", be.info_string.toChar());

    this->compare_equal("data/small_ut/cropped_1.nsi", "data/small_ut/cropped_1_ref.nsi");
    this->compare_equal("data/small_ut/cropped_2.nsi", "data/small_ut/cropped_2_ref.nsi");
    this->compare_equal("data/small_ut/cropped_3.nsi", "data/small_ut/cropped_3_ref.nsi");
    
  }

  void Tester ::
    mask(void) 
  {
    
    this->clearHistory(); 

    Fw::CmdStringArg frame1("data/small_ut/rosetta.nsi");
    Fw::CmdStringArg frame2("data/small_ut/masked.nsi");

    this->sendCmd_NEAS_SCIENCE_CMD_MASK_UPDATE(0, 9, frame1, frame2, 1, 100, 1);
    this->component.doDispatch();

    // Verify count of complete EVRs
    ASSERT_EQ(1, this->eventsSize_MaskUpdateComplete);

    // Verify count of asp_error EVRs
    ASSERT_EQ(0, this->eventHistory_asp_error->size());

    // Verify count of asp_log EVRs
    ASSERT_EQ(3, this->eventHistory_asp_log->size());

    const EventEntry_asp_log& a = this->eventHistory_asp_log->at(0);
    ASSERT_STREQ("3000:Allocated buffer 1\n", a.info_string.toChar());

    const EventEntry_asp_log& b = this->eventHistory_asp_log->at(1);
    ASSERT_STREQ("1601:Pixel value at r:1 c:100 set from 0 to 1\n", b.info_string.toChar());

    const EventEntry_asp_log& c = this->eventHistory_asp_log->at(2);
    ASSERT_STREQ("3001:Returned buffer 1\n", c.info_string.toChar());

    this->compare_equal("data/small_ut/masked.nsi", "data/small_ut/masked_ref.nsi");

  }

  // ----------------------------------------------------------------------
  // Handlers for typed from ports
  // ----------------------------------------------------------------------

  Fw::Buffer Tester ::
    from_bufferGetCaller_handler(
        const NATIVE_INT_TYPE portNum,
        U32 size
    )
  {
    this->pushFromPortEntry_bufferGetCaller(size);
    if(releasingBufferIndex == badBufferIndex)
        offnom = true;
    releasingBufferIndex += 1;

    if (offnom) {

       U8 *const data_off = new U8[size-1];

        Fw::Buffer buffer_offnom(
           MANAGER_ID,
           BUFFER_ID,
           reinterpret_cast<U64>(data_off),
           size-1
        );

        return buffer_offnom;

    } 
    else {

        U8 *const data = new U8[size];

        Fw::Buffer buffer(
           MANAGER_ID,
           BUFFER_ID,
           reinterpret_cast<U64>(data),
           size
        );
        return buffer;


    }

    return 0;
  }

  void Tester ::
    from_PingResponse_handler(
        const NATIVE_INT_TYPE portNum,
        U32 key
    )
  {
    this->pushFromPortEntry_PingResponse(key);
  }

  void Tester ::
    from_bufferReturn_handler(
        const NATIVE_INT_TYPE portNum,
        Fw::Buffer fwBuffer
    )
  {
    this->pushFromPortEntry_bufferReturn(fwBuffer);
  }

  // ----------------------------------------------------------------------
  // Helper methods 
  // ----------------------------------------------------------------------

  void Tester :: 
  compare_equal(char *file1, char *file2){

     FILE *fp1, *fp2;
     int ch1, ch2;
   
     fp1 = fopen(file1, "r");
     fp2 = fopen(file2, "r");
   
     ASSERT_TRUE(fp1 != NULL);
     ASSERT_TRUE(fp2 != NULL);
 
     ch1 = getc(fp1);
     ch2 = getc(fp2);

     while ((ch1 != EOF) && (ch2 != EOF) && (ch1 == ch2)) {
        ch1 = getc(fp1);
        ch2 = getc(fp2);
        ASSERT_EQ(ch1, ch2);
     }
 
     fclose(fp1);
     fclose(fp2);

  }


  void Tester ::
    connectPorts(void) 
  {

    // cmdIn
    this->connect_to_cmdIn(
        0,
        this->component.get_cmdIn_InputPort(0)
    );

    // PingRecv
    this->connect_to_PingRecv(
        0,
        this->component.get_PingRecv_InputPort(0)
    );

    // timeCaller
    this->component.set_timeCaller_OutputPort(
        0, 
        this->get_from_timeCaller(0)
    );

    // cmdRegOut
    this->component.set_cmdRegOut_OutputPort(
        0, 
        this->get_from_cmdRegOut(0)
    );

    // eventOut
    this->component.set_eventOut_OutputPort(
        0, 
        this->get_from_eventOut(0)
    );

    // bufferGetCaller
    this->component.set_bufferGetCaller_OutputPort(
        0, 
        this->get_from_bufferGetCaller(0)
    );

    // PingResponse
    this->component.set_PingResponse_OutputPort(
        0, 
        this->get_from_PingResponse(0)
    );

    // bufferReturn
    this->component.set_bufferReturn_OutputPort(
        0, 
        this->get_from_bufferReturn(0)
    );

    // cmdResponseOut
    this->component.set_cmdResponseOut_OutputPort(
        0, 
        this->get_from_cmdResponseOut(0)
    );

    // LogText
    this->component.set_LogText_OutputPort(
        0, 
        this->get_from_LogText(0)
    );

  }

  void Tester ::
    initComponents(void) 
  {
    this->init();
    this->component.init(
        QUEUE_DEPTH, INSTANCE
    );
  }

} // end namespace Neasc
