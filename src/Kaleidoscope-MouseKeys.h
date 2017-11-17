#pragma once

#include "Kaleidoscope.h"
#include "MouseKeyDefs.h"
#include "MouseWrapper.h" // Moved here to allow for inlining MouseWrapper.begin()

class MouseKeys_ : public KaleidoscopePlugin {
 public:
  MouseKeys_(void);

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
  
  void init() {
    MouseWrapper.begin();
  }
     
  void startLoopHook();
  
  void endLoopHook() {
     mouseMoveIntent = 0; // inlided to speed things up
  }
  
  bool eventHandlerHook(Key &mappedKey, const EventKey &eventKey);
};

extern MouseKeys_ MouseKeys;
