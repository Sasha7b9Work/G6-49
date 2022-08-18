// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct StructForReadDir;


namespace DLDrive
{
    void Init();

    void DeInit();

    void Update();

    bool IsConnected();

    extern void *handleUSBH;

    namespace File
    {
        // ��������� ���� ��� ������. ���������� ������ ����� ��� -1, ���� ���� �� ����������
        int Open(const char *name);

        void Seek(int pos);

        int Read(int size, uint8 *buffer);

        void Close();
    };
};
