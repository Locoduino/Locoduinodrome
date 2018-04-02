/*
 * LOCODUINO
 *
 * Locoduinodrome
 *
 * Classe pour les messages CAN
 */

#ifndef __ABSTRACT_CAN_MESSAGE_H__
#define __ABSTRACT_CAN_MESSAGE_H__

const uint8_t NO_SATELLITE_ID = 255;

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
};

#endif
