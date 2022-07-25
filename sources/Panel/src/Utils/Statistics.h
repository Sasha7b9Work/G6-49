// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class Statistics
{
public:
    static void BeginFrame();

    static void EndFrame();

    static void Show();

private:
    // ����� ������ �����
    static uint timeStart;
    // �����, ����������� �� ��������� ������ �����
    static int timeFrame;
    // ����� ����� ����������� ����� ����� ���������
    static uint timeAccumFrames;
    // ����� ��������� ���� ������ �� 1 �������
    static int timeAllFrames;
    // ����� ������ ���������� 1-���������� �������, � ������� �������� ���������� ����� ����� ���������
    static uint timeStartFrames;
};
