#pragma once
#include "defines.h"
#include "Settings/SettingsTypes.h"


#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4514)
#endif


/** @addtogroup Panel
 *  @{
 *  @addtogroup Interface
 *  @{
 */

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
    Структура комадны

1.  WRITE_SERVICE_COMMAND

    0                1        2
2.  SET_FORM_CHANNEL NUM_CHAN FormChannel

    0                     1        2                3...6
3.  SET_PARAMETER_CHANNEL NUM_CHAN ParameterChannel value

    0              1             2...5
12. WRITE_REGISTER NAME_REGISTER VALUE
*/

/// Возможные команды для передачи в основной процессор
struct CommandPanel
{
    enum E
    {
        COMMAND_NONE = 0,
        ENABLE_CHANNEL = 1,    ///< Включить/выключить канал
        SET_FORM_WAVE = 2,    ///< Установить форму сигнала
        SET_FREQUENCY = 3,    ///< Установить частоту
        SET_AMPLITUDE = 4,    ///< Установить амплитуду
        SET_OFFSET = 5,    ///< Установать смещение
        SET_DURATION = 6,
        SET_DUTYRATIO = 7,
        SET_PHASE = 8,
        RUN_RESET = 9,    ///< Сброс состояния
        MODE_DEBUG = 10,   ///< Установка отладочного режиме - когда идут непрерывные засылки в FPGA
        SET_DELAY = 11,   ///< Задержка
        WRITE_REGISTER = 12,   ///< Занести значение в регистр
        READ_DATA = 13,   ///< Чтение данных из генератора
        SET_DEPTH_MODULATION = 14,
        SET_POLARITY = 15,
        SET_DURATION_RISE = 16,
        SET_DURATION_FALL = 17,
        SET_DURATION_STADY = 18,
        SET_DUTY_FACTOR = 19,
        Number
    } value;
    CommandPanel(E v) : value(v){};
    operator uint8() const { return (uint8)value; };
};

/// Возможные команды, принимаемые от основного процессора
struct CommandGenerator
{
    enum E
    {
        //   0            1...5
        COM_FREQ_MEASURE = 1,   ///< FREQ_MEASURE частота                                                        /* Результат измерения частотомера */
        Number
    } value;
};

struct WaveParameter
{
    enum E
    {
        Frequency,          ///< Частота
        Period,             ///< Период
        Amplitude,          ///< Амплитуда
        Offset,             ///< Смещение
        Duration,           ///< Длительность
        DutyRatio,          ///< Скважность
        Phase,              ///< Сдвиг фазы
        Delay,              ///< Задержка
        DepthModulation,    ///< Глубина амплитудной модуляции
        Polarity,           ///< Полярность
        DurationRise,       ///< Длительность нарастания
        DurationFall,       ///< Длительность спада
        DurationStady,      ///< Длительность установившего значения
        DutyFactor,         ///< Коэффициент заполнения
        Number
    } value;

    WaveParameter(int v) : value((E)v) {};
    operator uint8() const { return (uint8)value; }
    float MinValue() const;
    float MaxValue() const;
    pString Name() const;
};

class Wave
{
public:
    struct Form
    {
        enum E
        {
            Sine,           ///< Синус
            Cosine,         ///< Косинус
            Meander,        ///< Меандр
            RampPlus,       ///< Пила+
            RampMinus,      ///< Пила-
            Triangle,       ///< Треугольник
            Trapeze,        ///< Трапеция
            Impulse,        ///< Импульс
            ExpPlus,        ///< Экспонента+
            ExpMinus,       ///< Экспонента-
            Noise,          ///< Шум
            Free,           ///< Произвольный
            Number
        } value;
        Form(E v) : value((E)v)
        {
        };
        operator uint8() const
        {
            return (uint8)value;
        };
        pString Name() const;
    };

#ifdef PANEL
    pString Name(uint num, Language lang);
#endif
};

struct Register
{
    enum E
    {
        Multiplexor1,
        Multiplexor2,
        OffsetA,
        OffsetB,
        FreqMeterLevel,
        FreqMeterHYS,
        FPGA_RG0_Control,           ///< Управляющий регистр
        FPGA_RG1_Freq,              ///< Регистр кода частоты
        FPGA_RG2_Mul,               ///< Регистр кода умножителя
        FPGA_RG3_RectA,             ///< Регистр кода сигнала прямоугольной формы канала А
        FPGA_RG4_RectB,             ///< Регистр кода сигнала прямоугольной формы канала B
        FPGA_RG5_PeriodImpulseA,    ///< Регистр кода периода прямоугольных импульсов канала А
        FPGA_RG6_DurationImpulseA,  ///< Регистр кода длительности прямоугльных импульсов канала А
        FPGA_RG7_PeriodImpulseB,    ///< Регистр кода периода прямоугольных импульсов канала B
        FPGA_RG8_DurationImpulseB,  ///< Регистр кода длительности прямоуольных импульсов канала B
        FPGA_RG9_FreqMeter,         ///< Регистр параметров частотомера
        FPGA_RG10_Offset,           ///< Регистр смещения
        Multiplexor3,
        FreqMeter_Resist,
        FreqMeter_Couple,
        FreqMeter_Filtr,
        Number
    } value;
    Register(E v) : value(v) {};
    operator uint8() const { return (uint8)value; };
    pString Name() const;
};


#define LENGTH_SPI_BUFFER   10

#define SPI_SLAVE_SYNBYTE   0x53

#define SPI_MASTER_SYNBYTE  0xAC

#define NAME_FORM(form, lang) namesWaveForm[form][lang]
#define FORM_RU(form) NAME_FORM(form, Language::RU)
#define FORM_EN(form) NAME_FORM(form, Language::EN)
extern const char * namesWaveForm[Wave::Form::Number][2];


//const char *Command_Name(CommandPanel command);


/** @}  @} */

#ifdef WIN32
#pragma warning(pop)
#endif
