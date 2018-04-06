/** Cette classe repr�sente une d�tection d zone. elle peut avoir deux �tats : libre ou occup�e.
Il y a une seule pin utilis�e pour la d�tection.
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

