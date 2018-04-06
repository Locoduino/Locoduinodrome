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

#include "SatelliteConfig.h"
#include "SatelliteWrapper.h"
#include "AbstractCANMessage.h"
#include "Feux.h"

/*
 * CAN messages
 */
AbstractCANOutSatelliteMessage outSatellitesMessages[NUMBER_OF_SATELLITES];

static const uint8_t NO_MESSAGE_INDEX = 255;
static const uint8_t NO_SATELLITE_INDEX = 255;

/* Envoi du message courant à destination des satellites */
void sendSatelliteMessage()
{
  static uint8_t messageIndex = 0;
  static uint32_t sendDate = 0;

  if (millis() > sendDate) {
    sendDate += OUT_MESSAGE_PERIOD;

    uint32_t frameId =
      (OUT_SATELLITE_MESSAGE_TYPE << NUMBER_OF_BITS_FOR_SATELLITE_ID) |
      outSatellitesMessages[messageIndex].satelliteId();
    /* passe au message suivant */
    messageIndex++;
    if (messageIndex == NUMBER_OF_SATELLITES) messageIndex = 0;
  }
}

/*
 * Lookup the message table for a satellite Id
 */

static uint8_t lookupMessageForId(const uint8_t inSatelliteId)
{
  for (uint8_t messIdx = 0; messIdx < NUMBER_OF_SATELLITES; messIdx++) {
    if (outSatellitesMessages[messIdx].satelliteId() == inSatelliteId ||
        outSatellitesMessages[messIdx].satelliteId() == NO_SATELLITE_ID)
    {
      return messIdx;
    }
  }
  return NO_MESSAGE_INDEX; /* overflow */
}

PointWrapper *PointWrapper::sPointList = NULL;
PointWrapper **PointWrapper::sPointTable = NULL;
int16_t PointWrapper::sHigherPointNumber = -1;

/*
 * Constructeur.
 *
 * Chaque objet PointWrapper établit une correspondance entre l'identifiant
 * de l'aiguillage dans le gestionnaire et le numéro du satellite qui
 * contrôle cet aiguillage. Le constructeur constitue également une liste de
 * ces objets qui est ensuite exploité dans begin.
 */
PointWrapper::PointWrapper(
  const uint16_t inPointNumber,
  const uint8_t inSatelliteId
) :
mPointNumber(inPointNumber),
mSatelliteId(inSatelliteId),
mSatelliteIndex(NO_SATELLITE_INDEX)
{
  /* Ajoute l'aiguillage dans la liste */
  mNext = sPointList;
  sPointList = this;
  /* Note le max des identifiants d'aiguillage dans le gestionnaire */
  if (mPointNumber > sHigherPointNumber) sHigherPointNumber = mPointNumber;
}

/*
 * begin construit la table indexée par l'identifiant d'aiguillage dans le
 * gestionnaire et l'objet PointWrapper qui établit la correspondance
 * entre l'identifiant d'aiguillage et l'identifiant du satellite qui le
 * commande. Il n'est pas nécessaire que les identifiants soient de un en un
 * mais la consommation mémoire dépend du max des identifiants.
 */
void PointWrapper::begin()
{
  if (sHigherPointNumber != -1) {
    sPointTable = new PointWrapper*[sHigherPointNumber + 1];
    if (sPointTable != NULL) {
      for (int16_t i = 0; i <= sHigherPointNumber; i++) {
        sPointTable[i] = NULL;
      }
      PointWrapper *currentPoint = sPointList;
      while (currentPoint != NULL) {
        sPointTable[currentPoint->mPointNumber] = currentPoint;
        /* inscrit l'aiguillage dans la messagerie CAN */
        currentPoint->lookupMessage();
        currentPoint = currentPoint->mNext;
      }
    }
  }
}

void PointWrapper::setPointPosition(
  const int16_t inPointNumber,
  const bool inPosition
)
{
  if (inPointNumber >= 0 && inPointNumber <= sHigherPointNumber) {
    if (sPointTable[inPointNumber] != NULL) {
      sPointTable[inPointNumber]->setPosition(inPosition);
    }
  }
}

void PointWrapper::setPosition(const bool inPosition)
{
  if (mSatelliteIndex != NO_MESSAGE_INDEX) {
    outSatellitesMessages[mSatelliteIndex].setPointPosition(inPosition);
  }
}

void PointWrapper::lookupMessage()
{
  mSatelliteIndex = lookupMessageForId(mSatelliteId);
  if (mSatelliteIndex != NO_MESSAGE_INDEX) {
    outSatellitesMessages[mSatelliteIndex].reserve(mSatelliteId);
  }
}

/*-----------------------------------------------------------------------------
 * SignalWrapper class
 *-----------------------------------------------------------------------------
 */
SignalWrapper *SignalWrapper::sSignalList = NULL;
SignalWrapper **SignalWrapper::sSignalTable = NULL;
int16_t SignalWrapper::sHigherSignalNumber = -1;

void SignalWrapper::lookupMessage()
{
  mSatelliteIndex = lookupMessageForId(mSatelliteId);
  if (mSatelliteIndex != NO_MESSAGE_INDEX) {
    outSatellitesMessages[mSatelliteIndex].reserve(mSatelliteId);
  }
}

void SignalWrapper::setSignalState(
  const int16_t inSignalNumber,
  const uint16_t inState
)
{
  if (inSignalNumber >= 0 && inSignalNumber <= sHigherSignalNumber) {
    if (sSignalTable[inSignalNumber] != NULL) {
      sSignalTable[inSignalNumber]->setState(inState);
    }
  }
}

