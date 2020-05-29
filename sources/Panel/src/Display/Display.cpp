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
    void Append(String &warning);
private:
    static const int NUM_WARNINGS = 10;
    Warning warnings[NUM_WARNINGS];
    int first;
    int last;
    void Update();
    bool IsEmpty()  { return (first == 0) && (last == 0); }
    Warning &Last() { return warnings[last - 1]; }
    Color ColorText() const;
    Color ColorBackground() const;
    int X() const;
    int Y() const;
    int Width() const;
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
    Painter::EndScene();
    Statistics::EndFrame();
}


void Display::ShowWarning(String warning)
{
    warnings.Append(warning);
}


void Warnings::Append(String &warning)
{
    //if (!IsEmpty() && Last().IsEqual(warning))
    //{
    //    Last().timeStart = TIME_MS;
    //}
    //else
    
    if(last < NUM_WARNINGS)
    {
        warnings[last++] = Warning(warning.c_str());
    }
}


void Warnings::Show()
{
    Update();

    int y = 0;

    for (int i = first; i < last; i++)
    {
        int width = warnings[i].message->Width();
        Rectangle(width + 2, 11).DrawFilled(0, y, ColorBackground(), Color::FILL);
        warnings[i].message->Draw(2, y + 1, ColorText());
        y += 11;
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
        if (warnings[i].timeStart + 5000 < TIME_MS)
        {
            warnings[i].Delete();
            warnings[i].message = nullptr;
            newFirst = i + 1;
        }
    }

    if (newFirst > 0)
    {
        first = newFirst;
    }

    if (last - first < 2 && first != 0)
    {
        first = 0;
        last = 0;
    }
}


Color Warnings::ColorText() const
{
    return (TIME_MS % 1000) < 500 ? Color::FILL : Color::BACK;
}


Color Warnings::ColorBackground() const
{
    return (ColorText().value == Color::FILL.value) ? Color::BACK : Color::FILL;
}


bool Warning::IsEqual(String &msg)
{
    return std::strcmp(msg.c_str(), message->c_str()) == 0;
}
