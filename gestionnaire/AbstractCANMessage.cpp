/*
 * LOCODUINO
 *
 * Locoduinodrome
 *
 * Classe pour les messages CAN
 */

#include "AbstractCANMessage.h"

static const NUMBER_OF_LED = 9;
static const LED_OFF = 0x0;
static const LED_BLINK = 0x1;
static const LED_ON = 0x2;

AbstractCANOutSatelliteMessage::AbstractCANOutSatelliteMessage() :
  mSatelliteId(NO_SATELLITE_ID)
{
  /* Valeurs par défaut pour les commandes */
  setPointPosition(false);
  for (uint8_t led = 0; led < NUMBER_OF_LED; led++) {
    setLED(led, LED_OFF);
  }
}

void setPointPosition(const bool inPosition)
{
  mData[2] &= 0x7F; /* reset the point position */
  mData[2] |= (inPosition ? 0x80 : 0x00); /* set the point position in the frame */
}

void setLED(const uint8_t inLED, const uint8_t inState)
{
  if (inLED <= 9) {
    uint8_t byteNum = inLED >> 2;
    uint8_t offsetInByte = inLED & 0x03;
    mData[byteNum] &= ~(0x03 << offsetInByte); /* reset */
    mData[byteNum] |= ((inState & 0x03) << offsetInByte);
  }
}
