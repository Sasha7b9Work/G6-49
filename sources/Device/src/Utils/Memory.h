// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Memory
{
    static void Init();
    // Выделяет участок памяти размером size байт и возращает указатель на него
    static void *Allocate(uint size);
    // Освобождает ранее выделенный участок памяти
    static void Free(void *address);
};
