// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Form.h"


namespace History
{
    // Добавляет форму в историю
    void Add(const Form *form);

    // Извлекает предыдущую форму из история
    Form *Prev();

    // Извлекает следующую форму из истории
    Form *Next();
};
