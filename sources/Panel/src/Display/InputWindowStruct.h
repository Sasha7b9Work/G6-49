#pragma once
#include "defines.h"
#include "Hardware/Controls.h"
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
    /// «аполнить структуру allowParameters значени€ми в соответствии с допустимыми дл€ данных формы сигнала и канала
    static void FillAllowParameters(Channel ch, WaveForm form, AllowableParameters *allowParameters);

	char          inputBuffer[NUM_DIGITS + 1];
    char          prevBuffer[NUM_DIGITS + 1];
    int8          posComma;                   ///< ѕосле какого знакоместа выводить зап€тую
    int8          prevPosComma;
	int8          hightLightDigit;            ///< ѕозици€ подсвеченного знакоместа
	Order         order                 : 3;
    uint8         param                 : 4;  ///< ѕеречисление типа WaveParameter
    SignValue     sign                  : 2;  ///< ѕеречисление типа SignValue
    uint          allow                 : 1;  ///< ≈сли 1, параметр разрешен дл€ данного сигнала
    uint          numLockMode           : 1;  ///< 1, когда происходит клавишный (0...9) ввод значени€
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const char *NameOrder(Order order);
const char *NameUnit(char buffer[10], Order order, WaveParameter parameter);

