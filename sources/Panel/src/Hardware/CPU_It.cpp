#pragma clang diagnostic ignored "-Wpadded"
#include <stm32f4xx.h>
#pragma clang diagnostic warning "-Wpadded"
#include "CPU.h"


#ifdef __cplusplus
extern "C" {
#endif

void TIM3_IRQHandler();

//----------------------------------------------------------------------------------------------------------------------------------------------------
void TIM3_IRQHandler(void)
{
    CPU::OnIRQHandlerTIM3();
}

#ifdef __cplusplus
}
#endif
