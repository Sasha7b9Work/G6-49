#include "defines.h"
#include "common/Command.h"
#include "Display/Painter.h"
#include "Generator/Generator_p.h"
#include "Generator/Signals.h"
#include "Generator/Wave.h"
#include <cmath>
#include <cstdlib>


using namespace Primitives;


#define SIZE_BUFFER     300
static uint8 formFlash[Chan::Count][SIZE_BUFFER];



pString Form::Name(uint lang) const
{
    static const pString names[TypeForm::Count][2] =
    {
        {"Синус", "Sinus"},
        {"Пила+", "Saw+"},
        {"Пила-", "Saw-"},
        {"Треугольник", "Triangle"},
        {"Меандр", "Meander"},
        {"Импульс", "Impulse"},
        {"Пакет", "Packet"},
        {"Произвольный", "Free"}
    };

    return names[value][lang];
}


pString Register::Name() const
{
    static const pString names[Register::Count] =
    {
        "Мультиплексор 1",
        "Мультиплексор 2",
        "5697 Смещение 1",
        "5697 Смещение 2",
        "5697 Част уровень",
        "5697 Част гистерезис",
        "RG0 Управление",
        "RG1 Частота",
        "RG2 Умножитель",
        "RG3 Прямоуг А",
        "RG4 Прямоуг B",
        "RG5 Период импульсов А",
        "RG6 Длит. импульсов А",
        "RG7 Период импульсов B",
        "RG8 Длит. импульсов B",
        "RG9 Парам. частотомера",
        "RG10 Смещение",
        "Частотомер - сопротивление",
        "Частотомер - связь",
        "Частотомер - фильтр"
    };

    return names[value];
}


Form *Wave::GetCurrentForm()
{
    return forms[indexCurrentForm];
}


void Wave::SetIndexForm(int8 num)
{
    indexCurrentForm = num;
}


int Wave::NumberOfForms() const
{
    return numForms;
}


Wave::Wave(Chan::E ch, Form **f) : channel(ch), indexCurrentForm(0), forms(f)
{
    numForms = 0;
    
    Form *form = forms[numForms];
    
    while(form)
    {
        numForms++;
        form = forms[numForms];
    }
}


Form::Form(TypeForm::E v, Parameter **parameters, Wave *w) : value(v), wave(w), params(parameters), currentParam(0), oldParams(nullptr), oldNumParams(0), oldCurrentParams(0)
{
    numParams = 0;

    if(params)
    {
        while(params[numParams])
        {
            numParams++;
        }

        for (int i = 0; i < numParams; i++)
        {
            params[i]->SetForm(this);
        }
    }

    if (v == TypeForm::Free)
    {
        std::memset(&formFlash[0][0], 127, SIZE_BUFFER * 2);
    }
}


Parameter *Form::CurrentParameter() const
{
    return params[currentParam];
}


int Form::NumParameters() const 
{
    return numParams;
}


Parameter *Form::GetParameter(int i)
{
    if(i < numParams)
    {
        return params[i];
    }
    return 0;
}


Form *Wave::GetForm(int i)
{
    if(i < numForms)
    {
        return forms[i];
    }
    
    return nullptr;
}


Form *Wave::GetForm(TypeForm::E form)
{
    for(int i = 0; i < numForms; i++)
    {
        if(forms[i]->value == form)
        {
            return forms[i];
        }
    }
   
    return nullptr;
}


void Form::SetNextParameter()
{
    currentParam++;
    if(currentParam >= NumParameters())
    {
        currentParam = 0;
    }
}


void Form::TuneGenerator()
{
    PGenerator::SetFormWave(GetWave());

    if(value == TypeForm::Sine)
    {
        if(CurrentParameter()->GetParent())                                 // Раскрыт параметр МАНИПУЛЯЦИЯ
        {
            SendParameterToGenerator(ParameterChoice::ManipulationEnabled); //-V525
            SendParameterToGenerator(ParameterValueType::ManipulationDuration);
            SendParameterToGenerator(ParameterValueType::ManipulationPeriod);

            int opened = currentParam;

            CloseOpenedParameter();

            SendParameterToGenerator(ParameterValueType::Frequency);
            SendParameterToGenerator(ParameterValueType::Amplitude);
            SendParameterToGenerator(ParameterValueType::Offset);

            OpenComplexParameter();

            currentParam = opened;
        }
        else                                                                // Параметр МАНИПУЛЯЦИЯ закрыт
        {
            OpenComplexParameter();

            SendParameterToGenerator(ParameterChoice::ManipulationEnabled); //-V525
            SendParameterToGenerator(ParameterValueType::ManipulationDuration);
            SendParameterToGenerator(ParameterValueType::ManipulationPeriod);

            CloseOpenedParameter();

            SendParameterToGenerator(ParameterValueType::Frequency);
            SendParameterToGenerator(ParameterValueType::Amplitude);
            SendParameterToGenerator(ParameterValueType::Offset);
        }

        if(GetWave()->GetChannel() != Chan::A)
        {
            SendParameterToGenerator(ParameterValueType::Phase);
        }
    }
    else
    {
        SendParameterToGenerator(ParameterValueType::Frequency);
        SendParameterToGenerator(ParameterValueType::Amplitude);
        SendParameterToGenerator(ParameterValueType::Offset);
        SendParameterToGenerator(ParameterValueType::Period);
        SendParameterToGenerator(ParameterValueType::Duration);
        SendParameterToGenerator(ParameterValueType::PacketNumber);
        SendParameterToGenerator(ParameterValueType::PacketPeriod);
        SendParameterToGenerator(ParameterChoice::Polarity);
    }
}


