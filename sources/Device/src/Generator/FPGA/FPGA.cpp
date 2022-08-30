// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Log.h"
#include "Generator/Calibrator.h"
#include "Generator/FPGA/FPGA.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include "Generator/FPGA/ClockImpulse.h"


#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4310)   // cast truncates constant value
#endif


#define MAX_VALUE ((1 << 14) - 1)
#define MIN_VALUE (0)


// \brief Здесь хранятся значения, предназначенные непосредственно для засылки в ПЛИС. Сначала идут младшие 8 бит, а потом старшие 6 бит
// Данные должны быть записаны в прямом коде - 0 в старшем разряде обозначает положительное число, а 1 - отрицательное
static uint8 dataDDS[Chan::Count][FPGA::NUM_POINTS * 2] __attribute__((section("CCM_DATA")));


namespace FPGA
{
    static void SetFormSine(const Chan &);

    // Установить режим Пила+
    static void SetFormRampPlus(const Chan &);

    // Установить режим Пила-
    static void SetFormRampMinus(const Chan &);

    // Установить режим Треугольник
    static void SetFormTriangle(const Chan &);

    // Установить режим произвольного сигнала, загруженного с флешки
    static void SetFormFree(const Chan &);

    static void SetFormMeander(const Chan &);

    static void SetFormImpulse(const Chan &);

    static void SetFormPackedImpulse(const Chan &);

    // Заслать рассчитанные точки обоих каналов в плис
    static void SendData();

    // Заслать рассчитанные точки одного канала в плис
    static void SendDataChannel(const Chan &);

    // Запись управляющего регистра
    static void WriteControlRegister();

    // Записывает коды, соответствующие максимальному и минимальному значению
    static void WriteMaxAmplitude(const Chan &);

    // Установить биты, соответствующие режиму запуска
    static void SetBitsStartMode(uint16 &data);
    static void SetBitsStartMode(const Chan &, uint16 &data);

    // Возвращает true, если по каналу ch работает DDS
    static bool InModeDDS(const Chan &);

    // Режим запуска для произвольного сигнала (0) и для импульсного сигнала (1)
    static StartMode::E startMode[Chan::Count][2] = { { StartMode::Auto, StartMode::Auto }, { StartMode::Auto, StartMode::Auto } };

    // Режим СтартА/СтопВ - вкл/откл
    static StartStopMode::E startStopMode = StartStopMode::Disable;

    namespace PacketImpulse
    {
        Value PacketImpulse::periodImpulse("0", Order::One);
        Value PacketImpulse::durationImpulse("0", Order::One);
    }

    namespace ModeWork
    {
        // Режим работы ПЛИС
        static E value[Chan::Count] = { FPGA::ModeWork::None, FPGA::ModeWork::None };;
    }

    ClockAD992::E ClockAD992::value = ClockAD992::_100MHz;
}


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

    WriteMaxAmplitude(ChA);
    WriteMaxAmplitude(ChB);
}


void FPGA::SetWaveForm(const Chan &ch, TypeForm::E form)
{
    typedef void(*pFuncFpgaVU8)(const Chan &);

    static const pFuncFpgaVU8 funcs[TypeForm::Count] =
    {
        SetFormSine,            // Здесь включается режим амплитудной манипуляции
        SetFormRampPlus,
        SetFormRampMinus,
        SetFormTriangle,
        SetFormMeander,
        SetFormImpulse,
        SetFormPackedImpulse,
        SetFormFree
    };
    
    funcs[form](ch);
    
    WriteControlRegister();
}


void FPGA::SetFormMeander(const Chan &ch)
{
    ModeWork::value[ch] = ModeWork::Meander;
    WriteControlRegister();
    AD9952::SetAmplitudeForMeander(ch);
}


void FPGA::WriteMaxAmplitude(const Chan &ch)
{
    // Записать максимальный размах сигнала в регистры

    uint64 data = ((uint64)16383 << 14) + (8191);

    static const Register::E regs[Chan::Count] = { Register::_3_RectA, Register::_4_RectB };

    Register::Write(regs[ch], data);
}


