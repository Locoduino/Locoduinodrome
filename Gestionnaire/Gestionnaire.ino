
/*
 * Locoduinodrome
 */
#include <mcp_can.h>
#include <mcp_can_dfs.h>
 
/*
 * Interface CAN
 */
//const uint8_t spiCS = 9;
const uint8_t spiCS = 53; // sur un Mega

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

/*
 * Pour l'automate de test
 */

unsigned long tempo;
int nsig = 0;
int sig = 0;
unsigned int feu = 1;

void setup()
{
  Serial.begin(115200);

  
  while (CAN_OK != canController.begin(CAN_500KBPS))              // init can bus : baudrate = 500k
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

void loop()
{
  /*
   * test des signaux et aiguilles 
   */
  unsigned char len = 0;
  unsigned char buf[8];
  bool timeOK;
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

  CommandeDirecte();
  
  sendSatelliteMessage();
}

void Aiguille(int a, bool i)
{
  Serial.print("Aiguille ");Serial.println(a);
  PointWrapper::setPointPosition(a,i);
  printOutBuffers();
}


void Signal(unsigned int f)
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
    Serial.print("Carre ");Serial.println(nsig);
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

void CommandeDirecte()
{
  if (Serial.available()) 
  {
    char code=Serial.read(); 
    switch (code)
    {
      case 'X':
      Aiguille(0, true);
      break;
      case 'x':
      Aiguille(0, false);
      break;
      case 'Y':
      Aiguille(1, true);
      break;
      case 'y':
      Aiguille(1, false);
      break;
      case '=':
      Serial.print("No Signal = ");Serial.println(nsig);
      break;
      case '+':
      if (nsig < 9) {nsig++;Serial.print("No Signal = ");Serial.println(nsig);}
      break;
      case '-':
      if (nsig > 0) {nsig--;Serial.print("No Signal = ");Serial.println(nsig);}
      break;
      case '0':
      Signal(E);
      break;
      case '1':
      Signal(Vl);
      break;
      case '2':
      Signal(A);
      break;
      case '3':
      Signal(S);
      break;
      case '4':
      Signal(C);
      break;
      case '5':
      Signal(R);
      break;
      case '6':
      Signal(RR);
      break;
      case '7':
      Signal(M);
      break;
      case '8':
      Signal(Cv);
      break;
      case '9':
      Signal(Vlc);
      break;
      case 'a':
      Signal(Ac);
      break;
      case 'b':
      Signal(Sc);
      break;
      case 'c':
      Signal(Rc);
      break;
      case 'd':
      Signal(RRc);
      break;
      case 'e':
      Signal(Mc);
      break;
      case 'f':
      Signal(D);
      break;
      case 'g':
      Signal(X);
      break;     
    }
  } 
}


