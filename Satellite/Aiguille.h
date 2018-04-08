//-------------------------------------------------------------------
#ifndef __aiguille_H__
#define __aiguille_H__
//-------------------------------------------------------------------

#include "SlowMotionServo.h"

/** Cette classe représente une aiguille. Une aiguille possède deux positions : droite et déviée. Elle est pilotée par une broche unique.
*/
class Aiguille : public Objet
{
	// Configuration
	unsigned int posDroit;
	unsigned int posDeviee;
	float vitesse;

	// Gestion locale
	SMSSmoothBounce Servomoteur;
	bool estDroit;
	
public:
	Aiguille();

	void begin(uint8_t inPin);
	void loop(uint8_t inNewState);
	void loopPrioritaire() { SlowMotionServo::update(); }

	uint8_t GetEEPROMSize()	{ return Objet::GetEEPROMSize() + (2 * sizeof(unsigned int)) + sizeof(float); }
	uint8_t EEPROM_chargement(int inAddr);
	uint8_t EEPROM_sauvegarde(int inAddr);
};
#endif
