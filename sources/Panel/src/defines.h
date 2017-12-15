#pragma once
#include <stm32f4xx.h>
#include <stm32f4xx_hal.h>


typedef unsigned int        uint;
typedef unsigned short int  uint16;
typedef signed short int    int16;
typedef unsigned char       uint8;
typedef unsigned char       uchar;
typedef signed char         int8;

void _Error_Handler(char *, int);
#define ERROR_HANDLER() /* _Error_Handler(__FILE__, __LINE__) */

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
typedef bool    (*pFuncBV)();
typedef void    (*pFuncVII)(int, int);

#define GetBit(value, bit) (((value) >> (bit)) & 0x01)
#define SetBit(value, bit) ((value) |= (1 << (bit)))
#define ClearBit(value, bit) ((value) &= (~(1 << bit)))

#define ENABLED_RU "ÂÊË"
#define ENABLED_EN "ON"
#define DISABLED_RU "ÎÒÊË"
#define DISABLED_EN "OFF"
