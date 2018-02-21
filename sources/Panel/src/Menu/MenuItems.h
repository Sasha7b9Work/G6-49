#pragma once
#include "defines.h"
#include "Keyboard/Controls.h"
#include "Settings/SettingsTypes.h"


#define MAX_CHOICE_VARIANTS 10  ///< Максимальное количество вариантов для Choice

/// Структура определяет допустимость параметра для определённой формы сигнала
struct AllowableParameters
{
    bool allow[NumParameters];
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef enum
{
    Item_None,
    Item_Choice,
    Item_Button,
    Item_Page,
    Item_SButton,
    Item_ChoiceParameter,
    Item_SB_Page
} Item;

#define COMMON_PART_ITEM            \
        Item       typeItem;        \
        uint8      notUsed0[3];     \
        pFuncVV    funcOnPress;     \
        const char *title[2];


/// Универсальный элементы выбора
struct Choice
{
    COMMON_PART_ITEM
    const char *names[MAX_CHOICE_VARIANTS * 2];
    uint8      *cell;
};

/// Элемент для выбора параметра сигнала
struct ChoiceParameter
{
    COMMON_PART_ITEM
    AllowableParameters     allowParameters;
    uint8                   *numParameter;
};

/// Кнопка
typedef struct
{
    COMMON_PART_ITEM
} Button;

/// Кнопка в режиме дополнительной страницы
struct SButton
{
    COMMON_PART_ITEM
    uint8       *cell;
    pFuncVII    funcDraw;
};

typedef struct
{
    COMMON_PART_ITEM
    void *items[NUM_ITEMS_ON_PAGE]; ///< Указатели на итемы, содержащиеся на странице
} Page;


const char* ItemTitle(void* item);

void* ItemFromPage(int numItem);

const char* Choice_CurrentName(Choice* choice);

char* ChoiceWaveParameter_CurrentName(ChoiceParameter *choice);

const char* Choice_Name(Choice* choice, int num);

const char* Page_Name(const Page* page);

bool ItemIsChoice(void* item);

bool ItemIsChoiceParameter(void *item);

bool ItemIsButton(void* item);

bool ItemIsSButton(void* item);

int Choice_NumChoices(Choice *choice);

Item TypeItem(void* address);
/// Возвращает порядковый номер пункта меню на странице
int PositionOnPage(void *item);
/// Возвращает функциональную клавишу, назначенную пункту меню
Control ButtonForItem(void *item);

int Choice_CurrentChoice(Choice *choice);
/// Возвращает адрес текущую страницу
Page* CurrentPage();

char* NameParameter(WaveParameter parameter);

char* NameWaveForm(WaveForm form);

