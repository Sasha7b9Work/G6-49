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

enum WaveParameter
{
    Frequency,      ///< Частота
    Period,         ///< Период
    Amplitude,      ///< Амплитуда
    Offset,         ///< Смещение
    Duration,       ///< Длительность
    DutyRatio,      ///< Скважность
    Phase,          ///< Фаза
    Delay,          ///< Задержка
    NumParameters
};

enum Type_WaveForm
{
    Form_Sine,
    Form_Saw,
    Form_Impulse,
    NumForms
};

struct WaveForm
{
    Type_WaveForm type;
    WaveForm(uint8 num = 0) : type((Type_WaveForm)num) {};
    const char * Name();
    uint8 ToValue();
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


const char *Command_Name(CommandWrite command);


/** @}  @} */
