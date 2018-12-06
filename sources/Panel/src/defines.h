#pragma once

#ifdef WIN32

#define __ARMCLIB_VERSION 6070001

#else

#define nullptr NULL;

#endif

#define DEBUG

#include <stm32f4xx.h>
#include <stm32f4xx_hal.h>

#ifndef __cplusplus
typedef int bool;
#endif

typedef const char *const   pString;
typedef unsigned int        uint;
typedef unsigned short int  uint16;
typedef unsigned long long int uint64;
typedef signed short int    int16;
typedef unsigned char       uint8;
typedef unsigned char       uchar;
typedef signed char         int8;
typedef unsigned int        col_val;

#define MAX_UINT ((uint)-1)

typedef void (*pFuncVV)();
typedef void (*pFuncVII)(int, int);
typedef void (*pFuncVI)(int);
typedef bool (*pFuncBV)();
typedef void (*pFuncVB)(bool);

// Для определения задания буферов лучше пользоваться этими макросами, чтобы потом легче было отследить, где можно памяти освободить
#define CHAR_BUF(name, size)            char name[size]
#define CHAR_BUF2(name, size1, size2)   char name[size1][size2]

#define _GET_BIT(value, bit)        (((value) >> bit) & 0x01)
#define _SET_BIT(value, bit)        ((value) |= (1 << bit))
#define _CLEAR_BIT(value, bit)      ((value) &= (~(1 << bit)))

#define ERROR_VALUE_FLOAT   1.111e29f
#define ERROR_VALUE_UINT8   255
#define ERROR_STRING_VALUE  "--.--"

#define GetBit(value, bit) (((value) >> (bit)) & 0x01)
#define SetBit(value, bit) ((value) |= (1 << (bit)))
#define ClearBit(value, bit) ((value) &= (~(1 << bit)))

#define ENABLED_RU  "Вкл"
#define ENABLED_EN  "On"
#define DISABLED_RU "Откл"
#define DISABLED_EN "Off"

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

#define BINARY_U32(bits3, bits2, bits1, bits0) ((BINARY_U16(bits3, bits2) << 16) | (BINARY_U16(bits1, bits0)))

#define DEF_STRUCT(name, type)  \
static const struct name        \
{                               \
    type val;                   \
    name(type v) : val(v) {};   \
}
