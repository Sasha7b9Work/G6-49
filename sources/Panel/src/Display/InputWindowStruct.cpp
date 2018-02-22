#include "InputWindowStruct.h"
#include "Log.h"
#include "Display/Painter.h"
#include "Generator/Generator.h"
#include "Settings/SettingsSignals.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CURRENT_POS         (iws->hightLightDigit)
#define CURRENT_DIGIT       (iws->inputBuffer[CURRENT_POS])
#define DIGIT(num)          (iws->inputBuffer[num])
#define POS_COMMA           (iws->posComma)

#define IN_NUM_LOCK_MODE    (iws->numLockMode)


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void IncreaseDigit(int num);
static void DecreaseDigit(int num);
/// Возвращает true, если все цифры слева от num ранвы нулю. И num тоже
static bool All0LeftWithThis(int num);
/// Возвращает true, елси все цифры слева и эта являются девятками
static bool All9LeftWithThis(int num);
/// Возвращает true, если среди цифр единственная единица и она находится в даной позиции
static bool Only1InThis(int num);
/// Сдвигает все разряды вправо
static void ShiftToRight();
/// Сдвиг всех разрядов влево
static void ShiftToLeft();
/// Возвращает true, если есть только одна крайняя справа цифра
static bool OnlyOneRigthDigit();
/// Возвращает число до запятой
static int ValueBeforeComma(InputWindowStruct *iws);
/// Возвращает число после запятой
static float ValueAfterComma(InputWindowStruct *iws);
/// Переключает порядок на следующий по возрастанию
static void IncreaseOrder();
/// Восстанавливает ранее сохранённое значение
static void RestoreValue();
/// Заполняет iws из inputBuffer
static void FillIWSfromInputBuffer();
/// Заслать текущее значение в генератор
static void SendIWStoGenerator();

static InputWindowStruct *iws = 0;
static WaveForm form;
static WaveParameter m_param;
static Channel ch;

