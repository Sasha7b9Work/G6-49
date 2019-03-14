#include "stdafx.h"
#ifndef WIN32
#include "Log.h"
#include "Generator/Signals.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Generator/GeneratorPanel.h"
#include "Menu/Menu.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include "Settings/Settings.h"
#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <cmath>
#endif
#include "Utils/IntValue.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CURRENT_POS         (param->hightLightDigit)
#define DIGIT(num)          (param->buffer[num])
#define CURRENT_DIGIT       (param->buffer[CURRENT_POS])
#define POS_COMMA           (param->posComma)
#define ORDER               (param->order)
#define SIGN                (param->sign)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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
        if (!OnlyOneRigthDigit() && POS_COMMA != NUM_DIGITS - 1)
        {
            ShiftToRight();
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char *StructValue::StringValue()
{
    if(!param->IsInputValue())
    {
        return "";
    }

    static char buf[20];
    buf[0] = '\0';



    char text[20];
    std::strcpy(text, param->buffer);
    int numDigits = NUM_DIGITS;
    int posComma = POS_COMMA;


    if (param->Is(ParameterValue::Offset))
    {
        IntValue value(text, posComma);
        value.Sub5(text, &posComma);
    }


    SU::ConcatenateSymbol(buf, SIGN);

    for (int i = 0; i < numDigits; i++)
    {
        SU::ConcatenateSymbol(buf, text[i]);

        if (posComma == i)
        {
            SU::ConcatenateSymbol(buf, '.');
        }
    }

    return buf;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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

    if(ValueNano().ToFloat() < param->MinValue() || ValueNano().ToFloat() > param->MaxValue())
    {
        *this = temp;
        return false;
    }

    return true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool StructValue::DecreaseDigit(int num)
{
    if (num < 0 || num >= NUM_DIGITS)
    {
        return false;
    }

    if (All0LeftWithThis(num) && SIGN == ' ')
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

    if(ValueNano().ToFloat() < param->MinValue() || ValueNano().ToFloat() > param->MaxValue())
    {
        *this = temp;
        return false;
    }

    return true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void StructValue::ShiftToRight()
{
    for (int i = NUM_DIGITS - 2; i >= 0; i--)
    {
        DIGIT(i + 1) = DIGIT(i);
    }
    DIGIT(0) = '0';
    ++POS_COMMA;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void StructValue::ShiftToLeft()
{
    for (int i = 1; i < NUM_DIGITS; i++)
    {
        DIGIT(i - 1) = DIGIT(i);
    }
    DIGIT(NUM_DIGITS - 1) = '0';
    --POS_COMMA;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
pString Order::Name() const
{
    DEF_STRUCT(StructName, const char *) names[Order::Number][2] =
    {
        {"н",   "n"},
        {"мк",  "u"},
        {"м",   "m"},
        {"",    ""},
        {"к",   "k"},
        {"М",   "M"}
    };

    return names[value][LANG].val;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool StructValue::OnlyOneRigthDigit()
{
    char digitLast = DIGIT(NUM_DIGITS - 1);

    if (digitLast != '0' && All0LeftWithThis(NUM_DIGITS - 2))
    {
        return true;
    }
    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int StructValue::ValueBeforeComma()
{
    return (int)(ValueBeforeCommaInNano() / (1000 * 1000 * 1000));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint64 StructValue::ValueBeforeCommaInNano()
{
    uint64 result = 0;

    uint64 pow = 1;

    for (int i = POS_COMMA; i >= 0; i--)
    {
        result += (0x0f & DIGIT(i)) * pow;
        pow *= 10;
    }

    return result * 1000 * 1000 * 1000;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint64 StructValue::ValueAfterCommaInNano()
{
    // Функция возвращает значение в нано-единицах измерения. Т.е.:
    // 9         ==               9 * 1е-9 = 9 нано
    // 999       ==             999 * 1е-9 = 999 нано
    // 999999    ==       999 * 999 * 1e-9 = 999999 нано = 999.999 микро
    // 999999999 == 999 * 999 * 999 * 1e-9 = 999999999 нано = 999999.999 микро == 999.999999 милли

    uint64 result = 0;

    //         123123123
    uint64 pow = 100000000;

    for (int i = POS_COMMA + 1; i < NUM_DIGITS; i++)    // Проходим все знакоместа, начиная с того, что после запятой
    {
        result += (DIGIT(i) & 0x0f) * pow;
        pow /= 10;
    }

    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void StructValue::IncreaseOrder()
{
    if (ORDER < Order::Number - 1)
    {
        ORDER = (Order)(Order::E)(ORDER + 1);

        POS_COMMA -= 3;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ParamValue StructValue::ValueNano()
{
    uint64 beforeNS = ValueBeforeCommaInNano();     // Здесь число до запятой, в нано-единицах
    uint64 afterNS = ValueAfterCommaInNano();       // Здесь число после запятой, в нано-единицах

    ParamValue result(beforeNS + afterNS);          // Теперь здесь количество нано-единиц в предпоожении, что размерность - One

    // Скорректируем значение в соответствии с реальной размерностью

    if (ORDER == Order::Milli)
    {
        result.Divide(1000);
    }
    else if (ORDER == Order::Micro)
    {
        result.Divide(1000 * 1000);
    }
    else if (ORDER == Order::Nano)
    {
        result.Divide(1000 * 1000 * 1000);
    }
    else if (ORDER == Order::Kilo)
    {
        result.Multiplie(1000);
    }
    else if (ORDER == Order::Mega)
    {
        result.Multiplie(1000 * 1000);
    }

    if (SIGN == '-')
    {
        result.SetSign(-1);
    }

    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void StructValue::SaveValue()
{
    SendToGenerator();

    Menu::ResetAdditionPage();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void StructValue::PressKey(Control key)
{
    static const struct StrControl
    {
        Control::E control;
        char       symbol;
        uint8      nu[3];
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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
