#pragma once
#include "common/stm32/429/stm429.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define TIME_TICKS CPU::_TIM_::TimeTicks()
#define TIME_US    CPU::_TIM_::TimeUS()
#define TIME_MS    CPU::_TIM_::TimeMS()


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CPU : public STM429
{
public:

    static void Config();

    /// Установка каллбэка для клавиатуры
    static void SetCallbackKeyboard(void (*func)());

    static void InitKeyboardInputs(uint16 sl[], char portSL[], int numSL, uint16 rl[], char portRL[], int numRL);

    //------------------------------------------------------------------------------------------------------------------------------------------------
    class _LTDC_
    {
    public:

        static void SetColors(uint clut[], uint numColors);

        static void SetBuffers(uint frontBuffer, uint backBuffer);

        static void ToggleBuffers();
    };

    //------------------------------------------------------------------------------------------------------------------------------------------------
    class _SPI4_
    {
    public:
        /// Переслать массив данных
        static void Transmit(uint8 *buffer, uint16 size, uint timeOut);
        /// Переслать массив данных с одновременным приёмом второго массива
        static void TransmitReceive(uint8 *trans, uint8 *receiv, uint16 size, uint timeOut);
        /// Возвращает true, если интерфейс занят - процессор не может приниммать команды
        static bool IsBusy();
    };

    //------------------------------------------------------------------------------------------------------------------------------------------------
    class _TIM_
    {
    public:
        /// Запускает счётчик для измерения малых промежутков времени
        static void StartMultiMeasurement();
        /// \brief Количество тиков, прошедших с момента последнего вызова StartMultiMeasurement(). Не более (1 << 32)
        static uint TimeTicks();

        static uint TimeUS();

        static uint TimeMS();
    };

    //------------------------------------------------------------------------------------------------------------------------------------------------
    class _TIM3_
    {
    public:
        /// Завести таймер, который остановится в timeStop мс
        static void Start(uint timeStopMS);

        static void Stop();
    };

    //------------------------------------------------------------------------------------------------------------------------------------------------
    class _GPIO_
    {
    public:
        static void WritePin(char port, uint16 maskPin, bool state);

        static bool ReadPin(char port, uint16 maskPin);
    };

private:
    /// Инициализация SPI для связи с основным процессором
    static void InitSPI4();

    static void InitTIM2();

    static void InitTIM5();

    static void InitHardware();
    /// Инициализация дисплея
    static void InitLTDC();
    /// Инициализация шины для связи с ПЛИС
    static void InitFSMC();

    static void EnablePeriphery();
};
