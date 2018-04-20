//-------------------------------------------------------------------
#ifndef __bus_H__
#define __bus_H__
//-------------------------------------------------------------------

// gestion du bus (Can)

#ifndef VISUALSTUDIO

/////////////////////////////////////////////////////////////////////////////
//#include <Arduino.h>
//#include <SPI.h>      //  SPI library
#include <mcp_can.h>    // MCP2515 library
#include <mcp_can_dfs.h>
#include "CANMessage.h"

// CAN parameters

const uint8_t CANInt = 2;                  // interrupt from CAN on pin 2 (int 0)
const uint8_t canCS = 10;                     // 328P chip select for CAN

MCP_CAN can(canCS);                        // CAN instance

const int baudrate = CAN_500KBPS;       // can throughput 500 Kb/s  
const int RETRY_CONNECTION = 10;        // retry connection

volatile bool FlagReceive = false;      // can interrupt flag

// message CAN information

unsigned long CanRxId;
unsigned char RxLen = 0;        // length of data, 3 octets by message
unsigned char RxBuf[8];         // buffer of data : byte1 : code, byte2 : numero, byte3: info

unsigned long CanTxId;
unsigned char TxLen = 0;        // length of data, 3 octets by message
unsigned char TxBuf[8];         // buffer of data : byte1 : code, byte2 : numero, byte3: info


// CAN interrupt routine

void MCP2515_ISR() {FlagReceive = true;}


// initialisation du bus
void busInit(uint8_t id) // init CAN
{
	uint8_t canFilter = 0x20 + id;                              // messages recus = 0x20 + Id
	int repeat = RETRY_CONNECTION;                              // try to open the CAN
	while (repeat > 0)
	{
		if (CAN_OK == can.begin(baudrate)) { break; }
		else { repeat--; }
		delay(500);
	}
	pinMode(CANInt, INPUT);                                       // CAN int pin
	attachInterrupt(0, MCP2515_ISR, FALLING);                     // CAN interrupt

	/*
	* set mask & filters
	* frameId = 32 + mSatelliteId = (32, 33, 34, 35, 36, 37, 38, 39}
	*/
	can.init_Mask(0, 0, 0x3FF);               // there are 2 mask in mcp2515, you need to set both of them
	can.init_Mask(1, 0, 0x3FF);               // mode standard (0), 

	can.init_Filt(0, 0, canFilter);                // Reception possible : Id 32 (hex) + mSatelliteId
	can.init_Filt(1, 0, canFilter);                // Reception possible : Id 32 (hex)
	can.init_Filt(2, 0, canFilter);                // Reception possible : Id 32 (hex)
	can.init_Filt(3, 0, canFilter);                // Reception possible : Id 32 (hex)
	can.init_Filt(4, 0, canFilter);                // Reception possible : Id 32 (hex)
	can.init_Filt(5, 0, canFilter);                // Reception possible : Id 32 (hex)

	CanTxId = 0x10 + id;                           // messages emis = 0x10 + Id
	Serial.print("CAN initialise avec id "); Serial.println(CanTxId);

  /*
   * ajouter une séquence de conversation avec le gestionnaire pour prendre en compte
   * l'existence diu satellite
   */
}

// envoi de message sur le bus (CAN)
void messageTx() 
{
  byte sndStat = can.sendMsgBuf(CanTxId, 0, 3, TxBuf);
 
}

// test et lecture message sur bus (CAN)
boolean messageRx() 
{
  //if (FlagReceive) 
  //{
  //  FlagReceive = false;   // if receive message from CAN bus
    if (CAN_MSGAVAIL == can.checkReceive()) 
    {
      can.readMsgBuf(&RxLen,RxBuf);
      CanRxId = can.getCanId();
      Serial.print("Rid: ");Serial.print(CanRxId);
      if (RxLen == 3)
      {
        Serial.print(" 0x");Serial.print(RxBuf[0], HEX);
        Serial.print(" 0x");Serial.print(RxBuf[1], HEX);
        Serial.print(" 0x");Serial.println(RxBuf[2], HEX);
        return true;
      } else
      {
        Serial.print("Wrong RxLen:");Serial.println(RxLen);
      }
    }
  //}
  return false; // ou true
} 

/////////////////////////////////////////////////////////////////////////////
#else
unsigned char RxBuf[8];         // buffer of data : byte1 : code, byte2 : numero, byte3: info
unsigned char TxBuf[8];         // buffer of data : byte1 : code, byte2 : numero, byte3: info

void busInit(uint8_t id)
{
}

// envoi de message sur le bus (CAN)
void messageTx()
{
}

// test et lecture message sur bus (CAN) = non utilisé => voir loop
boolean messageRx() 
{
	return false;
}
#endif
#endif
