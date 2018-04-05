/*
 * LOCODUINO
 *
 * Locoduinodrome
 *
 * Classe pour les messages CAN
 */

#ifndef __ABSTRACT_CAN_MESSAGE_H__
#define __ABSTRACT_CAN_MESSAGE_H__

#include "Arduino.h"

const uint8_t NO_SATELLITE_ID = 255;

static const uint8_t LED_OFF = 0x0;
static const uint8_t LED_BLINK = 0x1;
static const uint8_t LED_ON = 0x2;

class AbstractCANOutSatelliteMessage
{
private:
  uint8_t mSatelliteId;
  uint8_t mData[3];

public:
  AbstractCANOutSatelliteMessage();

  void setPointPosition(const bool inPosition);
  void setLED(const uint8_t inLED, const uint8_t inState);
  uint8_t satelliteId() { return mSatelliteId; }
  void reserve(const uint8_t inSatelliteId) { mSatelliteId = inSatelliteId; }
};

#endif
