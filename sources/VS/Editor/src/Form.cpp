#include "defines.h"
#include "Canvas.h"
#include "Form.h"
#include "History.h"
#pragma warning(push, 0)
#include <wx/msgdlg.h>
#pragma warning(pop)


using namespace MyMath;


Form *TheForm = nullptr;


static uint16 addData[Point::NUM_POINTS];
static bool drawAdditionData = false;

#define LABEL_FILE  wxT("Data file G6-49")


/// Преобразует строку в два целых числа и возвращает true в качестве успеха
static bool ConvertStringToTwoShort(const wxString &line, uint16 *val1, uint16 *val2);
/// Преобразует подстроку в целое число. Возвращает false в случае неудачи
static bool ConvertSubStringToShort(const wxString &line, uint startPos, uint endPos, uint16 *value, unsigned long max);
/// Вывод сообщения об ошибке
static void ErrorMessage(const wxString &text);
static void ErrorMessage(uint numString);


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
        data[i] = Point::AVE;
    }

    points.clear();

    SetPointInRealCoord(static_cast<uint16>(0), Point::AVE);
    SetPointInRealCoord(static_cast<uint16>(Point::NUM_POINTS - 1), Point::AVE);
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

    if (index == static_cast<uint>(-1))
    {
        points.push_back(point);

        std::sort(points.begin(), points.end());
    }
    else
    {
        points[index] = point;
    }

    CalculateNeighboringPoints(point);
}


