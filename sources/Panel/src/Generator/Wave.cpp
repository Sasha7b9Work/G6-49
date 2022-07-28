// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Command.h"
#include "Display/Painter.h"
#include "Generator/Generator_p.h"
#include "Generator/Signals.h"
#include "Generator/Wave.h"
#include <cmath>
#include <cstdlib>


using namespace Primitives;


static uint8 formFlash[Chan::Count][POINTS_IN_FORM];



pString Form::Name(uint lang) const
{
    if (lang == static_cast<uint>(-1))
    {
        lang = LANGUAGE;
    }

    static const pString names[TypeForm::Count][2] =
    {
        {"Синус",   "Sinus"},
        {"Пила+",   "Saw+"},
        {"Пила-",   "Saw-"},
        {"Треуг",   "Triangle"},
        {"Меандр",  "Meander"},
        {"Импульс", "Impulse"},
        {"Пакет",   "Packet"},
        {"Произв",  "Free"}
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
    return forms[numberForm];
}


int Wave::NumberOfForms() const
{
    return numForms;
}


Wave::Wave(Chan::E ch, Form **f) : channel(ch), numberForm(0), forms(f)
{
    numForms = 0;
    
    Form *form = forms[numForms];
    
    while(form)
    {
        numForms++;
        form = forms[numForms];
    }
}


void Wave::Reset()
{
    numberForm = 0;

    for (int i = 0; i < NumberOfForms(); i++)
    {
        GetForm(i)->Reset();
    }
}


Form::Form(TypeForm::E v, Parameter **parameters, Wave *w) : value(v), wave(w), params(parameters), currentParam(0)
{
    numParams = 0;
	
    old.Init(nullptr, 0, 0);

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
        std::memset(&formFlash[0][0], 127, POINTS_IN_FORM * 2);
    }
}


