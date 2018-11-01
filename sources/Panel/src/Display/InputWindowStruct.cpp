#include "stdafx.h"
#ifndef WIN32
#include "Log.h"
#include "Generator/Signals.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Generator/Generator.h"
#include "Menu/Menu.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include "Settings/Settings.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CURRENT_POS         (param->hightLightDigit)
#define DIGIT(num)          (param->buffer[num])
#define CURRENT_DIGIT       (param->buffer[CURRENT_POS])
#define POS_COMMA           (param->posComma)


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void StructValue::Set(ParameterValue *param_)
{
    param = param_;

	for (int i = NUM_DIGITS - 1; i > 0; --i)
	{
		if (DIGIT(i) == 0)
		{
			DIGIT(i) = '0';
		}
		else
		{
			break;
		}
	}
    
    param->hightLightDigit = 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void StructValue::KeyLeft()
{
	if (param->hightLightDigit > 0)
	{
		if (param->hightLightDigit == 1 && DIGIT(0) == '.')
		{
			return;
		}
		--param->hightLightDigit;
		if (CURRENT_DIGIT == '.')
		{
            KeyLeft();
		}
	}
    else
    {
        if (!OnlyOneRigthDigit() && param->posComma != NUM_DIGITS - 1)
        {
            ShiftToRight();
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void StructValue::KeyRight()
{
	if (param->hightLightDigit < NUM_DIGITS - 1)
	{
		if (param->hightLightDigit == NUM_DIGITS - 2 && param->buffer[NUM_DIGITS - 1] == '.')
		{
			return;
		}
		++param->hightLightDigit;
		if (CURRENT_DIGIT == '.')
		{
            KeyRight();
		}
	}
    else if(DIGIT(0) == '0')
    {
        ShiftToLeft();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void StructValue::RegLeft()
{
    if(DecreaseDigit(param->hightLightDigit))
    {
        if (TUNE_FULL)
        {
            SendToGenerator();
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void StructValue::RegRight()
{
    if(IncreaseDigit(param->hightLightDigit))
    {
        if(ValueBeforeComma() > 999)
        {
            IncreaseOrder();
        }
        if(TUNE_FULL)
        {
            SendToGenerator();
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
char *StructValue::StringValue()
{
    if(!param->IsInputValue())
    {
        return "";
    }

    static char buf[20];
    buf[0] = '\0';

    SU::ConcatenateSymbol(buf, param->sign);

    for (int i = 0; i < NUM_DIGITS; i++)
    {
        SU::ConcatenateSymbol(buf, DIGIT(i));

        if (param->posComma == i)
        {
            SU::ConcatenateSymbol(buf, '.');
        }
    }

    return buf;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool StructValue::IncreaseDigit(int num)
{
    if (num < 0 || num >= NUM_DIGITS)
    {
        return false;
    }

    StructValue temp = *this;

    if (All9LeftWithThis(num))
    {
        ShiftToRight();
        ++param->hightLightDigit;
        IncreaseDigit(param->hightLightDigit);
    }
    else
    {
        DIGIT(num)++;
        if (DIGIT(num) > '9')
        {
            DIGIT(num) = '0';
            if(!IncreaseDigit(DIGIT(num - 1) == '.' ? num - 2 : num - 1))
            {
                *this = temp;
                return false;
            }
        }
    }

    if(Value() < param->MinValue() || Value() > param->MaxValue())
    {
        *this = temp;
        return false;
    }

    return true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool StructValue::DecreaseDigit(int num)
{
    if (num < 0 || num >= NUM_DIGITS)
    {
        return false;
    }

    if (All0LeftWithThis(num) && param->sign == ' ')
    {
        return false;
    }

    StructValue temp = *this;

    DIGIT(num)--;

    if (DIGIT(num) < '0')
    {
        DIGIT(num) = '9';
        DecreaseDigit(DIGIT(num - 1) == '.' ? num - 2 : num - 1);
    }

    if(Value() < param->MinValue() || Value() > param->MaxValue())
    {
        *this = temp;
        return false;
    }

    return true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool StructValue::All0LeftWithThis(int num)
{
    for (int i = num; i >= 0; i--)
    {
        if (DIGIT(i) != '0')
        {
            return false;
        }
    }
    return true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool StructValue::All9LeftWithThis(int num)
{
    for (int i = num; i >= 0; i--)
    {
        if (DIGIT(i) != '9' && DIGIT(i) != '.')
        {
            return false;
        }
    }
    return true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void StructValue::ShiftToRight()
{
    for (int i = NUM_DIGITS - 2; i >= 0; i--)
    {
        DIGIT(i + 1) = DIGIT(i);
    }
    DIGIT(0) = '0';
    ++POS_COMMA;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void StructValue::ShiftToLeft()
{
    for (int i = 1; i < NUM_DIGITS; i++)
    {
        DIGIT(i - 1) = DIGIT(i);
    }
    DIGIT(NUM_DIGITS - 1) = '0';
    --POS_COMMA;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
pString Order::Name() const
{
    struct StructName
    {
        pString name;
        StructName(pString n) : name(n) {};
    }
    names[Order::Number][2] =
    {
        {"ם",   "n"},
        {"לך",  "u"},
        {"ל",   "m"},
        {"",    ""},
        {"ך",   "k"},
        {"ּ",   "M"}
    };

    return names[value][LANG].name;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool StructValue::OnlyOneRigthDigit()
{
    char digitLast = DIGIT(NUM_DIGITS - 1);

    if (digitLast != '0' && All0LeftWithThis(NUM_DIGITS - 2))
    {
        return true;
    }
    return false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int StructValue::ValueBeforeComma()
{
    int lowPos = param->posComma;     // ּכאהרטי באיע קטסכא

    int retValue = 0;

    int pow = 1;

    for (int i = lowPos; i >= 0; i--)
    {
        retValue += (0x0f & DIGIT(i)) * pow;
        pow *= 10;
    }

    return retValue;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
float StructValue::ValueAfterComma()
{
    int retValue = 0;
    int pow = 1;
    for (int i = NUM_DIGITS - 1; i > param->posComma; i--)
    {
        char digit = DIGIT(i);
        digit &= 0x0f;
        retValue += digit * pow;
        pow *= 10;
    }

    return 1.0f / pow * retValue;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void StructValue::IncreaseOrder()
{
    if (param->order < Order::Number - 1)
    {
        param->order = (Order)(Order::E)(param->order + 1);

        POS_COMMA -= 3;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
float StructValue::Value()
{
    float value = ValueBeforeComma() + ValueAfterComma();

    float mul = (param->sign == '-') ? -1.0f : 1.0f;

    if (param->order == Order::Nano)
    {
        return value * 1e-9f * mul;
    }
    if (param->order == Order::Micro)
    {
        return value * 1e-6f * mul;
    }
    if (param->order == Order::Milli)
    {
        return value * 1e-3f * mul;
    }
    if (param->order == Order::Kilo)
    {
        return value * 1e3f * mul;
    }
    if (param->order == Order::Mega)
    {
        return value * 1e6f * mul;
    }

    return value * mul;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void StructValue::SaveValue()
{
    SendToGenerator();

    Menu::ResetAdditionPage();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void StructValue::PressKey(Control key)
{
    static const struct StrControl
    {
        Control control;
        char    symbol;
    }
    command[] =
    {
        {Control::_1, '1'}, {Control::_2, '2'}, {Control::_3, '3'}, {Control::_4, '4'}, {Control::_5, '5'},
        {Control::_6, '6'}, {Control::_7, '7'}, {Control::_8, '8'}, {Control::_9, '9'}, {Control::_0, '0'},
        {Control::None, '.'}
    };

    int i = 0;
    while(command[i].control != Control::None)
    {
        if(command[i].control == key)
        {
            CURRENT_DIGIT = command[i].symbol;
            KeyRight();
            break;
        }
        i++;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void StructValue::SendToGenerator()
{
    if (param->Is(ParameterValue::Delay))
    {
    }
    else
    {
        Generator::SetParameter(param);
    }
}
