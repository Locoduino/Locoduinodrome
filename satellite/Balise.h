/** Cette classe repr�sente une d�tection d zone. elle peut avoir deux �tats : libre ou occup�e.
Il y a une seule pin utilis�e pour la d�tection.
*/
class Balise : public Objet
{
	bool activee;
public:
	Balise()
	{
	}

	void begin(uint8_t inPin)
	{
		this->pin = inPin;
		this->activee = digitalRead(this->pin) == 1;
		pinMode(this->pin, INPUT);
	}

	void loop()
	{
		bool activ = digitalRead(this->pin) == 1;
		if (activ != this->activee)
		{
			this->activee = activ;
		}
	}

	bool EstActivee() {	return this->activee; }
};

