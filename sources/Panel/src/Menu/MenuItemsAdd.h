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


bool ItemIsButton(void* item);

void Button_Press(Button *button);

bool ItemIsSButton(void* item);

void SButton_Press(SButton *button);

bool ItemIsChoiceParameter(void *item);

void ChoiceWaveParameter_Press(ChoiceParameter *choice, TypePress press);

void Item_Rotate(Control *item, PanelControl control);
