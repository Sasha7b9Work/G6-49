#include "stdafx.h"
#ifndef WIN32
#ifdef OPEN
#include <stm32f7xx.h>
#else
#include <stm32f4xx.h>
#endif
#include "CPU.h"
#include "LTDC.h"
#include "Display/Console.h"
#include "Display/Painter.h"
#include "Hardware/Timer.h"
#include "Hardware/Modules/SPI.h"
#include "Log.h"
#include <cstdlib>
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static GPIO_TypeDef * const ports[] = {GPIOA, GPIOB, GPIOC, GPIOD, GPIOE};

uint  CPU::timeStartMeasFPS = 0;
int   CPU::numFrames = 0;
float CPU::fps = 0.0f;

static CRC_HandleTypeDef handleCRC = {CRC};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPU::Init()
{
    SystemClockConfig();

#ifdef OPEN
    STM746::Init();
#else
    STM429::Init();
#endif

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOI_CLK_ENABLE();
    
    __HAL_RCC_TIM2_CLK_ENABLE();    // Для тиков
    __HAL_RCC_TIM3_CLK_ENABLE();    // Для таймеров
    __HAL_RCC_TIM4_CLK_ENABLE();    // Для опроса клавиатуры

    __HAL_RCC_DMA2D_CLK_ENABLE();
    __HAL_RCC_LTDC_CLK_ENABLE();
    __HAL_RCC_SPI4_CLK_ENABLE();

    FSMC::Init();

    SPI4_::Init();

    Keyboard::Init();

    CRC32::Init();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::GPIO_::WritePin(char port, uint16 maskPin, bool state)
{
    HAL_GPIO_WritePin(ports[port - 'A'], maskPin, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CPU::GPIO_::ReadPin(char port, uint16 maskPin)
{
    return HAL_GPIO_ReadPin(ports[port - 'A'], maskPin) == GPIO_PIN_SET;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::SystemClockConfig()
{
#ifdef OPEN
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

    __PWR_CLK_ENABLE();

    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 400;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 2;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    HAL_PWREx_ActivateOverDrive();

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6);

    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC|RCC_PERIPHCLK_USART1;
    PeriphClkInitStruct.PLLSAI.PLLSAIN = 192;
    PeriphClkInitStruct.PLLSAI.PLLSAIR = 3;
    PeriphClkInitStruct.PLLSAI.PLLSAIQ = 2;
    PeriphClkInitStruct.PLLSAI.PLLSAIP = RCC_PLLSAIP_DIV2;
    PeriphClkInitStruct.PLLSAIDivQ = 1;
    PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_2;
    PeriphClkInitStruct.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
#else
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

    /**Configure the main internal regulator output voltage
    */
    __HAL_RCC_PWR_CLK_ENABLE();

    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = 16;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 16;
    RCC_OscInitStruct.PLL.PLLN = 360;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    /**Activate the Over-Drive mode
    */
    if (HAL_PWREx_EnableOverDrive() != HAL_OK)
    {
        ERROR_HANDLER();
    }

    /**Initializes the CPU, AHB and APB busses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
    PeriphClkInitStruct.PLLSAI.PLLSAIN = 100;
    PeriphClkInitStruct.PLLSAI.PLLSAIR = 2;
    PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_2;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    /**Configure the Systick interrupt time
    */
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

    /**Configure the Systick
    */
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
#endif
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::FSMC::Init()
{
#ifndef OPEN
    /// \todo временно изменил - возможно, на флешку не пишет из-за неправильных таймингов
    static const FMC_NORSRAM_TimingTypeDef sramTiming =
    {
        6,                  ///< FSMC_AddressSetupTime
        8,                  ///< FSMC_AddressHoldTime
        14,                 ///< FSMC_DataSetupTime
        12,                 ///< FSMC_BusTurnAroundDuration
        3,                  ///< FSMC_CLKDivision
        0,                  ///< FSMC_DataLatency
        FMC_ACCESS_MODE_B   ///< FSMC_AccessMode
    };

    static const SRAM_HandleTypeDef sramHandle =
    {
        FMC_NORSRAM_DEVICE,
        FMC_NORSRAM_EXTENDED_DEVICE,
        {
            FMC_NORSRAM_BANK1,                 ///< Init.NSBank
            FMC_DATA_ADDRESS_MUX_ENABLE,       ///< Init.DataAddressMux
            FMC_MEMORY_TYPE_NOR,               ///< Init.MemoryType
            FMC_NORSRAM_MEM_BUS_WIDTH_8,       ///< Init.MemoryDataWidth
            FMC_BURST_ACCESS_MODE_DISABLE,     ///< Init.BurstAccessMode
            FMC_WAIT_SIGNAL_POLARITY_LOW,      ///< Init.WaitSignalPolarity
            FMC_WRAP_MODE_DISABLE,             ///< Init.WrapMode
            FMC_WAIT_TIMING_BEFORE_WS,         ///< Init.WaitSignalActive
            FMC_WRITE_OPERATION_ENABLE,        ///< Init.WriteOperation
            FMC_WAIT_SIGNAL_DISABLE,           ///< Init.WaitSignal
            FMC_EXTENDED_MODE_DISABLE,         ///< Init.ExtendedMode
            FMC_ASYNCHRONOUS_WAIT_DISABLE,     ///< Init.AsynchronousWait
            FMC_WRITE_BURST_DISABLE,           ///< Init.WriteBurst
            0, 0, 0
        },
        HAL_UNLOCKED, HAL_SRAM_STATE_RESET, 0
    };

    if (HAL_SRAM_Init((SRAM_HandleTypeDef*)(&sramHandle), (FMC_NORSRAM_TimingTypeDef*)(&sramTiming),
        (FMC_NORSRAM_TimingTypeDef*)(&sramTiming)) != HAL_OK)
    {
        ERROR_HANDLER();
    };
#endif
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::Update()
{
    ++numFrames;
    if(TIME_MS >= timeStartMeasFPS + 1000)  // Если прошло более 1 секунды с начала 1-сек отрезка
    {                                       // рассчитываем ФПС
        fps = (float)numFrames / (TIME_MS - timeStartMeasFPS) * 1e3f;
        numFrames = 0;
        timeStartMeasFPS = TIME_MS;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float CPU::GetFPS()
{
    return fps;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::CRC32::Init()
{
    if(HAL_CRC_Init(&handleCRC) != HAL_OK)
    {
        ERROR_HANDLER();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint CPU::CRC32::Calculate(void *data, uint size)
{
    uint sizeBuffer = size;
    while(sizeBuffer % 4)                           // Увеличиваем до ближайшего кратного четырём
    {
        ++sizeBuffer;
    }

    uint result = 0;

    uint *buffer = (uint *)std::malloc(sizeBuffer);      // Выделяем память для нового буфера

    if (buffer)
    {
        std::memcpy(buffer, data, size);                     // Копируем данные в новый буфер

        for (uint i = size; i < sizeBuffer; i++)          // Заполняем оставшееся место нулями
        {
            buffer[i] = 0;
        }

        result = HAL_CRC_Calculate(&handleCRC, buffer, sizeBuffer / 4);
    }

    std::free(buffer);

    return result;
}
