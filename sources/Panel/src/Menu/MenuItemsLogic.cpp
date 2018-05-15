#include "Menu/Menu.h"
#include "globals.h"
#include "Display/Display.h"
#include "Display/Painter.h"
#include "Display/Symbols.h"
#include "Hardware/Timer.h"
#include "Log.h"
#include "Settings/Settings.h"
#include "Hardware/CPU.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static pString nameParameter[NumParameters][2] =
{
    { "ЧАСТОТА",      "FREQUENCY" },
    { "ПЕРИОД",       "PERIOD" },
    { "АМПЛИТУДА",    "AMPLITUDE" },
    { "СМЕЩЕНИЕ",     "OFFSET" },
    { "ДЛИТЕЛЬНОСТЬ", "DURATION" },
    { "СКВАЖНОСТЬ",   "DUTY RATIO" },
    { "ФАЗА",         "PHASE" },
    { "ЗАДЕРЖКА",     "DELAY" }
};

typedef enum
{
    NONE,
    INCREASE,
    DECREASE
} DIRECTION;

/// Структура используется для анимации элементов ГУИ Choice и Governor
typedef struct
{
    void       *address;    ///< Адрес элемента. Если 0 - не движется
    uint        timeStart;  ///< Время начала анимации в миллисекундах
    DIRECTION   dir;        ///< Направление изменения значения
    uint8       notUsed0;
    uint8       notUsed1;
    uint8       notUsed2;
} TimeStruct;

static TimeStruct tsChoice = {0, 0, NONE, 0, 0, 0};
static TimeStruct tsGovernor = {0, 0, NONE, 0, 0, 0};

