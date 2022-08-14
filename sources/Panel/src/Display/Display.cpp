// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
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
#include "Settings/Settings.h"
#include <cstdlib>
#include <cstring>


using namespace Primitives;


namespace Display
{
    struct WarningMessage
    {
        WarningMessage(cstr msg = "") : timeStart(_TIME_MS) { if (msg[0] != '\0') { message = new String(msg); } else { message = nullptr; } };
        void Delete() { if (message) { delete message; message = nullptr; } }
        bool IsEqual(const String &msg);
        uint timeStart;
        String *message;
    };


    struct WarningsDisplay
    {
        WarningsDisplay() : last(0) { }
        void Draw();
        void AppendCenter(const String &);
        // Добавить две строки
        void AppendCenter2Strings(const String &, const String &);
        void ClearCenter();
        void AppendTop(const String &);
        void ClearTop();

    private:
        static const int NUM_WARNINGS = 10;
        WarningMessage warnings[NUM_WARNINGS];
        String warning_flash;
        int last;                               // Указыват на элемент за последним действительным. Если last == 0, то массив пуст
        void Update();
        bool IsEmpty() { return (last == 0); }
        WarningMessage &Last() { return warnings[last - 1]; }
        Color ColorText() const;
        Color ColorBackground() const;
        void DeleteFirst();
        int X0() const;
        int Y0() const;
        int Width() const;
    };

    static WarningsDisplay warnings;

    uint8 frontBuffer[BUFFER_WIDTH * BUFFER_HEIGHT];

    uint8 backBuffer[BUFFER_WIDTH * BUFFER_HEIGHT];
}


uint8 *Display::GetBuffer()
{
    return backBuffer;
}


uint8 *Display::GetRow(int row)
{
    return frontBuffer + row * 320;
}


void Display::DrawScreenSplash()
{
    Painter::BeginScene(Color::BACK);

    Font::Set(TypeFont::_GOST28);

    Font::Spacing::SetAndStore(5);

    Font::ForceUpperCase(false);

    String("ОАО МНИПИ").DrawInCenterRect(0, 0, 280, 100, Color::FILL);

    String("Г6-49").DrawInCenterRect(0, 50, 280, 100);

    Font::Set(TypeFont::_GOSTB20);

    Font::Spacing::Restore();
    Font::Spacing::SetAndStore(3);

    String("%s  %s", IDENTIFICATOR, VERSION).DrawInCenterRect(0, 150, 280, 100);

    Font::Spacing::Restore();

    Painter::EndScene();
}


void Display::Update()
{
    Font::Set(TypeFont::_8);
    Statistics::BeginFrame();
    Painter::BeginScene(Color::BACK);
    WaveGraphics::Draw(ChA);
    WaveGraphics::Draw(ChB);
    Menu::Draw();
    PageDebug::PageRegisters::Draw();
    Rectangle(318, 238).Draw(0, 0, Color::FILL);
    PFreqMeter::Draw();
    Hint::Draw();
    Statistics::Show();
    Console::Draw();
    Keyboard::Draw();
    warnings.Draw();
    Painter::EndScene();
    Statistics::EndFrame();
}


void Display::Warnings::Top::Show(const String &ru, const String &en)
{
    warnings.AppendTop(LANG_RU ? ru : en);
}


void Display::Warnings::Center::Show(const String &ru, const String &en)
{
    warnings.AppendCenter(LANG_RU ? ru : en);
}


void Display::Warnings::Center::Show(pchar ru, pchar en)
{
    warnings.AppendCenter(LANG_RU ? String(ru) : String(en));
}


void Display::Warnings::Center::Show2Strings(const String &ru1, const String &ru2, const String &en1, const String &en2)
{
    warnings.AppendCenter2Strings(LANG_RU ? ru1 : en1, LANG_RU ? ru2 : en2);
}


void Display::WarningsDisplay::AppendCenter(const String &warning)
{
    if (!IsEmpty() && Last().IsEqual(warning))
    {
        Last().timeStart = _TIME_MS;
    }
    else
    {
        if (last == NUM_WARNINGS)
        {
            DeleteFirst();
        }

        warnings[last++] = WarningMessage(warning.c_str());
    }
}


void Display::WarningsDisplay::AppendCenter2Strings(const String &str1, const String &str2)
{
    if (last >= 2 && (warnings[last - 1].IsEqual(str2) && warnings[last - 2].IsEqual(str1)))
    {
        uint time = _TIME_MS;
        warnings[last - 1].timeStart = time;
        warnings[last - 2].timeStart = time;
    }
    else
    {
        while (last >= NUM_WARNINGS - 1)
        {
            DeleteFirst();
        }

        warnings[last++] = WarningMessage(str1.c_str());
        warnings[last++] = WarningMessage(str2.c_str());
    }
}


void Display::WarningsDisplay::AppendTop(const String &warning)
{
    warning_flash = warning;
}


void Display::WarningsDisplay::ClearTop()
{
    warning_flash.Free();
}


void Display::WarningsDisplay::Draw()
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

    if (warning_flash.Size())
    {
        Rectangle(Display::WIDTH - 2, 18).DrawFilled(0, 0, Color::FLASH_01, Color::WHITE);
        warning_flash.Draw(3, 4, Color::FLASH_10);
    }
}


void Display::WarningsDisplay::Update()
{
    if ((last != 0) && (warnings[0].timeStart + 5000 < _TIME_MS))
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


void Display::WarningsDisplay::ClearCenter()
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


void Display::Warnings::Clear()
{
    warnings.ClearCenter();
    warnings.ClearTop();
}


void Display::Warnings::Top::Clear()
{
    warnings.ClearTop();
}


void Display::WarningsDisplay::DeleteFirst()
{
    warnings[0].Delete();

    last--;

    for (int i = 0; i < last; i++)
    {
        warnings[i] = warnings[i + 1];
    }
}


Color Display::WarningsDisplay::ColorText() const
{
    return (_TIME_MS % 1000) < 500 ? Color::FILL : Color::BACK;
}


Color Display::WarningsDisplay::ColorBackground() const
{
    return (ColorText().value == Color::FILL.value) ? Color::BACK : Color::FILL;
}


int Display::WarningsDisplay::X0() const
{
    return (Display::WIDTH - Width()) / 2;
}


int Display::WarningsDisplay::Y0() const
{
    return Display::HEIGHT / 2 - last * 11 / 2;
}


int Display::WarningsDisplay::Width() const
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


bool Display::WarningMessage::IsEqual(const String &msg)
{
    return std::strcmp(msg.c_str(), message->c_str()) == 0;
}
