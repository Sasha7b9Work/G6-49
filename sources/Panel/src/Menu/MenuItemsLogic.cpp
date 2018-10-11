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
#include "Generator/Signals.h"
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
    if (!IsActive())
    {
        CHOICE_RUN_FUNC_CHANGED(this, false);
    }
    else
    {
        tsChoice.address = this;
        tsChoice.timeStart = TIME_MS;
        
        if(delta > 0)
        {
            tsChoice.dir = INCREASE;
        }
        else
        {
            tsChoice.dir = DECREASE;
        }
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
        CHOICE_RUN_FUNC_CHANGED(this, IsActive());
        tsChoice.dir = NONE;
        return 0.0f;
    }
    return 0.0f;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Item *Choice::Press(Control key)
{   
    if((key.Is(Control::Right) && key.action.IsRelease()) || key.Is(Control::Reg::Right))
    {
        StartChange(-1);
    }
    else if((key.Is(Control::Left) && key.action.IsRelease()) || key.Is(Control::Reg::Left))
    {
        StartChange(1);
    }
    else if(key.Is(Control::Esc) && key.action.IsRelease())
    {
        Menu::ResetOpenedItem();
    }
    else if(Keeper()->GetItem(key))
    {
        if(key.action.Is(Control::Action::Up))
        {
            StartChange(1);
        }
        else if(key.action.Is(Control::Action::Long))
        {
            if(Menu::GetOpenedItem() == 0)
            {
                Menu::SetOpenedItem(this);
            }
            else
            {
                Menu::ResetOpenedItem();
            }
        }
    }

    return Menu::GetOpenedItem();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Governor::Press(Control control)
{
    if(control.IsFunctional() && Keeper()->GetItem(control) == this)
    {
        if(control.action.Is(Control::Action::Up))
        {
            if (!Menu::GetCurrentItem())
            {
                Menu::SetCurrentItem(this);
            }
            else if (Menu::GetCurrentItem() == this)
            {
                Menu::ResetCurrentItem();
            }
        }
        else if(control.action.Is(Control::Action::Long))
        {
        }
    }
    else if(control.IsRotate())
    {
        if(control.Is(Control::Reg::Left))
        {
            if(*cell > minValue)
            {
                *cell = (*cell)--;
            }
        }
        else if(control.Is(Control::Reg::Right))
        {
            if(*cell < maxValue)
            {
                *cell = (*cell)++;
            }
        }
    }
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
        Ideograph::_8::Governor::Shift::_0,
        Ideograph::_8::Governor::Shift::_1,
        Ideograph::_8::Governor::Shift::_2,
        Ideograph::_8::Governor::Shift::_3
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
void Item::Press(Control key)
{
    if(key.action.Is(Control::Action::Long))
    {
        if (IsOpened() || key.Is(Control::Reg::Button) || key.Is(Control::Esc))
        {
            Menu::ResetOpenedItem();
            return;
        }
    }

    if(IsShade())
    {
        return;
    }

    if(CURRENT_PAGE->GetItem(key) == this || key.IsRotate() || key.Is(Control::Esc) || key.IsCursors())
    {
        Menu::pressedItem = (key.action.Is(Control::Action::Down)) && !IsOpened() ? this : 0;

        if (type == Item::Type::Choice)
        {
            ((Choice *)this)->Press(key);
        }
        else if (type == Item::Type::Button)
        {
            ((Button *)this)->Press(key.action);
        }
        else if (type == Item::Type::ChoiceParameter)
        {
            ((ChoiceParameter *)this)->Press(key.action);
        }
        else if (type == Item::Type::SmallButton)
        {
            ((SButton *)this)->Press(key.action);
        }
        else if(type == Item::Type::Page)
        {
            ((Page *)this)->Press(key);
        }
        else if(type == Item::Type::Governor)
        {
            ((Governor *)this)->Press(key);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Item::Type Item::GetType() const
{
    return (Item::Type::E)type;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int8 Choice::CurrentIndex()
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

        retValue = (int8)((ParameterValue *)form->CurrentParameter())->value;
    }

    return retValue;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int8 ChoiceBase::CurrentIndex() const
{
    if(type == Item::Type::Choice)
    {
        return *cell;
    }
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Item::PositionOnPage()
{
    if(Keeper() == 0)                       // Если у страницы нет хранителя - она принадлежит главному меню
    {
        return Menu::GetPosition((Page *)this);
    }

    for(int i = 0; i < Keeper()->NumItems(); i++)
    {
        if(this == Keeper()->items[i])
        {
            return i;
        }
    }

    return -1;
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
    if(action.Is(Control::Action::Down))
    {
        Menu::pressedItem = this;
        return this;
    }
    if(action.IsRelease())
    {
        if (funcOnPress)
        {
            funcOnPress();
            Menu::pressedItem = 0;
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
    return ((ParameterValue *)(form->GetParameter(number)))->Name();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
pString ChoiceParameter::NameCurrentSubItem() const
{
    return form->CurrentParameter()->Name();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
char *Item::FullPath()
{
    const char *titles[5] = {GetTitle()};
    int pointer = 1;

    Page *parent = Keeper();

    while(parent)
    {
        titles[pointer++] = parent->titleHint[LANG];
        parent = parent->Keeper();
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
    return NumSubItems() * 10 + 2 + Item::Title::HEIGHT;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Page::Press(Control control)
{
    if (funcOnKey)
    {
        if(funcOnKey(control))
        {
            return true;
        }
    }

    if (CURRENT_PAGE == this)
    {
        if (control.IsRotate() && Menu::RegIsControlSubPages())
        {
            ChangeSubPage(control.Is(Control::Reg::Left) ? -1 : 1);
            return true;
        }
        else if (control.Is(Control::Esc) && control.action.Is(Control::Action::Up))
        {
            if (Keeper())
            {
                CURRENT_PAGE = Keeper();
                return true;
            }
        }
        else if (control.IsFunctional())
        {
            GetItem(control)->Press(control);
            return true;
        }
    }
    else if (control.action.IsRelease())
    {
        CURRENT_PAGE = this;
        return true;
    }

    return false;
}


#ifdef WIN32
#pragma warning(pop)
#endif
