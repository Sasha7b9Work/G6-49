#include "defines.h"
#include "Canvas.h"
#include "Form.h"
#include "History.h"


using namespace MyMath;


Form *TheForm = nullptr;


static uint16 addData[Point::NUM_POINTS];
static bool drawAdditionData = false;


bool operator==(const Point &left, const Point &right)
{
    return (left.pos == right.pos) && (left.data == right.data);
}


Form::Form()
{
    Clear();
}


Form::~Form()
{
    points.clear();
}


void Form::Clear()
{
    for (int i = 0; i < Point::NUM_POINTS; i++)
    {
        data[i] = Point::AVE_VALUE;
    }

    points.clear();

    SetPoint(static_cast<uint16>(0), Point::AVE_VALUE);
    SetPoint(static_cast<uint16>(Point::NUM_POINTS - 1), Point::AVE_VALUE);
}


uint Form::PointInPosition(uint16 pos)
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


void Form::SetPoint(Point point)
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
    SetPoint(Point(mouseX, mouseY));
	History::Add(TheForm);
}


void Form::UndoHistory()
{	
	Form* form = History::Prev();

	if (form)
	{
		*TheForm = *form;
	}
}


void Form::RedoHistory()
{
	Form* form = History::Next();

	if (form)
	{
		*TheForm = *form;
	}
}


void Form::SetPoint(uint16 pos, uint16 dat)
{
    SetPoint(Point(pos, dat));
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
        data[Point::NUM_POINTS - 1] = points[iCurPoint].data;
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
        else
        {
            // здесь ничего
        }

        uint tempIndex = iCurPoint;

        RemovePoint();

        iCurPoint = tempIndex;

        SetPoint(point);
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
        SetPoint(points[iCurPoint]);
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


void Form::CalculateNeighboringPoints(const Point &point)
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


static void DrawForm(const uint16 data[Point::NUM_POINTS], Color color)
{
    float scaleX = Point::ScaleX();
    float scaleY = Point::ScaleY();

    TheCanvas->SetColor(color);

    for (int i = 1; i < Point::NUM_POINTS; i++)
    {
        int x0 = Round<int>(scaleX * (i - 1));
        int y0 = Round<int>(scaleY * data[i - 1]);

        int x1 = Round<int>(scaleX * i);
        int y1 = Round<int>(scaleY * data[i]);

        TheCanvas->DrawLine(x0, y0, x1, y1);
    }
}


void Form::Draw()
{
    float scaleX = Point::ScaleX();
    float scaleY = Point::ScaleY();

    DrawForm(data, Color::WHITE);

    TheCanvas->SetColor(Color::GREEN);

    for (Point point : points)
    {
        int x = Round<int>(scaleX * point.pos);
        int y = Round<int>(scaleY * point.data);

        TheCanvas->SetPoint(x, y, Point::SIZE);
    }

    if (iCurPoint != static_cast<uint>(-1))
    {
        TheCanvas->SetPoint(Round<int>(scaleX * points[iCurPoint].pos), Round<int>(scaleY * points[iCurPoint].data), Point::SIZE * 3);
    }

    if (drawAdditionData)
    {
        DrawForm(addData, Color::BLUE);
    }
}


void Form::LinearInterpolation(uint16 pos1, uint16 pos2)
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


void Form::LinearInterpolationLeft(uint index)
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


void Form::LinearInterpolationRight(uint index)
{
    Point point = points[index];

    if (index == points.size() - 1)
    {
        if (point.pos < Point::NUM_POINTS - 1)
        {
            LinearInterpolation(point.pos, Point::NUM_POINTS - 1);
        }
    }
    else
    {
        LinearInterpolation(point.pos, points[index + 1].pos);
    }
}


void Form::SetMainForm(const uint16 dat[Point::NUM_POINTS])
{
    for (int i = 0; i < Point::NUM_POINTS; i++)
    {
        data[i] = Point::AVE_VALUE;
    }

    points.clear();

    SetPoint(static_cast<uint16>(0), dat[0]);
    SetPoint(static_cast<uint16>(Point::NUM_POINTS - 1), dat[Point::NUM_POINTS - 1]);

    std::memcpy(data, dat, Point::NUM_POINTS * 2);

    History::Add(TheForm);
}


void Form::SetAdditionForm(const uint16 d[Point::NUM_POINTS])
{
    if (d)
    {
        std::memcpy(addData, d, Point::NUM_POINTS * 2);
        drawAdditionData = true;
    }
    else
    {
        drawAdditionData = false;
    }
}
