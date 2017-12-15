#pragma once
#include "MenuItems.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define NUM_PAGES           4   ///< Количество страниц в меню


extern const Page *menu[NUM_PAGES];

class Menu
{
public:
        
    void Init();

    void Update();

    void Draw();
    /// Возвращает true, если ручка управляет переклюичением страниц меню
    bool RegIsControlPages();

    char *NameCurrentParameter();

private:

    void DrawTitle();

    void DrawPagesUGO();

    void ProcessControl(StructControl control);
};


extern Menu gMenu;
