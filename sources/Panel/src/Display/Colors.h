#pragma once
#include "common/CommonTypes.h"


class Color
{
public:
    static Color BLACK;             // Чёрный
    static Color WHITE;             // Белый
    static Color GRAY_10;
    static Color GRAY_25;           // Серый 25%
    static Color GRAY_50;           // Серый 50%
    static Color GRAY_75;           // Серый 75%
    static Color BLUE;
    static Color BLUE_10;
    static Color BLUE_25;
    static Color BLUE_50;
    static Color BLUE_75;
    static Color GREEN;
    static Color GREEN_5;
    static Color GREEN_10;
    static Color GREEN_25;
    static Color GREEN_50;
    static Color GREEN_75;
    static Color RED;
    static Color RED_25;
    static Color RED_50;
    static Color RED_75;
    static Color CHAN_A;
    static Color CHAN_B;
    static Color MENU_ITEM_SHADE;
    
    static Color NUMBER;

    static Color FLASH_10;
    static Color FLASH_01;

    static Color MENU_ITEM;
    static Color MENU_TITLE;
    static Color MENU_ITEM_DARK;


    static Color FILL;
    static Color BACK;
    
    explicit Color(const uint8 val);
    Color(const Color &color);

    static Color MenuItem(bool shade);      // Цвет элемента меню.
    static Color MenuTitle(bool shade);     // Цвет заголовка страницы. inShade == true, если страница затенена
    static Color BorderMenu(bool shade);    // Цвет окантовки меню
    static Color LightShadingText();        // Светлый цвет в тени.
    static Color Contrast(Color color);     // Возвращает цвет, контрастный к color. Может быть белым или чёрным.
    static Color Channel(const Chan &);
    
    uint8 value;

    void SetAsCurrent() const;
    static Color GetCurrent();
    static uint8 CurrentValue();
    col_val GetRGB();

    Color& operator=(const Color &color);

private:

    static Color current;

    enum
    {
        COLOR_BLACK,
        COLOR_WHITE,
        COLOR_GRAY_10,
        COLOR_GRAY_25,
        COLOR_GRAY_50,
        COLOR_GRAY_75,
        COLOR_BLUE,
        COLOR_BLUE_10,
        COLOR_BLUE_25,
        COLOR_BLUE_50,
        COLOR_BLUE_75,
        COLOR_GREEN,
        COLOR_GREEN_5,
        COLOR_GREEN_10,
        COLOR_GREEN_25,
        COLOR_GREEN_50,
        COLOR_GREEN_75,
        COLOR_RED,
        COLOR_RED_25,
        COLOR_RED_50,
        COLOR_RED_75,
        COLOR_CHAN_A,
        COLOR_CHAN_B,
        COLOR_MENU_ITEM_SHADE,

        COLOR_NUMBER,

        COLOR_FLASH_10,
        COLOR_FLASH_01,
        COLOR_MENU_ITEM,
        COLOR_MENU_TITLE,
        COLOR_MENU_ITEM_DARK,
        COLOR_INVERSE
    };
};

bool operator!=(const Color &left, const Color &right);
bool operator==(const Color &left, const Color &right);
bool operator>(const Color &left, const Color &right);


class ColorType
{
public:
    float   red;
    float   green;
    float   blue;
    float   stepRed;
    float   stepGreen;
    float   stepBlue;
    float   brightness;
    Color   color;
    int8    currentField;
    bool    alreadyUsed;
    bool    notUsed;

    // Если forced == false, то инициализация происходит только при первом вызове функции
    void Init(bool forced);
    void SetBrightness(float brightness = -1.0F);
    void BrightnessChange(int delta);
    void ComponentChange(int delta);
private:
    void CalcSteps();
    void SetColor();
};

#define DEF_COLOR_TYPE(name, r, g, b, sR, sG, sB, bright, col) ColorType name = {r, g, b, sR, sG, sB, bright, col, 0, false, false};
#define COLOR_TYPE(r, g, b, sR, sG, sB, bright, col) {r, g, b, sR, sG, sB, bright, col, 0, false, false};


#define MAKE_COLOR(r, g, b) ((col_val)(static_cast<uint>(b) + (static_cast<uint>(g) << 8) + (static_cast<uint>(r) << 16)))
#define R_FROM_COLOR(color) (((col_val)(color) >> 16) & 0xff)
#define G_FROM_COLOR(color) (((col_val)(color) >> 8)  & 0xff)
#define B_FROM_COLOR(color) (((col_val)(color))       & 0xff)
