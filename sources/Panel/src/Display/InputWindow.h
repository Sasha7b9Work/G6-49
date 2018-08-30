#pragma once
#include "Hardware/Controls.h"
#include "Command.h"
#include "Menu/MenuItems.h"


#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4514)
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define NUM_DIGITS          5


struct Order
{
    enum E
    {
        Nano,       // нано
        Micro,      // микро
        Milli,      // милли
        One,        // единицы
        Kilo,       // кило
        Mega,       // мега
        Number
    } value;
    Order(E v) : value(v)  { };
    operator uint8() const { return (uint8)value; };
    pString Name() const;
};

struct SignValue
{
    enum E
    {
        None,
        Minus,
        Plus
    } value;
    SignValue(E v) : value(v) { };
    operator uint8() const    { return (uint8)value; };
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const char *NameUnit(char buffer[10], Order order, Wave::Parameter parameter);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Окно редактирования численного значения параметра
class InputWindow
{
public:
    static void Init();

    static void Draw();

    static void KeyLeft();

    static void KeyRight();

    static void KeyEnter();

    static void ProcessContorl(Control control);

    /// Заполнить структуру allowParameters значениями в соответствии с допустимыми для данных формы сигнала и канала
    static void FillAllowParameters(Chan ch, Wave::Form form, AllowableParameters *allowParameters);

private:
    static void DrawAmplitude(int x, int y);

    static void DrawFrequency(int x, int y);

    static void DrawOffset(int x, int y);

    static void DrawPhase(int x, int y);

    static void DrawDelay(int x, int y);

    static void DrawPeriod(int x, int y);

    static void DrawDutyRatio(int x, int y);

    static void DrawDuration(int x, int y);

    static void DrawDigits(int x, int y);

    static void DrawDepthModulation(int x, int y);

    static void DrawPolarity(int x, int y);

    static void DrawDurationRise(int x, int y);

    static void DrawDurationFall(int x, int y);

    static void DrawDurationStady(int x, int y);

    static void DrawDutyFactor(int x, int y);
    /// Канал, для которого вводится значение
    static Chan              chan;
    /// Форма сигнала, для которого вводится значение
    static Wave::Form        form;
    /// Параметт, значение которого вводится
    static Wave::Parameter   m_param;

public:
    class Struct
    {
    public:

        void Fill(Chan ch, Wave::Form form, Wave::Parameter param);

        char *StringValue();

        float Value();

        void DrawInputField(int x, int y);

        void KeyLeft();

        void KeyRight();

        void RegLeft();

        void RegRight();

        void PressKey(Control key);

        void SaveValue();

        static Wave::Form        form;
        static Wave::Parameter   m_param;
        static Chan              ch;
        /// Возвращает символ, размещённый в позиции pos - без учёта позиции знака
        char SymbolInPos(int pos);
        int8 PosComma();

        char     inputBuffer[NUM_DIGITS + 1];
        int8     posComma;            ///< После какого знакоместа выводить запятую
        int8     hightLightDigit;     ///< Позиция подсвеченного знакоместа
        uint8    order       : 3;
        uint8    param       : 4;     ///< Перечисление типа Wave::Parameter
        uint8    sign        : 2;     ///< Перечисление типа SignValue
        uint     allow       : 1;     ///< Если 1, параметр разрешен для данного сигнала
        uint     numLockMode : 1;     ///< 1, когда происходит клавишный (0...9) ввод значения
        float    min;                 ///< Минимальное значение параметра
        float    max;                 ///< Максимальное значение параметра

    private:
        /// Увеличивает значение в заданной позиции. Возвращает false, если итоговое значение не изменилось.
        bool IncreaseDigit(int num);
        /// Уменьшает значение в заданной позиции. Возвращает false, если итоговое значение не изменилось
        bool DecreaseDigit(int num);
        /// Возвращает true, если все цифры слева от num ранвы нулю. И num тоже
        bool All0LeftWithThis(int num);
        /// Возвращает true, елси все цифры слева и эта являются девятками
        bool All9LeftWithThis(int num);
        /// Возвращает true, если среди цифр единственная единица и она находится в даной позиции
        bool Only1InThis(int num);
        /// Сдвигает все разряды вправо
        void ShiftToRight();
        /// Сдвиг всех разрядов влево
        void ShiftToLeft();
        /// Возвращает true, если есть только одна крайняя справа цифра
        bool OnlyOneRigthDigit();
        /// Возвращает число до запятой
        int ValueBeforeComma();
        /// Возвращает число после запятой
        float ValueAfterComma();
        /// Переключает порядок на следующий по возрастанию
        void IncreaseOrder();
        /// Заполняет iws из inputBuffer
        void FillFromInputBuffer();
        /// Заслать текущее значение в генератор
        void SendToGenerator();
    };

    static Struct m_iws;
};


#ifdef WIN32
#pragma warning(pop)
#endif
