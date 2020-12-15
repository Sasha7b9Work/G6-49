#include "defines.h"
#include "Display/Font/Font.h"
#include "Utils/Stack.h"
#include <cstdlib>


template class Stack<uint>;
template class Stack<int8>;
template class Stack<uint8>;
template class Stack<char>;
template class Stack<TypeFont::E>;


template <typename T>
Stack<T>::Stack(int _size) : buffer(0), size(_size), numElements(0U)
{
    buffer = static_cast<T *>(std::malloc(sizeof(T) * _size)); //-V2511
}


template <typename T>
Stack<T>::~Stack()
{
    std::free(buffer); //-V2511
}


template <typename T>
void Stack<T>::Push(T elem)
{
    if(numElements < size)
    {
        buffer[numElements] = elem; //-V2563
        numElements++;
    }
}


template <typename T>
T Stack<T>::Pop() //-V2506
{
    if(numElements != 0)
    {
        numElements--;
        return buffer[numElements]; //-V2563
    }

    return (T)0;
}


template <typename T>
int Stack<T>::Size() const
{
    return numElements;
}


template <typename T>
bool Stack<T>::IsEmpty() const
{
    return Size() == 0;
}


template <typename T>
T &Stack<T>::operator[](const int index) const //-V2506
{
    if (index < numElements)
    {
        return buffer[index]; //-V2563
    }

    static T nullValue = static_cast<T>(0);

    return nullValue;
}


template <typename T>
void Stack<T>::Clear()
{
    numElements = 0;
}
