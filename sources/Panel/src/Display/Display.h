// 2022/8/1 11:34:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Display/DisplayTypes.h"
#include "Display/Text.h"
#include "Keyboard/Controls.h"


#define BUFFER_WIDTH    Display::WIDTH    // ������ ������ ��� ���������
#define BUFFER_HEIGHT   Display::HEIGHT   // ������ ������ ��� ���������


namespace Display
{
    static const int WIDTH = 320;
    static const int HEIGHT = 240;

    void Init();

    void Update();

    void DrawScreenSplash();

    uint8 *GetBuffer();

    uint8 *GetRow(int row);

    // �����
    extern uint8 frontBuffer[BUFFER_WIDTH * BUFFER_HEIGHT];

    // ������ �����. � �� ���������� ���������, � ����� ����������� ���������� �� frontBuffer
    extern uint8 backBuffer[BUFFER_WIDTH * BUFFER_HEIGHT];

    struct Warnings
    {
        // ������� ��������
        static void ShowTemp(const String &ru, const String &en);
        static void ShowTemp(pchar ru, pchar en);
        static void ClearTemp();

        // ������� �� ���������
        static void ShowAllTheTime(const String &);
        static void ShowAllTheTime(const pchar &ru, const pchar &en);
        static void ClearFlash();
    };
};
