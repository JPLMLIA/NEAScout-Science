/**
 * \file    asp_error.cc
 * \brief   Defs for error reporting methods used in asp.
 * \author  Steve Schaffer, Jack Lightholder
 * 
 * Copyright 2011 California Institute of Technology.  ALL RIGHTS RESERVED.
 * U.S. Government Sponsorship acknowledged.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "asp_error.h"

  // Global buffers for storing EVRs coming from C ASP library. 
  //   F' component grabs from these buffers when reporting EVRs.
  char errorEventBuffer[BUFFER_DEPTH][ASP_MAX_LOGSTRING];
  char logEventBuffer[BUFFER_DEPTH][ASP_MAX_LOGSTRING];

  // Counters for tracking which EVRs have been captured/reported
  int errorBufferIndex = 0;
  int logBufferIndex = 0;
  

  void reset_error_buffer()
  {
   /**
    *
    * Once report_error_events() has cleared the buffer of pending
    *   error messages, this function resets the errorEventsBuffer and
    *   associated errorBufferIndex counter. Makes room for new event 
    *   strings in the ASP error reporting buffer.
    *
    */
    for(int i=0; i<BUFFER_DEPTH; i++) {
         (void)memset(errorEventBuffer[i], 0, ASP_MAX_LOGSTRING);
    }
   
     /* Indicate errorEventBuffer is now empty and 
          available for storing new errors */
     errorBufferIndex = 0;

  }


  void reset_log_buffer()
  {
   /**
    *
    * Once report_log_events() has cleared the buffer of pending
    *   log messages, this function resets the logEventsBuffer and
    *   associated logBufferIndex counter. Makes room for new event 
    *   strings in the ASP logging buffer.
    *
    */
    for(int i=0; i<BUFFER_DEPTH; i++) {
        (void)memset(logEventBuffer[i], 0, ASP_MAX_LOGSTRING);
    }
  
    /* Indicate logEventBuffer is now empty and 
         available for storing new log messages */
    logBufferIndex = 0;

  }



  void asp_error(const asp_error_code_t errorCode,
                 const char* errorMessage)
  {
   /**
    *
    * @param errorCode    error code used by ground software to identify software position
    * @param errorMessage dynamic string reporting error messages of software status. 
    *
    * Error reporting function used in ASP c library. Stores messages as strings in global 
    *   errorEventBuffer for eventual transmission from c++ report_error_events function.  
    *   Sent as WARNING_LO EVRs.
    *  
    * A list of possible error codes can be found in asp_error.h
    * 
    */
    char stringValue[ASP_MAX_LOGSTRING-1];	

    if(errorBufferIndex < BUFFER_DEPTH){
        (void)snprintf(stringValue, ASP_MAX_LOGSTRING-1, "%x:%s\n", errorCode, errorMessage);
        (void)strncpy(errorEventBuffer[errorBufferIndex], stringValue, ASP_MAX_LOGSTRING);
        errorBufferIndex += 1;
    }

  }


  void asp_log(const asp_log_code_t logCode,
               const char* logMessage)
  {
   /**
    *
    * @param logCode    Logging code used by ground software to identify software position
    * @param logMessage Dynamic string reporting log messages of software status. 
    *
    * Logging function used in ASP c library. Stores messages as strings in global 
    *   logEventBuffer for eventual transmission from c++ report_log_events function.  
    *   Sent as ACTIVITY_LO EVRs. 
    *
    * A list of possible logging codes can be found in asp_error.h
    *
    */
    char stringValue[ASP_MAX_LOGSTRING-1];
  
    if(logBufferIndex < BUFFER_DEPTH){
         (void)snprintf(stringValue, ASP_MAX_LOGSTRING-1, "%x:%s\n", logCode, logMessage);
         (void)strncpy(logEventBuffer[logBufferIndex], stringValue, ASP_MAX_LOGSTRING);
         logBufferIndex += 1;
    }

  }


  void asp_imgstat(const char* logMessage)
  {
   /**
    *
    * @param logMessage dynamic string reporting image statistics in IMGSTAT command. 
    *
    * Similar to asp_log, stores messages as strings in global logEventBuffer for eventual 
    *   transmission from c++ report_log_events function. Sent as ACTIVITY_LO EVRs. Kept
    *   separate from asp_log so logCode does not appear in front of every chunk of the IMGSTAT
    *   information block.  Helps with ground parsing and minimizes data volume.
    *
    */
    char stringValue[ASP_MAX_LOGSTRING-1];

    if(logBufferIndex < BUFFER_DEPTH){
        (void)snprintf(stringValue, ASP_MAX_LOGSTRING-1, "%s", logMessage);
        (void)strncpy(logEventBuffer[logBufferIndex], stringValue, ASP_MAX_LOGSTRING);
        logBufferIndex += 1;
    }

  }



