#include "Menu/Menu.h"
#include "Display/Display.h"
#include "Display/Painter.h"
#include "Display/Symbols.h"
#include "Hardware/Timer.h"
#include "Log.h"
#include "Settings/Settings.h"
#include "Hardware/CPU.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include "Wave.h"
#include <string.h>


#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4514 4626 5027)
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum DIRECTION
{
    NONE,
    INCREASE,
    DECREASE
};

/// Структура используется для анимации элементов ГУИ Choice и Governor
struct TimeStruct
{
    const void *address;    ///< Адрес элемента. Если 0 - не движется
    uint        timeStart;  ///< Время начала анимации в миллисекундах
    DIRECTION   dir;        ///< Направление изменения значения
    uint8       notUsed0;
    uint8       notUsed1;
    uint8       notUsed2;
};

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
    static const float speed = 0.3f;
    static const int numLines = 60;
    if (tsChoice.address == this)
    {
        float delta = speed * (TIME_MS - tsChoice.timeStart);
        if (delta == 0.0f)
        {
            delta = 0.001f; // Таймер в несколько первых кадров может показать, что прошло 0 мс, но мы возвращаем большее число, потому что ноль будет говорить о том, что движения нет
        }
        int8 index = CurrentIndex();
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

        if(isPageSB)
        {
            uint *address = (uint *)cell;
            *address ^= (1 << (int)nameOrNumBit);
        }
        else
        {
            *cell = index;
        }

        tsChoice.address = 0;
        CHOICE_RUN_FUNC_CHANGED(this, IS_ACTIVE(this));
        tsChoice.dir = NONE;
        return 0.0f;
    }
    return 0.0f;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Item *Choice::Press(Control key)
{
    if(key.Is(Control::Left) && key.action.Is(Control::Action::Up) || key.Is(Control::Reg::Right))
    {
        StartChange(-1);
    }
    else if(key.Is(Control::Right) && key.action.Is(Control::Action::Up) || key.Is(Control::Reg::Left) || key.action.Is(Control::Action::Up))
    {
        StartChange(1);
    }
    else if(key.action.Is(Control::Action::Long))
    {
        return this;
    }

    return Menu::GetOpenedItem();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Choice::ChangeIndex(int delta)
{
    if(isPageSB)
    {
        uint *address = (uint *)cell;
        *address ^= (1 << nameOrNumBit);
    }
    else
    {
        int8 index = *cell;
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
    }
    CHOICE_RUN_FUNC_CHANGED(this, IS_ACTIVE(this));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int8 Choice::NumSubItems() const
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
    if (Menu::GetOpenedItem() == this)
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
    return (NumItems() - 1) / Menu::NUM_ITEMS_ON_DISPLAY + 1;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Page::NumItems() const
{
    return num;
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
Item *Item::Press(Control key)
{
    if(key.action.Is(Control::Action::Long))
    {
        if (IsOpened() || key.Is(Control::Reg::Button) || key.Is(Control::Esc))
        {
            return 0;
        }
    }

    if(key.Is(Control::Esc) && (key.action.Is(Control::Action::Up) || key.action.Is(Control::Action::Long)))
    {
        return 0;
    }

    if(IsShade())
    {
        return Menu::GetOpenedItem();
    }

    Menu::itemUnderKey = (key.action.Is(Control::Action::Down)) && !IsOpened() ? this : 0;

    if (type == Item::Type::Choice)
    {
        return ((Choice *)this)->Press(key);
    }
    else if (type == Item::Type::Button)
    {
        return ((Button *)this)->Press(key.action);
    }
    else if (type == Item::Type::ChoiceParameter)
    {
        return ((ChoiceParameter *)this)->Press(key.action);
    }
    else if (type == Item::Type::SmallButton)
    {
        return ((SButton *)this)->Press(key.action);
    }
    else if(type == Item::Type::Page)
    {
        return ((Page *)this)->Press(key);
    }

    return 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Item *Page::Press(Control key)
{
    if(key.action.Is(Control::Action::Up))
    {
        if(key.Is(Control::Esc))
        {
            if(KEEPER(this))
            {
                CURRENT_PAGE = (Page *)KEEPER(this);
            }
        }
        else
        {
            CURRENT_PAGE = this;
        }
    }
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Item::Type Item::GetType() const
{
    return (Item::Type::E)type;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int8 Choice::CurrentIndex() const
{
    int8 retValue = 0;

    if (type == Item::Type::Choice)
    {
        if(isPageSB)
        {
            uint *address = (uint *)cell;
            retValue = (int8)((*address >> nameOrNumBit) & 0x01);
        }
        else
        {
            retValue = *cell;
        }
    }
    else if (type == Item::Type::ChoiceParameter)
    {
        ChoiceParameter *param = (ChoiceParameter *)this;

        Form *form = param->form;

        retValue = (int8)form->CurrentParameter()->value;
    }

    return retValue;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Item::PositionOnPage() const
{
    if(KEEPER(this) == 0)                       // Если у страницы нет хранителя - она принадлежит главному меню
    {
        return Menu::GetPosition((Page *)this);
    }

    Page *parent = (Page *)KEEPER(this);

    for(int i = 0; i < parent->NumItems(); i++)
    {
        if(this == parent->items[i])
        {
            return i;
        }
    }

    return -1;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Control Item::ButtonForItem() const
{
    int pos = PositionOnPage();

    if (pos >= 0)
    {
        return (Control::E)(Control::F1 + pos);
    }

    return Control::None;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Item *ChoiceParameter::Press(Control::Action action)
{
    if (action.Is(Control::Action::Up))
    {
        form->SetNextParameter();
    }
    else if (action.Is(Control::Action::Long))
    {
        return this;
    }

    return 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Item *SButton::Press(Control::Action action)
{
    if(action.Is(Control::Action::Up))
    {
        if (funcOnPress)
        {
            funcOnPress();
        }
    }

    return 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Item *Button::Press(Control::Action action)
{
    if (action.Is(Control::Action::Up) && funcOnPress)
    {
        funcOnPress();
    }

    return 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
pString ChoiceParameter::NameSubItem(int number) const
{
    return form->GetParameter(number)->Name();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
pString ChoiceParameter::NameCurrentSubItem() const
{
    return form->CurrentParameter()->Name();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
char *Item::FullPath() const
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

    if(pointer > 1)
    {
        pointer--;
    }

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



#ifdef WIN32
#pragma warning(pop)
#endif


