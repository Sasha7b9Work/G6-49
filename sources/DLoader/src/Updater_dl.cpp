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
    2.1 Стирается столько секторов, сколько нужно, чтобы зашить основную прошивку
    2.2 В PLoader полааются сообщения PortionUpgradeDevice для оповещения о процессе обновления. Они посылаются при 0, 1, 2 и т.д. процентах обновления.
    3. Поиск на флешке файла G6-49-P.bat. Если файл обнаружен, происходит следующиее.
    3.1 Посылается сообщение AnswerUpgrade с информацией о размере прошивки.
    3.2 Далее PLoader начинает запрашивать "порции" прошивки размером SIZE_CHUNK с помощью команды RequestPortionUpgradePanel. В ответ на каждую такую команду DLoader посылает команду
        AnswerPortionUpgradePanel с SIZE_CHUNK байтами прошивки, порядковым номером порции (для расчёта начального адреса) и контрольной суммой порции данных.
        Когда все порции получены и успешно верифицированы, PLoader засылает RequestPortionUpgradePanel с номером 65535 - это означает конец обновления.
    4. В конце обновления (или вместо, если запрос на обновление не пришёл) DLoader засылает команду StartApplication
*/



#define FILE_NAME_DEVICE "G6-49-D.bin"
#define FILE_NAME_PANEL  "G6-49-P.bin"

static bool needUpgrade = false;

struct StructUpgradePanel
{
    StructUpgradePanel() : inProcess(false), sizeFirmware(-1), sector(-1) { }

    bool inProcess;     // Установленное в true значение означает, что идёт процесс апгрейда panel
    int sizeFirmware;   // Размер прошивки Panel
    int sector;         // Этот сектор прошивки Panel сейчас записан в сектор EEPROM::TEMP. -1, если сектор не записан
};


volatile static StructUpgradePanel sup;


// Пустой обработчик сообщений
static void E(SimpleMessage *);

// Обработчик запроса на обновление
static void OnRequestUpgrade(SimpleMessage *);

// Обработчик запроса на очередную порцию данных прошивки
static void OnRequestPortionUpgradePanel(SimpleMessage *);

// Послать сообщение о текущем состоянии обновления device
static void SendMessageAboutDevicePortion(int size, int fullSize);

static int CalculatePortion(int size, int fullSize);


void Updater::Handler(SimpleMessage *message)
{
    typedef void(*pFunc)(SimpleMessage *);

    static const pFunc funcs[Command::Count] =
    {
        /* RequestData                */ E,
        /* EnableChannel              */ E,
        /* SetFormWave                */ E,
        /* SetFrequency               */ E,
        /* SetAmplitude               */ E,
        /* SetOffset                  */ E,
        /* SetDuration                */ E,
        /* SetDutyRatio               */ E,
        /* SetPhase                   */ E,
        /* ModeDebug                  */ E,
        /* SetDelay                   */ E,
        /* WriteRegister              */ E,
        /* SetDurationRise            */ E,
        /* SetDurationFall            */ E,
        /* SetDurationStady           */ E,
        /* SetDutyFactor              */ E,
        /* SetManipulation            */ E,
        /* SetManipulationDuration    */ E,
        /* SetManipulationPeriod      */ E,
        /* SetPacketPeriod            */ E,
        /* SetPacketNumber            */ E,
        /* SetStartMode               */ E,
        /* SetPeriod                  */ E,
        /* SetPolarity                */ E,
        /* LoadFromDDS                */ E,
        /* FreqMeasure                */ E,
        /* Log                        */ E,
        /* FDrive_NumDirsAndFiles     */ E,
        /* FDrive_Mount               */ E,
        /* FDrive_RequestDir          */ E,
        /* FDrive_RequestFile         */ E,
        /* Test                       */ E,
        /* FDrive_RequestFileSize     */ E,
        /* FDrive_RequestFileString   */ E,
        /* FDrive_LoadFromExtStorage  */ E,
        /* FDrive_GetPictureDDS       */ E,
        /* SCPI_RecvData              */ E,
        /* PortCPU                    */ E,
        /* CalibrationLoad            */ E,
        /* CalibrationSet             */ E,

        /* StartApplication           */ E,
        /* RequestUpgrade             */ OnRequestUpgrade,
        /* PortionUpgradeDevice       */ E,
        /* AnswerUpgradePanel         */ E,
        /* RequestPortionUpgradePanel */ OnRequestPortionUpgradePanel,
        /* AnswerPortionUpgradePanel  */ E
    };

    message->ResetPointer();

    uint8 com = message->TakeUINT8();

    if(com < Command::Count)
    {
        pFunc func = funcs[com];

        func(message);
    }
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
        int numSectors = fullSize / (128 * 1024) + 1;

        HAL_EEPROM::EraseSectors(numSectors);

        uint address = MAIN_PROGRAM_START_ADDRESS;

        static uint8 buffer[SIZE_CHUNK];

        int size = fullSize;

        while(size > 0)
        {
            int readed = (size < SIZE_CHUNK) ? size : SIZE_CHUNK;
            size -= readed;

            DLDrive::File::Read(readed, buffer);

            HAL_EEPROM::WriteBuffer(address, buffer, readed);

            address += readed;

            SendMessageAboutDevicePortion(size, fullSize);
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

        sup.inProcess = true;       // Устанавливаем признак обновления панели

        while(sup.inProcess)        // И ждём, пока он не выйдет из этого состояния.
        {
        }

        DLDrive::File::Close();

//        int numSectors = fullSize / (128 * 1024) + 1;
//
//        static uint8 buffer[SIZE_CHUNK];
//
//        int size = fullSize;
//
//        int numPortion = 0;
//
//        while(size > 0)
//        {
//            int readed = (size < SIZE_CHUNK) ? size : SIZE_CHUNK;
//            size -= readed;
//
//            DLDrive::File::Read(readed, buffer);
//
//            SendMessageAboutPanelPortion(numPortion++, buffer, size, fullSize);
//        }
//
//        DLDrive::File::Close();
    }
}


static void OnRequestPortionUpgradePanel(SimpleMessage *msg)
{
    uint num = msg->TakeUINT();

    if(num == 0xFFFF)               // Если запрос на порцию 65535, это признак того, что процесс обновления завершён
    {
        sup.inProcess = false;
    }
    else
    {

    }
}


static void SendMessageAboutDevicePortion(int size, int fullSize)
{
    static int prevPortion = -1;        // Какая часть обновления уже случилась. Изменяется от 0 до 100. Засылается только когда изменилось

    int portion = CalculatePortion(size, fullSize);

    if(portion != prevPortion)
    {
        prevPortion = portion;

        Message::PortionUpgradeDevice(portion).TransmitAndSend();
    }
}


static int CalculatePortion(int size, int fullSize)
{
    return static_cast<int>((1.0F - static_cast<float>(size) / fullSize) * 100);
}


bool Updater::NeedUpgrade()
{
    return needUpgrade;
}
