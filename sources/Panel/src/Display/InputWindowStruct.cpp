#include "Log.h"
#include "globals.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Generator/Generator.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include "Settings/Settings.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CURRENT_POS         (param->hightLightDigit)
#define DIGIT(num)          (param->buffer[num])
#define CURRENT_DIGIT       (param->buffer[CURRENT_POS])
#define POS_COMMA           (param->posComma)
#define IN_NUM_LOCK_MODE    (param->InNumLockMode())


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SIZE_INPUT_BUFFER_IWS 17
static char m_inputBuffer[SIZE_INPUT_BUFFER_IWS];


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void StructValue::Set(Parameter *param_)
{
    param = param_;

    memset(m_inputBuffer, 0, SIZE_INPUT_BUFFER_IWS);

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

	param->hightLightDigit = param->buffer[NUM_DIGITS - 1] == '.' ? NUM_DIGITS - 2 : NUM_DIGITS - 1;
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
            StructValue::KeyLeft();
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
            StructValue::KeyRight();
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
    static const char *names[Order::Number][2] =
    {
        {"н",   "n"},
        {"мк",  "u"},
        {"м",   "m"},
        {"",    ""},
        {"к",   "k"},
        {"М",   "M"}
    };

    return names[value][LANG];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
const char *NameUnit(char buffer[10], Order order, Parameter parameter)
{
    static const char *names[][2] =
    {
        {"Гц",  "Hz"},
        {"с",   "s"},
        {"В",   "V"},
        {"В",   "V"},
        {"с",  "s"},
        {"",    ""},
        {"o",  "o"},
        {"с",  "s"}
    };

    sprintf(buffer, "%s%s", order.Name(), names[parameter.value][LANG]);

    return buffer;
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
    int lowPos = param->posComma;     // Младший байт числа

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
    if (IN_NUM_LOCK_MODE)
    {
        param->SetNumLockMode(false);

        FillFromInputBuffer();
    }

    SendToGenerator();

    ADDITION_PAGE = 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void StructValue::PressKey(Control key)
{
    struct StrControl
    {
        Control control;
        char    symbol;
    };

    static const StrControl command[] =
    {
        {Control::B_1, '1'}, {Control::B_2, '2'}, {Control::B_3, '3'}, {Control::B_4, '4'}, {Control::B_5, '5'},
        {Control::B_6, '6'}, {Control::B_7, '7'}, {Control::B_8, '8'}, {Control::B_9, '9'}, {Control::B_0, '0'}, {Control::B_Dot, '.'},
        {Control::B_None, '.'}
    };

    if (!IN_NUM_LOCK_MODE)
    {
        param->SetNumLockMode(true);

        m_inputBuffer[0] = 0;
    }

    if (strlen(m_inputBuffer) < SIZE_INPUT_BUFFER_IWS - 1)
    {
        int i = 0;
        while (command[i].control != Control::B_None)
        {
            if (command[i].control == key)
            {
                int length = (int)strlen(m_inputBuffer);
                m_inputBuffer[length] = command[i].symbol;
                m_inputBuffer[length + 1] = 0;
                break;
            }
            i++;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void StructValue::DrawInputField(int x, int y)
{
    int width = 230;
    int height = 60;

    Painter::FillRegion(x, y, width, height, Color::BACK);
    Painter::DrawRectangle(x, y, width, height, Color::FILL);

    x += 8;
    y += 19;

    int i = 0;

    while (m_inputBuffer[i])
    {
        x = Text::DrawBigChar(x, y, 3, m_inputBuffer[i]);
        x += 2;
        ++i;
    }

    Painter::FillRegion(270, 30, 45, 100, Color::BACK);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void StructValue::SendToGenerator()
{
    if (param->Is(Parameter::Delay))
    {
    }
    else
    {
        Generator::SetParameter(CURRENT_CHANNEL, *param, StructValue(param).Value());
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void StructValue::FillFromInputBuffer()
{
    if (param->Is(Parameter::Duration) || param->Is(Parameter::Delay))
    {
        param->order = Order::Micro;
    }
    else
    {
        param->order = Order::One;
    }

    if (SU::FindSymbol(m_inputBuffer, '.') == -1)             // Если точки нету
    {
        m_inputBuffer[strlen(m_inputBuffer)] = '.';         // То ставим её вместо завершающего нуля
        m_inputBuffer[strlen(m_inputBuffer) + 1] = 0;       // и перемещаем нуль вправо
    }
    else
    {
        while ((int)fabs(atof(m_inputBuffer)) == 0)     // Пока целая часть числа в inputBuffer == 0
        {
            // Сдвигаем запятую на три места вправо
            int pos = SU::FindSymbol(m_inputBuffer, '.');

            for (int i = pos; i < pos + 3; i++)
            {
                m_inputBuffer[i] = m_inputBuffer[i + 1];
                if(m_inputBuffer[i] == 0)
                {
                    m_inputBuffer[i] = '0';
                }
            }
            m_inputBuffer[pos + 3] = '.';
            if(m_inputBuffer[pos + 4] == 0)
            {
                m_inputBuffer[pos + 4] = '0';
            }

            param->order = (Order::E)(param->order - 1);
        }
    }

    if (param->sign != ' ')
    {
        param->sign = (atof(m_inputBuffer) >= 0.0) ? '+' : '-';
    }

    param->hightLightDigit = NUM_DIGITS - 1;

    while ((int)fabs(atof(m_inputBuffer)) > 999)    // Пока целая часть числа в inputBuffer > 999
    {
        // Сдвигаем запятую на три места влево
        int pos = SU::FindSymbol(m_inputBuffer, '.');
        
        for (int i = pos; i > pos - 3; i--)         // Сдвигаем три символа слева от точки на одну позицию вправо
        {
            m_inputBuffer[i] = m_inputBuffer[i - 1];
        }

        m_inputBuffer[pos - 3] = '.';               // И ставим точку слева от этой тройки

        param->order = (Order::E)(param->order + 1); // И увеличиваем степень на три порядка
    }

    // В этой точке целая часть числа уже не превышает 999

    float value = (float)fabs(atof(m_inputBuffer));

    int intValue = (int)value;

    // Заносим целую часть числа в буфер
    sprintf(param->buffer, "%d", intValue);

    param->posComma = (int8)strlen(param->buffer) - 1;

    int numDigits = NUM_DIGITS - (int)strlen(param->buffer);      // Столько цифр нужно записать после запятой

    int pos = SU::FindSymbol(m_inputBuffer, '.');       // Находим позицию точки в исходной строке. Символы после неё нужно писать в iws->inputBuffer

    for (int i = 0; i < numDigits; i++)
    {
        param->buffer[param->posComma + 1 + i] = m_inputBuffer[pos + 1 + i];
    }
}
