/*
 * LOCODUINO
 *
 * Locoduinodrome
 *
 * Classe pour les messages CAN
 */

#include "AbstractCANMessage.h"
#include "mcp_can.h"

static const uint8_t NUMBER_OF_LED = 9;
static const uint32_t OUT_SATELLITE_MESSAGE_TYPE = 4;
static const uint32_t NUMBER_OF_BITS_FOR_SATELLITE_ID = 3;

extern MCP_CAN canController;

AbstractCANOutSatelliteMessage::AbstractCANOutSatelliteMessage() :
  mSatelliteId(NO_SATELLITE_ID)
{
  /* Valeurs par défaut pour les commandes */
  setPointPosition(false);
  for (uint8_t led = 0; led < NUMBER_OF_LED; led++) {
    setLED(led, LED_OFF);
  }
}

void AbstractCANOutSatelliteMessage::setPointPosition(const bool inPosition)
{
  mData[2] &= 0x7F; /* reset the point position */
  mData[2] |= (inPosition ? 0x80 : 0x00); /* set the point position in the frame */
}

void AbstractCANOutSatelliteMessage::setLED(const uint8_t inLED, const uint8_t inState)
{
  if (inLED <= 9) {
    uint8_t byteNum = inLED >> 2;
    uint8_t offsetInByte = (inLED & 0x03) << 1;
    mData[byteNum] &= ~(0x03 << offsetInByte); /* reset */
    mData[byteNum] |= ((inState & 0x03) << offsetInByte);
  }
}

void AbstractCANOutSatelliteMessage::print()
{
  Serial.print('<');
  Serial.print(mSatelliteId);
  Serial.print("> ");
  for (uint8_t led = 0; led < 9; led++) {
    uint8_t element = led >> 2;
    uint8_t offset = (led & 0x3) << 1;
    uint8_t ledState = (mData[element] >> offset) & 0x3;
    Serial.print('[');
    switch (ledState) {
      case LED_OFF:   Serial.print(' '); break;
      case LED_ON:    Serial.print('+'); break;
      case LED_BLINK: Serial.print('^'); break;
    }
    Serial.print("] ");
  }
  Serial.print('<');
  Serial.print(mData[2] & 0x80 ? '/' : '|');
  Serial.print('>');
}

void AbstractCANOutSatelliteMessage::send()
{
  uint32_t frameId =
    (OUT_SATELLITE_MESSAGE_TYPE << NUMBER_OF_BITS_FOR_SATELLITE_ID) | mSatelliteId;
  canController.sendMsgBuf(frameId, 0, 3, mData);
}
