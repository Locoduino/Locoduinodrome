#include <Arduino.h>
#include <EEPROM.h>

#include "Objet.h"
#include "Zone.h"

Zone::Zone()
{
	this->intervalle = 500;
	this->precedentTest = 0;
}

void Zone::begin(uint8_t inPin)
{
	this->pin = inPin;
	pinMode(this->pin, INPUT);
	this->occupee = digitalRead(this->pin) == 1;
}

void Zone::loop()
{
	if (millis() - this->precedentTest < this->intervalle)
		return;

	bool occup = digitalRead(this->pin) == 1;
	if (occup != this->occupee)
	{
		this->occupee = occup;
	}

	this->precedentTest = millis();
}

uint8_t Zone::EEPROM_chargement(int inAddr)
{
	int addr = Objet::EEPROM_chargement(inAddr);

	EEPROMGET(addr, this->intervalle, sizeof(unsigned long));
	addr += sizeof(unsigned long);
	return addr;
}

uint8_t Zone::EEPROM_sauvegarde(int inAddr)
{
	int addr = Objet::EEPROM_sauvegarde(inAddr);

	EEPROMPUT(addr, this->intervalle, sizeof(unsigned long));
	addr += sizeof(unsigned long);

	return addr;
}

