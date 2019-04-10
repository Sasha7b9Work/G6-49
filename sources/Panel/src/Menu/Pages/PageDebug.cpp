#include "stdafx.h"
#ifndef WIN32
#include "PageDebug.h"
#include "Settings/Settings.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Display/Symbols.h"
#include "PageService.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include "Utils/NumberBuffer.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define X_INPUT      5
#define Y_INPUT      183
#define WIDTH_INPUT  240
#define HEIGHT_INPUT 52


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pRegisters;
extern const PageBase pDebug;
extern volatile const ButtonBase bBackspace;
extern volatile const ButtonBase bCancel;
extern volatile const ButtonBase bSave;

static Item emptyItem = {Item::Type::None};

Page *PageDebug::pointer = (Page *)&pDebug;
/// Регистр, в который будет производится занесение значения по нажатию кнопки ЗАСЛАТЬ
Register::E currentRegister = Register::FreqMeterLevel;
/// Флаг показа окна ввода
static bool showInputWindow = false;
#define MAX_SIZE_BUFFER 14
/// Здесь хранятся введённые символы
static char buffer[MAX_SIZE_BUFFER + 1];

/// Тип вводимых чисел в окне ввода
enum TypeInput
{
    Uint,           ///< Десятичное число. Можно ввести значение до 2^64.
    Binary,         ///< Последовательность нулей и единиц
    Uint10_Uint10,  ///< Два восьмибитных числа в десятичном виде.
    Uint14_Uint14   ///< Два числа, максимум 2^14, в десятичном виде
};

