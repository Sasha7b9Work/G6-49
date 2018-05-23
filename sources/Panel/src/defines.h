#pragma once

#ifdef WIN32
#define __ARMCLIB_VERSION 6070001
#endif

#define DEBUG

#ifdef STM32F429xx
#include <stm32f4xx.h>
#endif

#ifdef STM32F746xx
#include <stm32f7xx.h>
#endif

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

#define NUM_ITEMS_ON_PAGE   4

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

#define ENABLED_RU "ВКЛ"
#define ENABLED_EN "ON"
#define DISABLED_RU "ОТКЛ"
#define DISABLED_EN "OFF"

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
