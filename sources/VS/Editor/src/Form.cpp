#include "defines.h"
#include "Canvas.h"
#include "Form.h"
#include "MyMath.h"
#include <vector>


using namespace MyMath;


Form *TheForm = nullptr;

#define NUM_POINTS  (8192)
#define MAX_VALUE   ((1 << 12) - 1)
#define AVE_VALUE   ((MAX_VALUE) / 2)

#define SIZE_POINT  (5)

// Данные, готовые для передачи в прибор
uint16 data[NUM_POINTS];

struct Point
{
//    Point(uint16 p, uint16 d) : pos(p), data(d) {};
    Point(int mouseX, int mouseY)
    {
        pos = Round<uint16>(mouseX / ScaleX());

        data = Round<uint16>(mouseY / ScaleY());
    }
    uint16 pos;
    uint16 data;
    bool operator < (const Point &point) const
    {
        return (pos < point.pos);
    }
    /// Возвращает true, если курсор мыши находится над этой точкой
    bool UnderMouse(int mouseX, int mouseY)
    {
        return (Abs(mouseX - static_cast<int>(pos)) <= SIZE_POINT * 5) && (Abs(mouseY - static_cast<int>(data)) <= SIZE_POINT * 5);
    }

    int CanvasX()
    {
        return Round<int>(pos * ScaleX());
    }

    int CanvasY()
    {
        return Round<int>(data * ScaleY());
    }
    /// Масштаб по горизонтали
    static float ScaleX()
    {
        return TheCanvas->GetSize().x / static_cast<float>(NUM_POINTS);
    }
    /// Масштаб по вертикали
    static float ScaleY()
    {
        return TheCanvas->GetSize().y / static_cast<float>(MAX_VALUE);
    }
};


bool operator==(const Point &left, const Point &right)
{
    return (left.pos == right.pos) && (left.data == right.data);
}


std::vector<Point> points;


/// Здесь хранится индекс точки, подлежащей удалению
uint indexRemovedPoint = 0;


/// Рассчитать соседние с point точки
static void CalculateNeighboringPoints(const Point &point);
/// Линейно интерполировать точки, расположенные между pos1 и pos2
static void LinearInterpolation(uint16 pos1, uint16 pos2);
/// Интерполировать точки слева от точки с индексом index из points
static void LinearInterpolationLeft(uint index);
/// Интерполировать точки справа от точки с индексом index из points
static void LinearInterpolationRight(uint index);

Form::Form()
{
    for (int i = 0; i < NUM_POINTS; i++)
    {
        data[i] = AVE_VALUE;
    }

    points.clear();

    wxSize size = TheCanvas->GetSize();

    points.push_back(Point(0, AVE_VALUE));
    points.push_back(Point(size.x - 1, AVE_VALUE));
}


void Form::SetPoint(int mouseX, int mouseY)
{
    Point point(mouseX, mouseY);

    data[point.pos] = point.data;

    points.push_back(point);

    std::sort(points.begin(), points.end());

    CalculateNeighboringPoints(point);
}


void Form::RemovePoint()
{
    if (indexRemovedPoint != 0 && indexRemovedPoint != points.size() - 1)
    {
        points.erase(points.begin() + static_cast<const int>(indexRemovedPoint));

        LinearInterpolationLeft(indexRemovedPoint);
    }
}


void Form::MovePoint(int mouseX, int mouseY)
{
    //if (indexRemovedPoint == 0)                         // Для первой точки. Её позиция всегда 0
    //{
    //
    //}
    //else if (indexRemovedPoint == points.size() - 1)    // Для последнией точки. Её позиция всегда NUM_POINTS - 1
    //{
    //
    //}
    //else
    //{
    ////    Point point()
    //}

    RemovePoint();
    SetPoint(mouseX, mouseY);
}


bool Form::ExistPoint(int mouseX, int mouseY)
{
    float scaleX = Point::ScaleX();
    float scaleY = Point::ScaleY();

    for (uint i = 0; i < points.size(); i++)
    {
        if (points[i].UnderMouse(Round<int>(mouseX / scaleX), Round<int>(mouseY / scaleY)))
        {
            indexRemovedPoint = i;
            return true;
        }
    }

    return false;
}


void CalculateNeighboringPoints(const Point &point)
{
    uint index = 0;                     // Здесь будет индекс point в векторе points

    for (; index < points.size(); index++)
    {
        if (point == points[index])
        {
            break;
        }
    }

    LinearInterpolationLeft(index);

    LinearInterpolationRight(index);
}


void Form::Draw()
{
    TheCanvas->SetColor(Color::WHITE);

    float scaleX = Point::ScaleX();
    float scaleY = Point::ScaleY();

    for (int i = 1; i < NUM_POINTS; i++)
    {
        int x0 = Round<int>(scaleX * (i - 1));
        int y0 = Round<int>(scaleY * data[i - 1]);

        int x1 = Round<int>(scaleX * i);
        int y1 = Round<int>(scaleY * data[i]);

        TheCanvas->DrawLine(x0, y0, x1, y1);
    }

    TheCanvas->SetColor(Color::GREEN);

    for (Point point : points)
    {
        int x = Round<int>(scaleX * point.pos);
        int y = Round<int>(scaleY * point.data);

        TheCanvas->SetPoint(x, y, SIZE_POINT);
    }
}


static void LinearInterpolation(uint16 pos1, uint16 pos2)
{
    uint16 data1 = data[pos1];
    uint16 data2 = data[pos2];

    float delta = static_cast<float>(data2 - data1) / (pos2 - pos1);  // Разность значений между соседними точками
    
    float value = data[pos1] + delta;                                  // Значение в текущей позиции
    
    for (int i = pos1 + 1; i < pos2; i++)
    {
        data[i] = Round<uint16>(value);
        value += delta;
    }
}


static void LinearInterpolationLeft(uint index)
{
    Point point = points[index];

    if (index == 0)                     // Если точка самая первая
    {
        if (point.pos > 0)
        {
            LinearInterpolation(0, point.pos);
        }
    }
    else
    {
        LinearInterpolation(points[index - 1].pos, point.pos);
    }

}


static void LinearInterpolationRight(uint index)
{
    Point point = points[index];

    if (index == points.size() - 1)
    {
        if (point.pos < NUM_POINTS - 1)
        {
            LinearInterpolation(point.pos, NUM_POINTS - 1);
        }
    }
    else
    {
        LinearInterpolation(point.pos, points[index + 1].pos);
    }
}
