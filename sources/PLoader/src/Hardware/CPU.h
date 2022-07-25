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
    
    // ����� ������ ������ ��� (����� ������ ���������� 1-���������� ������� �������)
    static uint timeStartMeasFPS;
    
    // ������� ������� ��������� � ������ ���������� 1-���������� ������� �������
    static int numFrames;

    static float fps;
};
