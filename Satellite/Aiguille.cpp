#include <Arduino.h>
#include <EEPROM.h>

#include "Objet.h"
#include "Aiguille.h"

Aiguille::Aiguille()
{
	this->posDroit = 800;
	this->posDeviee = 2200;
	this->vitesse = 1.5;
	this->estDroit = true;
}

void Aiguille::begin(uint8_t inPin)
{
	this->pin = inPin;	// inutile puisque stockée dans ServoMoteur, mais puisque Objet le permet...

	Servomoteur.setMin(this->posDroit); // il faudra �talonner cette valeur min
	Servomoteur.setMax(this->posDeviee); // il faudra �talonner cette valeur max
	Servomoteur.setSpeed(this->vitesse);
	Servomoteur.setReverted(true);
	Servomoteur.setInitialPosition(this->estDroit ? 0.0 : 1.0);
	Servomoteur.setPin(this->pin);
}

void Aiguille::loop(uint8_t inEstDroit)
{
	if ((bool)inEstDroit == this->estDroit)
		return;

	this->estDroit = (bool)inEstDroit;
	this->Servomoteur.goTo(inEstDroit ? 0.0 : 1.0);
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

	Servomoteur.setMin(this->posDroit); // il faudra �talonner cette valeur min
	Servomoteur.setMax(this->posDeviee); // il faudra �talonner cette valeur max
	Servomoteur.setSpeed(this->vitesse);
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
