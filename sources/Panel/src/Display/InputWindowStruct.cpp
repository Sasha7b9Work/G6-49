#include "Log.h"
#include "Generator/Signals.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Generator/Generator_p.h"
#include "Menu/Menu.h"
#include "Utils/IntValue.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include "Settings/Settings.h"
#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <cmath>



#define CURRENT_POS         (param->hightLightDigit)
#define DIGIT(num)          (param->buffer[num])
#define CURRENT_DIGIT       (param->buffer[CURRENT_POS])
#define POS_COMMA           (param->posComma)
#define ORDER               (param->order)
#define SIGN                (param->sign)



void StructValue::Set(ParameterValue *param_)
{
    param = param_;

	for (int i = param->numDigits - 1; i > 0; --i)
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
        if (!OnlyOneRigthDigit() && (POS_COMMA != param->numDigits - 1))
        {
            ShiftToRight();
        }
    }
}


void StructValue::KeyRight()
{
	if (param->hightLightDigit < (param->numDigits - 1))
	{
		if ((param->hightLightDigit == param->numDigits - 2) &&
            param->buffer[param->numDigits - 1] == '.')
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
    else
    {
        // здесь ничего
    }
}


void StructValue::SetParameterIfUnderLimit(const ParameterValue &_param)
{
    if (param->Is(ParameterValue::Offset) || param->Is(ParameterValue::Amplitude))
    {
        float offset = ((int)(CURRENT_FORM->GetOffset() * 100.0F)) / 100.0F;
        float amplitude = ((int)(CURRENT_FORM->GetAmplitude() * 100.0F)) / 100.0F;

        if ((std::fabsf(offset) + amplitude / 2.0F) > 5.0F)
        {
            *param = _param;
        }
    }
}


void StructValue::RegLeft()
{
    ParameterValue old = *param;        // Сохраняем предыдущее значение параметра

    if(DecreaseDigit(param->hightLightDigit))
    {
        if (TUNE_FULL)
        {
            SendToGenerator();
        }
    }

    SetParameterIfUnderLimit(old);
}


void StructValue::RegRight()
{
    ParameterValue old = *param;

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

    SetParameterIfUnderLimit(old);
}


char *StructValue::StringDigits()
{
    if(!param->IsInputValue())
    {
        return "";
    }

    static char buf[20];
    buf[0] = '\0';

    char text[20];
    std::strcpy(text, param->buffer);
    int posComma = POS_COMMA;

    char *pointer = text;

    if (param->Is(ParameterValue::Offset))
    {
        IntValue value(text, posComma);
        value.Sub5(text, &posComma);
        posComma--;
        SU::ConcatenateSymbol(buf, ' ');
        SU::ConcatenateSymbol(buf, text[0]);
        pointer++;
    }
    else
    {
        SU::ConcatenateSymbol(buf, SIGN);
    }

    for (int i = 0; i < param->numDigits; i++)
    {
        SU::ConcatenateSymbol(buf, pointer[i]);

        if (posComma == i)
        {
            SU::ConcatenateSymbol(buf, '.');
        }
    }

    return buf;
}


bool StructValue::IncreaseDigit(int num)
{
    if (num < 0 || num >= param->numDigits)
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
            if(!IncreaseDigit(DIGIT(num - 1) == '.' ? num - 2 : num - 1)) //-V557
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


bool StructValue::DecreaseDigit(int num)
{
    if (num < 0 || num >= param->numDigits)
    {
        return false;
    }

    if (All0LeftWithThis(num) && SIGN == ' ')
    {
        return false;
    }

    StructValue temp = *this; //-V821 //-V2551

    DIGIT(num)--;

    if (DIGIT(num) < '0')
    {
        DIGIT(num) = '9';
        DecreaseDigit(DIGIT(num - 1) == '.' ? num - 2 : num - 1); //-V557
    }

    if(ValueNano().ToFloat() < param->MinValue() || ValueNano().ToFloat() > param->MaxValue())
    {
        *this = temp;
        return false;
    }

    return true;
}


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


void StructValue::ShiftToRight()
{
    for (int i = param->numDigits - 2; i >= 0; i--)
    {
        DIGIT(i + 1) = DIGIT(i);
    }
    DIGIT(0) = '0';
    ++POS_COMMA;
}


void StructValue::ShiftToLeft()
{
    for (int i = 1; i < param->numDigits; i++)
    {
        DIGIT(i - 1) = DIGIT(i);
    }
    DIGIT(param->numDigits - 1) = '0';
    --POS_COMMA;
}


pString Order::Name() const
{
    static const pString names[Order::Count] =
    {
        "н",
        "мк",
        "м",
        "",
        "к",
        "М"
    };

    return names[value];
}


bool StructValue::OnlyOneRigthDigit()
{
    char digitLast = DIGIT(param->numDigits - 1);

    if (digitLast != '0' && All0LeftWithThis(param->numDigits - 2))
    {
        return true;
    }
    return false;
}


int StructValue::ValueBeforeComma()
{
    return static_cast<int>(ValueBeforeCommaInNano() / (1000 * 1000 * 1000));
}


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

    for (int i = POS_COMMA + 1; i < param->numDigits; i++)    // Проходим все знакоместа, начиная с того, что после запятой
    {
        result += (DIGIT(i) & 0x0f) * pow;
        pow /= 10;
    }

    return result;
}


void StructValue::IncreaseOrder()
{
    if (ORDER < Order::Count - 1)
    {
        ORDER = (Order)(Order::E)(ORDER + 1);

        POS_COMMA -= 3;
    }
}


FloatValue StructValue::ValueNano()
{
    uint64 beforeNS = ValueBeforeCommaInNano();     // Здесь число до запятой, в нано-единицах
    uint64 afterNS = ValueAfterCommaInNano();       // Здесь число после запятой, в нано-единицах

    FloatValue result(beforeNS + afterNS);          // Теперь здесь количество нано-единиц в предпоожении, что размерность - One

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
    else
    {
        // здесь ничего
    }

    if (SIGN == '-')
    {
        result.SetSign(-1);
    }

    return result;
}


float StructValue::Value()
{
    FloatValue value = ValueNano();

    return value.ToFloat();
}


void StructValue::SaveValue()
{
    SendToGenerator();

    Menu::ResetAdditionPage();
}


void StructValue::PressKey(const Key &key)
{
    if (key.value == Key::Dot)
    {
        POS_COMMA = CURRENT_POS - 1;

        return;
    }



    static const struct StrControl
    {
        Key::E control;
        char       symbol;
        uint8      nu[3];
    }
    command[] =
    {
        {Key::_1, '1'}, {Key::_2, '2'}, {Key::_3, '3'}, {Key::_4, '4'}, {Key::_5, '5'},
        {Key::_6, '6'}, {Key::_7, '7'}, {Key::_8, '8'}, {Key::_9, '9'}, {Key::_0, '0'},
        {Key::None, '.'}
    };

    int i = 0;
    while(command[i].control != Key::None)
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


void StructValue::SendToGenerator()
{
    if (param->Is(ParameterValue::Delay))
    {
    }
    else
    {
        PGenerator::SetParameter(param);
    }
}