void SignalWrapper::begin()
{
  if (sHigherSignalNumber != -1) {
    sSignalTable = new SignalWrapper*[sHigherSignalNumber + 1];
    if (sSignalTable != NULL) {
      for (int16_t i = 0; i <= sHigherSignalNumber; i++) {
        sSignalTable[i] = NULL;
      }
      SignalWrapper *currentSignal = sSignalList;
      while (currentSignal != NULL) {
        sSignalTable[currentSignal->mSignalNumber] = currentSignal;
        /* inscrit l'aiguillage dans la messagerie CAN */
        currentSignal->lookupMessage();
        currentSignal = currentSignal->mNext;
      }
    }
  }
}

SignalWrapper::SignalWrapper(
  const uint16_t inSignalNumber,
  const uint8_t inSatelliteId,
  const uint8_t inSlot
) :
mSignalNumber(inSignalNumber),
mSatelliteId(inSatelliteId),
mSlot(inSlot)
{
  /* Ajoute l'aiguillage dans la liste */
  mNext = sSignalList;
  sSignalList = this;
  /* Note le max des identifiants d'aiguillage dans le gestionnaire */
  if (mSignalNumber > sHigherSignalNumber) sHigherSignalNumber = mSignalNumber;
}

/*-----------------------------------------------------------------------------
 * Wrapper pour les sémaphores : 3 feux, jaune, rouge, vert
 *
 * L'ordre des LED sur le satellite est :
 * - jaune
 * - rouge
 * - vert
 */
void SemaphoreSignalWrapper::setState(const uint16_t inState)
{
  if (mSatelliteIndex != NO_MESSAGE_INDEX) {
    AbstractCANOutSatelliteMessage &message = outSatellitesMessages[mSatelliteIndex];
    message.setLED(mSlot, inState & A ? LED_ON : LED_OFF);      /* jaune */
    message.setLED(mSlot + 1, inState & S ? LED_ON : LED_OFF);  /* rouge */
    message.setLED(mSlot + 2, inState & Vl ? LED_ON : LED_OFF); /* vert  */
  }
}

/*-----------------------------------------------------------------------------
 * Wrapper pour les carrés
 *
 * L'ordre des LED sur le satellite est :
 * - jaune
 * - rouge
 * - vert
 * - rouge2
 * - oeilleton (blanc)
 */
void CarreSignalWrapper::setState(const uint16_t inState)
{
  if (mSatelliteIndex != NO_MESSAGE_INDEX) {
    AbstractCANOutSatelliteMessage &message = outSatellitesMessages[mSatelliteIndex];
    message.setLED(mSlot, inState & A ? LED_ON : LED_OFF);        /* jaune */
    message.setLED(mSlot + 1, inState & S ? LED_ON : inState & C ? LED_ON : LED_OFF);    /* rouge */
    message.setLED(mSlot + 2, inState & Vl ? LED_ON : LED_OFF);   /* vert  */
    message.setLED(mSlot + 3, inState & C ? LED_ON : LED_OFF);    /* rouge2 */
    message.setLED(mSlot + 4, inState & C ? LED_OFF : LED_ON); /* oeilleton */
  }
}

/*-----------------------------------------------------------------------------
 * Wrapper pour les s&maphores avec ralentissement
 *
 * L'ordre des LED sur le satellite est :
 * - jaune
 * - rouge
 * - vert
 * - jaune2
 * - jaune3
 */
void SemaphoreRalentissementSignalWrapper::setState(const uint16_t inState)
{
  if (mSatelliteIndex != NO_MESSAGE_INDEX) {
    AbstractCANOutSatelliteMessage &message = outSatellitesMessages[mSatelliteIndex];
    message.setLED(mSlot, inState & A ? LED_ON : LED_OFF);      /* jaune */
    message.setLED(mSlot + 1, inState & S ? LED_ON : LED_OFF);  /* rouge */
    message.setLED(mSlot + 2, inState & Vl ? LED_ON : LED_OFF); /* vert  */
    message.setLED(mSlot + 3, inState & R ? LED_ON : inState & Rc ? LED_BLINK : LED_OFF);  /* jaune2 */
    message.setLED(mSlot + 4, inState & R ? LED_ON : inState & Rc ? LED_BLINK : LED_OFF);  /* jaune3 */
  }
}

/*-----------------------------------------------------------------------------
 * Wrapper pour les carrés avec rappel ralentissement
 *
 * L'ordre des LED sur le satellite est :
 * - jaune
 * - rouge
 * - vert
 * - rouge2
 * - jaune2
 * - jaune3
 * - oeilleton
 */
void CarreRappelRalentissementSignalWrapper::setState(const uint16_t inState)
{
  if (mSatelliteIndex != NO_MESSAGE_INDEX) {
    AbstractCANOutSatelliteMessage &message = outSatellitesMessages[mSatelliteIndex];
    message.setLED(mSlot, inState & A ? LED_ON : LED_OFF);      /* jaune */
    message.setLED(mSlot + 1, inState & C ? LED_ON : LED_OFF);  /* rouge */
    message.setLED(mSlot + 2, inState & Vl ? LED_ON : LED_OFF); /* vert  */
    message.setLED(mSlot + 3, inState & C ? LED_ON : LED_OFF);  /* rouge2 */
    message.setLED(mSlot + 4, inState & RR ? LED_ON : inState & RRc ? LED_BLINK : LED_OFF);  /* jaune2 */
    message.setLED(mSlot + 5, inState & RR ? LED_ON : inState & RRc ? LED_BLINK : LED_OFF);  /* jaune3 */
    message.setLED(mSlot + 6, inState & C ? LED_OFF : LED_ON); /* oeilleton */
  }
}
