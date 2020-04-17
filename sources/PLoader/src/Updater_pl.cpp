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
    StructUpgradePanel() : sizeFirmware(-1), numChunk(-1), portionUpgrade(-1.0F) { }
    int sizeFirmware;               // Общий размер прошивки
    int numChunk;                   // Текущий обрабатываемый чанк
    float portionUpgrade;
};


static StructUpgradePanel sup;
static StructUpgradeDevice sud;

static void Draw();

static void DrawProgress(int y, float portion);


void Updater::Update()
{
    Painter::BeginScene(Color::BLACK);

    Draw();

    Painter::EndScene();
}


static void Draw()
{
    static const int yDevice = 50;
    static const int yPanel = 150;

    if(!(sud.portionUpgrade < 0.0F))
    {
        DrawProgress(yDevice, sud.portionUpgrade);
    }

    if(!(sup.portionUpgrade < 0.0F))
    {
        DrawProgress(yPanel, sup.portionUpgrade);
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
        break;

    case Command::AnswerPortionUpgradePanel:
        break;
    }

    return true;
}
