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
        Painter_DrawTextRelativelyRight(315, y + 30, Choice_CurrentName((Choice *)item));
    }
    else if (type == Item_Button)
    {
        Painter_DrawTextInRect(x + 5, y + 17, WIDTH_ITEM, ItemTitle(item));
    }
    else if (type == Item_ChoiceParameter)
    {
        Painter::DrawText(x + 5, y + 5, ItemTitle(item));
        Painter_DrawTextRelativelyRight(315, y + 30, ChoiceWaveParameter_CurrentName((ChoiceParameter *)item));
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
    int width = WIDTH_ITEM;
    int height = Choice_NumChoices(choice) * 10 + 2 + 12;

    int y = HEIGHT_TITLE + PositionOnPage(choice) * HEIGHT_ITEM;
    int x = WIDTH_SCREEN - WIDTH_ITEM - 20;

    y += (HEIGHT_ITEM - height) / 2;

    ++y;

    Painter::FillRegionC(x, y, width, height, COLOR_BACK);
    Painter::DrawRectangleC(x, y, width, height, COLOR_FILL);
    Painter::DrawHLine(y + 12, x, x + width);

    Painter_DrawTextRelativelyRight(x + width - 2, y + 2, ItemTitle(choice));

    y += 14;

    for (int i = 0; i < Choice_NumChoices(choice); i++)
    {
        if (Choice_CurrentChoice(choice) == i)
        {
            Painter::FillRegionC(x + 2, y, WIDTH_ITEM - 4, 8, COLOR_FILL);
            Painter::DrawTextC(x + 3, y, Choice_Name(choice, i), COLOR_BACK);
        }
        else
        {
            Painter::DrawTextC(x + 3, y, Choice_Name(choice, i), COLOR_FILL);
        }
        y += 10;
    }
}
