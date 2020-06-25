#include "defines.h"
#include "Display/Console.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Display/WaveGraphics.h"
#include "FreqMeter/FreqMeter_p.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Keyboard/Keyboard.h"
#include "Menu/Hint.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Pages.h"
#include "Utils/Statistics.h"
#include <cstdlib>
#include <cstring>


using namespace Primitives;


struct Warning
{
    Warning(pString msg = "") : timeStart(TIME_MS) { if (msg[0] != '\0') { message = new String(msg); } else { message = nullptr; } };
    void Delete()   { if (message) { delete message; message = nullptr; } }
    bool IsEqual(const String &msg);
    uint timeStart;
    String *message;
};

struct Warnings
{
    Warnings() : last(0) { }
    void Show();
    void Append(const String &warning);
    void Clear();
private:
    static const int NUM_WARNINGS = 10;
    Warning warnings[NUM_WARNINGS];
    int last;                               // Указыват на элемент за последним действительным. Если last == 0, то массив пуст
    void Update();
    bool IsEmpty()  { return (last == 0); }
    Warning &Last() { return warnings[last - 1]; }
    Color ColorText() const;
    Color ColorBackground() const;
    void DeleteFirst();
    int X0() const;
    int Y0() const;
    int Width() const;
};


static Warnings warnings;

uint8 Display::frontBuffer[BUFFER_WIDTH * BUFFER_HEIGHT];

uint8 Display::backBuffer[BUFFER_WIDTH * BUFFER_HEIGHT];


uint8 *Display::GetBuffer()
{
    return backBuffer;
}


void Display::DrawScreenSplash()
{
    Painter::BeginScene(Color::BACK);

    Font::Set(TypeFont::_GOST28);

    Font::Spacing::SetAndStore(5);

    Font::ForceUpperCase(false);

    String("ОАО МНИПИ").DrawInCenterRect(0, 0, 280, 100, Color::FILL);

    String("Г6-49").DrawInCenterRect(0, 50, 280, 100);

    Font::Spacing::Restore();

    Painter::EndScene();
}


void Display::Update()
{
    Font::Set(TypeFont::_8);
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


void Warnings::Append(const String &warning)
{
    if (!IsEmpty() && Last().IsEqual(warning))
    {
        Last().timeStart = TIME_MS;
    }
    else
    {
        if (last == NUM_WARNINGS)
        {
            DeleteFirst();
        }

        warnings[last++] = Warning(warning.c_str());
    }
}


void Warnings::Show()
{
    Update();

    int y = Y0();

    Font::ForceUpperCase(false);

    for (int i = 0; i < last; i++)
    {
        Rectangle(Width(), 11).DrawFilled(X0(), y, ColorBackground(), Color::FILL);
        warnings[i].message->Draw(X0() + 2, y + 1, ColorText());
        y += 11;
    }
}


void Warnings::Update()
{
    while ((last != 0) && (warnings[0].timeStart + 5000 < TIME_MS))
    {
        warnings[0].Delete();

        for (int i = 0; i < last; i++)
        {
            int index = i + 1;

            if (index < last)
            {
                warnings[i] = warnings[index];
            }
        }

        last--;
    }
}


void Warnings::Clear()
{
    if (last != 0)
    {
        for (int i = 0; i < last; i++)
        {
            warnings[i].Delete();
        }

        last = 0;
    }
}


void Display::ClearWarnings()
{
    warnings.Clear();
}


void Warnings::DeleteFirst()
{
    warnings[0].Delete();

    last--;

    for (int i = 0; i < last; i++)
    {
        warnings[i] = warnings[i + 1];
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


int Warnings::X0() const
{
    return (Display::WIDTH - Width()) / 2;
}


int Warnings::Y0() const
{
    return Display::HEIGHT / 2 - last * 11 / 2;
}


int Warnings::Width() const
{
    int width = 0;

    for (int i = 0; i < last; i++)
    {
        if (warnings[i].message->Width() > width)
        {
            width = warnings[i].message->Width();
        }
    }

    return width + 2;
}


bool Warning::IsEqual(const String &msg)
{
    return std::strcmp(msg.c_str(), message->c_str()) == 0;
}
