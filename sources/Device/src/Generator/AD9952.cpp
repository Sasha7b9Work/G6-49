#include "AD9952.h"
#include "defines.h"
#include "GeneratroSettings.h"
#include "Hardware/CPU.h"
#include "Utils/Console.h"
#include "FPGA.h"
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

bool AD9952::Manipulation::enabled[Chan::Number] = {false, false};


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

    WriteARR(Chan::A);      // Здесь скорость нарастания фронта импульса при манипуляции
    WriteARR(Chan::B);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void AD9952::Manipulation::SetEnabled(Chan ch, bool enable)
{
    enabled[ch] = enable;
    WriteCFR1(ch);

    FPGA::SetWaveForm(ch, Form::Sine);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void AD9952::SetFrequency(Chan ch, float frequency)
{
    setDDS.ad9952[ch].frequency = frequency;
    WriteRegister(ch, Register::FTW0);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void AD9952::SetPhase(Chan ch, float phase)
{
    setDDS.ad9952[ch].phase = phase;
    if(setDDS.ad9952[Chan::A].frequency == setDDS.ad9952[Chan::B].frequency)
    {
        WriteRegister(ch, Register::POW);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void AD9952::SetAmplitude(Chan ch, float amplitude)
{
    setDDS.ad9952[ch].amplitude = amplitude;
    WriteRegister(ch, Register::ASF);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void AD9952::WriteRegister(Chan ch, Register reg)
{
    typedef void(*pFuncVCh)(Chan);

    static const pFuncVCh func[] = {WriteCFR1, WriteCFR2, WriteASF, WriteARR, WriteFTW0, WritePOW};

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

    if(ch == Chan::B)
    {
        SetBit(value, 1);
        SetBit(value, 23);
    }
    SetBit(value, 9);       // Однонаправленный режим
    SetBit(value, 13);
    if(Manipulation::enabled[ch])
    {
        SetBit(value, 24);  // Устанавливаем режим манипуляции
    }
    SetBit(value, 25);      // OSK enable - управление амплитудой
    SetBit(value, 26);

    WriteToHardware(ch, Register::CFR1, value);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void AD9952::WriteCFR2(Chan ch)
{
    uint value = 0;
    SetBit(value, 3);
    WriteToHardware(ch, Register::CFR2, value);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void AD9952::WritePOW(Chan ch)
{
    uint value = (uint)(setDDS.ad9952[Chan::B].phase / 360.0f * (1 << 13) + 0.5f);
    WriteToHardware(ch, Register::POW, value * 2);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void AD9952::WriteASF(Chan ch)
{
    uint value = (((uint)((setDDS.ad9952[ch].amplitude / 5.0f) * ((1 << 7) - 1))) << 7) / 2;
    SetBit(value, 14);  // \ Это биты множителя скорости
    SetBit(value, 15);  // / нарастания фронта 
    WriteToHardware(ch, Register::ASF, value);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void AD9952::WriteFTW0(Chan ch)
{
    float FTWf = (setDDS.ad9952[ch].frequency / 1e8f) * powf(2, 32);

    WriteToHardware(ch, Register::FTW0, (uint)(FTWf + 0.5f));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void AD9952::WriteARR(Chan ch)
{
    WriteToHardware(ch, Register::ARR, 1);
}



//----------------------------------------------------------------------------------------------------------------------------------------------------
void AD9952::WriteToHardware(Chan ch, Register reg, uint value)
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
    
    CPU::WritePin(GeneratorWritePin::AD9952_IO_UPD, true);
    volatile int i = 0;
    for (; i < 1000; ++i)
    {
    };
    CPU::WritePin(GeneratorWritePin::AD9952_IO_UPD, false);

    CPU::WritePin(ChipSelect(ch), true);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
GeneratorWritePin AD9952::ChipSelect(Chan ch)
{
    return (GeneratorWritePin::E)(ch == Chan::A ? GeneratorWritePin::AD9952_SPI3_CSA : GeneratorWritePin::AD9952_SPI3_CSB);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void AD9952::Reset()
{
    CPU::WritePin(GeneratorWritePin::AD9952_RES_DDS, false);
    CPU::WritePin(GeneratorWritePin::AD9952_RES_DDS, true);
    CPU::WritePin(GeneratorWritePin::AD9952_RES_DDS, false);

    CPU::WritePin(GeneratorWritePin::AD9952_SPI3_CSA, true);
    CPU::WritePin(GeneratorWritePin::AD9952_SPI3_CSB, true);
    CPU::WritePin(GeneratorWritePin::AD9952_IO_UPD, false);
    CPU::WritePin(GeneratorWritePin::AD9952_IOSYNA, false);
    CPU::WritePin(GeneratorWritePin::AD9952_IOSYNB, false);

    CPU::WritePin(GeneratorWritePin::AD9952_RES_DDS, false);
    CPU::WritePin(GeneratorWritePin::AD9952_RES_DDS, true);
    CPU::WritePin(GeneratorWritePin::AD9952_RES_DDS, false);
}
