
/*
 * Locoduinodrome SAM
 */
#include <mcp_can.h>
#include <mcp_can_dfs.h>
 
/*
 * Interface CAN
 */
const uint8_t spiCS = 9;  // pour un Uno
//const uint8_t spiCS = 53; // sur un Mega

MCP_CAN canController(spiCS);


// CAN interrupt routine
volatile bool FlagReceive = false;      // can interrupt flag
void MCP2515_ISR() {FlagReceive = true;}

/*
 * Les satellites
 */
#include "SatelliteWrapper.h"
#include "Feux.h"

/*
 *  Les aiguillages
 */
PointWrapper pointWrapper0(0, 2); /* aiguillage 0 de SAM sur satellite 2 */
PointWrapper pointWrapper1(1, 6); /* aiguillage 1 de SAM sur satellite 6 */

/*
 * Les signaux
 */
SemaphoreSignalWrapper                  S1wrapper(0, 4, SIGNAL_0); /* signal 0 de SAM sur satellite 4, slot 0 */
SemaphoreSignalWrapper                  S2wrapper(1, 3, SIGNAL_0); /* signal 1 de SAM sur satellite 3, slot 0 */
CarreSignalWrapper                      C3wrapper(4, 0, SIGNAL_0); /* signal 4 de SAM sur satellite 0, slot 0 */
CarreSignalWrapper                      C4wrapper(5, 1, SIGNAL_0); /* signal 5 de SAM sur satellite 1, slot 0 */
CarreSignalWrapper                      C5wrapper(6, 6, SIGNAL_0); /* signal 6 de SAM sur satellite 6, slot 0 */
CarreSignalWrapper                      C6wrapper(7, 2, SIGNAL_0); /* signal 7 de SAM sur satellite 2, slot 0 */
CarreRappelRalentissementSignalWrapper  C1wrapper(2, 7, SIGNAL_0); /* signal 2 de SAM sur satellite 7, slot 0 */
CarreRappelRalentissementSignalWrapper  C2wrapper(3, 5, SIGNAL_0); /* signal 3 de SAM sur satellite 5, slot 0 */
SemaphoreRalentissementSignalWrapper    S3wrapper(8, 3, SIGNAL_1); /* signal 8 de SAM sur satellite 3, slot 1 */
SemaphoreRalentissementSignalWrapper    S4wrapper(9, 4, SIGNAL_1); /* signal 9 de SAM sur satellite 4, slot 1 */

/*
 * Pour l'animation des signaux dans Loop
 */

unsigned long tempo;
int sig = 0;
unsigned int feu = 1;

///////////////////////////////////////////

void setup()
{
  Serial.begin(115200);

 // init CAN

  canController.start();
  
 /*
  * set mask & filters
  */
  
  canController.init_Mask(0, 0, 0x3F0);               // there are 2 mask in mcp2515, you need to set both of them
  canController.init_Mask(1, 0, 0x3F0);               // precisement : Id 0x10 à 0x4F
  // filtres du buffer 0
  canController.init_Filt(0, 0, 0x10);                // Reception possible : Id 10 & 1F (hex)  : Satellites
  canController.init_Filt(1, 0, 0x40);                // Reception possible : Id 4x (hex) 
  // filtres du buffer 1
  canController.init_Filt(2, 0, 0x10);                // Reception possible : Id 1x (hex) 
  canController.init_Filt(3, 0, 0x40);                // Reception possible : Id 4x (hex) 40 conduite via centrale DCC
  canController.init_Filt(4, 0, 0x43);                // Reception possible : Id 4x (hex) 43 keepalive
  canController.init_Filt(5, 0, 0x48);                // Reception possible : Id 4x (hex) 48 etat DCC
  
  while (CAN_OK != canController.begin(CAN_500KBPS))              // init can bus : baudrate = 500k (carte NiRem a 16 Mhz)
  {
    Serial.println("CAN BUS Shield init fail");
    delay(100);
  }
  Serial.println("CAN BUS Shield init ok!");
  attachInterrupt(0, MCP2515_ISR, FALLING); // start interrupt

  PointWrapper::begin();
  SignalWrapper::begin();

  Serial.println("Initial");
  printOutBuffers();

  tempo = millis();
}

///////////////////////////////////////////

void loop()
{
  
  // réception d'un message Can

  unsigned char len = 0;
  unsigned char buf[8];

  if(FlagReceive)                      // test si message
  {
    FlagReceive=0;
    if (CAN_MSGAVAIL == canController.checkReceive())            // check if data coming
    {
      canController.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
      unsigned int canId = canController.getCanId();
      Serial.print(" ID: 0x");
      Serial.print(canId, HEX);
      Serial.print(" data:");
      for(int i = 0; i<len; i++)    // print the data
      {
        Serial.print(" 0x");
        Serial.print(buf[i], HEX);
      }
      Serial.println();
    }
  }

  // une animation des signaux
  
  if (millis() - tempo > 500)
  {
    tempo = millis();
    Serial.print("signal ");Serial.print(sig);Serial.print(" feu ");Serial.println(feu);
    SignalWrapper::setSignalState(sig, feu);
    sig++;
    if (sig > 9) {
      sig = 0;
      feu = feu << 1;
      if (feu == 0) feu = 1;
    }
  }
  
  sendSatelliteMessage();
}

// utilitaire de commande d'une aiguille a en position i

void aiguille(int a, bool i)
{
  Serial.print("Aiguille ");Serial.println(a);
  PointWrapper::setPointPosition(a,i);
  printOutBuffers();
}

// utilitaire de commande d'un signal nsig avec un type de feux f

void signal(int nsig, unsigned int f)
{
  switch (nsig)
  {
    case 0:
    case 1:
    Serial.print("Semaphore ");Serial.println(nsig);
    break;
    case 4:
    case 5:
    case 6:
    case 7:
    Serial.print("Carre ");Serial.println();
    break;
    case 2:
    case 3:
    Serial.print("CarreRappelRalentissement ");Serial.println(nsig);
    break;    
    case 8:
    case 9:
    Serial.print("SemaphoreRalentissement ");Serial.println(nsig);
    break;    
  }
  SignalWrapper::setSignalState(nsig, f);
  printOutBuffers();
}

///////////////////////////////////////////
