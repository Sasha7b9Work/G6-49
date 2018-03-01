#pragma once
#include "Hardware/stm32/407/stm407.h"


class CPU : public STM407
{
public:
    static void Init();

private:

    static void EnablePeriphery();

    static void InitGPIOS();
};
