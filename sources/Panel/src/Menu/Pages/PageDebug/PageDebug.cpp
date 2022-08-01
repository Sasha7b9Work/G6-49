// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "common/Messages.h"
#include "Menu/Pages/Pages.h"
#include "Settings/Settings.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Display/Symbols.h"
#include "Display/WaveGraphics.h"
#include "Settings/CalibrationSettings.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include "Utils/NumberBuffer.h"


using namespace Primitives;


#define X_INPUT      5
#define Y_INPUT      183
#define WIDTH_INPUT  240
#define HEIGHT_INPUT 52



extern const PageBase pRegisters;
extern const PageBase pDebug;
extern const ButtonBase bSend;


Page *PageDebug::self = reinterpret_cast<Page *>(const_cast<PageBase *>(&pDebug));
// Регистр, в который будет производится занесение значения по нажатию кнопки ЗАСЛАТЬ
Register::E currentRegister = Register::FreqMeterLevel;
// Флаг показа окна ввода
static bool showInputWindow = false;
#define MAX_SIZE_BUFFER 14
// Здесь хранятся введённые символы
static char dbuffer[MAX_SIZE_BUFFER + 1];

// Тип вводимых чисел в окне ввода
enum TypeInput
{
    Uint,           // Десятичное число. Можно ввести значение до 2^64.
    Binary,         // Последовательность нулей и единиц
    Uint10_Uint10,  // Два восьмибитных числа в десятичном виде.
    Uint14_Uint14   // Два числа, максимум 2^14, в десятичном виде
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

static DescInput desc[Register::Count] =
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




// Возвращает размер буфера для регистра i
static int SizeBuffer(Register::E name = Register::Count);
// Возвращает тип ввода для регистра i
static TypeInput TypeBuffer(Register::E name = Register::Count);
// Возращает true, если символ является корректным для данного типа ввода
static bool AllowableSymbol(Key::E key);
// Выводит значение регистра i
static void DrawValue(int x, int y, uint8 i);
// Возвращает из буфера значение, предшествующее точке
static uint64 FirstValue();
// Возвращает из буфера значение, следующее за точкой
static uint64 SecondValue();
// Преобразует строку из buffer в uint
static uint64 BufferToValue();

static void OnPress_Send();

static void OnPress_DebugCancel();




static void LoadRegister()
{
    volatile uint64 value = BufferToValue();

    VALUE(currentRegister) = value;
    SENDING(currentRegister) = true;
    PGenerator::LoadRegister(currentRegister, VALUE(currentRegister));
}


static bool OnRegulator(Control control)
{
    if (TypeBuffer(currentRegister) == Uint)
    {
        if (control.IsRotate())
        {
            NumberBuffer::ProcessKey(control.key);
            LoadRegister();
            return true;
        }
    }

    return false;
}


static bool OnKey_PageRegisters(const Control &control)
{
    if (!showInputWindow)
    {
        if (AllowableSymbol(control.key))
        {
            SENDING(currentRegister) = false;
            OnPress_Send();
            std::memset(dbuffer, 0, MAX_SIZE_BUFFER + 1);
            dbuffer[0] = Key(control.key).ToChar();
            NumberBuffer::Set(dbuffer, MAX_SIZE_BUFFER, 1, (currentRegister == Register::FreqMeterLevel ||
                                                           currentRegister == Register::FreqMeterHYS) ? 4095 : 0);
            return true;
        }
    }
    else if (control.IsDown())
    {
        if (AllowableSymbol(control.key))
        {
            NumberBuffer::ProcessKey(control.key);
            return true;
        }
        else if (control.IsCursors())
        {
            NumberBuffer::ProcessKey(control.key);
            return true;
        }
        else if (control.Is(Key::Esc))
        {
            OnPress_DebugCancel();
            return true;
        }
        else
        {
            return OnRegulator(control);
        }
    }
    
    return false;
}


static int SizeBuffer(Register::E name)
{
    name = (name == Register::Count) ? currentRegister : name;

    return desc[name].size;
}


static TypeInput TypeBuffer(Register::E name)
{
    name = (name == Register::Count) ? currentRegister : name;

    return desc[name].type;
}


uint64 FirstValue()
{
    char buff[20];

    for (uint i = 0; i < sizeof(dbuffer); i++)
    {
        if (dbuffer[i] == '.')
        {
            for (uint j = 0; j < i; j++)
            {
                buff[j] = dbuffer[j];
            }
            buff[i] = 0;

            uint64 result = 0;

            if (SU::String2UInt64(buff, &result))
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
    for (uint i = 0; i < sizeof(dbuffer); i++)
    {
        if (dbuffer[i] == '.')
        {
            uint64 result = 0;

            if (SU::String2UInt64(&dbuffer[i + 1], &result))
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

    if (type == Uint)
    {
        if (!SU::String2UInt64(dbuffer, &result))
        {
            result = 0;
        }
    }
    else if (type == Binary)
    {
        result = SU::StringToBin32(dbuffer);
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


static bool AllowableSymbol(Key::E key)
{
    TypeInput type = TypeBuffer(currentRegister);

    if (type == Uint)
    {
        return Key(key).IsDigit();
    }
    else if (type == Binary)
    {
        return (key == Key::_0) || (key == Key::_1);
    }
    else // if (type == Uint10_Uint10 || type == Uint14_Uint14)
    {
        if (Key(key).IsDigit())
        {
            return true;
        }

        if (key == Key::Comma)
        {
            for (int i = 0; i < sizeof(dbuffer); i++)
            {
                if (dbuffer[i] == '.')
                {
                    return false;
                }
            }
            return true;
        }
    }

    return false;
}


void PageDebug::PageRegisters::Draw()
{
    if (CURRENT_PAGE != reinterpret_cast<Page *>(const_cast<PageBase *>(&pRegisters)))
    {
        return;
    }

    Rectangle(WaveGraphics::Width() - 1, WaveGraphics::Height() * 2).Fill(WaveGraphics::X(), WaveGraphics::Y(ChA) + 1, Color::BACK);

    DrawRegisters(WaveGraphics::X() + 4, WaveGraphics::Y(ChA) + 3);

    DrawInputWindow();
}


void PageDebug::PageRegisters::DrawRegisters(int x, int y)
{
    for (uint8 i = 0; i < Register::Count; i++)
    {
        Register reg(i);
        Color color = Color::FILL;
        if (i == currentRegister)
        {
            Rectangle(132, 8).Fill(x - 1, y + i * 10, Color::FILL);
            color = Color::BACK;
        }
        String(reg.Name()).Draw(x, y + i * 10, color);
        DrawValue(x + 135, y + i * 10, i);
    }
}


void PageDebug::PageRegisters::DrawInputWindow()
{
    if (!showInputWindow)
    {
        return;
    }

    Rectangle(WIDTH_INPUT, HEIGHT_INPUT).Draw(X_INPUT, Y_INPUT, Color::FILL);
    Rectangle(WIDTH_INPUT - 2, HEIGHT_INPUT - 2).Fill(X_INPUT + 1, Y_INPUT + 1, Color::BACK);
    Register reg(currentRegister);
    BigText(reg.Name(), 2).Draw(X_INPUT + 3, Y_INPUT + 2, Color::FILL);

    int x = X_INPUT + 5;

    int position = NumberBuffer::PositionCursor();

    int size = static_cast<int>(std::strlen(dbuffer));

    for (int i = 0; i < size; i++)
    {
        if (i == position)
        {
            Rectangle(19, 31).DrawFilled(x - 2, Y_INPUT + 19, Color::GRAY_10, Color::BLUE);
            Color::FILL.SetAsCurrent();
        }
        x = BigChar(dbuffer[i], 4).Draw(x, Y_INPUT + 20) + 3;
    }
    if (position == static_cast<int>(std::strlen(dbuffer)) && position < SizeBuffer())
    {
        Rectangle(19, 31).DrawFilled(x - 2, Y_INPUT + 19, Color::GRAY_10, Color::BLUE);
    }
}


static void DrawValue(int x, int y, uint8 i)
{
    if (!SENDING(i))
    {
        return;
    }

    Color::FILL.SetAsCurrent();

    Register name(i);

    TypeInput type = TypeBuffer(name);

    if (type == Uint)
    {
        String(SU::UInt64_2String(VALUE(i))).Draw(x, y);
    }
    else if (type == Binary)
    {
        char buf[33];

        String(SU::Bin2StringN((uint)VALUE(i), buf, SizeBuffer(name))).Draw(x, y);
    }
    else // if (type == Uint10_Uint10 || type == Uint14_Uint14)
    {
        uint mask = type == Uint10_Uint10 ? 0x3ffU : 0x3fffU;
        int numBits = type == Uint10_Uint10 ? 10 : 14;

        uint first = VALUE(i) & mask;
        uint second = (VALUE(i) >> numBits) & mask;
        x = String(SU::UInt2String(first)).Draw(x, y);
        x = String(".").Draw(x, y);
        String(SU::UInt2String(second)).Draw(x, y);
    }
}


static void OnPress_DebugMode(bool)
{
    PGenerator::SetDebugMode(set.dbg.modeEnabled != 0);
}

DEF_CHOICE_2(cConsole,                                                                                                                                           //--- ОТЛАДКА - КОНСОЛЬ ---
    "КОНСОЛЬ", "CONSOLE",
    "Включает и выключает отображение отладочной консоли", "Turns the debug console display on or off",
    DISABLED_RU, DISABLED_EN,
    "Отображение консоли выключено", "Console display off",
    ENABLED_RU, ENABLED_EN,
    "Отображение консоли включено", "Console Display Enabled",
    set.dbg.showConsole, pDebug, Item::FuncActive, OnPress_DebugMode, FuncDraw
)


static bool saveSreenEnabled = false;


bool PageDebug::SaveScreenToFlashIsEnabled()
{
    return saveSreenEnabled;
}


DEF_CHOICE_2( cSaveScreen,
    "Сохранение", "Saving",
    "При включенной опции и подключенной флешке при нажатии на ручку будет происходить сохранение экрана",
    "When the option is enabled and a USB flash drive is connected, pressing the handle will save the screen",
    DISABLED_RU, DISABLED_EN,
    "Ручка выполняет стандартную функцию", "The handle performs a standard function",
    ENABLED_RU, ENABLED_EN,
    "Нажатие на ручку сохраняет содержимое экрана на флешку", "Pressing the pen saves the contents of the screen to a USB flash drive",
    saveSreenEnabled, pDebug, Item::FuncActive, FuncChangedChoice, FuncDraw
)


static void OnPress_SaveSettings()
{
}

DEF_BUTTON( bSaveSettings,                                                                                                                            //--- ОТЛАДКА - СОХРАНИТЬ НАСТРОЙКИ ---
    "СОХРАНИТЬ НАСТРОЙКИ", "SAVE SETTINGS",
    "Сохранить текущие настройки", "Save current settings",
    pDebug, Item::FuncActive, OnPress_SaveSettings, FuncDraw
)


DEF_CHOICE_2( cStatistics,                                                                                                                                     //--- ОТЛАДКА - Статистика ---
    "СТАТИСТИКА", "STATISTICS",
    "Включает/отключает показ на экране статистики по частоте кадров в виде ФПС / время отрисовки одного кадра / время, потраченое на отрисовку за весь кадр",
    "Enables / disables the display on the screen of statistics on the frame rate in the form of FPS / time for rendering one frame / time spent on rendering for the entire frame",
    DISABLED_RU,                 DISABLED_EN,
    "Показ статистики отключён", "Show statistics disabled",
    ENABLED_RU,                  ENABLED_EN,
    "Показ статистики включен",  "Show statistics enabled",
    set.dbg.showStatistics, pDebug, Item::FuncActive, FuncChangedChoice, FuncDraw
)


DEF_CHOICE_2( cShowSends,                                                                                                                            //--- ОТЛАДКА - Показывать параметры ---
    "Показывать параметры", "Show options",
    "Показывает параметры, засылаемые в ПЛИС", "Shows parameters sent to FPGA",
    DISABLED_RU,                 DISABLED_EN,
    "Показ параметров отключён", "Parameter display disabled",
    ENABLED_RU,                  ENABLED_EN,
    "Показ параметров включён",  "Parameter display enabled",
    set.dbg.showSends, pDebug, Item::FuncActive, FuncChangedChoice, FuncDraw
)


static void OnPress_Prev()
{
    Math::CircleDecrease<uint8>(reinterpret_cast<uint8 *>(&currentRegister), 0, Register::Count - 1);
}

DEF_BUTTON( bPrev,                                                                                                                                            //--- РЕГИСТРЫ - Предыдущий ---
    "Предыдущий", "Previous",
    "Переход к предыдущему регистру", "Go to previous register",
    pRegisters, Item::FuncActive, OnPress_Prev, FuncDraw
)


static void OnPress_Next()
{
    Math::CircleIncrease<uint8>(reinterpret_cast<uint8 *>(&currentRegister), 0, Register::Count - 1);
}

DEF_BUTTON( bNext,                                                                                                                                             //--- РЕГИСТРЫ - Следующий ---
    "Следующий", "Next",
    "Переход к следующему регистру", "Go to the next register",
    pRegisters, Item::FuncActive, OnPress_Next, FuncDraw
)


static void OnPress_DebugCancel()
{
    showInputWindow = false;
    std::memset(dbuffer, 0, MAX_SIZE_BUFFER + 1);
    pRegisters.items[0] = reinterpret_cast<Item*>(const_cast<ButtonBase*>(&bPrev));
    pRegisters.items[1] = reinterpret_cast<Item*>(const_cast<ButtonBase*>(&bNext));
    pRegisters.items[2] = reinterpret_cast<Item*>(const_cast<ButtonBase*>(&bSend));
}

static void OnDraw_Cancel(int x, int y)
{
    Char(Ideograph::_8::Delete).Draw4InRect(x + 15, y + 30, Color::FILL);
}

DEF_BUTTON( bCancel,                                                                                                                               //--- РЕГИСТРЫ - Input window - Отмена ---
    "Отмена", "Cancel",
    "Отменяет засылку значения в регистр и закрывает окно ввода", "Cancels registering a value and closes the input window",
    pRegisters, Item::FuncActive, OnPress_DebugCancel, OnDraw_Cancel
)


static void OnDraw_Backspace(int x, int y)
{
    Char(Ideograph::_8::Left).Draw4InRect(x + 15, y + 30, Color::FILL);
}

DEF_BUTTON( bBackspace,                                                                                                                         //--- РЕГИСТРЫ - Input window - Backspace ---
    "Backspace", "Backspace",
    "Удаляет последний введённый символ", "Deletes the last character entered",
    pRegisters, Item::FuncActive, NumberBuffer::PressBackspace, OnDraw_Backspace
)


static void OnPress_Save()
{
    LoadRegister();
    OnPress_DebugCancel();
}

static void OnDraw_Save(int x, int y)
{
    Char(Ideograph::_8::Save).Draw4InRect(x + 15, y + 30, Color::FILL);
}

DEF_BUTTON( bSave,                                                                                                                                //--- РЕГИСТРЫ - Окно ввода --- Заслать ---
    "Заслать", "Send",
    "Записывает значение в выбранный регистр", "Writes a value to the selected register",
    pRegisters, Item::FuncActive, OnPress_Save, OnDraw_Save
)


static void OnPress_Send()
{
    showInputWindow = true;
    std::memset(dbuffer, 0, MAX_SIZE_BUFFER + 1);

    pRegisters.items[0] = reinterpret_cast<Item*>(const_cast<ButtonBase*>(&bBackspace));
    pRegisters.items[1] = reinterpret_cast<Item*>(const_cast<ButtonBase*>(&bCancel));
    pRegisters.items[2] = reinterpret_cast<Item*>(const_cast<ButtonBase*>(&bSave));

    int position = 0;

    if (SENDING(currentRegister))
    {
        TypeInput type = TypeBuffer();

        if (type == Uint)
        {
            SU::UInt64_2String(VALUE(currentRegister), dbuffer);
            position = static_cast<int>(std::strlen(dbuffer));
        }
        else if (type == Binary)
        {
            SU::Bin2StringN((uint)VALUE(currentRegister), dbuffer, SizeBuffer(currentRegister));
            position = static_cast<int>(std::strlen(dbuffer));
        }
        else // if (type == Uint10_Uint10 || type == Uint14_Uint14)
        {
            uint mask = type == Uint10_Uint10 ? 0x3ffU : 0x3fffU;
            int numBits = type == Uint10_Uint10 ? 10 : 14;

            uint first = VALUE(currentRegister) & mask;
            uint second = (VALUE(currentRegister) >> numBits)& mask;

            std::strcpy(dbuffer, SU::UInt2String(first));
            std::strcat(dbuffer, ".");
            std::strcat(dbuffer, SU::UInt2String(second));
        }
    }
    else
    {
        position = 0;
        std::memset(dbuffer, 0, MAX_SIZE_BUFFER + 1);
        VALUE(position) = 0;
    }

    NumberBuffer::Set(dbuffer, desc[currentRegister].size, position, (currentRegister == Register::FreqMeterLevel ||
        currentRegister == Register::FreqMeterHYS) ? 4095 : 0);
}

DEF_BUTTON( bSend,                                                                                                                                               //--- РЕГИСТРЫ - Заслать ---
    "Заслать", "Send",
    "Открывает окно ввода значения регистра", "Opens a register value input window",
    pRegisters, Item::FuncActive, OnPress_Send, FuncDraw
)


DEF_PAGE_4_VAR( pRegisters,                                                                                                                                       //--- РЕГИСТРЫ --- //-V641
    "РЕГИСТРЫ", "REGISTERS",   //-V641
    "", "",
    &bPrev,
    &bNext,
    &bSend,
    &Item::emptyLight,
    Page::Registers, &pDebug, Item::FuncActive, Page::FuncEnter, OnKey_PageRegisters, FuncBeforeDraw
)


static void OnChange_FreqMeterK()
{
    Message::SetFreqMeterTrigK(setCal.kFreqMeter).Transmit();
    PFreqMeter::LoadLevel();
    setCal.Save();
}


DEF_GOVERNOR(gFreqMeterK,                                                                                                                                          //--- ЧАСТОТОМЕР - Уровень ---
    "Ур синхр", "Trig lev",
    "Калибровочный коэффициент уровня синхронизации частотомера", "Frequency meter calibration factor",
    setCal.kFreqMeter, -150, 150, pDebug, Item::FuncActive, OnChange_FreqMeterK, EmptyFuncVV, 0
)


DEF_PAGE_10( pDebug,                                                                                                                                                 //--- ОТЛАДКА --- //-V641
    "ОТЛАДКА", "DEBUG",   //-V641
    "", "",
    PageDebug::Calibartion::self,
    &pRegisters,                                // ОТЛАДКА - Регистры
    PageDebug::SubRange::self,
    &gFreqMeterK,
    PageDebug::Colors::self,
    &cConsole,                                  // ОТЛАДКА - Консоль
    &cSaveScreen,
    &cStatistics,                               // ОТЛАДКА - Статистика
    &cShowSends,                                // ОТЛАДКА - Показывать параметры
    &bSaveSettings,                             // ОТЛАДКА - Сохранить настройки
    Page::Debug, PageService::self, Item::FuncActive, Page::FuncEnter, FuncOnKey, Page::FuncDraw
)



/*
static void OnPress_BigSymbols(bool)
{
    Font::ToggleCharacterSize();
}

DEF_CHOICE_2_OLD (  cBigSymbols,                                                                                                                              //--- ОТЛАДКА - Большие символы ---
    "Большие символы", "Large characters",
    "Отображение информации более крупными символами",
    "Display information with larger characters",
    DISABLED_RU, DISABLED_EN,   "Отображение информации стандартными символами",
                                "Displaying information with standard symbols",
    ENABLED_RU, ENABLED_EN,     "Отображение информации увеличенными символами",
                                "Displaying information with enlarged symbols",
    FLAG, BIT_BIG_SYMBOLS, pDebug, FuncActive, OnPress_BigSymbols, FuncDraw
)
*/
