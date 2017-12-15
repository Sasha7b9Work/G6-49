#pragma once


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
typedef enum
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
} CommandWrite;

typedef enum
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
} WaveParameter;

typedef enum
{
    Form_Sine,
    Form_Saw,
    Form_Impulse,
    NumForms
} WaveForm;


/// Возможные команды, принимаемые от основного процессора
typedef enum
{
    READ_SERVICE_COMMAND,   ///< Служебная команда. Используется для того, чтобы можно было посылать данные основному процессору
    READ_POINTS             ///< Принимает точки произвольного сигнала
} CommandRead;


#define LENGTH_SPI_BUFFER   10

#define SPI_SLAVE_SYNBYTE   0x53

#define SPI_MASTER_SYNBYTE  0xAC


char *Command_Name(CommandWrite command);

/** @}  @} */
