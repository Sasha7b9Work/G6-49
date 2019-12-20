#include "AD9952.h"
#include "defines.h"
#include "GeneratroSettings.h"
#include "Hardware/CPU.h"
#include "FPGA.h"
#include "Settings/CalibrationSettings.h"
#include "Utils/Math.h"
#include <cmath>


AD9952::ClockFrequency::E AD9952::clock = ClockFrequency::_100MHz;
    

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

bool                            AD9952::Manipulation::enabled[Chan::Count] = {false, false};



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


void AD9952::Manipulation::SetEnabled(Chan::E ch, bool enable)
{
    enabled[ch] = enable;
    WriteCFR1(ch);

    FPGA::SetWaveForm(ch, Form::Sine);
}


void AD9952::SetFrequency(Chan::E ch, ParamValue frequency)
{
    float freq = frequency.ToFloat();

    FPGA::SetClockAD992(freq < 0.1F ? FPGA::ClockFrequency::_1MHz : FPGA::ClockFrequency::_100MHz);

    setDDS.ad9952[ch].frequency = frequency.ToFloat();
    WriteRegister(ch, Register::FTW0);
}


void AD9952::SetPhase(Chan::E ch, ParamValue phase)
{
    setDDS.ad9952[ch].phase = phase.ToFloat();
    if(setDDS.ad9952[Chan::A].frequency == setDDS.ad9952[Chan::B].frequency) //-V550 //-V2550
    {
        WriteRegister(ch, Register::POW);
    }
}


void AD9952::SetAmplitude(Chan::E ch, ParamValue amplitude)
{
    setDDS.ad9952[ch].amplitude = amplitude.ToFloat() * 0.8F * (1.0F + CAL_AD9952_AMPLITUDE(ch) / 1000.0F);    // 0.8f в этой формуле оттуда, что схема устройства настроена на то, что 100% максимальной
                                                                                                    // амплитуды сигнала на выходе получаются при засылке 80% кода от максимального
    WriteRegister(ch, Register::ASF);
}


void AD9952::WriteRegister(Chan::E ch, Register reg)
{
    typedef void(*pFuncVCh)(Chan::E);

    static const pFuncVCh func[] = {WriteCFR1, WriteCFR2, WriteASF, WriteARR, WriteFTW0, WritePOW};

    pFuncVCh f = func[reg];

    if (f)
    {
        f(ch);
    }
}


void AD9952::WriteCFR1(Chan::E ch)
{
    uint value = 0;

    if(ch == Chan::B)
    {
        Bit::Set(value, 1);
        Bit::Set(value, 23);
    }
    Bit::Set(value, 9);       // Однонаправленный режим
    Bit::Set(value, 13);
    if(Manipulation::enabled[ch])
    {
        Bit::Set(value, 24);  // Устанавливаем режим манипуляции
    }
    Bit::Set(value, 25);      // OSK enable - управление амплитудой
    Bit::Set(value, 26);

    WriteToHardware(ch, Register::CFR1, value);
}


void AD9952::WriteCFR2(Chan::E ch)
{
    uint value = 0;
    Bit::Set(value, 3);
    WriteToHardware(ch, Register::CFR2, value);
}


void AD9952::WritePOW(Chan::E ch)
{
    uint value = static_cast<uint>(setDDS.ad9952[Chan::B].phase / 360.0F * (1 << 13) + 0.5F);
    WriteToHardware(ch, Register::POW, value * 2);
}


void AD9952::WriteASF(Chan::E ch)
{
    uint value = ((static_cast<uint>((setDDS.ad9952[ch].amplitude / 5.0F) * ((1 << 7) - 1))) << 7) / 2;
    Bit::Set(value, 14);  // \ Это биты множителя скорости
    Bit::Set(value, 15);  // / нарастания фронта 
    WriteToHardware(ch, Register::ASF, value);
}


void AD9952::WriteFTW0(Chan::E ch)
{
    float FTWf = (setDDS.ad9952[ch].frequency / (FPGA::clock == FPGA::ClockFrequency::_100MHz ? 1e8F : 1e6F)) * std::powf(2, 32);

    WriteToHardware(ch, Register::FTW0, static_cast<uint>(FTWf + 0.5F));
}


void AD9952::WriteARR(Chan::E ch)
{
    WriteToHardware(ch, Register::ARR, 1);
}


pString AD9952::Register::Name() const
{
    static const pString names[Number] =
    {
        "CFR1",
        "CFR2",
        "ASF",
        "ARR",
        "FTW0",
        "POW"
    };

    return names[value];
}


void AD9952::WriteToHardware(Chan::E ch, Register reg, uint value)
{
    static const int numBytes[] =               // Число байт данных для передачи
    { //CFR1 CFR2 ASF ARR FTW0 POW
         4,   3,   2,  1,  4,   2
    };

    uint8 buffer[5];
    buffer[0] = static_cast<uint8>(reg);
    int pointer = 1;                    // В это место буфера будем записывать каждый следующий байт

    int curByte = numBytes[reg] - 1;    // Здесь будем перебирать байты value от старшего к младшему
    while (curByte >= 0)
    {
        buffer[pointer++] = static_cast<uint8>(value >> (curByte * 8));
        --curByte;
    }
    
    CPU::WritePin(ChipSelect(ch), false);

    HAL_SPI_Transmit(&hSPI3, buffer, static_cast<uint16>(numBytes[reg] + 1), 1);
    
    CPU::WritePin(GeneratorWritePin::AD9952_IO_UPD, true);
    volatile int i = 0;
    for (; i < 1000; ++i)
    {
    };
    CPU::WritePin(GeneratorWritePin::AD9952_IO_UPD, false);

    CPU::WritePin(ChipSelect(ch), true);
}


GeneratorWritePin AD9952::ChipSelect(Chan::E ch)
{
    return static_cast<GeneratorWritePin::E>(ch == Chan::A ? GeneratorWritePin::AD9952_SPI3_CSA : GeneratorWritePin::AD9952_SPI3_CSB);
}


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
