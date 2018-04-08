//-------------------------------------------------------------------
#ifndef __detecteur_H__
#define __detecteur_H__
//-------------------------------------------------------------------

/** Cette classe repr�sente une d�tection d zone. elle peut avoir deux �tats : libre ou occup�e.
Il y a une seule pin utilis�e pour la d�tection.
*/
class Detecteur : public Objet
{
	// Configuration
	unsigned long intervalle;

	// Gestion locale
	bool estDetecte;
	unsigned long precedentTest;

public:
	Detecteur();

	void begin(uint8_t inPin);
	void loop(uint8_t inNewState);

	bool EstActivee() {	return this->estDetecte; }

	uint8_t GetEEPROMSize() { return Objet::GetEEPROMSize() + sizeof(unsigned long); }
	uint8_t EEPROM_chargement(int inAddr);
	uint8_t EEPROM_sauvegarde(int inAddr);
};
#endif
