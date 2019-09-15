#pragma once
#include "Hardware/Controls.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define RTC_GET_PACKED_TIME() {}
#define RTC_SET_TIME_AND_DATA(d, m, y, h, min, s)

#define ERROR_HANDLER() CPU::ErrorHandler()


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CPU
{
public:

    static void Init();

    static void Update();

    static float GetFPS();

    static void ErrorHandler();

    //----------------------------------------------------------------------------------------------------------------------------------- Keyboard ---
    class Keyboard
    {
    friend class CPU;

    private:

        static void Init();

    public:
        /// Установка каллбэка для клавиатуры
        static void SetCallback(void(*func)());

        static void InitInputs(const uint16 *sl, const char *portSL, int numSL, const uint16 *rl, const char *portRL, int numRL);
        /// Возвращает true, если буфер пуст
        static bool BufferIsEmpty();
        /// Возвращает следующий орган управления, если таковой имеется
        static KeyEvent GetNextControl();
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

    //-------------------------------------------------------------------------------------------------------------------------------------- GPIO ----
    class GPIO_
    {
    public:
        static void WritePin(char port, uint16 maskPin, bool state);

        static bool ReadPin(char port, uint16 maskPin);
    };

    //--------------------------------------------------------------------------------------------------------------------------------------- FSMC ---
    // Шина для связи с ПЛИС
    class FSMC
    {
    friend class CPU;

    private:
        static void Init();
    };

    //-------------------------------------------------------------------------------------------------------------------------------------- CRC32 ---
    class CRC32
    {
    public:
        static uint Calculate(const void *address, uint size);

    private:
        friend class CPU;

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


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
