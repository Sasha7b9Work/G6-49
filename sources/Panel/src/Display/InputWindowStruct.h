#pragma once
#include "defines.h"
#include "Hardware/Controls.h"
#include "Menu/MenuItems.h"
#include "Settings/SettingsTypes.h"


#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4514)
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define NUM_DIGITS          5


struct Order
{
    enum
    {
        Nano,       // нано
        Micro,      // микро
        Milli,      // милли
        One,        // единицы
        Kilo,       // кило
        Mega,       // мега
        Number
    };
    uint8 value;
    Order(uint8 v) : value(v) {};
    operator uint8() const { return value; };
};

struct SignValue
{
    enum
    {
        None,
        Minus,
        Plus
    };
    uint8 value;
    SignValue(uint8 v) : value(v) {};
    operator uint8() const { return value; };
};

class InputWindowStruct
{
public:

    void Fill(Chan ch, WaveForm form, WaveParameter param);

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
    static void FillAllowParameters(Chan ch, WaveForm form, AllowableParameters *allowParameters);

	char          inputBuffer[NUM_DIGITS + 1];
    char          prevBuffer[NUM_DIGITS + 1];
    int8          posComma;                   ///< ѕосле какого знакоместа выводить зап€тую
    int8          prevPosComma;
	int8          hightLightDigit;            ///< ѕозици€ подсвеченного знакоместа
	uint8         order                 : 3;
    uint8         param                 : 4;  ///< ѕеречисление типа WaveParameter
    uint8         sign                  : 2;  ///< ѕеречисление типа SignValue
    uint          allow                 : 1;  ///< ≈сли 1, параметр разрешен дл€ данного сигнала
    uint          numLockMode           : 1;  ///< 1, когда происходит клавишный (0...9) ввод значени€
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const char *NameOrder(Order order);
const char *NameUnit(char buffer[10], Order order, WaveParameter parameter);


#ifdef WIN32
#pragma warning(pop)
#endif
