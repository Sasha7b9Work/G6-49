#pragma once
#include "Command.h"


struct Settings
{
    ParamValue amplitude[Chan::Count];  ///< Размах для обоих каналов
    ParamValue offset[Chan::Count];     ///< Смещение для обоих каналов
};


extern Settings set;
