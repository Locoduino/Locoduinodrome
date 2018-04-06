/* Affectation des pins 
 *  MINI / UNO / NANO
 * pin 0 = RX
 * pin 1 = TX
 * pin 2 = CANInt
 * pin 3 = Led 0
 * pin 4 = Led 1
 * pin 5 = Led 2
 * pin 6 = Led 3
 * pin 7 = Led 4
 * pin 8 = Led 5
 * pin 9 = Led 6
 * pin 10 = CANCS
 * pin 11 = MOSI
 * pin 12 = MISO
 * pin 13 = SCK
 * pin A0 = Led 7
 * pin A1 = Led 8
 * pin A2 = Aiguille 0
 * pin A3 = Balise 0
 * pin A4 = Balise 1
 * pin A5 = Zone 0
 * pin A6 (nu)
 * pin A7 (nu)
 */ 
 
#if !defined ARDUINO_AVR_NANO && !defined ARDUINO_AVR_UNO
#if !defined VISUALSTUDIO
  #error CANNOT COMPILE - ONLY WORKS WITH AN ARDUINO UNO, NANO
#endif
#endif

#include "Arduino.h"

#include "Objet.h"

#include "Aiguille.h"
#include "Balise.h"
#include "Led.h"
#include "Zone.h"

#include "Bus.h"

#define LONGUEUR_ETAT	3

#define NB_LEDS			9
#define NB_AIGUILLES	1
#define NB_ZONES		1
#define NB_BALISES		2

const uint8_t leds_pins[] = { 3, 4, 5, 6, 7, 8, 9, A0, A1 };
const uint8_t aiguilles_pins[] = { A2 };
const uint8_t zones_pins[] = { A5 };
const uint8_t balises_pins[] = { A3, A4 };

class Satellite
{
	Objet*		objets[NB_LEDS + NB_AIGUILLES + NB_ZONES + NB_BALISES];
	uint8_t		nbObjets;

	void AddObjetRange(Objet *inObjList, uint8_t inNb, const uint8_t *inPins)
	{
		for (int i = 0; i < inNb; i++)
		{
			if (inPins[i] != 255)
			{
				inObjList[i].begin(inPins[i]);

				objets[this->nbObjets++] = inObjList + i;
			}
		}
	}

public:

	uint8_t		etat[LONGUEUR_ETAT];

	Led			leds[NB_LEDS];
	Aiguille	aiguilles[NB_AIGUILLES];
	Zone		zones[NB_ZONES];
	Balise		balises[NB_BALISES];

	Satellite()
	{
		this->nbObjets = 0;
		for (int i = 0; i < NB_LEDS + NB_AIGUILLES + NB_ZONES + NB_BALISES; i++)
			objets[i] = NULL;
	}

	void begin()
	{
		AddObjetRange(balises, NB_BALISES, balises_pins);
		AddObjetRange(leds, NB_LEDS, leds_pins);
		AddObjetRange(aiguilles, NB_AIGUILLES, aiguilles_pins);
		AddObjetRange(zones, NB_ZONES, zones_pins);
	}

	void loop()
	{
		for (int i = 0; i < this->nbObjets; i++)
			objets[i]->loop();
	}
};
