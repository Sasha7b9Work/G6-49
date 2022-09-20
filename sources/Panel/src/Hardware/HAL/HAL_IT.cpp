// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "log.h"
#include "Display/Console.h"
#include "Display/Painter.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Debug.h"
#include <stm32f4xx_hal.h>


#define TRACE_HANDLER                                     \
    LOG_FUNC_ENTER();                                     \
    LOG_WRITE("%s : %d", Debug::file[0], Debug::line[0]); \
    Color::FILL.SetAsCurrent();                           \
    Console::Draw();                                      \
    Painter::EndScene();


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
        TRACE_HANDLER;

        while (1)
        {
        }
    }


    void UsageFault_Handler()
    {
        TRACE_HANDLER;

        while (1)
        {
        }
    }


    void HardFault_Handler()
    {
        while (1) //-V776
        {
            TRACE_HANDLER;
        }
    }


    __attribute((noreturn)) void BusFault_Handler()
    {
        TRACE_HANDLER;
        while (1)
        {
        }
    }


    void PendSV_Handler()
    {
        TRACE_HANDLER;
    }


    void NMI_Handler()
    {
        TRACE_HANDLER;
    }


    void SVC_Handler()
    {
        TRACE_HANDLER;
    }


    void DebugMon_Handler()
    {
        TRACE_HANDLER;
    }

    void TIM4_IRQHandler()
    {
        if ((TIM4->SR & TIM_SR_UIF) == TIM_SR_UIF)
        {
            if ((TIM4->DIER & TIM_DIER_UIE) == TIM_DIER_UIE)
            {
                TIM4->SR = ~TIM_DIER_UIE;
                HAL_TIM4::ElapsedCallback();
            }
        }
    }

    void TIM3_IRQHandler()
    {
        if ((TIM3->SR & TIM_SR_UIF) == TIM_SR_UIF)
        {
            if ((TIM3->DIER & TIM_DIER_UIE) == TIM_DIER_UIE)
            {
                TIM3->SR = ~TIM_DIER_UIE;
                Timer::ElapsedCallback();
            }
        }
    }

#ifdef __cplusplus
}
#endif
