#include <Arduino.h>
#include <EEPROM.h>

#include "Satellite.h"
#include "Objet.h"
#include "Led.h"
#include "Messaging.h"

Led::Led()
{
	this->number = 0;
}

void Led::begin(uint8_t inPin, uint8_t inNumber)
{
	this->pin = inPin;
	this->number = inNumber;
	dimmer.begin(this->pin, HIGH);
}

void Led::loop(Satellite *inpSat)
{
	uint8_t inNewState = inpSat->MessageIn.ledState(this->number);
	switch (inNewState) {
		case LED_BLINK:	this->dimmer.startBlink(); break;
		case LED_ON:	this->dimmer.on(); break;
		case LED_OFF:	this->dimmer.off(); break;
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
