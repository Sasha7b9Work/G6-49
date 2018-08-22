#pragma once
#include "defines.h"
#include "Command.h"
#include "CommonTypes.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#undef COMMON_ENUM
#define COMMON_ENUM     uint8 value; operator uint8() const { return value; }; operator uint8() { return value; }



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4514)
#endif

struct BillingTime
{
    enum EBillingTime
    {
        _1ms,
        _10ms,
        _100ms,
        _1000ms,
        _10000ms
    } value;
    operator int() const { return (int)value; };
};

struct Language
{
    enum ELanguage
    {
        RU,
        EN
    } value;
    Language(int v) : value((ELanguage)v) {};
    operator uint8() const { return (uint8)value; };
};

struct SizeByte
{
    enum ESizeByte
    {
        _7bit,
        _8bit
    } value;
};

struct StopBit
{
    enum EStopBit
    {
        _1,
        _2
    } value;
};

struct Parity
{
    enum EParity
    {
        Off,
        On
    } value;
};

/// 
struct FreqInterval
{
    enum EFreqInterval
    {
        _1,
        _10
    } value;
    FreqInterval(int v) : value((EFreqInterval)v) {};
    operator uint8() const { return (uint8)value; };
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
