// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
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
#include "Signals/Wave.h"
#include "Signals/Signals.h"
#include <string.h>


enum DIRECTION
{
    NONE,
    INCREASE,
    DECREASE
};

// ��������� ������������ ��� �������� ��������� ��� Choice � Governor
struct TimeStruct
{
    const void *address;    // ����� ��������. ���� 0 - �� ��������
    uint        timeStart;  // ����� ������ �������� � �������������
    DIRECTION   dir;        // ����������� ��������� ��������
    uint8       notUsed0;
    uint8       notUsed1;
    uint8       notUsed2;
};

static TimeStruct tsChoice = {0, 0, NONE, 0, 0, 0};
static TimeStruct tsGovernor = {0, 0, NONE, 0, 0, 0};

int8 gCurDigit = 0;



void Choice::StartChange(int delta) const
{
    if (funcOfActive())
    {
        int8 index = CurrentIndex();

        if (delta > 0)
        {
            Math::CircleIncrease<int8>(&index, 0, (int8)NumSubItems() - 1);
        }
        else if (delta < 0)
        {
            Math::CircleDecrease<int8>(&index, 0, (int8)NumSubItems() - 1);
        }

        *cell = index;
    }

    funcOnChanged(funcOfActive());
}


float Choice::Step()
{
    static const float speed = 3000.0F;
    static const int numLines = 60;
    if (tsChoice.address == this)
    {
        float delta = speed * (float)(_TIME_MS - tsChoice.timeStart);
        if (delta == 0.0F)  // -V550 //-V2550 //-V550
        {
            delta = 0.001F; // ������ � ��������� ������ ������ ����� ��������, ��� ������ 0 ��, �� �� ���������� ������� �����, ������ ��� ���� ����� �������� � ���, ��� �������� ���
        }
        int8 index = CurrentIndex();
        if (tsChoice.dir == INCREASE)
        {
            if (delta <= numLines)
            {
                return delta;
            }
            Math::CircleIncrease<int8>(&index, 0, (int8)NumSubItems() - 1);
        }
        else if (tsChoice.dir == DECREASE)
        {
            delta = -delta;
    
            if (delta >= -numLines)
            {
                return delta;
            }
            Math::CircleDecrease<int8>(&index, 0, (int8)NumSubItems() - 1);
        }

        *cell = index;

        tsChoice.address = 0;

        funcOnChanged(funcOfActive());

        tsChoice.dir = NONE;
        return 0.0F;
    }
    return 0.0F;
}


