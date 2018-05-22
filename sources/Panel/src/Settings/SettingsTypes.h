#pragma once
#include "Command.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum
{
    A,
    B,
    NumChannels
} Channel;

/// 
typedef enum
{
    Interval_1,
    Interval_10
} Interval;

typedef enum
{
    BillingTime_1ms,
    BillingTime_10ms,
    BillingTime_100ms,
    BillingTime_1000ms,
    BillingTime_10000ms
} BillingTime;

typedef enum
{
    RU,
    EN
} Language;

typedef enum
{
    SizeByte_7bit,
    SizeByte_8bit
} SizeByte;

typedef enum
{
    StopBit_1,
    StopBit_2
} StopBit;

typedef enum
{
    Parity_Off,
    Parity_On
} Parity;


float MinValue(WaveParameter param);
float MaxValue(WaveParameter param);
