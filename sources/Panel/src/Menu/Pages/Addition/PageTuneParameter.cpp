#include "Display/Painter.h"
#include "Display/Symbols.h"
#include "Display/Text.h"
#include "Generator/Signals.h"
#include "Menu/Menu.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Addition/PageTuneParameter.h"
#include "Settings/Settings.h"


static ParameterValue stored = ParameterAmplitude();    // Здесь будем сохранять настраиваемый параметр перед его изменением, чтобы восстановить в случае необходимости //-V1054

static ParameterValue *tuned = nullptr;     // Настраиваемый параметр //-V1054


void PageTuneParameter::SetParameter(ParameterValue *parameter)
{
    if(parameter->IsValue())
    {
        stored = *parameter;
        tuned = parameter;
    }
}


static void OnPress_More()
{

}


static void Draw_More(int _x, int _y)
{
    int x[3] = { _x + 15, _x + 35, _x + 55};
    int y[3] = { _y + 35, _y + 15, _y + 35};

    Primitives::Line::Draw(x[0], y[0], x[1], y[1]);
    Primitives::Line::Draw(x[1], y[1], x[2], y[2]);
    Primitives::Line::Draw(x[0], y[0], x[2], y[2]);
}


DEF_SMALL_BUTTON(sbMore,
    "БОЛЬШЕ", "MORE",
    "Увеличить порядок", "Increase order",
    *PageTuneParameter::self, Item::FuncActive, OnPress_More, Draw_More
)


static void OnPress_Less()
{

}


static void Draw_Less(int _x, int _y)
{
    int x[3] = { _x + 15, _x + 35, _x + 55 };
    int y[3] = { _y + 15, _y + 35, _y + 15 };

    Primitives::Line::Draw(x[0], y[0], x[1], y[1]);
    Primitives::Line::Draw(x[1], y[1], x[2], y[2]);
    Primitives::Line::Draw(x[0], y[0], x[2], y[2]);
}


DEF_SMALL_BUTTON(sbLess,
    "МЕНЬШЕ", "LESS",
    "Уменьшить порядок", "Decrease order",
    *PageTuneParameter::self, Item::FuncActive, OnPress_Less, Draw_Less
)


static void OnPress_Cancel()
{
    Parameter *parameter = CURRENT_WAVE.GetCurrentForm()->CurrentParameter();

    if(parameter->IsValue())
    {
        *reinterpret_cast<ParameterValue *>(parameter) = stored;
    }

    PGenerator::TuneChannel(CURRENT_CHANNEL);

    Menu::ResetAdditionPage();
}

static void Draw_Cancel(int x, int y)
{
    String("Отменить").Draw(x + 13, y + 12);
    Text::Draw4SymbolsInRect(x + 25, y + 28, Ideograph::_8::Delete);
}

DEF_SMALL_BUTTON(sbCancel,                                                                                                                                      //--- ОКНО ВВОДА - ОТМЕНА ---
    "ОТМЕНА", "CANCEL",
    "Отказаться от ввода нового значения", "Refuse to enter a new value",
    *PageTuneParameter::self, Item::FuncActive, OnPress_Cancel, Draw_Cancel
)


static void Draw_Enter(int x, int y)
{
    String("Подтвердить").Draw(x + 3, y + 12);
    Text::Draw4SymbolsInRect(x + 25, y + 28, Ideograph::_8::Save);
}

static void OnPress_Enter()
{
    Menu::ResetAdditionPage();
}

DEF_SMALL_BUTTON(sbEnter,                                                                                                                                         //--- ОКНО ВВОДА - ВВОД ---
    "ВВОД", "ENTER",
    "Подтвердить ввод нового значения", "Confirm new value entry",
    *PageTuneParameter::self, Item::FuncActive, OnPress_Enter, Draw_Enter
)


static bool OnControl_TuneParameter(const Control control) //-V801
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
            Menu::ResetAdditionPage();
            return true;
        }
        else
        {
            return tuned->GetTuner()->OnControlKey(control);
        }
    }

    return false;
}

static void OnDraw_TuneParameter()
{
    tuned->GetTuner()->Draw();
}


DEF_PAGE_SB(pTuneParameter,   //-V641
    "ВВОД ЗНАЧЕНИЯ", "ENTER VALUE", //-V641
    "", "",
    Item::EmptyDark(),
    Item::EmptyDark(),
    &sbCancel,
    &sbEnter,
    Page::SB_Input, 0, Item::FuncActive, Page::FuncEnter, OnDraw_TuneParameter, OnControl_TuneParameter
)

Page *PageTuneParameter::self = reinterpret_cast<Page *>(const_cast<PageBase *>(&pTuneParameter));


void PageTuneParameter::SetModeEntering()
{
    pTuneParameter.items[0] = reinterpret_cast<Item *>(const_cast<SButtonBase *>(&sbMore));
    pTuneParameter.items[1] = reinterpret_cast<Item *>(const_cast<SButtonBase *>(&sbLess));
}
