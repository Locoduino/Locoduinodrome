//-------------------------------------------------------------------
#ifndef __aiguille_H__
#define __aiguille_H__
//-------------------------------------------------------------------

#include "SlowMotionServo.h"

/** Cette classe repr�sent une aiguille. Une aiguille poss�de deux positions : droite et d�vi�e. Elle est pilot�e par une broche unique.
*/
class Aiguille : public Objet
{
	unsigned int posDroit;
	unsigned int posDeviee;
	float vitesse;

	SMSSmoothBounce Servomoteur;
	
public:
	Aiguille();

	void begin(uint8_t inPin);
	void loop();

	uint8_t GetEEPROMSize()	{ return Objet::GetEEPROMSize() + (2 * sizeof(unsigned int)) + sizeof(float); }
	uint8_t EEPROM_chargement(int inAddr);
	uint8_t EEPROM_sauvegarde(int inAddr);
};
#endif
