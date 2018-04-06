
/* Classe pour une Led, avec sa configuration, son état et ses fonctions de manipulation.
*/
class Led : public Objet
{
public:

	Led()
	{
	}

	void begin(uint8_t inPin)
	{
		this->pin = inPin;
		pinMode(this->pin, OUTPUT);
	}

	void loop()
	{
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
