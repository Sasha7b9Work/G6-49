#include "defines.h"
#include "Generator/Parameters.h"


void Parameter::SetForm(Form *f)
{
    form = f;
}


void ParameterComplex::SetForm(Form *f)
{
    Parameter::SetForm(f);

    for (int i = 0; i < numParams; i++)
    {
        params[i]->SetForm(f);
    }
}
