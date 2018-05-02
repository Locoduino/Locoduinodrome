// gestion du bus (Can)

/////////////////////////////////////////////////////////////////////////////
//#include <Arduino.h>
//#include <SPI.h>      //  SPI library
#include "CANBus.h"
#include "CANMessage.h"

// CAN parameters

volatile bool FlagReceive = false;      // can interrupt flag
MCP_CAN can(canCS);                     // CAN instance

// message CAN information

#ifndef VISUALSTUDIO

// CAN interrupt routine

void MCP2515_ISR() {FlagReceive = true;}

// initialisation du bus
void CANBus::begin(uint8_t id) // init CAN
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
	attachInterrupt(digitalPinToInterrupt(CANInt), MCP2515_ISR, FALLING); // CAN interrupt
  
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
	Serial.print("CAN initialise avec id 0x"); Serial.println(CanTxId, HEX);

  /*
   * ajouter une séquence de conversation avec le gestionnaire pour prendre en compte
   * l'existence diu satellite
   */
}

// envoi de message sur le bus (CAN)
void CANBus::messageTx()
{
  can.sendMsgBuf(CanTxId, 0, 1, TxBuf);
}

// test et lecture message sur bus (CAN)
bool CANBus::messageRx()
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
void CANBus::begin(uint8_t id)
{
}

// envoi de message sur le bus (CAN)
void CANBus::messageTx()
{
}

// test et lecture message sur bus (CAN) = non utilisé => voir loop
bool CANBus::messageRx()
{
	return false;
}
#endif