int8 gCurDigit = 0;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------------------
void Choice::StartChange(int delta)
{
    if (tsChoice.address != 0)
    {
        return;
    }
    if (HINT_MODE_ENABLED)
    {
        Menu::SetItemForHint(this);
    }
    else if (!IS_ACTIVE(this))
    {
        CHOICE_RUN_FUNC_CHANGED(this, false);
    }
    else
    {
        tsChoice.address = this;
        tsChoice.timeStart = TIME_MS;
        tsChoice.dir = delta > 0 ? INCREASE : DECREASE;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
float Choice::Step()
{
    static const float speed = 0.1f;
    static const int numLines = 12;
    if (tsChoice.address == this)
    {
        float delta = speed * (TIME_MS - tsChoice.timeStart);
        if (delta == 0.0f)
        {
            delta = 0.001f; // Таймер в несколько первых кадров может показать, что прошло 0 мс, но мы возвращаем большее число, потому что ноль будет говорить о том, что движения нет
        }
        int8 index = *cell;
        if (tsChoice.dir == INCREASE)
        {
            if (delta <= numLines)
            {
                return delta;
            }
            CircleIncrease<int8>(&index, 0, (int8)NumSubItems() - 1);
        }
        else if (tsChoice.dir == DECREASE)
        {
            delta = -delta;

            if (delta >= -numLines)
            {
                return delta;
            }
            CircleDecrease<int8>(&index, 0, (int8)NumSubItems() - 1);
        }
        *cell = index;
        tsChoice.address = 0;
        CHOICE_RUN_FUNC_CHANGED(this, IS_ACTIVE(this));
        NEED_FINISH_DRAW = 1;
        tsChoice.dir = NONE;
        return 0.0f;
    }
    return 0.0f;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Choice::Press(TypePress press)
{
    if(press == TypePress_Release)
    {
        CircleIncrease<int8>(cell, 0, (int8)(NumSubItems() - 1));
        if(funcOnChanged)
        {
            funcOnChanged(true);
        }
    }
    else if(press == TypePress_LongPress)
    {
        OPENED_ITEM = this;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Choice::ChangeIndex(int delta)
{
    int index = *cell;
    if (delta < 0)
    {
        ++index;
        if (index == NumSubItems())
        {
            index = 0;
        }
    }
    else
    {
        --index;
        if (index < 0)
        {
            index = NumSubItems() - 1;
        }
    }
    *cell = (int8)index;
    CHOICE_RUN_FUNC_CHANGED(this, IS_ACTIVE(this));
    NEED_FINISH_DRAW = 1;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Choice::NumSubItems()
{
    return num;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Governor::StartChange(int delta)
{
    if (delta > 0 && tsGovernor.address == this && tsGovernor.dir == INCREASE)
    {
        *cell = NextValue();
    }
    else if (delta < 0 && tsGovernor.address == this && tsGovernor.dir == DECREASE)
    {
        *cell = PrevValue();
    }
    else
    {
        tsGovernor.timeStart = TIME_MS;
        tsGovernor.address = this;
    }
    tsGovernor.dir = delta > 0 ? INCREASE : DECREASE;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int16 Governor::NextValue()
{
    return ((*cell) < maxValue) ? ((*cell) + 1) : minValue;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int16 Governor::PrevValue()
{
    return ((*cell) > minValue) ? ((*cell) - 1) : maxValue;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
float Governor::Step()
{
    static const float speed = 0.05f;
    static const int numLines = 10;
    float delta = 0.0f;
    if (tsGovernor.address == this)
    {
        delta = speed * (TIME_MS - tsGovernor.timeStart);
        if (tsGovernor.dir == DECREASE)
        {
            delta *= -1.0f;
            if (delta == 0.0f)
            {
                return -0.001f;
            }
            if (delta < -numLines)
            {
                tsGovernor.dir = NONE;
                *cell = PrevValue();
                if (funcOfChanged)
                {
                    funcOfChanged();
                }
                delta = 0.0f;
                tsGovernor.address = 0;
            }
        }
        else if (tsGovernor.dir == INCREASE)
        {
            if (delta == 0.0f)
            {
                return 0.001f;
            }
            if (delta > numLines)
            {
                tsGovernor.dir = NONE;
                *cell = NextValue();
                if (funcOfChanged)
                {
                    funcOfChanged();
                }
                delta = 0.0f;
                tsGovernor.address = 0;
            }
        }
    }
    return delta;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Governor::ChangeValue(int delta)
{
    int16 oldValue = *cell;
    *cell += (int16)(Sign(delta) * Pow10(gCurDigit));
    LIMITATION(*cell, minValue, maxValue);
    if (*cell != oldValue)
    {
        if (funcOfChanged)
        {
            funcOfChanged();
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Governor::NextPosition()
{
    if (Menu::OpenedItem() == this)
    {
        CircleIncrease<int8>(&gCurDigit, 0, (int8)(NumDigits() - 1));
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Governor::NumDigits()
{
    int min = SU::NumDigitsInNumber(Abs(minValue));
    int max = SU::NumDigitsInNumber(Abs(maxValue));
    if (min > max)
    {
        max = min;
    }
    return max;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
char Governor::GetSymbol(int value)
{
    static const char chars[] =
    {
        SYMBOL_GOVERNOR_SHIFT_0,
        SYMBOL_GOVERNOR_SHIFT_1,
        SYMBOL_GOVERNOR_SHIFT_2,
        SYMBOL_GOVERNOR_SHIFT_3
    };
    while (value < 0)
    {
        value += 4;
    }
    return chars[value % 4];
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
int Page::NumSubPages() const
{
    return (NumItems() - 1) / MENU_ITEMS_ON_DISPLAY + 1;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Page::NumItems() const
{
    if(PAGE_IS_MAIN)
    {
        return SHOW_DEBUG_MENU ? num : (num - 1);
    }
    return num;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
NamePage Page::GetNamePage() const
{
    return name;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Time::SetOpened()
{
    PackedTime time = RTC_GET_PACKED_TIME();
    *(seconds) = (int8)time.seconds;
    *(minutes) = (int8)time.minutes;
    *(hours) = (int8)time.hours;
    *(day) = (int8)time.day;
    *(month) = (int8)time.month;
    *(year) = (int8)time.year;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Time::IncCurrentPosition()
{
    int8 *value[] = { 0, day, month, year, hours, minutes, seconds };
    int8 position = *curField;
    if (position != iSET && position != iEXIT)
    {
        static const int8 max[] = { 0, 31, 12, 99, 23, 59, 59 };
        static const int8 min[] = { 0, 1, 1, 15, 0, 0, 0 };
        *(value[position]) = (*(value[position]))++;
        if (*value[position] > max[position])
        {
            *value[position] = min[position];
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Time::SetNewTime()
{
    RTC_SET_TIME_AND_DATA(*day, *month, *year, *hours, *minutes, *seconds);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Time::SelectNextPosition()
{
    CircleIncrease<int8>(curField, 0, 7);
    Painter::ResetFlash();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void MACaddress::ChangeValue(int delta)
{
    uint8 *value = mac0 + gCurDigit;
    *value += delta > 0 ? 1 : -1;
    DISPLAY_SHOW_WARNING(NeedRebootDevice);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Time::DecCurrentPosition()
{
    static const int8 max[] = {0, 31, 12, 99, 23, 59, 59};
    static const int8 min[] = {0, 1, 1, 15, 0, 0, 0};
    int8 *value[] = {0, day, month, year, hours, minutes, seconds};
    int8 position = *curField;
    if (position != iSET && position != iEXIT)
    {
        (*(value[position]))--;
        if (*value[position] < min[position])
        {
            *value[position] = max[position];
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void GovernorColor::ChangeValue(int delta)
{
    if (ct->currentField == 0)
    {
        ct->BrightnessChange(delta);
    }
    else
    {
        ct->ComponentChange(delta);
    }

    Color::InitGlobalColors();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void IPaddress::NextPosition()
{
    CircleIncrease<int8>(&gCurDigit, 0, port == 0 ? 11 : 16);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void IPaddress::ChangeValue(int delta)
{
    int numByte = 0;
    int numPos = 0;

    GetNumPosIPvalue(&numByte, &numPos);

    int oldValue = 0;

    if (numByte < 4)
    {
        uint8 *bytes = ip0;
        oldValue = bytes[numByte];
    }
    else
    {
        oldValue = *port;
    }

    int newValue = oldValue + Sign(delta) * Pow10(numPos);
    LIMITATION(newValue, 0, numByte == 4 ? 65535 : 255);

    if (oldValue != newValue)
    {
        if (numByte == 4)
        {
            *port = (uint16)newValue;
        }
        else
        {
            ip0[numByte] = (uint8)newValue;
        }
        DISPLAY_SHOW_WARNING(NeedRebootDevice);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void IPaddress::GetNumPosIPvalue(int *numIP, int *selPos)
{
    if (gCurDigit < 12)
    {
        *numIP = gCurDigit / 3;
        *selPos = 2 - (gCurDigit - (*numIP * 3));
    }
    else
    {
        *numIP = 4;
        *selPos = 4 - (gCurDigit - 12);
    }


}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Control::Press(TypePress press)
{
    if(type == Item_Choice)
    {
        ((Choice *)this)->Press(press);
    }
    else if(type == Item_Button)
    {
        ((Button *)this)->Press();
    }
    else if(type == Item_ChoiceParameter)
    {
        ((ChoiceParameter *)this)->Press(press);
    }
    else if(type == Item_SmallButton)
    {
        ((SButton *)this)->Press();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
TypeItem Control::Type()
{
    return type;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Choice::CurrentChoice()
{
    int retValue = 0;

    if (type == Item_Choice)
    {
        retValue = *cell;
    }
    else if (type == Item_ChoiceParameter)
    {
        ChoiceParameter *param = (ChoiceParameter *)this;

        for (int i = 0; i < NumParameters; i++)
        {
            if ((param->allowParameters).allow[i] == false)
            {
                continue;
            }
            if (i == *param->numParameter)
            {
                break;
            }
            retValue++;
        }
    }

    return retValue;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Control::PositionOnPage()
{
    for (int numPage = 0; numPage < NUM_PAGES; numPage++)
    {
        for (int numItem = 0; numItem < NUM_ITEMS_ON_PAGE; numItem++)
        {
            if (this == menu[numPage]->items[numItem])
            {
                return numItem;
            }
        }
    }
    return -1;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
PanelControl Control::ButtonForItem()
{
    int pos = PositionOnPage();

    if (pos >= 0)
    {
        return (PanelControl)((int)B_F1 + pos);
    }

    return B_None;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Control::Rotate(PanelControl control)
{
    if (type == Item_Choice)
    {
        Choice *choice = (Choice *)this;

        if (control == REG_LEFT)
        {
            CircleIncrease<int8>(choice->cell, 0, (int8)(choice->NumSubItems() - 1));
        }
        else if (control == REG_RIGHT)
        {
            if (*(choice->cell) == 0)
            {
                *(choice->cell) = (int8)(choice->NumSubItems() - 1);
            }
            else
            {
                --(*(choice->cell));
            }
        }
        if (choice->funcOnChanged)
        {
            choice->funcOnChanged(true);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void ChoiceParameter::Press(TypePress press)
{
    if (press == TypePress_Release)
    {
        volatile bool allow = false;
        do
        {
            CircleIncrease<uint8>(numParameter, 0, (uint8)(NumParameters - 1));
            allow = (allowParameters).allow[*numParameter];
        } while (!allow);
    }
    else if (press == TypePress_LongPress)
    {
        OPENED_ITEM = this;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void SButton::Press()
{
    if (funcOnPress)
    {
        funcOnPress();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Button::Press()
{
    if (funcOnPress)
    {
        funcOnPress();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
pString ChoiceParameter::NameSubItem(int number)
{
    const char * retValue = 0;
    for (int i = 0; i < NumParameters; i++)
    {
        if ((allowParameters).allow[i] == false)
        {
            continue;
        }
        if (number == 0)
        {
            retValue = (char*)nameParameter[i][LANGUAGE];
        }
        --number;
    }
    return retValue;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
const char* Parameter_Name(WaveParameter parameter)
{
    return (char*)nameParameter[parameter][LANG];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
pString ChoiceParameter::CurrentName()
{
    return (const char*)(nameParameter[*numParameter][LANG]);
}
