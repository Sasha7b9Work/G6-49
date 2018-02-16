#pragma once
#include "Generator/GeneratorSettingsTypes.h"
#include <stm32f4xx_hal.h>

// warning: use of old-style cast
#pragma clang diagnostic ignored "-Wold-style-cast"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void _Error_Handler(char *, int);
#define Error_Handler() _Error_Handler(__FILE__, __LINE__)


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef unsigned int   uint;
typedef unsigned short uint16;
typedef unsigned char  uint8;

typedef void(*pFuncVV)();
typedef void(*pFuncVI)(int);
typedef void(*pFuncVCh)(Channel);

#define GetBit(value, bit) (((value) >> (bit)) & 0x01)
#define SetBit(value, bit) ((value) |= (1 << (bit)))
#define ClearBit(value, bit) ((value) &= (~(1 << bit)))

#define _bitset(bits)                               \
  ((uint8)(                                         \
  (((uint8)((uint)bits / 01)        % 010) << 0) |  \
  (((uint8)((uint)bits / 010)       % 010) << 1) |  \
  (((uint8)((uint)bits / 0100)      % 010) << 2) |  \
  (((uint8)((uint)bits / 01000)     % 010) << 3) |  \
  (((uint8)((uint)bits / 010000)    % 010) << 4) |  \
  (((uint8)((uint)bits / 0100000)   % 010) << 5) |  \
  (((uint8)((uint)bits / 01000000)  % 010) << 6) |  \
  (((uint8)((uint)bits / 010000000) % 010) << 7)))

#define BINARY_U8( bits ) _bitset(0##bits)

#ifdef _CONSOLE

#define __IO
#define uint32_t uint

#endif
