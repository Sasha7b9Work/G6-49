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
static DParam storedDouble = PAmplitudePic(); //-V1054 Здесь ошибки не случится, потому что в наследуемых от DParam
                                                            // классах нет данных

static IParam storedInteger = IParam(TypeIParam::PacketNumber, "", "", Value(1));

static Param *tuned = nullptr;     // Настраиваемый параметр


void PageTuneParameter::SetParameter(Param *parameter)
{
    if(parameter->IsDouble())
    {
        storedDouble = *(DParam *)parameter;
        tuned = parameter;
    }
    else if (parameter->IsInteger())
    {
        storedInteger = *(IParam *)parameter;
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
    *PageTuneParameter::self, Item::EFuncActive, DisplayEntering::OnButtonOrderMore, Draw_More
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
    *PageTuneParameter::self, Item::EFuncActive, DisplayEntering::OnButtonOrderLess, Draw_Less
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
    *PageTuneParameter::self, Item::EFuncActive, OnPress_Cancel, Draw_Cancel
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
}

DEF_GRAPH_BUTTON(sbEnter,
    "Применить", "Apply",
    "Подтвердить ввод нового значения", "Confirm new value entry",
    *PageTuneParameter::self, Item::EFuncActive, PageTuneParameter::OnPress_Apply, Draw_Apply
)


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


DEF_PAGE_SB(pTuneParameter, //-V1027
    "ВВОД ЗНАЧЕНИЯ", "ENTER VALUE",
    "", "",
    &Item::emptyDark,
    &Item::emptyDark,
    &sbCancel,
    &sbEnter,
    Page::SB_Input, 0, Item::EFuncActive, OnOpenClose_TuneParameter, OnDraw_TuneParameter, OnControl_TuneParameter
)

Page *PageTuneParameter::self = (Page *)&pTuneParameter;


void PageTuneParameter::SetModeEntering()
{
    if (Tuner::Current()->IsNotOrdered() || Tuner::Current()->GetParameter()->IsInteger())
    {

    }
    else
    {
        pTuneParameter.items[0] = (Item *)&sbMore; //-V1027
        pTuneParameter.items[1] = (Item *)&sbLess; //-V1027
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
    Param *parameter = CURRENT_WAVE.GetCurrentForm()->CurrentParameter();

    if (parameter->IsDouble())
    {
        *((DParam *)parameter) = storedDouble;
    }
    else if (parameter->IsInteger())
    {
        *((IParam *)parameter) = storedInteger;
    }

    PGenerator::TuneChannel(CURRENT_CHANNEL);

    Menu::ResetAdditionPage();
}


void PageTuneParameter::CallbackOnButtonApply()
{
    Menu::ResetAdditionPage();
}
