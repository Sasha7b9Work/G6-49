// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Keyboard/Controls.h"


struct Keyboard
{
    friend class CPU;

public:
    
    static void InitInputs(const uint16 *sl, const char *portSL, int numSL, const uint16 *rl, const char *portRL, int numRL);
    
    // ���������� true, ���� ����� ����
    static bool BufferIsEmpty();
    
    // ���������� ��������� ����� ����������, ���� ������� �������
    static Control GetNextControl();
    
    // �������-��������. ��� ���������, �� ������� ��� ���������� ���������� � ����� � �������� �� ������
    static void Draw();

    static void AppendEvent(Key::E key, Action::E action);

    // ��������� ������������� ����������� ��� ������������� ������������ �������������������
    struct Decoder
    {
        // ���������� true, ���� ���������� ������������������, �������� �������� keys. �������� ������� ������� ������� ������ ���� Key::Count.
        // ������������������ ����������, ���� ���������� �������� ������� ��������� � ���������� ������� � ������� �������, � ������������� ����� key.
        // ������ ������� �� ����� ��������� 20
        static bool Decode(const Control controls[20], const Control &control);
    };

private:

    static void Init();

    static void Update();
};
