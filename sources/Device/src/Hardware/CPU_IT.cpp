#include <stm32f4xx.h>
#include "Hardware/VCP.h"
#include "Hardware/HAL/HAL.h"
#include "Interface/Interface_d.h"
#include "Utils/Debug.h"


#define TRACE_HANDLER                   
/*
    Console::AddString(__FUNCTION__);
*/

#ifdef __cplusplus
extern "C" {
#endif

    
    void SysTick_Handler()
    {
        HAL_IncTick();
        HAL_SYSTICK_IRQHandler();
    }

    
    void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *)
    {
        TRACE_HANDLER;
//        Console::AddInt((int)handle->ErrorCode);
    }


    
    void OTG_FS_IRQHandler()
    {
        // Флешка
        HAL_HCD_IRQHandler(&HAL_HCD::handle);
    }

    
    void OTG_HS_IRQHandler()
    {
        // VCP
        HAL_PCD_IRQHandler(&CPU::PCD::handle);
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
        while (1)
        {
        }
    }

    
    void BusFault_Handler()
    {
        TRACE_HANDLER;
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

#ifdef __cplusplus
}
#endif
