#pragma once
#include "MenuItems.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define NUM_PAGES           4   ///< Количество страниц в меню


extern const Page *menu[NUM_PAGES];

class Menu
{
public:
        
    static void Init();

    static void Update();

    static void Draw();
    /// Возвращает true, если ручка управляет переклюичением страниц меню
    static bool RegIsControlPages();

    static char *NameCurrentParameter();

private:

    static void DrawTitle();

    static void DrawPagesUGO();

    static void ProcessControl(StructControl control);
};
