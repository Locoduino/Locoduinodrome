//-------------------------------------------------------------------
#ifndef __zone_H__
#define __zone_H__
//-------------------------------------------------------------------

/** Cette classe repr�sente une d�tection d zone. elle peut avoir deux �tats : libre ou occup�e.
Il y a une seule pin utilis�e pour la d�tection.
*/
class Zone : public Objet
{
	unsigned long intervalle;

	// Ne pas sauver ces donn�es dans la config !
	bool occupee;
	unsigned long precedentTest;

public:
	Zone();

	void begin(uint8_t inPin);
	void loop();

	bool EstOccupee() { return this->occupee; }

	uint8_t GetEEPROMSize() { return Objet::GetEEPROMSize() + sizeof(unsigned long); }
	uint8_t EEPROM_chargement(int inAddr);
	uint8_t EEPROM_sauvegarde(int inAddr);
};
#endif
