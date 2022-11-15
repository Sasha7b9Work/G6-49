// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Generator/AD9952.h"
#include "Generator/Calibrator.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Math.h"
#include "Generator/FPGA/Clock.h"
#include <cmath>


bool AD9952::Manipulation::enabled[Chan::Count] = { false, false };


namespace AD9952
{
    struct Register
    {
        enum E
        {
            CFR1,
            CFR2,
            ASF,
            ARR,
            FTW0,
            POW,
            Number
        } value;
        Register(E v) : value(v) { };
        operator uint8() const { return (uint8)value; };
        cstr Name() const;
        bool Is(E v) const { return value == v; };
    };

    static void WriteToHardware(const Chan &, Register::E reg, uint value);
    static StructPIN ChipSelect(const Chan &);
    static void Reset();
    static void WriteRegister(const Chan &, Register::E reg);
    static void WriteCFR1(const Chan &);
    static void WriteCFR2(const Chan &);
    static void WriteARR(const Chan &);
    static void WriteASF(const Chan &);
    static void WriteFTW0(const Chan &);
    static void WritePOW(const Chan &);

    static double phase[Chan::Count] = { 0.0, 0.0 };

    // Последнее записанное в ASF регистр значение. Используется для того, чтобы меандр при установленной амплитуде синусоиды "0" существовал
    volatile uint lastASF[Chan::Count] = { 100, 100 };
}


void AD9952::Init()
{
    HAL_SPI3::Init();

    Reset();

    WriteCFR1(ChA);
    WriteCFR1(ChB);

    WriteCFR2(ChA);
    WriteCFR2(ChB);

    WriteARR(ChA);      // Здесь скорость нарастания фронта импульса при манипуляции
    WriteARR(ChB);
}


void AD9952::Manipulation::SetEnabled(const Chan &ch, bool enable)
{
    enabled[ch] = enable;
    WriteCFR1(ch);

    FPGA::SetWaveForm(ch, TypeForm::Sine);
}


void AD9952::SetFrequency(const Chan &ch)
{
    FPGA::Clock::AD992::Set(SettingsGenerator::Frequency(ch) < 0.2F ? FPGA::Clock::AD992::_1MHz : FPGA::Clock::AD992::_100MHz);

    WriteRegister(ch, Register::FTW0);

    WriteRegister(ch, Register::CFR2);
}


void AD9952::SetPhase(const Chan &ch, Value ph)
{
    phase[ch] = ph.ToDouble();

    if(SettingsGenerator::Frequency(ChA) == SettingsGenerator::Frequency(ChB)) //-V550 //-V2550
    {
        WriteRegister(ch, Register::POW);
    }
}


void AD9952::SetAmplitude(const Chan &ch)
{
    WriteRegister(ch, Register::ASF);
}


void AD9952::WriteRegister(const Chan &ch, Register::E reg)
{
    typedef void(*pFuncVCh)(const Chan &);

    static const pFuncVCh func[] = {WriteCFR1, WriteCFR2, WriteASF, WriteARR, WriteFTW0, WritePOW};

    pFuncVCh f = func[reg];

    if (f)
    {
        f(ch);
    }
}


void AD9952::WriteCFR1(const Chan &ch)
{
    uint value = 0;

    if(ch.IsB())
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


void AD9952::WriteCFR2(const Chan &ch)
{
    uint value = 0;
    if (FPGA::Clock::AD992::Is100MHz())
    {
        Bit::Set(value, 4);
        Bit::Set(value, 6);
    }
    WriteToHardware(ch, Register::CFR2, value);
}


void AD9952::WritePOW(const Chan &ch)
{
    uint value = (uint)(phase[ch] / 360.0F * (float)(1 << 13) + 0.5F);
    WriteToHardware(ch, Register::POW, value * 2);
}


void AD9952::WriteASF(const Chan &ch)
{
    float k = Calibrator::GetAmplitudeK(ch);

    double att = 1.0 / Amplifier::GetAmplification(ch);

    double amplitude = k * att * SettingsGenerator::Amplitude(ch);

    uint value = (uint)(amplitude * (float)0x3FFF);

    Bit::Set(value, 14);  // \ Это биты множителя скорости
    Bit::Set(value, 15);  // / нарастания фронта 
    WriteToHardware(ch, Register::ASF, value);
}


void AD9952::SetAmplitudeForMeander(const Chan &ch)
{
    uint value = 0x1FFF;
    Bit::Set(value, 14);
    Bit::Set(value, 15);
    WriteToHardware(ch, Register::ASF, value);
}


void AD9952::WriteFTW0(const Chan &ch)
{
    double FTWf = (SettingsGenerator::Frequency(ch) / (FPGA::Clock::AD992::Is100MHz() ? 1e8F : 1e6F)) * std::powf(2.0F, 32.0F);

    WriteToHardware(ch, Register::FTW0, (uint)(FTWf + 0.5F));
}


void AD9952::WriteARR(const Chan &ch)
{
    WriteToHardware(ch, Register::ARR, 1);
}


cstr AD9952::Register::Name() const
{
    static const cstr names[Number] =
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


void AD9952::WriteToHardware(const Chan &ch, Register::E reg, uint value)
{
    if (reg == Register::ASF)
    {
        lastASF[ch] = value;
    }

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

    StructPIN cs = ChipSelect(ch);
    
    HAL_PIO::Reset(cs);

    HAL_SPI3::Transmit(buffer, (uint16)(numBytes[reg] + 1));
    
    HAL_PIO::Set(WR_AD9952_IO_UPD);
    volatile int i = 0;
    for (; i < 1000; ++i)
    {
    };
    HAL_PIO::Reset(WR_AD9952_IO_UPD);

    HAL_PIO::Set(cs);
}


StructPIN AD9952::ChipSelect(const Chan &ch)
{
    static const StructPIN cs[Chan::Count] = { {WR_AD9952_SPI3_CSA}, {WR_AD9952_SPI3_CSB} };

    return cs[ch];
}


void AD9952::Reset()
{
    HAL_PIO::Reset(WR_AD9952_RES_DDS);
    HAL_PIO::Set(WR_AD9952_RES_DDS);
    HAL_PIO::Reset(WR_AD9952_RES_DDS);

    HAL_PIO::Set(WR_AD9952_SPI3_CSA);
    HAL_PIO::Set(WR_AD9952_SPI3_CSB);

    HAL_PIO::Reset(WR_AD9952_IO_UPD);
    HAL_PIO::Reset(WR_AD9952_IOSYNA);
    HAL_PIO::Reset(WR_AD9952_IOSYNB);

    HAL_PIO::Reset(WR_AD9952_RES_DDS);
    HAL_PIO::Set(WR_AD9952_RES_DDS);
    HAL_PIO::Reset(WR_AD9952_RES_DDS);
}
