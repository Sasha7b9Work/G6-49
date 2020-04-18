#pragma once


class Statistics
{
public:
    static void BeginFrame();

    static void EndFrame();

    static void Show();

private:
    // ¬рем€ начала кадра
    static uint timeStart;
    // ¬рем€, потраченное на отрисовку одного кадра
    static int timeFrame;
    // «десь будем накапливать общее врем€ отрисовки
    static uint timeAccumFrames;
    // ¬рем€ отрисовки всех кадров за 1 секунду
    static int timeAllFrames;
    // ¬рем€ начала очередного 1-секундного отрезка, в течение которого замер€етс€ общее врем€ отрисовки
    static uint timeStartFrames;
};
