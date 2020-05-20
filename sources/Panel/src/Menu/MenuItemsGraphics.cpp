#include "Menu/Menu.h"
#include "Menu/MenuItems.h"
#include "Display/DisplayTypes.h"
#include "Display/Font/Font.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Settings/Settings.h"
#include "Utils/Debug.h"


using namespace Primitives;


void Item::Draw(bool opened, int x, int y) const
{
    switch(type)
    {
    case TypeItem::Choice:          static_cast<const Choice *>(this)->Draw(opened, x, y);          break;
    case TypeItem::Button:          static_cast<const Button *>(this)->Draw(x, y);                  break;
    case TypeItem::ChoiceParameter: static_cast<const ChoiceParameter *>(this)->Draw(opened, x, y); break;
    case TypeItem::SmallButton:     static_cast<const SButton *>(this)->Draw(x, y);                 break;
    case TypeItem::Governor:        static_cast<const Governor *>(this)->Draw(x, y);                break;
    case TypeItem::Page:            static_cast<const Page *>(this)->DrawClosed(x, y);              break;

    case TypeItem::NoneLight:
    case TypeItem::NoneDark:
    case TypeItem::GovernorColor:
        break;
    default:    
        Rectangle(Item::WIDTH - 5, Item::HEIGHT - 4).Fill(x + 2, y + 2, Menu::GetOpenedItem() ? Color::GRAY_10 : Color::GREEN_25);
        break;
    }
}


void SButton::Draw(int x, int y) const
{
    if(Menu::pressedItem == this)
    {
        Rectangle(Item::WIDTH - 2, Item::HEIGHT - 2).Fill(x + 1, y + 1, Color::FILL);
        Color::BACK.SetAsCurrent();
    }
    funcForDraw(x, y);
}


void Button::Draw(int x, int y) const
{
    bool isShade = IsShade();

    Rectangle(Item::WIDTH - 5, Item::HEIGHT - 4).Fill(x + 2, y + 2, isShade ? Color::GRAY_10 : Color::GREEN_10);

    if(isShade)
    {
        Color::GRAY_25.SetAsCurrent();
    }
    else
    {
        Color::FILL.SetAsCurrent();
    }

    Text::DrawTextInColumn(x + 5, y + 17, Item::WIDTH, GetTitle(LANGUAGE).c_str());
    funcForDraw(x, y);
}


void Page::DrawClosed(int x, int y) const
{
    Color color = Color::MENU_ITEM;
    if(IsShade())
    {
        color = Color::GRAY_10;
    }
    else if(IsPressed())
    {
        color = Color::GREEN_50;
    }

    Rectangle(Item::WIDTH - 5, Item::HEIGHT - 4).Fill(x + 2, y + 2, color);

    if(IsShade())
    {
        Color::GRAY_25.SetAsCurrent();
    }
    else
    {
        Color::FILL.SetAsCurrent();
    }

    Text::DrawTextInColumn(x + 4, y + 17, Item::WIDTH, GetTitle(LANGUAGE).c_str());
}


void ChoiceParameter::Draw(bool opened, int x, int y) const
{
    if(opened)
    {
    }
    else
    {
        bool pressed = IsPressed();
        bool isShade = IsShade();
        opened = IsOpened();

        Rectangle(Item::WIDTH - 5, 15).Fill(x + 2, y + 2, (pressed || opened) ? Color::GRAY_50 : (isShade ? Color::GRAY_10 : Color::GREEN_10));
        GetTitle(LANGUAGE).Draw(x + 5, y + 5, (pressed || opened) ? Color::BACK : (isShade ? Color::GRAY_25 : Color::FILL));
        Rectangle(Item::WIDTH - 5, 34).Fill(x + 2, y + 19, isShade ? Color::GRAY_10 : Color::GREEN_25);
        Text::DrawTextRelativelyRight(315, y + 30, NameCurrentSubItem(), Color::BACK);
    }
}


