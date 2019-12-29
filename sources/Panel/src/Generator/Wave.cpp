#include "Command.h"
#include "defines.h"
#include "Log.h"
#include "Display/Painter.h"
#include "Settings/Settings.h"
#include "Display/InputWindow.h"
#include "Display/Symbols.h"
#include "Generator_p.h"
#include "Signals.h"
#include "Utils/Debug.h"
#include "Utils/Math.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Addition/PageInput.h"
#include "Utils/StringUtils.h"
#include <cstdio>
#include <cstdlib>
#include <cmath>


#define SIZE_BUFFER     300
static uint8 formFlash[Chan::Count][SIZE_BUFFER];



pString Form::Name() const
{
    static const pString names[Form::Count] =
    {
        "Синус",
        "Пила+",
        "Пила-",
        "Треугольник",
        "Меандр",
        "Импульс",
        "Пакет",
        "Произвольный"
    };

    return names[value];
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


pString ParameterValue::Name() const
{
    static const pString nameParameter[ParameterValue::Count] =
    {
        "Частота",
        "Период",
        "Размах",
        "Смещение",
        "Длительность",
        "Скважность",
        "Фаза",
        "Задержка",
        "Вр нарастания",
        "Вр спада",
        "Вр пика",
        "Коэфф заполн",
        "Длительность",
        "Период",
        "Период пакета",
        "Кол-во имп",
        "     Выход ( ESC )"
    };

    return nameParameter[value];
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


Form::Form(E v, ParameterBase **parameters, Wave *w) : value(v), wave(w), params(parameters), currentParam(0), oldParams(nullptr), oldNumParams(0), oldCurrentParams(0)
{
    numParams = 0;

    if(params)
    {
        while(params[numParams])
        {
            numParams++;
        }
    }

    for(int i = 0; i < numParams; i++)
    {
        params[i]->form = this;
    }

    if (v == Free)
    {
        std::memset(&formFlash[0][0], 127, SIZE_BUFFER * 2);
    }
}


ParameterBase *Form::CurrentParameter() const
{
    return params[currentParam];
}


int Form::NumParameters() const 
{
    return numParams;
}


ParameterBase *Form::GetParameter(int i)
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
    
    LOG_ERROR("Неправильный индекс %d", i);
    
    return nullptr;
}


Form *Wave::GetForm(Form::E form)
{
    for(int i = 0; i < numForms; i++)
    {
        if(forms[i]->value == form)
        {
            return forms[i];
        }
    }
    
    LOG_ERROR("Не найдена форма %d", static_cast<int>(form));
    
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


void Form::TuneGenerator(Chan::E ch)
{
    PGenerator::SetFormWave(GetWave());

    if(value == Form::Sine)
    {
        if(CurrentParameter()->GetParent())                                 // Раскрыт параметр МАНИПУЛЯЦИЯ
        {
            SendParameterToGenerator(ParameterChoice::ManipulationEnabled); //-V525
            SendParameterToGenerator(ParameterValue::ManipulationDuration);
            SendParameterToGenerator(ParameterValue::ManipulationPeriod);

            int opened = currentParam;

            CloseOpenedParameter();

            SendParameterToGenerator(ParameterValue::Frequency);
            SendParameterToGenerator(ParameterValue::Amplitude);
            SendParameterToGenerator(ParameterValue::Offset);

            OpenCurrentParameter();

            currentParam = opened;
        }
        else                                                                // Параметр МАНИПУЛЯЦИЯ закрыт
        {
            OpenCurrentParameter();

            SendParameterToGenerator(ParameterChoice::ManipulationEnabled); //-V525
            SendParameterToGenerator(ParameterValue::ManipulationDuration);
            SendParameterToGenerator(ParameterValue::ManipulationPeriod);

            CloseOpenedParameter();

            SendParameterToGenerator(ParameterValue::Frequency);
            SendParameterToGenerator(ParameterValue::Amplitude);
            SendParameterToGenerator(ParameterValue::Offset);
        }

        if(!Chan(ch).IsA())
        {
            SendParameterToGenerator(ParameterValue::Phase);
        }
    }
    else
    {
        SendParameterToGenerator(ParameterValue::Frequency);
        SendParameterToGenerator(ParameterValue::Amplitude);
        SendParameterToGenerator(ParameterValue::Offset);
        SendParameterToGenerator(ParameterValue::Period);
        SendParameterToGenerator(ParameterValue::Duration);
        SendParameterToGenerator(ParameterValue::PacketNumber);
        SendParameterToGenerator(ParameterValue::PacketPeriod);
        SendParameterToGenerator(ParameterChoice::Polarity);
    }
}


ParameterBase *Form::FindParameter(ParameterValue::E p)
{
    for(int i = 0; i < numParams; i++)
    {
        ParameterBase *param = params[i];

        if(param->IsValue() && (static_cast<ParameterValue *>(param))->value == p)
        {
            return param;
        }
    }
    return 0;
}


ParameterBase *Form::FindParameter(ParameterChoice::E p)
{
    for(int i = 0; i < numParams; i++)
    {
        ParameterBase *param = params[i];
        if(param->IsChoice() && (static_cast<ParameterChoice *>(param))->value == p)
        {
            return param;
        }
    }
    return 0;
}


void Form::SendParameterToGenerator(ParameterValue::E p)
{
    ParameterBase *param = FindParameter(p);
    if (param)
    {
        PGenerator::SetParameter(static_cast<ParameterValue *>(param));
    }
}


void Form::SendParameterToGenerator(ParameterChoice::E p)
{
    ParameterBase *param = FindParameter(p);
    if(param)
    {
        PGenerator::SetParameter(static_cast<ParameterChoice *>(param));
    }
}


void Form::OpenCurrentParameter()
{
    if(!CurrentParameter()->IsComplex())
    {
        return;
    }

    oldParams = params;
    oldNumParams = numParams;
    oldCurrentParams = currentParam;

    ParameterComplex *parent = static_cast<ParameterComplex *>(CurrentParameter());

    numParams = parent->numParams;
    params = parent->params;
    currentParam = 0;

    for(int i = 0; i < numParams; i++)
    {
        params[i]->form = this;
        params[i]->parent = parent;
    }
}


bool Form::CloseOpenedParameter()
{
    if (ParameterIsOpened())
    {
        params = oldParams;
        numParams = oldNumParams;
        currentParam = oldCurrentParams;
        return true;
    }

    return false;
}


bool Form::ParameterIsOpened() const
{
    return params[0]->parent != 0;
}


FloatValue ParameterValue::GetValueNano() const
{
    StructValue input(const_cast<ParameterValue *>(this));
    return input.ValueNano();
}


pString ParameterValue::GetStringDigits() const
{
    StructValue input(const_cast<ParameterValue *>(this));
    return input.StringDigits();
}


pString ParameterValue::GetStringValue() const
{
    static char buf[100];

    std::snprintf(buf, 99, "%E %s", GetValueNano().ToFloat(), Units());

    return buf;
}


pString ParameterValue::Units() const
{
    if(value == Frequency)
    {
        return "Hz";
    }

    return "";
}


float ParameterValue::Value() const
{
    return GetValueNano().ToFloat();
}


void ParameterValue::SetValue(float val)
{
    if(val >= 1e6F)
    {
        order = Order::Mega;
        val /= 1e6F;
    }
    else if(val >= 1e3F)
    {
        order = Order::Kilo;
        val /= 1e3F;
    }
    else if(val >= 1.0F)
    {
        order = Order::One;
    }
    else if(val >= 1e-3F)
    {
        order = Order::Milli;
        val *= 1e3F;
    }
    else if(val >= 1e-6F)
    {
        order = Order::Micro;
        val *= 1e6F;
    }
    else
    {
        order = Order::Nano;
        val *= 1e9F;
    }

    FillBuffer(val);
}


void ParameterValue::FillBuffer(float val)
{
    FloatValue floatValue(val);

    int integer = floatValue.Integer();

    int fract = floatValue.Fract(numDigits - Math::DigitsInInt(integer));

    if(sign != ' ')
    {
        sign = (integer < 0) ? '-' : '+';
    }

    if(integer < 0)
    {
        integer = -integer;
    }

    String strInteger = SU::Int2String(integer, false, 1);
    String strFract = SU::Int2String(fract, false, 1);

    int8 pos = 0;

    for(uint i = 0; i < strInteger.Size(); i++)
    {
        buffer[pos++] = strInteger.c_str()[i];
    }

    posComma = pos - 1;

    for(uint i = 0; i < strFract.Size(); i++)
    {
        buffer[pos++] = strFract.c_str()[i];
    }

    while(pos < numDigits)
    {
        buffer[pos++] = '0';
    }
}


bool ParameterValue::AssumeArbitaryOrder() const
{
    if (value == Offset || value == Amplitude)
    {
        return false;
    }

    return true;
}


void ParameterValue::IncreaseOrder()
{
    if (AssumeArbitaryOrder())
    {
        if (order < Order::Max(this))
        {
            order++; //-V803
        }
    }
}


void ParameterValue::DecreaseOrder()
{
    if (AssumeArbitaryOrder())
    {
        if (order > Order::Min(this))
        {
            order--; //-V803
        }
    }
}


pString ParameterComplex::GetStringDigits() const
{
    if(Is(Manipulation))
    {
        static const char *values[2] =
        {
            " Откл", " Вкл"
        };

        ParameterValue *pointer = reinterpret_cast<ParameterValue *>(const_cast<ParameterComplex *>(this));
        return values[SINE_MANIPULATION_ENABLED(pointer->GetForm()->GetWave()->GetChannel()) ? 1 : 0];
    }

    return "";
}


pString ParameterValue::NameUnit(char buf[10]) const
{
    static const pString names[ParameterValue::Count] =
    {
        "Гц",
        "с",
        "В",
        "В",
        "с",
        "",
        "\x7b",
        "с",
        "",
        "",
        "",
        "",
        "c",
        "c",
        "c",
        "",
        ""
    };

    std::sprintf(buf, "%s%s", order.Name(), names[value]);
    return buf;
}


pString ParameterBase::NameUnit(char buffer[10]) const
{
    if(IsValue())
    {
        return (static_cast<const ParameterValue *>(this))->NameUnit(buffer);
    }
    return "";
}


bool ParameterValue::IsOpened()
{
    return IsComplex() && GetForm()->ParameterIsOpened();
}


pString ParameterChoice::Name() const
{
    static const pString namesParam[ParameterChoice::Count] =
    {
        "Полярность",
        "Запуск",
        "Вид",
        "Манипуляция"
    };

    return namesParam[value];
}


pString ParameterComplex::Name() const
{
    static const pString names[ParameterComplex::Count] =
    {
        "МАНИПУЛЯЦИЯ"
    };

    return names[value];
}


pString ParameterPage::Name() const
{
    static const pString names[ParameterPage::Count] =
    {
        "ВЫБОР"
    };

    return names[value];
}


void ParameterPage::OpenPage()
{
    Menu::SetAdditionPage(reinterpret_cast< ::Page *>(page));
}


pString ParameterBase::Name() const
{
    if(IsValue())
    {
        return (static_cast<const ParameterValue *>(this))->Name();
    }
    else if(IsChoice())
    {
        return (static_cast<const ParameterChoice *>(this))->Name();
    }
    else if(IsComplex())
    {
        return (static_cast<const ParameterComplex *>(this))->Name();
    }
    else if(IsPage())
    {
        return (static_cast<const ParameterPage *>(this))->Name();
    }
    else
    {
        // здесь ничего
    }

    return "";
}


pString ParameterBase::GetStringDigits() const
{
    if(IsValue())
    {
        return (static_cast<const ParameterValue *>(this))->GetStringDigits();
    }
    else if(IsChoice())
    {
        return (static_cast<const ParameterChoice *>(this))->GetStringDigits();
    }
    else if(IsComplex())
    {
        return (static_cast<const ParameterComplex *>(this))->GetStringDigits();
    }
    else
    {
        // здесь ничего
    }
    return "";
}


pString ParameterChoice::GetStringDigits() const
{
    return names[choice];
}


void Form::ChangeParameter()
{
    ParameterBase *param = CurrentParameter();

    if(param->IsChoice())
    {
        static_cast<ParameterChoice *>(param)->NextChoice();
    }
    else if(param->IsValue() && static_cast<ParameterValue *>(param)->IsInputValue())
    {
        InputWindow::Init();
        Menu::SetAdditionPage(static_cast<Page *>(PageInput::self));
    }
    else if (param->IsExit())
    {
        CloseOpenedParameter();
    }
    else if (param->IsComplex())
    {
        OpenCurrentParameter();
    }
    else if(param->IsPage())
    {
        static_cast<ParameterPage *>(param)->OpenPage();
    }
    else
    {
        // здесь ничего
    }
}


void ParameterChoice::NextChoice()
{
    Math::CircleIncrease(&choice, 0, num - 1);

    Chan::E ch = form->GetWave()->GetChannel();

    if(value == ModeStart)
    {
        PGenerator::LoadStartMode(ch, choice);
    }
    else
    {
        if(value == ManipulationEnabled)
        {
            SINE_MANIPULATION_ENABLED(ch) = (choice != 0);
        }
        PGenerator::TuneChannel(ch);
    }
}


bool ParameterChoice::DrawChoice(int x, int y)
{
    if(func[choice])
    {
        func[choice](x, y);
    }

    return func[choice] != 0;
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


bool ParameterBase::AssumeArbitaryOrder() const
{
    if (IsValue())
    {
        return static_cast<const ParameterValue *>(this)->AssumeArbitaryOrder();
    }

    return true;
}


bool ParameterBase::IsExit() const
{
    return IsValue() && static_cast<const ParameterValue *>(this)->IsExit();
}


Order& Order::operator++(int)
{
    value = static_cast<E>(value + 1);
    return *this;
}


Order& Order::operator--(int)
{
    value = static_cast<E>(value - 1);
    return *this;
}


ParameterChoice::ParameterChoice(E v, pString var0, pString var1, funcDraw func0, funcDraw func1) :
    ParameterBase(Choice), value(v), choice(0), num(2)
{
    names[0] = const_cast<char *>(var0);
    names[1] = const_cast<char *>(var1);

    func[0] = func0;
    func[1] = func1;
};


ParameterChoice::ParameterChoice(E v, pString var0, pString var1, pString var2, pString var3) : 
    ParameterBase(Choice), value(v), choice(0), num(4)
{
    names[0] = const_cast<char *>(var0);
    names[1] = const_cast<char *>(var1);
    names[2] = const_cast<char *>(var2);
    names[3] = const_cast<char *>(var3);

    func[0] = nullptr;
    func[1] = nullptr;
    func[2] = nullptr;
    func[3] = nullptr;
}


ParameterComplex::ParameterComplex(E v, ParameterBase **param) : ParameterBase(Complex), value(v), params(param) //-V2504
{
    numParams = 0;
    while (params[numParams])
    {
        numParams++;
    }
};


ParameterValue::ParameterValue(int v) : ParameterBase(ParameterBase::Value), //-V730
    value(static_cast<E>(v)), hightLightDigit(0), posComma(0), sign('+'), numDigits(NUM_DIGITS), inNumLockMode(false)
{
    std::memset(buffer, 0, NUM_DIGITS + 1);

    if (v == Amplitude || v == Offset)
    {
        numDigits = 3;
    }
};


ParameterValue::ParameterValue(int v, float _min, float _max, pString buf, int8 pos, Order o, int8 hd, char s) : ParameterBase(ParameterBase::Value),
    value(static_cast<E>(v)), order(o), hightLightDigit(hd), posComma(pos), sign(s), numDigits(NUM_DIGITS), min(_min), max(_max), inNumLockMode(false)
{
    std::strcpy(buffer, buf);

    if (v == Amplitude || v == Offset)
    {
        numDigits = 3;
    }
};


bool Form::IsDDS() const
{
    return  (value == RampPlus) || 
            (value == RampMinus) ||
            (value == Triangle) ||
            (value == Free);
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

    Painter::DrawVLine(x0, minY - 3, maxY + 3);
    Painter::DrawHLine(aveY, x0, x0 + width);

    typedef void(*pFuncIIII)(Chan::E, int, int, int, int);

    static const pFuncIIII funcs[Form::Count] =
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

        Painter::DrawLine(x0 + i - delta, y1, x0 + i, y2);
    }
}


void Form::DrawRampPlus(Chan::E, int x0, int y0, int, int height)
{
    y0 += height;
    int dX = 28;
    for (int x = x0; x < x0 + 80; x += dX)
    {
        Painter::DrawLine(x, y0, x + dX, y0 - height);
        Painter::DrawLine(x + dX, y0, x + dX, y0 - height);
    }
}


void Form::DrawRampMinus(Chan::E, int x0, int y0, int, int height)
{
    int aveY = y0 + height / 2;
    int dX = 28;
    int dY = 20;
    for (int x = x0; x < x0 + 80; x += dX)
    {
        Painter::DrawLine(x, aveY - dY, x + dX, aveY + dY);
        Painter::DrawVLine(x + dX, aveY - dY, aveY + dY);
    }
}


void Form::DrawTriangle(Chan::E, int x, int y, int width, int height)
{
    int min = y + height;
    Painter::DrawLine(x, min, x + width / 2, y);
    Painter::DrawLine(x + width / 2, y, x + width, y + height);
}


void Form::DrawMeander(Chan::E, int x0, int y0, int, int height)
{
    int dX = 40;
    int dY = 20;
    int aveY = y0 + height / 2;
    for (int x = x0; x < x0 + 80; x += dX)
    {
        Painter::DrawHLine(aveY - dY, x, x + dX / 2);
        Painter::DrawVLine(x + dX / 2, aveY - dY, aveY + dY);
        Painter::DrawHLine(aveY + dY, x + dX / 2, x + dX);
        Painter::DrawVLine(x + dX, aveY - dY, aveY + dY);
    }
}


void Form::DrawImpulse(Chan::E ch, int x0, int y0, int, int height)
{
    int minY = y0;
    int maxY = y0 + height;

    ParameterChoice *param = (ParameterChoice *)WAVE(ch).GetCurrentForm()->FindParameter(ParameterChoice::Polarity);

    if (param->GetChoice() == 1)
    {
        minY = maxY;
        maxY = y0;
    }

    int deltaX = 20;

    for (int i = 0; i < 5; i++)
    {
        Painter::DrawVLine(x0, minY, maxY);
        Painter::DrawVLine(x0 + 5, minY, maxY);
        Painter::DrawHLine(minY, x0, x0 + 5);
        Painter::DrawHLine(maxY, x0 + 5, x0 + (i == 4 ? 7 : deltaX));
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
            Painter::DrawVLine(x0, minY, maxY);
            Painter::DrawHLine(minY, x0, x0 + 4);
            Painter::DrawVLine(x0 + 4, minY, maxY);
            Painter::DrawHLine(maxY, x0 + 4, x0 + deltaX);
            x0 += deltaX;
        }

        if (j == 0)
        {
            Painter::DrawHLine(maxY, x0, x0 + 35);
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
        Painter::SetPoint(static_cast<int>(x0 + sX * i), static_cast<int>(y0 + height - formFlash[ch][i] * sY));
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


ParameterValue *Form::GetParameterValue(ParameterValue::E _value)
{
    for (int i = 0; i < NumParameters(); i++)
    {
        ParameterBase *param = GetParameter(i);

        if (param && param->IsValue())
        {
            ParameterValue *paramValue = static_cast<ParameterValue *>(param);

            if (paramValue->Is(_value))
            {
                return paramValue;
            }
        }
    }

    return nullptr;
}


float Form::GetOffset()
{
    ParameterValue *parameter = GetParameterValue(ParameterValue::Offset);

    return (parameter) ? parameter->GetValueNano().ToFloat() - 5.0F : 0.0F;
}


float Form::GetAmplitude()
{
    ParameterValue *parameter = GetParameterValue(ParameterValue::Amplitude);

    return (parameter) ? parameter->GetValueNano().ToFloat() : 0.0F;
}


Order::E Order::Min(const ParameterValue *param)
{
    if (param->value == ParameterValue::Frequency)
    {
        return Order::Micro;
    }

    return Order::Nano;
}


Order::E Order::Max(const ParameterValue *param)
{
    if (param->value == ParameterValue::Frequency)
    {
        return Order::Mega;
    }

    return Order::One;
}
