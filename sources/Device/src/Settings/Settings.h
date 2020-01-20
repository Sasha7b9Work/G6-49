#pragma once
#include "common/Common.h"
#include "common/Command.h"


struct Settings
{
    FloatValue amplitude[Chan::Count];  ///< Размах для обоих каналов
    FloatValue offset[Chan::Count];     ///< Смещение для обоих каналов
};


extern Settings set;
