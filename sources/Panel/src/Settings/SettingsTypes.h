#pragma once
#include "defines.h"
#include "CommonTypes.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4514)
#endif

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
    FreqInterval(uint v) : value((E)v) {};
    operator uint8() const { return (uint8)value; };
};

/// Измерять частоту или период
struct FreqMeasure
{
    enum E
    {
        Freq,
        Period
    } value;
    FreqMeasure(uint v) : value((E)v) {};
    operator uint8() const { return (uint8)value; };
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
    operator uint8() const { return (uint8)value; };
};

/// Тестовый режим частотомера
struct FreqTest
{
    enum E
    {
        Off,
        On
    } value;
    FreqTest(uint v) : value((E)v) {};
    operator uint8() const { return (uint8)value; };
};


#ifdef WIN32
#pragma warning(pop)
#endif
