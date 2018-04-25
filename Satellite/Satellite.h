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
 
#if !defined (__AVR_ATmega328P__)
#if !defined VISUALSTUDIO
  #error CANNOT COMPILE - ONLY WORKS WITH AN ARDUINO UNO, NANO OR MINI
#endif
#endif

#include <Arduino.h>
#include <EEPROM.h>

#include "Objet.h"

#include "Aiguille.h"
#include "Detecteur.h"
#include "Led.h"

#include "CANMessage.h"

#define LONGUEUR_ETAT	3

#define NB_LEDS			9
#define NB_AIGUILLES	1
#define NB_DETECTEURS	3

const uint8_t leds_pins[] = { 3, 4, 5, 6, 7, 8, 9, A0, A1 };
const uint8_t aiguilles_pins[] = { A2 };
const uint8_t detecteurs_pins[] = { A3, A4, A5 };

const char *EEPROM_ID = { "LCDO" };

/*
 * Temporairement on peut fixer l'Id des satellites à la main !
 */
 
//#define ID_SAT        0 // carre 4
//#define ID_SAT        1 // carre 5
#define ID_SAT        2 // carre 7 et aiguille 1
//#define ID_SAT        3 // semaphore 1 et semaphore RR 8
//#define ID_SAT        4 // semaphore 0 et semaphore RR 9
//#define ID_SAT        5 // carre RR 3
//#define ID_SAT        6 // carre 6 et aiguille 0
//#define ID_SAT        7 // carre RR 2

class Satellite
{
	// Configuration
	uint8_t		id;	// Identifiant du satellite

	// Gestion locale	
	Objet*		objets[NB_LEDS + NB_AIGUILLES + NB_DETECTEURS];	// Liste de tous les objets gérés
	uint8_t		nbObjets;
	byte		objetCourantLoop;

	void AddObjet(Objet *inpObjet, uint8_t inPin, uint8_t inNumber)
	{
		if (inpObjet != NULL && inPin != 255)
		{
			inpObjet->begin(inPin, inNumber);
			this->objets[this->nbObjets] = inpObjet;
		}
		this->nbObjets++;
	}

	void EEPROM_sauvegarde()
	{
		int addr = 0;

		////////////////////////////////////// Partie entete

		/* Un descripteur du contenu est sauve en premier.
		Si ce descripteur n'est pas conforme a la lecture, alors l'EEPROM sera consideree vierge.
		Ca signifie aussi que changer la forme (nombre d'objets, taille d'un objet) annule toute sauvegarde !
		*/

		eeprom_write_block(EEPROM_ID, (void *)addr, 4);
		addr += 4;

		EEPROM.write(addr++, this->nbObjets);

		EEPROM.write(addr++, leds[0].GetEEPROMSize());
		EEPROM.write(addr++, aiguilles[0].GetEEPROMSize());
		EEPROM.write(addr++, detecteurs[0].GetEEPROMSize());

		////////////////////////////////////// Partie satellite

		EEPROM.write(addr++, this->id);

		////////////////////////////////////// Partie objets

		for (int i = 0; i < this->nbObjets; i++)
			if (objets[i] != NULL)
				addr = objets[i]->EEPROM_sauvegarde(addr);
	}

	/* Chargement de la configuration depuis l'EEPROM. La fonction retourne true si tout est bien charge. */
	bool EEPROM_chargement()
	{
		int addr = 0;
		char buf[5];

		////////////////////////////////////// Partie entete

		// ID EEPROM
		eeprom_read_block(buf, (const void *)addr, 4);
		addr += 4;
		buf[4] = 0;

		for (int i = 0; i < 4; i++)
			if (buf[i] != EEPROM_ID[i])
				return false;

		// Nombre d'objets
		uint8_t nb = EEPROM.read(addr++);
		if (nb != this->nbObjets)
			return false;

		// taille des objets
		eeprom_read_block(buf, (const void *)addr, 3);
		addr += 3;

		if (buf[0] != leds[0].GetEEPROMSize())	return false;
		if (buf[1] != aiguilles[0].GetEEPROMSize())	return false;
		if (buf[2] != detecteurs[0].GetEEPROMSize())	return false;

		////////////////////////////////////// Partie satellite

		this->id = EEPROM.read(addr++);

		////////////////////////////////////// Partie objets

		for (int i = 0; i < this->nbObjets; i++)
			if (objets[i] != NULL)
				addr = objets[i]->EEPROM_chargement(addr);

		return true;
	}

public:
	Led			leds[NB_LEDS];
	Aiguille	aiguilles[NB_AIGUILLES];
	Detecteur	detecteurs[NB_DETECTEURS];

	CommandCANMessage MessageIn;


 	Satellite()
	{
		this->nbObjets = 0;
		for (int i = 0; i < NB_LEDS + NB_AIGUILLES + NB_DETECTEURS; i++)
			objets[i] = NULL;
		this->objetCourantLoop = 0;
	}

	void begin()
	{
		this->id = ID_SAT;
		busInit(this->id);

		this->nbObjets = 0;
		for (int i = 0; i < NB_LEDS; i++)
		{
		  this->AddObjet(&this->leds[i], leds_pins[i], i);
      digitalWrite(leds_pins[i], HIGH);
      delay(250);
      digitalWrite(leds_pins[i], LOW);
		}
		for (int i = 0; i < NB_AIGUILLES; i++)	
		{
		  this->AddObjet(&this->aiguilles[i], aiguilles_pins[i], i);
		}
		for (int i = 0; i < NB_DETECTEURS; i++)	
		{
		  this->AddObjet(&this->detecteurs[i], detecteurs_pins[i], i);
		}

		/*if (EEPROM_chargement() == false)
			EEPROM_sauvegarde();*/
	}

	void loop()
	{
		if (messageRx())
    {
			this->MessageIn.receive(RxBuf); // synchronisation sur les réceptions periodiques
      messageTx();                    // des emissions periodiquesconcernant les capteurs
    }
		
		// traite les loop prioritaires
		Aiguille::loopPrioritaire();
		Detecteur::loopPrioritaire();
		Led::loopPrioritaire();

		if (objets[this->objetCourantLoop] != NULL)
		{
			// fait juste le loop de l'objet courant
			uint8_t etat = 0;

			if (this->objetCourantLoop < NB_LEDS)
				etat = this->MessageIn.ledState(this->objetCourantLoop);
			else
				if (this->objetCourantLoop < NB_LEDS + NB_AIGUILLES)
					etat = this->MessageIn.pointState();	// i - NB_LEDS le jour où il y aura plus d'une aiguille.
			objets[this->objetCourantLoop]->loop(etat);
		}

		// puis passe à l'objet suivant pour le prochain loop...
		this->objetCourantLoop++;

		// Si on est à la fin de la liste, on recommence !
		if (this->objetCourantLoop >= this->nbObjets)
			this->objetCourantLoop = 0;
	}
};
