#pragma once


/*
    ������������ �������� ���������
*/


struct Param;


class Viewer
{
public:

    Viewer(Param *param);

    // ���������� �������� �� ��������� � ����������� y. �������� ��������� ��������� � xName, �������� - � xValue, ������� ��������� - c xUnits
    void Draw(int y, int xName, int xValue, int xUnits, const Chan &);

    void DrawName(int x, int y, int width, const Chan &);

private:

    void DrawValue(int y, int xValue, int xUnits);

    void DrawChoice(int y, int xValue, int xUnits);

    void DrawComposite(int y, int xValue, int xUnits);

    void DrawButton(int y, int xValue, int xUnits);

    Param *param;
};