void Form::Reset()
{
    currentParam = 0;

    for (int i = 0; i < NumParameters(); i++)
    {
        GetParameter(i)->Reset();
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

    return nullptr;
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


void Form::SetPrevParameter()
{
    currentParam--;
    if (currentParam < 0)
    {
        currentParam = NumParameters() - 1;
    }
}


void Form::TuneGenerator()
{
    PGenerator::SetFormWave(GetWave());

    if(value == TypeForm::Sine)
    {
        Parameter *manipulation = FindParameter(ParameterChoiceType::ManipulationEnabled);
        bool manipulationEnabled = reinterpret_cast<ParameterChoice *>(manipulation)->GetChoice() == 1;

        if(CurrentParameter()->GetParent())                                 // Раскрыт параметр МАНИПУЛЯЦИЯ
        {
            SendParameterToGenerator(ParameterChoiceType::ManipulationEnabled); //-V525
            if(manipulationEnabled)
            {
                SendParameterToGenerator(ParameterDoubleType::ManipulationDuration);
                SendParameterToGenerator(ParameterDoubleType::ManipulationPeriod);
            }

            int opened = currentParam;

            CloseCompositeParameter();

            SendParameterToGenerator(ParameterDoubleType::Frequency);
            SendParameterToGenerator(ParameterDoubleType::Amplitude);
            SendParameterToGenerator(ParameterDoubleType::Offset);

            OpenCompositeParameter();

            currentParam = opened;
        }
        else                                                                // Параметр МАНИПУЛЯЦИЯ закрыт
        {
            OpenCompositeParameter();

            SendParameterToGenerator(ParameterChoiceType::ManipulationEnabled); //-V525
            if(manipulationEnabled)
            {
                SendParameterToGenerator(ParameterDoubleType::ManipulationDuration);
                SendParameterToGenerator(ParameterDoubleType::ManipulationPeriod);
            }

            CloseCompositeParameter();

            SendParameterToGenerator(ParameterDoubleType::Frequency);
            SendParameterToGenerator(ParameterDoubleType::Amplitude);
            SendParameterToGenerator(ParameterDoubleType::Offset);
        }

        if(GetWave()->GetChannel().IsB())
        {
            SendParameterToGenerator(ParameterDoubleType::Phase);
        }
    }
    else
    {
        SendParameterToGenerator(ParameterDoubleType::Frequency);
        SendParameterToGenerator(ParameterDoubleType::Amplitude);
        SendParameterToGenerator(ParameterDoubleType::Offset);
        SendParameterToGenerator(ParameterDoubleType::Period);
        SendParameterToGenerator(ParameterDoubleType::Duration);
        SendParameterToGenerator(ParameterIntegerType::PacketNumber);
        SendParameterToGenerator(ParameterDoubleType::PacketPeriod);
        SendParameterToGenerator(ParameterChoiceType::Polarity);
    }
}


ParameterDouble *Form::FindParameter(ParameterDoubleType::E p)
{
    for(int i = 0; i < numParams; i++)
    {
        Parameter *param = params[i];

        if(param->IsDouble())
        {
            ParameterDouble *parameter = static_cast<ParameterDouble *>(param);

            if (parameter->GetType() == p)
            {
                return parameter;
            }
        }
        else if(param->IsComposite())
        {
            ParameterComposite *parameter = static_cast<ParameterComposite *>(param);

            ParameterDouble *val = parameter->FindParameter(p);

            if(val)
            {
                return val;
            }
        }
    }

    return nullptr;
}


ParameterChoice *Form::FindParameter(ParameterChoiceType::E p)
{
    for(int i = 0; i < numParams; i++)
    {
        Parameter *param = params[i];

        if(param->IsChoice())
        {
            ParameterChoice *choice = static_cast<ParameterChoice *>(param);

            if (choice->GetType() == p)
            {
                return choice;
            }
        }
        
        if(param->IsComposite())
        {
            ParameterComposite *complex = reinterpret_cast<ParameterComposite *>(param);

            ParameterChoice *choice = complex->FindParameter(p);

            if(choice)
            {
                return choice;
            }
        }
    }

    return nullptr;
}


ParameterComposite *Form::FindParameter(ParameterCompositeType::E t)
{
    for (int i = 0; i < numParams; i++)
    {
        Parameter *param = params[i];

        if (param->IsComposite())
        {
            ParameterComposite *composite = static_cast<ParameterComposite *>(param);

            if (composite->GetType() == t)
            {
                return composite;
            }
        }
    }

    if (old.params)
    {
        for (int i = 0; i < numParams; i++)
        {
            Parameter *param = old.params[i];

            if (param->IsComposite())
            {
                ParameterComposite *composite = static_cast<ParameterComposite *>(param);

                if (composite->GetType() == t)
                {
                    return composite;
                }
            }
        }
    }

    return nullptr;
}


ParameterInteger *Form::FindParameter(ParameterIntegerType::E t)
{
    for (int i = 0; i < numParams; i++)
    {
        Parameter *param = params[i];

        if (param->IsInteger())
        {
            ParameterInteger *integer = static_cast<ParameterInteger *>(param);

            if (integer->GetType() == t)
            {
                return integer;
            }
        }
    }

    return nullptr;
}


void Form::SendParameterToGenerator(ParameterDoubleType::E p)
{
    Parameter *param = FindParameter(p);

    if (param)
    {
        PGenerator::SetParameter(param);
    }
}


void Form::SendParameterToGenerator(ParameterChoiceType::E p)
{
    Parameter *param = FindParameter(p);

    if(param)
    {
        PGenerator::SetParameter(param);
    }
}


void Form::SendParameterToGenerator(ParameterIntegerType::E p)
{
    Parameter *param = FindParameter(p);

    if (param)
    {
        PGenerator::SetParameter(param);
    }
}


void Form::OpenCompositeParameter()
{
    if(!CurrentParameter()->IsComposite())
    {
        return;
    }

    old.Init(params, numParams, currentParam);

    ParameterComposite *parent = static_cast<ParameterComposite *>(CurrentParameter());

    numParams = parent->NumParameters();
    params = parent->Parameters();
    currentParam = 0;

    for(int i = 0; i < numParams; i++)
    {
        params[i]->SetForm(this);
        params[i]->SetParent(parent);
    }
}


bool Form::CloseCompositeParameter()
{
    if (params[0]->IsOpened())
    {
        params = old.params;
        numParams = old.numParams;
        currentParam = old.currentParam;
        return true;
    }

    return false;
}


bool Wave::StartModeIsSingle()
{
    ParameterChoice* param = static_cast<ParameterChoice *>(GetCurrentForm()->FindParameter(ParameterChoiceType::ModeStart));

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


void Form::DrawUGO(const Chan &ch, int y0)
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
        std::srand(2); //-V1057
        for (int i = 0; i < 100; i++)
        {
            yNoise[i] = aveY - static_cast<int>(std::rand() % 50 - 25);

            yExp[i] = aveY - static_cast<int>(std::expf(static_cast<float>(i) / 12.5F) + 0.5F) + 1;
        }

        first = false;
    }

    VLine::Draw(x0, minY - 3, maxY + 3);
    HLine::Draw(aveY, x0, x0 + width);

    typedef void(*pFuncIIII)(const Chan &, int, int, int, int);

    static const pFuncIIII funcs[TypeForm::Count] =
    {
        DrawSine,
        DrawRampPlus,
        DrawRampMinus,
        DrawTriangle,
        DrawMeander,
        DrawImpulse,
        DrawPacketImpulse,
        DrawFree
    };

    funcs[value](ch, x0, minY, width, height);
}


void Form::DrawSine(const Chan &ch, int x0, int y0, int width, int height)
{
    ParameterComposite *param = FORM(ch)->FindParameter(ParameterCompositeType::Manipulation);

    if (param)
    {
        ParameterChoice *choice = param->FindParameter(ParameterChoiceType::ManipulationEnabled);

        if (choice->GetChoice() == 1)
        {
            float speed = 0.6F;

            int delta = 1;

            y0 += height / 2;

            for (int j = 0; j < 2; j++)
            {
                int dX = j * (width / 3) * 2;

                if (dX > 0)
                {
                    dX -= width / 3 / 2;
                }

                for (int i = delta; i < width / 3; i++)
                {
                    int y1 = y0 - static_cast<int>(std::sinf(static_cast<float>(i - delta) * speed) * static_cast<float>(height) / 2.0F);
                    int y2 = y0 - static_cast<int>(std::sinf(static_cast<float>(i) * speed) * static_cast<float>(height) / 2.0F);

                    Line::Draw(dX + x0 + i - delta, y1, dX + x0 + i, y2);
                }
            }

            return;
        }
    }

    float speed = 0.2F;
    int delta = 1;
    y0 += height / 2;

    for (int i = delta; i < width; i++)
    {
        int y1 = y0 - static_cast<int>(std::sinf(static_cast<float>(i - delta) * speed) * static_cast<float>(height) / 2.0F);
        int y2 = y0 - static_cast<int>(std::sinf(static_cast<float>(i) * speed) * static_cast<float>(height) / 2.0F);

        Line::Draw(x0 + i - delta, y1, x0 + i, y2);
    }
}


void Form::DrawRampPlus(const Chan &, int x0, int y0, int, int height)
{
    y0 += height;
    int dX = 28;
    for (int x = x0; x < x0 + 80; x += dX)
    {
        Line::Draw(x, y0, x + dX, y0 - height);
        Line::Draw(x + dX, y0, x + dX, y0 - height);
    }
}


void Form::DrawRampMinus(const Chan &, int x0, int y0, int, int height)
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


void Form::DrawTriangle(const Chan &, int x, int y, int width, int height)
{
    int min = y + height;
    Line::Draw(x, min, x + width / 2, y);
    Line::Draw(x + width / 2, y, x + width, y + height);
}


void Form::DrawMeander(const Chan &, int x0, int y0, int, int height)
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


void Form::DrawImpulse(const Chan &ch, int x0, int y0, int, int height)
{
    int minY = y0;
    int maxY = y0 + height;

    ParameterChoice *param = WAVE(ch).GetCurrentForm()->FindParameter(ParameterChoiceType::Polarity);

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


void Form::DrawPacketImpulse(const Chan &, int x0, int y0, int, int height)
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


void Form::DrawFree(const Chan &ch, int x0, int y0, int width, int height)
{
    int numPoints = 240;

    float sX = static_cast<float>(width) / static_cast<float>(numPoints);
    float sY = static_cast<float>(height) / 255.0F;

    for (int i = 1; i < numPoints; i++)
    {
        int x1 = static_cast<int>(static_cast<float>(x0) + sX * static_cast<float>(i - 1));
        int y1 = static_cast<int>(static_cast<float>(y0 + height) - static_cast<float>(formFlash[ch.value][i - 1]) * sY);

        int x2 = static_cast<int>(static_cast<float>(x0) + sX * static_cast<float>(i));
        int y2 = static_cast<int>(static_cast<float>(y0 + height) - static_cast<float>(formFlash[ch.value][i]) * sY);

        Line().Draw(x1, y1, x2, y2);
    }
}


void Form::SetFormFlash(const Chan &ch, const uint8 data[POINTS_IN_FORM])
{
    std::memcpy(&formFlash[ch.value][0], data, POINTS_IN_FORM);
}


uint8 *Form::GetFormFlash(const Chan &ch)
{
    return &formFlash[ch.value][0];
}


double Form::GetOffset()
{
    ParameterDouble *parameter = FindParameter(ParameterDoubleType::Offset);

    return (parameter) ? parameter->GetValue().ToDouble() : 0.0;
}


double Form::GetAmplitude()
{
    ParameterDouble *parameter = FindParameter(ParameterDoubleType::Amplitude);

    return (parameter) ? parameter->GetValue().ToDouble() : 0.0;
}


void Form::StoreState()
{
    for (int i = 0; i < NumParameters(); i++)
    {
        Parameter *parameter = GetParameter(i);

        if (parameter)
        {
            parameter->StoreState();
        }
    }
}


void Form::RestoreState()
{
    for (int i = 0; i < NumParameters(); i++)
    {
        Parameter *parameter = GetParameter(i);

        if (parameter)
        {
            parameter->RestoreState();
        }
    }
}


void Wave::SetForm(Form *form)
{
    for (int i = 0; i < NumberOfForms(); i++)
    {
        if (form == GetForm(i))
        {
            SetIndexForm(i);
            break;
        }
    }
}
