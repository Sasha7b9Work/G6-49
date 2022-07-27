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
    
    void ShowWarning(String warning);
    void ShowWarning(const char *warning);

    void ClearWarnings();

    // �����
    extern uint8 frontBuffer[BUFFER_WIDTH * BUFFER_HEIGHT];

    // ������ �����. � �� ���������� ���������, � ����� ����������� ���������� �� frontBuffer
    extern uint8 backBuffer[BUFFER_WIDTH * BUFFER_HEIGHT];
};
