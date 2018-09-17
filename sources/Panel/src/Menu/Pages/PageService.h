#pragma once


class PageService
{
public:
    static Page *pointer;

    class PageRegisters
    {
    public:

        static void Draw();

    private:
        /// Написать регистры
        static void DrawRegisters(int x, int y);

        static void DrawInputWindow();
    };
};
