#include "defines.h"
#include "Canvas.h"
#include "Form.h"
#include "MyMath.h"
#include <vector>


using namespace MyMath;


Form *TheForm = nullptr;

#define NUM_POINTS  (8192)
#define MAX_VALUE   static_cast<uint16>((1 << 12) - 1)
#define AVE_VALUE   static_cast<uint16>((MAX_VALUE) / 2)

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
    Point(uint16 p, uint16 d) : pos(p), data(d) {};
    void SetY(int mouseY)
    {
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


/// Здесь хранится индекс точки, которой управляем в текущий момент
uint iCurPoint = 0;


/// Рассчитать соседние с point точки
static void CalculateNeighboringPoints(const Point &point);
/// Линейно интерполировать точки, расположенные между pos1 и pos2
static void LinearInterpolation(uint16 pos1, uint16 pos2);
/// Интерполировать точки слева от точки с индексом index из points
static void LinearInterpolationLeft(uint index);
/// Интерполировать точки справа от точки с индексом index из points
static void LinearInterpolationRight(uint index);
/// Заносит точку куда следует
static void SetPoint(Point point);
/// Возвращает index точки в позиции pos. 0xFFFFFFFF, если точки в этой позиции нет
static uint PointInPosition(uint16 pos);

Form::Form()
{
    Clear();
}


void Form::Clear()
{
    for (int i = 0; i < NUM_POINTS; i++)
    {
        data[i] = AVE_VALUE;
    }

    points.clear();

    wxSize size = TheCanvas->GetSize();

    SetPoint(static_cast<uint16>(0), AVE_VALUE);
    SetPoint(static_cast<uint16>(NUM_POINTS - 1), AVE_VALUE);
}


static uint PointInPosition(uint16 pos)
{
    for (uint i = 0; i < points.size(); i++)
    {
        if (points[i].pos == pos)
        {
            return i;
        }
    }

    return static_cast<uint>(-1);
}


static void SetPoint(Point point)
{
    data[point.pos] = point.data;

    uint index = PointInPosition(point.pos);

    if (index != static_cast<uint>(-1))
    {
        points[index] = point;
    }
    else
    {
        points.push_back(point);

        std::sort(points.begin(), points.end());
    }

    CalculateNeighboringPoints(point);
}


void Form::SetPoint(int mouseX, int mouseY)
{
    ::SetPoint(Point(mouseX, mouseY));
}


void Form::SetPoint(uint16 pos, uint16 dat)
{
    ::SetPoint(Point(pos, dat));
}


void Form::RemovePoint()
{
    if (iCurPoint != 0 && iCurPoint != points.size())
    {
        points.erase(points.begin() + static_cast<const int>(iCurPoint));

        LinearInterpolationLeft(iCurPoint);

        iCurPoint = static_cast<uint>(-1);
    }
}


void Form::MovePoint(int mouseX, int mouseY)
{
    if (iCurPoint == 0)
    {
        points[0].SetY(mouseY);
        data[0] = points[0].data;
        LinearInterpolationRight(0);
    }
    else if (iCurPoint == points.size() - 1)
    {
        points[iCurPoint].SetY(mouseY);
        data[NUM_POINTS - 1] = points[iCurPoint].data;
        LinearInterpolationLeft(iCurPoint);
    }
    else
    {
        Point point(mouseX, mouseY);

        Point left = points[iCurPoint - 1];

        Point right = points[iCurPoint + 1];

        if (point.pos <= left.pos)
        {
            point.pos = static_cast<uint16>(left.pos + 1);
        }
        else if (point.pos >= right.pos)
        {
            point.pos = static_cast<uint16>(right.pos - 1);
        }

        uint tempIndex = iCurPoint;

        RemovePoint();

        iCurPoint = tempIndex;

        ::SetPoint(point);
    }
}


static bool CompareLess(Point left, Point right)
{
    return left.data < right.data;
}


static bool CompareMore(Point left, Point right)
{
    return left.data > right.data;
}


typedef bool (*pFuncCompare)(Point, Point);


void Form::AlignPoint(Align::E align)
{
    uint index = static_cast<uint>(-1);

    Point point = points[iCurPoint];

    if (align == Align::Left || align == Align::LeftTop || align == Align::LeftDown)
    {
        if (iCurPoint > 0)
        {
            if(align == Align::Left)
            {
                index = iCurPoint - 1;
            }
            else
            {
                uint i = iCurPoint - 1;

                pFuncCompare func = (align == Align::LeftTop) ? CompareLess : CompareMore;

                while (i < points.size())
                {
                    if(func(points[i], point))
                    {
                        index = i;
                        break;
                    }
                    i--;
                }
            }
        }
    }
    else
    {
        if (iCurPoint < points.size() - 1)
        {
            if (align == Align::Right)
            {
                index = iCurPoint + 1;
            }
            else
            {
                uint i = iCurPoint + 1;

                pFuncCompare func = (align == Align::RightTop) ? CompareMore : CompareLess;

                while (i < points.size() - 1)
                {
                    if (func(point, points[i]))
                    {
                        index = i;
                        break;
                    }
                    i++;
                }
            }
        }
    }

    if (index != static_cast<uint>(-1))
    {
        points[iCurPoint].data = points[index].data;
        ::SetPoint(points[iCurPoint]);
    }
}


bool Form::ExistPoint(int mouseX, int mouseY, bool pressed)
{
    float scaleX = Point::ScaleX();
    float scaleY = Point::ScaleY();

    for (uint i = 0; i < points.size(); i++)
    {
        if (points[i].UnderMouse(Round<int>(mouseX / scaleX), Round<int>(mouseY / scaleY)))
        {
            iCurPoint = i;
            return true;
        }
    }

    if (!pressed)
    {
        iCurPoint = static_cast<uint>(-1);
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

    if (iCurPoint != static_cast<uint>(-1))
    {
        TheCanvas->SetPoint(Round<int>(scaleX * points[iCurPoint].pos), Round<int>(scaleY * points[iCurPoint].data), SIZE_POINT * 3);
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
