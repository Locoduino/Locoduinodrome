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

public:
  CommandCANMessage();
  uint8_t ledState(const uint8_t inLedNumber);
  bool pointState();
  void receive(uint8_t *inData);
    void transmit(uint8_t *inData);
};

#endif /* __CAN_MESSAGE_H__ */
