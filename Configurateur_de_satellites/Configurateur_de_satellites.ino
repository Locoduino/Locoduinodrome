 /*
  * SOFTWARE : Testeur et Configurateur de Satellites
  * AUTEUR : Dominique Bultez 
  * Copyright (c) LOCODUINO 2019
  *
  * Permission is hereby granted, free of charge, to any person obtaining a copy
  * of this software and associated documentation files (the "Software"), to deal
  * in the Software without restriction, including without limitation the rights
  * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  * copies of the Software, and to permit persons to whom the Software is
  * furnished to do so, subject to the following conditions:
  *
  * The above copyright notice and this permission notice shall be included in
  * all copies or substantial portions of the Software.
  *
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  * THE SOFTWARE.
  *
  * CARTE : Arduino UNO avec INTERFACE CAN
  * INTERFACE UTILISATEUR : LCD 20x4, Encodeur Quadratique avec bouton et Inverseur unipolaire
  * 
  * --------------------------
  * Version du 12 Mai 2019
  * --------------------------
  * Pour test des signaux, aiguilles et capteurs des 8 satellites du LOCODUINODROME 
  * Un message requète est envoyé toutes les 100 ms
  * 
  *
  * PINS utilisées : 
  * 0,1 = Tx, Rx
  * 2 = Int CAN
  * 3,5 = encoder
  * 4 = bouton encoder
  * 7 = inverseur config/test
  * 9,11,12,13, RST = SPI CAN
  *
  */
  
#define VERSION F("Satellites Test & Config")
#define SHORTVERSION F("TC_Satellites 0.5")
#include <SPI.h>
#include <mcp_can.h>
#include <mcp_can_dfs.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Encoder.h>
#include <Bounce2.h>                // boutons poussoir encodeur et inverseur test/config

///////////////// variables pour l'interface CAN //////////////

MCP_CAN canController(9);         // Set CS to pin 9 for CAN
volatile bool Flag_Recv = false;  // pour IRQ CAN
volatile boolean CANOK = false;
unsigned int messagesRecus = 0;   //nb de message reçus pour affichage

unsigned long IdR;
unsigned char lenR = 0;
unsigned char bufR[8];            // buffer reception

unsigned long IdS;
unsigned char lenS = 0;
unsigned char bufS[8];            // buffer emission

//////////////// encodeur et boutons /////////////////////

Encoder Encodeur(3, 5);               // encodeur
long oldPosition  = -999;

const int SWencPin = 4;               // bouton encodeur
Bounce SWenc = Bounce();
bool gSWencState = false;

const int SWmodePin = 7;              // inverseur test/config
Bounce SWmode = Bounce();
bool gSWmodeState = false;

/////////////// Gestion Encodeur ///////////////
/*  le principe est de savoir s'il y a rotation ou non, et dans quel sens
 *  peu importe le nombre de crans 
 *  On obtient +1 = incrément, 0 = pas de rotation, -1 = décrement
 */

int lireEncodeur()
{
  int retour = 0;
  long newPosition = Encodeur.read()/4;
  if (newPosition != oldPosition) {
    if (newPosition > oldPosition) {
      retour = 1;
    } else {
      retour = -1;
    }
    oldPosition = newPosition;
  }
  return (retour);  
}

/////////////////// ecran LCD 4 x 20 //////////////////////

LiquidCrystal_I2C lcd(0x27,20,4);   // LCD address = 0x27 pour 4 lignes de 20 cars
bool _backlit = true;
int ShowId = 0;

////////////////// L'Interface SAM /////////////////////

#include "SatelliteWrapper.h" // gere les aiguilles (point) et les signaux
#include "Feux.h"             // défini les types de feux

///////////////// les tests ont besoin de la variable lcd et de l'interface SAM //////

#include "LCD.h"                    // gestion de l'afficheur LCD
#include "A_Tests.h"                  // programmes de test
#include "B_Configuration.h"          // programmes de configuration

/*
 *  Les aiguillages
 */
PointWrapper pointWrapper0(0, 2); /* aiguillage 0 du gestionnaire sur satellite 2 */
PointWrapper pointWrapper1(1, 6); /* aiguillage 1 du gestionnaire sur satellite 6 */

/*
 * Les signaux
 */
