#pragma once
#include "Menu/MenuItems.h"


class PageFrequencyCounter
{
public:
    /// —формировать содержимое регистра управлени€ частотомером в соответствии с настройками
    static void WriteRegisterRG9();

    static Page *pointer;
};
