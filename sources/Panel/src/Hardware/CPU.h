#pragma once
#include "common/stm32/429/stm429.h"


class CPU : public STM429
{
public:

    static void Config();

    static void EnablePeriphery();

    static void InitHardware();
    /// Инициализация дисплея
    static void InitLTDC();
    /// Инициализация шины для связи с ПЛИС
    static void InitFSMC();

    /// Установка каллбэка для клавиатуры
    static void SetCallbackKeyboard(void (*func)());

    static void InitKeyboardInputs(uint16 sl[], char portSL[], int numSL, uint16 rl[], char portRL[], int numRL);
};


#ifdef __cplusplus
extern "C" {
#endif

void LTDC_IRQHandler();
void TIM3_IRQHandler();

#ifdef __cplusplus
}
#endif