void Governor::Press(const Control &control) //-V801
{
    if(control.IsFunctional() && Keeper()->GetItem(control.key) == this)
    {
        if(control.IsUp())
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
        else if(control.IsLong())
        {
        }
    }
    else if(control.IsRotate())
    {
        if(control.Is(Key::RotateLeft))
        {
            if(*cell > minValue)
            {
                *cell = *cell - 1;
                funcOfChanged();
            }
        }
        else if(control.Is(Key::RotateRight))
        {
            if(*cell < maxValue)
            {
                *cell = *cell + 1;
                funcOfChanged();
            }
        }
    }
    else if(control.IsUp())
    {
        if(control.Is(Key::Esc))
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
        tsGovernor.timeStart = _TIME_MS;
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
        delta = speed * (float)(_TIME_MS - tsGovernor.timeStart);
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
    *cell += (int16)(Math::Sign(delta) * Math::Pow10(gCurDigit));
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
        Math::CircleIncrease<int8>(&gCurDigit, 0, (int8)(NumDigits() - 1));
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


void Item::Press(const Control &control) //-V801
{
    if(control.IsLong())
    {
        if (IsOpened() || control.Is(Key::RegButton) || control.Is(Key::Esc))
        {
            Menu::ResetOpenedItem();
            return;
        }
    }

    if(IsShade())
    {
        return;
    }

    if(CURRENT_PAGE->GetItem(control.key) == this || control.IsRotate() || control.Is(Key::Esc) || control.IsCursors())
    {
        Menu::pressedItem = (control.IsDown() && !IsOpened()) ? this : 0;

        switch (type.value)
        {
        case TypeItem::Choice:          ((Choice *)this)->Press(control);           break;
        case TypeItem::Button:          ((Button *)this)->Press(control.action);    break;
        case TypeItem::ChoiceParameter: ((ChoiceParameter *)this)->Press(control);  break;
        case TypeItem::SmallButton:     ((SButton *)this)->Press(control.action);   break;
        case TypeItem::Page:            ((Page *)this)->Press(control);             break;
        case TypeItem::Governor:        ((Governor *)this)->Press(control);         break;
        case TypeItem::NoneLight:
        case TypeItem::NoneDark:
        case TypeItem::GovernorColor:
            break;
        }
    }
}


int8 Choice::CurrentIndex() const
{
    int8 retValue = 0;

    if (type.IsChoice())
    {
        retValue = *cell;
    }
    else if (type.IsChoiceParameter())
    {
        const ChoiceParameter *param = (const ChoiceParameter *)(this);

        Form *form = param->form;

        retValue = (int8)((const DParam *)form->CurrentParameter())->GetType();
    }

    return retValue;
}


int8 ChoiceBase::CurrentIndex() const
{
    if(type.IsChoice())
    {
        return *cell;
    }

    return 0;
}


int Item::PositionOnPage() const
{
    if(Keeper() == 0)                       // ���� � �������� ��� ��������� - ��� ����������� �������� ����
    {
        return Menu::GetPosition((const Page *)this);
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


Item *ChoiceParameter::Press(const Control &control)
{
    if ((control.Is(Key::Right) && control.IsRelease()) || control.Is(Key::RotateRight))
    {
        form->SetPrevParameter();
    }
    else if ((control.Is(Key::Left) && control.IsRelease()) || control.Is(Key::RotateLeft))
    {
        form->SetNextParameter();
    }
    else if (control.Is(Key::Esc) && control.IsRelease())
    {
        Menu::ResetOpenedItem();
    }
    else if (Keeper()->GetItem(control.key))
    {
        if (control.IsUp())
        {
            form->SetNextParameter();
        }
        else if (control.IsLong())
        {
            if (Menu::GetOpenedItem() == nullptr)
            {
                Menu::SetOpenedItem(this);
            }
            else
            {
                Menu::ResetOpenedItem();
            }
        }
    }
    

    return nullptr;
}


Item *Choice::Press(const Control &control)
{
    if ((control.Is(Key::Right) && control.IsRelease()) || control.Is(Key::RotateRight))
    {
        StartChange(-1);
    }
    else if ((control.Is(Key::Left) && control.IsRelease()) || control.Is(Key::RotateLeft))
    {
        StartChange(1);
    }
    else if (control.Is(Key::Esc) && control.IsRelease())
    {
        Menu::ResetOpenedItem();
    }
    else if (Keeper()->GetItem(control.key))
    {
        if (control.IsUp())
        {
            StartChange(1);
        }
        else if (control.IsLong())
        {
            if (Menu::GetOpenedItem() == 0)
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


Item *SButton::Press(Action::E action)
{
    if(action == Action::Down)
    {
        Menu::pressedItem = this;
        return this;
    }
    if(action == Action::Up || action == Action::Long)
    {
        if (funcOnPress)
        {
            funcOnPress();
            Menu::pressedItem = 0;
        }
    }

    return 0;
}


Item *Button::Press(Action::E action)
{
    if (action == Action::Up)
    {
        funcOnPress();
    }

    return nullptr;
}


cstr ChoiceParameter::NameSubItem(int number) const
{
    return ((DParam *)form->GetParameter(number))->Name();
}


String ChoiceParameter::NameCurrentSubItem() const
{
    return String(form->CurrentParameter()->Name());
}


String Item::FullPath()
{
    String titl = GetTitle();
    pchar titles[5] = {titl.c_str(), 0, 0, 0, 0};
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


int ChoiceParameter::GetHeightOpened() const
{
    return  form->NumParameters() * 10 + 2 + Item::Title::HEIGHT;
}


bool Page::Press(const Control &control)
{
    if(funcOnKey(control))
    {
        return true;
    }

    if (CURRENT_PAGE == this)
    {
        if (control.IsRotate() && Menu::RegIsControlSubPages())
        {
            ChangeSubPage(control.Is(Key::RotateLeft) ? -1 : 1);
            return true;
        }
        else if (control.Is(Key::Esc) && control.IsUp())
        {
            if (Keeper())
            {
                this->funcEnter(false);
                CURRENT_PAGE = Keeper();
                return true;
            }
        }
        else if (control.IsFunctional())
        {
            GetItem(control.key)->Press(control);
            return true;
        }
    }
    else if (control.IsRelease())
    {
        CURRENT_PAGE = this;
        this->funcEnter(true);
        return true;
    }

    return false;
}
