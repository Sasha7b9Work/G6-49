// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


template <typename T>
class Queue
{
public:
    Queue();

    T *Front();

    bool Push(T *message);

    void Pop();

    int Size();
    // Возвращает true, если очередь полностью заполнена (Push() невозможен)
    bool IsFull() const;

private:
    int first;

    int last;

    static const uint SIZE = 100U;

    T *storage[SIZE];
};
