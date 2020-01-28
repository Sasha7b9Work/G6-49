#include "defines.h"
#include "Generator/Calibrator.h"
#include "Generator/FPGA.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Math.h"


#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4310)   // cast truncates constant value
#endif


#define MAX_VALUE ((1 << 14) - 1)
#define MIN_VALUE (0)


FPGA::ModeWork::E       FPGA::modeWork[Chan::Count] = { FPGA::ModeWork::None, FPGA::ModeWork::None };;
FPGA::ClockFrequency::E FPGA::clock = FPGA::ClockFrequency::_100MHz;
FloatValue              FPGA::PacketImpulse::periodImpulse(static_cast<uint64>(0));
FloatValue              FPGA::PacketImpulse::durationImpulse(static_cast<uint64>(0));
StartMode               FPGA::startMode[Chan::Count] = { StartMode::Auto, StartMode::Auto };
uint64                  FPGA::registers[RG::Count] = { 0 };


/// \brief Здесь хранятся значения, предназначенные непосредственно для засылки в ПЛИС. Сначала идут младшие 8 бит, а потом старшие 6 бит
/// Данные должны быть записаны в прямом коде - 0 в старшем разряде обозначает положительное число, а 1 - отрицательное
static uint8 dataDDS[Chan::Count][FPGA::NUM_POINTS * 2] __attribute__((section("CCM_DATA")));


void FPGA::Init()
{
    HAL_PIO::Init(WR_D0, HMode::Output_PP, HPull::Down);
    HAL_PIO::Init(WR_D1, HMode::Output_PP, HPull::Down);
    HAL_PIO::Init(WR_D2, HMode::Output_PP, HPull::Down);
    HAL_PIO::Init(WR_D3, HMode::Output_PP, HPull::Down);
    HAL_PIO::Init(WR_D4, HMode::Output_PP, HPull::Down);
    HAL_PIO::Init(WR_D5, HMode::Output_PP, HPull::Down);
    HAL_PIO::Init(WR_D6, HMode::Output_PP, HPull::Down);
    HAL_PIO::Init(WR_D7, HMode::Output_PP, HPull::Down);

    // Настраиваем выходы для записи в регистры ПЛИС

    HAL_PIO::Reset(WR_FPGA_WR_RG);
    HAL_PIO::Reset(WR_FPGA_CLK_RG);
    HAL_PIO::Reset(WR_FPGA_DT_RG);
    HAL_PIO::Reset(WR_FPGA_A0_RG);
    HAL_PIO::Reset(WR_FPGA_A1_RG);
    HAL_PIO::Reset(WR_FPGA_A2_RG);
    HAL_PIO::Reset(WR_FPGA_A3_RG);

    WriteMaxAmplitude(Chan::A);
    WriteMaxAmplitude(Chan::B);
}


void FPGA::SetWaveForm(Chan::E ch, TypeForm::E form)
{
    typedef void(*pFuncFpgaVU8)(Chan::E);

    static const pFuncFpgaVU8 funcs[TypeForm::Count] =
    {
        SetModeSine,            ///< Здесь включается режим амплитудной манипуляции
        SetModeRampPlus,
        SetModeRampMinus,
        SetModeTriangle,
        SetModeMeander,
        SetModeImpulse,
        SetModePackedImpulse,
        SetModeDDS
    };
    
    funcs[form](ch);
    
    WriteControlRegister();
}


void FPGA::SetModeMeander(Chan::E ch)
{
    modeWork[ch] = ModeWork::Meander;
    WriteControlRegister();
}


void FPGA::WriteMaxAmplitude(Chan::E ch)
{
    // Записать максимальный размах сигнала в регистры

    uint64 data = (static_cast<uint64>(16383) << 14) + (8191);

    static const RG::E regs[Chan::Count] = { RG::_3_RectA, RG::_4_RectB };

    WriteRegister(regs[ch], data);
}


void FPGA::SetModeSine(Chan::E ch)
{
    modeWork[ch] = ModeWork::Sine;
}


void FPGA::SetModeDDS(Chan::E ch)
{
    modeWork[ch] = ModeWork::DDS;
    SendData(DataFlash(ch));
}


