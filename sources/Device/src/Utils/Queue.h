#pragma once


class Queue
{
public:
    Queue();

    Message *Front();

    void Push(Message *message);

    void Pop();

    int Size();

private:
    int first;

    int last;

    static const uint SIZE = 100U;

    Message *storage[SIZE];
};
