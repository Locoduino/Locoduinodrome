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

/*
 * CAN messages
 */
AbstractCANOutSatelliteMessage outSatellitesMessages[NUMBER_OF_SATELLITES];

PointWrapper *PointWrapper::sPointList = NULL;
PointWrapper **PointWrapper::sPointTable = NULL;
int16_t PointWrapper::sHigherPointNumber = -1;

static const uint8_t NO_MESSAGE_INDEX = 255;

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
mSatelliteIndex(NO_MESSAGE_INDEX)
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
      for (uint16_t i = 0; i <= sHigherPointNumber; i++) {
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
