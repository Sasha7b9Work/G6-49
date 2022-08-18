// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
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

namespace HPin
{
    extern const uint16 _0;
    extern const uint16 _1;
    extern const uint16 _2;
    extern const uint16 _3;
    extern const uint16 _4;
    extern const uint16 _5;
    extern const uint16 _6;
    extern const uint16 _7;
    extern const uint16 _8;
    extern const uint16 _9;
    extern const uint16 _10;
    extern const uint16 _11;
    extern const uint16 _12;
    extern const uint16 _13;
    extern const uint16 _14;
    extern const uint16 _15;
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

namespace HAL_PIO
{
    // Начальная инициализация управляющих выводов
    void Init();

    void Init(HPort::E port, uint pins, HMode::E mode, HPull::E pull, HSpeed::E speed = HSpeed::Low, HAlternate::E alternate = HAlternate::AF0_MCO);
    // Установка в "1"
    void Set(HPort::E port, uint16 pin);
    void Set(const StructPIN &pin);
    // Установка в "0"
    void Reset(HPort::E port, uint16 pin);
    void Reset(const StructPIN &pin);

    void Write(HPort::E port, uint16 pin, HState::E state);
    void Write(HPort::E port, uint16 pin, bool state);
    uint8 Read(HPort::E port, uint16 pin);

    // Запись байта в ПЛИС
    void WriteFPGA(uint8 byte);
};
