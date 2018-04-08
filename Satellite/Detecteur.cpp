#include <Arduino.h>
#include <EEPROM.h>

#include "Objet.h"
#include "Detecteur.h"

Detecteur::Detecteur()
{
	this->intervalle = 500;
	this->precedentTest = 0;
}

void Detecteur::begin(uint8_t inPin)
{
	this->pin = inPin;
	pinMode(this->pin, INPUT);
	this->estDetecte = digitalRead(this->pin) == 1;
}

void Detecteur::loop(uint8_t inNewState)
{
	if (millis() - this->precedentTest < this->intervalle)
		return;

	bool activ = digitalRead(this->pin) == 1;
	if (activ != this->estDetecte)
	{
		this->estDetecte = activ;
	}

	this->precedentTest = millis();
}

uint8_t Detecteur::EEPROM_chargement(int inAddr)
{
	int addr = Objet::EEPROM_chargement(inAddr);

	EEPROMGET(addr, this->intervalle, sizeof(unsigned long));
	addr += sizeof(unsigned long);
	return addr;
}

uint8_t Detecteur::EEPROM_sauvegarde(int inAddr)
{
	int addr = Objet::EEPROM_sauvegarde(inAddr);

	EEPROMPUT(addr, this->intervalle, sizeof(unsigned long));
	addr += sizeof(unsigned long);

	return addr;
}
