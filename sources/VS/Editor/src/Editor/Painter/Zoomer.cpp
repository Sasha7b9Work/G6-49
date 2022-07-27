// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Editor/Editor.h"
#include "Editor/Form.h"
#include "Editor/Painter/Canvas.h"
#include "Editor/Painter/Painter.h"
#include "Editor/Painter/Zoomer.h"
#include <vector>


int Zoomer::scale = 100;
int Zoomer::indexMiddlePoint = Point::AMOUNT / 2;
int Zoomer::indexFirstPoint = 0;
int Zoomer::numberDrawingPoints = Point::AMOUNT;


static const int SCALE_MIN = 100;
static const int SCALE_MAX = 13000;

static const std::vector<int> scales { SCALE_MIN, 150, 225, 350, 500, 800, 1100, 1700, 2600, 3800, 5800, 8600, SCALE_MAX };


// “очки, отображаемые в сетке
struct Window
{
    static void Draw();

    // ¬озвращает true, если мышь находитс€ под окном
    static bool UnderMouse(int mouseX, int mouseY);

    // ѕереместить в mouseX
    static bool MoveTo(int mouseX);

    // —только точек помещаетс€ в одном пикселе окна
    static double PointsInPixel();

    static int X();
    static int Y() { return Zoomer::Y(); }
    static int Width();
    static int Height();
    static int Right();
    static int Bottom();
};


struct Grabber
{
    static bool IsGrabbing() { return isGrabbing; }
    static void Grab(int mouseX, int mouseY);
    static bool UnGrab();
    static int CoordX()          { return grabX; }
    static int IndexFirstPoint() { return indexFirstPoint; }
private:
    static bool isGrabbing;     // true, если окно захвачено мышью
    static int grabX;           //  оордината X курсора мыши в момент захвата
    static int indexFirstPoint;
};


bool Grabber::isGrabbing = false;
int Grabber::grabX = -1;
int Grabber::indexFirstPoint = -1;


bool Grabber::UnGrab()
{
    if (isGrabbing)
    {
        isGrabbing = false;
        return true;
    }

    return false;
}


void Zoomer::Increase(int /*mouseX*/)
{
    for (uint i = 0; i < scales.size() - 1; i++)
    {
        if (scales[i] == scale)
        {
            scale = scales[i + 1];
            break;
        }
    }

    CalculateIndexesPoints();

    TheCanvas->Redraw();

    TheFrame->UpdateStatusBar();
}


void Zoomer::Decrease(int /*mouseX*/)
{
    for (uint i = scales.size() - 1; i > 0; i--)
    {
        if (scales[i] == scale)
        {
            scale = scales[i - 1];
            break;
        }
    }

    CalculateIndexesPoints();

    TheCanvas->Redraw();

    TheFrame->UpdateStatusBar();
}


void Zoomer::CalculateIndexesPoints()
{
    numberDrawingPoints = static_cast<int>(100.0F / static_cast<float>(scale) * Point::AMOUNT);

    indexFirstPoint = indexMiddlePoint - numberDrawingPoints / 2;

    indexFirstPoint = Math::LimitationBelow(indexFirstPoint, 0);
}


void Zoomer::SetOnRegion()
{

}


bool Zoomer::NoScaling()
{
    return (scale == 100);
}


int Zoomer::Scale()
{
    return scale;
}


float Zoomer::ScaleF()
{
    return static_cast<float>(scale) / 100.0F;
}


void Zoomer::Draw()
{
    if (NoScaling())
    {
        return;
    }

    Painter::DrawRectangle(X(), Y(), Width(), Height(), Color::WHITE);

    Window::Draw();
}


void Window::Draw()
{
    int x = X();
    int y = Y();
    int width = Width();
    int height = Height();

    Painter::FillRegion(x, y, width, height, Color::GRAY_3F);
    Painter::DrawRectangle(x, y, width, height, Color::WHITE);
}


int Window::X()
{
    float dX = static_cast<float>(Zoomer::IndexFirsPoint()) / Point::AMOUNT * static_cast<float>(Grid::Width());

    return Zoomer::X() + static_cast<int>(dX + 0.5F);
}


bool Window::MoveTo(int mouseX)
{
    if (Grabber::IsGrabbing())
    {
        int deltaPixels = mouseX - Grabber::CoordX();

        int deltaPoints = static_cast<int>(PointsInPixel() * deltaPixels);

        Zoomer::indexFirstPoint = Math::Limitation<int>(Grabber::IndexFirstPoint() + deltaPoints, 0, static_cast<int>(Point::AMOUNT - Zoomer::numberDrawingPoints));

        Zoomer::indexMiddlePoint = Zoomer::indexFirstPoint + Zoomer::numberDrawingPoints / 2;

        TheCanvas->Redraw();

        return true;
    }

    return false;
}


int Window::Right()
{
    return X() + Width();
}


int Window::Width()
{
    float width = static_cast<float>(Zoomer::NumberDrawingPoints()) / Point::AMOUNT * static_cast<float>(Grid::Width());

    return static_cast<int>(width + 0.5F);
}


int Window::Bottom()
{
    return Y() + Height();
}


int Window::Height()
{
    return Zoomer::Height();
}


int Zoomer::X()
{
    return 0;
}


int Zoomer::Right()
{
    return X() + Width();
}


int Zoomer::Y()
{
    return 0;
}


int Zoomer::Width()
{
    return Grid::Width();
}


int Zoomer::Height()
{
    return 18;
}


int Zoomer::Bottom()
{
    return Y() + Height();
}


int Zoomer::IndexFirsPoint()
{
    return indexFirstPoint;
}


int Zoomer::IndexLastPoint()
{
    return indexFirstPoint + numberDrawingPoints + 1;
}


int Zoomer::NumberDrawingPoints()
{
    return numberDrawingPoints;
}


bool Zoomer::UnderMouse(int mouseX, int mouseY)
{
    if (NoScaling())
    {
        return false;
    }

    return (mouseX >= X()) && (mouseX <= Right()) && (mouseY >= Y()) && (mouseY <= Bottom());
}


bool Zoomer::Grab(int mouseX, int mouseY)
{
    if (!UnderMouse(mouseX, mouseY))
    {
        return false;
    }

    Grabber::Grab(mouseX, mouseY);

    return true;
}


bool Zoomer::UnGrab()
{
    return Grabber::UnGrab();
}


bool Zoomer::MoveWindow(int mouseX)
{
    return Window::MoveTo(mouseX);
}


bool Window::UnderMouse(int mouseX, int mouseY)
{
    if (Zoomer::NoScaling())
    {
        return false;
    }

    return (mouseX > X()) && (mouseX < Right()) && (mouseY > Y()) && (mouseY < Bottom());
}


void Grabber::Grab(int mouseX, int mouseY)
{
    if (!Window::UnderMouse(mouseX, mouseY))
    {
        return;
    }

    grabX = mouseX;
    isGrabbing = true;
    indexFirstPoint = Zoomer::IndexFirsPoint();
}


double Window::PointsInPixel()
{
    return static_cast<double>(Point::AMOUNT) / Zoomer::Width();
}
