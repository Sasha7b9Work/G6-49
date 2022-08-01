// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace CPU
{
    void Init();

    // Установить на PA2 признак того, что процессор занят
    void SetBusy();

    // Установить на PA2 признак того, что процессор свободен
    void SetReady();
};
