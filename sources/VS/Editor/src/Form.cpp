#include "defines.h"
#include "Canvas.h"
#include "Form.h"
#include "MyMath.h"


using namespace MyMath;


Form *TheForm = nullptr;

#define NUM_POINTS  (8192)
#define MAX_VALUE   ((1 << 12) - 1)
#define AVE_VALUE   ((MAX_VALUE) / 2)

// Данные, готовые для передачи в прибор
uint16 data[NUM_POINTS];


Form::Form()
{
    for (int i = 0; i < NUM_POINTS; i++)
    {
        data[i] = AVE_VALUE;
    }
}


void Form::SetPoint(int mouseX, int mouseY)
{
    wxSize size = TheCanvas->GetSize();

    int x = Round<int>(static_cast<float>(NUM_POINTS) / size.x * mouseX);

    uint16 y = Round<uint16>(static_cast<float>(MAX_VALUE) / size.y * mouseY);

    data[x] = static_cast<uint16>(y);
}


void Form::Draw()
{
    TheCanvas->SetColor(Color::WHITE);

    wxSize size = TheCanvas->GetSize();

    float scaleX = size.x / static_cast<float>(NUM_POINTS);
    float scaleY = size.y / static_cast<float>(MAX_VALUE);

    for (int i = 0; i < NUM_POINTS; i++)
    {
        int x = Round<int>(scaleX * i);
        int y = Round<int>(scaleY * data[i]);

        TheCanvas->SetPoint(x, y);
    }
}
