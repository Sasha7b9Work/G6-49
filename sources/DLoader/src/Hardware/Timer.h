// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


 // @brief ���������� �����, ��������� � ������� ���������� ������ ������� Timer_StartMultiMeasurement(). �� ����� (1 << 32)
 // � ����� ������� 120.000.000 ����� ��� �8-53 � 90.000.000 ����� ��� �8-54.
 // ������������ ������� �������, ������� ����� ��������� � � ������� - 35 ���.
#define TIME_TICKS (HAL_TIM2::GetTicks())
#define TIME_US    (HAL_TIM2::TimeUS())
#define TIME_MS    (HAL_TIM::TimeMS())

struct Timer
{
    struct Type
    {
        enum E 
        {
            kPressKey,                  // ����� �������������, ����� �������� ������� �������.
            kShowLevelRShiftA,          // ����� �������������, ����� ���������� ��������� ����� �������� ������ 1.
            kShowLevelRShiftB,          // ����� �������������, ����� ���������� ��������� ����� �������� ������ 2.
            kShowLevelTrigLev,          // ����� �������������, ����� ���������� ��������� ����� ������ �������������
            kENumSignalsInSec,          // ��� ��������� ���������� ���������� ������� � �������.
            kFlashDisplay,              // ������ ��� ��������� �������� ������ �����-������ �������.
            kShowMessages,              // ������ ��� ��������� ����� ������ �������������� � ��������������� ���������.
            kMenuAutoHide,              // ������ ��� ������� ������� ��������� ����.
            kRShiftMarkersAutoHide,     // ������ ��� ������� ������� ��������� �������������� ������� �����.
            kUSB,                       // ���������������, ��� ����� ����.
            kStopSound,                 // ��������� ����
            kTemporaryPauseFPGA,        // ��������� ����� ��� �������� ������� �� ������ ����� �������� �����
            kStrNaviAutoHide,           // ������� ������ ��������� ����
            kTimerStartP2P,             // ������ ��� ������� ���������� ����� ����������� ������
            kTimerDisplay,              // ������ �� ������ ��������� ������
            USB,
            kTemp,
            Count
        } value;
        Type(E v) : value(v) {};
        operator uint8() const { return (uint8)value; };
    };

    static void Init();

    // ��������� ������� timer ������� � ����� ������������
    static void Set(Type type, pFuncVV func, uint dTms);

    static void SetAndStartOnce(Type type, pFuncVV func, uint dTms);

    static void SetAndEnable(Type type, pFuncVV func, uint dTms);

    static void StartOnce(Type type);

    static void Enable(Type type);

    static void Disable(Type type);

    static bool IsRun(Type type);

    static void PauseOnTicks(uint numTicks);
    // ������������� ��������� ����� ������������. ����� ������ Timer_LogPoint �������� ��������� ��������� �� ��� �����
    static void StartLogging();

    static uint LogPointUS(char *name);

    static uint LogPointMS(char *name);
    // ���������� ��� ������������ �������
    static void ElapsedCallback();
};
