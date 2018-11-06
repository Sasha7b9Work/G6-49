#include "stdafx.h"
#ifndef WIN32
#ifdef OPEN
#include <stm32f7xx.h>
#else
#include <stm32f4xx.h>
#endif
#include "log.h"
#include "Display/Console.h"
#include "Display/Painter.h"
#include "Hardware/CPU.h"
#include "Utils/Debug.h"
#endif


#define TRACE_HANDLER               \
    LOG_FUNC_ENTER;                 \
    Painter::SetColor(Color::FILL); \
    Console::Draw();                \
    Painter::EndScene();


#ifdef __cplusplus
extern "C" {
#endif

    //------------------------------------------------------------------------------------------------------------------------------------------------
    void SysTick_Handler()
    {
        HAL_IncTick();
        HAL_SYSTICK_IRQHandler();
    }
    
        //--------------------------------------------------------------------------------------------------------------------------------------------
    void MemManage_Handler()
    {
        TRACE_HANDLER;

        while (1)
        {
        }
    }

    //------------------------------------------------------------------------------------------------------------------------------------------------
    void UsageFault_Handler()
    {
        TRACE_HANDLER;

        while (1)
        {
        }
    }

    //------------------------------------------------------------------------------------------------------------------------------------------------
    void HardFault_Handler()
    {
        TRACE_HANDLER;

        __IO int line = Debug::line;
        __IO char *file = Debug::file;
        
        while (1)
        {
            line++;
            file = file;
        }
    }

    //------------------------------------------------------------------------------------------------------------------------------------------------
    __attribute((noreturn)) void BusFault_Handler()
    {
        while (1)
        {
        }
    }

    //------------------------------------------------------------------------------------------------------------------------------------------------
    void PendSV_Handler()
    {
        TRACE_HANDLER;
    }

    //------------------------------------------------------------------------------------------------------------------------------------------------
    void NMI_Handler()
    {
        TRACE_HANDLER;
    }

    //------------------------------------------------------------------------------------------------------------------------------------------------
    void SVC_Handler()
    {
        TRACE_HANDLER;
    }

    //------------------------------------------------------------------------------------------------------------------------------------------------
    void DebugMon_Handler()
    {
        TRACE_HANDLER;
    }

#ifdef __cplusplus
}
#endif