void FPGA::SetModeRampPlus(Chan::E ch)
{
    modeWork[ch] = ModeWork::DDS;
    SendData(&dataDDS[Chan::A][0]);
}


void FPGA::SetModeRampMinus(Chan::E ch)
{
    modeWork[ch] = ModeWork::DDS;
    SendData(&dataDDS[Chan::A][0]);
}


void FPGA::SetModeTriangle(Chan::E ch)
{
    modeWork[ch] = ModeWork::DDS;
    SendData(&dataDDS[Chan::A][0]);
}


void FPGA::SetModePackedImpulse(Chan::E)
{
    modeWork[Chan::A] = ModeWork::PackedImpulse;
    WriteControlRegister();

    uint64 data = (16383 << 14) + 8191;

    WriteRegister(RG::_3_RectA, data);

    SetPolarity(Chan::A, 0);
}


void FPGA::SetPolarity(Chan::E ch, uint8 polarity)
{
    uint64 data = (8191 << 14) + 16383;     // Положительная полярность

    if(polarity == 1)
    {
        data = (16383 << 14) + 8191;        // Отрицательная полярность
    }

    static const RG::E regs[Chan::Count] = {RG::_3_RectA, RG::_4_RectB};

    WriteRegister(regs[ch], data);
}


void FPGA::SetModeImpulse(Chan::E ch)
{
    modeWork[ch] = ModeWork::Impulse;
    WriteControlRegister();

    SetPolarity(ch, 0);         // Устанавливаем положительную полярность
}


void FPGA::SetFrequency(Chan::E ch)
{
    WriteControlRegister();

    float frequency = SetGenerator::Frequency(ch);
    
    if(modeWork[ch] == ModeWork::Meander)
    {
        AD9952::SetFrequency(ch);
    }
    else if (modeWork[ch] == ModeWork::DDS)
    {
        uint64 N = static_cast<uint64>((frequency * (static_cast<uint64>(1) << 40)) / 1E8F);
        WriteRegister(RG::_1_Freq, N);
    }
    else if(modeWork[ch] == ModeWork::Impulse || modeWork[ch] == ModeWork::Impulse2)
    {
        if ((ch == Chan::B) && (modeWork[ch] == ModeWork::Impulse2))
        {
            modeWork[ch] = ModeWork::Impulse;
            WriteControlRegister();
        }
        uint N = static_cast<uint>(1E8F / frequency + 0.5F);
        WriteRegister(ch == Chan::A ? RG::_5_PeriodImpulseA : RG::_7_PeriodImpulseB, N);
    }
    else
    {
        // здесь ничего
    }
}


void FPGA::SetDurationImpulse(Chan::E ch, FloatValue duration)
{
    PacketImpulse::durationImpulse = duration;

    RG::E reg = Chan(ch).IsA() ? RG::_6_DurationImpulseA : RG::_8_DurationImpulseB;
    if(Chan(ch).IsA() && (modeWork[Chan::A] == ModeWork::PackedImpulse))
    {
        reg = RG::_8_DurationImpulseB;
    }

    uint64 value = duration.ToUINT64() / 10;

    WriteRegister(reg, value);
}


void FPGA::PacketImpulse::SetPeriodPacket(FloatValue period)
{
    uint64 value = period.ToUINT64() / 10 - 2;

    WriteRegister(RG::_5_PeriodImpulseA, value);
}


void FPGA::PacketImpulse::SetNumberImpules(uint value)
{
    uint64 n = static_cast<uint64>(((value - 1) * periodImpulse.ToFloat() + durationImpulse.ToFloat()) / 10E-9F);

    WriteRegister(RG::_6_DurationImpulseA, n);
}


void FPGA::SetPeriodImpulse(Chan::E ch, FloatValue period)
{
    // Для пакетного и одиночного импульсных режимов период задаётся здесь. Поэтому сохраняем значение периода импульсов, чтобы использовать его
    // в пакетном режиме при необходимости

    PacketImpulse::periodImpulse = period;

    RG::E reg = Chan(ch).IsA() ? RG::_5_PeriodImpulseA : RG::_7_PeriodImpulseB;
    if(Chan(ch).IsA() && (modeWork[Chan::A] == ModeWork::PackedImpulse))
    {
        reg = RG::_7_PeriodImpulseB;
    }

    uint64 value = period.ToUINT64() / 10 - 2;

    WriteRegister(reg, value);
}


