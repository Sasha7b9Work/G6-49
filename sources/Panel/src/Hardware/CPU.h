#pragma once
#include "Hardware/Controls.h"

#ifdef OPEN
#include "Hardware/stm746.h"
#else
#include "Hardware/stm429.h"
#endif

#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4514)
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define RTC_GET_PACKED_TIME() {}
#define RTC_SET_TIME_AND_DATA(d, m, y, h, min, s)


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef OPEN
class CPU : public STM746
#else
class CPU : public STM429
#endif
{
public:

    static void Init();

    static void Update();

    static float GetFPS();

    //----------------------------------------------------------------------------------------------------------------------------------- Keyboard ---
    class Keyboard
    {
    friend class CPU;

    private:

        static void Init();

    public:
        /// Установка каллбэка для клавиатуры
        static void SetCallback(void(*func)());

        static void InitInputs(uint16 sl[], char portSL[], int numSL, uint16 rl[], char portRL[], int numRL);
        /// Возвращает true, если буфер пуст
        static bool BufferIsEmpty();
        /// Возвращает следующий орган управления, если таковой имеется
        static Control GetNextControl();
        /// Функция-заглушка. Для эмулятора, на котором нет физической клавиатуры и нужно её рисовать на экране
        static void Draw();

        //----------------------------------------------------------------------------------------------------------------------------------- TIM4 ---
        class TIM4_
        {
        public:
            /// Завести таймер, который остановится в timeStop мс
            static void Start(uint timeStopMS);

            static void Stop();

            static void ElapsedCallback(void *htim);
        };

    private:

        static void Update();
    };

    //--------------------------------------------------------------------------------------------------------------------------------------- SPI4 ---
    // SPI для связи с основным процессором
    class SPI4_
    {
    friend class CPU;
    private:
        static void Init();
    public:
        /// Переслать массив данных
        static void Transmit(const void *buffer, uint16 size);
        /// Переслать массив данных с одновременным приёмом второго массива
        static bool TransmitReceive(void *trans, void *receiv, uint16 size);
        /// Принять массив данных
        static void Receive(void *recv, uint16 size);
        /// Возвращает true, если интерфейс занят - процессор не может приниммать команды
        static bool IsBusy();
    };

    //-------------------------------------------------------------------------------------------------------------------------------------- GPIO ----
    class GPIO_
    {
    public:
        static void WritePin(char port, uint16 maskPin, bool state);

        static bool ReadPin(char port, uint16 maskPin);
    };

    //-------------------------------------------------------------------------------------------------------------------------------------- FLASH ---
    class FLASH_
    {
    public:
        static void SaveSettings();
        static void LoadSettings();
        static void EraseSettings();
    private:
        static bool EraseSector(uint startAdddress);
        static void WriteBufferBytes(uint address, void *buffer, int numBytes);
        static uint GetSector(uint startAddress);
        static void ReadBufferBytes(uint addrSrc, void *bufferDest, int size);
    };

    //--------------------------------------------------------------------------------------------------------------------------------------- FSMC ---
    // Шина для связи с ПЛИС
    class FSMC
    {
    friend class CPU;

    private:
        static void Init();
    };

private:

    static void SystemClockConfig();
    /// Время начала замера ФПС (время начала очередного 1-секундного отрезка времени)
    static uint timeStartMeasFPS;
    /// Сколько фреймов насчитано с начала очередного 1-секундного отрезка времени
    static int numFrames;

    static float fps;
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif

    /// Прерывание на таймер. Timer.cpp
    void TIM3_IRQHandler();
    /// Прерывание на опрос клавиатуры. CPU_Keyboard.cpp
    void TIM4_IRQHandler();

    void NMI_Handler();
    void HardFault_Handler();
    void MemManage_Handler();
    void BusFault_Handler();
    void UsageFault_Handler();
    void SVC_Handler();
    void DebugMon_Handler();
    void PendSV_Handler();
    void SysTick_Handler();
    
#ifdef __cplusplus
}
#endif


#ifdef WIN32
#pragma warning(pop)
#endif
