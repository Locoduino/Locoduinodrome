/** Cette classe représente une détection d zone. elle peut avoir deux états : libre ou occupée.
Il y a une seule pin utilisée pour la détection.
*/
class Zone : public Objet
{
	bool occupee;
public:
	Zone()
	{
	}

	void begin(uint8_t inPin)
	{
		this->pin = inPin;
		this->occupee = digitalRead(this->pin) == 1;
		pinMode(this->pin, INPUT);
	}

	void loop()
	{
		bool occup = digitalRead(this->pin) == 1;
		if (occup != this->occupee)
		{
			this->occupee = occup;
		}
	}

	bool EstOccupee() { return this->occupee; }
};

