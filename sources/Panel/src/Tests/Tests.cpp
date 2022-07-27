// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Painter.h"
#include "Hardware/HAL/HAL.h"
#include "Keyboard/Keyboard.h"
#include "Tests/Tests.h"
#include <cstring>


static void TestKeyboard();


void TestsHardware::Run()
{
    TestKeyboard();
}


static void TestKeyboard()
{
    if (!Keyboard::BufferIsEmpty())
    {
        bool keys[Key::Count];

        for (int i = 0; i < Key::Count; i++)
        {
            keys[i] = false;
        }

        while (!Keyboard::BufferIsEmpty())
        {
            Control control = Keyboard::GetNextControl();

            keys[control.key] = true;
        }

        static const int SIZE_BUFFER = Key::Count * 10;

        char buffer[SIZE_BUFFER];

        std::memset(buffer, 0, SIZE_BUFFER);

        int counter = 0;

        for (int i = 0; i < Key::Count; i++)
        {
            if (keys[i])
            {
                std::strcat(buffer, Key(static_cast<Key::E>(i)).Name());
                std::strcat(buffer, ", ");
                counter++;
            }
        }

        Painter::BeginScene(Color::BACK);

        Font::Set(TypeFont::_8);

        Font::ForceUpperCase(true);

        String("Обнаружена неисравность клавиатуры").Draw(10, 10, Color::FILL);
        String("Неисправные кнопки:").Draw(10, 30);
        String(buffer).DrawInColumnWithTransfers(10, 50, 300);

        Painter::EndScene();

        HAL_TIM::Delay(5000);
    }
}
