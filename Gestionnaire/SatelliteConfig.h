/*
 * LOCODUINO
 *
 * Locoduinodrome
 *
 * Config des satellites
 */

#ifndef __SATELLITE_CONFIG_H__
#define __SATELLITE_CONFIG_H__

#include "Arduino.h"

static const uint32_t NUMBER_OF_SATELLITES = 8;
static const uint32_t NUMBER_OF_BITS_FOR_SATELLITE_ID = 3;
static const uint32_t OUT_MESSAGE_PERIOD = 1000;
static const uint32_t OUT_SATELLITE_MESSAGE_TYPE = 1;

#endif /* __SATELLITE_CONFIG_H__ */
