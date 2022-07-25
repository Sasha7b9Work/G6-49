// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


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

inline void EmptyFuncVV() {};

#define _bitset(bits)                               \
  ((uint8)(                                         \
  (((uint8)((uint)bits / 0x000001) % 0x08) << 0) |  \
  (((uint8)((uint)bits / 0x000008) % 0x08) << 1) |  \
  (((uint8)((uint)bits / 0x000040) % 0x08) << 2) |  \
  (((uint8)((uint)bits / 0x000200) % 0x08) << 3) |  \
  (((uint8)((uint)bits / 0x001000) % 0x08) << 4) |  \
  (((uint8)((uint)bits / 0x008000) % 0x08) << 5) |  \
  (((uint8)((uint)bits / 0x040000) % 0x08) << 6) |  \
  (((uint8)((uint)bits / 0x200000) % 0x08) << 7)))

#define BIN_U8(bits) (_bitset(0##bits))

#define BIN_U16(bits1, bits0) ((BIN_U8(bits1) << 8) | (BIN_U8(bits0)))

#define BIN_U32(bits3, bits2, bits1, bits0) (BIN_U8(bits3) << 24 | BIN_U8(bits2) << 16 | BIN_U8(bits1) << 8 | BIN_U8(bits0))

#define HEX_FROM_2(hex1, hex0) ((uint)(0x##hex1) << 16 | (uint)0x##hex0)


#ifdef WIN32

#define __attribute__(x)

#else

#ifndef NULL
#define NULL 0
#endif
#define nullptr NULL   //-V1059

#endif


#define MAX_UINT ((uint)-1)

#define ERROR_VALUE_FLOAT   1.111e29f
#define ERROR_VALUE_UINT8   255
#define ERROR_STRING_VALUE  "--.--"
