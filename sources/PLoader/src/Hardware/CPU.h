// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


#define PRIORITY_SOUND_DMA1_STREAM5 5, 0


#define RTC_GET_PACKED_TIME() {}
#define RTC_SET_TIME_AND_DATA(d, m, y, h, min, s)


class CPU
{
public:

    static void Init();

    static void Update();

    static float GetFPS();

private:
    
    // Время начала замера ФПС (время начала очередного 1-секундного отрезка времени)
    static uint timeStartMeasFPS;
    
    // Сколько фреймов насчитано с начала очередного 1-секундного отрезка времени
    static int numFrames;

    static float fps;
};