void FPGA::SetFormSine(const Chan &ch)
{
    ModeWork::value[ch] = ModeWork::Sine;
}


void FPGA::SetFormFree(const Chan &ch)
{
    ModeWork::value[ch] = ModeWork::Free;
    SendData();
}


void FPGA::SetFormRampPlus(const Chan &ch)
{
    ModeWork::value[ch] = ModeWork::DDS;
    SendData();
}


void FPGA::SetFormRampMinus(const Chan &ch)
{
    ModeWork::value[ch] = ModeWork::DDS;
    SendData();
}


void FPGA::SetFormTriangle(const Chan &ch)
{
    ModeWork::value[ch] = ModeWork::DDS;
    SendData();
}


void FPGA::SetFormPackedImpulse(const Chan &)
{
    ModeWork::value[Chan::A] = ModeWork::PackedImpulse;
    WriteControlRegister();

    uint64 data = (16383 << 14) + 8191;

    Register::Write(Register::_3_RectA, data);

    SetPolarity(ChA, 0);
}


void FPGA::SetPolarity(const Chan &ch, uint8 polarity)
{
    uint64 data = (8191 << 14) + 16383;     // Положительная полярность

    if(polarity == 0)
    {
        data = (16383 << 14) + 8191;        // Отрицательная полярность
    }

    static const Register::E regs[Chan::Count] = { Register::_3_RectA, Register::_4_RectB };

    Register::Write(regs[ch], data);
}


void FPGA::SetFormImpulse(const Chan &ch)
{
    ModeWork::value[ch] = ModeWork::Impulse;
    WriteControlRegister();

    SetPolarity(ch, 0);         // Устанавливаем положительную полярность
}


void FPGA::SetFrequency(const Chan &ch)
{
    WriteControlRegister();

    double frequency = SettingsGenerator::Frequency(ch);
    
    if(ModeWork::value[ch] == ModeWork::Meander)
    {
        AD9952::SetFrequency(ch);
    }
    else if (InModeDDS(ch))
    {
        uint64 N = (uint64)((frequency * ((uint64)1 << 40)) / 1E8F);
        Register::Write(Register::_1_Freq, N);
    }
    else if(ModeWork::value[ch] == ModeWork::Impulse || ModeWork::value[ch] == ModeWork::Impulse2)
    {
        if (ch.IsB() && (ModeWork::value[ch] == ModeWork::Impulse2))
        {
            ModeWork::value[ch] = ModeWork::Impulse;
            WriteControlRegister();
        }
        uint N = (uint)(1E8F / frequency + 0.5F);
        Register::Write(ch.IsA() ? Register::_5_PeriodImpulseA : Register::_7_PeriodImpulseB, N);
    }
}


void FPGA::SetDurationImpulse(const Chan &ch, const Value &duration)
{
    PacketImpulse::durationImpulse = duration;

    Register::E reg = ch.IsA() ? Register::_6_DurationImpulseA : Register::_8_DurationImpulseB;

    if(ch.IsA() && (ModeWork::value[Chan::A] == ModeWork::PackedImpulse))
    {
        reg = Register::_8_DurationImpulseB;
    }

    ClockImpulse::SetDuration(ch, duration);

    uint64 value = duration.ToUINT64() / ClockImpulse::GetDivider();

    Register::Write(reg, value);
}


void FPGA::PacketImpulse::SetPeriodPacket(const Value &period)
{
    ClockImpulse::SetPeriod(ChA, period);

    uint64 value = period.ToUINT64() / ClockImpulse::GetDivider();

    Register::Write(Register::_5_PeriodImpulseA, value);
}


void FPGA::PacketImpulse::SetNumberImpules(uint value)
{
    uint64 n = (uint64)(((value - 1) * periodImpulse.ToDouble() + durationImpulse.ToDouble()) / 10E-9);

    Register::Write(Register::_6_DurationImpulseA, n);
}


