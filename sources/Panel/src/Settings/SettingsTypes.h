#pragma once
#include "../../Common/Command.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum
{
    A,
    B,
    A_B,
    NumChannels
} Channel;

typedef enum
{
    Russian,
    English
} Language;


float MinValue(WaveParameter param);
float MaxValue(WaveParameter param);