struct DescInput
{
    int size;
    TypeInput type;
    bool sending;       // true означает, что значение в этот регистр уже засылалось
    uint8 nu[7];
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
    {0},
    {0},
    {0}
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// Возвращает размер буфера для регистра i
static int SizeBuffer(Register::E name = Register::Number);
/// Возвращает тип ввода для регистра i
static TypeInput TypeBuffer(Register::E name = Register::Number);
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

static void OnPress_Send();

static void OnPress_Cancel();



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void LoadRegister()
{
    volatile uint64 value = BufferToValue();

    VALUE(currentRegister) = value;
    SENDING(currentRegister) = true;
    Generator::LoadRegister(currentRegister, VALUE(currentRegister));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool OnRegulator(Control key)
{
    if (TypeBuffer(currentRegister) == Uint)
    {
        if (key.IsRotate())
        {
            NumberBuffer::ProcessKey(key);
            LoadRegister();
            return true;
        }
    }

    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool OnKey(Control key)
{
    if (!showInputWindow)
    {
        if (AllowableSymbol(key))
        {
            SENDING(currentRegister) = false;
            OnPress_Send();
            std::memset(buffer, 0, MAX_SIZE_BUFFER + 1);
            buffer[0] = key.ToChar();
            NumberBuffer::Set(buffer, MAX_SIZE_BUFFER, 1, (currentRegister == Register::FreqMeterLevel ||
                                                           currentRegister == Register::FreqMeterHYS) ? 4095 : 0);
            return true;
        }
    }
    else if (key.action.Is(Control::Action::Down))
    {
        if (AllowableSymbol(key))
        {
            NumberBuffer::ProcessKey(key
            );
            return true;
        }
        else if (key.IsCursors())
        {
            NumberBuffer::ProcessKey(key);
            return true;
        }
        else if (key.Is(Control::Esc))
        {
            OnPress_Cancel();
            return true;
        }
        else
        {
            return OnRegulator(key);
        }
    }
    else
    {
        // здесь ничего
    }

    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static int SizeBuffer(Register::E name)
{
    name = (name == Register::Number) ? currentRegister : name;

    return desc[name].size;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static TypeInput TypeBuffer(Register::E name)
{
    name = (name == Register::Number) ? currentRegister : name;

    return desc[name].type;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint64 FirstValue()
{
    char buff[20];

    for (uint i = 0; i < sizeof(buffer); i++)
    {
        if (buffer[i] == '.')
        {
            for (uint j = 0; j < i; j++)
            {
                buff[j] = buffer[j];
            }
            buff[i] = 0;

            uint64 result = 0;

            if (String2UInt64(buff, &result))
            {
                return result;
            }
            break;
        }
    }

    return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint64 SecondValue()
{
    for (uint i = 0; i < sizeof(buffer); i++)
    {
        if (buffer[i] == '.')
        {
            uint64 result = 0;

            if (String2UInt64(&buffer[i + 1], &result))
            {
                return result;
            }
            break;
        }
    }

    return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static uint64 BufferToValue()
{
    TypeInput type = TypeBuffer(currentRegister);

    uint64 result = 0;

    if (type == Uint)
    {
        if (!String2UInt64(buffer, &result))
        {
            result = 0;
        }
    }
    else if (type == Binary)
    {
        result = StringToBin32(buffer);
    }
    //else if (type == Uint10_Uint10 || type == Uint14_Uint14)
    else
    {
        int numBits = type == Uint10_Uint10 ? 10 : 14;

        uint64 first = FirstValue();
        uint64 second = SecondValue();

        result = first + (second << numBits);
    }

    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool AllowableSymbol(Control key)
{
    TypeInput type = TypeBuffer(currentRegister);

    if (type == Uint)
    {
        return key.IsDigit();
    }
    else if (type == Binary)
    {
        return key.Is(Control::_0) || key.Is(Control::_1);
    }
    else // if (type == Uint10_Uint10 || type == Uint14_Uint14)
    {
        if (key.IsDigit())
        {
            return true;
        }

        if (key.Is(Control::Dot))
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageDebug::PageRegisters::Draw()
{
    if (CURRENT_PAGE != (Page *)&pRegisters)
    {
        return;
    }

    Painter::FillRegion(Wave::Graphics::X(), Wave::Graphics::Y(Chan::A) + 1, Wave::Graphics::Width() - 1, Wave::Graphics::Height() * 2, Color::BACK);

    DrawRegisters(Wave::Graphics::X() + 4, Wave::Graphics::Y(Chan::A) + 3);

    DrawInputWindow();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageDebug::PageRegisters::DrawRegisters(int x, int y)
{
    for (uint8 i = 0; i < Register::Number; i++)
    {
        Register reg((Register::E)i);
        Color color = Color::FILL;
        if (i == currentRegister)
        {
            Painter::FillRegion(x - 1, y + i * 10, 132, 8, Color::FILL);
            color = Color::BACK;
        }
        String(reg.Name()).Draw(x, y + i * 10, color);
        DrawValue(x + 135, y + i * 10, i);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageDebug::PageRegisters::DrawInputWindow()
{
    if (!showInputWindow)
    {
        return;
    }

    Painter::DrawRectangle(X_INPUT, Y_INPUT, WIDTH_INPUT, HEIGHT_INPUT, Color::FILL);
    Painter::FillRegion(X_INPUT + 1, Y_INPUT + 1, WIDTH_INPUT - 2, HEIGHT_INPUT - 2, Color::BACK);
    Register reg(currentRegister);
    Text::DrawBigText(X_INPUT + 3, Y_INPUT + 2, 2, reg.Name(), Color::FILL);

    int x = X_INPUT + 5;

    int position = NumberBuffer::PositionCursor();

    int size = (int)std::strlen(buffer);

    for (int i = 0; i < size; i++)
    {
        if (i == position)
        {
            Painter::DrawFilledRectangle(x - 2, Y_INPUT + 19, 19, 31, Color::GRAY_10, Color::BLUE);
            Painter::SetColor(Color::FILL);
        }
        x = Text::DrawBigChar(x, Y_INPUT + 20, 4, buffer[i]) + 3;
    }
    if (position == (int)std::strlen(buffer) && position < SizeBuffer())
    {
        Painter::DrawFilledRectangle(x - 2, Y_INPUT + 19, 19, 31, Color::GRAY_10, Color::BLUE);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawValue(int x, int y, uint8 i)
{
    if (!SENDING(i))
    {
        return;
    }

    Painter::SetColor(Color::FILL);

    Register::E name((Register::E)i);

    TypeInput type = TypeBuffer(name);

    if (type == Uint)
    {
        Text::DrawFormatText(x, y, UInt64_2String(VALUE(i)));
    }
    else if (type == Binary)
    {
        char buf[33];

        Text::DrawFormatText(x, y, Bin2StringN((uint)VALUE(i), buf, SizeBuffer(name)));
    }
    else // if (type == Uint10_Uint10 || type == Uint14_Uint14)
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_DebugMode(bool)
{
    Generator::SetDebugMode(DEBUG_MODE_ENABLED);
}

DEF_CHOICE_2(cConsole,                                                                                                     //--- ОТЛАДКА - КОНСОЛЬ ---
    "КОНСОЛЬ", "CONSOLE",
    "Включает и выключает отображение отладочной консоли",
    "Turns on or off the display of the debug console",
    DISABLED_RU, DISABLED_EN, "Отображение консоли включено",
                              "Console display enabled",
    ENABLED_RU,  ENABLED_EN,  "Отображение консоли выключено",
                              "Console display disabled",
    FLAG_1, BIT_CONSOLE, pDebug, FuncActive, OnPress_DebugMode, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_SaveSettings()
{
    set.Save();
}

DEF_BUTTON( bSaveSettings,                                                                                     //--- ОТЛАДКА - СОХРАНИТЬ НАСТРОЙКИ ---
    "СОХРАНИТЬ НАСТРОЙКИ", "SAVE SETTINGS",
    "Сохранить текущие настройки",
    "Save current settings",
    pDebug, FuncActive, OnPress_SaveSettings, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cStatistics,                                                                                              //--- ОТЛАДКА - Статистика ---
    "СТАТИСТИКА", "STATISTICS",
    "Включает/отключает показ на экране статистики по частоте кадров в виде ФПС / время отрисовки одного кадра / время, потраченое на отрисовку за "
    "весь кадр",
    "Enables / disables the display of statistics on the frame rate in the form of FPS / time of drawing one frame / time spent on rendering for the "
    "whole frame",
    DISABLED_RU, DISABLED_EN, "Показ статистики включён",
                              "Statistics show on",
    ENABLED_RU, ENABLED_EN,   "Показ статистики выключен",
                              "Statistics showing off",
    FLAG_1, BIT_STATISTICS, pDebug, FuncActive, FuncChangedChoice, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(   cShowSends,                                                                                   //--- ОТЛАДКА - Показывать параметры ---
    "Показывать параметры", "Show parameters",
    "Показывает параметры, засылаемые в ПЛИС",
    "Shows the parameters sent to the FPGA",
    DISABLED_RU, DISABLED_EN, "Показ параметров включён",
                              "Parameter display enabled",
    ENABLED_RU, ENABLED_EN, "Показ параметров выключен",
                            "Parameter display disabled",
    FLAG_1, BIT_SHOW_SENDS, pDebug, FuncActive, FuncChangedChoice, FuncDraw
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPress_Prev()
{
    CircleDecrease<uint8>((uint8 *)&currentRegister, 0, Register::Number - 1);
}

DEF_BUTTON(bPrev,                                                                                                     //--- РЕГИСТРЫ - Предыдущий ---
    "Предыдущий", "Previous",
    "Переход к предыдущему регистру",
    "Go to the previous register",
    pRegisters, FuncActive, OnPress_Prev, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Send()
{
    showInputWindow = true;
    std::memset(buffer, 0, MAX_SIZE_BUFFER);

    pRegisters.items[0] = (Item *)&bBackspace;
    pRegisters.items[1] = (Item *)&bCancel;
    pRegisters.items[2] = (Item *)&bSave;

    int position = 0;

    if (SENDING(currentRegister))
    {
        TypeInput type = TypeBuffer();

        if (type == Uint)
        {
            UInt64_2String(VALUE(currentRegister), buffer);
            position = (int)std::strlen(buffer);
        }
        else if (type == Binary)
        {
            Bin2StringN((uint)VALUE(currentRegister), buffer, SizeBuffer(currentRegister));
            position = (int)std::strlen(buffer);
        }
        else if (type == Uint10_Uint10 || type == Uint14_Uint14)
        {
            uint mask = type == Uint10_Uint10 ? 0x3ffU : 0x3fffU;
            int numBits = type == Uint10_Uint10 ? 10 : 14;

            uint first = VALUE(currentRegister) & mask;
            uint second = (VALUE(currentRegister) >> numBits) & mask;

            std::strcpy(buffer, UInt2String(first));
            std::strcat(buffer, ".");
            std::strcat(buffer, UInt2String(second));
        }
    }
    else
    {
        position = 0;
        std::memset(buffer, 0, MAX_SIZE_BUFFER);
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnDraw_Backspace(int x, int y)
{
    Text::Draw4SymbolsInRect(x + 15, y + 30, Ideograph::_8::Left, Color::FILL);
}

DEF_BUTTON(bBackspace,                                                                                   //--- РЕГИСТРЫ - Input window - Backspace ---
    "Backspace", "Backspace",
    "Удаляет последний введённый символ",
    "Deletes the last character you typed",
    pRegisters, FuncActive, NumberBuffer::PressBackspace, OnDraw_Backspace
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Save()
{
    LoadRegister();
    OnPress_Cancel();
}

static void OnDraw_Save(int x, int y)
{
    Text::Draw4SymbolsInRect(x + 15, y + 30, Ideograph::_8::Save, Color::FILL);
}

DEF_BUTTON(bSave,                                                                                          //--- РЕГИСТРЫ - Окно ввода --- Заслать ---
    "Заслать", "Send",
    "Записывает значение в выбранный регистр",
    "Writes a value to the selected register",
    pRegisters, FuncActive, OnPress_Save, OnDraw_Save
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Cancel()
{
    showInputWindow = false;
    std::memset(buffer, 0, MAX_SIZE_BUFFER);
    pRegisters.items[0] = (Item *)&bPrev;
    pRegisters.items[1] = (Item *)&bNext;
    pRegisters.items[2] = (Item *)&bSend;
}

static void OnDraw_Cancel(int x, int y)
{
    Text::Draw4SymbolsInRect(x + 15, y + 30, Ideograph::_8::Delete, Color::FILL);
}

DEF_BUTTON(bCancel,                                                                                         //--- РЕГИСТРЫ - Input window - Отмена ---
    "Отмена", "Cancel",
    "Отменяет засылку значения в регистр и закрывает окно ввода",
    "Cancels the sending of values into the register and closes the input window",
    pRegisters, FuncActive, OnPress_Cancel, OnDraw_Cancel
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_4_VAR(pRegisters,                                                                                                          //--- РЕГИСТРЫ ---
    "РЕГИСТРЫ", "REGISTERS",
    "",
    "",
    bPrev,
    bNext,
    bSend,
    emptyItem,
    Page::Registers, &pDebug, FuncActive, FuncPress, OnKey, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_5( pDebug,                                                                                                                  //--- ОТЛАДКА ---
    "ОТЛАДКА", "DEBUG",
    "", "",
    &pRegisters,     ///< ОТЛАДКА - Регистры
    &cConsole,       ///< ОТЛАДКА - Консоль
    &cStatistics,    ///< ОТЛАДКА - Статистика
    &cShowSends,     ///< ОТЛАДКА - Показывать параметры
    &bSaveSettings,  ///< ОТЛАДКА - Сохранить настройки
//    cBigSymbols,    ///< ОТЛАДКА - Больщие символы
    Page::Debug, PageService::pointer, FuncActive, FuncPress, FuncOnKey, FuncDrawPage
)


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
static void OnPress_BigSymbols(bool)
{
    Font::ToggleCharacterSize();
}

DEF_CHOICE_2 (  cBigSymbols,                                                                                       //--- ОТЛАДКА - Большие символы ---
    "Большие символы", "Large characters",
    "Отображение информации более крупными символами",
    "Display information with larger characters",
    DISABLED_RU, DISABLED_EN,   "Отображение информации стандартными символами",
                                "Displaying information with standard symbols",
    ENABLED_RU, ENABLED_EN,     "Отображение информации увеличенными символами",
                                "Displaying information with enlarged symbols",
    FLAG_1, BIT_BIG_SYMBOLS, pDebug, FuncActive, OnPress_BigSymbols, FuncDraw
)
*/
