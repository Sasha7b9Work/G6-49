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
#include <string.h>


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
    const void *address;    ///< Адрес элемента. Если 0 - не движется
    uint        timeStart;  ///< Время начала анимации в миллисекундах
    DIRECTION   dir;        ///< Направление изменения значения
    uint8       notUsed0;
    uint8       notUsed1;
    uint8       notUsed2;
} TimeStruct;

static TimeStruct tsChoice = {0, 0, NONE, 0, 0, 0};
static TimeStruct tsGovernor = {0, 0, NONE, 0, 0, 0};

int8 gCurDigit = 0;


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Choice::StartChange(int delta) const
{
    if (tsChoice.address != 0)
    {
        return;
    }
    if (!IS_ACTIVE(this))
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
    if(isPageSB)                                            // Для Choice это означает, что вариантов выбора всего два
    {
        static const float speed = 0.3f;
        static const int numLines = 60;
        if (tsChoice.address == this)
        {
            float delta = speed * (TIME_MS - tsChoice.timeStart);
            if (delta == 0.0f)
            {
                delta = 0.001f; // Таймер в несколько первых кадров может показать, что прошло 0 мс, но мы возвращаем большее число, потому что ноль будет говорить о том, что движения нет
            }

            int8 index = CurrentChoice();

            if (tsChoice.dir == INCREASE)
            {
                if (delta <= numLines)
                {
                    return delta;
                }
//                CircleIncrease<int8>(&index, 0, (int8)NumSubItems() - 1);
                index ^= 1;
            }
            else if (tsChoice.dir == DECREASE)
            {
                delta = -delta;

                if (delta >= -numLines)
                {
                    return delta;
                }
//                CircleDecrease<int8>(&index, 0, (int8)NumSubItems() - 1);
                index ^= 1;
            }

            if(index == 0)
            {
                _CLEAR_BIT(*cell_, (int)nameOrNumBit);
            }
            else
            {
                _SET_BIT(*cell_, (int)nameOrNumBit);
            }

            //_SET_BIT_VALUE(*cell_, (int)nameOrNumBit, index & 0x01);

            tsChoice.address = 0;
            CHOICE_RUN_FUNC_CHANGED(this, IS_ACTIVE(this));
            tsChoice.dir = NONE;
            return 0.0f;
        }
    }
    else                                                    // А здесь обычный Choice - со множеством вариантов выбора
    {
        static const float speed = 0.3f;
        static const int numLines = 60;
        if (tsChoice.address == this)
        {
            float delta = speed * (TIME_MS - tsChoice.timeStart);
            if (delta == 0.0f)
            {
                delta = 0.001f; // Таймер в несколько первых кадров может показать, что прошло 0 мс, но мы возвращаем большее число, потому что ноль будет говорить о том, что движения нет
            }
            int8 index = CurrentChoice();
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
            *cell_ = index;
            tsChoice.address = 0;
            CHOICE_RUN_FUNC_CHANGED(this, IS_ACTIVE(this));
            tsChoice.dir = NONE;
            return 0.0f;
        }
    }
    return 0.0f;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Control *Choice::Press(StructControl strControl)
{
    TypePress press = strControl.typePress;
    PanelControl key = strControl.key;

    if(key == B_LEFT && press == Up || key == REG_RIGHT)
    {
        StartChange(-1);
    }
    else if(key == B_RIGHT && press == Up || key == REG_LEFT || press == Up)
    {
        StartChange(1);
    }
    else if(press == Long)
    {
        return this;
    }

    return Menu::OpenedItem();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Choice::ChangeIndex(int delta)
{
    if(isPageSB)
    {
        *cell_ ^= (1 << nameOrNumBit);
    }
    else
    {
        int index = *cell_;
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
        *cell_ = (int8)index;
    }
    CHOICE_RUN_FUNC_CHANGED(this, IS_ACTIVE(this));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Choice::NumSubItems() const
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
int16 Governor::NextValue() const
{
    return ((*cell) < maxValue) ? ((*cell) + 1) : minValue;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int16 Governor::PrevValue() const
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
int Governor::NumDigits() const
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
    return num;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
NamePage Page::GetNamePage() const
{
    return nameOrNumBit;
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
Control *Control::Press(StructControl strControl)
{
    TypePress press = strControl.typePress;
    PanelControl key = strControl.key;

    if(press == Long)
    {
        if (IsOpened() || key == REG_BTN || key == B_ESC)
        {
            return 0;
        }
    }

    if(key == B_ESC && (press == Up || press == Long))
    {
        return 0;
    }

    if(IsShade())
    {
        return Menu::OpenedItem();
    }

    Menu::itemUnderKey = (press == Down) && !IsOpened() ? this : 0;

    if (type == Control_Choice)
    {
        return ((Choice *)this)->Press(strControl);
    }
    else if (type == Control_Button)
    {
        return ((Button *)this)->Press(press);
    }
    else if (type == Control_ChoiceParameter)
    {
        return ((ChoiceParameter *)this)->Press(press);
    }
    else if (type == Control_SmallButton)
    {
        return ((SButton *)this)->Press(press);
    }

    return 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
TypeControl Control::Type() const
{
    return type;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int8 Choice::CurrentChoice() const
{
    int8 retValue = 0;

    if (type == Control_Choice)
    {
        if(isPageSB)
        {
            retValue = (*cell_ >> nameOrNumBit) & 0x01;
        }
        else
        {
            retValue = *cell_;
        }
    }
    else if (type == Control_ChoiceParameter)
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
int Control::PositionOnPage() const
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
PanelControl Control::ButtonForItem() const
{
    int pos = PositionOnPage();

    if (pos >= 0)
    {
        return (PanelControl)((int)B_F1 + pos);
    }

    return B_None;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Control *ChoiceParameter::Press(TypePress press)
{
    if (press == Up)
    {
        volatile bool allow = false;
        do
        {
            CircleIncrease<uint8>(numParameter, 0, (uint8)(NumParameters - 1));
            allow = (allowParameters).allow[*numParameter];
        } while (!allow);
    }
    else if (press == Long)
    {
        return this;
    }

    return 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Control *SButton::Press(TypePress press)
{
    if(press == Up)
    {
        if (funcOnPress)
        {
            funcOnPress();
        }
    }

    return 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Control *Button::Press(TypePress press)
{
    if (press == Up && funcOnPress)
    {
        funcOnPress();
    }

    return 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
pString ChoiceParameter::NameSubItem(int number) const
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
            retValue = (char*)nameParameter[i][LANG];
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
pString ChoiceParameter::NameCurrentSubItem() const
{
    return (const char*)(nameParameter[*numParameter][LANG]);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
char *Control::FullPath() const
{
    const PageBase *parent = keeper;

    const char *titles[5] = {Title()};
    int pointer = 1;

    while(parent)
    {
        titles[pointer++] = parent->titleHint[LANG];
        parent = parent->keeper;
    }

    static char buffer[100];
    buffer[0] = 0;

    do
    {
        pointer--;
        strcat(buffer, titles[pointer]);
        strcat(buffer, " - ");
        
    } while(pointer > 0);

    buffer[strlen(buffer) - 3] = 0;

    return buffer;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Choice::GetHeightOpened() const 
{
    return NumSubItems() * 10 + 2 + MI_TITLE_HEIGHT;
}
