#pragma once
#include "common/Common.h"


class Parameter
{

};


class ParameterAmplitude : public Parameter
{
public:
    ParameterAmplitude(int minVolts, int maxVolts, int value);

private:
    FloatValue min;
    FloatValue max;
    FloatValue value;
};