void FPGA::SetPeriodImpulse(const Chan &ch, const Value &period)
{
    // Для пакетного и одиночного импульсных режимов период задаётся здесь. Поэтому сохраняем значение периода импульсов, чтобы использовать его
    // в пакетном режиме при необходимости

    PacketImpulse::periodImpulse = period;

    Register::E reg = ch.IsA() ? Register::_5_PeriodImpulseA : Register::_7_PeriodImpulseB;

    if(ch.IsA() && (ModeWork::value[Chan::A] == ModeWork::PackedImpulse))
    {
        reg = Register::_7_PeriodImpulseB;
    }

    ClockImpulse::SetPeriod(ch, period);

    uint64 value = period.ToUINT64() / ClockImpulse::GetDivider();

    if (ClockImpulse::Is100MHz())
    {
        value -= 2;
    }

    Register::Write(reg, value);
}


void FPGA::SetDelayStartStop(const Value &delay)
{
    Register::E reg = Register::_7_PeriodImpulseB;

    uint64 value = delay.ToUINT64() / ClockImpulse::GetDivider();

    if (ClockImpulse::Is100MHz())
    {
        value -= 2;
    }

    Register::Write(reg, value);
}


void FPGA::SetStartMode(const Chan &ch, uint8 signal, StartMode::E mode)
{
    startMode[ch][signal] = mode;
    WriteControlRegister();
}


void FPGA::EnableStartStopMode(StartStopMode::E mode)
{
    startStopMode = mode;
    WriteControlRegister();
}


void FPGA::ClockAD992::Set(ClockAD992::E _clock)
{
    value = _clock;
    WriteControlRegister();
}


FPGA::ClockAD992::E FPGA::ClockAD992::Get()
{
    return value;
}


void FPGA::WriteControlRegister()
{
    //                               6543
    uint16 data = BIN_U16(00000000, 01101000);       // Биты 3, 4, 5, 6 устанавливаем в единичное состояние. Их активное состояние - нулевое

    Bit::Set(data, RG0::_0_WriteData);                    // В нулевом бите 0 записываем только для записи данных в память

    if((ModeWork::value[Chan::A] == ModeWork::Sine) && AD9952::Manipulation::IsEnabled(ChA))
    {
        Bit::Clear(data, RG0::_3_ManipulationOSK2);
    }

    if ((ModeWork::value[Chan::B] == ModeWork::Sine) && AD9952::Manipulation::IsEnabled(ChB))
    {
        Bit::Clear(data, RG0::_5_ManipulationOSK1);
    }

    switch((uint8)ModeWork::value[Chan::A]) //-V2520
    {
        case ModeWork::Meander:     
            Bit::Set(data, RG0::_8_MeanderA);
        case ModeWork::Rectangle:
        case ModeWork::Impulse:
        case ModeWork::PackedImpulse:
            Bit::Set(data, RG0::_1_ImpulseA);
            break;
    }

    switch((uint8)ModeWork::value[Chan::B]) //-V2520
    {
        case ModeWork::Meander:   
            Bit::Set(data, RG0::_9_MeanderB);
        case ModeWork::Rectangle:
        case ModeWork::Impulse:
            Bit::Set(data, RG0::_2_ImpulseB);
            break;
    }

    if(ClockAD992::Get() == ClockAD992::_1MHz)
    {
        Bit::Set(data, RG0::_7_ClockAD9952);
    }

    if (ModeWork::value[Chan::A] == ModeWork::PackedImpulse)
    {
        Bit::Set(data, RG0::_12_PacketImpulse);
    }

    SetBitsStartMode(data);

    if (startStopMode == StartStopMode::Enable)
    {
        _SET_BIT(data, 11);
        _SET_BIT(data, 12);
    }

    if(ClockImpulse::Is100MHz())
    {
        _CLEAR_BIT(data, RG0::_4_ClockImpulse);
    }
    else
    {
        _SET_BIT(data, RG0::_4_ClockImpulse);
    }

    Register::Write(Register::_0_Control, data);
}


bool FPGA::InModeDDS(const Chan &ch)
{
    return (ModeWork::value[ch] == FPGA::ModeWork::DDS) || (ModeWork::value[ch] == FPGA::ModeWork::Free);
}


