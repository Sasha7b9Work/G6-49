#pragma once
#include <stm32f4xx.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef unsigned int       uint;
typedef unsigned short     uint16;
typedef unsigned long long uint64;
typedef unsigned char      uint8;
typedef const char * const pString;

typedef void(*pFuncVV)();
typedef void(*pFuncVI)(int);

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

#ifndef __IO
#define __IO volatile
#endif
#define uint32_t uint

#endif

#define MAX_UINT ((uint)-1)
