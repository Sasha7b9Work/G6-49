#include "Hint.h"
#include "Display/DisplayTypes.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Settings/Settings.h"
#include "Menu/Menu.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Hint::show = false;
const Control *Hint::item = 0;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Hint::Show()
{
    return show;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Hint::Switch()
{
    show = !show;
    if(!show)
    {
        item = 0;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Hint::ClearItem()
{
    item = 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Hint::SetItem(const Control * const item_)
{
    item = item_;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Hint::Draw()
{
    if (Show())
    {
        int x0 = 0;
        int y0 = MP_TITLE_HEIGHT;
        int width = 319 - MI_WIDTH;

        y0 = Text::DrawTextInBoundedRectWithTransfers(x0, y0, width,
            LANG_RU ?
            "Включён режим подсказок.\n"
            "Для получения информации по элементу меню или назначению кнопки используйте соответствующий орган управления.\n"
            "Для выхода из режима подсказок нажмите и удерживайте кнопку ESC."
            :
            "Prompt mode is enabled.\n"
            "For information about the menu item or the purpose of the button, use the appropriate control.\n"
            "To exit the prompt mode, press and hold the ESC key."
            ,
            Color::BACK, Color::FILL, Color::GRAY_50);

        Painter::DrawFilledRectangle(x0, y0, width, 239 - y0 - 1, Color::BACK, Color::FILL);

        if (item)
        {
            Painter::SetColor(Color::GREEN_50);

            Text::DrawFormatStringInCenterRect(x0, y0 + 4, width, 10, "*** %s ***", item->Title());
            Painter::SetColor(Color::GREEN);

            y0 = item->DrawHint(x0 + 5, y0 + 17, width - 8) + 5;

            Painter::SetColor(Color::WHITE);

            int number = 1;

            if (item->Type() == Control_Choice)
            {
                Choice *choice = (Choice *)item;

                for (int i = 0; i < choice->NumSubItems(); i++)
                {
                    y0 = Text::DrawFormatTextInColumnWithTransfersDiffColors(x0 + 2, y0, width - 10, Color::GREEN, "%d. \"%s\" %s", number++,
                                                                             choice->NameSubItem(i), LANG_RU ? choice->hintsRu[i] : choice->hintsEn[i]) + 5;
                }
            }
        }
        else if (Menu::PanelControlHint())
        {

        }
    }
}
