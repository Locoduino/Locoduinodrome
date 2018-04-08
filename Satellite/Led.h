//-------------------------------------------------------------------
#ifndef __led_H__
#define __led_H__
//-------------------------------------------------------------------

#include "LightDimmer.h"

/* Classe pour une Led, avec sa configuration, son �tat et ses fonctions de manipulation.
*/
class Led : public Objet
{
	// Configuration
	unsigned int valeurPWM;
	unsigned long clignotement;
	unsigned long dimming;

	// Gestion locale
	LightDimmerSoft dimmer;

public:
	Led();

	void begin(uint8_t inPin);
	void loop(uint8_t inNewState);
	void loopPrioritaire() { LightDimmer::update();	}

	uint8_t GetEEPROMSize() { return Objet::GetEEPROMSize() + sizeof(unsigned int) + (2 * sizeof(unsigned long)); }
	uint8_t EEPROM_chargement(int inAddr);
	uint8_t EEPROM_sauvegarde(int inAddr);
};
#endif