void FPGA::SetBitsStartMode(const Chan &ch, uint16 &data)
{
    ModeWork::E mode = ModeWork::value[ch];

    if (mode == ModeWork::Impulse || mode == ModeWork::PackedImpulse)
    {
        if (startMode[ch][1] == StartMode::Single)
        {
            Bit::Set(data, ch.IsA() ? RG0::_10_HandStartA : RG0::_11_HandStartB);
        }
    }

    if (InModeDDS(ch))
    {
        StartMode::E start = startMode[ch][0];

        if (start == StartMode::ComparatorA)
        {
            Bit::Set(data, RG0::_13_StartMode0);
        }
        else if (start == StartMode::ShaperB)
        {
            Bit::Set(data, RG0::_14_StartMode1);
        }
        else if (start == StartMode::Single)
        {
            Bit::Set(data, RG0::_13_StartMode0);
            Bit::Set(data, RG0::_14_StartMode1);
        }
    }
}


void FPGA::SetBitsStartMode(uint16 &data)
{
    SetBitsStartMode(ChA, data);
    SetBitsStartMode(ChB, data);
}


void FPGA::SingleStart()
{
    Register::Write(Register::_11_Start, 2);
}


void FPGA::SendData()
{
    /*
        Данные сигнала произвольной формы, засылаемые в ПЛИС, организованы следующим образом.
        1. Количество точек - 8192
        2. Каждая точка имеет разрядность 14 бит, старший бит - знак. "+" - 0, "-" - 1
        3. Порядок засылки данных следующий:
            - вначале засылаются младшие байты (8192 шт);
            - затем засылаются старшие байты (8192 шт).
    */


    Register::Write(Register::_0_Control, 0);
    
    SendDataChannel(ChA);
    SendDataChannel(ChB);

    Register::Write(Register::_0_Control, 1);
}


void FPGA::SendDataChannel(const Chan &ch)
{
    volatile int j = 0;
    for(j = 0; j < 10; j++) { }

    uint8 *pointer = (ModeWork::value[ch] == ModeWork::Free) ? DataFreeSignal(ch) : &dataDDS[ch][0];

    for (int i = 0; i < FPGA::NUM_POINTS * 2; i++)
    {
        HAL_PIO::WriteFPGA(*pointer++);

        // \todo Это временно так задержка организована
        for (j = 0; j < 10; j++) { }
        HAL_PIO::Set(WR_FPGA_WR_DATA);
        for (j = 0; j < 10; j++) { }
        HAL_PIO::Reset(WR_FPGA_WR_DATA);
    }
}


// Расчёт кода для засылки в регистр амлпитуды FPGA
static uint CalculateCodeAmplitude(const Chan &ch)
{
    float k = Calibrator::GetAmplitudeK(ch);

    double att = 1.0 / Amplifier::GetAmplification(ch);

    double amplitude = k * att * SettingsGenerator::Amplitude(ch);

    return (uint)(amplitude * 1023);
}


void FPGA::SetAmplitude()
{
    uint nA = CalculateCodeAmplitude(ChA);
    uint nB = CalculateCodeAmplitude(ChB);

    Register::Write(Register::_2_Amplitude, nA + (nB << 10));
}


uint8 *FPGA::DataDDS(const Chan &ch)
{
    return &dataDDS[ch][0];
}


uint8 *FPGA::DataFreeSignal(const Chan &ch)
{
    return (uint8 *)HAL_EEPROM::Signal::Get(ch);
}


void FPGA::SaveExtSignal(const Chan &ch, uint8 *data)
{
    HAL_EEPROM::Signal::Save(ch, (uint16 *)data);
}


void FPGA::TransformCodeToData(const uint8 codeIn[FPGA::NUM_POINTS * 2], float dataOut[FPGA::NUM_POINTS])
{
    for (int i = 0; i < FPGA::NUM_POINTS; i++)
    {
        uint16 code = (uint16)(codeIn[i] + (codeIn[i + FPGA::NUM_POINTS] << 8));

        float sign = _GET_BIT(code, 13) ? 1.0F : -1.0F;

        _CLEAR_BIT(code, 13);

        float data = (float)code / 0x1fff;

        dataOut[i] = data * sign;
    }
}


#ifdef WIN32
#pragma warning(pop)
#endif
