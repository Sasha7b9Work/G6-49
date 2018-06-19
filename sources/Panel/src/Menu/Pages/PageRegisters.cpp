#include "PageRegisters.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Generator.h"
#include "Display/Symbols.h"
#include "Wave.h"
#include "Menu/Menu.h"
#include "Command.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include <string.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define X_INPUT      5
#define Y_INPUT      170
#define WIDTH_INPUT  240
#define HEIGHT_INPUT 60

extern PageBase pRegisters;
Page *PageRegisters::pointer = (Page *)&pRegisters;
Name_Register currentRegister = FreqMeterLevel;
bool showInputWindow = false;
extern const ButtonBase bBackspace;
extern const ButtonBase bCancel;
extern const ButtonBase bSave;

#define SIZE_BUFFER 10
/// Здесь хранятся введённые символы
static char buffer[SIZE_BUFFER + 1];
/// Это указатель на первый своодный символ
static int pos = 0;
/// true, означает, что значение в этот регистр уже засылалось
static bool sending[NumRegisters] = {false, false, false, false, false, false};
/// Здесь засланные значения для каждого регистра
static uint values[NumRegisters];


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
        if(sending[i])
        {
            if(i == currentRegister)
            {
                Painter::SetColor(Color::FILL);
            }
            Text::DrawFormatText(x + 130, y + i * 10, Int2String((int)values[i], false, 1));
        }
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

    int x = X_INPUT + 5;

    for(int i = 0; i < pos; i++)
    {
        x = Text::DrawBigChar(x, Y_INPUT + 20, 5, buffer[i]) + 3;
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPress_Prev()
{
    CircleDecrease<uint8>((uint8 *)&currentRegister, 0, NumRegisters - 1);
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
    CircleIncrease<uint8>((uint8 *)&currentRegister, 0, NumRegisters - 1);
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
    showInputWindow = true;

    pRegisters.items[0] = (Item *)&bBackspace;
    pRegisters.items[1] = (Item *)&bCancel;
    pRegisters.items[2] = (Item *)&bSave;

    if(sending[currentRegister])
    {
        Int2String((int)values[currentRegister], false, 1, buffer);
        pos = (int)strlen(buffer);
    }
    else
    {
        pos = 0;

    }
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
    if(pos > 0)
    {
        pos--;
    }
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

void LoadRegister()
{
    buffer[pos] = '\0';

    uint value = 0;

    if(String2UInt(buffer, &value))
    {
        Generator::LoadRegister(currentRegister, value);
        values[currentRegister] = value;
        sending[currentRegister] = true;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Save()
{
    LoadRegister();
    OnPress_Cancel();
}

static void OnDraw_Save(int x, int y)
{
    Text::SetFont(TypeFont_UGO2);
    Text::Draw4SymbolsInRect(x + 15, y + 30, SYMBOL_SAVE, Color::FILL);
    Text::SetFont(TypeFont_8);
}

DEF_BUTTON(bSave,                                                                                          //--- РЕГИСТРЫ - Окно ввода --- Заслать ---
    "Заслать", "Send",
    "Записывает значение в выбранный регистр",
    "Writes a value to the selected register",
    pRegisters, FuncActive, OnPress_Save, OnDraw_Save
)


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool OnKey(StructControl strCntrl)
{
    if(!showInputWindow && KeyIsDigit(strCntrl.key))
    {
        showInputWindow = true;
        pos = 0;
    }
    else if(showInputWindow && KeyIsDigit(strCntrl.key) && strCntrl.typePress == Down)
    {
        if(pos < SIZE_BUFFER)
        {
            buffer[pos++] = KeyToChar(strCntrl.key);
        }
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
