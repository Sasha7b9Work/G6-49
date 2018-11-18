#pragma once
#include "stm32f429xx.h"


typedef enum
{
    DISABLE = 0U,
    ENABLE = !DISABLE
} FunctionalState;
