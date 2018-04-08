//-------------------------------------------------------------------
#ifndef __objet_H__
#define __objet_H__
//-------------------------------------------------------------------

/** Cette classe est la classe de base de tous les objet utilisant une seule broche.
*/
class Objet
{
protected:
	uint8_t pin;

public:
	Objet()	{ this->pin = 255; }

	bool IsValid() { return this->pin != 255; }

	virtual void begin(uint8_t inPin) = 0;
	virtual void loop(uint8_t inNewState) = 0;
	virtual void loopPrioritaire() {}

	virtual uint8_t GetEEPROMSize() { return 0;	}

	virtual uint8_t EEPROM_chargement(int inAddr) { return inAddr; }
	virtual uint8_t EEPROM_sauvegarde(int inAddr) { return inAddr; }
};

// Assure la compatibilit� avec mes fonctions EEPROM dans Visual Studio...
#ifdef VISUALSTUDIO
#define EEPROMGET(SRC, DATA, SIZE)	EEPROM.get(SRC, &DATA, SIZE)
#define EEPROMPUT(SRC, DATA, SIZE)	EEPROM.put(SRC, &DATA, SIZE)
#else
#define EEPROMGET(SRC, DATA, SIZE)	EEPROM.get(SRC, DATA)
#define EEPROMPUT(SRC, DATA, SIZE)	EEPROM.put(SRC, DATA)
#endif

#endif
