#include "PageRegisters.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Generator/Generator.h"
#include "Display/Symbols.h"
#include "Wave.h"
#include "Menu/Menu.h"
#include "Command.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include "Utils/NumberBuffer.h"
#include <string.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define X_INPUT      5
#define Y_INPUT      183
#define WIDTH_INPUT  240
#define HEIGHT_INPUT 52

/// Тип вводимых чисел в окне ввода
enum TypeInput
{
    Uint,           ///< Десятичное число. Можно ввести значение до 2^64.
    Binary,         ///< Последовательность нулей и единиц
    Uint10_Uint10,  ///< Два восьмибитных числа в десятичном виде.
    Uint14_Uint14,  ///< Два числа, максимум 2^14, в десятичном виде
};

extern PageBase pRegisters;
Page *PageRegisters::pointer = (Page *)&pRegisters;
/// Регистр, в который будет производится занесение значения по нажатию кнопки ЗАСЛАТЬ
Register currentRegister = Register::FreqMeterLevel;
/// Флаг показа окна ввода
static bool showInputWindow = false;
extern const ButtonBase bBackspace;
extern const ButtonBase bCancel;
extern const ButtonBase bSave;

#define MAX_SIZE_BUFFER 14
/// Здесь хранятся введённые символы
static char buffer[MAX_SIZE_BUFFER + 1];

struct DescInput
{
    int size;
    TypeInput type;
    bool sending;       // true означает, что значение в этот регистр уже засылалось
    uint64 value;       // засланное значение
};

#define VALUE(i)   (desc[i].value)
#define SENDING(i) (desc[i].sending)

