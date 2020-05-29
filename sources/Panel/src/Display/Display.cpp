#include "defines.h"
#include "Display/Console.h"
#include "Display/Painter.h"
#include "Display/WaveGraphics.h"
#include "FreqMeter/FreqMeter_p.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Keyboard/Keyboard.h"
#include "Menu/Hint.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Pages.h"
#include "Utils/Statistics.h"
#include <cstring>


using namespace Primitives;


struct Warning
{
    Warning(pString msg = "") : timeStart(TIME_MS), message(new String(msg)) { };
    void Delete()              { delete message; }
    bool IsEqual(String &msg);
    uint timeStart;
    String *message;
};

struct Warnings
{
    Warnings() : first(0), last(0) { }
    void Show();
    void Update();
    void Append(String &warning);
private:
    static const int NUM_WARNINGS = 10;
    Warning warnings[NUM_WARNINGS];
    int first;
    int last;
    bool IsEmpty()  { return (first == 0) && (last == 0); }
    Warning &Last() { return warnings[last - 1]; }
};


static Warnings warnings;

uint8 Display::frontBuffer[BUFFER_WIDTH * BUFFER_HEIGHT];

uint8 Display::backBuffer[BUFFER_WIDTH * BUFFER_HEIGHT];


uint8 *Display::GetBuffer()
{
    return backBuffer;
}


void Display::Update()
{
    Statistics::BeginFrame();
    Painter::BeginScene(Color::BACK);
    WaveGraphics::Draw(Chan::A);
    WaveGraphics::Draw(Chan::B);
    Menu::Draw();
    PageDebug::PageRegisters::Draw();
    Rectangle(318, 238).Draw(0, 0, Color::FILL);
    PFreqMeter::Draw();
    Hint::Draw();
    Statistics::Show();
    Console::Draw();
    Keyboard::Draw();
    warnings.Show();
    warnings.Update();
    Painter::EndScene();
    Statistics::EndFrame();
}


void Display::ShowWarning(String warning)
{
    warnings.Append(warning);
}


void Warnings::Append(String &warning)
{
    if (!IsEmpty() && Last().IsEqual(warning))
    {
        Last().timeStart = TIME_MS;
    }
    else if(last < NUM_WARNINGS)
    {
        warnings[last++] = Warning(warning.c_str());
    }
}


void Warnings::Show()
{
    int y = 0;

    for (int i = first; i < last; i++)
    {
        int width = warnings[i].message->Width();
        Rectangle(width + 2, 11).DrawFilled(0, y, Color::BACK, Color::FILL);
        warnings[i].message->Draw(2, y + 1, Color::FILL);
        y += 9;
    }
}


void Warnings::Update()
{
    if (first == 0 && last == 0)
    {
        return;
    }

    int newFirst = 0;

    for (int i = first; i < last; i++)
    {
        if (warnings[i].timeStart + 2000 < TIME_MS)
        {
            warnings[i].Delete();
            warnings[i].message = nullptr;
            newFirst = i + 1;
        }
    }

    first = newFirst;

    if (first == last)
    {
        first = 0;
        last = 0;
    }
}


bool Warning::IsEqual(String &msg)
{
    return std::strcmp(msg.c_str(), message->c_str()) == 0;
}
