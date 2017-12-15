#pragma once
#include "defines.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define LIMITATION(var, value, min, max)    var = (value); if(var < (min)) { (var) = (min); } else if(var > (max)) { var = (max); };
#define LIMIT_BELOW(var, min)               if((var) < (min)) (var) = (min);
#define LIMIT_ABOVE(var, max)               if((var) > (max)) (var) = (max);

#define CIRCLE_DECREASE(x, min, max)    if((--(x)) < (min)) x = (max);
#define CIRCLE_INCREASE(x, min, max)    if((++(x)) > (max)) x = (min);

/// Возвращает максимальное значение из трёх
float MaxFloat(float val1, float val2, float val3);

int Math_Sign(int vlaue);
/// Прибавить к значению по адресу val delta. Если результат не вписывается в диапазон [min; max], ограничить
void AddLimitationFloat(float *val,             ///< по этому адресу хранится значение.
                        float delta,            ///< а это дельта.
                        float min,              ///< нижняя граница диапазона.
                        float max               ///< верхняя граница диапазона.
);
/// Возвращает позицию символа symbol в строке string. Если символ не обнаружен, возвращает -1
int FindSymbol(char *string, char symbol);
/// Возвращает количество десятичных символов в числе
int NumDigitsInUInt(uint value);
