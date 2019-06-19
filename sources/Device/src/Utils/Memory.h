#pragma once


namespace Memory
{
    void Init();
    /// Выделяет участок памяти размером size байт и возращает указатель на него
    void *Allocate(uint size);
    /// Освобождает ранее выделенный участок памяти
    void Free(void *address);
};
