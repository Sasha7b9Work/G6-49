// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/String.h"
#include <ff.h>



class FatFS
{
public:
    static String ErrorString(FRESULT result);
};

