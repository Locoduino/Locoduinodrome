#include <Arduino.h>
#include <EEPROM.h>

#include "Objet.h"
#include "Aiguille.h"

Aiguille::Aiguille()
{
	this->estDroit = true;
}

void Aiguille::begin(uint8_t inPin, uint8_t inNumber)
{
	this->pin = inPin;	// inutile puisque stockée dans ServoMoteur, mais puisque Objet le permet...

	servoMoteur.setReverted(true);
	servoMoteur.setInitialPosition(this->estDroit ? 0.0f : 1.0f);
	servoMoteur.setPin(this->pin);
  servoMoteur.setMin(1000);
  servoMoteur.setMax(1800);
  servoMoteur.setSpeed(1.5);
}

void Aiguille::loop(uint8_t inEstDroit)
{
	if ((bool)inEstDroit == this->estDroit)
		return;

	this->estDroit = (bool)inEstDroit;
	this->servoMoteur.goTo(inEstDroit ? 0.0f : 1.0f);
}

uint8_t Aiguille::EEPROM_chargement(int inAddr)
{
	int addr = Objet::EEPROM_chargement(inAddr);
	unsigned int valeurUInt;
	float valeurFloat;

	EEPROMGET(addr, valeurUInt, sizeof(unsigned int));
	this->servoMoteur.setMin(valeurUInt);
	addr += sizeof(unsigned int);
	EEPROMGET(addr, valeurUInt, sizeof(unsigned int));
	this->servoMoteur.setMax(valeurUInt);
	addr += sizeof(unsigned int);
	EEPROMGET(addr, valeurFloat, sizeof(float));
	this->servoMoteur.setSpeed(valeurFloat);
	addr += sizeof(float);

	return addr;
}

uint8_t Aiguille::EEPROM_sauvegarde(int inAddr)
{
	int addr = Objet::EEPROM_sauvegarde(inAddr);
	unsigned int valeurUInt;
	float valeurFloat;

	valeurUInt = this->servoMoteur.minimumPulse();
	EEPROMPUT(addr, valeurUInt, sizeof(unsigned int));
	addr += sizeof(unsigned int);
	valeurUInt = this->servoMoteur.maximumPulse();
	EEPROMPUT(addr, valeurUInt, sizeof(unsigned int));
	addr += sizeof(unsigned int);
	valeurFloat = this->servoMoteur.minToMaxSpeed() * 10000.f;
	EEPROMPUT(addr, valeurFloat, sizeof(float));
	addr += sizeof(float);

	return addr;
}
