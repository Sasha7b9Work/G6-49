#pragma once


namespace MyMath
{
    /// Округляет до ближайшего целого
    template<class T>
    T Round(float value);

    template<class T>
    void Swap(T *value0, T *value1)
    {
        T temp = *value0;
        *value0 = *value1;
        *value1 = temp;
    }

    template<class T>
    T Abs(T value)
    {
        return (value < static_cast<T>(0)) ? -value : value;
    }
}
