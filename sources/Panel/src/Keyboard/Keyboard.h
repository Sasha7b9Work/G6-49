// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Keyboard/Controls.h"


namespace Keyboard
{
    void Init();

    void Update();

    void InitInputs(const uint16 *sl, pchar portSL, int numSL, const uint16 *rl, pchar portRL, int numRL);
    
    // ���������� true, ���� ����� ����
    bool BufferIsEmpty();
    
    // ���������� ��������� ����� ����������, ���� ������� �������
    Control GetNextControl();
    
    // �������-��������. ��� ���������, �� ������� ��� ���������� ���������� � ����� � �������� �� ������
    void Draw();

    void AppendEvent(Key::E key, Action::E action);

    // ��������� ������������� ����������� ��� ������������� ������������ �������������������
    namespace Decoder
    {
        // ���������� true, ���� ���������� ������������������, �������� �������� keys. �������� ������� ������� ������� ������ ���� Key::Count.
        // ������������������ ����������, ���� ���������� �������� ������� ��������� � ���������� ������� � ������� �������, � ������������� ����� key.
        // ������ ������� �� ����� ��������� 20
        bool Decode(const Control controls[20], const Control &control);
    };
};
