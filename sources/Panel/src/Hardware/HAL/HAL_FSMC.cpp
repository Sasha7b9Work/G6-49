#include "defines.h"
#include "Hardware/CPU.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f4xx_hal.h>


void HAL_FSMC::Init()
{
    // \todo временно изменил - возможно, на флешку не пишет из-за неправильных таймингов
    static const FMC_NORSRAM_TimingTypeDef sramTiming =
    {
        6,                  // FSMC_AddressSetupTime
        8,                  // FSMC_AddressHoldTime
        14,                 // FSMC_DataSetupTime
        12,                 // FSMC_BusTurnAroundDuration
        3,                  // FSMC_CLKDivision
        0,                  // FSMC_DataLatency
        FMC_ACCESS_MODE_B   // FSMC_AccessMode
    };

    static const SRAM_HandleTypeDef sramHandle =
    {
        FMC_NORSRAM_DEVICE, //-V2571
        FMC_NORSRAM_EXTENDED_DEVICE, //-V2571
        {
            FMC_NORSRAM_BANK1,                 // Init.NSBank
            FMC_DATA_ADDRESS_MUX_ENABLE,       // Init.DataAddressMux
            FMC_MEMORY_TYPE_NOR,               // Init.MemoryType
            FMC_NORSRAM_MEM_BUS_WIDTH_8,       // Init.MemoryDataWidth
            FMC_BURST_ACCESS_MODE_DISABLE,     // Init.BurstAccessMode
            FMC_WAIT_SIGNAL_POLARITY_LOW,      // Init.WaitSignalPolarity
            FMC_WRAP_MODE_DISABLE,             // Init.WrapMode
            FMC_WAIT_TIMING_BEFORE_WS,         // Init.WaitSignalActive
            FMC_WRITE_OPERATION_ENABLE,        // Init.WriteOperation
            FMC_WAIT_SIGNAL_DISABLE,           // Init.WaitSignal
            FMC_EXTENDED_MODE_DISABLE,         // Init.ExtendedMode
            FMC_ASYNCHRONOUS_WAIT_DISABLE,     // Init.AsynchronousWait
            FMC_WRITE_BURST_DISABLE,           // Init.WriteBurst
            0, 0, 0
        },
        HAL_UNLOCKED, HAL_SRAM_STATE_RESET, 0
    };

    if(HAL_SRAM_Init(const_cast<SRAM_HandleTypeDef *>(&sramHandle), const_cast<FMC_NORSRAM_TimingTypeDef *>(&sramTiming), const_cast<FMC_NORSRAM_TimingTypeDef *>(&sramTiming)) != HAL_OK) //-V2567
    {
        ERROR_HANDLER();
    };
}
