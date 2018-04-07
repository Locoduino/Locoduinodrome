
/** Cette classe représent une aiguille. Une aiguille possède deux positions : droite et déviée. Elle est pilotée par une broche unique.
*/
class Aiguille : public Objet
{
	unsigned int posDroit;
	unsigned int posDeviee;

public:
	Aiguille()
	{
		this->posDroit = 700;
		this->posDeviee = 2300;
	}

	void begin(uint8_t inPin)
	{
		this->pin = inPin;
		pinMode(this->pin, OUTPUT);
	}

	void loop()
	{}

	uint8_t GetEEPROMSize()	{ return Objet::GetEEPROMSize() + (2 * sizeof(unsigned int)); }

	uint8_t EEPROM_chargement(int inAddr)
	{
		int addr = Objet::EEPROM_chargement(inAddr);

		EEPROM.get(addr, &this->posDroit, sizeof(unsigned int));
		addr += sizeof(unsigned int);
		EEPROM.get(addr, &this->posDeviee, sizeof(unsigned int));
		addr += sizeof(unsigned int);
		return addr;
	}

	uint8_t EEPROM_sauvegarde(int inAddr)
	{
		int addr = Objet::EEPROM_sauvegarde(inAddr);

		EEPROM.put(addr, &this->posDroit, sizeof(unsigned int));
		addr += sizeof(unsigned int);
		EEPROM.put(addr, &this->posDeviee, sizeof(unsigned int));
		addr += sizeof(unsigned int);

		return addr;
	}

	//void init(byte no) { /*direct=???*/ message(CODE_AIGUILLE_INIT,no,direct?1:0); }

	//void manoeuvrer() { direct=!direct; moteur(direct); } // inversion de l'aiguille 
	void manoeuvrer(boolean b)
	{
		//  moteur(b); 
		//  direct=b; 
	}     // positionnement de l'aiguille (DIRECTE ou DEVIEE)
};