ParameterValue *Form::FindParameter(ParameterValueType::E p)
{
    for(int i = 0; i < numParams; i++)
    {
        Parameter *param = params[i];

        if(param->IsValue())
        {
            ParameterValue *parameter = static_cast<ParameterValue *>(param);

            if (parameter->Type() == p)
            {
                return parameter;
            }
        }
        else if(param->IsComplex())
        {
            ParameterComplex *parameter = static_cast<ParameterComplex *>(param);

            ParameterValue *val = parameter->FindParameter(p);

            if(val)
            {
                return val;
            }
        }
    }

    return nullptr;
}


ParameterChoice *Form::FindParameter(ParameterChoice::E p)
{
    for(int i = 0; i < numParams; i++)
    {
        Parameter *param = params[i];

        if(param->IsChoice())
        {
            ParameterChoice *parameter = static_cast<ParameterChoice *>(param);

            if (parameter->Type() == p)
            {
                return parameter;
            }
        }
        
        if(param->IsComplex())
        {
            ParameterComplex *complex = reinterpret_cast<ParameterComplex *>(param);

            ParameterChoice *choice = complex->FindParameter(p);

            if(choice)
            {
                return choice;
            }
        }
    }

    return nullptr;
}


void Form::SendParameterToGenerator(ParameterValueType::E p)
{
    Parameter *param = FindParameter(p);

    if (param)
    {
        PGenerator::SetParameter(param);
    }
}


void Form::SendParameterToGenerator(ParameterChoice::E p)
{
    Parameter *param = FindParameter(p);

    if(param)
    {
        PGenerator::SetParameter(param);
    }
}


void Form::OpenComplexParameter()
{
    if(!CurrentParameter()->IsComplex())
    {
        return;
    }

    oldParams = params;
    oldNumParams = numParams;
    oldCurrentParams = currentParam;

    ParameterComplex *parent = static_cast<ParameterComplex *>(CurrentParameter());

    numParams = parent->NumParams();
    params = parent->Params();
    currentParam = 0;

    for(int i = 0; i < numParams; i++)
    {
        params[i]->SetForm(this);
        params[i]->SetParent(parent);
    }
}


bool Form::CloseOpenedParameter()
{
    if (params[0]->IsOpened())
    {
        params = oldParams;
        numParams = oldNumParams;
        currentParam = oldCurrentParams;
        return true;
    }

    return false;
}


bool Wave::StartModeIsSingle()
{
    ParameterChoice* param = static_cast<ParameterChoice *>(GetCurrentForm()->FindParameter(ParameterChoice::ModeStart));

    if(param)
    {
        return param->GetChoice() == 1;
    }

    return false;
}


bool Form::IsDDS() const
{
    return  (value == TypeForm::RampPlus) || 
            (value == TypeForm::RampMinus) ||
            (value == TypeForm::Triangle) ||
            (value == TypeForm::Free);
}


void Form::DrawUGO(Chan::E ch, int y0)
{
    y0 += 30;
    int height = 50;
    int width = 90;
    int aveY = y0 + 5 + height / 2;
    int minY = y0 + 5;
    int maxY = minY + height;
    int x0 = 10;


    static bool first = true;

    static volatile int yNoise[100];
    static volatile int yExp[100];

    if (first)
    {
        std::srand(2);
        for (int i = 0; i < 100; i++)
        {
            yNoise[i] = aveY - static_cast<int>(std::rand() % 50 - 25);

            yExp[i] = aveY - static_cast<int>(std::expf(i / 12.5F) + 0.5F) + 1;
        }

        first = false;
    }

    VLine::Draw(x0, minY - 3, maxY + 3);
    HLine::Draw(aveY, x0, x0 + width);

    typedef void(*pFuncIIII)(Chan::E, int, int, int, int);

    static const pFuncIIII funcs[TypeForm::Count] =
    {
        DrawSine,
        DrawRampPlus,
        DrawRampMinus,
        DrawTriangle,
        DrawMeander,
        DrawImpulse,
        DrawPacketImpulse,
        DrawDDS
    };

    funcs[value](ch, x0, minY, width, height);
}


