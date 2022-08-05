// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "Display/Painter.h"
#include "Display/Symbols.h"
#include "Display/Text.h"
#include "Signals/Signals.h"
#include "Menu/Menu.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Pages.h"
#include "Settings/Settings.h"
#include <cmath>


// Здесь будем сохранять настраиваемый параметр перед его изменением, чтобы восстановить в случае необходимости
static ParameterDouble storedDouble = ParameterAmplitude(); //-V1054 Здесь ошибки не случится, потому что в наследуемых от ParameterDouble
                                                            // классах нет данных

static ParameterInteger storedInteger = ParameterInteger(ParameterIntegerType::PacketNumber, "", "", //-V810
                Value("0", Order::One), Value("100", Order::One), Value("0", Order::One));

static Parameter *tuned = nullptr;     // Настраиваемый параметр


void PageTuneParameter::SetParameter(Parameter *parameter)
{
    if(parameter->IsDouble())
    {
        storedDouble = *reinterpret_cast<ParameterDouble *>(parameter);
        tuned = parameter;
    }
    else if (parameter->IsInteger())
    {
        storedInteger = *reinterpret_cast<ParameterInteger *>(parameter);
        tuned = parameter;
    }
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Draw_More(int x, int y)
{
    String(LANG_RU ? "Больше" : "Increase").DrawInCenterRect(x, y, 70, 30);
    Char(Ideograph::_8::Up).Draw2Horizontal(x + 25, y + 30);
}

DEF_GRAPH_BUTTON(sbMore,
    "Больше", "More",
    "Увеличить порядок", "Increase order",
    *PageTuneParameter::self, Item::FuncActive, DisplayEntering::OnButtonOrderMore, Draw_More
)


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Draw_Less(int x, int y)
{
    String(LANG_RU ? "Меньше" : "Decrease").DrawInCenterRect(x, y, 70, 30);
    Char(Ideograph::_8::Down).Draw2Horizontal(x + 25, y + 30);
}


DEF_GRAPH_BUTTON(sbLess,
    "Меньше", "Less",
    "Уменьшить порядок", "Decrease order",
    *PageTuneParameter::self, Item::FuncActive, DisplayEntering::OnButtonOrderLess, Draw_Less
)


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Cancel()
{
    Tuner::Current()->OnButtonCancel();
}

static void Draw_Cancel(int x, int y)
{
    String(LANG_RU ? "Отменить" : "Cancel").DrawInCenterRect(x, y, 70, 30);
    Char(Ideograph::_8::Delete).Draw4InRect(x + 25, y + 28);
}

DEF_GRAPH_BUTTON(sbCancel,
    "Отменить", "Cancel",
    "Отказаться от ввода нового значения", "Refuse to enter a new value",
    *PageTuneParameter::self, Item::FuncActive, OnPress_Cancel, Draw_Cancel
)


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Draw_Apply(int x, int y)
{
    String(LANG_RU ? "Применить" : "Apply").DrawInCenterRect(x, y, 70, 30);
    Char(Ideograph::_8::Save).Draw4InRect(x + 25, y + 28);
}

void PageTuneParameter::OnPress_Apply()
{
    Tuner::Current()->OnButtonApply();

    String invalid_parameters = CURRENT_FORM->InvalidParameters();

    if (invalid_parameters.Size())
    {
        Display::Warnings::Show(invalid_parameters, false);
    }
    else
    {
        Display::Warnings::ClearAllTheTime();
    }
}

DEF_GRAPH_BUTTON(sbEnter,
    "Применить", "Apply",
    "Подтвердить ввод нового значения", "Confirm new value entry",
    *PageTuneParameter::self, Item::FuncActive, PageTuneParameter::OnPress_Apply, Draw_Apply
)


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool PageTuneParameter::VerifyForPossiblyChangesAmplitude(const Control &control)
{
    if (control.IsRotate() || control.IsEntering())
    {
        Parameter *parameter = Tuner::Current()->GetParameter();

        if (parameter->GetKind() == ParameterKind::Double)
        {
            ParameterDouble *paramDouble = reinterpret_cast<ParameterDouble *>(parameter);

            if (paramDouble->GetType() == ParameterDoubleType::Amplitude)
            {
                if (paramDouble->GetValue().Abs() == 0)
                {
                    ParameterDouble *offset = CURRENT_FORM->FindParameter(ParameterDoubleType::Offset);

                    if (offset)
                    {
                        if (std::fabs(offset->GetValue().ToDouble()) > 2.5)
                        {
                            Display::Warnings::Show("Смещение не более +/- 2.5В", "Offset no more than +/- 2.5V", true);

                            return false;
                        }
                    }
                }
            }
        }
    }

    return true;
}


static bool OnControl_TuneParameter(const Control &control)
{
    if (control.IsUp() && control.Is(Key::Esc))
    {
        OnPress_Cancel();
        return true;
    }

    if(control.IsDown())
    {
        if(control.Is(Key::RegButton))
        {
            PageTuneParameter::OnPress_Apply();
            return true;
        }
        else
        {
            if (!PageTuneParameter::VerifyForPossiblyChangesAmplitude(control))
            {
                return true;
            }

            return Tuner::Current()->OnControlKey(control);
        }
    }

    return false;
}

static void OnDraw_TuneParameter()
{
    tuned->GetTuner()->Draw();
}


static void OnOpenClose_TuneParameter(bool enter)
{
    if (enter)
    {
        tuned->GetTuner()->Init();
    }
    else
    {
        tuned = nullptr;
    }
}


DEF_PAGE_SB(pTuneParameter,
    "ВВОД ЗНАЧЕНИЯ", "ENTER VALUE",
    "", "",
    &Item::emptyDark,
    &Item::emptyDark,
    &sbCancel,
    &sbEnter,
    Page::SB_Input, 0, Item::FuncActive, OnOpenClose_TuneParameter, OnDraw_TuneParameter, OnControl_TuneParameter
)

Page *PageTuneParameter::self = reinterpret_cast<Page *>(const_cast<PageBase *>(&pTuneParameter));


void PageTuneParameter::SetModeEntering()
{
    if (Tuner::Current()->ParameterIsNotOrdered() || Tuner::Current()->GetParameter()->IsInteger())
    {

    }
    else
    {
        pTuneParameter.items[0] = reinterpret_cast<Item *>(const_cast<SButtonBase *>(&sbMore));
        pTuneParameter.items[1] = reinterpret_cast<Item *>(const_cast<SButtonBase *>(&sbLess));
    }
}


void PageTuneParameter::ResetModeEntering()
{
    pTuneParameter.items[0] = &Item::emptyDark;
    pTuneParameter.items[1] = &Item::emptyDark;
}


bool PageTuneParameter::IsOpened()
{
    return (CURRENT_PAGE == PageTuneParameter::self);
}


void PageTuneParameter::CallbackOnButtonCancel()
{
    Parameter *parameter = CURRENT_WAVE.GetCurrentForm()->CurrentParameter();

    if (parameter->IsDouble())
    {
        *reinterpret_cast<ParameterDouble *>(parameter) = storedDouble;
    }
    else if (parameter->IsInteger())
    {
        *reinterpret_cast<ParameterInteger *>(parameter) = storedInteger;
    }

    PGenerator::TuneChannel(CURRENT_CHANNEL);

    Menu::ResetAdditionPage();
}


void PageTuneParameter::CallbackOnButtonApply()
{
    Menu::ResetAdditionPage();
}
