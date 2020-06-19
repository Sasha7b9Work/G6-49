#include "defines.h"
#include "common/Messages.h"
#include "common/Messages_l.h"
#include "Updater_pl.h"
#include "Display/Painter.h"
#include "Hardware/HAL/HAL.h"


struct StructUpgradeDevice
{
    StructUpgradeDevice() : portionUpgrade(-1.0F) { }
    float portionUpgrade;
};


struct StructUpgradePanel
{
    StructUpgradePanel() : sizeFirmware(-1), numChunk(-1), needRequest(false) { }
    int sizeFirmware;                   // Общий размер прошивки
    int16 numChunk;                     // Текущий обрабатываемый чанк
    bool needRequest;                   // Если true, то нужно посылать запрос на numChunk "чанк"
    float PortionUpgrade();             // Возвращает процент записанного объёма прошивки
    bool LastChunk();                   // Возвращает true, если numChunk - последний "чанк" из прошивки
};


static StructUpgradePanel sup;
static StructUpgradeDevice sud;

static void Draw();

static void DrawProgress(int y, float portion);

// Записать новвый чанк в EEPROM
static void WriteNewChunk(int16 num, uint crc, uint8 chunk[SIZE_CHUNK]);


void Updater::Update()
{
    Painter::BeginScene(Color::BLACK);

    Draw();

    Painter::EndScene();

    if(sup.needRequest)
    {
        Message::RequestPortionUpgradePanel(sup.numChunk).Transmit();
        sup.needRequest = false;
    }
}


static void Draw()
{
    static const int yDevice = 50;
    static const int yPanel = 150;

    if(!(sud.portionUpgrade < 0.0F))
    {
        DrawProgress(yDevice, sud.portionUpgrade);
    }

    if(!(sup.PortionUpgrade() < 0.0F))
    {
        DrawProgress(yPanel, sup.PortionUpgrade());
    }
}


static void DrawProgress(int y, float portion)
{
    static const int width = 300;
    static const int height = 20;
    static const int x = 10;

    Painter::DrawRectangle(x, y, width, height, Color::WHITE);

    Painter::FillRegion(x, y, static_cast<int>(width * portion), height);
}


bool Updater::Handler(SimpleMessage *message)
{
    message->ResetPointer();

    uint8 com = message->TakeUINT8();

    switch(com)
    {
    case Command::PortionUpgradeDevice:
        sud.portionUpgrade = message->TakeUINT() / 100.0F;
        break;

    case Command::AnswerUpgradePanel:
        sup.sizeFirmware = message->TakeINT();
        sup.numChunk = 0;
        sup.needRequest = true;
        HAL_EEPROM::EraseSectors(sup.sizeFirmware / (128 * 1024) + 1);
        break;

    case Command::AnswerPortionUpgradePanel:
        int16 num = message->TakeINT16();
        uint crc = message->TakeUINT();
        WriteNewChunk(num, crc, message->TakeData(1 + sizeof(int16) + sizeof(uint)));
        break;
    }

    return true;
}


float StructUpgradePanel::PortionUpgrade()
{
    if(numChunk == -1)
    {
        return -1.0F;
    }

    return static_cast<float>(numChunk * SIZE_CHUNK) / sizeFirmware;
}


bool StructUpgradePanel::LastChunk()
{
    return ((numChunk * SIZE_CHUNK) + SIZE_CHUNK) >= sizeFirmware;
}


static void WriteNewChunk(int16 num, uint crc, uint8 chunk[SIZE_CHUNK])
{
    if(num == sup.numChunk && HAL_CRC32::CalculateALIGN32(chunk, SIZE_CHUNK) == crc)
    {
        HAL_EEPROM::WriteBuffer(Updater::MAIN_PROGRAM_START_ADDRESS + num * SIZE_CHUNK, chunk, SIZE_CHUNK);

        if(sup.LastChunk())
        {
            sup.numChunk = -1;
        }
        else
        {
            sup.numChunk++;
        }
    }

    sup.needRequest = true;
}