#define SIZE_INPUT_BUFFER_IWS 17
static char m_inputBuffer[SIZE_INPUT_BUFFER_IWS];


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void InputWindowStruct::Fill(Channel ch_, WaveForm form_, WaveParameter param_)
{
    ch = ch_;
    form = form_;
    m_param = param_;
    iws = this;

    IN_NUM_LOCK_MODE = false;

    memset(m_inputBuffer, 0, SIZE_INPUT_BUFFER_IWS);

	for (int i = 0; i < NUM_DIGITS; i++)
	{
		iws->inputBuffer[i] = PARAMETER_DIG(ch, form, m_param, i);
	}
	for (int i = NUM_DIGITS - 1; i > 0; --i)
	{
		if (iws->inputBuffer[i] == 0)
		{
			iws->inputBuffer[i] = '0';
		}
		else
		{
			break;
		}
	}

	CURRENT_POS = DIGIT(NUM_DIGITS - 1) == '.' ? NUM_DIGITS - 2 : NUM_DIGITS - 1;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindowStruct::KeyLeft(void)
{
	if (CURRENT_POS > 0)
	{
		if (CURRENT_POS == 1 && DIGIT(0) == '.')
		{
			return;
		}
		--CURRENT_POS;
		if (CURRENT_DIGIT == '.')
		{
            InputWindowStruct::KeyLeft();
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

//----------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindowStruct::KeyRight()
{
	if (CURRENT_POS < NUM_DIGITS - 1)
	{
		if (CURRENT_POS == NUM_DIGITS - 2 && DIGIT(NUM_DIGITS - 1) == '.')
		{
			return;
		}
		++CURRENT_POS;
		if (CURRENT_DIGIT == '.')
		{
            InputWindowStruct::KeyRight();
		}
	}
    else if(DIGIT(0) == '0')
    {
        ShiftToLeft();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindowStruct::RegLeft()
{
    DecreaseDigit(CURRENT_POS);

    if (set.sig_tuneFull)
    {
        SendIWStoGenerator();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindowStruct::RegRight(void)
{

    // Сохраняем значение
    for (int i = 0; i < NUM_DIGITS; i++)
    {
        iws->prevBuffer[i] = DIGIT(i);
    }
    iws->prevPosComma = iws->posComma;

    IncreaseDigit(CURRENT_POS);

    if (iws->Value() > MaxValue(iws->param))
    {
        RestoreValue();
    }
    else
    {
        if (ValueBeforeComma(iws) > 999)
        {
            IncreaseOrder();
        }
    }

    if (set.sig_tuneFull)
    {
        SendIWStoGenerator();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
char *InputWindowStruct::StringValue()
{
    static char buffer[20];
    buffer[0] = '\0';

    for (int i = 0; i < NUM_DIGITS; i++)
    {
        char str[2] = {0, 0};
        str[0] = inputBuffer[i];
        strcat(buffer, str);
        if (iws->posComma == i)
        {
            str[0] = '.';
            strcat(buffer, str);
        }
    }

    return buffer;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void IncreaseDigit(int num)
{
    if (num < 0 || num >= NUM_DIGITS)
    {
        return;
    }

    if (All9LeftWithThis(num))
    {
        ShiftToRight();
        ++CURRENT_POS;
        IncreaseDigit(CURRENT_POS);
    }
    else
    {
        DIGIT(num)++;
        if (DIGIT(num) > '9')
        {
            DIGIT(num) = '0';
            IncreaseDigit(DIGIT(num - 1) == '.' ? num - 2 : num - 1);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DecreaseDigit(int num)
{
    if (num < 0 || num >= NUM_DIGITS)
    {
        return;
    }

    if (All0LeftWithThis(num))
    {
        return;
    }

    if (Only1InThis(num))
    {
        return;
    }

    DIGIT(num)--;
    if (DIGIT(num) < '0')
    {
        DIGIT(num) = '9';
        DecreaseDigit(DIGIT(num - 1) == '.' ? num - 2 : num - 1);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool All0LeftWithThis(int num)
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
static bool All9LeftWithThis(int num)
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
static bool Only1InThis(int num)
{
    if (DIGIT(num) != '1')
    {
        return false;
    }

    for (int i = 0; i < num; i++)
    {
        if (DIGIT(i) != '0')
        {
            return false;
        }
    }

    for (int i = num + 1; i < NUM_DIGITS; i++)
    {
        if (DIGIT(i) != '0')
        {
            return false;
        }
    }

    return true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void ShiftToRight(void)
{
    for (int i = NUM_DIGITS - 2; i >= 0; i--)
    {
        DIGIT(i + 1) = DIGIT(i);
    }
    DIGIT(0) = '0';
    ++POS_COMMA;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void ShiftToLeft(void)
{
    for (int i = 1; i < NUM_DIGITS; i++)
    {
        DIGIT(i - 1) = DIGIT(i);
    }
    DIGIT(NUM_DIGITS - 1) = '0';
    --POS_COMMA;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
const char *NameOrder(Order order)
{
    static const char *names[NumOrders][2] =
    {
        {"н",   "n"},
        {"мк",  "u"},
        {"м",   "m"},
        {"",    ""},
        {"к",   "k"},
        {"М",   "M"}
    };

    return names[order][LANGUAGE];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
const char *NameUnit(char buffer[10], Order order, WaveParameter parameter)
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

    sprintf(buffer, "%s%s", NameOrder(order), names[parameter][LANGUAGE]);

    return buffer;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool OnlyOneRigthDigit(void)
{
    char digitLast = DIGIT(NUM_DIGITS - 1);

    if (digitLast != '0' && All0LeftWithThis(NUM_DIGITS - 2))
    {
        return true;
    }
    return false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static int ValueBeforeComma(InputWindowStruct *iws_)
{
    int lowPos = iws_->posComma;     // Младший байт числа

    int retValue = 0;

    int pow = 1;

    for (int i = lowPos; i >= 0; i--)
    {
        retValue += (0x0f & iws_->inputBuffer[i]) * pow;
        pow *= 10;
    }

    return retValue;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static float ValueAfterComma(InputWindowStruct *iws_)
{
    int retValue = 0;
    int pow = 1;
    for (int i = NUM_DIGITS - 1; i > iws_->posComma; i--)
    {
        uint8 digit = iws_->inputBuffer[i];
        digit &= 0x0f;
        retValue += digit * pow;
        pow *= 10;
    }

    return 1.0f / pow * retValue;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void IncreaseOrder(void)
{
    if (iws->order < NumOrders - 1)
    {
        ++iws->order;
        POS_COMMA -= 3;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
float InputWindowStruct::Value()
{
    float value = ValueBeforeComma(this) + ValueAfterComma(this);

    if (order == Nano)
    {
        return value * 1e-9f;
    }
    if (order == Micro)
    {
        return value * 1e-6f;
    }
    if (order == Milli)
    {
        return value * 1e-3f;
    }
    if (order == Kilo)
    {
        return value * 1e3f;
    }
    if (order == Mega)
    {
        return value * 1e6f;
    }

    return value;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindowStruct::SaveValue(void)
{
    if (IN_NUM_LOCK_MODE)
    {
        IN_NUM_LOCK_MODE = false;

        FillIWSfromInputBuffer();
    }

    SendIWStoGenerator();

    ADDITION_PAGE = 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void RestoreValue(void)
{
    for (int i = 0; i < NUM_DIGITS; i++)
    {
        DIGIT(i) = iws->prevBuffer[i];
    }
    iws->posComma = iws->prevPosComma;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindowStruct::PressKey(Control key)
{
    typedef struct
    {
        Control control;
        char    symbol;
    } StructControl;

    static const StructControl command[] =
    {
        {B_1, '1'}, {B_2, '2'}, {B_3, '3'}, {B_4, '4'}, {B_5, '5'},
        {B_6, '6'}, {B_7, '7'}, {B_8, '8'}, {B_9, '9'}, {B_0, '0'}, {B_Dot, '.'},
        {Control_None, '.'}
    };

    if (!IN_NUM_LOCK_MODE)
    {
        IN_NUM_LOCK_MODE = true;
        m_inputBuffer[0] = 0;
    }

    if (strlen(m_inputBuffer) < SIZE_INPUT_BUFFER_IWS - 1)
    {
        int i = 0;
        while (command[i].control != Control_None)
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
void InputWindowStruct::DrawInputField(int x, int y)
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
        x = Painter::DrawBigChar(x, y, 3, m_inputBuffer[i]);
        x += 2;
        ++i;
    }

    Painter::FillRegion(270, 30, 45, 100, Color::BACK);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindowStruct::FillAllowParameters(Channel ch_, WaveForm form_, AllowableParameters *allowParameters)
{
    for (int i = 0; i < NumParameters; i++)
    {
        allowParameters->allow[i] = INPUT_WINDOW_STRUCT(ch_, form_, i).allow;
    }
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
static void SendIWStoGenerator(void)
{
    PARAMETER(ch, form, m_param) = *iws;

    if (m_param == Delay)
    {
        PARAMETER(B, Form_Impulse, Frequency) = PARAMETER(B, Form_Impulse, Frequency);
        float frequency = PARAMETER(A, Form_Impulse, Frequency).Value();
        Generator::SetParameter(B, Frequency, frequency);

        float value = PARAMETER(ch, form, m_param).Value();
        Generator::SetParameter(ch, m_param, value);
    }
    else
    {
        float value = PARAMETER(ch, form, m_param).Value();
        Generator::SetParameter(ch, m_param, value);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void FillIWSfromInputBuffer(void)
{
    if (m_param == Duration || m_param == Delay)
    {
        iws->order = Micro;
    }
    else
    {
        iws->order = One;
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

            --iws->order;
        }
    }

    if (iws->sign != Sign_None)
    {
        iws->sign = (atof(m_inputBuffer) >= 0.0) ? Sign_Plus : Sign_Minus;
    }

    iws->hightLightDigit = NUM_DIGITS - 1;

    while ((int)fabs(atof(m_inputBuffer)) > 999)    // Пока целая часть числа в inputBuffer > 999
    {
        // Сдвигаем запятую на три места влево
        int pos = SU::FindSymbol(m_inputBuffer, '.');
        
        for (int i = pos; i > pos - 3; i--)         // Сдвигаем три символа слева от точки на одну позицию вправо
        {
            m_inputBuffer[i] = m_inputBuffer[i - 1];
        }

        m_inputBuffer[pos - 3] = '.';               // И ставим точку слева от этой тройки

        ++iws->order;                               // И увеличиваем степень на три порядка
    }

    // В этой точке целая часть числа уже не превышает 999

    float value = (float)fabs(atof(m_inputBuffer));

    int intValue = (int)value;

    // Заносим целую часть числа в буфер
    sprintf(iws->inputBuffer, "%d", intValue);

    iws->posComma = (int)strlen(iws->inputBuffer) - 1;

    int numDigits = NUM_DIGITS - (int)strlen(iws->inputBuffer);      // Столько цифр нужно записать после запятой

    int pos = SU::FindSymbol(m_inputBuffer, '.');       // Находим позицию точки в исходной строке. Символы после неё нужно писать в iws->inputBuffer

    for (int i = 0; i < numDigits; i++)
    {
        iws->inputBuffer[iws->posComma + 1 + i] = m_inputBuffer[pos + 1 + i];
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Order& operator++(Order& order)
{
    int value = (int)order + 1;
    order = (Order)value;
    return order;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Order& operator--(Order& order)
{
    int value = (int)order - 1;
    order = (Order)value;
    return order;
}
