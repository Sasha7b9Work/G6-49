#include "defines.h"
#include "MyMath.h"
#include <cmath>


template int MyMath::Round(float);
template uint16 MyMath::Round(float);


template <class T>
T MyMath::Round(float value)
{
    return static_cast<T>(std::round(value));
}