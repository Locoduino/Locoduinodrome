#include <Arduino.h>
#include <EEPROM.h>

#include "Objet.h"
#include "Detecteur.h"
#include "CANMessage.h"

extern messageTx();

Detecteur::Detecteur()
{
	this->intervalle = 100;
	this->remanence = 1000;
	this->etatDetecte = HIGH;
  this->etatPrecedent = HIGH;
  this->detectNumber = 0;
	this->precedentTest = 0;
	this->perteDetection = 0;
}

void Detecteur::begin(uint8_t inPin, uint8_t inNumber)
{
	this->pin = inPin;
  this->detectNumber = inNumber;
	pinMode(this->pin, INPUT_PULLUP);
	this->estDetecte = digitalRead(this->pin) == this->etatDetecte;
}

void Detecteur::loop(uint8_t inNewState)
{
	if (millis() - this->precedentTest < this->intervalle)
		return;

	this->precedentTest = millis();
	int etat = digitalRead(this->pin);        // Occupation=LOW, liberation=HIGH

	bool activ = etat == this->etatDetecte;

	if (activ && activ != this->estDetecte)
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

	if (activ != this->etatPrecedent) 
  {
    if (activ)
    {
      statusMessage.setDetection(this->detectNumber, false);
      messageTx();
		  Serial.println(F("Lib"));
    }
	  else
    {
      statusMessage.setDetection(this->detectNumber, true);
      messageTx();
		  Serial.println(F("Occ"));
    } 
  this->etatPrecedent = activ;
  }
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
