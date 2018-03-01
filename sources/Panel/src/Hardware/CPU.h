#pragma once
#include "Hardware/Controls.h"
#include "Hardware/stm32/429/stm429.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define TIME_TICKS CPU::TIM_::TimeTicks()
#define TIME_US    CPU::TIM_::TimeUS()
#define TIME_MS    CPU::TIM_::TimeMS()


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CPU : public STM429
{
public:

    static void Init();

    static void OnIRQHandlerTIM3();

    //------------------------------------------------------------------------------------------------------------------------------------------------
    class Keyboard
    {
    public:

        static void Init();
        /// Установка каллбэка для клавиатуры
        static void SetCallback(void(*func)());

        static void InitInputs(uint16 sl[], char portSL[], int numSL, uint16 rl[], char portRL[], int numRL);
        /// Возвращает true, если буфер пуст
        static bool BufferIsEmpty();
        /// Возвращает следующий орган управления, если таковой имеется
        static StructControl GetNextControl();

    private:

        static void Update();
    };

    //------------------------------------------------------------------------------------------------------------------------------------------------
    class LTDC_
    {
    public:

        static void SetColors(uint clut[], uint numColors);

        static void SetBuffers(uint frontBuffer, uint backBuffer);

        static void ToggleBuffers();
    };

    //------------------------------------------------------------------------------------------------------------------------------------------------
    class SPI4_
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
    class TIM_
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
    class TIM3_
    {
    public:
        /// Завести таймер, который остановится в timeStop мс
        static void Start(uint timeStopMS);

        static void Stop();
    };

    //------------------------------------------------------------------------------------------------------------------------------------------------
    class GPIO_
    {
    public:
        static void WritePin(char port, uint16 maskPin, bool state);

        static bool ReadPin(char port, uint16 maskPin);
    };

    //------------------------------------------------------------------------------------------------------------------------------------------------
    class FLASH_
    {
    public:
        static void SaveSettings();
        static void LoadSettings();
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
