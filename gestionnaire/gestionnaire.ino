/*
 * Locoduinodrome
 */
 
#include "SatelliteWrapper.h"

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

void setup()
{
  PointWrapper::begin();
  SignalWrapper::begin();
}

void loop()
{

}
