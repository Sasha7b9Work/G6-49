#pragma once
#include "defines.h"
#include "common/CommonTypes.h"


struct SizeByte
{
    enum E
    {
        _7bit,
        _8bit
    } value;
};

struct StopBit
{
    enum E
    {
        _1,
        _2
    } value;
};

struct Parity
{
    enum E
    {
        Off,
        On
    } value;
};

/// 
struct FreqInterval
{
    enum E
    {
        _1,
        _10
    } value;
    FreqInterval(uint v) : value(static_cast<E>(v)) {};
    operator uint8() const { return static_cast<uint8>(value); };
};

/// Измерять частоту или период
struct FreqMeasure
{
    enum E
    {
        Disable,
        Freq,
        Period
    } value;
    explicit FreqMeasure(uint v) : value(static_cast<E>(v)) {};
    operator uint8() const { return static_cast<uint8>(value); };
};

/// Метки времени
struct FreqTimeStamps
{
    enum E
    {
        _1kHz,
        _10kHz,
        _100kHz,
        _1MHz,
        _10MHz
    } value;
    operator uint8() const { return static_cast<uint8>(value); };
};

/// Тестовый режим частотомера
struct FreqTest
{
    enum E
    {
        Off,
        On
    } value;
    FreqTest(uint v) : value(static_cast<E>(v)) {};
    operator uint8() const { return static_cast<uint8>(value); };
};


struct Language
{
    enum E
    {
        RU,
        EN,
        Count
    };
};
