#include "defines.h"
#include "log.h"
#include "Display/Console.h"
#include "Display/Painter.h"
#include "Hardware/CPU.h"
#include "Utils/Debug.h"
#include <stm32f4xx_hal.h>


#define TRACE_HANDLER                               \
    LOG_FUNC_ENTER();                               \
    LOG_WRITE("%s : %d", Debug::file, Debug::line); \
    Color::FILL.SetAsCurrent();                     \
    Console::Draw();                                \
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
        TRACE_HANDLER;
        
        char *file = Debug::file;
        int line = Debug::line;
        
        while (1) //-V776
        {
            TRACE_HANDLER;
            file = file; //-V570
            line = line; //-V570
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

#ifdef __cplusplus
}
#endif
