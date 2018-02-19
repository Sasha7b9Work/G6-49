#pragma once
#include "common/stm32/429/stm429.h"


class CPU : public STM429
{
public:
    static void InitLTDC();
    static void InitFSMC();
};
