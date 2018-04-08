#include <Arduino.h>
#include <EEPROM.h>

#include "Objet.h"
#include "Detecteur.h"

Detecteur::Detecteur()
{
	this->intervalle = 300;
	this->remanence = 2000;
	this->etatDetecte = HIGH;

	this->precedentTest = 0;
	this->perteDetection = 0;
}

void Detecteur::begin(uint8_t inPin)
{
	this->pin = inPin;
	pinMode(this->pin, INPUT);
	this->estDetecte = digitalRead(this->pin) == this->etatDetecte;
}

void Detecteur::loop(uint8_t inNewState)
{
	if (millis() - this->precedentTest < this->intervalle)
		return;

	this->precedentTest = millis();
	int etat = digitalRead(this->pin);

	bool activ = etat == this->etatDetecte;

	if (!activ && activ != this->estDetecte)
	{
		if (this->perteDetection == 0)
		{
			this->perteDetection = millis();
			return;
		}

		if (millis() - this->perteDetection < this->remanence)
			return;

		this->perteDetection = 0;
	}

	if (activ)
		Serial.println(F("Detecté"));
	else
		Serial.println(F("Fin Detection"));

	this->estDetecte = activ;
}

uint8_t Detecteur::EEPROM_chargement(int inAddr)
{
	int addr = Objet::EEPROM_chargement(inAddr);

	EEPROMGET(addr, this->intervalle, sizeof(unsigned long));
	addr += sizeof(unsigned long);
	EEPROMGET(addr, this->remanence, sizeof(unsigned long));
	addr += sizeof(unsigned long);
	EEPROMGET(addr, this->etatDetecte, sizeof(byte));
	addr += sizeof(byte);

	return addr;
}

uint8_t Detecteur::EEPROM_sauvegarde(int inAddr)
{
	int addr = Objet::EEPROM_sauvegarde(inAddr);

	EEPROMPUT(addr, this->intervalle, sizeof(unsigned long));
	addr += sizeof(unsigned long);
	EEPROMPUT(addr, this->remanence, sizeof(unsigned long));
	addr += sizeof(unsigned long);
	EEPROMPUT(addr, this->etatDetecte, sizeof(byte));
	addr += sizeof(byte);

	return addr;
}
