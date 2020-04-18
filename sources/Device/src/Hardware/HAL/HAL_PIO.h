#pragma once
#include "Hardware/HAL/HAL_PINS.h"


struct HPort
{
    enum E
    {
        _A,
        _B,
        _C,
        _D,
        _E,
        _F,
        _G,
        _H,
        Count
    };
};

struct HPin
{
    static const uint16 _0;
    static const uint16 _1;
    static const uint16 _2;
    static const uint16 _3;
    static const uint16 _4;
    static const uint16 _5;
    static const uint16 _6;
    static const uint16 _7;
    static const uint16 _8;
    static const uint16 _9;
    static const uint16 _10;
    static const uint16 _11;
    static const uint16 _12;
    static const uint16 _13;
    static const uint16 _14;
    static const uint16 _15;
};


struct StructPIN
{
    HPort::E port;
    uint16   pin;
};


struct HState
{
    enum E
    {
        Reset,
        Set,
        Count
    };
};

struct HMode
{
    enum E
    {
        Analog,
        Output_PP,
        RisingIT,
        Input,
        AF_PP,
        Count
    };
};

struct HPull
{
    enum E
    {
        No,
        Down,
        Up,
        Count
    };
};

struct HSpeed
{
    enum E
    {
        Low,
        High,
        VeryHigh,
        Count
    };
};

struct HAlternate
{
    enum E
    {
        AF0_MCO,
        AF5_SPI1,
        AF7_USART3,
        AF10_OTG_FS,
        AF12_OTG_HS_FS,
        Count
    };
};

struct HAL_PIO
{
    // Начальная инициализация управляющих выводов
    static void Init();

    static void Init(HPort::E port, uint pins, HMode::E mode, HPull::E pull, HSpeed::E speed = HSpeed::Low, HAlternate::E alternate = HAlternate::AF0_MCO);
    // Установка в "1"
    static void Set(HPort::E port, uint16 pin);
    static void Set(const StructPIN &pin);
    // Установка в "0"
    static void Reset(HPort::E port, uint16 pin);
    static void Reset(const StructPIN &pin);

    static void Write(HPort::E port, uint16 pin, HState::E state);
    static void Write(HPort::E port, uint16 pin, bool state);
    static uint8 Read(HPort::E port, uint16 pin);

    // Запись байта в ПЛИС
    static void WriteFPGA(uint8 byte);
};
