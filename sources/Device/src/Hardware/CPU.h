// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct CPU
{
    static void Init();
    // Установить на PA2 признак того, что процессор занят
    static void SetBusy();
    // Установить на PA2 признак того, что процессор свободен
    static void SetReady();

private:

    static void InitGPIOS();
};
