#include "defines.h"
#include "common/Command.h"
#include "common/Messages_pl.h"
#include "Updater_dl.h"
#include "FDrive/FDrive_dl.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Interface/Interface_dl.h"
#include <cstring>


/*

    Обновление происходит следующим образом.
    При запуске DLoader ожидает команду RequestUpdate. Если она приходт в течение некоторого времени, запускается процесс обновления.
    Этот процесс происходит так.
    1. Определяется, подключена ли флешка. Если не подключена - выход.
    2. Поиск на флешке файла G6-49-D.bat. Если файл обнаружен, происходит следующий процесс.
    2.1 Определяется, сколько секторов нужно стереть.
    2.2 Посылается сообщение в панель о том, что начат процесс стирания секторов.
    2.3 После окончания стирания посылается сообщение о том, что стирание завершено.
    2.4 В панель посылается сообщение о начале записи
    2.5 С флешки блоками считывается двоичный файл с обновлением и записывается в память. После каждой порции в панель посылается сообщение о том, какая часть обновления завершена.
    2.6 По окончании записи в панель посылается сообщение о том, что обновление прошивки Device завершено.

*/


#define FILE_NAME_DEVICE "G6-49-D.bin"
#define FILE_NAME_PANEL  "G6-49-P.bin"


struct Mode
{
    enum E
    {
        Device,
        Panel
    };
};


static bool needUpgrade = false;

// Пустой обработчик сообщений
static void E(SimpleMessage *);

// Обработчик запроса на обновление
static void OnRequestUpdate(SimpleMessage *);

// Обновить прошивку из файла fileName (прошивку device или panel)
static void Upgrade(const char *fileName);

// Послать сообщение о текущем состоянии обновления, если необходимо
static void SendMessageToPanelIfNeed(Mode::E mode, int size, int fullSize);


void Updater::Handler(SimpleMessage *message)
{
    typedef void(*pFunc)(SimpleMessage *);

    static const pFunc funcs[Command::Count] =
    {
        /* RequestData               */ E,
        /* EnableChannel             */ E,
        /* SetFormWave               */ E,
        /* SetFrequency              */ E,
        /* SetAmplitude              */ E,
        /* SetOffset                 */ E,
        /* SetDuration               */ E,
        /* SetDutyRatio              */ E,
        /* SetPhase                  */ E,
        /* ModeDebug                 */ E,
        /* SetDelay                  */ E,
        /* WriteRegister             */ E,
        /* SetDurationRise           */ E,
        /* SetDurationFall           */ E,
        /* SetDurationStady          */ E,
        /* SetDutyFactor             */ E,
        /* SetManipulation           */ E,
        /* SetManipulationDuration   */ E,
        /* SetManipulationPeriod     */ E,
        /* SetPacketPeriod           */ E,
        /* SetPacketNumber           */ E,
        /* SetStartMode              */ E,
        /* SetPeriod                 */ E,
        /* SetPolarity               */ E,
        /* SetManipulationMode       */ E,
        /* LoadFromDDS               */ E,
        /* FreqMeasure               */ E,
        /* Log                       */ E,
        /* FDrive_NumDirsAndFiles    */ E,
        /* FDrive_Mount              */ E,
        /* FDrive_RequestDir         */ E,
        /* FDrive_RequestFile        */ E,
        /* Test                      */ E,
        /* SetKoeffCalibration       */ E,
        /* GetKoeffCalibration       */ E,
        /* FDrive_RequestFileSize    */ E,
        /* FDrive_RequestFileString  */ E,
        /* FDrive_LoadFromExtStorage */ E,
        /* FDrive_GetPictureDDS      */ E,
        /* SCPI_RecvData             */ E,
        /* PortCPU                   */ E,
        /* CalibrationLoad           */ E,
        /* CalibrationSet            */ E,
        /* StartApplication          */ E,
        /* RequestUpdate             */ OnRequestUpdate,
        /* PortionUpdateDevice       */ E
    };

    message->ResetPointer();

    uint8 com = message->TakeByte();

    if(com < Command::Count)
    {
        pFunc func = funcs[com];

        func(message);
    }
}


static void E(SimpleMessage *)
{

}


static void OnRequestUpdate(SimpleMessage *)
{
    needUpgrade = true;
}


void Updater::UpgradeDevice()
{
    Upgrade(FILE_NAME_DEVICE);
}


void Updater::UpgradePanel()
{
    Upgrade(FILE_NAME_PANEL);
}


static void Upgrade(const char *fileName)
{
    Mode::E mode = (std::strcmp(fileName, FILE_NAME_DEVICE) == 0 ? Mode::Device : Mode::Panel);

    static const int SIZE_CHUNK = 512;    /* Размер элементарной порции данных */

    const int fullSize = DLDrive::File::Open(fileName);

    if(fullSize != -1)
    {
        int numSectors = fullSize / (128 * 1024) + 1;

        HAL_EEPROM::EraseSectors(numSectors);

        uint address = Updater::MAIN_PROGRAM_START_ADDRESS;

        static uint8 buffer[SIZE_CHUNK];

        int size = fullSize;

        while(size > 0)
        {
            int readed = (size < SIZE_CHUNK) ? size : SIZE_CHUNK;
            size -= readed;

            DLDrive::File::Read(readed, buffer);

            HAL_EEPROM::WriteBuffer(address, buffer, readed);

            address += readed;

            SendMessageToPanelIfNeed(mode, size, fullSize);
        }

        DLDrive::File::Close();
    }
}


static void SendMessageToPanelIfNeed(Mode::E mode, int size, int fullSize)
{
    static int prevPortion = -1;        // Какая часть обновления уже случилась. Изменяется от 0 до 100. Засылается только когда изменилось

    int portion = static_cast<int>((1.0F - static_cast<float>(size) / fullSize) * 100);

    if(portion != prevPortion)
    {
        if(mode == Mode::Device)
        {
            Message::PortionUpdateDevice(portion).Transmit();
        }
        
        prevPortion = portion;

        while(DInterface::GetOutbox().Size())
        {
            DInterface::Update();
        }
    }
}


bool Updater::NeedUpgrade()
{
    return needUpgrade;
}
