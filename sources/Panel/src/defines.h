#pragma once

// warning: use of old-style cast
#pragma clang diagnostic ignored "-Wold-style-cast"

// warning: variadic macros are incompatible with C++98
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"

// warning: illegal character encoding in string literal
#pragma clang diagnostic ignored "-Winvalid-source-encoding"

// warning: array subscript is of type 'char'
#pragma clang diagnostic ignored "-Wchar-subscripts"

// warning: format string is not a string literal
#pragma clang diagnostic ignored "-Wformat-nonliteral"

// warning: declartation requires a global constuctor
#pragma clang diagnostic ignored "-Wglobal-constructors"

// warning: cast from ... to ... increase required aligment from ... to ...
#pragma clang diagnostic ignored "-Wcast-align"

// warning: ISO C++11 does not allow conversion from string literal to 'char *'
#pragma clang diagnostic ignored "-Wwritable-strings"


#define DEBUG


typedef const char *const   pString;
typedef unsigned int        uint;
typedef unsigned short int  uint16;
typedef signed short int    int16;
typedef unsigned char       uint8;
typedef unsigned char       uchar;
typedef signed char         int8;

#define MAX_UINT ((uint)-1)

#define NUM_ITEMS_ON_PAGE   4

typedef void    (*pFuncVV)();
typedef void    (*pFuncVII)(int, int);

// Для определения задания буферов лучше пользоваться этими макросами, чтобы потом легче было отследить, где можно памяти освободить
#define CHAR_BUF(name, size)            char name[size]
#define CHAR_BUF2(name, size1, size2)   char name[size1][size2]

#define _GET_BIT(value, bit)                    (((value) >> bit) & 0x01)
#define _SET_BIT(value, bit)                    ((value) |= (1 << bit))
#define _CLEAR_BIT(value, bit)                  ((value) &= (~(1 << bit)))
#define _SET_BIT_VALUE(value, numBit, bitValue) (value |= (bitValue << numBit))

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
