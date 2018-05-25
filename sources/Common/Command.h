#pragma once
#include "defines.h"


/** @addtogroup Panel
 *  @{
 *  @addtogroup Interface
 *  @{
 */


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
    Структура комадны

1.    WRITE_SERVICE_COMMAND

           0             1         2
2.    SET_FORM_CHANNEL NUM_CHAN FormChannel

            0                  1           2          3...6
3.    SET_PARAMETER_CHANNEL NUM_CHAN ParameterChannel value
*/

/// Возможные команды для передачи в основной процессор
enum CommandWrite
{
    WRITE_SERVICE_COMMAND   = 0,    ///< Служебная. Код посыалется в отсутствие информации для передачи.
    ENABLE_CHANNEL          = 1,    ///< Включить/выключить канал
    SET_FORM_WAVE           = 2,    ///< Установить форму сигнала
    SET_FREQUENCY           = 3,    ///< Установить частоту
    SET_AMPLITUDE           = 4,    ///< Установить амплитуду
    SET_OFFSET              = 5,    ///< Установать смещение
    SET_DURATION            = 6,
    SET_DUTYRATIO           = 7,
    SET_PHASE               = 8,
    RUN_RESET               = 9,   ///< Сброс состояния
    MODE_DEBUG              = 10,   ///< Установка отладочного режиме - когда идут непрерывные засылки в FPGA
    SET_DELAY               = 11,   ///< Задержка
    NUM_COMMAND_WRITE,
    COMMAND_NONE
};

enum Type_WaveParameter
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
    NumParameters
};

enum Type_WaveForm
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
    NumForms
};

struct WaveForm
{
    Type_WaveForm type;
    WaveForm(uint8 num = 0);
    const char * Name() const;
    uint8 ToValue() const;
};


/// Возможные команды, принимаемые от основного процессора
enum CommandRead
{
    READ_SERVICE_COMMAND,   ///< Служебная команда. Используется для того, чтобы можно было посылать данные основному процессору
    READ_POINTS             ///< Принимает точки произвольного сигнала
};

#define LENGTH_SPI_BUFFER   10

#define SPI_SLAVE_SYNBYTE   0x53

#define SPI_MASTER_SYNBYTE  0xAC

#define NAME_FORM(form, lang) namesWaveForm[form][lang]
#define FORM_RU(form) NAME_FORM(form, RU)
#define FORM_EN(form) NAME_FORM(form, EN)
extern const char * namesWaveForm[NumForms][2];


//const char *Command_Name(CommandWrite command);


/** @}  @} */
