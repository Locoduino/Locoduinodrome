#include <Arduino.h>
#include <EEPROM.h>

#include "Objet.h"
#include "Led.h"

Led::Led()
{
	this->valeurPWM = 255;
	this->clignotement = 500;
	this->dimming = 250;
}

void Led::begin(uint8_t inPin)
{
	this->pin = inPin;
	dimmer.begin(this->pin, HIGH);
}

void Led::loop(uint8_t inNewState)
{
	LightDimmer::update();

/*	if (_state == CLIGNOTANTE && this->dimmer.off())
	{
		this->dimmer.startBlink();
		this->dimmer.on();
	}

	if (_state == ALLUMEE && this->dimmer.isOff())
	{
		this->dimmer.stopBlink();
		this->dimmer.on();
	}

	if (_state != ALLUMEE && this->dimmer.isOn())
	{
		this->dimmer.off();
	}*/
}
	
uint8_t Led::EEPROM_chargement(int inAddr)
{
	int addr = Objet::EEPROM_chargement(inAddr);

	EEPROMGET(addr, this->valeurPWM, sizeof(unsigned int));
	addr += sizeof(unsigned int);
	EEPROMGET(addr, this->clignotement, sizeof(unsigned long));
	addr += sizeof(unsigned long);
	EEPROMGET(addr, this->dimming, sizeof(unsigned long));
	addr += sizeof(unsigned long);
	return addr;
}

uint8_t Led::EEPROM_sauvegarde(int inAddr)
{
	int addr = Objet::EEPROM_sauvegarde(inAddr);

	EEPROMPUT(addr, this->valeurPWM, sizeof(unsigned int));
	addr += sizeof(unsigned int);
	EEPROMPUT(addr, this->clignotement, sizeof(unsigned long));
	addr += sizeof(unsigned long);
	EEPROMPUT(addr, this->dimming, sizeof(unsigned long));
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
