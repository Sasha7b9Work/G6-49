#include "PageRegisters.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Display/Symbols.h"
#include "Wave.h"
#include "Menu/Menu.h"
#include "Command.h"
#include "Utils/Math.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern PageBase pRegisters;
Page *PageRegisters::pointer = (Page *)&pRegisters;
Name_Register PageRegisters::currentRegister = FreqMeterLevel;
bool showInputWindow = false;

#define X_INPUT      5
#define Y_INPUT      170
#define WIDTH_INPUT  240
#define HEIGHT_INPUT 60


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PageRegisters::Draw()
{
    if(Menu::CurrentPage() != pointer)
    {
        return;
    }

    Painter::FillRegion(Wave::X(), Wave::Y(A) + 1, Wave::Width() - 1, Wave::Height() * 2, Color::BACK);

    DrawRegisters(Wave::X() + 4, Wave::Y(A) + 3);

    DrawInputWindow();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void PageRegisters::DrawRegisters(int x, int y)
{
    for(uint8 i = 0; i < NumRegisters; i++)
    {
        Register reg(i);
        Color color = Color::FILL;
        if(i == currentRegister)
        {
            Painter::FillRegion(x - 1, y + i * 10, 127, 8, Color::FILL);
            color = Color::BACK;
        }
        Text::DrawText(x, y + i * 10, reg.Name(), color);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void PageRegisters::DrawInputWindow()
{
    if(!showInputWindow)
    {
        return;
    }

    Painter::DrawRectangle(X_INPUT, Y_INPUT, WIDTH_INPUT, HEIGHT_INPUT, Color::FILL);
    Register reg(currentRegister);
    Text::DrawBigText(X_INPUT + 17, Y_INPUT + 2, 2, reg.Name());
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPress_Prev()
{
    CircleDecrease<uint8>((uint8 *)&PageRegisters::currentRegister, 0, NumRegisters - 1);
}

DEF_BUTTON( bPrev,                                                                                                     //--- РЕГИСТРЫ - Предыдущий ---
    "Предыдущий", "Previous",
    "Переход к предыдущему регистру",
    "Go to the previous register",
    pRegisters, FuncActive, OnPress_Prev, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Next()
{
    CircleIncrease<uint8>((uint8 *)&PageRegisters::currentRegister, 0, NumRegisters - 1);
}

DEF_BUTTON(bNext,                                                                                                       //--- РЕГИСТРЫ - Следующий ---
    "Следующий", "Next",
    "Переход к следующему регистру",
    "Go to the next register",
    pRegisters, FuncActive, OnPress_Next, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Send()
{
}

DEF_BUTTON(bSend,                                                                                                         //--- РЕГИСТРЫ - Заслать ---
    "Заслать", "Send",
    "Открывает окно ввода значения регистра",
    "Opens the register value entry window",
    pRegisters, FuncActive, OnPress_Send, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Backspace()
{
}

static void OnDraw_Backspace(int x, int y)
{
    Text::SetFont(TypeFont_UGO2);
    Text::Draw4SymbolsInRect(x + 15, y + 30, SYMBOL_LEFT, Color::FILL);
    Text::SetFont(TypeFont_8);
}

DEF_BUTTON(bBackspace,                                                                                   //--- РЕГИСТРЫ - Input window - Backspace ---
    "Backspace", "Backspace",
    "Удаляет последний введённый символ",
    "Deletes the last character you typed",
    pRegisters, FuncActive, OnPress_Backspace, OnDraw_Backspace
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Cancel()
{
    showInputWindow = false;
    pRegisters.items[0] = (Item *)&bPrev;
    pRegisters.items[1] = (Item *)&bNext;
    pRegisters.items[2] = (Item *)&bSend;
}

static void OnDraw_Cancel(int x, int y)
{
    Text::SetFont(TypeFont_UGO2);
    Text::Draw4SymbolsInRect(x + 15, y + 30, SYMBOL_DELETE, Color::FILL);
    Text::SetFont(TypeFont_8);
}

DEF_BUTTON(bCancel,                                                                                         //--- РЕГИСТРЫ - Input window - Отмена ---
    "Отмена", "Cancel",
    "Отменяет засылку значения в регистр и закрывает окно ввода",
    "Cancels the sending of values into the register and closes the input window",
    pRegisters, FuncActive, OnPress_Cancel, OnDraw_Cancel
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Save()
{
}

static void OnDraw_Save(int x, int y)
{
    Text::SetFont(TypeFont_UGO2);
    Text::Draw4SymbolsInRect(x + 15, y + 30, SYMBOL_SAVE, Color::FILL);
    Text::SetFont(TypeFont_8);
}

DEF_BUTTON(bSave,
    "Заслать", "Send",
    "Записывает значение в выбранный регистр",
    "Writes a value to the selected register",
    pRegisters, FuncActive, OnPress_Save, OnDraw_Save
)


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool OnKey(StructControl strCntrl)
{
    if(KeyIsDigit(strCntrl.key) && !showInputWindow)
    {
        showInputWindow = true;

        pRegisters.items[0] = (Item *)&bBackspace;
        pRegisters.items[1] = (Item *)&bCancel;
        pRegisters.items[2] = (Item *)&bSave;
    }

    return false;
}

static Item emptyItem = {Item_None};

DEF_PAGE_4_VAR(pRegisters,                                                                                                          //--- РЕГИСТРЫ ---
    "РЕГИСТРЫ", "REGISTERS",
    "",
    "",
    bPrev,
    bNext,
    bSend,
    emptyItem,
    Page_Registers, 0, FuncActive, FuncPress, OnKey
)
