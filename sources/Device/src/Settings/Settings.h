#pragma once
#include "Command.h"


struct Settings
{
    ParamValue offset[Chan::Count];      ///< Смещение для обоих каналов
};


extern Settings set;
