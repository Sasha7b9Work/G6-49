#include "defines.h"
#include "common/Command.h"
#include "common/Interface_d.h"
#include "common/Messages_l.h"
#include "Updater_dl.h"
#include "FDrive/FDrive_dl.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include <cstring>


/*

    ���������� ���������� ��������� �������.
    ��� ������� DLoader ������� ������� RequestUpdate. ���� ��� ������� � ������� ���������� �������, ����������� ������� ����������.
    ���� ������� ���������� ���.
    1. ������������, ���������� �� ������. ���� �� ���������� - �����.
    2. ����� �� ������ ����� G6-49-D.bat. ���� ���� ���������, ���������� ��������� �������.
    2.1 ��������� ������� ��������, ������� �����, ����� ������ �������� ��������
    2.2 � PLoader ��������� ��������� PortionUpgradeDevice ��� ���������� � �������� ����������. ��� ���������� ��� 0, 1, 2 � �.�. ��������� ����������.
    3. ����� �� ������ ����� G6-49-P.bat. ���� ���� ���������, ���������� ����������.
    3.1 ���������� ��������� AnswerUpgrade � ����������� � ������� ��������.
    3.2 ����� PLoader �������� ����������� "������" �������� �������� SIZE_CHUNK � ������� ������� RequestPortionUpgradePanel. � ����� �� ������ ����� ������� DLoader �������� �������
        AnswerPortionUpgradePanel � SIZE_CHUNK ������� ��������, ���������� ������� ������ (��� ������� ���������� ������) � ����������� ������ ������ ������.
        ����� ��� ������ �������� � ������� ��������������, PLoader �������� RequestPortionUpgradePanel � ������� 65535 - ��� �������� ����� ����������.
    4. � ����� ���������� (��� ������, ���� ������ �� ���������� �� ������) DLoader �������� ������� StartApplication
*/



#define FILE_NAME_DEVICE "G6-49-D.bin"
#define FILE_NAME_PANEL  "G6-49-P.bin"

static bool needUpgrade = false;

struct StructUpgradePanel
{
    StructUpgradePanel() : inProcess(false), sizeFirmware(-1) { }

    bool inProcess;     // ������������� � true �������� ��������, ��� ��� ������� �������� panel
    int sizeFirmware;   // ������ �������� Panel
};


volatile static StructUpgradePanel sup;


// ������ ���������� ���������
static void E(SimpleMessage *);

// ���������� ������� �� ����������
static void OnRequestUpgrade(SimpleMessage *);

// ���������� ������� �� ��������� ������ ������ ��������
static void OnRequestPortionUpgradePanel(SimpleMessage *);

// ������� ��������� � ������� ��������� ���������� device
static void SendMessageAboutDevicePortion(int size, int fullSize);

static int CalculatePortion(int size, int fullSize);


void Updater::Handler(SimpleMessage *message)
{
    typedef void(*pFunc)(SimpleMessage *);

    message->ResetPointer();

    uint8 com = message->TakeUINT8();

    pFunc func = E;
    
    switch(com)
    {
    case Command::RequestUpgrade:             func = OnRequestUpgrade;             break;
    case Command::RequestPortionUpgradePanel: func = OnRequestPortionUpgradePanel; break;
    }

    func(message);
}


static void E(SimpleMessage *)
{

}


static void OnRequestUpgrade(SimpleMessage *)
{
    needUpgrade = true;
}


void Updater::UpgradeDevice()
{
    const int fullSize = DLDrive::File::Open(FILE_NAME_DEVICE);

    if(fullSize != -1)
    {
        int numSectors = fullSize / (128 * 1024);

        if(fullSize % (128 * 1024))
        {
            numSectors++;
        }

        HAL_EEPROM::EraseSectors(numSectors);

        int address = MAIN_PROGRAM_START_ADDRESS;

        static uint8 buffer[SIZE_CHUNK];

        int size = fullSize;

        while(size > 0)
        {
            int readed = DLDrive::File::Read(SIZE_CHUNK, buffer);

            HAL_EEPROM::WriteBuffer(address, buffer, readed);

            SendMessageAboutDevicePortion(size, fullSize);

            size -= readed;
            address += readed;
        }

        DLDrive::File::Close();
    }
}


void Updater::UpgradePanel()
{
    sup.sizeFirmware = DLDrive::File::Open(FILE_NAME_PANEL);

    if(sup.sizeFirmware != -1)
    {
        Message::AnswerUpgradePanel(sup.sizeFirmware).TransmitAndSend();

        sup.inProcess = true;       // ������������� ������� ���������� ������

        while(sup.inProcess)        // � ���, ���� �� �� ������ �� ����� ���������.
        {
            DInterface::Update();
        }

        DLDrive::File::Close();
    }
}


static void OnRequestPortionUpgradePanel(SimpleMessage *msg)
{
    int16 num = msg->TakeINT16();

    if(num == -1)                               // ���� ������ �� ������ 65535, ��� ������� ����, ��� ������� ���������� ��������
    {
        sup.inProcess = false;
    }
    else
    {
        int address = num * SIZE_CHUNK;         // ����� ������������ ������ ��������

        DLDrive::File::Seek(address);

        uint8 buffer[1024];
        std::memset(buffer, 0xFF, 1024);

        DLDrive::File::Read(1024, buffer);

        Message::AnswerPortionUpgradePanel(num, buffer).Transmit();
    }
}

static void SendMessageAboutDevicePortion(int size, int fullSize)
{
    static int prevPortion = -1;        // ����� ����� ���������� ��� ���������. ���������� �� 0 �� 100. ���������� ������ ����� ����������

    int portion = CalculatePortion(size, fullSize);

    if(portion != prevPortion)
    {
        prevPortion = portion;

        Message::PortionUpgradeDevice(portion).TransmitAndSend();
    }
}


static int CalculatePortion(int size, int fullSize)
{
    return static_cast<int>((1.0F - static_cast<float>(size) / static_cast<float>(fullSize)) * 100);
}


bool Updater::NeedUpgrade()
{
    return needUpgrade;
}
