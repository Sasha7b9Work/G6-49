#pragma once
#include "Utils/CommonFunctions.h"
#define FuncDraw            EmptyFuncVII
#define FuncOnKey           EmptyFuncBKey
#define FuncPress           EmptyFuncVV
#define EmptyPressPage      EmptyFuncVV
#define FuncChanged         EmptyFuncVV
#define FuncChangedChoice   EmptyFuncVB
#define FuncBeforeDraw      EmptyFuncVV
#define FuncRegSetPage      EmptyFuncVI



#define DEF_BUTTON(name, title, hint, keeper, funcActive, funcPress, funcDraw)                                                              \
static const ButtonBase name = { Item::Type::Button, 0, false, Page::NoPage, &keeper, funcActive, {title, hint},                            \
    funcPress, funcDraw };


#define DEF_TIME(name, titleRU, titleEN, hintRU, hintEN, keeper, funcActive, cur, h, mi, s, mo, d, y)                                       \
static const TimeBase name = { TypeItem::E::Time, 0, false, Page::NoPage, &keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},          \
    &cur, &h, &mi, &s, &mo, &d, &y };


#define DEF_GOVERNOR(name, title, hint, cell, min, max, keeper, funcActive, funcChanged, funcBeforeDraw, funcPress)                         \
static const GovernorBase name = { Item::Type::Governor, 0, false, Page::NoPage, &keeper, funcActive,                                       \
    {title, hint}, min, max, (int16 *)(&(cell)), funcChanged, funcBeforeDraw, funcPress};


#define DEF_GOVERNOR_COLOR(name, titleRU, titleEN, hintRU, hintEN, colorType, keeper)                                                       \
static const GovernorColorBase name = { TypeItem::E::GovernorColor, 0, false, Page::NoPage, &keeper, FuncActive,                            \
    {titleRU, titleEN, hintRU, hintEN}, &colorType, 0};


#define DEF_PAGE_SB(name, title, hint, item0, item1, item2, item3, item4, item5,                                                            \
    namePage, keeper, funcActive, funcPress, funcDraw, funcOnKey)                                                                           \
static Item * items##name[] = {                                                                                                             \
    (Item *)reinterpret_cast<const Item *>(item0),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item1),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item2),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item3),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item4),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item5)};                                                                                         \
const PageBase name = { Item::Type::Page, 6, true, namePage, keeper, funcActive, {title, hint},                                             \
    items##name, funcPress, funcDraw, funcOnKey};

#define DEF_PAGE_1(name, titleRU, titleEN, hintRU, hintEN, item1, namePage, keeper, funcActive, funcPress)                                  \
static Item * items##name[] = {(Item *)&item1};                                                                                             \
const PageBase name = { Item::Type::Page, 1, false, namePage, keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},                       \
    items##name, funcPress, Page::FuncDraw, FuncRegSetPage};

#define DEF_PAGE_2(name, titleRU, titleEN, hintRU, hintEN, item1, item2, namePage, keeper, funcActive, funcPress)                           \
static Item * items##name[] = {(Item *)&item1, (Item *)&item2};                                                                             \
const PageBase name = { Item::Type::Page, 2, false, namePage, keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},                       \
    items##name, funcPress, FuncDrawPage, FuncRegSetPage};

#define DEF_PAGE_3(name, title, hint, item1, item2, item3, namePage, keeper, funcActive, funcPress, funcOnKey)                              \
static Item * items##name[] = {                                                                                                             \
    (Item *)reinterpret_cast<const Item *>(&item1),                                                                                         \
    (Item *)reinterpret_cast<const Item *>(&item2),                                                                                         \
    (Item *)reinterpret_cast<const Item *>(&item3)};                                                                                        \
const PageBase name = { Item::Type::Page, 3, false, namePage, (const PageBase *)keeper, funcActive, {title, hint},                          \
    items##name, funcPress, Page::FuncDraw, funcOnKey};

#define DEF_PAGE_4(name, title, hint,                                                                                                       \
    item1, item2, item3, item4, namePage, keeper, funcActive, funcPress, funcOnKey, funcDraw)                                               \
