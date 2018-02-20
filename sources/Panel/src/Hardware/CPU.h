#pragma once
#include "common/stm32/429/stm429.h"


class CPU : public STM429
{
public:
    static void InitLTDC();
    static void InitFSMC();
};


#ifdef __cplusplus
extern "C" {
#endif

void LTDC_IRQHandler();
void TIM3_IRQHandler();

#ifdef __cplusplus
}
#endif
