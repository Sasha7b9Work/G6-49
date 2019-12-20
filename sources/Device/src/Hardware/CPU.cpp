#include "defines.h"
#include <stm32f4xx.h>
#include "CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include <cstdlib>
#include <cstring>


static void SystemClockConfig();


static CRC_HandleTypeDef handleCRC = {CRC};

struct StructPort
{
    GPIO_TypeDef *port;
    uint16        pin;
};

static const StructPort registers[GeneratorWritePin::Count] =
{
    {GPIOF, GPIO_PIN_6},    // AD9952_SPI3_CSA
    {GPIOF, GPIO_PIN_7},    // AD9952_SPI3_CSB
    {GPIOC, GPIO_PIN_7},    // AD9952_IO_UPD
    {GPIOF, GPIO_PIN_8},    // AD9952_IOSYNA
    {GPIOF, GPIO_PIN_9},    // AD9952_IOSYNB
    {GPIOC, GPIO_PIN_8},    // AD9952_RES_DDS
    {GPIOE, GPIO_PIN_12},   // Pin_P1_AmplifierA
    {GPIOE, GPIO_PIN_13},   // Pin_P2_AmplifierB
    {GPIOF, GPIO_PIN_1},    // Pin_OutA
    {GPIOC, GPIO_PIN_15},   // Pin_OutB
    {GPIOC, GPIO_PIN_2},    // AD5697_Offset
    {GPIOC, GPIO_PIN_3},    // AD5697_Freq
    {GPIOC, GPIO_PIN_5},    // AD5697_D_RSA
    {GPIOB, GPIO_PIN_0},    // AD5697_D_RSB
    {GPIOD, GPIO_PIN_11},   // FPGA_WR_RG
    {GPIOD, GPIO_PIN_12},   // FPGA_CLK_RG
    {GPIOD, GPIO_PIN_13},   // FPGA_DT_RG
    {GPIOG, GPIO_PIN_5},    // FPGA_A0_RG
    {GPIOG, GPIO_PIN_6},    // FPGA_A1_RG
    {GPIOG, GPIO_PIN_7},    // FPGA_A2_RG
    {GPIOG, GPIO_PIN_8},    // FPGA_A3_RG
    {GPIOG, GPIO_PIN_12},   // FREQ_METER_RESIST
    {GPIOG, GPIO_PIN_13},   // FREQ_METER_COUPLE
    {GPIOG, GPIO_PIN_14},   // FREQ_METER_FILTR
    {GPIOC, GPIO_PIN_1},    // FREQ_METER_CLK
    {GPIOD, GPIO_PIN_14},   // D0
    {GPIOD, GPIO_PIN_15},   // D1
    {GPIOD, GPIO_PIN_0},    // D2
    {GPIOD, GPIO_PIN_1},    // D3
    {GPIOE, GPIO_PIN_7},    // D4
    {GPIOE, GPIO_PIN_8},    // D5
    {GPIOE, GPIO_PIN_9},    // D6
    {GPIOE, GPIO_PIN_10},   // D7
    {GPIOD, GPIO_PIN_5}     // FPGA_WR_DATA
};

static const StructPort registersRead[GeneratorReadPin::Count] =
{
    {GPIOC, GPIO_PIN_0},    // FREQ_METER_DRY
    {GPIOB, GPIO_PIN_12}    // FREQ_METER_DATA
};

/// Время последней установки состояния "занято"
static uint timeBusy = 0;


void CPU::Init()
{
    SystemClockConfig();

    HAL_Init();

    EnablePeriphery();

    InitGPIOS();

    InitPins();

    WritePin(GeneratorWritePin::Pin_OutA, true);
    WritePin(GeneratorWritePin::Pin_OutB, true);

    SPI1_::Init();

    CRC32::Init();
}


void CPU::EnablePeriphery()
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();

    __HAL_RCC_FSMC_CLK_ENABLE();
    __HAL_RCC_I2C1_CLK_ENABLE();
    __HAL_RCC_SPI1_CLK_ENABLE();
    __HAL_RCC_SPI3_CLK_ENABLE();    // AD9952
}


