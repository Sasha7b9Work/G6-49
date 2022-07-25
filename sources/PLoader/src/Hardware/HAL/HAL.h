// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Display/Colors.h"


#define ERROR_HANDLER() HAL::ErrorHandler()


struct CalibrationSettings;


struct HAL
{
	static void Init();

    static void ErrorHandler();
};


struct HAL_CRC32
{
    static void Init();

    static uint CalculateALIGN32(void *address, int size);
};


struct HAL_EEPROM
{
    // ������� count ��������, ������� �� ���������� ������ �������� ��������
    static void EraseSectors(int count);

    static void WriteBuffer(int address, const uint8 *data, int size);
};


struct HAL_LTDC
{
    static void Init();

    static void SetColors(uint *clut, uint8 numColors);

    static void ToggleBuffers();

    static void FillRegion(int x, int y, int width, int height, Color color);

    static uint8 *GetBuffer();
};


struct HAL_SPI4
{
    static void Init();
    // ��������� ������ ������
    static bool Transmit(const void *buffer, int size, int timeout);

    static bool Transmit(uint value, int timeout);
    static bool Transmit(int value, int timeout);
    // ������� ������ ������
    static bool Receive(void *recv, int size, int timeout);
    // ������� ��������� size ���� � ���������� �� � compared. ���������� ���������� ��������� ����
    static uint ReceiveAndCompare(const void *compared, int size);
    // ���������� true, ���� ������ ����� � ������
    static bool IsReady();
    // �������� �������� ������� ���������� �� ��������� "�����" � ��������� "��������"
    static void WaitFalling();
};


struct HAL_TIM
{
    static uint TimeMS();

    static void Delay(uint timeMS);
};


// ����������� ��� �����
struct HAL_TIM2
{
    static void Init();

    static void DeInit();

    static uint TimeUS();

    static uint GetTicks();
    // ��������� ������� ��� ��������� ����� �������� �������
    static void StartMultiMeasurement();
};


// ��� ��������
struct HAL_TIM3
{
    static void Init();

    static void StartIT(uint period);

    static void StopIT();

    static void DeInit();
};


struct HAL_TIM4
{
    static void Init(void (*func)());
    // ������� ������, ������� ����������� � timeStop ��
    static void Start(uint timeStopMS);

    static void Stop();

    static void ElapsedCallback();
};