void FPGA::SetStartMode(Chan::E ch, StartMode mode)
{
    startMode[ch] = mode;
    WriteControlRegister();
}


void FPGA::SetClockAD992(ClockFrequency::E _clock)
{
    clock = _clock;
    WriteControlRegister();
}


void FPGA::WriteControlRegister()
{
    uint16 data = BIN_U16(00000000, 01111000);       // Биты 3, 4, 5, 6 устанавливаем в единичное состояние. Их активное состояние - нулевое

    Bit::Set(data, RG0::_0_WriteData);                    // В нулевом бите 0 записываем только для записи данных в память

    if((modeWork[Chan::A] == ModeWork::Sine) && AD9952::Manipulation::IsEnabled(Chan::A))
    {
        Bit::Clear(data, RG0::_3_ManipulationOSK2);
    }

    if ((modeWork[Chan::B] == ModeWork::Sine) && AD9952::Manipulation::IsEnabled(Chan::B))
    {
        Bit::Clear(data, RG0::_5_ManipulationOSK1);
    }

    switch(static_cast<uint8>(modeWork[Chan::A])) //-V2520
    {
        case ModeWork::Meander:     
            Bit::Set(data, RG0::_8_MeanderA);
        case ModeWork::Rectangle:
        case ModeWork::Impulse:
        case ModeWork::PackedImpulse:
            Bit::Set(data, RG0::_1_ImpulseA);
            break;
    }

    switch(static_cast<uint8>(modeWork[Chan::B])) //-V2520
    {
        case ModeWork::Meander:   
            Bit::Set(data, RG0::_9_MeanderB);
        case ModeWork::Rectangle:
        case ModeWork::Impulse:
            Bit::Set(data, RG0::_2_ImpulseB);
            break;
    }

    if(FPGA::clock == FPGA::ClockFrequency::_1MHz)
    {
        Bit::Set(data, RG0::_7_Freq_MHz);
    }

    if(modeWork[Chan::A] == ModeWork::PackedImpulse)
    {
        Bit::Set(data, RG0::_12_HandStartPacket);
    }

    data = SetBitsStartMode(data);

    WriteRegister(RG::_0_Control, data);
}


uint16 FPGA::SetBitsStartMode(uint16 data)
{
    ModeWork::E mode = modeWork[Chan::A];
    StartMode start = startMode[Chan::A];

    if((mode == ModeWork::Impulse) || (mode == ModeWork::PackedImpulse))
    {
        if(start.Is(StartMode::Single))
        {
            Bit::Set(data, RG0::_10_HandStartA);
        }
    }

    if(mode == ModeWork::DDS)
    {
        if(start.Is(StartMode::ComparatorA))
        {
            Bit::Set(data, RG0::_13_StartMode0);
        }
        else if(start.Is(StartMode::ShaperB))
        {
            Bit::Set(data, RG0::_14_StartMode1);
        }
        else if(start.Is(StartMode::Single))
        {
            Bit::Set(data, RG0::_13_StartMode0);
            Bit::Set(data, RG0::_14_StartMode1);
        }
        else
        {
            // здесь ничего
        }
    }

    mode = modeWork[Chan::B];
    start = startMode[Chan::B];

    if((mode == ModeWork::Impulse) || (mode == ModeWork::PackedImpulse))
    {
        if(start.Is(StartMode::Single))
        {
            Bit::Set(data, RG0::_11_HandStartB);
        }
    }

    if (mode == ModeWork::DDS)
    {
        if (start.Is(StartMode::ComparatorA))
        {
            Bit::Set(data, RG0::_13_StartMode0);
        }
        else if (start.Is(StartMode::ShaperB))
        {
            Bit::Set(data, RG0::_14_StartMode1);
        }
        else if (start.Is(StartMode::Single))
        {
            Bit::Set(data, RG0::_13_StartMode0);
            Bit::Set(data, RG0::_14_StartMode1);
        }
        else
        {
            // здесь ничего
        }
    }

    return data;
}


