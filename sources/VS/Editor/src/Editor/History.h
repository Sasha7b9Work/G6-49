// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Form.h"


namespace History
{
    // ��������� ����� � �������
    void Add(const Form *form);

    // ��������� ���������� ����� �� �������
    Form *Prev();

    // ��������� ��������� ����� �� �������
    Form *Next();
};
