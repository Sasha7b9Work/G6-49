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

// Переписать с флешки сектор sector в EEPROM
static void WriteSectorToEEPROM(int sector);


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
        int numSectors = fullSize / (128 * 1024) + 1;

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

        sup.inProcess = true;       // Устанавливаем признак обновления панели

        while(sup.inProcess)        // И ждём, пока он не выйдет из этого состояния.
        {
            DInterface::Update();
        }

        DLDrive::File::Close();
    }
}


static void OnRequestPortionUpgradePanel(SimpleMessage *msg)
{
    int16 num = msg->TakeINT16();

    if(num == 0xFFFF)                           // Если запрос на порцию 65535, это признак того, что процесс обновления завершён
    {
        sup.inProcess = false;
    }
    else
    {
        int address = num * SIZE_CHUNK;         // Адрес относительно начала прошивки

        int sector = address / (128 * 1024);    // Порядковый номер сектора, в котором находится запрашиваемый "чанк"

        if (sector != sup.sector)
        {
            WriteSectorToEEPROM(sector);        // Переписываем сектор с флешки в EEPROM, если это необходимо

            sup.sector = sector;
        }

        address = address - sector * (128 * 1024) + HAL_EEPROM::ADDRESS_SECTOR_TEMP;    // Теперь это адрес нашего чанка относительно начала сектора

        Message::AnswerPortionUpgradePanel(num, reinterpret_cast<uint8 *>(address)).TransmitAndSend();
    }
}


static void WriteSectorToEEPROM(int sector)
{
    HAL_EEPROM::EraseSectorTemp();

    static const int SIZE_STRING = 1024;

    DLDrive::File::Seek(sector * 128 * SIZE_STRING);        // Устанавливаем указатель в файле на начало сектора

    uint8 buffer[SIZE_STRING];                              // Сюда будем считывать файл сюда

    int readed = SIZE_STRING;                               // Здесь будет храниться количество реально считанных байт

    int address = HAL_EEPROM::ADDRESS_SECTOR_TEMP;          // С этого адреса будем сохранять сектор в EEPROM

    for (int i = 0; (i < 128) && (readed == SIZE_STRING); i++)
    {
        readed = DLDrive::File::Read(SIZE_STRING, buffer);  // Читаем очередную порцию данных

        HAL_EEPROM::WriteBuffer(address, buffer, readed);   // Записываем её в EEPROM

        address += SIZE_STRING;                             // Следующую порцию данных будем записывать по этому адресу
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
