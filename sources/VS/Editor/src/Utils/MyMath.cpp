// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "MyMath.h"
#include <cmath>


template int Math::Round(float);
template uint16 Math::Round(float);


template <class T>
T Math::Round(float value)
{
    return static_cast<T>(std::round(value));
}
