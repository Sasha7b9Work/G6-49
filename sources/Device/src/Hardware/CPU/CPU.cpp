#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include <stm32f4xx.h>
#include "CPU.h"
#include <stdlib.h>
#include <string.h>
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static CRC_HandleTypeDef handleCRC = {CRC};

SPI_HandleTypeDef CPU::SPI1_::handle =                                   // Для связи с панелью
{
    SPI1,
    {
        SPI_MODE_SLAVE,                 // Init.Mode
        SPI_DIRECTION_2LINES,           // Init.Direction
        SPI_DATASIZE_8BIT,              // Init.DataSize
        SPI_POLARITY_HIGH,              // Init.CLKPolarity
        SPI_PHASE_2EDGE,                // Init.CLKPhase
        SPI_NSS_SOFT,                   // Init.NSS
        SPI_BAUDRATEPRESCALER_64,       // Init.BaudRatePrescaler
        SPI_FIRSTBIT_MSB,               // Init.FirstBit
        SPI_TIMODE_DISABLED,            // Init.TIMode
        SPI_CRCCALCULATION_DISABLED,    // Init.CRCCalculation
        7                               // InitCRCPolynomial
    },
    0, 0, 0, 0, 0, 0,
    0,
    0,
    0, 0, HAL_UNLOCKED, HAL_SPI_STATE_RESET, 0
};

struct StructPort
{
    GPIO_TypeDef *port;
    uint16        pin;
};

static const StructPort registers[GeneratorWritePin::Number] =
{
    {GPIOF, GPIO_PIN_6},    // AD9952_SPI3_CSA
    {GPIOF, GPIO_PIN_7},    // AD9952_SPI3_CSB
    {GPIOC, GPIO_PIN_7},    // AD9952_IO_UPD
    {GPIOF, GPIO_PIN_8},    // AD9952_IOSYNA
    {GPIOF, GPIO_PIN_9},    // AD9952_IOSYNB
    {GPIOC, GPIO_PIN_8},    // AD9952_RES_DDS
    {GPIOE, GPIO_PIN_12},   // Pin_P1_AmplifierA
    {GPIOE, GPIO_PIN_13},   // Pin_P2_AmplifierB
    {GPIOE, GPIO_PIN_12},   // Pin_P3_OutA
    {GPIOE, GPIO_PIN_13},   // Pin_P4_OutB
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
    {GPIOE, GPIO_PIN_14},   // FREQ_METER_RESIST
    {GPIOE, GPIO_PIN_15},   // FREQ_METER_COUPLE
    {GPIOB, GPIO_PIN_10},   // FREQ_METER_FILTR
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

static const StructPort registersRead[GeneratorReadPin::Number] =
{
    {GPIOC, GPIO_PIN_0},    // FREQ_METER_DRY
    {GPIOB, GPIO_PIN_12}    // FREQ_METER_DATA
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPU::Init()
{
    STM407::Init();

    EnablePeriphery();

    InitGPIOS();

    InitPins();

    WritePin(GeneratorWritePin::Pin_P3_OutA, false);
    WritePin(GeneratorWritePin::Pin_P4_OutB, false);

    SPI1_::Init();

    CRC32::Init();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
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

//----------------------------------------------------------------------------------------------------------------------------------------------------
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

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::SetBusy()
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::SetReady()
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
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

    for (int i = 0; i < GeneratorWritePin::Number; ++i)
    {
        isGPIO.Pin = registers[i].pin;
        HAL_GPIO_Init(registers[i].port, &isGPIO);
        HAL_GPIO_WritePin(registers[i].port, registers[i].pin, GPIO_PIN_RESET);
    }

    isGPIO.Mode = GPIO_MODE_INPUT;
    isGPIO.Pull = GPIO_PULLDOWN;

    for(int i = 0; i < GeneratorReadPin::Number; ++i)
    {
        isGPIO.Pin = registersRead[i].pin;
        HAL_GPIO_Init(registersRead[i].port, &isGPIO);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::WritePin(GeneratorWritePin pin, bool set)
{
    HAL_GPIO_WritePin(registers[pin].port, registers[pin].pin, set ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool CPU::ReadPin(GeneratorReadPin pin)
{
    GPIO_PinState state = HAL_GPIO_ReadPin(registersRead[pin].port, registersRead[pin].pin);

    return state == GPIO_PIN_SET;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::SPI1_::Init()
{
    GPIO_InitTypeDef isGPIOA =
    {   //  SCK         MI           MO
        GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,
        GPIO_MODE_AF_PP,
        GPIO_NOPULL,
        GPIO_SPEED_HIGH,
        GPIO_AF5_SPI1
    };
    HAL_GPIO_Init(GPIOA, &isGPIOA);

    HAL_SPI_Init(&handle);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::SPI1_::Receive(void *buffer, uint size)
{
    CPU::SetReady();
    HAL_SPI_Receive(&handle, (uint8 *)buffer, (uint16)size, 100);
    CPU::SetBusy();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::SPI1_::Transmit(void *buffer, uint size)
{
    CPU::SetReady();
    HAL_SPI_Transmit(&handle, (uint8 *)buffer, (uint16)size, 66);
    CPU::SetBusy();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::CRC32::Init()
{
    if (HAL_CRC_Init(&handleCRC) != HAL_OK)
    {
        ERROR_HANDLER();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint CPU::CRC32::Calculate(uint8 *data, uint size)
{
    uint sizeBuffer = size;
    while (sizeBuffer % 4)                           // Увеличиваем до ближайшего кратного четырём
    {
        ++sizeBuffer;
    }

    uint *buffer = (uint *)malloc(sizeBuffer);      // Выделяем память для нового буфера

    memcpy(buffer, data, size);                     // Копируем данные в новый буфер

    for (uint i = size; i < sizeBuffer; i++)          // Заполняем оставшееся место нулями
    {
        buffer[i] = 0;
    }

    uint result = HAL_CRC_Calculate(&handleCRC, buffer, sizeBuffer / 4);

    free(buffer);

    return result;
}
