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
        // ��������� ���� ��� ������. ���������� ������ ����� ��� -1, ���� ���� �� ����������
        static int Open(const char *name);

        static void Seek(int pos);

        static int Read(int size, uint8 *buffer);

        static void Close();
    };
};
