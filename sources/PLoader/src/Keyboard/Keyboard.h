// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Keyboard/Controls.h"


struct Keyboard
{
public:

    static void Init();
    
    // ���������� true, ���� ����� ����
    static bool BufferIsEmpty();
    
    // ���������� ��������� ����� ����������, ���� ������� �������
    static Key GetNextControl();
    
    // �������-��������. ��� ���������, �� ������� ��� ���������� ���������� � ����� � �������� �� ������
    static void Draw();

    static void AppendEvent(Key::E key, Key::Action action);

    // ��������� ������������� ����������� ��� ������������� ������������ �������������������
    struct Decoder
    {
        // ���������� true, ���� ���������� ������������������, �������� �������� keys. �������� ������� ������� ������� ������ ���� Key::Count.
        // ������������������ ����������, ���� ���������� �������� ������� ��������� � ���������� ������� � ������� �������, � ������������� ����� key.
        // ������ ������� �� ����� ��������� 20
        static bool Decode(const Key keys[20], const Key &key);
    };

private:

    static void Update();

    static void InitInputs(const uint16 *sl, const char *portSL, int numSL, const uint16 *rl, const char *portRL, int numRL);
};
