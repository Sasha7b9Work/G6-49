#pragma once


class SimpleMessage;


class Updater
{
public:

    static const uint MAIN_PROGRAM_START_ADDRESS = 0x8020000;

    // true, если нужно запускать процедуру обновления
    static bool NeedUpgrade();

    // Запуск процедуры обновления
    static void UpgradeDevice();
    static void UpgradePanel();

    // Обработчик поступающих команд
    static void Handler(SimpleMessage *);
};
