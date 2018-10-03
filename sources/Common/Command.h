#pragma once
#include "defines.h"
#include "Settings/SettingsTypes.h"
#include "CommonTypes.h"


#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4514 4623)
#endif

#include "Wave.h"


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

    0                1        2...3       4...9
20  CREATE_HAND_WAVE NUM_CHAN START_POINT points

    0              1        2
22  SetRampForSine NUM_CHAN 0/1

    0                      1        2...5
23  SetDurationRampForSine NUM_CHAN value
*/

/// Возможные команды для передачи в основной процессор
struct CommandPanel
{
    enum E
    {
        None,
        EnableChannel,              ///< Включить/выключить канал
        SetFormWave,                ///< Установить форму сигнала
        SetFrequency,               ///< Установить частоту
        SetAmplitude,               ///< Установить амплитуду
        SetOffset,                  ///< Установать смещение
        SetDuration,
        SetDutyRatio,
        SetPhase,
        RunReset,                   ///< Сброс состояния
        ModeDebug,                  ///< Установка отладочного режиме - когда идут непрерывные засылки в FPGA
        SetDelay,                   ///< Задержка
        WriteRegister,              ///< Занести значение в регистр
        SET_DURATION_RISE,
        SET_DURATION_FALL,
        SET_DURATION_STADY,
        SET_DUTY_FACTOR,
        SetManipulation,            ///< Установить режим манипуляции
        SetManipulationDuration,    ///< Установить длительность ипульса в режиме манипуляции
        SetManipulationPeriod,      ///< Установить период следования импульсов в режиме манипуляции
        SetPacketPeriod,
        SetPacketNumber,
        // 0            1        2
        // SetStartMode NUM_CHAN value
        SetStartMode,               ///< Установить режим запуска по каналу
        SetPeriod,                  ///< Установить период
        // 0           1        2
        // SetPolarity NUM_CHAN polarity
        SetPolarity,
        // 0                   1        2
        // SetModeManipulation NUM_CHAN mode
        SetModeManipulation,
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
        COM_LOG = 2,
        Number
    } value;
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
        FPGA_RG2_Amplitude,         ///< Регистр кода умножителя
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


#define LENGTH_SPI_BUFFER   20

#define SPI_SLAVE_SYNBYTE   0x53

#define SPI_MASTER_SYNBYTE  0xAC

#define NAME_FORM(form, lang)  Form(form).Name(lang)
#define FORM_RU(form) NAME_FORM(form, Language::RU)
#define FORM_EN(form) NAME_FORM(form, Language::EN)


/** @}  @} */

#ifdef WIN32
#pragma warning(pop)
#endif
