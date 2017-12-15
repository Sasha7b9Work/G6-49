#include "defines.h"
#include "Common.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
    GPIO_TypeDef *port;
    uint16        pin;
} StructPort;

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
    {GPIOE, GPIO_PIN_14},   // Pin_P3_OutA
    {GPIOE, GPIO_PIN_15},   // Pin_P4_OutB
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
void InitPins(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();

    GPIO_InitTypeDef isGPIO =
    {
        GPIO_PIN_0,
        GPIO_MODE_OUTPUT_PP,
        GPIO_NOPULL,
        GPIO_SPEED_FREQ_HIGH
    };

    for (int i = 0; i < NumPins; ++i)
    {
        isGPIO.Pin = registers[i].pin;
        HAL_GPIO_Init(registers[i].port, &isGPIO);
        HAL_GPIO_WritePin(registers[i].port, registers[i].pin, GPIO_PIN_RESET);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void WritePin(GeneratorPin pin, bool set)
{
    HAL_GPIO_WritePin(registers[pin].port, registers[pin].pin, set ? GPIO_PIN_SET : GPIO_PIN_RESET);
}
