#include <Arduino.h>
#include <EEPROM.h>

#include "Objet.h"
#include "Aiguille.h"

Aiguille::Aiguille()
{
	this->posDroit = 800;
	this->posDeviee = 2200;
}

void Aiguille::begin(uint8_t inPin)
{
	this->pin = inPin;

	Servomoteur.setMin(this->posDroit); // il faudra �talonner cette valeur min
	Servomoteur.setMax(this->posDeviee); // il faudra �talonner cette valeur max
	Servomoteur.setSpeed(1.5);
	Servomoteur.setReverted(true);
	Servomoteur.setInitialPosition(0.0);
	Servomoteur.setPin(this->pin);
}

void Aiguille::loop(uint8_t inNewState)
{
	static float servoTarget = 0.0;
	//if (b) 
		servoTarget = 1.0;

	this->Servomoteur.goTo(servoTarget);
}

uint8_t Aiguille::EEPROM_chargement(int inAddr)
{
	int addr = Objet::EEPROM_chargement(inAddr);

	EEPROMGET(addr, this->posDroit, sizeof(unsigned int));
	addr += sizeof(unsigned int);
	EEPROMGET(addr, this->posDeviee, sizeof(unsigned int));
	addr += sizeof(unsigned int);
	EEPROMGET(addr, this->vitesse, sizeof(float));
	addr += sizeof(float);
	return addr;
}

uint8_t Aiguille::EEPROM_sauvegarde(int inAddr)
{
	int addr = Objet::EEPROM_sauvegarde(inAddr);

	EEPROMPUT(addr, this->posDroit, sizeof(unsigned int));
	addr += sizeof(unsigned int);
	EEPROMPUT(addr, this->posDeviee, sizeof(unsigned int));
	addr += sizeof(unsigned int);
	EEPROMPUT(addr, this->vitesse, sizeof(float));
	addr += sizeof(float);

	return addr;
}
