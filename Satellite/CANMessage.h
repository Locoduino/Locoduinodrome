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

enum AIGUILLE_CONFIG_TYPE
{
	Minimum = B000,
	Maximum = B001,
	Speed = B010
};

enum LED_CONFIG_TYPE
{
	Power = B000,
	TimeToOn = B001,
	TimeToOff = B010,
	TimeON = B011,
	Period = B100
};

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
  uint8_t configButeeState();
  void receive(uint8_t *inData);

  bool IsConfig();
  bool IsPermanentConfig();
  uint8_t LedToConfig();		// 255 si pas une Led...
  uint8_t AiguilleToConfig();	// 255 si pas une aiguille
  uint8_t DetecteurToConfig();	// 255 si pas un detecteur
  AIGUILLE_CONFIG_TYPE AiguilleConfigType();
  LED_CONFIG_TYPE LedConfigType();
  uint8_t ConfigByteValue();
  uint16_t ConfigIntValue();
  float ConfigFloatValue();
};

class StatusCANMessage
{
private:
  uint8_t dData[3];

public:
  StatusCANMessage();
  void setDetection(uint8_t *inData, const uint8_t inDetectNumber, bool inDetectState);
};

#endif /* __CAN_MESSAGE_H__ */
