// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Keyboard/Controls.h"


namespace Keyboard
{
    void Init();

    // ���������� true, ���� ����� ����
    bool BufferIsEmpty();

    // ���������� ��������� ����� ����������, ���� ������� �������
    Key GetNextControl();

    // �������-��������. ��� ���������, �� ������� ��� ���������� ���������� � ����� � �������� �� ������
    void Draw();

    void AppendEvent(Key::E key, Key::Action action);

    // ��������� ������������� ����������� ��� ������������� ������������ �������������������
    namespace Decoder
    {
        // ���������� true, ���� ���������� ������������������, �������� �������� keys. �������� ������� ������� ������� ������ ���� Key::Count.
        // ������������������ ����������, ���� ���������� �������� ������� ��������� � ���������� ������� � ������� �������, � ������������� ����� key.
        // ������ ������� �� ����� ��������� 20
        bool Decode(const Key keys[20], const Key &key);
    };
};
