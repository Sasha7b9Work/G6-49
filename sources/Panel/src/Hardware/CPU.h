// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


#define PRIORITY_SOUND_DMA1_STREAM5 5, 0


#define RTC_GET_PACKED_TIME() {}
#define RTC_SET_TIME_AND_DATA(d, m, y, h, min, s)


namespace CPU
{
    void Init();

    void Update();

    float GetFPS();
};
