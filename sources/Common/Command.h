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
        None = 0,
        ENABLE_CHANNEL = 1,             ///< Включить/выключить канал
        SET_FORM_WAVE = 2,              ///< Установить форму сигнала
        SET_FREQUENCY = 3,              ///< Установить частоту
        SET_AMPLITUDE = 4,              ///< Установить амплитуду
        SET_OFFSET = 5,                 ///< Установать смещение
        SET_DURATION = 6,
        SET_DUTYRATIO = 7,
        SET_PHASE = 8,
        RUN_RESET = 9,                  ///< Сброс состояния
        MODE_DEBUG = 10,                ///< Установка отладочного режиме - когда идут непрерывные засылки в FPGA
        SET_DELAY = 11,                 ///< Задержка
        WRITE_REGISTER = 12,            ///< Занести значение в регистр
        READ_DATA = 13,                 ///< Чтение данных из генератора
        SET_DEPTH_MODULATION = 14,
        SET_POLARITY = 15,
        SET_DURATION_RISE = 16,
        SET_DURATION_FALL = 17,
        SET_DURATION_STADY = 18,
        SET_DUTY_FACTOR = 19,
        CREATE_HAND_WAVE = 20,          ///< Засылает точки произвольного сигнала
        SET_HAND_WAVE = 21,             ///< Устанавливает вывод произвольного сигнала
        SetRampForSine = 22,            ///< Установить режим модуляции пилой для синуса
        SetRampSineDuration = 23,       ///< Установить длительность ипульса при модуляции синуса пилой
        SetRampSineAmplitude = 24,      ///< Установить амплитуду для пилы, модулирующей синус
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


#define LENGTH_SPI_BUFFER   10

#define SPI_SLAVE_SYNBYTE   0x53

#define SPI_MASTER_SYNBYTE  0xAC

#define NAME_FORM(form, lang)  Form(form).Name(lang)
#define FORM_RU(form) NAME_FORM(form, Language::RU)
#define FORM_EN(form) NAME_FORM(form, Language::EN)


/** @}  @} */

#ifdef WIN32
#pragma warning(pop)
#endif
