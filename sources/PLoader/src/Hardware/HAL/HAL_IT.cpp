#include "defines.h"
#include "common/Messages.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Debug.h"
#include <stm32f4xx_hal.h>


#ifdef __cplusplus
extern "C" {
#endif

    
    void SysTick_Handler()
    {
        HAL_IncTick();
        HAL_SYSTICK_IRQHandler();
    }
    

    void MemManage_Handler()
    {
        while (1)
        {
        }
    }

    
    void UsageFault_Handler()
    {
        while (1)
        {
        }
    }

    
    void HardFault_Handler()
    {
        const char *file = Debug::file[0];
        int line = Debug::line[0];

        int allocated = SimpleMessage::created;
        int destroed = SimpleMessage::destroed;
        int allocatedSize = SimpleMessage::createdSize;
        int destroedSize = SimpleMessage::destroedSize;


        while (1) //-V776
        {
            file = file; //-V570
            line = line; //-V570
            allocated = allocated; //-V570
            destroed = destroed; //-V570
            allocatedSize = allocatedSize; //-V570
            destroedSize = destroedSize; //-V570
        }
    }

    
    __attribute((noreturn)) void BusFault_Handler()
    {
        while (1)
        {
        }
    }

    
    void PendSV_Handler()
    {
    }

    
    void NMI_Handler()
    {
    }

    
    void SVC_Handler()
    {
    }

    
    void DebugMon_Handler()
    {
    }

    void TIM4_IRQHandler()
    {
        if((TIM4->SR & TIM_SR_UIF) == TIM_SR_UIF)
        {
            if((TIM4->DIER & TIM_DIER_UIE) == TIM_DIER_UIE)
            {
                TIM4->SR = ~TIM_DIER_UIE;
                HAL_TIM4::ElapsedCallback();
            }
        }
    }

    void TIM3_IRQHandler()
    {
        if((TIM3->SR & TIM_SR_UIF) == TIM_SR_UIF)
        {
            if((TIM3->DIER & TIM_DIER_UIE) == TIM_DIER_UIE)
            {
                TIM3->SR = ~TIM_DIER_UIE;
                Timer::ElapsedCallback();
            }
        }
    }

#ifdef __cplusplus
}
#endif
