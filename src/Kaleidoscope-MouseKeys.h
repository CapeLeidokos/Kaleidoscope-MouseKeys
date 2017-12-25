#pragma once

#include "Kaleidoscope.h"
#include "MouseKeyDefs.h"
#include "MouseWrapper.h" // Moved here to allow for inlining MouseWrapper.begin()

class MouseKeys_ : public KaleidoscopePlugin {
 public:
  MouseKeys_(void);
  
  void init() {
    MouseWrapper.begin();
  }
     
  void preReportHook();
  
  void postReportHook() {
     mouseMoveIntent = 0; // inlided to speed things up
  }
  
  // Note the explicit namespace specification kaleidoscope:: can be ommited as soon
  // as MouseKeys_ lives in a namespace below kaleidoscope.
  //
  bool eventHandlerHook(kaleidoscope::Key &mappedKey, const kaleidoscope::EventKey &eventKey);

  static uint8_t speed;
  static uint16_t speedDelay;
  static uint8_t accelSpeed;
  static uint16_t accelDelay;
  static uint8_t wheelSpeed;
  static uint16_t wheelDelay;

 private:
  static uint8_t mouseMoveIntent;
  static uint32_t endTime;
  static uint32_t accelEndTime;
  static uint32_t wheelEndTime;

  static void scrollWheel(uint8_t keyCode);
};

extern MouseKeys_ MouseKeys;
