#include "defines.h"
#include <stm32f4xx.h>
#include "CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/HAL/HAL_PIO.h"
#include <cstdlib>
#include <cstring>


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

/// ¬рем€ последней установки состо€ни€ "зан€то"
static uint timeBusy = 0;


void CPU::Init()
{
    HAL::Init();

    InitGPIOS();


    HAL_PIO::Init();

    WritePin(GeneratorWritePin::Pin_OutA, true);
    WritePin(GeneratorWritePin::Pin_OutB, true);
}


void CPU::InitGPIOS()
{
    GPIO_InitTypeDef isGPIO;

    // Ќа этом пине будем выставл€ть зан€тость генератора
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


void CPU::WritePin(GeneratorWritePin pin, bool st)
{
    HAL_GPIO_WritePin(registers[pin].port, registers[pin].pin, st ? GPIO_PIN_SET : GPIO_PIN_RESET);
}


bool CPU::ReadPin(GeneratorReadPin pin)
{
    GPIO_PinState state = HAL_GPIO_ReadPin(registersRead[pin].port, registersRead[pin].pin);

    return state == GPIO_PIN_SET;
}
