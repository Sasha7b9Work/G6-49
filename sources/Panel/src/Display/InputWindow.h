#pragma once
#include "InputWindowStruct.h"
#include "Hardware/Controls.h"


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

        static void DrawInputField(int x, int y);

        static void KeyLeft();

        static void KeyRight();

        static void RegLeft();

        static void RegRight();

        static void PressKey(Control key);

        static void SaveValue();
        /// Заполнить структуру allowParameters значениями в соответствии с допустимыми для данных формы сигнала и канала
        static void FillAllowParameters(Chan ch, Wave::Form form, AllowableParameters *allowParameters);

        char   inputBuffer[NUM_DIGITS + 1];
        char   prevBuffer[NUM_DIGITS + 1];
        int8   posComma;            ///< После какого знакоместа выводить запятую
        int8   prevPosComma;
        int8   hightLightDigit;     ///< Позиция подсвеченного знакоместа
        uint8  order : 3;
        uint8  param : 4;           ///< Перечисление типа Wave::Parameter
        uint8  sign : 2;            ///< Перечисление типа SignValue
        uint   allow : 1;           ///< Если 1, параметр разрешен для данного сигнала
        uint   numLockMode : 1;     ///< 1, когда происходит клавишный (0...9) ввод значения
    };

    static Struct m_iws;
};
