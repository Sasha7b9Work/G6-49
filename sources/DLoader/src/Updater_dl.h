// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct SimpleMessage;


namespace Updater
{
    static const int MAIN_PROGRAM_START_ADDRESS = 0x8020000;

    // true, если нужно запускать процедуру обновления
    bool NeedUpgrade();

    // Запуск процедуры обновления
    void UpgradeDevice();
    void UpgradePanel();

    // Обработчик поступающих команд
    void Handler(SimpleMessage *);
};
