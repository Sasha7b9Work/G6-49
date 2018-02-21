#pragma once
#include "defines.h"
#include "Keyboard/Controls.h"
#include "Menu/MenuItems.h"
#include "Settings/SettingsTypes.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define NUM_DIGITS          5


enum Order
{
    Nano,       // нано
    Micro,      // микро
    Milli,      // милли
    One,        // единицы
    Kilo,       // кило
    Mega,       // мега
    NumOrders
};

Order& operator++(Order& order);
Order& operator--(Order& order);

enum SignValue
{
    Sign_None,
    Sign_Minus,
    Sign_Plus
};

class InputWindowStruct
{
public:

    void Fill(Channel ch, WaveForm form, WaveParameter param);

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
    static void FillAllowParameters(Channel ch, WaveForm form, AllowableParameters *allowParameters);

    bool            allow;                      ///< Если true, параметр разрешен для данного сигнала
    SignValue       sign;
	char            inputBuffer[NUM_DIGITS + 1];
    char            prevBuffer[NUM_DIGITS + 1];
    uint8           notUsed0[2];
    int             posComma;                   ///< После какого знакоместа выводить запятую
    int             prevPosComma;
	int	            hightLightDigit;            ///< Позиция подсвеченного знакоместа
	Order           order;
    WaveParameter   param;
    bool            numLockMode;                ///< true, когда происходит клавишный (0...9) ввод значения
    uint8           notUsed1;
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const char *NameOrder(Order order);
const char *NameUnit(char buffer[10], Order order, WaveParameter parameter);

