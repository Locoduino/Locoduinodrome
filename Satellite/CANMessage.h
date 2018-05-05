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
	Min = B000,
	Max = B001,
	Speed = B010
};

enum LED_CONFIG_TYPE
{
	Maximum = B000,
	FadingTime = B001,
	BrighteningTime = B010,
	OnTime = B011,
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

  bool IsConfig() { return false; }
  bool IsPermanentConfig() { return false; }
  uint8_t LedToConfig() { return 255; }			// 255 si pas une Led...
  uint8_t AiguilleToConfig() { return 255; }	// 255 si pas une aiguille
  uint8_t DetecteurToConfig() { return 255; }	// 255 si pas un detecteur
  AIGUILLE_CONFIG_TYPE AiguilleConfigType() { return AIGUILLE_CONFIG_TYPE::Max; }
  LED_CONFIG_TYPE LedConfigType() { return LED_CONFIG_TYPE::Maximum; }
  uint8_t ConfigByteValue() { return 0; }
  uint16_t ConfigIntValue() { return 0; }
  float ConfigFloatValue() { return 0.f; }
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
