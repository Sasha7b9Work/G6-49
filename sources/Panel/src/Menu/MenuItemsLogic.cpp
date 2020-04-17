#include "Menu/Menu.h"
#include "Display/Display.h"
#include "Display/Painter.h"
#include "Display/Symbols.h"
#include "Hardware/Timer.h"
#include "Log.h"
#include "Settings/Settings.h"
#include "Hardware/CPU.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include "Generator/Wave.h"
#include "Generator/Signals.h"
#include <string.h>


#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4626 5027)
#endif



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



void Choice::StartChange(int delta) const
{
    if (tsChoice.address != 0)
    {
        return;
    }
    if (!IsActive())
    {
        funcOnChanged(false);
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


float Choice::Step()
{
    static const float speed = 0.3F;
    static const int numLines = 60;
    if (tsChoice.address == this)
    {
        float delta = speed * (TIME_MS - tsChoice.timeStart);
        if (delta == 0.0F)  // -V550 //-V2550 //-V550
        {
            delta = 0.001F; // Таймер в несколько первых кадров может показать, что прошло 0 мс, но мы возвращаем большее число, потому что ноль будет говорить о том, что движения нет
        }
        int8 index = CurrentIndex();
        if (tsChoice.dir == INCREASE)
        {
            if (delta <= numLines)
            {
                return delta;
            }
            Math::CircleIncrease<int8>(&index, 0, static_cast<int8>(NumSubItems()) - 1);
        }
        else if (tsChoice.dir == DECREASE)
        {
            delta = -delta;
    
            if (delta >= -numLines)
            {
                return delta;
            }
            Math::CircleDecrease<int8>(&index, 0, static_cast<int8>(NumSubItems()) - 1);
        }

        if(isPageSB)
        {
            uint *address = reinterpret_cast<uint *>(cell);
            *address ^= (1 << static_cast<int>(nameOrNumBit));
        }
        else
        {
            *cell = index;
        }

        tsChoice.address = 0;

        funcOnChanged(IsActive());

        tsChoice.dir = NONE;
        return 0.0F;
    }
    return 0.0F;
}


Item *Choice::Press(const Key &key)
{   
    if((key.Is(Key::Right) && key.IsRelease()) || key.Is(Key::RegRight))
    {
        StartChange(-1);
    }
    else if((key.Is(Key::Left) && key.IsRelease()) || key.Is(Key::RegLeft))
    {
        StartChange(1);
    }
    else if(key.Is(Key::Esc) && key.IsRelease())
    {
        Menu::ResetOpenedItem();
    }
    else if(Keeper()->GetItem(key))
    {
        if(key.IsUp())
        {
            StartChange(1);
        }
        else if(key.IsLong())
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


void Governor::Press(const Key &key)
{
    if(key.IsFunctional() && Keeper()->GetItem(key) == this)
    {
        if(key.IsUp())
        {
            if (!Menu::GetCurrentItem())
            {
                Menu::SetCurrentItem(this);
                if (funcPress)
                {
                    funcPress(true);
                }
            }
            else if (Menu::GetCurrentItem() == this)
            {
                Menu::ResetCurrentItem();
            }
        }
        else if(key.IsLong())
        {
        }
    }
    else if(key.IsRotate())
    {
        if(key.Is(Key::RegLeft))
        {
            if(*cell > minValue)
            {
                *cell = *cell - 1;
                funcOfChanged();
            }
        }
        else if(key.Is(Key::RegRight))
        {
            if(*cell < maxValue)
            {
                *cell = *cell + 1;
                funcOfChanged();
            }
        }
    }
    else if(key.IsUp())
    {
        if(key.Is(Key::Esc))
        {
            Menu::ResetCurrentItem();
        }
    }
}


int8 Choice::NumSubItems() const
{
    return num;
}


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


int16 Governor::NextValue() const
{
    return ((*cell) < maxValue) ? ((*cell) + 1) : minValue;
}


int16 Governor::PrevValue() const
{
    return ((*cell) > minValue) ? ((*cell) - 1) : maxValue;
}


float Governor::Step()
{
    static const float speed = 0.05F;
    static const int numLines = 10;
    float delta = 0.0F;
    if (tsGovernor.address == this)
    {
        delta = speed * (TIME_MS - tsGovernor.timeStart);
        if (tsGovernor.dir == DECREASE)
        {
            delta *= -1.0F;
            if (delta == 0.0F)  // -V550 //-V2550 //-V550
            {
                return -0.001F;
            }
            if (delta < -numLines)
            {
                tsGovernor.dir = NONE;
                *cell = PrevValue();
                if (funcOfChanged)
                {
                    funcOfChanged();
                }
                delta = 0.0F;
                tsGovernor.address = 0;
            }
        }
        else if (tsGovernor.dir == INCREASE)
        {
            if (delta == 0.0F)  // -V550 //-V2550 //-V550
            {
                return 0.001F;
            }
            if (delta > numLines)
            {
                tsGovernor.dir = NONE;
                *cell = NextValue();
                if (funcOfChanged)
                {
                    funcOfChanged();
                }
                delta = 0.0F;
                tsGovernor.address = 0;
            }
        }
    }
    return delta;
}


void Governor::ChangeValue(int delta)
{
    int16 oldValue = *cell;
    *cell += static_cast<int16>(Math::Sign(delta) * Math::Pow10(gCurDigit));
    LIMITATION(*cell, minValue, maxValue);
    if (*cell != oldValue)
    {
        if (funcOfChanged)
        {
            funcOfChanged();
        }
    }
}


void Governor::NextPosition()
{
    if (Menu::GetOpenedItem() == this)
    {
        Math::CircleIncrease<int8>(&gCurDigit, 0, static_cast<int8>(NumDigits() - 1));
    }
}


int Governor::NumDigits() const
{
    int min = SU::NumDigitsInNumber(Math::Abs(minValue));
    int max = SU::NumDigitsInNumber(Math::Abs(maxValue));
    if (min > max)
    {
        max = min;
    }
    return max;
}

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



int Page::NumSubPages() const
{
    return (NumItems() - 1) / Menu::NUM_ITEMS_ON_DISPLAY + 1;
}


int Page::NumItems() const
{
    return num;
}


void Item::Press(const Key &key)
{
    if(key.IsLong())
    {
        if (IsOpened() || key.Is(Key::RegButton) || key.Is(Key::Esc))
        {
            Menu::ResetOpenedItem();
            return;
        }
    }

    if(IsShade())
    {
        return;
    }

    if(CURRENT_PAGE->GetItem(key) == this || key.IsRotate() || key.Is(Key::Esc) || key.IsCursors())
    {
        Menu::pressedItem = (key.IsDown() && !IsOpened()) ? this : 0;

        if (type == Item::Type::Choice)
        {
            static_cast<Choice *>(this)->Press(key);
        }
        else if (type == Item::Type::Button)
        {
            static_cast<Button *>(this)->Press(key.action);
        }
        else if (type == Item::Type::ChoiceParameter)
        {
            static_cast<ChoiceParameter *>(this)->Press(key.action);
        }
        else if (type == Item::Type::SmallButton)
        {
            static_cast<SButton *>(this)->Press(key.action);
        }
        else if(type == Item::Type::Page)
        {
            static_cast<Page *>(this)->Press(key);
        }
        else if(type == Item::Type::Governor)
        {
            static_cast<Governor *>(this)->Press(key);
        }
    }
}


Item::Type Item::GetType() const
{
    return static_cast<Item::Type::E>(type);
}


int8 Choice::CurrentIndex() const
{
    int8 retValue = 0;

    if (type == Item::Type::Choice)
    {
        if(isPageSB)
        {
            uint *address = reinterpret_cast<uint *>(cell);
            retValue = static_cast<int8>((*address >> nameOrNumBit) & 0x01);
        }
        else
        {
            retValue = *cell;
        }
    }
    else if (type == Item::Type::ChoiceParameter)
    {
        const ChoiceParameter *param = reinterpret_cast<const ChoiceParameter *>(this);

        Form *form = param->form;

        retValue = static_cast<int8>(static_cast<const ParameterValue *>(form->CurrentParameter())->Type());
    }

    return retValue;
}


int8 ChoiceBase::CurrentIndex() const
{
    if(type == Item::Type::Choice)
    {
        return *cell;
    }
    return 0;
}


int Item::PositionOnPage() const
{
    if(Keeper() == 0)                       // Если у страницы нет хранителя - она принадлежит главному меню
    {
        return Menu::GetPosition(static_cast<const Page *>(this));
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


Item *ChoiceParameter::Press(Key::Action action)
{
    if (action == Key::Up)
    {
        form->SetNextParameter();
    }
    else if (action == Key::Long)
    {
        return this;
    }

    return 0;
}


Item *SButton::Press(Key::Action action)
{
    if(action == Key::Down)
    {
        Menu::pressedItem = this;
        return this;
    }
    if(action == Key::Up || action == Key::Long)
    {
        if (funcOnPress)
        {
            funcOnPress();
            Menu::pressedItem = 0;
        }
    }

    return 0;
}


Item *Button::Press(Key::Action action)
{
    if (action == Key::Up)
    {
        funcOnPress();
    }

    return nullptr;
}


pString ChoiceParameter::NameSubItem(int number) const
{
    return static_cast<ParameterValue *>(form->GetParameter(number))->Name(LANGUAGE);
}


pString ChoiceParameter::NameCurrentSubItem() const
{
    return form->CurrentParameter()->Name(LANGUAGE);
}


String Item::FullPath()
{
    String titl = GetTitle(LANGUAGE);
    const char *titles[5] = {titl.c_str(), 0, 0, 0, 0};
    int pointer = 1;

    Page *parent = Keeper();

    while(parent)
    {
        titles[pointer++] = parent->title[LANGUAGE];
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
        std::strcat(buffer, titles[pointer]);
        std::strcat(buffer, " - ");
        
    } while(pointer > 0);

    buffer[std::strlen(buffer) - 3] = 0;

    return String(buffer);
}


int Choice::GetHeightOpened() const 
{
    return NumSubItems() * 10 + 2 + Item::Title::HEIGHT;
}


bool Page::Press(const Key &key)
{
    if(funcOnKey(key))
    {
        return true;
    }

    if (CURRENT_PAGE == this)
    {
        if (key.IsRotate() && Menu::RegIsControlSubPages())
        {
            ChangeSubPage(key.Is(Key::RegLeft) ? -1 : 1);
            return true;
        }
        else if (key.Is(Key::Esc) && key.IsUp())
        {
            if (Keeper())
            {
                this->funcEnter(false);
                CURRENT_PAGE = Keeper();
                return true;
            }
        }
        else if (key.IsFunctional())
        {
            GetItem(key)->Press(key);
            return true;
        }
    }
    else if (key.IsRelease())
    {
        CURRENT_PAGE = this;
        this->funcEnter(true);
        return true;
    }

    return false;
}


#ifdef WIN32
#pragma warning(pop)
#endif