static Item * items##name[] = {(Item *)reinterpret_cast<const Item *>(item1), (Item *)item2, (Item *)item3, (Item *)item4};                 \
const PageBase name = { Item::Type::Page, 4, false, namePage, (const PageBase *)keeper, funcActive, {title, hint},                          \
    items##name, funcPress, funcDraw, funcOnKey};

#define DEF_PAGE_4_VAR(name, title, hint, item1, item2, item3, item4, namePage, keeper, funcActive, funcPress, funcOnKey, funcDraw)         \
static Item * items##name[] = {                                                                                                             \
    (Item *)reinterpret_cast<const Item *>(&item1),                                                                                         \
    (Item *)reinterpret_cast<const Item *>(&item2),                                                                                         \
    (Item *)reinterpret_cast<const Item *>(&item3),                                                                                         \
    (Item *)reinterpret_cast<const Item *>(&item4)};                                                                                        \
const PageBase name = { Item::Type::Page, 4, false, namePage, keeper, funcActive, {title, hint},                                            \
    items##name, funcPress, funcDraw, funcOnKey};

#define DEF_PAGE_5(name, title, hint, item1, item2, item3, item4, item5, namePage, keeper, funcActive, funcPress, funcOnKey, funcDraw)      \
static Item * items##name[] = {                                                                                                             \
    (Item *)reinterpret_cast<const Item *>(item1),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item2),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item3),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item4),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item5)};                                                                                         \
const PageBase name = { Item::Type::Page, 5, false, namePage, (const PageBase *)keeper, funcActive, {title, hint},                          \
    items##name, funcPress, funcDraw, funcOnKey};

#define DEF_PAGE_6(name, title, hint, item1, item2, item3, item4, item5, item6, namePage, keeper, funcActive, funcPress, funcOnKey, funcDraw)      \
static Item * items##name[] = {                                                                                                             \
    (Item *)reinterpret_cast<const Item *>(item1),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item2),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item3),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item4),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item5),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item6)};                                                                                         \
const PageBase name = { Item::Type::Page, 6, false, namePage, (const PageBase *)keeper, funcActive, {title, hint},                          \
    items##name, funcPress, funcDraw, funcOnKey};

#define DEF_PAGE_7(name, title, hint,                                                                                                       \
    item1, item2, item3, item4, item5, item6, item7, namePage, keeper, funcActive, funcPress, funcOnKey, funcDraw)                          \
static Item * items##name[] = {                                                                                                             \
    (Item *)reinterpret_cast<const Item *>(item1),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item2),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item3),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item4),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item5),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item6),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item7)};                                                                                         \
const PageBase name = { Item::Type::Page, 7, false, namePage, (const PageBase *)keeper, funcActive, {title, hint},                          \
    items##name, funcPress, funcDraw, funcOnKey};

#define DEF_PAGE_8(name, title, hint,                                                                                                       \
    item1, item2, item3, item4, item5, item6, item7, item8, namePage, keeper, funcActive, funcPress, funcOnKey, funcDraw)                   \
static Item * items##name[] = {                                                                                                             \
    (Item *)reinterpret_cast<const Item *>(item1),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item2),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item3),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item4),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item5),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item6),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item7),                                                                                          \
    (Item *)reinterpret_cast<const Item *>(item8)};                                                                                         \
const PageBase name = { Item::Type::Page, 8, false, namePage, (const PageBase *)keeper, funcActive, {title, hint},                          \
    items##name, funcPress, funcDraw, funcOnKey};

#define DEF_PAGE_9(name, titleRU, titleEN, hintRU, hintEN,                                                                                  \
    item1, item2, item3, item4, item5, item6, item7, item8, item9, namePage, keeper, funcActive, funcPress, funcOnKey, funcDraw)            \
static Item * items##name[] = {                                                                                                             \
    (Item *)item1, (Item *)item2, (Item *)item3, (Item *)item4, (Item *)item5, (Item *)item6, (Item *)item7,                                \
    (Item *)item8, (Item *)item9};                                                                                                          \
const PageBase name = { Item::Type::Page, 9, false, namePage, (const PageBase *)keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},     \
    items##name, funcPress, funcDraw, funcOnKey};

#define DEF_PAGE_10(name, titleRU, titleEN, hintRU, hintEN,                                                                                 \
    item1, item2, item3, item4, item5, item6, item7, item8, item9, item10, namePage, keeper, funcActive, funcPress, funcOnKey)              \
