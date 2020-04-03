#include "defines.h"
#include "common/Messages.h"
#include "Updater_pl.h"
#include "Display/Painter.h"


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

    uint8 com = message->TakeByte();

    if(com == Command::PortionUpdateDevice)
    {
        uint word = message->TakeWord();
        
        portionDevice = word / 100.0F;
    }

    return true;
}