void Choice::Draw(bool opened, int x, int y) const
{
    Choice *choice = const_cast<Choice *>(this);

    int step = static_cast<int>(choice->Step() + 0.5F);

    if(opened)
    {
        int width = Item::WIDTH;
        int height = GetHeightOpened();
        
        if(x == -1)
        {
            y = Page::Title::HEIGHT + PositionOnPage() % Menu::NUM_ITEMS_ON_DISPLAY * Item::HEIGHT;
            if(y + GetHeightOpened() > SCREEN_HEIGHT)
            {
                y = SCREEN_HEIGHT - GetHeightOpened() - 2;
            }
            x = SCREEN_WIDTH - Item::WIDTH - 20;
        }

        Rectangle(width, Item::Title::HEIGHT).Fill(x, y, Color::GRAY_50);
        Rectangle(width, height - Item::Title::HEIGHT).Fill(x, y + Item::Title::HEIGHT, Color::BACK);
        Rectangle(width, height).Draw(x, y, Color::FILL);


        HLine::Draw(y + 12, x, x + width);
        Rectangle(width + 2, height + 2).Draw(x - 1, y - 1, Color::BACK);
        Text::DrawTextRelativelyRight(x + width - 2, y + 2, GetTitle(LANGUAGE).c_str());

        y += 14;

        for (int i = 0; i < NumSubItems(); i++)
        {
            if (CurrentIndex() == i)
            {
                Rectangle(Item::WIDTH - 4, 8).Fill(x + 2, y, Color::GREEN_50);
                HLine::Draw(y - 1, x + 1, x - 1 + Item::WIDTH - 1, Color::GREEN_75);
                VLine::Draw(x + 1, y - 1, y - 1 + 10);
                HLine::Draw(y + 9, x + 1, x + Item::WIDTH - 1, Color::GREEN_25);
                VLine::Draw(x - 1 + Item::WIDTH, y - 1, y + 9);
                NameSubItem(i, LANGUAGE).Draw(x + 3, y, Color::BACK);
            }
            else
            {
                NameSubItem(i, LANGUAGE).Draw(x + 3, y, Color::FILL);
            }
            y += 10;
        }
    }
    else
    {
        bool pressed = IsPressed();
        bool isShade = IsShade();
        opened = IsOpened();

        Rectangle(Item::WIDTH - 5, 15).Fill(x + 2, y + 2, (pressed || opened) ? Color::GRAY_50 : (isShade ? Color::GRAY_10 : Color::GREEN_10));
        GetTitle(LANGUAGE).Draw(x + 5, y + 5, pressed || opened? Color::BACK : (isShade ? Color::GRAY_25 : Color::FILL));
        Rectangle(Item::WIDTH - 5, 34).Fill(x + 2, y + 19, isShade ? Color::GRAY_10 : Color::GREEN_25);
        Color::BACK.SetAsCurrent();

        if (step == 0.0F) //-V2550 //-V550
        {
            Text::DrawTextRelativelyRight(315, y + 30, NameCurrentSubItem(LANGUAGE));
        }
        else
        {
            int x0 = 315 - step;

            Text::DrawTextRelativelyRight(x0, y + 30, NameCurrentSubItem(LANGUAGE));

            int length = Font::GetLengthText(NameNextSubItem(LANGUAGE).c_str());

            if (x0 + length > x + Item::WIDTH - 5)
            {
                NameNextSubItem(LANGUAGE).Draw(x0, y + 30);
            }
            else
            {
                Text::DrawTextRelativelyRight(315, y + 30, NameNextSubItem(LANGUAGE).c_str());
            }
        }
    }
}


void Governor::Draw(int x, int y) const
{
    bool pressed = IsPressed();
    bool isShade = IsShade();
    bool opened = IsOpened();

    Rectangle(Item::WIDTH - 5, 15).Fill(x + 2, y + 2, (pressed || opened) ? Color::GRAY_50 : (isShade ? Color::GRAY_10 : Color::GREEN_10));
    GetTitle(LANGUAGE).Draw(x + 5, y + 5, (pressed || opened) ? Color::BACK : (isShade ? Color::GRAY_25 : Color::FILL));
    Rectangle(Item::WIDTH - 5, 34).Fill(x + 2, y + 19, isShade ? Color::GRAY_10 : Color::GREEN_25);

    Color::BACK.SetAsCurrent();

    DrawValue(x + 15, y + 30);
}


void Governor::DrawValue(int x, int y) const
{
    if(Menu::GetCurrentItem() == this)
    {
        int x1 = Text::DrawFormatText(x, y, "%d", *cell);

        Char('\x90').Draw(x - 6, y);
        Char('\x91').Draw(x1 + 1, y);
    }
}


void Page::DrawOpened() const
{
    int numPage = CurrentSubPage();
    for (int i = numPage * 4; i < numPage * 4 + 4; i++)
    {
        int x = SCREEN_WIDTH - Item::WIDTH - 1;
        int y = Page::Title::HEIGHT + (i % Menu::NUM_ITEMS_ON_DISPLAY) * Item::HEIGHT;
        Rectangle(Item::WIDTH, Item::HEIGHT).Draw(x, y, Color::FILL);

        if(i < NumItems())
        {
            items[i]->Draw(false, x, y);
        }
    }

    funcOnDraw();
}
