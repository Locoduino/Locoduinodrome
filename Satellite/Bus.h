//-------------------------------------------------------------------
#ifndef __bus_H__
#define __bus_H__
//-------------------------------------------------------------------

// gestion du bus (Can)

#ifndef VISUALSTUDIO

/////////////////////////////////////////////////////////////////////////////
#include <Arduino.h>
#include <SPI.h>      //  SPI library
#include <mcp_can.h>  // MCP2515 library

// CAN parameters

const byte CANInt = 2;                  // interrupt from CAN on pin 2 (int 0)
const byte CS = 10;                     // NANO chip select for CAN
const int baudrate = CAN_500KBPS;       // can throughput 500 Kb/s  
const int speedset = MCP_8MHZ;
const int Id_Mode = CAN_STDID;
const int RETRY_CONNECTION = 10;        // retry connection

MCP_CAN can(CS);                        // CAN instance

volatile bool FlagReceive = false;      // can interrupt flag

// message CAN information

unsigned long CanRx;
unsigned char RxLen = 0;        // length of data, 3 octets by message
unsigned char RxBuf[8];         // buffer of data : byte1 : code, byte2 : numero, byte3: info

unsigned long CanTx;
unsigned char TxLen = 0;        // length of data, 3 octets by message
unsigned char TxBuf[8];         // buffer of data : byte1 : code, byte2 : numero, byte3: info


// CAN interrupt routine

void MCP2515_ISR() {FlagReceive = true;}


// initialisation du bus
void busInit() // init CAN
{
  int repeat = RETRY_CONNECTION;                              // try to open the CAN
  while (repeat > 0) 
  {
    if (CAN_OK == can.begin(Id_Mode, baudrate, speedset)){break;}
    else {repeat--;}
    delay(500);
  }
  pinMode(CANInt,INPUT);                                       // CAN int pin
  attachInterrupt(0,&MCP2515_ISR,FALLING);                     // CAN interrupt

   /*
   * set mask & filters
   */
  can.init_Mask(0, 0, 0x3F0);               // there are 2 mask in mcp2515, you need to set both of them
  can.init_Mask(1, 0, 0x3F0);               // mode standard (0) et Id = pole
   
  can.init_Filt(0, 0, 0x10);                // Reception possible : Id 10 (hex)
  can.init_Filt(1, 0, 0x11);                // Reception possible : Id 11 (hex)
  can.init_Filt(2, 0, 0x12);                // Reception possible : Id 12 (hex)
  can.init_Filt(3, 0, 0x13);                // Reception possible : Id 13 (hex)
  can.init_Filt(4, 0, 0x14);                // Reception possible : Id 14 (hex)
  can.init_Filt(5, 0, 0x15);                // Reception possible : Id 15 (hex)

  CanTx = dip;
}

// envoi de message sur le bus (CAN)
void messageTx(byte code,byte numero,byte info) 
{
  TxBuf[0] = code;
  TxBuf[1] = numero;
  TxBuf[2] = info;

  byte sndStat = can.sendMsgBuf(CanTx, 3, TxBuf);
    
  //if(sndStat == CAN_OK)
      //Serial.println("Message Sent Successfully!");
    //else
      //Serial.println("Error Sending Message...");
}

// test et lecture message sur bus (CAN) = non utilisé => voir loop
boolean messageRx(byte* code,byte* numero,byte* info) { 
  
  // test si message   a completer
  // lecture du message   a completer
  return false; // ou true
} 

/////////////////////////////////////////////////////////////////////////////
#endif
#endif
