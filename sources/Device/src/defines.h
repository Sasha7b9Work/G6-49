#pragma once
#include <stm32f4xx.h>
#include <usbd_def.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef unsigned int       uint;
typedef unsigned short     uint16;
typedef unsigned short     uint16_t;
typedef signed   short     int16;
typedef unsigned long long uint64;
typedef unsigned char      uint8;
typedef unsigned char      uint8_t;
typedef signed char        int8;
typedef const char * const pString;

typedef void(*pFuncVV)();
typedef void(*pFuncVI)(int);

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

#define BINARY_U16(bits1, bits0) ((BINARY_U8(bits1) << 8) | (BINARY_U8(bits0)))

#ifdef WIN32

#define __attribute__(x)

#else

#define nullptr NULL

#endif


#define MAX_UINT ((uint)-1)

#define ERROR_VALUE_FLOAT   1.111e29f
#define ERROR_VALUE_UINT8   255
#define ERROR_STRING_VALUE  "--.--"


#define DEF_STRUCT(name, type)  \
static const struct name        \
{                               \
    type val;                   \
    name(type v) : val(v) {};   \
}
