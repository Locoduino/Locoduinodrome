
/** Cette classe repr�sent une aiguille. Une aiguille poss�de deux positions : droite et d�vi�e. Elle est pilot�e par une broche unique.
*/
class Aiguille : public Objet
{
public:
	Aiguille()
	{
	}

	void begin(uint8_t inPin)
	{
		this->pin = inPin;
		pinMode(this->pin, OUTPUT);
	}

	void loop()
	{}

	//void init(byte no) { /*direct=???*/ message(CODE_AIGUILLE_INIT,no,direct?1:0); }

	//void manoeuvrer() { direct=!direct; moteur(direct); } // inversion de l'aiguille 
	void manoeuvrer(boolean b)
	{
		//  moteur(b); 
		//  direct=b; 
	}     // positionnement de l'aiguille (DIRECTE ou DEVIEE)
};
