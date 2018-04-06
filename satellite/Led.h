
/* Classe pour une Led, avec sa configuration, son état et ses fonctions de manipulation.
*/
class Led : public Objet
{
	unsigned int valeurPWM;
	unsigned long clignotement;
	unsigned long dimming;

public:

	Led()
	{
		this->valeurPWM = 255;
		this->clignotement = 500;
		this->dimming = 250;
	}

	void begin(uint8_t inPin)
	{
		this->pin = inPin;
		pinMode(this->pin, OUTPUT);
	}

	void loop()
	{
	}
	
	uint8_t GetEEPROMSize() { return Objet::GetEEPROMSize() + sizeof(unsigned int) + (2 * sizeof(unsigned long)); }

	uint8_t EEPROM_chargement(int inAddr)
	{
		int addr = Objet::EEPROM_chargement(inAddr);

		EEPROM.get(addr, &this->valeurPWM, sizeof(unsigned int));
		addr += sizeof(unsigned int);
		EEPROM.get(addr, &this->clignotement, sizeof(unsigned long));
		addr += sizeof(unsigned long);
		EEPROM.get(addr, &this->dimming, sizeof(unsigned long));
		addr += sizeof(unsigned long);
		return addr;
	}

	uint8_t EEPROM_sauvegarde(int inAddr)
	{
		int addr = Objet::EEPROM_sauvegarde(inAddr);

		EEPROM.put(addr, &this->valeurPWM, sizeof(unsigned int));
		addr += sizeof(unsigned int);
		EEPROM.put(addr, &this->clignotement, sizeof(unsigned long));
		addr += sizeof(unsigned long);
		EEPROM.put(addr, &this->dimming, sizeof(unsigned long));
		addr += sizeof(unsigned long);

		return addr;
	}


	/*void feu(byte f)
	{ // feu
		if (f == X) { messageTx(CODE_ERREUR, 9, 2); return; }
		digitalWrite(f, 1);
	}

	void feu_cli(byte f) { // feu clignotant
		if (f == X) { messageTx(CODE_ERREUR, 9, 2); return; }
		feuCli = f;
	}

	void oeilleton() { // oeilleton
		if (o == X) { messageTx(CODE_ERREUR, 9, 3); return; }
		digitalWrite(o, 1);
	}

	void clignotement(boolean b) { if (feuCli != ETEINT) digitalWrite(feuCli, b ? 1 : 0); }*/
};
