#pragma once
#include "Menu/MenuItems.h"



bool ItemIsButton(void* item);

void Button_Press(Button *button);

bool ItemIsSButton(void* item);

void SButton_Press(SButton *button);

bool ItemIsChoiceParameter(void *item);
