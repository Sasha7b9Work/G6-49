// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class Queue
{
public:
    Queue();

    SimpleMessage *Front();

    bool Push(SimpleMessage *message);

    void Pop();

    int Size();

private:
    int first;

    int last;

    static const uint SIZE = 100U;

    SimpleMessage *storage[SIZE];
};
