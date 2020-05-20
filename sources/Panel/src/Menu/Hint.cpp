#include "Hint.h"
#include "Log.h"
#include "Display/DisplayTypes.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Settings/Settings.h"
#include "Menu/Menu.h"


bool        Hint::show = false;
const Item *Hint::item = 0;
Key::E  Hint::control = Key::None;
int         Hint::numPages = 0;
int         Hint::currentPage = 0;
int         Hint::firstItemOnSecondPage = 0;
bool        Hint::needCalculate = false;



bool Hint::ProcessControl(const Key &key)
{
    if (key.Is(Key::Esc) && key.IsLong())
    {
        show = !show;
        item = 0;
        control = Key::None;
        numPages = 0;
    }

    if (show)
    {
        if (key.IsFunctional())
        {
            if(CURRENT_PAGE)
            {
                item = CURRENT_PAGE->GetItem(key);
            }
            control = Key::None;
            needCalculate = true;
        }
        else if (key.IsRotate())
        {
            if(numPages > 1)
            {
                if(key.Is(Key::RegLeft))
                {
                    if(currentPage > 1)
                    {
                        currentPage--;
                    }
                }
                else if(currentPage < numPages)
                {
                    currentPage++;
                }
            }
        }
        else
        {
            item = 0;
        }

        return true;
    }

    return false;
}


void Hint::Draw()
{
    if (show)
    {
        int x0 = 0;
        int y0 = Page::Title::HEIGHT;
        int width = 319 - Item::WIDTH;

        y0 = Text::DrawTextInBoundedRectWithTransfers(x0, y0, width,
            "Включён режим подсказок.\n"
            "Для получения информации по элементу меню или назначению кнопки используйте соответствующий орган управления.\n"
            "Для выхода из режима подсказок нажмите и удерживайте кнопку ESC.",
            Color::BACK, Color::FILL, Color::GRAY_50);

        Painter::DrawFilledRectangle(x0, y0, width, 239 - y0 - 1, Color::BACK, Color::FILL);

        if (item)
        {
            int yUGO = y0 + 5;

            Color::GREEN_50.SetAsCurrent();
            Text::DrawFormatStringInCenterRect(x0, y0 + 4, width, 10, "*** %s ***", item->GetTitle(LANGUAGE).c_str());
            Color::GREEN.SetAsCurrent();

            y0 = item->DrawHint(x0 + 5, y0 + 17, width - 8) + 5; //-V2007
            if (item->GetType() == TypeItem::Choice)
            {
                const Choice *choice = static_cast<const Choice *>(item);
                width -= 10;

                if(needCalculate)
                {
                    Calcualte(choice, x0, y0, width);
                    needCalculate = false;
                }

                DrawPagesUGO(width + 5, yUGO);

                if (numPages == 1)
                {
                    DrawDetailedHint(choice, x0, y0, width, 0, choice->NumSubItems() - 1);
                }
                else
                {
                    if(currentPage == 1)
                    {
                        DrawDetailedHint(choice, x0, y0, width, 0, firstItemOnSecondPage - 1);
                    }
                    else if(currentPage == 2)
                    {
                        DrawDetailedHint(choice, x0, y0, width, firstItemOnSecondPage, choice->NumSubItems() - 1);
                    }
                }
            }
            
        }
    }
}


int Hint::DrawDetailedHint(const Choice *choice, int x0, int y0, int width, int start, int end, bool calculate)
{
    Color colorWhite = Color::WHITE;
    Color colorGreen = Color::GREEN;

    if(calculate)
    {
        colorWhite = Color::BACK;
        colorGreen = Color::BACK;
    }

    colorWhite.SetAsCurrent();

    int number = start + 1;

    for (int i = start; i <= end; i++)
    {
        y0 = Text::DrawFormatTextInColumnWithTransfersDiffColors(x0 + 2, y0, width, colorGreen, "%d. \"%s\" %s", number++,
            choice->NameSubItem(i, LANGUAGE).c_str(), LANGUAGE ? choice->hintsEN[i] : choice->hintsRU[i]) + 5;

    }

    return y0;
}


int Hint::NumPagesInHint(const Choice *choice, int x, int y, int width)
{
    if(DrawDetailedHint(choice, x, y, width, 0, choice->NumSubItems(), true) < SCREEN_HEIGHT)
    {
        return 1;
    }

    return 2;
}


void Hint::Calcualte(const Choice *choice, int x, int y, int width)
{
    numPages = NumPagesInHint(choice, x, y, width);
    currentPage = 1;
    firstItemOnSecondPage = 0;

    if(numPages == 2)
    {
        for(int i = 0; i < choice->NumSubItems(); i++)
        {
            if(DrawDetailedHint(choice, x, y, width, 0, i, true) > SCREEN_HEIGHT - 1)
            {
                firstItemOnSecondPage = i;
                break;
            }
        }
    }
}


void Hint::DrawPagesUGO(const int xRight, const int y0)
{
    if(numPages < 2)
    {
        return;
    }

    int delta = 3;
    int size = 6;

    for(int i = 1; i <= numPages; i++)
    {
        int x = xRight - size - (i - 1) * (delta + size);

        if (numPages + 1 - i == currentPage)
        {
            Region(size, size).Fill(x, y0, Color::FILL);
            Font::Set(TypeFont::_5);
            Text::DrawChar(x + 2, y0 - 3, static_cast<char>(currentPage + 0x30), Color::BACK);
            Font::Set(TypeFont::_8);
        }
        else
        {
            Painter::DrawRectangle(x, y0, size, size, Color::FILL);
        }
    }
}
