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
	Balise()
	{
		this->intervalle = 500;
		this->precedentTest = 0;
	}

	void begin(uint8_t inPin)
	{
		this->pin = inPin;
		this->activee = digitalRead(this->pin) == 1;
		pinMode(this->pin, INPUT);
	}

	void loop()
	{
		if (millis() - this->precedentTest < this->intervalle)
			return;

		bool activ = digitalRead(this->pin) == 1;
		if (activ != this->activee)
		{
			this->activee = activ;
		}
	}

	bool EstActivee() {	return this->activee; }

	uint8_t GetEEPROMSize() { return Objet::GetEEPROMSize() + sizeof(unsigned long); }

	uint8_t EEPROM_chargement(int inAddr)
	{
		int addr = Objet::EEPROM_chargement(inAddr);

		EEPROM.get(addr, &this->intervalle, sizeof(unsigned long));
		addr += sizeof(unsigned long);
		return addr;
	}

	uint8_t EEPROM_sauvegarde(int inAddr)
	{
		int addr = Objet::EEPROM_sauvegarde(inAddr);

		EEPROM.put(addr, &this->intervalle, sizeof(unsigned long));
		addr += sizeof(unsigned long);

		return addr;
	}
};

