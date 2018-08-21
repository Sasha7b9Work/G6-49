#include "AD9952.h"
#include "defines.h"
#include "GeneratroSettings.h"
#include "Hardware/CPU.h"
#include <math.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static SPI_HandleTypeDef hSPI3 =
{
    SPI3,
    {
        SPI_MODE_MASTER,
        SPI_DIRECTION_1LINE,
        SPI_DATASIZE_8BIT,
        SPI_POLARITY_LOW,
        SPI_PHASE_1EDGE,
        SPI_NSS_SOFT,
        SPI_BAUDRATEPRESCALER_2,
        SPI_FIRSTBIT_MSB,
        SPI_TIMODE_DISABLE,
        SPI_CRCCALCULATION_DISABLE,
        10
    },
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, HAL_UNLOCKED, HAL_SPI_STATE_RESET, 0
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void WriteToHardware(Chan ch, uint8 reg, uint value);
static GeneratorWritePin ChipSelect(Chan ch);
static void Reset();


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AD9952::Init()
{
    GPIO_InitTypeDef isGPIO = 
    {   //   SCK          MI           MO
        GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12,
        GPIO_MODE_AF_PP,
        GPIO_NOPULL,
        GPIO_SPEED_FREQ_MEDIUM,
        GPIO_AF6_SPI3
    };

    HAL_GPIO_Init(GPIOC, &isGPIO);

    HAL_SPI_Init(&hSPI3);

    Reset();

    WriteCFR1(Chan::A);
    WriteCFR1(Chan::B);

    WriteCFR2(Chan::A);
    WriteCFR2(Chan::B);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void AD9952::SetFrequency(Chan ch, float frequency)
{
    setDDS.ad9952[ch].frequency = frequency;
    WriteRegister(ch, AD9952::FTW0);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void AD9952::SetAmplitude(Chan ch, float amplitude)
{
    setDDS.ad9952[ch].amplitude = amplitude;
    WriteRegister(ch, AD9952::ASF);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void AD9952::WriteRegister(Chan ch, uint8 reg)
{
    typedef void(*pFuncVCh)(Chan);

    static const pFuncVCh func[] = {WriteCFR1, WriteCFR2, WriteASF, 0, WriteFTW0, 0};

    pFuncVCh f = func[reg];

    if (f)
    {
        f(ch);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void AD9952::WriteCFR1(Chan ch)
{
    uint value = 0;
    if(ch.IsB())
    {
        SetBit(value, 1);
        SetBit(value, 23);
    }
    SetBit(value, 9);       // Однонаправленный режим
    SetBit(value, 13);
    SetBit(value, 25);      // OSK enable - управление амплитудой
    WriteToHardware(ch, AD9952::CFR1, value);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void AD9952::WriteCFR2(Chan ch)
{
    uint value = 0;
    SetBit(value, 3);
    WriteToHardware(ch, AD9952::CFR2, value);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void AD9952::WriteASF(Chan ch)
{
    uint value = ((uint)((setDDS.ad9952[ch].amplitude / 5.0f) * ((1 << 7) - 1))) << 7;

    WriteToHardware(ch, AD9952::ASF, value);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void AD9952::WriteFTW0(Chan ch)
{
    float FTWf = (setDDS.ad9952[ch].frequency / 1e8f) * powf(2, 32);

    WriteToHardware(ch, AD9952::FTW0, (uint)(FTWf + 0.5f));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void WriteToHardware(Chan ch, uint8 reg, uint value)
{
    static const int numBytes[] =               // Число байт данных для передачи
    { //CFR1 CFR2 ASF ARR FTW0 POW
         4,   3,   2,  1,  4,   2
    };

    uint8 buffer[5];
    buffer[0] = (uint8)reg;
    int pointer = 1;                    // В это место буфера будем записывать каждый следующий байт

    int curByte = numBytes[reg] - 1;    // Здесь будем перебирать байты value от старшего к младшему
    while (curByte >= 0)
    {
        buffer[pointer++] = (uint8)(value >> (curByte * 8));
        --curByte;
    }
    
    CPU::WritePin(ChipSelect(ch), false);

    HAL_SPI_Transmit(&hSPI3, buffer, (uint16)(numBytes[reg] + 1), 1);
    
    CPU::WritePin(AD9952_IO_UPD, true);
    volatile int i = 0;
    for (; i < 1000; ++i)
    {
    };
    CPU::WritePin(AD9952_IO_UPD, false);

    CPU::WritePin(ChipSelect(ch), true);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static GeneratorWritePin ChipSelect(Chan ch)
{
    return ch.IsA() ? AD9952_SPI3_CSA : AD9952_SPI3_CSB;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void Reset()
{
    CPU::WritePin(AD9952_RES_DDS, false);
    CPU::WritePin(AD9952_RES_DDS, true);
    CPU::WritePin(AD9952_RES_DDS, false);

    CPU::WritePin(AD9952_SPI3_CSA, true);
    CPU::WritePin(AD9952_SPI3_CSB, true);
    CPU::WritePin(AD9952_IO_UPD, false);
    CPU::WritePin(AD9952_IOSYNA, false);
    CPU::WritePin(AD9952_IOSYNB, false);

    CPU::WritePin(AD9952_RES_DDS, false);
    CPU::WritePin(AD9952_RES_DDS, true);
    CPU::WritePin(AD9952_RES_DDS, false);
}
