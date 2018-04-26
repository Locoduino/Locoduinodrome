/*
 * LOCODUINO
 *
 * Locoduinodrome
 *
 * Classe pour les messages CAN
 */

#ifndef __CAN_MESSAGE_H__
#define __CAN_MESSAGE_H__

#include <stdint.h>

/*
 * Message CAN de commande venant du gestionnaire
 */
class CommandCANMessage
{
private:
  uint8_t mData[3];
  uint8_t dData[3];

public:
  CommandCANMessage();
  uint8_t ledState(const uint8_t inLedNumber);
  bool pointState();
  uint8_t buteeState();
  void receive(uint8_t *inData);
};

class StatusCANMessage
{
private:
  uint8_t dData[3];

public:
  StatusCANMessage();
  void setDetection(const uint8_t inDetectNumber, bool inDetectState);
};

extern  StatusCANMessage statusMessage;

#endif /* __CAN_MESSAGE_H__ */
