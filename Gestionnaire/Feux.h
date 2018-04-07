#ifndef __FEUX_H__
#define __FEUX_H__

#include "Arduino.h"

typedef uint16_t typeFeux; // 16 cas possibles ( 32 avec long )

// les feux possibles
enum {
  E=0,        // eteint
  Vl = bit(0),   // 1 ou 0b0000000000000001
  A  = bit(1),   // 2 ou 0b0000000000000010
  S  = bit(2),   // 4 ou 0b0000000000000100
  C  = bit(3),   // 8 ou 0b0000000000001000
  R  = bit(4),   // 16 ou 0b0000000000010000
  RR = bit(5),   // 32 ou 0b0000000000100000
  M  = bit(6),   // 64 ou 0b0000000001000000
  Cv = bit(7),   // 128 ou 0b0000000010000000
  Vlc= bit(8),   // 256 ou 0b0000000100000000
  Ac = bit(9),   // 512 ou 0b0000001000000000
  Sc = bit(10),  // 1024 ou 0b0000010000000000
  Rc = bit(11),  // 2048 ou 0b0000100000000000
  RRc= bit(12),  // 4096 ou 0b0001000000000000
  Mc = bit(13),  // 8192 ou 0b0010000000000000
  D  = bit(14),  // 16384 ou 0b0100000000000000
  X  = bit(15)   // 32768 ou 0b1000000000000000
};

#endif /* __FEUX_H__ */
