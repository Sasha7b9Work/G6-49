#include "Display/Painter.h"
#include "Display/Symbols.h"
#include "Display/Text.h"
#include "Generator/Signals.h"
#include "Menu/Menu.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Pages.h"
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


static void Draw_More(int x, int y)
{
    String(LANG_IS_RU ? "Больше" : "Increase").DrawInCenterRect(x, y, 70, 30);
    Char(Ideograph::_8::Up).Draw2Horizontal(x + 25, y + 30);
}


DEF_SMALL_BUTTON(sbMore,
    "Больше", "More",
    "Увеличить порядок", "Increase order",
    *PageTuneParameter::self, Item::FuncActive, OnPress_More, Draw_More
)


static void OnPress_Less()
{

}


static void Draw_Less(int x, int y)
{
    String(LANG_IS_RU ? "Меньше" : "Decrease").DrawInCenterRect(x, y, 70, 30);
    Char(Ideograph::_8::Down).Draw2Horizontal(x + 25, y + 30);
}


DEF_SMALL_BUTTON(sbLess,
    "Меньше", "Less",
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
    String(LANG_IS_RU ? "Отменить" : "Cancel").DrawInCenterRect(x, y, 70, 30);
    Char(Ideograph::_8::Delete).Draw4InRect(x + 25, y + 28);
}

DEF_SMALL_BUTTON(sbCancel,                                                                                                                                      //--- ОКНО ВВОДА - ОТМЕНА ---
    "Отменить", "Cancel",
    "Отказаться от ввода нового значения", "Refuse to enter a new value",
    *PageTuneParameter::self, Item::FuncActive, OnPress_Cancel, Draw_Cancel
)


static void Draw_Enter(int x, int y)
{
    String(LANG_IS_RU ? "Применить" : "Apply").DrawInCenterRect(x, y, 70, 30);
    Char(Ideograph::_8::Save).Draw4InRect(x + 25, y + 28);
}

static void OnPress_Enter()
{
    Menu::ResetAdditionPage();
}

DEF_SMALL_BUTTON(sbEnter,                                                                                                                                         //--- ОКНО ВВОДА - ВВОД ---
    "Применить", "Apply",
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
