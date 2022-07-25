// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Common.h"
#include "common/CommonTypes.h"


struct Calibrator
{
    // ������������� ����������� ����������, ������������ ������������ ������������ ��� ����� ������������ ���������
    // param : 0 - ���������
    //         1 - +2.5� / +5�
    //         2 - 0�
    //         3 - -2.5� / -5�
    static void SetK(uint8 channel, uint8 signal, uint8 range, uint8 param, int16 k);
    
    // ���������� ����������� ���������� ���������
    static float GetAmplitudeK(Chan::E ch);
    
    // ���������� ������������� ����������� �������� ��� 0�
    static float GetOffsetK_Zero(Chan::E ch);
    
    // ���������� ������������� ����������� �������� ��� -5�
    static double GetOffsetK_Negative(Chan::E ch);
    
    // ���������� ������������� ����������� �������� ��� +5�
    static double GetOffsetK_Positive(Chan::E ch);
    
    // ��������� �������������� ������������ ������ ������������� �����������
    static void SetFreqMeterK_Trig(int16 k);

    // ���������� ������������� ����������� ������ ������������� �����������
    static float GetFreqMeterK_Trig();

private:
    static uint8 CalculateRange(Chan::E ch);
    
    // ������������� ����� ������� ��� �������������� ����� �������
    static void SetFormWave(Chan::E ch, uint8 signal);
    
    // ���������� ��������� ��� ��������� ���������
    static void SetAmplitude(Chan::E ch, bool zeroAmplitude);
    
    // ���������� �������� ��� ��������� ���������
    static void SetOffset(Chan::E ch, uint8 param);
    
    // ���� true, �� �� ��������� � ������ ����������, � �������� ����� �� ������������, ������ �� �������������� ��������, � ����� ���, ������� ������ �����������
    static bool inModeCalibration;
    
    // ��� ����� ��������� ������ ���������� ����������
    static uint8 range[Chan::Count];
    
    // ��� ���� ����� ������� ���������� ����������. 0 - �����, 1 - ������������
    static uint8 signal[Chan::Count];
};
