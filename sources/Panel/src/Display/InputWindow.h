#pragma once
#include "Hardware/Controls.h"
#include "Command.h"
#include "Menu/MenuItems.h"
#include <string.h>


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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const char *NameUnit(char buffer[10], Order order, Form::Parameter parameter);


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
    static Chan                     chan;
    /// Форма сигнала, для которого вводится значение
    static Form               *form;
    /// Параметт, значение которого вводится
    static Form::Parameter    *m_param;

public:
    class Struct
    {
    public:

        void Fill(Chan ch, Form *form, Form::Parameter *param);

        char *StringValue();

        float Value();

        void DrawInputField(int x, int y);

        void KeyLeft();

        void KeyRight();

        void RegLeft();

        void RegRight();

        void PressKey(Control key);

        void SaveValue();

        static Form        form;
        static Form::Parameter   m_param;
        static Chan              ch;

        Struct(char s, char *buff = "10000", int8 comma = 0, int8 hd = NUM_DIGITS - 1, uint8 o = Order::Kilo, uint8 p = Form::Parameter::Frequency, 
               uint a = 0, uint numlm = 0, float _min = 0.1f, float _max = 100e6f) :
            sign(s), posComma(comma), hightLightDigit(hd), order(o), param(p), allow(a), numLockMode(numlm), min(_min), max(_max)
        {
            strcpy(buffer, buff);
        }
        char     sign;
        char     buffer[NUM_DIGITS + 1];    ///< В этом буфере хранится знак числа, а затем цифры с точкой
        int8     posComma;            ///< После какого знакоместа выводить запятую
        int8     hightLightDigit;     ///< Позиция подсвеченного знакоместа
        uint8    order       : 3;
        uint8    param       : 4;     ///< Перечисление типа Form::Parameter
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
