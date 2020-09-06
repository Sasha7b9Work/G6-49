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
    numberDrawingPoints = 100.0F / scale * Point::AMOUNT;

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

    DrawWindow();
}


void Zoomer::DrawWindow()
{
    float dX = static_cast<float>(IndexFirsPoint()) / Point::AMOUNT * static_cast<float>(Grid::Width());

    float width = static_cast<float>(NumberDrawingPoints()) / Point::AMOUNT * static_cast<float>(Grid::Width());

    Painter::DrawRectangle(X() + static_cast<int>(dX + 0.5F), Y(), static_cast<int>(width + 0.5F), Height(), Color::WHITE);
}


int Zoomer::X()
{
    return 0;
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
    return 20;
}


int Zoomer::IndexFirsPoint()
{
    return indexFirstPoint;
}


int Zoomer::NumberDrawingPoints()
{
    return numberDrawingPoints;
}