static DescInput desc[Register::Number] =
{
    {2,  Uint          , false, 0 }, // Multiplexor1,
    {2,  Uint          , false, 0 }, // Multiplexor2,
    {10, Uint          , false, 0 }, // OffsetA,
    {10, Uint          , false, 0 }, // OffsetB,
    {10, Uint          , false, 0 }, // FreqMeterLevel,
    {10, Uint          , false, 0 }, // FreqMeterHYS,
    {8,  Binary        , false, 0 }, // FPGA_RG0_Control,
    {13, Uint          , false, 0 }, // FPGA_RG1_Freq,
    {11, Uint10_Uint10 , false, 0 }, // FPGA_RG2_Amplitude,
    {11, Uint14_Uint14 , false, 0 }, // FPGA_RG3_RectA,
    {11, Uint14_Uint14 , false, 0 }, // FPGA_RG4_RectB,
    {10, Uint          , false, 0 }, // FPGA_RG5_PeriodImpulseA,
    {10, Uint          , false, 0 }, // FPGA_RG6_DurationImpulseA,
    {10, Uint          , false, 0 }, // FPGA_RG7_PeriodImpulseB,
    {10, Uint          , false, 0 }, // FPGA_RG8_DurationImpulseB,
    {12, Binary        , false, 0 }, // FPGA_RG9_FreqMeter
    {11, Uint14_Uint14 , false, 0 }, // FPGA_RG10_Offset
    {2,  Uint          , false, 0 }, // Multiplexor3
    {0},
    {0},
    {0}
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Возвращает размер буфера для регистра i
static int SizeBuffer(Register name = Register::Number);
/// Возвращает тип ввода для регистра i
static TypeInput TypeBuffer(Register name = Register::Number);
/// Возращает true, если символ является корректным для данного типа ввода
static bool AllowableSymbol(Control key);
/// Выводит значение регистра i
static void DrawValue(int x, int y, uint8 i);
/// Возвращает из буфера значение, предшествующее точке
static uint64 FirstValue();
/// Возвращает из буфера значение, следующее за точкой
static uint64 SecondValue();
/// Преобразует строку из buffer в uint
static uint64 BufferToValue();


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static int SizeBuffer(Register name)
{
    name = (name == Register::Number) ? currentRegister : name;

    return desc[name].size;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static TypeInput TypeBuffer(Register name)
{
    name = (name == Register::Number) ? currentRegister : name;

    return desc[name].type;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool AllowableSymbol(Control key)
{
    TypeInput type = TypeBuffer(currentRegister);

    if(type == Uint)
    {
        return key.IsDigit();
    }
    else if(type == Binary)
    {
        return key.Is(Control::B_0) || key.Is(Control::B_1);
    }
    else if(type == Uint10_Uint10 || type == Uint14_Uint14)
    {
        if(key.IsDigit())
        {
            return true;
        }

        if (key.Is(Control::B_Dot))
        {
            for (int i = 0; i < sizeof(buffer); i++)
            {
                if (buffer[i] == '.')
                {
                    return false;
                }
            }
            return true;
        }
    }

    return false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void PageRegisters::Draw()
{
    if(Menu::CurrentPage() != pointer)
    {
        return;
    }

    Painter::FillRegion(Wave::Graphics::X(), Wave::Graphics::Y(Chan::A) + 1, Wave::Graphics::Width() - 1, Wave::Graphics::Height() * 2, Color::BACK);

    DrawRegisters(Wave::Graphics::X() + 4, Wave::Graphics::Y(Chan::A) + 3);

    DrawInputWindow();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void PageRegisters::DrawRegisters(int x, int y)
{
    for(uint8 i = 0; i < Register::Number; i++)
    {
        Register reg((Register::E)i);
        Color color = Color::FILL;
        if(i == currentRegister)
        {
            Painter::FillRegion(x - 1, y + i * 10, 132, 8, Color::FILL);
            color = Color::BACK;
        }
        Text::DrawText(x, y + i * 10, reg.Name(), color);
        DrawValue(x + 135, y + i * 10, i);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawValue(int x, int y, uint8 i)
{
    if(!SENDING(i))
    {
        return;
    }

    Painter::SetColor(Color::FILL);

    Register name((Register::E)i);

    TypeInput type = TypeBuffer(name);

    if(type == Uint)
    {
        Text::DrawFormatText(x, y, UInt64_2String(VALUE(i)));
    }
    else if(type == Binary)
    {
        char buf[33];

        Text::DrawFormatText(x, y, Bin2StringN((uint)VALUE(i) , buf, SizeBuffer(name)));
    }
    else if(type == Uint10_Uint10 || type == Uint14_Uint14)
    {
        uint mask = type == Uint10_Uint10 ? 0x3ffU : 0x3fffU;
        int numBits = type == Uint10_Uint10 ? 10 : 14;

        uint first = VALUE(i) & mask;
        uint second = (VALUE(i) >> numBits) & mask;
        x = Text::DrawFormatText(x, y, UInt2String(first));
        x = Text::DrawFormatText(x, y, ".");
        Text::DrawFormatText(x, y, UInt2String(second));
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
    Painter::FillRegion(X_INPUT + 1, Y_INPUT + 1, WIDTH_INPUT - 2, HEIGHT_INPUT - 2, Color::BACK);
    Register reg(currentRegister);
    Text::DrawBigText(X_INPUT + 3, Y_INPUT + 2, 2, reg.Name(), Color::FILL);

    int x = X_INPUT + 5;

    int position = NumberBuffer::PositionCursor();

    for(int i = 0; i < (int)strlen(buffer); i++)
    {
        if(i == position)
        {
            Painter::DrawFilledRectangle(x - 2, Y_INPUT + 19, 19, 31, Color::GRAY_10, Color::BLUE);
            Painter::SetColor(Color::FILL);
        }
        x = Text::DrawBigChar(x, Y_INPUT + 20, 4, buffer[i]) + 3;
    }
    if (position == (int)strlen(buffer) && position < SizeBuffer())
    {
        Painter::DrawFilledRectangle(x - 2, Y_INPUT + 19, 19, 31, Color::GRAY_10, Color::BLUE);
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPress_Prev()
{
    CircleDecrease<uint8>((uint8 *)&currentRegister, 0, Register::Number - 1);
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
    CircleIncrease<uint8>((uint8 *)&currentRegister, 0, Register::Number - 1);
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
    memset(buffer, 0, MAX_SIZE_BUFFER);

    pRegisters.items[0] = (Item *)&bBackspace;
    pRegisters.items[1] = (Item *)&bCancel;
    pRegisters.items[2] = (Item *)&bSave;

    int position = 0;

    if(SENDING(currentRegister))
    {
        TypeInput type = TypeBuffer();

        if (type == Uint)
        {
            UInt64_2String(VALUE(currentRegister), buffer);
            position = (int)strlen(buffer);
        }
        else if(type == Binary)
        {
            Bin2StringN((uint)VALUE(currentRegister), buffer, SizeBuffer(currentRegister));
            position = (int)strlen(buffer);
        }
        else if(type == Uint10_Uint10 || type == Uint14_Uint14)
        {
            uint mask = type == Uint10_Uint10 ? 0x3ffU : 0x3fffU;
            int numBits = type == Uint10_Uint10 ? 10 : 14;

            uint first = VALUE(currentRegister) & mask;
            uint second = (VALUE(currentRegister) >> numBits) & mask;
            
            strcpy(buffer, UInt2String(first));
            strcat(buffer, ".");
            strcat(buffer, UInt2String(second));
        }
    }
    else
    {
        position = 0;
        memset(buffer, 0, MAX_SIZE_BUFFER);
        VALUE(position) = 0;
    }

    NumberBuffer::Set(buffer, desc[currentRegister].size, position, (currentRegister == Register::FreqMeterLevel || 
                                                                     currentRegister == Register::FreqMeterHYS) ? 4095 : 0);
}

DEF_BUTTON(bSend,                                                                                                         //--- РЕГИСТРЫ - Заслать ---
    "Заслать", "Send",
    "Открывает окно ввода значения регистра",
    "Opens the register value entry window",
    pRegisters, FuncActive, OnPress_Send, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnDraw_Backspace(int x, int y)
{
    Font::Set(Font::Type::_UGO2);
    Text::Draw4SymbolsInRect(x + 15, y + 30, SYMBOL_LEFT, Color::FILL);
    Font::Set(Font::Type::_8);
}

DEF_BUTTON(bBackspace,                                                                                   //--- РЕГИСТРЫ - Input window - Backspace ---
    "Backspace", "Backspace",
    "Удаляет последний введённый символ",
    "Deletes the last character you typed",
    pRegisters, FuncActive, NumberBuffer::PressBackspace, OnDraw_Backspace
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Cancel()
{
    showInputWindow = false;
    memset(buffer, 0, MAX_SIZE_BUFFER);
    pRegisters.items[0] = (Item *)&bPrev;
    pRegisters.items[1] = (Item *)&bNext;
    pRegisters.items[2] = (Item *)&bSend;
}

static void OnDraw_Cancel(int x, int y)
{
    Font::Set(Font::Type::_UGO2);
    Text::Draw4SymbolsInRect(x + 15, y + 30, SYMBOL_DELETE, Color::FILL);
    Font::Set(Font::Type::_8);
}

DEF_BUTTON(bCancel,                                                                                         //--- РЕГИСТРЫ - Input window - Отмена ---
    "Отмена", "Cancel",
    "Отменяет засылку значения в регистр и закрывает окно ввода",
    "Cancels the sending of values into the register and closes the input window",
    pRegisters, FuncActive, OnPress_Cancel, OnDraw_Cancel
)

uint64 FirstValue()
{
    char buff[20];

    for(uint i = 0; i < sizeof(buffer); i++)
    {
        if(buffer[i] == '.')
        {
            for(uint j = 0; j < i; j++)
            {
                buff[j] = buffer[j];
            }
            buff[i] = 0;

            uint64 result = 0;

            if(String2UInt64(buff, &result))
            {
                return result;
            }
            break;
        }
    }

    return 0;
}

uint64 SecondValue()
{
    for(uint i = 0; i < sizeof(buffer); i++)
    {
        if(buffer[i] == '.')
        {
            uint64 result = 0;

            if(String2UInt64(&buffer[i + 1], &result))
            {
                return result;
            }
            break;
        }
    }

    return 0;
}

static uint64 BufferToValue()
{
    TypeInput type = TypeBuffer(currentRegister);

    uint64 result = 0;

    if(type == Uint)
    {
        if(!String2UInt64(buffer, &result))
        {
            result = 0;
        }
    }
    else if(type == Binary)
    {
        result = StringToBin32(buffer);
    }
    else if(type == Uint10_Uint10 || type == Uint14_Uint14)
    {
        int numBits = type == Uint10_Uint10 ? 10 : 14;

        uint64 first = FirstValue();
        uint64 second = SecondValue();

        result = first + (second << numBits);
    }

    return result;
}

static void LoadRegister()
{
    volatile uint64 value = BufferToValue();

    VALUE(currentRegister) = value;
    SENDING(currentRegister) = true;
    Generator::LoadRegister(currentRegister, VALUE(currentRegister));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Save()
{
    LoadRegister();
    OnPress_Cancel();
}

static void OnDraw_Save(int x, int y)
{
    Font::Set(Font::Type::_UGO2);
    Text::Draw4SymbolsInRect(x + 15, y + 30, SYMBOL_SAVE, Color::FILL);
    Font::Set(Font::Type::_8);
}

DEF_BUTTON(bSave,                                                                                          //--- РЕГИСТРЫ - Окно ввода --- Заслать ---
    "Заслать", "Send",
    "Записывает значение в выбранный регистр",
    "Writes a value to the selected register",
    pRegisters, FuncActive, OnPress_Save, OnDraw_Save
)

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool OnRegulator(Control key)
{
    if (TypeBuffer(currentRegister) == Uint)
    {
        if(key.Is(Control::REG_RIGHT) || key.Is(Control::REG_LEFT))
        {
            NumberBuffer::ProcessKey(key);
            LoadRegister();
            return true;
        }
    }

    return false;
}


static bool OnKey(Control control)
{
    if(!showInputWindow)
    {
        if (AllowableSymbol(control))
        {
            SENDING(currentRegister) = false;
            OnPress_Send();
            memset(buffer, 0, MAX_SIZE_BUFFER);
            buffer[0] = control.ToChar();
            NumberBuffer::Set(buffer, MAX_SIZE_BUFFER, 1, (currentRegister == Register::FreqMeterLevel || 
                                                           currentRegister == Register::FreqMeterHYS) ? 4095 : 0);
            return true;
        }
    }
    else if(control.action.Is(Control::Action::Down))
    {
        if (AllowableSymbol(control))
        {
            NumberBuffer::ProcessKey(control);
            return true;
        }
        else if(control.Is(Control::B_RIGHT) || control.Is(Control::B_LEFT))
        {
            NumberBuffer::ProcessKey(control);
            return true;
        }
        else if(control.Is(Control::B_ESC))
        {
            OnPress_Cancel();
            return true;
        }
        else
        {
            return OnRegulator(control);
        }
    }

    return false;
}

static Item emptyItem = {Item::Type::None};

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