void CPU::InitGPIOS()
{
    GPIO_InitTypeDef isGPIO;

    // На этом пине будем выставлять занятость генератора
    isGPIO.Pin = GPIO_PIN_2;
    isGPIO.Mode = GPIO_MODE_OUTPUT_PP;
    isGPIO.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &isGPIO);

    SetBusy();

    isGPIO.Pin = GPIO_PIN_12 | GPIO_PIN_14 | GPIO_PIN_15;
    isGPIO.Mode = GPIO_MODE_AF_PP;
    isGPIO.Pull = GPIO_NOPULL;
    isGPIO.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    isGPIO.Alternate = GPIO_AF12_OTG_HS_FS;
    HAL_GPIO_Init(GPIOB, &isGPIO);

    isGPIO.Pin = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
    isGPIO.Mode = GPIO_MODE_AF_PP;
    isGPIO.Pull = GPIO_NOPULL;
    isGPIO.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    isGPIO.Alternate = GPIO_AF10_OTG_FS;
    HAL_GPIO_Init(GPIOA, &isGPIO);
}


void CPU::SetBusy()
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);

    timeBusy = TIME_MS;
}


void CPU::SetReady()
{
    while (TIME_MS - timeBusy < 2)
    {
    };

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
}


void CPU::InitPins()
{
    GPIO_InitTypeDef isGPIO =
    {
        GPIO_PIN_0,
        GPIO_MODE_OUTPUT_PP,
        GPIO_NOPULL,
        GPIO_SPEED_FREQ_HIGH,
        0
    };

    for (int i = 0; i < GeneratorWritePin::Count; ++i)
    {
        isGPIO.Pin = registers[i].pin;
        HAL_GPIO_Init(registers[i].port, &isGPIO);
        HAL_GPIO_WritePin(registers[i].port, registers[i].pin, GPIO_PIN_RESET);
    }

    isGPIO.Mode = GPIO_MODE_INPUT;
    isGPIO.Pull = GPIO_PULLDOWN;

    for(int i = 0; i < GeneratorReadPin::Count; ++i)
    {
        isGPIO.Pin = registersRead[i].pin;
        HAL_GPIO_Init(registersRead[i].port, &isGPIO);
    }
}


void CPU::WritePin(GeneratorWritePin pin, bool st)
{
    HAL_GPIO_WritePin(registers[pin].port, registers[pin].pin, st ? GPIO_PIN_SET : GPIO_PIN_RESET);
}


bool CPU::ReadPin(GeneratorReadPin pin)
{
    GPIO_PinState state = HAL_GPIO_ReadPin(registersRead[pin].port, registersRead[pin].pin);

    return state == GPIO_PIN_SET;
}


void CPU::CRC32::Init()
{
    if (HAL_CRC_Init(&handleCRC) != HAL_OK)
    {
        ERROR_HANDLER();
    }
}


uint CPU::CRC32::Calculate(const uint8 *data, uint size)
{
    uint sizeBuffer = size;
    while (sizeBuffer % 4)                           // Увеличиваем до ближайшего кратного четырём
    {
        ++sizeBuffer;
    }

    uint *buffer = static_cast<uint *>(std::malloc(sizeBuffer));      // Выделяем память для нового буфера

    uint result = 0U;

    if (buffer)
    {
        std::memcpy(buffer, data, size);                     // Копируем данные в новый буфер

        for (uint i = size; i < sizeBuffer; i++)          // Заполняем оставшееся место нулями
        {
            buffer[i] = 0;
        }

        result = HAL_CRC_Calculate(&handleCRC, buffer, sizeBuffer / 4);

        std::free(buffer);
    }

    return result;
}


void HAL_MspInit()
{
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

    /* System interrupt init*/
    /* MemoryManagement_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
    /* BusFault_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
    /* UsageFault_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
    /* SVCall_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SVCall_IRQn, 0, 0);
    /* DebugMonitor_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DebugMonitor_IRQn, 0, 0);
    /* PendSV_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(PendSV_IRQn, 0, 0);
    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}


static void SystemClockConfig()
{
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;

    __HAL_RCC_PWR_CLK_ENABLE();

    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 25;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
        | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
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
}


void CPU::ErrorHandler()
{
    while (true)
    {

    }
}