static Item * items##name[] = {                                                                                                             \
    (Item *)&item1, (Item *)&item2, (Item *)&item3, (Item *)&item4, (Item *)&item5, (Item *)&item6, (Item *)&item7,                         \
    (Item *)&item8, (Item *)&item9, (Item *)&item10};                                                                                       \
const PageBase name = { Item::Type::Page, 10, false, namePage, (const PageBase *)keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},    \
    items##name, funcPress, FuncDrawPage, funcOnKey};

#define DEF_PAGE_11(name, titleRU, titleEN, hintRU, hintEN,                                                                                 \
    item1, item2, item3, item4, item5, item6, item7, item8, item9, item10, item11, namePage, keeper, funcActive, funcPress, funcOnKey)      \
static Item * items##name[] = {                                                                                                             \
    (Item *)&item1, (Item *)&item2, (Item *)&item3, (Item *)&item4, (Item *)&item5, (Item *)&item6, (Item *)&item7,                         \
    (Item *)&item8, (Item *)&item9, (Item *)&item10, (Item *)&item11};                                                                      \
const PageBase name = { Item::Type::Page, 11, false, namePage, (const PageBase *)keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},    \
    items##name, funcPress, FuncDrawPage, funcOnKey};

#define DEF_PAGE_11_GLOBAL(name, titleRU, titleEN, hintRU, hintEN,                                                                          \
    item1, item2, item3, item4, item5, item6, item7, item8, item9, item10, item11, namePage, keeper, funcActive, funcPress)                 \
static const Item * const  items##name[] = {(Item *)&item1, (Item *)&item2, (Item *)&item3, (Item *)&item4, (Item *)&item5,                 \
    (Item *)&item6, (Item *)&item7, (Item *)&item8, (Item *)&item9, (Item *)&item10, (Item *)&item11};                                      \
const PageBase name = { Item_Page, 11, false, namePage, keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},                             \
    items##name, funcPress, FuncDrawPage, FuncRegSetPage};

#define DEF_PAGE_12(name, titleRU, titleEN, hintRU, hintEN,                                                                                 \
    item1, item2, item3, item4, item5, item6, item7, item8, item9, item10, item11, item12,                                                  \
    namePage, keeper, funcActive, funcPress, funcOnKey)                                                                                     \
static Item * items##name[] = {                                                                                                             \
    (Item *)&item1, (Item *)&item2, (Item *)&item3, (Item *)&item4, (Item *)&item5, (Item *)&item6, (Item *)&item7,                         \
    (Item *)&item8, (Item *)&item9, (Item *)&item10, (Item *)&item11, (Item *)&item12};                                                     \
const PageBase name = { Item::Type::Page, 12, false, namePage, (const PageBase *)keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},    \
    items##name, funcPress, FuncDrawPage, funcOnKey};

#define DEF_PAGE_13(name, namePage, keeper, funcActive, funcPress, titleRU, titleEN, hintRU, hintEN,                                        \
    item1, item2, item3, item4, item5, item6, item7, item8, item9, item10, item11, item12, item13)                                          \
static const Item * const  items##name[] = {                                                                                                \
    (Item *)&item1, (Item *)&item2, (Item *)&item3, (Item *)&item4, (Item *)&item5, (Item *)&item6, (Item *)&item7,                         \
    (Item *)&item8, (Item *)&item9, (Item *)&item10, (Item *)&item11, (Item *)&item12, (Item *)&item13};                                    \
const PageBase name = { Item_Page, 13, false, namePage, keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},                             \
    items##name, funcPress, FuncDrawPage, FuncRegSetPage};

#define DEF_PAGE_15(name, namePage, keeper, funcActive, funcPress, titleRU, titleEN, hintRU, hintEN,                                        \
    item1, item2, item3, item4, item5, item6, item7, item8, item9, item10,                                                                  \
    item11, item12, item13, item14, item15)                                                                                                 \
static const Item * const  items##name[] = {(Item *)&item1, (Item *)&item2, (Item *)&item3, (Item *)&item4, (Item *)&item5,                 \
    (Item *)&item6, (Item *)&item7, (Item *)&item8, (Item *)&item9, (Item *)&item10, (Item *)&item11, (Item *)&item12,                      \
    (Item *)&item13, (Item *)&item14,                                                                                                       \
    (Item *)&item15};                                                                                                                       \