SemaphoreSignalWrapper                  S1wrapper(0, 4, SIGNAL_0); /* signal 0 du gestionnaire sur satellite 4, slot 0 */
SemaphoreSignalWrapper                  S2wrapper(1, 3, SIGNAL_0); /* signal 1 du gestionnaire sur satellite 3, slot 0 */
CarreSignalWrapper                      C3wrapper(4, 0, SIGNAL_0); /* signal 4 du gestionnaire sur satellite 0, slot 0 */
CarreSignalWrapper                      C4wrapper(5, 1, SIGNAL_0); /* signal 5 du gestionnaire sur satellite 1, slot 0 */
CarreSignalWrapper                      C5wrapper(6, 6, SIGNAL_0); /* signal 6 du gestionnaire sur satellite 6, slot 0 */
CarreSignalWrapper                      C6wrapper(7, 2, SIGNAL_0); /* signal 7 du gestionnaire sur satellite 2, slot 0 */
CarreRappelRalentissementSignalWrapper  C1wrapper(2, 7, SIGNAL_0); /* signal 2 du gestionnaire sur satellite 7, slot 0 */
CarreRappelRalentissementSignalWrapper  C2wrapper(3, 5, SIGNAL_0); /* signal 3 du gestionnaire sur satellite 5, slot 0 */
SemaphoreRalentissementSignalWrapper    S3wrapper(8, 3, SIGNAL_1); /* signal 8 du gestionnaire sur satellite 3, slot 1 */
SemaphoreRalentissementSignalWrapper    S4wrapper(9, 4, SIGNAL_1); /* signal 9 du gestionnaire sur satellite 4, slot 1 */

/////////////////////// ISR du CAN ///////////////////////////
// le flag IRQ monte quand au moins un message est recu
// le flag IRQ ne retombe QUE si tous les messages sont lus

void MCP2515_ISR()  { Flag_Recv = true; }      // surveillance de l'IRQ

////////////////// Utilitaires d'envoi de messages CAN /////////////////////

void sendConfig(byte cId, byte cLg)
{
  unsigned long longId = 0x1FFFFF20 + cId;
  canController.sendMsgBuf(longId, 1, cLg, bufS);
}

void sendTest(byte tId)
{
  uint32_t frameId = 0x20 + tId;
  canController.sendMsgBuf(frameId, 0, 3, bufS);
}

void sendAiguille(int a, bool i)
{
  PointWrapper::setPointPosition(a,i);
}

void sendSignal(int sig, unsigned int f)
{
  SignalWrapper::setSignalState(sig, f);
}

bool _debug = true;          // pour valider les "Serial.print" vers la Console


/////////////////////////////// SETUP /////////////////////
void setup()
{
  Serial.begin(115200);
  Serial.flush();
  Serial.println(SHORTVERSION);
  lcd.init();                         // initialise le lcd 
  lcd.backlight();
  lcd.setCursor(0, 0);                // LCD 1e ligne
  lcd.print(SHORTVERSION);
  
  oldPosition = Encodeur.read()/4;    // initialise encodeur et boutons
  pinMode(SWencPin,INPUT_PULLUP);
  SWenc.attach(SWencPin);
  SWenc.interval(100);
  pinMode(SWmodePin,INPUT_PULLUP);
  SWmode.attach(SWmodePin);
  SWmode.interval(100);

  const int baudrate = CAN_500KBPS;   // initialise le Can à 500 Kb/s  
  canController.start();  
  int repeat = 10;                    // open the CAN
  while (repeat > 0) {
    if (CAN_OK == canController.begin(baudrate)) {CANOK=true;break;}
    else {repeat--;}
    delay(200);
  }
  lcd.setCursor(0, 1);                // LCD 2e ligne
  if (CANOK) {lcd.print(F("Can OK"));}else{lcd.print(F("Can NOK!"));}
  attachInterrupt(0, MCP2515_ISR, FALLING); // start interrupt from pin 2

  /*
   * Pas de filtres Can
   */

  PointWrapper::begin();              // initialise SAM
  SignalWrapper::begin();

  Serial.println("Etat Initial");
  printOutBuffers();                  // etat initial des satellites

  sVit.f = 2.0;                       // vitesse des servos par défaut
      
  delay(2000);                        // le temps de lire le LCD 
  
  gModeConfig = digitalRead(SWmodePin);
  if (gModeConfig) {                  // clé T/C mode config ou test
    initConfig();                     // voir Configuration.h
  } else {
    initTest();                       // voir Tests.h
  }
}  // fin de Setup

