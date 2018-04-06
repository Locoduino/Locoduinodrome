/** Cette classe est la classe de base de tous les objet utilisant une seule broche.
*/
class Objet
{
protected:
	uint8_t pin;

public:
	Objet()
	{
		this->pin = 255;
	}

	bool IsValid() { return this->pin != 255; }

	virtual void begin(uint8_t inPin) = 0;
	virtual void loop() = 0;
};

