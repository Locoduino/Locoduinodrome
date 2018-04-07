//-------------------------------------------------------------------
#ifndef __balise_H__
#define __balise_H__
//-------------------------------------------------------------------

/** Cette classe repr�sente une d�tection d zone. elle peut avoir deux �tats : libre ou occup�e.
Il y a une seule pin utilis�e pour la d�tection.
*/
class Balise : public Objet
{
	unsigned long intervalle;

	// Ne pas sauver ces donn�es dans la config !
	bool activee;
	unsigned long precedentTest;

public:
	Balise();

	void begin(uint8_t inPin);
	void loop();

	bool EstActivee() {	return this->activee; }

	uint8_t GetEEPROMSize() { return Objet::GetEEPROMSize() + sizeof(unsigned long); }
	uint8_t EEPROM_chargement(int inAddr);
	uint8_t EEPROM_sauvegarde(int inAddr);
};
#endif
