/*
 * LOCODUINO
 *
 * Locoduinodrome
 *
 * Classe pour les messages CAN
 */

#include "CANMessage.h"
#include "Messaging.h"

/*
 * Message CAN de commande venant du gestionnaire
 */
CommandCANMessage::CommandCANMessage()
{
  /* Initialise la trame à toutes les leds OFF et aiguillage à false */
  mData[0] = mData[1] =
    (LED_OFF << 6) | (LED_OFF << 4) | (LED_OFF << 2) | LED_OFF;
  mData[2] = (false << 7) | LED_OFF;
  /* Initialise la trame des détecteurs tous Libres */
  dData[0] = 0;
}

uint8_t CommandCANMessage::ledState(const uint8_t inLedNumber)
{
  uint8_t element = inLedNumber >> 2;
  uint8_t offset = (inLedNumber & 0x3) << 1;
  return (mData[element] >> offset) & 0x3;
}

bool CommandCANMessage::pointState()
{
  return (mData[2] & 0x80) != 0;
}

void CommandCANMessage::receive(uint8_t *inData)
{
  mData[0] = inData[0];
  mData[1] = inData[1];
  mData[2] = inData[2];
}

void CommandCANMessage::transmit(unsigned char *outData)
{
  outData[0] = dData[0];
}

