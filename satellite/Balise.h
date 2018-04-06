/** Cette classe représente une détection d zone. elle peut avoir deux états : libre ou occupée.
Il y a une seule pin utilisée pour la détection.
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

