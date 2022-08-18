// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


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
    static void Init(char port, uint pins, HMode::E mode, HPull::E pull, HSpeed::E speed = HSpeed::Low, HAlternate::E alternate = HAlternate::AF0_MCO);

    static void WritePin(char port, uint16 maskPin, bool state);

    static bool ReadPin(char port, uint16 maskPin);
};
