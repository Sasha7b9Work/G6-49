#include "Display/Painter.h"
#include "Display/Symbols.h"
#include "Display/Text.h"
#include "Generator/ParameterPainter.h"
#include "Generator/Signals.h"
#include "Menu/Menu.h"
#include "Menu/MenuItems.h"
#include "Settings/Settings.h"
#include "PageTuneParameter.h"


static Parameter *parameter = nullptr;


static void OnDraw_TuneParameter();


void PageTuneParameter::SetParameter(Parameter *param)
{
    parameter = param;
}


static void Draw_Left(int, int)
{
}

static void OnPress_OrderUp()
{

}

DEF_SMALL_BUTTON(sbOrderUp,                                                                                                                               //--- ОКНО ВВОДА - СИМВОЛ ВЛЕВО ---
    "Порядок больше",
    "Увеличить порядок единицы измерения",
    *PageTuneParameter::self, Item::FuncActive, OnPress_OrderUp, Draw_Left
)


static void Draw_Right(int, int)
{
}

static void OnPress_OrderDown()
{

}

DEF_SMALL_BUTTON(sbOrderDown,                                                                                                                            //--- ОКНО ВВОДА - СИМВОЛ ВПРАВО ---
    "Порядок меньшше",
    "Уменьшить порядок единицы измерения",
    *PageTuneParameter::self, Item::FuncActive, OnPress_OrderDown, Draw_Right
)



static void OnPress_Cancel()
{
    parameter = nullptr;
    Menu::ResetAdditionPage();
}

static void Draw_Cancel(int x, int y)
{
    String("Отменить").Draw(x + 13, y + 12);
    Text::Draw4SymbolsInRect(x + 25, y + 28, Ideograph::_8::Delete);
}

DEF_SMALL_BUTTON(sbCancel,                                                                                                                                      //--- ОКНО ВВОДА - ОТМЕНА ---
    "ЕДИНИЦЫ ИЗМЕРЕНИЯ",
    "Отказаться от ввода нового значения",
    *PageTuneParameter::self, Item::FuncActive, OnPress_Cancel, Draw_Cancel
)


static void Draw_Enter(int x, int y)
{
    String("Подтвердить").Draw(x + 3, y + 12);
    Text::Draw4SymbolsInRect(x + 25, y + 28, Ideograph::_8::Save);
}

static void OnPress_Enter()
{

}

DEF_SMALL_BUTTON(sbEnter,                                                                                                                                         //--- ОКНО ВВОДА - ВВОД ---
    "ВВОД",
    "Подтвердить ввод нового значения",
    *PageTuneParameter::self, Item::FuncActive, OnPress_Enter, Draw_Enter
)


static bool OnControl_TuneParameter(Key &)
{
    return false;
}


DEF_PAGE_SB( pTuneParameter,   //-V641
    "ВВОД ЗНАЧЕНИЯ", //-V641
    "",
    &sbOrderUp,         ///< ОКНО ВВОДА - СИМВОЛ ВЛЕВО
    &sbOrderDown,       ///< ОКНО ВВОДА - СИМВОЛ ВПРАВО
    &sbCancel,          ///< ОКНО ВВОДА - ОТМЕНА
    &sbEnter,           ///< ОКНО ВВОДА - ВВОД
    Page::SB_Input, 0, Item::FuncActive, FuncEnter, OnDraw_TuneParameter, OnControl_TuneParameter
)

Page *PageTuneParameter::self = reinterpret_cast<Page *>(const_cast<PageBase *>(&pTuneParameter));


/// Отобразить название параметра
static void DrawNameParameter(int x, int y, int width)
{
    int length = Font::GetLengthText(parameter->Name());

    int pos = x + width / 2 - length / 2;

    Text::Draw(pos, y + 15, parameter->Name(), Color::WHITE);
}


/// Отобразить значение параметра
static void DrawParameter(const ParameterPainterSupporting &support, int y)
{
    Color::WHITE.SetAsCurrent();

    char buffer[2] = { 0, 0 };

    for(uint i = 0; i < support.NumSymbols(); i++)
    {
        buffer[0] = support.Symbol(i);
        Text::Draw(support.X(i), y, buffer);
    }
}


static void OnDraw_TuneParameter()
{
    if(parameter == nullptr)
    {
        return;
    }

    Font::Store();
    Font::Set(TypeFont::_GOSTB20);
    Text::SetUpperCase(false);

    int x = Wave::Graphics::X();
    int y = Wave::Graphics::Y(Chan::GetInverse(CURRENT_CHANNEL)) + 1;
    int width = Wave::Graphics::Width() - 1;
    int height = Wave::Graphics::Height() - 4;

    Painter::FillRegion(x, y, width, height, Color::BLACK);

    DrawNameParameter(x, y, width);

    ParameterPainterSupporting support(parameter, x, width);

    DrawParameter(support, y + 60);

    Font::Restore();
}
