// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Display/Colors.h"


#define ERROR_HANDLER() HAL::ErrorHandler()


struct CalibrationSettings;


namespace HAL
{
    void Init();

    void ErrorHandler();
};


namespace HAL_CRC32
{
    void Init();

    uint CalculateALIGN32(void *address, int size);
};


namespace HAL_EEPROM
{
    // ������� count ��������, ������� �� ���������� ������ �������� ��������
    void EraseSectors(int count);

    void WriteBuffer(int address, const uint8 *data, int size);
};


namespace HAL_LTDC
{
    void Init();

    void SetColors(uint *clut, uint8 numColors);

    void ToggleBuffers();

    void FillRegion(int x, int y, int width, int height, const Color &);

    uint8 *GetBuffer();
};


namespace HAL_SPI4
{
    void Init();

    // ��������� ������ ������
    bool Transmit(const void *buffer, int size, int timeout);

    bool Transmit(uint value, int timeout);

    bool Transmit(int value, int timeout);

    // ������� ������ ������
    bool Receive(void *recv, int size, int timeout);

    // ������� ��������� size ���� � ���������� �� � compared. ���������� ���������� ��������� ����
    uint ReceiveAndCompare(const void *compared, int size);

    // ���������� true, ���� ������ ����� � ������
    bool IsReady();

    // �������� �������� ������� ���������� �� ��������� "�����" � ��������� "��������"
    void WaitRelease();
};


namespace HAL_TIM
{
    uint TimeMS();

    void Delay(uint timeMS);
};


// ����������� ��� �����
namespace HAL_TIM2
{
    void Init();

    void DeInit();

    uint TimeUS();

    uint GetTicks();

    // ��������� ������� ��� ��������� ����� �������� �������
    void StartMultiMeasurement();
};


// ��� ��������
namespace HAL_TIM3
{
    void Init();

    void StartIT(uint period);

    void StopIT();

    void DeInit();
};


namespace HAL_TIM4
{
    void Init(void (*func)());
    // ������� ������, ������� ����������� � timeStop ��
    void Start(uint timeStopMS);

    void Stop();

    void ElapsedCallback();
};