////////////////////// AUTOMATE d'ETAT ////////////
/*
 * Les tests sont fait par doTest() et la configuration par doConfig() 
 * selon la position de l'inverseur TEST / CONFIG (globale gModeConfig)
 * Ils s'articule autour de variables d'état propres à chaque programme. :
 * 
 * Pour les tests :
 * menuTest est le numéro de menu principal affiché sur la 1ère ligne du lcd
 * menu va de 0 à maxMenu (3) : Aiguille 0, Aiguille 1 et tous les feux
 * sousmenuTest est le numéro d'action (pour les aiguilles, menuTest = 0 ou 1) 
 * ou un choix d'action (choix du type de feu, menuTest = 2)
 * Le bouton de l'encodeur permet de circuler dans tous les cas de menuTest + sousmenuTest (14 cas)
 * L'appui sur le bouton de l'encodeur quand menuTest = 2 permet de choisir le type de feu (16 cas)
 * Chaque rotation de l'encodeur passe au test suivant ou précédent et execute le test 
 * Dans le cas des feux, un nouvel appui sur le bouton remonte au choix supérieur.
 * 
 * Pour les configurations
 * initConfig() gère l'encodeur pour choisir le satellite à configurer
 * l'appui sur le bouton permet de sortir d'initConfig() et d'executer doConfig()
 * l'encodeur permet de choisir parmi 9 parametres de configuration
 * le choix du 10eme cas remonte au choix du satellite
 * le choix d'un parametre, par appui sur le bouton, affiche la valeur courante 
 * le boolean gModeSWConfig conditionne la configuration provisore ou définitive
 * gnumConfig est le numéro de valeur à configurer (9 par satellite)
 * la rotation du bouton change la valeur d'une unité en + ou en - 
 * et envoie un message de configuration provisoire au satellite
 * L'appui sur le bouton envoie un message de configuration définitif et sort du reglage.
 * L'inverseur TEST / CONFIG permet de changer de mode à tout moment
 * L'afficheur LCD indique tous les choix et les valeurs.
 */
/////////////////////// LOOP ///////////////////////
void loop()
{    
  if (Flag_Recv)  {               // reception et traitement des messages Can
    while (CAN_MSGAVAIL == canController.checkReceive())  {
      canController.readMsgBuf(&lenR, bufR);    // read data,  lenR: data length, bufR: data buf
      IdR = canController.getCanId();
      if (_debug)  {
      Serial.print(" id:0x");Serial.print(IdR, HEX);Serial.print(" Data");
      for (int k = 0; k < lenR; k++)  {
        Serial.print(' ');Serial.print("0x");Serial.print(bufR[k], HEX);
        }
      Serial.println();
      }
      messagesRecus++;
      if (!gModeConfig) {         // en mode test on affiche l'état des capteurs lignes 3 et 4
        displayCapteurs(IdR, bufR);    // gestion des messages CAN reçus : affichage LCD (dans LCD.h)
      }
    }
    Flag_Recv = false;            // Flag MCP2515 pret pour un nouvel IRQ    
  }

  if ( SWmode.update() ) {        // test de la clé T/C
    if ( SWmode.fell() ) {
      gModeConfig = false;
      initTest();    
    } else {
      if ( SWmode.rose() ) {
        gModeConfig = true;
        initConfig();
      }
    }
  }

  if (gModeConfig) {              // mode config ou test ?
    doConfig();
  } else {
    doTest();
  }
    
  if (gModeConfig == false)       // uniquement en mode test
  {
    sendSatelliteMessage(); // emission periodique définie dans SatelliteConfig.h OUT_MESSAGE_PERIOD = 100;  
  }
  
  if (_debug) {
    if (gModeConfig == false) {lcd.setCursor(16, 0);} else {lcd.setCursor(16, 3);}
      if (messagesRecus > 9999) messagesRecus = messagesRecus - 10000;
      if (messagesRecus < 1000) lcd.print(' ');
      if (messagesRecus < 100) lcd.print(' ');
      if (messagesRecus < 10) lcd.print(' ');
      lcd.print(messagesRecus); 
  }

}  // fin de Loop




/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
