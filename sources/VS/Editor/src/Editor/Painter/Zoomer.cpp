#include "defines.h"
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


// Точки, отображаемые в сетке
struct Window
{
    static void Draw();

private:

    static void DrawText(int x, int y, int width);
};


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
    float dX = static_cast<float>(Zoomer::IndexFirsPoint()) / Point::AMOUNT * static_cast<float>(Grid::Width());

    float width = static_cast<float>(Zoomer::NumberDrawingPoints()) / Point::AMOUNT * static_cast<float>(Grid::Width());

    int x = Zoomer::X() + static_cast<int>(dX + 0.5F);

    int y = Zoomer::Y();

    Painter::FillRegion(x, y, static_cast<int>(width + 0.5F), Zoomer::Height(), Color::GRAY_3F);
    Painter::DrawRectangle(x, y, static_cast<int>(width + 0.5F), Zoomer::Height(), Color::WHITE);

    DrawText(x, y + 1, static_cast<int>(width + 0.5F));
}


void Window::DrawText(int x, int y, int width)
{
    wxString text = wxString::Format(wxT("%d%%"), Zoomer::Scale());

    int length = Painter::GetLengthText(text);

    if (length < width)
    {
        Painter::DrawTextInZone(x, y, width, text);
    }
    else if(width + length < Grid::Bottom())
    {
        Painter::DrawText(x + width + 5, y, text);
    }
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


int Zoomer::NumberDrawingPoints()
{
    return numberDrawingPoints;
}


bool Zoomer::UnderMouse(int mouseX, int mouseY)
{
    return (mouseX >= X()) && (mouseX <= Right()) && (mouseY >= Y()) && (mouseY <= Bottom());
}
