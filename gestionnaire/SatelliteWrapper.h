/*
 * LOCODUINO
 *
 * Locoduinodrome
 *
 * Classe de wrapping des satellites dans le gestionnaire.
 *
 * Ces classes permettent au gestionnaire de s'interfacer à la messagerie
 * CAN lui permettant de dialoguer avec les satellites.
 */

#ifndef __SATELLITE_WRAPPER_H__
#define __SATELLITE_WRAPPER_H__

#include "Arduino.h"

class PointWrapper
{
private:
  static PointWrapper *sPointList;
  static PointWrapper **sPointTable;
  static int16_t sHigherPointNumber;

  int16_t mPointNumber;    /* numéro de l'aiguillage dans le gestionnaire */
  uint8_t mSatelliteId;    /* indentifiant du satellite                   */
  uint8_t mSatelliteIndex; /* index du satellite dans la table            */
  PointWrapper *mNext;

  void setPosition(const bool inPosition);
  void lookupMessage();

public:
  /*
   * begin construit la table indexée par le numéro d'aiguillage à partir de
   * la liste. La liste est construite par le constructeur.
   * begin demande également à chaque aiguillage de chercher leur
   * message CAN en fonction de l'identifiant du satellite.
   * À appeler dans setup.
   */
  static void begin();

  /*
   * setPointPosition positionne l'aiguillage inPointNumber à la position
   * inPosition
   */
  static void setPointPosition(const int16_t inPointNumber, const bool inPosition);

  /*
   * constructeur
   */
  PointWrapper(const uint16_t inPointNumber, const uint8_t inSatelliteId);

};

enum { SIGNAL_1 = 0, SIGNAL_2 = 1, SIGNAL_3 = 2 };

class SignalWrapper
{
private:
  static SignalWrapper *sSignalList;
  static SignalWrapper **sSignalTable;
  static uint16_t sHigherSignalNumber;

  int16_t mSignalNumber;
  uint8_t mSatelliteId;
  uint8_t mSatelliteIndex;
  uint8_t mSlot;
  SignalWrapper *mNext;

  void setState(const uint16_t inState);
  void lookupMessage();

public:
  static void begin();
  static void setSignalState(const uint16_t inState);

  SignalWrapper(const uint16_t inSignalNumber, const uint8_t inSatelliteId, const uint8_t inSlot);
};

#endif /* __SATELLITE_WRAPPER_H__ */
