#pragma once
#include "Generator/Wave.h"
#include "Menu/MenuItems.h"



class PageSignals
{
public:
    static void Init();

    static void SetCurrentChanenl(Chan::E ch);

    static void OnPress_Channel(bool);

    static void OnPress_Form(bool);

    static void SetForm(TypeForm::E form);

    static Page *self;
};
