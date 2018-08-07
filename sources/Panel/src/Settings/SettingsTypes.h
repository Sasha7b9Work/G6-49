#pragma once
#include "Command.h"
#include "CommonTypes.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum BillingTime
{
    BillingTime_1ms,
    BillingTime_10ms,
    BillingTime_100ms,
    BillingTime_1000ms,
    BillingTime_10000ms
};

enum Language
{
    RU,
    EN
};

enum SizeByte
{
    Size_7bit,
    Size_8bit
};

enum StopBit
{
    StopBit_1,
    StopBit_2
};

enum Parity
{
    Parity_Off,
    Parity_On
};

/// 
enum FreqInterval
{
    FreqInterval_1,
    FreqInterval_10
};

/// Измерять частоту или период
enum FreqMeasure
{
    FreqMeasure_Freq,
    FreqMeasure_Period
};


float MinValue(Type_WaveParameter param);
float MaxValue(Type_WaveParameter param);