const PageBase name = { Item_Page, 15, false, namePage, keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},                             \
    items##name, funcPress, FuncDrawPage, FuncRegSetPage};



#define DEF_CHOICE_PARAMETER(name, title, hint, keeper, funcActive, funcPress, form)                                                        \
ChoiceParameterBase name = { Item::Type::ChoiceParameter, 0, false, Page::NoPage, &keeper, funcActive, {title, hint} ,                      \
    funcPress, form};



#define DEF_SMALL_BUTTON(name, title, hint, keeper, funcActive, funcPress, funcDraw)                                                        \
static const SButtonBase name = { Item::Type::SmallButton, 0, false, Page::NoPage, &keeper, funcActive, {title, hint},                      \
    funcPress, funcDraw, 0, 0};

#define DEF_SMALL_BUTTON_EXIT(name, keeper, funcActive, funcPress, funcDraw)                                                                \
static const SButtonBase name = { Item_SmallButton, 0, false, Page::NoPage, &keeper, funcActive, {"Выход", "Exit",                          \
    "Кнопка для выхода в предыдущее меню", "Button to return to the previous menu"}, funcPress, funcDraw, 0, 0};

#define DEF_SMALL_BUTTON_HINTS_2(name, titleRU, titleEN, hintRU, hintEN, keeper, funcActive, funcPress, funcDraw,                           \
    FuncDrawHint1, hintRu1, hintEn1, FuncDrawHint2, hintRu2, hintEn2)                                                                       \
static const StructHelpSmallButton hints##name[] = {{ FuncDrawHint1, hintRu1, hintEn1 }, { FuncDrawHint2, hintRu2, hintEn2 }};              \
static const SButtonBase name = { Item_SmallButton, 0, false, Page::NoPage, &keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},        \
    funcPress, funcDraw, hints##name, 2};

#define DEF_SMALL_BUTTON_HINTS_3(name, titleRU, titleEN, hintRU, hintEN, keeper, funcActive, funcPress, funcDraw,                           \
    FuncDrawHint1, hintRu1, hintEn1, FuncDrawHint2, hintRu2, hintEn2, FuncDrawHint3, hintRu3, hintEn3)                                      \
static const StructHelpSmallButton hints##name[] = {{ FuncDrawHint1, hintRu1, hintEn1 }, { FuncDrawHint2, hintRu2, hintEn2 },               \
    { FuncDrawHint3, hintRu3, hintEn3 } };                                                                                                  \
static const SButtonBase name = { Item_SmallButton, 0, false, Page::NoPage, &keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},        \
    funcPress, funcDraw, hints##name, 3};

#define DEF_SMALL_BUTTON_HINTS_5(name, titleRU, titleEN, hintRU, hintEN, keeper, funcActive, funcPress, funcDraw,                           \
    FuncDrawHint1, hintRu1, hintEn1, FuncDrawHint2, hintRu2, hintEn2, FuncDrawHint3, hintRu3, hintEn3,                                      \
    FuncDrawHint4, hintRu4, hintEn4, FuncDrawHint5, hintRu5, hintEn5)                                                                       \
static const StructHelpSmallButton hints##name[] = {{ FuncDrawHint1, hintRu1, hintEn1 }, { FuncDrawHint2, hintRu2, hintEn2 },               \
    { FuncDrawHint3, hintRu3, hintEn3 }, { FuncDrawHint4, hintRu4, hintEn4 }, { FuncDrawHint5, hintRu5, hintEn5 } };                        \
static const SButtonBase name = { Item_SmallButton, 0, false, Page::NoPage, &keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},        \
    funcPress, funcDraw, hints##name, 5};



