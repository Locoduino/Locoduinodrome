// Programme pour les cartes satellite du Locoduinodrome
// Thierry 04 Avril 2018
// Dominique 25 Avril 2018
// Copyright Locoduino 2018
// Version 0.1 

#include "Bus.h"
#include "Satellite.h"

Satellite sat;

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

	sat.begin();
}

/////////////////////////////////////////////////////////////////////////////

void loop() 
{
	sat.loop();
}

/////////////////////////////////////////////////////////////////////////////

