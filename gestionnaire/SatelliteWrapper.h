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

void printOutBuffers();

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

enum { SIGNAL_0 = 0, SIGNAL_1 = 3, SIGNAL_2 = 6 };

/*-----------------------------------------------------------------------------
 * La classe SignalWrapper est une classe abtraite représentant
 * la correspondance entre un signal du gestionnaire et le signal matériel
 * sur le satellite.
 */
class SignalWrapper
{
private:
  static SignalWrapper *sSignalList;
  static SignalWrapper **sSignalTable;
  static int16_t sHigherSignalNumber;

  int16_t mSignalNumber;    /* Numéro du signal dans le gestionnaire */
  uint8_t mSatelliteId;     /* Identifiant du satellite sur lequel le signal est implanté */
  SignalWrapper *mNext;

  virtual void setState(const uint16_t inState) = 0;
  void lookupMessage();

protected:
  uint8_t mSatelliteIndex;  /* Index du satellite dans la table */
  uint8_t mSlot;            /* Slot / connecteur sur lequel le signal est connecté sur le satellite */

public:
  static void begin();
  static void setSignalState(const int16_t inSignalNumber, const uint16_t inState);

  SignalWrapper(const uint16_t inSignalNumber, const uint8_t inSatelliteId, const uint8_t inSlot);
};

/*-----------------------------------------------------------------------------
 * Wrapper pour les sémaphores : 3 feux, jaune, rouge, vert
 */
class SemaphoreSignalWrapper : public SignalWrapper
{
private:
  virtual void setState(const uint16_t inState);

public:
  SemaphoreSignalWrapper(const uint16_t inSignalNumber, const uint8_t inSatelliteId, const uint8_t inSlot) :
    SignalWrapper(inSignalNumber, inSatelliteId, inSlot) {}
};

/*-----------------------------------------------------------------------------
 * Wrapper pour les carrés
 */
class CarreSignalWrapper : public SignalWrapper
{
private:
  virtual void setState(const uint16_t inState);

public:
  CarreSignalWrapper(const uint16_t inSignalNumber, const uint8_t inSatelliteId, const uint8_t inSlot) :
    SignalWrapper(inSignalNumber, inSatelliteId, inSlot) {}
};

/*-----------------------------------------------------------------------------
 * Wrapper pour les s&maphores avec ralentissement
 */
class SemaphoreRalentissementSignalWrapper : public SignalWrapper
{
private:
  virtual void setState(const uint16_t inState);

public:
  SemaphoreRalentissementSignalWrapper(const uint16_t inSignalNumber, const uint8_t inSatelliteId, const uint8_t inSlot) :
    SignalWrapper(inSignalNumber, inSatelliteId, inSlot) {}
};

/*-----------------------------------------------------------------------------
 * Wrapper pour les s&maphores avec ralentissement
 */
class CarreRappelRalentissementSignalWrapper : public SignalWrapper
{
private:
  virtual void setState(const uint16_t inState);

public:
  CarreRappelRalentissementSignalWrapper(const uint16_t inSignalNumber, const uint8_t inSatelliteId, const uint8_t inSlot) :
    SignalWrapper(inSignalNumber, inSatelliteId, inSlot) {}
};

#endif /* __SATELLITE_WRAPPER_H__ */