bool FPGA::Start()
{
    if(startMode[Chan::A] != StartMode::Auto || startMode[Chan::B] != StartMode::Auto)
    {
        WriteRegister(RG::_11_Start, 2);
        return true;
    }
    return false;
}


void FPGA::SendData(uint8 *data)
{
    WriteRegister(RG::_0_Control, 0);
    
    uint8 *pointer = data;

    /// \todo Это временно так задержка организована
    volatile int i = 0;

    for(i = 0; i < 10; i++)
    {
        *pointer = data[0];
    }

    for(i = 0; i < FPGA::NUM_POINTS * 4; i++)
    {
        HAL_PIO::WriteFPGA(*pointer++);

        /// \todo Это временно так задержка организована
        volatile int j = 0;
        for(j = 0; j < 10; j++) {}
        HAL_PIO::Set(WR_FPGA_WR_DATA);
        for(j = 0; j < 10; j++) {}
        HAL_PIO::Reset(WR_FPGA_WR_DATA);
    }

    WriteRegister(RG::_0_Control, 1);
}


void FPGA::WriteRegister(RG::E reg, uint64 value)
{
    static const int numBits[RG::Count] =
    {
        16, // _0_Control,
        40, // _1_Freq,
        20, // _2_Amplitude,
        28, // _3_RectA,
        28, // _4_RectB,
        32, // _5_PeriodImpulseA,
        32, // _6_DurationImpulseA,
        32, // _7_PeriodImpulseB,
        32, // _8_DurationImpulseB,
        13, // _9_FreqMeter
        28, // _10_Offset
        2   // _11_Start
    };

    registers[reg] = value;

    WriteAddress(reg);

    for(int i = 0; i < 2; i++)
    {
        for (int bit = numBits[reg] - 1; bit >= 0; bit--)
        {
            HAL_PIO::Write(WR_FPGA_DT_RG, Bit::Get(value, bit));    // Устанавливаем или сбрасываем соответствующий бит
            HAL_PIO::Set(WR_FPGA_CLK_RG);                           // И записываем его в ПЛИС
            HAL_PIO::Reset(WR_FPGA_CLK_RG);
        }
    }

    HAL_PIO::Set(WR_FPGA_WR_RG);                                    // Теперь переписываем данные из сдвиговоого регистра в FPGA
    HAL_PIO::Reset(WR_FPGA_WR_RG);
    HAL_TIM::Delay(10);                                         // Ждём 10 миллисекунд, пока данные перепишутся в FPGA
}


void FPGA::WriteAddress(RG::E reg)
{
    HAL_PIO::Write(WR_FPGA_A0_RG, Bit::Get(reg, 0));
    HAL_PIO::Write(WR_FPGA_A1_RG, Bit::Get(reg, 1));
    HAL_PIO::Write(WR_FPGA_A2_RG, Bit::Get(reg, 2));
    HAL_PIO::Write(WR_FPGA_A3_RG, Bit::Get(reg, 3));
}


void FPGA::SetAmplitude()
{
    uint nA = (uint)((SetGenerator::Amplitude(Chan::A) * (1023 + Calibrator::GetOffsetK_Zero(Chan::A))) / 10);
    uint nB = (uint)((SetGenerator::Amplitude(Chan::B) * (1023 + Calibrator::GetOffsetK_Zero(Chan::B))) / 10);

    WriteRegister(RG::_2_Amplitude, nA + (nB << 10));
}


uint8 *FPGA::DataDDS(Chan::E ch)
{
    return &dataDDS[ch][0];
}


uint8 *FPGA::DataFlash(Chan::E ch)
{
    return reinterpret_cast<uint8 *>(HAL_EEPROM::Signal::Get(ch));
}


void FPGA::SaveExtSignal(Chan::E ch, uint8 *data)
{
    HAL_EEPROM::Signal::Save(ch, reinterpret_cast<uint16 *>(data));
}

#ifdef WIN32
#pragma warning(pop)
#endif
