// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Interface_p.h"
#include "common/Messages.h"
#include "Utils/Queue.h"


template Queue<Task>::Queue();
template bool Queue<Task>::Push(Task *);
template bool Queue<Task>::Full() const;



template <typename T>
Queue<T>::Queue() : first(0), last(0)
{

}


template <typename T>
void Queue<T>::Pop()
{
    if (Size() > 0)
    {
        delete storage[first];
        first++;
        if (first == last)
        {
            first = 0;
            last = 0;
        }
    }
}


template <typename T>
bool Queue<T>::Push(T *message)
{
    if (last < SIZE - 1)
    {
        storage[last] = message;
        last++;
        return true;
    }

    return false;
}


template <typename T>
int Queue<T>::Size()
{
    return last - first;
}


template <typename T>
bool Queue<T>::Full() const
{
    return (last == SIZE - 1);
}


template <typename T>
T *Queue<T>::Front()
{
    T *result = 0;

    if (Size() > 0)
    {
        result = storage[first];
    }

    return result;
}
