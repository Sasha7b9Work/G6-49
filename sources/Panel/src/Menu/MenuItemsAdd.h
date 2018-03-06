#pragma once
#include "Menu/MenuItems.h"


struct AllowableParameters
{
    bool allow[NumParameters];
};

///
class ChoiceParameterBase
{
public:
    COMMON_PART_MENU_ITEM;
    pFuncVV              funcOnPress;
    AllowableParameters  allowParameters;
    uint8               *numParameter;
};


class ChoiceParameter : public Control
{
public:
    pFuncVV              funcOnPress;
    AllowableParameters  allowParameters;
    uint8               *numParameter;
};



char* NameWaveForm(WaveForm form);

char* NameParameter(WaveParameter parameter);

void* ItemFromPage(int numItem);

bool ItemIsChoice(void* item);

void Choice_Press(Choice *choice, TypePress press);

bool ItemIsButton(void* item);

void Button_Press(Button *button);

bool ItemIsSButton(void* item);

void SButton_Press(SButton *button);

bool ItemIsChoiceParameter(void *item);

void ChoiceWaveParameter_Press(ChoiceParameter *choice, TypePress press);

int Choice_NumChoices(Choice *choice);

void Item_Rotate(void *item, PanelControl control);
/// Возвращает функциональную клавишу, назначенную пункту меню
PanelControl ButtonForItem(void *item);

char* ChoiceWaveParameter_CurrentName(ChoiceParameter *choice);

/// Возвращает адрес текущую страницу
Page* CurrentPage();

void Item_Draw(int x, int y, void *item);

void DrawOpenedItem();

const char* Page_Name(const Page* page);

TypeItem TypeOfItem(void* address);

const char* ItemTitle(void* item);

const char* Choice_CurrentName(Choice* choice);
/// Возвращает порядковый номер пункта меню на странице
int PositionOnPage(void *item);

int Choice_CurrentChoice(Choice *choice);

const char* Choice_Name(Choice* choice, int num);
