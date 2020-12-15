#include "defines.h"
#include "common/Messages.h"
#include "Queue.h"


Queue::Queue() : first(0), last(0)
{
    storage[0] = nullptr;
}


void Queue::Pop()
{
    if (Size() > 0)
    {
        delete storage[first]; //-V2511
        first++;
        if (first == last)
        {
            first = 0;
            last = 0;
        }
    }
}


bool Queue::Push(SimpleMessage *message) //-V2506
{
    if (last < SIZE - 1)
    {
        storage[last] = message;
        last++;
        return true;
    }

    return false;
}


int Queue::Size()
{
    return last - first;
}


SimpleMessage *Queue::Front()
{
    SimpleMessage *result = 0;

    if (Size() > 0)
    {
        result = storage[first];
    }

    return result;
}