#define DEF_CHOICE_2(name, title, hint, name1, hint1, name2, hint2, cell, numBit, keeper, funcActive, funcChanged, funcDraw)                        \
static pString hints##name[] = {name1, name2};                                                                                                      \
static pString hints##name##Ru[] = {hint1, hint2};                                                                                                  \
static const ChoiceBase name = { Item::Type::Choice, 2, true, (uint8)numBit, (const PageBase *)(&keeper), funcActive, {title, hint},                                    \
    (int8 *)&cell, hints##name, hints##name##Ru, funcChanged, funcDraw};

#define DEF_CHOICE_3(name, title, hint, name1, hint1, name2, hint2, name3, hint3, cell, keeper, funcActive, funcChanged, funcDraw)                  \
static pString hints##name[] = {name1, name2, name3};                                                                                               \
static pString hints##name##Ru[] = {hint1, hint2, hint3};                                                                                           \
static const ChoiceBase name = { Item::Type::Choice, 3, false, Page::NoPage, &keeper, funcActive, {title, hint},                                    \
    (int8 *)&cell, hints##name, hints##name##Ru, funcChanged, funcDraw};

#define DEF_CHOICE_4(name, titleRU, titleEN, hintRU, hintEN,                                                                                        \
    nameRu1, nameEn1, hintRu1, hintEn1, nameRu2, nameEn2, hintRu2, hintEn2, nameRu3, nameEn3, hintRu3, hintEn3, nameRu4, nameEn4, hintRu4, hintEn4, \
    cell, keeper, funcActive, funcChanged, funcDraw)                                                                                                \
static pString hints##name[] = {nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4};                                            \
static pString hints##name##Ru[] = {hintRu1, hintRu2, hintRu3, hintRu4};                                                                            \
static pString hints##name##En[] = {hintEn1, hintEn2, hintEn3, hintEn4};                                                                            \
static const ChoiceBase name = { Item::Type::Choice, 4, false, Page::NoPage, &keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},               \
    (int8 *)&cell, hints##name, hints##name##Ru, hints##name##En, funcChanged, funcDraw};

#define DEF_CHOICE_5(name, title, hint, name1, hint1, name2, hint2, name3, hint3, name4, hint4, name5, hint5,                                       \
    cell, keeper, funcActive, funcChanged, funcDraw)\
static pString hints##name[] = {name1, name2, name3, name4,                                                                                         \
    name5};                                                                                                                                         \
static pString hints##name##Ru[] = {hint1, hint2, hint3, hint4, hint5};                                                                             \
static const ChoiceBase name = { Item::Type::Choice, 5, false, Page::NoPage, &keeper, funcActive, {title, hint},                                    \
    (int8 *)&cell, hints##name, hints##name##Ru, funcChanged, funcDraw};

#define DEF_CHOICE_6(name, titleRU, titleEN, hintRU, hintEN,                                                                                        \
    nameRu1, nameEn1, hintRu1, hintEn1, nameRu2, nameEn2, hintRu2, hintEn2, nameRu3, nameEn3, hintRu3, hintEn3, nameRu4, nameEn4, hintRu4, hintEn4, \
    nameRu5, nameEn5, hintRu5, hintEn5, nameRu6, nameEn6, hintRu6, hintEn6,                                                                         \
    cell, keeper, funcActive, funcChanged, funcDraw)                                                                                                \
static pString hints##name[] = {nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4,                                             \
    nameRu5, nameEn5, nameRu6, nameEn6};                                                                                                            \
static pString hints##name##Ru[] = {hintRu1, hintRu2, hintRu3, hintRu4, hintRu5, hintRu6};                                                          \
static pString hints##name##En[] = {hintEn1, hintEn2, hintEn3, hintEn4, hintEn5, hintEn6};                                                          \
static const ChoiceBase name = { Item::Type::Choice, 6, false, Page::NoPage, &keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},               \
    (int8 *)&cell, hints##name, hints##name##Ru, hints##name##En, funcChanged, funcDraw};

#define DEF_CHOICE_7(name, title, hint, name1, hint1, name2, hint2, name3, hint3, name4, hint4, name5, hint5, name6, hint6, name7, hint7,           \
    cell, keeper, funcActive, funcChanged, funcDraw)                                                                                                \
static pString hints##name[] = {name1, name2, name3, name4, name5, name6, name7};                                                                   \
static pString hints##name##Ru[] = {hint1, hint2, hint3, hint4, hint5, hint6, hint7};                                                               \
static const ChoiceBase name = { Item::Type::Choice, 7, false, Page::NoPage, &keeper, funcActive, {title, hint},                                    \
    (int8 *)&cell, hints##name, hints##name##Ru, funcChanged, funcDraw};

#define DEF_CHOICE_8(name, title, hint, name1, hint1, name2, hint2, name3, hint3, name4, hint4, name5, hint5, name6, hint6, name7, hint7,           \
    name8, hint8, cell, keeper, funcActive, funcChanged, funcDraw)                                                                                  \
