#pragma once
#include "Command.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum Channel
{
    A,
    B,
    NumChannels
};

/// 
enum Interval
{
    Interval_1,
    Interval_10
};

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


float MinValue(Type_WaveParameter param);
float MaxValue(Type_WaveParameter param);