void Form::SetPointInMouseCoord(int mouseX, int mouseY)
{
    if (!TheCanvas->IsBlocknig())
    {
        SetPoint(Point(mouseX, mouseY));
        History::Add(TheForm);
    }
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


void Form::SetPointInRealCoord(uint16 pos, uint16 dat)
{
    if (!TheCanvas->IsBlocknig())
    {
        SetPoint(Point(pos, dat));
    }
}


void Form::SetPointInPosition(uint16 pos)
{
    Point point(pos, data[pos]);

    uint index = PointInPosition(point.pos);

    if(index != static_cast<uint>(-1))
    {
        points[index] = point;
    }
    else
    {
        points.push_back(point);

        std::sort(points.begin(), points.end());
    }
}


void Form::RemovePoint()
{
    if (iCurPoint < points.size())
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

    TheCanvas->Redraw();
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

                pFuncCompare func = (align == Align::LeftDown) ? CompareLess : CompareMore;

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

                pFuncCompare func = (align == Align::RightDown) ? CompareMore : CompareLess;

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

    History::Add(TheForm);
}


bool Form::ExistPoint(int mouseX, int mouseY, bool pressed, uint16 *index, uint16 *value)
{
    float scaleX = Point::ScaleX();
    float scaleY = Point::ScaleY();

    uint16 positionNearestPoint = static_cast<uint16>(-1);
    double nearestDistance = 1e10;

    for(uint16 i = 0; i < points.size(); i++)
    {
        double distance = points[i].DistanceFromMouse(Round<int>(static_cast<float>(mouseX) / scaleX), Round<int>(static_cast<float>(mouseY) / scaleY));

        if(distance < nearestDistance)
        {
            nearestDistance = distance;
            positionNearestPoint = i;
        }
    }

    if(nearestDistance < Point::SIZE * 5)
    {
        if(iCurPoint == static_cast<uint>(-1) || !pressed)
        {
            iCurPoint = positionNearestPoint;
        }

        if (index)
        {
            *index = points[positionNearestPoint].pos;
        }

        if (value)
        {
            *value = points[positionNearestPoint].data;
        }

        return true;
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
        int x0 = Round<int>(scaleX * static_cast<float>(i - 1));
        int y0 = Round<int>(scaleY * static_cast<float>(Point::MAX - data[i - 1]));

        int x1 = Round<int>(scaleX * static_cast<float>(i));
        int y1 = Round<int>(scaleY * static_cast<float>(Point::MAX - data[i]));

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
        int x = Round<int>(scaleX * static_cast<float>(point.pos));
        int y = Round<int>(scaleY * static_cast<float>(Point::MAX - point.data));

        TheCanvas->DrawPoint(x, y, Point::SIZE);
    }

    if (iCurPoint != static_cast<uint>(-1))
    {
        TheCanvas->DrawPoint(Round<int>(scaleX * static_cast<float>(points[iCurPoint].pos)), Round<int>(scaleY * static_cast<float>(Point::MAX - points[iCurPoint].data)), Point::SIZE * 3);
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

    float delta = static_cast<float>(data2 - data1) / static_cast<float>(pos2 - pos1);  // Разность значений между соседними точками
    
    float value = static_cast<float>(data[pos1]) + delta;                                  // Значение в текущей позиции
    
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


void Form::SetMainForm(const uint16 dat[Point::NUM_POINTS], const std::vector<Point> *p)
{
    for (int i = 0; i < Point::NUM_POINTS; i++)
    {
        data[i] = Point::AVE;
    }

    points.clear();

    SetPointInRealCoord(static_cast<uint16>(0), dat[0]);
    SetPointInRealCoord(static_cast<uint16>(Point::NUM_POINTS - 1), dat[Point::NUM_POINTS - 1]);

    std::memcpy(data, dat, Point::NUM_POINTS * 2);

    if(p)
    {
        for(Point point : *p)
        {
            SetPoint(point);
        }
    }

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

    TheCanvas->Redraw();
}


bool Form::IsEquals(const Form *form) const
{
    if(points != form->points)
    {
        return false;
    }

    for(int i = 0; i < Point::NUM_POINTS; i++)
    {
        if(data[i] != form->data[i])
        {
            return false;
        }
    }

    return true;
}


void Form::SaveToFile(wxTextFile &file)
{
    file.Clear();

    file.AddLine(LABEL_FILE);

    file.AddLine(wxT("points"));

    for(Point &point : points)
    {
        file.AddLine(wxString::Format(wxT("%i %i"), point.pos, point.data));
    }
}


void Form::LoadFromFile(wxTextFile &file)
{
    wxString line = file.GetFirstLine();

    if(line.Cmp(LABEL_FILE) != 0)
    {
        return ErrorMessage(wxT("Файл не является файлом данных."));
    }

    if(file.GetNextLine().Cmp(wxT("points")) != 0)
    {
        return ErrorMessage(file.GetCurrentLine());
    }

    while(true)
    {
        line = file.GetNextLine();
        if(line.IsEmpty())
        {
            break;
        }

        uint16 index = 0;
        uint16 d = 0;

        if(!ConvertStringToTwoShort(line, &index, &d))
        {
            return ErrorMessage(file.GetCurrentLine());
        }

        SetPointInRealCoord(index, d);
    }
}


static void ErrorMessage(const wxString &text)
{
    wxMessageBox(text);
}


static void ErrorMessage(uint numString)
{
    wxMessageBox(wxString::Format(wxT("Ошибка данных в строке %i"), numString));
}


static bool ConvertStringToTwoShort(const wxString &line, uint16 *val1, uint16 *val2)
{
    int pos = line.Find(' ');

    if(pos == -1)
    {
        return false;
    }

    if(!ConvertSubStringToShort(line, 0, static_cast<uint>(pos - 1), val1, Point::NUM_POINTS))
    {
        return false;
    }

    if(!ConvertSubStringToShort(line, static_cast<uint>(pos + 1), line.size() - 1, val2, Point::MAX))
    {
        return false;
    }

    return true;
}


static bool ConvertSubStringToShort(const wxString &line, uint startPos, uint endPos, uint16 *value, unsigned long max)
{
    wxString str = line.SubString(startPos, endPos);

    unsigned long val = 0;

    if(!str.ToULong(&val) || val > max)
    {
        return false;
    }

    *value = static_cast<uint16>(val);

    return true;
}
