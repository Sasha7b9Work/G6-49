// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct StructForReadDir;


struct DLDrive
{
    static void Init();

    static void DeInit();

    static void Update();

    static bool IsConnected();

    static void *handleUSBH;

    struct File
    {
        // Открывает файл для чтения. Возвращает размер файла или -1, если файл не существует
        static int Open(const char *name);

        static void Seek(int pos);

        static int Read(int size, uint8 *buffer);

        static void Close();
    };
};
