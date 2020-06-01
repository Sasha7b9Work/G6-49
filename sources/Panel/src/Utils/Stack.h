#pragma once


template<typename T>
class Stack
{
public:
    Stack(int size);
    
    ~Stack();
    
    void Push(T elem);
    
    T Pop();
    
    int Size() const;
    
    bool IsEmpty() const;
    
    T &operator[](const int index) const;

    void Clear();

private:
    T *buffer;
    int size;
    int numElements;
};