static pString hints##name[] = {name1, name2, name3, name4, name5, name6, name7, name8};                                                            \
static pString hints##name##Ru[] = {hint1, hint2, hint3, hint4, hint5, hint6, hint7, hint8};                                                        \
static const ChoiceBase name = { Item::Type::Choice, 8, false, Page::NoPage, &keeper, funcActive, {title, hint},                                    \
    (int8 *)&cell, hints##name, hints##name##Ru, funcChanged, funcDraw};

#define DEF_CHOICE_12(name, titleRU, titleEN, hintRU, hintEN,                                                                                       \
    nameRu1,  nameEn1,  hintRu1,  hintEn1,  nameRu2,  nameEn2,  hintRu2,  hintEn2,  nameRu3,  nameEn3,  hintRu3,  hintEn3,                          \
    nameRu4,  nameEn4,  hintRu4,  hintEn4,  nameRu5,  nameEn5,  hintRu5,  hintEn5,  nameRu6,  nameEn6,  hintRu6,  hintEn6,                          \
    nameRu7,  nameEn7,  hintRu7,  hintEn7,  nameRu8,  nameEn8,  hintRu8,  hintEn8,  nameRu9,  nameEn9,  hintRu9,  hintEn9,                          \
    nameRu10, nameEn10, hintRu10, hintEn10, nameRu11, nameEn11, hintRu11, hintEn11, nameRu12, nameEn12, hintRu12, hintEn12,                         \
    cell, keeper, funcActive, funcChanged, funcDraw)                                                                                                \
static pString hints##name[] = {nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4,                                             \
    nameRu5, nameEn5, nameRu6,  nameEn6, nameRu7, nameEn7, nameRu8, nameEn8, nameRu9, nameEn9, nameRu10, nameEn10, nameRu11, nameEn11,              \
    nameRu12, nameEn12};                                                                                                                            \
static pString hints##name##Ru[] = {hintRu1, hintRu2, hintRu3, hintRu4, hintRu5, hintRu6, hintRu7, hintRu8, hintRu9, hintRu10, hintRu11, hintRu12}; \
static pString hints##name##En[] = {hintEn1, hintEn2, hintEn3, hintEn4, hintEn5, hintEn6, hintEn7, hintEn8, hintEn9, hintEn10, hintEn11, hintEn12}; \
static const ChoiceBase name = { Item::Type::Choice, 12, false, Page::NoPage, &keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},              \
    (int8 *)&cell, hints##name, hints##name##Ru, hints##name##En, funcChanged, funcDraw};


#define DEF_CHOICE_REG_9(name, titleRU, titleEN, hintRU, hintEN,                                                                                    \
    nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4, nameRu5, nameEn5,                                                       \
    nameRu6, nameEn6, nameRu7, nameEn7, nameRu8, nameEn8, nameRu9, nameEn9,                                                                         \
    cell, keeper, funcActive, funcChanged, funcDraw)                                                                                                \
static pString hints##name[] = {nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4,                                             \
    nameRu5, nameEn5, nameRu6, nameEn6, nameRu7, nameEn7, nameRu8, nameEn8, nameRu9, nameEn9};                                                      \
static const ChoiceBase name = { Item_ChoiceReg, 9, false, Page::NoPage, &keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},                   \
    (int8 *)&cell, hints##name, funcChanged, funcDraw};

#define DEF_CHOICE_REG_10(name, titleRU, titleEN, hintRU, hintEN,                                                                                   \
    nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4, nameRu5, nameEn5,                                                       \
    nameRu6, nameEn6, nameRu7, nameEn7, nameRu8, nameEn8, nameRu9, nameEn9, nameRu10, nameEn10,                                                     \
    cell, keeper, funcActive, funcChanged, funcDraw)                                                                                                \
static pString hints##name[] = {nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4,                                             \
    nameRu5, nameEn5, nameRu6, nameEn6, nameRu7, nameEn7, nameRu8, nameEn8, nameRu9, nameEn9, nameRu10, nameEn10};                                  \
static const ChoiceBase name = { Item_ChoiceReg, 10, false, Page::NoPage, &keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},                  \
    (int8 *)&cell, hints##name, funcChanged, funcDraw};
