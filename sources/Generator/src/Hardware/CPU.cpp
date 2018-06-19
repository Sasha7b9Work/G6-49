#include "defines.h"
#include <stm32f4xx.h>
#include "CPU.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct StructPort
{
    GPIO_TypeDef *port;
    uint16        pin;
};

static const StructPort registers[NumPins] =
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
    {GPIOC, GPIO_PIN_2},    // AD5697_LDACA
    {GPIOC, GPIO_PIN_3},    // AD5697_LDACB
    {GPIOC, GPIO_PIN_5},    // AD5697_D_RSA
    {GPIOB, GPIO_PIN_0},    // AD5697_D_RSB
    {GPIOD, GPIO_PIN_11},   // FPGA_WR_RG
    {GPIOD, GPIO_PIN_12},   // FPGA_CLK_RG
    {GPIOD, GPIO_PIN_13},   // FPGA_DT_RG
    {GPIOG, GPIO_PIN_5},    // FPGA_A0_RG
    {GPIOG, GPIO_PIN_6},    // FPGA_A1_RG
    {GPIOG, GPIO_PIN_7},    // FPGA_A2_RG
    {GPIOG, GPIO_PIN_8}     // FPGA_A3_RG
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPU::Init()
{
    STM407::Init();

    EnablePeriphery();

    InitGPIOS();

    InitPins();

    WritePin(Pin_P3_OutA, false);
    WritePin(Pin_P4_OutB, false);
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

    for (int i = 0; i < NumPins; ++i)
    {
        isGPIO.Pin = registers[i].pin;
        HAL_GPIO_Init(registers[i].port, &isGPIO);
        HAL_GPIO_WritePin(registers[i].port, registers[i].pin, GPIO_PIN_RESET);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::WritePin(GeneratorPin pin, bool set)
{
    HAL_GPIO_WritePin(registers[pin].port, registers[pin].pin, set ? GPIO_PIN_SET : GPIO_PIN_RESET);
}
