#pragma once


// warning: use of old-style cast
#pragma clang diagnostic ignored "-Wold-style-cast"

// warning: padding struct ... with 2 bytes to align
#pragma clang diagnostic ignored "-Wpadded"

// warning: variadic macros are incompatible with C++98
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"

// warning: illegal character encoding in string literal
#pragma clang diagnostic ignored "-Winvalid-source-encoding"

// warning: array subscript is of type 'char'
#pragma clang diagnostic ignored "-Wchar-subscripts"

#include <stm32f4xx.h>

typedef unsigned int        uint;
typedef unsigned short int  uint16;
typedef signed short int    int16;
typedef unsigned char       uint8;
typedef unsigned char       uchar;
typedef signed char         int8;

typedef enum
{
    COLOR_BACK          = 0x00,
    COLOR_FILL          = 0x01,
    COLOR_EMPTY_DIGIT    = 0x02,
    COLOR_MENU_TITLE     = 0x03,
    COLOR_MENU_ITEM_DARK = 0x04,
    NUM_COLORS,
    COLOR_FLASH_10,
    COLOR_FLASH_01
} Color;

#define MAX_UINT ((uint)-1)

#define NUM_ITEMS_ON_PAGE   4

typedef void    (*pFuncVV)();
typedef void    (*pFuncVII)(int, int);

#define GetBit(value, bit) (((value) >> (bit)) & 0x01)
#define SetBit(value, bit) ((value) |= (1 << (bit)))
#define ClearBit(value, bit) ((value) &= (~(1 << bit)))

#define ENABLED_RU "ÂÊË"
#define ENABLED_EN "ON"
#define DISABLED_RU "ÎÒÊË"
#define DISABLED_EN "OFF"
