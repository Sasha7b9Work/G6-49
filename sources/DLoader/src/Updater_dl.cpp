#include "defines.h"
#include "common/Command.h"
#include "common/Messages_pl.h"
#include "Updater_dl.h"
#include "FDrive/FDrive_dl.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Interface/Interface_dl.h"

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


#define NAME_DEVICE "G6-49-D.bin"
#define NAME_PANEL "G6-49-P.bin"


static bool needUpgrade = false;

static void E(SimpleMessage *);
static void OnRequestUpdate(SimpleMessage *);


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
        /* RunReset                  */ E,
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


void Updater::Upgrade()
{
    uint start = TIME_MS;

    while(TIME_MS - start < 10000 && !DLDrive::IsConnected())
    {
        DLDrive::Update();
    }

    if(!DLDrive::IsConnected())
    {
        return;
    }

    static const int SIZE_CHUNK = 128;    /* Размер элементарной порции данных */

    const int fullSize = DLDrive::File::Open(NAME_DEVICE);

    if(fullSize != -1)
    {
        int numSectors = fullSize / (128 * 1024) + 1;

        HAL_EEPROM::EraseSectors(numSectors);

        uint address = Updater::MAIN_PROGRAM_START_ADDRESS;

        uint8 buffer[SIZE_CHUNK];

        static int prevPortion = -1;        // Какая часть обновления уже случилась. Изменяется от 0 до 100. Засылается только когда изменилось

        int size = fullSize;

        while(size > 0)
        {
            int readed = (size < SIZE_CHUNK) ? size : SIZE_CHUNK;
            size -= readed;

            DLDrive::File::Read(readed, buffer);

            HAL_EEPROM::WriteBuffer(address, buffer, readed);

            address += readed;

            int portion = static_cast<int>(1.0F - static_cast<float>(size) / fullSize * 100);

            if(portion != prevPortion)
            {
                Message::PortionUpdateDevice(portion).Transmit();
                prevPortion = portion;

                while(DInterface::GetOutbox().Size())
                {
                    DInterface::Update();
                }
            }
        }

        DLDrive::File::Close();
    }
}


bool Updater::NeedUpgrade()
{
    return needUpgrade;
}
