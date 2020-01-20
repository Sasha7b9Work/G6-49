#include "defines.h"
#include "Generator/Parameters.h"


ParameterAmplitude::ParameterAmplitude(int minVolts, int maxVolts, int val) : Parameter()
{
    min = FloatValue(minVolts, 0);
    max = FloatValue(maxVolts, 0);
    value = FloatValue(val, 0);
}
