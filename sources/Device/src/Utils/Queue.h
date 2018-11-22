#pragma once


class Queue
{
public:
    Message *Front();

    void Push(Message *message);

    void Pop();

    uint Size();
};