void Form::DrawSine(Chan::E, int x0, int y0, int width, int height)
{
    float speed = 0.2F;
    int delta = 1;
    y0 += height / 2;

    for (int i = delta; i < width; i++)
    {
        int y1 = y0 - static_cast<int>(std::sinf((i - delta) * speed) * height / 2.0F);
        int y2 = y0 - static_cast<int>(std::sinf(i * speed) * height / 2.0F);

        Line::Draw(x0 + i - delta, y1, x0 + i, y2);
    }
}


void Form::DrawRampPlus(Chan::E, int x0, int y0, int, int height)
{
    y0 += height;
    int dX = 28;
    for (int x = x0; x < x0 + 80; x += dX)
    {
        Line::Draw(x, y0, x + dX, y0 - height);
        Line::Draw(x + dX, y0, x + dX, y0 - height);
    }
}


void Form::DrawRampMinus(Chan::E, int x0, int y0, int, int height)
{
    int aveY = y0 + height / 2;
    int dX = 28;
    int dY = 20;
    for (int x = x0; x < x0 + 80; x += dX)
    {
        Line::Draw(x, aveY - dY, x + dX, aveY + dY);
        VLine::Draw(x + dX, aveY - dY, aveY + dY);
    }
}


void Form::DrawTriangle(Chan::E, int x, int y, int width, int height)
{
    int min = y + height;
    Line::Draw(x, min, x + width / 2, y);
    Line::Draw(x + width / 2, y, x + width, y + height);
}


void Form::DrawMeander(Chan::E, int x0, int y0, int, int height)
{
    int dX = 40;
    int dY = 20;
    int aveY = y0 + height / 2;
    for (int x = x0; x < x0 + 80; x += dX)
    {
        HLine::Draw(aveY - dY, x, x + dX / 2);
        VLine::Draw(x + dX / 2, aveY - dY, aveY + dY);
        HLine::Draw(aveY + dY, x + dX / 2, x + dX);
        VLine::Draw(x + dX, aveY - dY, aveY + dY);
    }
}


void Form::DrawImpulse(Chan::E ch, int x0, int y0, int, int height)
{
    int minY = y0;
    int maxY = y0 + height;

    ParameterChoice *param = WAVE(ch).GetCurrentForm()->FindParameter(ParameterChoice::Polarity);

    if (param->GetChoice() == 1)
    {
        minY = maxY;
        maxY = y0;
    }

    int deltaX = 20;

    for (int i = 0; i < 5; i++)
    {
        VLine::Draw(x0, minY, maxY);
        VLine::Draw(x0 + 5, minY, maxY);
        HLine::Draw(minY, x0, x0 + 5);
        HLine::Draw(maxY, x0 + 5, x0 + (i == 4 ? 7 : deltaX));
        x0 += deltaX;
    }
}


void Form::DrawPacketImpulse(Chan::E, int x0, int y0, int, int height)
{
    int minY = y0;
    int maxY = y0 + height;
    int deltaX = 8;
    for (int j = 0; j < 2; j++)
    {
        for (int i = 0; i < 3; i++)
        {
            VLine::Draw(x0, minY, maxY);
            HLine::Draw(minY, x0, x0 + 4);
            VLine::Draw(x0 + 4, minY, maxY);
            HLine::Draw(maxY, x0 + 4, x0 + deltaX);
            x0 += deltaX;
        }

        if (j == 0)
        {
            HLine::Draw(maxY, x0, x0 + 35);
        }

        x0 += 37;
    }
}


void Form::DrawDDS(Chan::E ch, int x0, int y0, int width, int height)
{
    int numPoints = 240;

    float sX = width / static_cast<float>(numPoints);
    float sY = height / 255.0F;

    for (int i = 0; i < numPoints; i++)
    {
        Point().Draw(static_cast<int>(x0 + sX * i), static_cast<int>(y0 + height - formFlash[ch][i] * sY));
    }
}


void Form::SetFormFlash(Chan::E ch, const uint8 data[SIZE_BUFFER])
{
    std::memcpy(&formFlash[ch][0], data, SIZE_BUFFER);
}


uint8 *Form::GetFormFlash(Chan::E ch)
{
    return &formFlash[ch][0];
}


float Form::GetOffset()
{
    ParameterValue *parameter = FindParameter(ParameterValueType::Offset);

    return (parameter) ? parameter->GetValue().ToFloat() : 0.0F;
}


float Form::GetAmplitude()
{
    ParameterValue *parameter = FindParameter(ParameterValueType::Amplitude);

    return (parameter) ? parameter->GetValue().ToFloat() : 0.0F;
}
