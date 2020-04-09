#include "defines.h"
#include "common/Messages.h"
#include "common/Messages_pl.h"
#include "Updater_pl.h"
#include "Display/Painter.h"
#include "Hardware/HAL/HAL.h"


static float portionDevice = 0.0F;
static float portionPanel = 0.0F;


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

    DrawProgress(yDevice, portionDevice);

    DrawProgress(yPanel, portionPanel);
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

    if(com == Command::EraseSectors)
    {
        int num = message->TakeINT();

        HAL_EEPROM::EraseSectors(num);
    }
    else if(com == Command::PortionUpgradeDevice)
    {
        portionDevice = message->TakeUINT() / 100.0F;
    }
    else if(com == Command::PortionUpgradePanel)
    {
        portionPanel = message->TakeUINT() / 100.0F;

        int num = message->TakeINT();

        uint address = static_cast<uint>(MAIN_PROGRAM_START_ADDRESS + num * SIZE_CHUNK);

        uint8 buffer[SIZE_CHUNK];

        for(int i = 0; i < SIZE_CHUNK; i++)
        {
            buffer[i] = message->TakeUINT8();
        }

        HAL_EEPROM::WriteBuffer(address, buffer, SIZE_CHUNK);
    }

    return true;
}
