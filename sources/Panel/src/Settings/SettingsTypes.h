#pragma once
#include "defines.h"
#include "Command.h"
#include "CommonTypes.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define COMMON_ENUM     uint8 value; operator uint8() const { return value; }; operator uint8() { return value; }



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4514)
#endif

struct BillingTime
{
    enum
    {
        _1ms,
        _10ms,
        _100ms,
        _1000ms,
        _10000ms
    };
    COMMON_ENUM;
};

struct Language
{
    enum
    {
        RU,
        EN
    };
    COMMON_ENUM;
    Language(int v) : value((uint8)v)
    {
    };
};

struct SizeByte
{
    enum
    {
        _7bit,
        _8bit
    };
    COMMON_ENUM;
};

struct StopBit
{
    enum
    {
        _1,
        _2
    };
    COMMON_ENUM;
};

struct Parity
{
    enum
    {
        Off,
        On
    };
    COMMON_ENUM;
};

/// 
struct FreqInterval
{
    enum
    {
        _1,
        _10
    };
    COMMON_ENUM;
    FreqInterval(int v) : value((uint8)v)
    {
    }
};

/// Измерять частоту или период
struct FreqMeasure
{
    enum
    {
        Freq,
        Period
    };
    COMMON_ENUM;
    FreqMeasure(int v) : value((uint8)v)
    {
    }
};

/// Число усредняемых период в режиме измерения периода сигнала
struct FreqAvePeriod
{
    enum
    {
        _1,
        _10,
        _100,
        _1000,
        _10000
    };
    COMMON_ENUM;
};

/// Метки времени
struct FreqTimeStamps
{
    enum
    {
        _1kHz,
        _10kHz,
        _100kHz,
        _1MHz,
        _10MHz
    };
    COMMON_ENUM;
};

/// Тестовый режим частотомера
struct FreqTest
{
    enum
    {
        Off,
        On
    };
    COMMON_ENUM;
    FreqTest(int v) : value((uint8)v)
    {
    };
};


#ifdef WIN32
#pragma warning(pop)
#endif
