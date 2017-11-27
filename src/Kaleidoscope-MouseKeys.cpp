#include <Arduino.h>

#include "Kaleidoscope-MouseKeys.h"
#include "Kaleidoscope.h"

uint8_t MouseKeys_::mouseMoveIntent;

uint8_t MouseKeys_::speed = 1;
uint16_t MouseKeys_::speedDelay = 1;

uint8_t MouseKeys_::accelSpeed = 1;
uint16_t MouseKeys_::accelDelay = 64;

uint8_t MouseKeys_::wheelSpeed = 1;
uint16_t MouseKeys_::wheelDelay = 50;

uint32_t MouseKeys_::accelEndTime;
uint32_t MouseKeys_::endTime;
uint32_t MouseKeys_::wheelEndTime;

void MouseKeys_::scrollWheel(uint8_t keyCode) {
  if (millis() < wheelEndTime)
    return;

  wheelEndTime = millis() + wheelDelay;

  if (keyCode & KEY_MOUSE_UP)
    kaleidoscope::hid::moveMouse(0, 0, wheelSpeed);
  else if (keyCode & KEY_MOUSE_DOWN)
    kaleidoscope::hid::moveMouse(0, 0, -wheelSpeed);
}

void MouseKeys_::preReportHook()
{
  if (mouseMoveIntent == 0) {
    MouseWrapper.accelStep = 0;
    endTime = 0;
    accelEndTime = 0;
    return;
  }

  if (millis() < endTime)
    return;

  endTime = millis() + speedDelay;

  int8_t moveX = 0, moveY = 0;

  if (millis() >= accelEndTime) {
    if (MouseWrapper.accelStep < 255 - accelSpeed)
      MouseWrapper.accelStep += accelSpeed;
  }

  if (mouseMoveIntent & KEY_MOUSE_UP)
    moveY = -speed;
  else if (mouseMoveIntent & KEY_MOUSE_DOWN)
    moveY = speed;

  if (mouseMoveIntent & KEY_MOUSE_LEFT)
    moveX = -speed;
  else if (mouseMoveIntent & KEY_MOUSE_RIGHT)
    moveX = speed;

  MouseWrapper.move(moveX, moveY);
}

bool MouseKeys_::eventHandlerHook(Key &mappedKey, const EventKey &eventKey)
{
  if (mappedKey.flags != (SYNTHETIC | IS_MOUSE_KEY))
    return true; // Do not modify mappedKey and let other handlers 
                 // be called afterwards.

  if (mappedKey.keyCode & KEY_MOUSE_BUTTON && !(mappedKey.keyCode & KEY_MOUSE_WARP)) {
    uint8_t button = mappedKey.keyCode & ~KEY_MOUSE_BUTTON;

    if (keyToggledOn(eventKey.keyState_)) {
      MouseWrapper.pressButton(button);
    } else if (keyToggledOff(eventKey.keyState_)) {
      MouseWrapper.release_button(button);
    }
  } else if (!(mappedKey.keyCode & KEY_MOUSE_WARP)) {
    if (keyToggledOn(eventKey.keyState_)) {
      endTime = millis() + speedDelay;
      accelEndTime = millis() + accelDelay;
      wheelEndTime = 0;
    }
    if (keyIsPressed(eventKey.keyState_)) {
      if (mappedKey.keyCode & KEY_MOUSE_WHEEL) {
        scrollWheel(mappedKey.keyCode);
      } else {
        mouseMoveIntent |= mappedKey.keyCode;
      }
    }
  } else if (keyToggledOn(eventKey.keyState_)) {
    if (mappedKey.keyCode & KEY_MOUSE_WARP && mappedKey.flags & IS_MOUSE_KEY) {
      // we don't pass in the left and up values because those are the
      // default, "no-op" conditionals
      MouseWrapper.warp(((mappedKey.keyCode & KEY_MOUSE_WARP_END) ? WARP_END : 0x00) |
                        ((mappedKey.keyCode & KEY_MOUSE_DOWN) ? WARP_DOWN : 0x00) |
                        ((mappedKey.keyCode & KEY_MOUSE_RIGHT) ? WARP_RIGHT : 0x00));
    }
  }

  mappedKey = Key_NoKey; // mappedKey is passed by reference, 
                         // so we can simply modify it.
  
  // Prevent call of other plugins' handlers
  //
  return false;
}

MouseKeys_::MouseKeys_(void) {
}

MouseKeys_ MouseKeys;
