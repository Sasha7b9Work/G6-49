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
//    {GPIOC, GPIO_PIN_7},    // AD9952_IO_UPD
//    {GPIOF, GPIO_PIN_8},    // AD9952_IOSYNA
//    {GPIOF, GPIO_PIN_9},    // AD9952_IOSYNB
//    {GPIOC, GPIO_PIN_8},    // AD9952_RES_DDS
};


/// ¬рем€ последней установки состо€ни€ "зан€то"
static uint timeBusy = 0;


void CPU::Init()
{
    HAL::Init();

    InitGPIOS();

    HAL_PIO::Set(WR_OUT_A);
    HAL_PIO::Set(WR_OUT_B);
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
