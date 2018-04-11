#include <Arduino.h>
#include <EEPROM.h>

#include "Objet.h"
#include "Led.h"
#include "Messaging.h"

Led::Led()
{
}

void Led::begin(uint8_t inPin)
{
	this->pin = inPin;
	dimmer.begin(this->pin, HIGH);
}

void Led::loop(uint8_t inNewState)
{
	if (inNewState == LED_BLINK && this->dimmer.isOff())
	{
		this->dimmer.startBlink();
		this->dimmer.on();
	}

	if (inNewState == LED_ON && this->dimmer.isOff())
	{
		this->dimmer.stopBlink();
		this->dimmer.on();
	}

	if (inNewState == LED_OFF && this->dimmer.isOn())
	{
		this->dimmer.off();
	}
}
	
uint8_t Led::EEPROM_chargement(int inAddr)
{
	int addr = Objet::EEPROM_chargement(inAddr);
	uint8_t valeur8;
	uint16_t valeur16;

	EEPROMGET(addr, valeur8, sizeof(uint8_t));
	this->dimmer.setMax(valeur8);
	addr += sizeof(uint8_t);
	EEPROMGET(addr, valeur16, sizeof(uint16_t));
	this->dimmer.setBrighteningTime(valeur16);
	addr += sizeof(uint16_t);
	EEPROMGET(addr, valeur16, sizeof(uint16_t));
	this->dimmer.setFadingTime(valeur16);
	addr += sizeof(uint16_t);
	return addr;
}

uint8_t Led::EEPROM_sauvegarde(int inAddr)
{
	int addr = Objet::EEPROM_sauvegarde(inAddr);
	uint8_t valeur8;
	uint16_t valeur16;

	valeur8 = this->dimmer.maximum();
	EEPROMPUT(addr, valeur8, sizeof(uint8_t));
	addr += sizeof(uint8_t);
	valeur16 = this->dimmer.brighteningTime();
	EEPROMPUT(addr, valeur16, sizeof(uint16_t));
	addr += sizeof(uint16_t);
	valeur16 = this->dimmer.fadingTime();
	EEPROMPUT(addr, valeur16, sizeof(uint16_t));
	addr += sizeof(uint16_t);

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
