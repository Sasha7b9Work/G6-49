#pragma clang diagnostic ignored "-Wpadded"
#include <stm32f4xx.h>
#pragma clang diagnostic warning "-Wpadded"
#include "CPU.h"


#ifdef __cplusplus
extern "C" {
#endif

void LTDC_IRQHandler();
void TIM3_IRQHandler();

//----------------------------------------------------------------------------------------------------------------------------------------------------
void LTDC_IRQHandler(void)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void TIM3_IRQHandler(void)
{
    CPU::OnIRQHandlerTIM3();
}

#ifdef __cplusplus
}
#endif
