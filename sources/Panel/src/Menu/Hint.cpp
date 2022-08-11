// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "Hint.h"
#include "Log.h"
#include "Display/DisplayTypes.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Settings/Settings.h"
#include "Menu/Menu.h"


using namespace Primitives;


bool        Hint::show = false;
const Item *Hint::item = 0;
Key::E      Hint::key = Key::None;
int         Hint::numPages = 0;
int         Hint::currentPage = 0;
int         Hint::firstItemOnSecondPage = 0;
bool        Hint::needCalculate = false;



bool Hint::ProcessControl(const Control &control)
{
    if (control.Is(Key::Esc) && control.IsLong())
    {
        show = !show;
        item = 0;
        key = Key::None;
        numPages = 0;
    }

    if (show)
    {
        if (control.IsFunctional())
        {
            if(CURRENT_PAGE)
            {
                item = CURRENT_PAGE->GetItem(key);
            }
            key = Key::None;
            needCalculate = true;
        }
        else if (control.IsRotate())
        {
            if(numPages > 1)
            {
                if(control.Is(Key::RotateLeft))
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

        y0 = String(
            "Включён режим подсказок.\n"
            "Для получения информации по элементу меню или назначению кнопки используйте соответствующий орган управления.\n"
            "Для выхода из режима подсказок нажмите и удерживайте кнопку ESC."
        ).DrawInBoundedRectWithTransfers(x0, y0, width, Color::BACK, Color::FILL, Color::GRAY_50);

        Rectangle(width, 239 - y0 - 1).DrawFilled(x0, y0, Color::BACK, Color::FILL);

        if (item)
        {
            int yUGO = y0 + 5;

            Color::GREEN_50.SetAsCurrent();
            String("*** %s ***", item->GetTitle().c_str()).DrawInCenterRect(x0, y0 + 4, width, 10);
            Color::GREEN.SetAsCurrent();

            y0 = item->DrawHint(x0 + 5, y0 + 17, width - 8) + 5; //-V2007
            if(item->type.IsChoice())
            {
                const Choice *choice = (const Choice *)item;
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
        y0 = String(
            "%d. \"%s\" %s", number++, choice->NameSubItem(i).c_str(), LANG_RU ? choice->hintsRU[i] : choice->hintsEN[i]
                    ).DrawInColumnWithTransfersDiffColors(x0 + 2, y0, width, colorGreen) + 5;
    }

    return y0;
}


int Hint::NumPagesInHint(const Choice *choice, int x, int y, int width)
{
    if(DrawDetailedHint(choice, x, y, width, 0, choice->NumSubItems(), true) < Display::HEIGHT)
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
            if(DrawDetailedHint(choice, x, y, width, 0, i, true) > Display::HEIGHT - 1)
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
            Rectangle(size, size).Fill(x, y0, Color::FILL);
            Font::Set(TypeFont::_5);
            Char((char)(currentPage + 0x30)).Draw(x + 2, y0 - 3, Color::BACK);
            Font::Set(TypeFont::_8);
        }
        else
        {
            Rectangle(size, size).Draw(x, y0, Color::FILL);
        }
    }
}
