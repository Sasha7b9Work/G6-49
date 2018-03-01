#pragma once
#include "MenuItems.h"
#include "Hardware/Keyboard.h"


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Choice_Press(Choice *choice, TypePress press);
void ChoiceWaveParameter_Press(ChoiceParameter *choice, TypePress press);
void Button_Press(Button *button);
void SButton_Press(SButton *button);
void Item_Rotate(void *item, Control control);
