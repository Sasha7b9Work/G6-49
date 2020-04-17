#include "defines.h"
#include "common/Messages.h"
#include "common/Messages_pl.h"
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

    return (numChunk * SIZE_CHUNK) / sizeFirmware * 100.0F;
}


static void WriteNewChunk(int16 num, uint crc, uint8 chunk[SIZE_CHUNK])
{

}
