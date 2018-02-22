#include "MenuItemsDrawing.h"
#include "Display/DisplayTypes.h"
#include "Display/Painter.h"
#include "Settings/Settings.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void DrawOpenedChoice(Choice *choice);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Item_Draw(int x, int y, void *item)
{
    Item type = TypeItem(item);

    if (type == Item_Choice)
    {
        Painter::DrawText(x + 5, y + 5, ItemTitle(item));
        Painter::DrawTextRelativelyRight(315, y + 30, Choice_CurrentName((Choice *)item));
    }
    else if (type == Item_Button)
    {
        Painter::DrawTextInRect(x + 5, y + 17, ITEM_WIDTH, ItemTitle(item));
    }
    else if (type == Item_ChoiceParameter)
    {
        Painter::DrawText(x + 5, y + 5, ItemTitle(item));
        Painter::DrawTextRelativelyRight(315, y + 30, ChoiceWaveParameter_CurrentName((ChoiceParameter *)item));
    }
    else if (type == Item_SButton)
    {
        int size = 23;
        x += 20;
        y += 17;
        Painter::DrawRectangle(x, y, size, size);
        SButton *button = (SButton*)item;
        if (button->funcDraw)
        {
            button->funcDraw(x, y);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void DrawOpenedItem(void)
{
    if (OPENED_ITEM)
    {
        Item typeItem = TypeItem(OPENED_ITEM);

        if (typeItem == Item_Choice)
        {
            DrawOpenedChoice((Choice *)OPENED_ITEM);
        }
        else if (typeItem == Item_ChoiceParameter)
        {
            DrawOpenedChoice((Choice *)OPENED_ITEM);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawOpenedChoice(Choice *choice)
{
    int width = ITEM_WIDTH;
    int height = Choice_NumChoices(choice) * 10 + 2 + 12;

    int y = TITLE_HEIGHT + PositionOnPage(choice) * ITEM_HEIGHT;
    int x = SCREEN_WIDTH - ITEM_WIDTH - 20;

    y += (ITEM_HEIGHT - height) / 2;

    ++y;

    Painter::FillRegion(x, y, width, height, Color::BACK);
    Painter::DrawRectangle(x, y, width, height, Color::FILL);
    Painter::DrawHLine(y + 12, x, x + width);

    Painter::DrawTextRelativelyRight(x + width - 2, y + 2, ItemTitle(choice));

    y += 14;

    for (int i = 0; i < Choice_NumChoices(choice); i++)
    {
        if (Choice_CurrentChoice(choice) == i)
        {
            Painter::FillRegion(x + 2, y, ITEM_WIDTH - 4, 8, Color::FILL);
            Painter::DrawText(x + 3, y, Choice_Name(choice, i), Color::BACK);
        }
        else
        {
            Painter::DrawText(x + 3, y, Choice_Name(choice, i), Color::FILL);
        }
        y += 10;
    }
}
