#pragma once
#include "defines.h"
#include "Hardware/Controls.h"
#include "Menu/MenuItems.h"
#include "Settings/SettingsTypes.h"


#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4514)
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define NUM_DIGITS          5


struct Order
{
    enum E
    {
        Nano,       // нано
        Micro,      // микро
        Milli,      // милли
        One,        // единицы
        Kilo,       // кило
        Mega,       // мега
        Number
    } value;
    Order(E v) : value(v) {};
    operator uint8() const { return (uint8)value; };
    pString Name() const;
};

struct SignValue
{
    enum E
    {
        None,
        Minus,
        Plus
    } value;
    SignValue(E v) : value(v) {};
    operator uint8() const { return (uint8)value; };
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const char *NameUnit(char buffer[10], Order order, Wave::Parameter parameter);


#ifdef WIN32
#pragma warning(pop)
#endif
