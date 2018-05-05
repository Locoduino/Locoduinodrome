// Programme pour les cartes satellite du Locoduinodrome
// Thierry 04 Avril 2018
// Dominique 25 Avril 2018
// Copyright Locoduino 2018
// Version 0.1 

#include "Satellite.h"

Satellite sat;

/*
* Temporairement on peut fixer l'Id des satellites à la main !
*/

//#define ID_SAT        0 // carre 4
//#define ID_SAT        1 // carre 5
//#define ID_SAT        2 // carre 7 et aiguille 1
#define ID_SAT        3 // semaphore 1 et semaphore RR 8
//#define ID_SAT        4 // semaphore 0 et semaphore RR 9
//#define ID_SAT        5 // carre RR 3
//#define ID_SAT        6 // carre 6 et aiguille 0
//#define ID_SAT        7 // carre RR 2

/*#include "CommandInterpreter.h"

CommandInterpreter CLI(Serial);
Command satid("id", satelliteId, "Fixe l'identifiant du satellite.");
Command configLed("led", ledConfig, "Configure une led");
Command configAiguille("aig", aiguilleConfig, "Configure une aiguille");
Command configBalise("bal", baliseConfig, "Configure une balise");
Command configZone("zon", zoneConfig, "Configure une zone");

void satelliteId(CommandInterpreter &cmdInt, byte argCount)
{
	HardwareSerial &serial = cmdInt.Serial();
	unsigned int id;
	bool printHelp = true;

	if (argCount > 0) 
	{
		if (cmdInt.readUnsignedInt(id)) {
			sat.id = id;
			serial.print(F("Satellite nouvel id : "));
			serial.println(id);
			printHelp = false;
		}
		else
		{
			serial.println(F("Nombre d'arguments incorrect"));
		}
	}
	if (printHelp)
		serial.println(F("Usage : id <valeur>"));
}*/
	
void setup()
{ 
	Serial.begin(115200);

	sat.begin(ID_SAT);
}

/////////////////////////////////////////////////////////////////////////////

void loop() 
{
	sat.loop();
}

/////////////////////////////////////////////////////////////////////////////

