// 2026/11/10 14:40:19 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once


template <class T>
class List
{
public:

    List() : index_write(0), index_read(0) { }

    // Добавляем новый элемент в конец списка
    void Append(T *t)
    {
        if (index_write < MAX_SIZE)
        {
            buffer[index_write++] = t;
        }
    }

    // Удаляет элемент из списка
    void Remove(T *t)
    {
        for (uint i = 0; i < Size(); i++)
        {
            if (buffer[i] == t)
            {
                if (Size() == 1)
                {
                    Clear();
                    return;
                }
                else
                {
                    if (i == Size() - 1)
                    {
                        index_write--;
                        return;
                    }
                    else
                    {
                        for (uint index = i + 1; index < index_write; index++)
                        {
                            buffer[index - 1] = buffer[index];
                        }

                        index_write--;

                        return;
                    }
                }
            }
        }
    }

    // Возвращает true, если список содержит element
    bool IsMember(T *t)
    {
        for (uint i = 0; i < Size(); i++)
        {
            if (buffer[i] == t)
            {
                return true;
            }
        }

        return false;
    }

    T *First()
    {
        index_read = 0;

        return Next();
    }

    T *Next()
    {
        if (index_read < index_write)
        {
            return buffer[index_read++];
        }

        return nullptr;
    }

    // Возвращает размер
    uint Size() const
    {
        return index_write;
    }

    void Clear()
    {
        index_write = 0;
        index_read = 0;
    }

private:

    static const uint MAX_SIZE = 10;
    T *buffer[MAX_SIZE];
    uint index_write;
    uint index_read;
};
