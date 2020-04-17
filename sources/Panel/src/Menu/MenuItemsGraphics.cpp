#include "Menu/Menu.h"
#include "Menu/MenuItems.h"
#include "Display/DisplayTypes.h"
#include "Display/Font/Font.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Settings/Settings.h"
#include "Utils/Debug.h"


void Item::Draw(bool opened, int x, int y) const
{
    switch(type)
    {
    case Item::Type::Choice:          static_cast<const Choice *>(this)->Draw(opened, x, y);          break;
    case Item::Type::Button:          static_cast<const Button *>(this)->Draw(x, y);                  break;
    case Item::Type::ChoiceParameter: static_cast<const ChoiceParameter *>(this)->Draw(opened, x, y); break;
    case Item::Type::SmallButton:     static_cast<const SButton *>(this)->Draw(x, y);                 break;
    case Item::Type::Governor:        static_cast<const Governor *>(this)->Draw(x, y);                break;
    case Item::Type::Page:            static_cast<const Page *>(this)->DrawClosed(x, y);              break;
    case Item::Type::NoneDark:                                                                        break;
    default:    
        Painter::FillRegion(x + 2, y + 2, Item::WIDTH - 5, Item::HEIGHT - 4, Menu::GetOpenedItem() ? Color::GRAY_10 : Color::GREEN_25);
    }
}


void SButton::Draw(int x, int y) const
{
    if(Menu::pressedItem == this)
    {
        Painter::FillRegion(x + 1, y + 1, Item::WIDTH - 2, Item::HEIGHT - 2, Color::FILL);
        Color::BACK.SetAsCurrent();
    }
    funcForDraw(x, y);
}


void Button::Draw(int x, int y) const
{
    bool isShade = IsShade();

    Painter::FillRegion(x + 2, y + 2, Item::WIDTH - 5, Item::HEIGHT - 4, isShade ? Color::GRAY_10 : Color::GREEN_10);

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

    Painter::FillRegion(x + 2, y + 2, Item::WIDTH - 5, Item::HEIGHT - 4, color);

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

        Painter::FillRegion(x + 2, y + 2, Item::WIDTH - 5, 15, pressed || opened ? Color::GRAY_50 : (isShade ? Color::GRAY_10 : Color::GREEN_10));
        GetTitle(LANGUAGE).Draw(x + 5, y + 5, pressed || opened ? Color::BACK : (isShade ? Color::GRAY_25 : Color::FILL));
        Painter::FillRegion(x + 2, y + 19, Item::WIDTH - 5, 34, isShade ? Color::GRAY_10 : Color::GREEN_25);
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

        Painter::FillRegion(x, y, width, Item::Title::HEIGHT, Color::GRAY_50);
        Painter::FillRegion(x, y + Item::Title::HEIGHT, width, height - Item::Title::HEIGHT, Color::BACK);
        Painter::DrawRectangle(x, y, width, height, Color::FILL);
        Painter::DrawHLine(y + 12, x, x + width);
        Painter::DrawRectangle(x - 1, y - 1, width + 2, height + 2, Color::BACK);
        Text::DrawTextRelativelyRight(x + width - 2, y + 2, GetTitle(LANGUAGE).c_str());

        y += 14;

        for (int i = 0; i < NumSubItems(); i++)
        {
            if (CurrentIndex() == i)
            {
                Painter::FillRegion(x + 2, y, Item::WIDTH - 4, 8, Color::GREEN_50);
                Painter::DrawHLine(y - 1, x + 1, x - 1 + Item::WIDTH - 1, Color::GREEN_75);
                Painter::DrawVLine(x + 1, y - 1, y - 1 + 10);
                Painter::DrawHLine(y + 9, x + 1, x + Item::WIDTH - 1, Color::GREEN_25);
                Painter::DrawVLine(x - 1 + Item::WIDTH, y - 1, y + 9);
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

        Painter::FillRegion(x + 2, y + 2, Item::WIDTH - 5, 15, pressed || opened ? Color::GRAY_50 : (isShade ? Color::GRAY_10 : Color::GREEN_10));
        GetTitle(LANGUAGE).Draw(x + 5, y + 5, pressed || opened? Color::BACK : (isShade ? Color::GRAY_25 : Color::FILL));
        Painter::FillRegion(x + 2, y + 19, Item::WIDTH - 5, 34, isShade ? Color::GRAY_10 : Color::GREEN_25);
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

    Painter::FillRegion(x + 2, y + 2, Item::WIDTH - 5, 15, pressed || opened ? Color::GRAY_50 : (isShade ? Color::GRAY_10 : Color::GREEN_10));
    GetTitle(LANGUAGE).Draw(x + 5, y + 5, pressed || opened ? Color::BACK : (isShade ? Color::GRAY_25 : Color::FILL));
    Painter::FillRegion(x + 2, y + 19, Item::WIDTH - 5, 34, isShade ? Color::GRAY_10 : Color::GREEN_25);

    Color::BACK.SetAsCurrent();

    DrawValue(x + 15, y + 30);
}


void Governor::DrawValue(int x, int y) const
{
    if(Menu::GetCurrentItem() == this)
    {
        int x1 = Text::DrawFormatText(x, y, "%d", *cell);

        Text::DrawChar(x - 6, y, '\x90');
        Text::DrawChar(x1 + 1, y, '\x91');
    }
}


void Page::DrawOpened() const
{
    int numPage = CurrentSubPage();
    for (int i = numPage * 4; i < numPage * 4 + 4; i++)
    {
        int x = SCREEN_WIDTH - Item::WIDTH - 1;
        int y = Page::Title::HEIGHT + (i % Menu::NUM_ITEMS_ON_DISPLAY) * Item::HEIGHT;
        Painter::DrawRectangle(x, y, Item::WIDTH, Item::HEIGHT, Color::FILL);

        if(i < NumItems())
        {
            items[i]->Draw(false, x, y);
        }
    }

    funcOnDraw();
}
