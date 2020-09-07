#pragma once
#include "defines.h"
#include "Editor/Painter/Canvas.h"
#include "Editor/Painter/Zoomer.h"
#include "Utils/MyMath.h"

#include <vector>

#pragma warning(push, 0)
#include <wx/textfile.h>
#pragma warning(pop)

/*
    Класс представления формы сигнала
*/


struct Align
{
    enum E
    {
        Left,
        Right,
        LeftTop,
        LeftDown,
        RightTop,
        RightDown
    };
};


struct Marker
{
	static const int SIZE = 5;
};


struct Point
{
	static const int SIZE = 3;

    static const uint16 MIN = static_cast<uint16>(0);
	static const uint16 MAX = static_cast<uint16>((1 << 12) - 1);
	static const uint16 AVE = static_cast<uint16>((MAX) / 2);

	static const int AMOUNT = 8192;

	Point(int canvasX, int canvasY)
	{
		pos = Math::Round<uint16>(static_cast<float>(canvasX) / PixelsInPointX());
		SetCanvasY(canvasY);
	}
	Point(uint16 p, uint16 d) : pos(p), data(d) {};
	void SetCanvasY(int canvasY)
	{
		data = static_cast<uint16>(Point::MAX - Math::Round<uint16>(static_cast<float>(canvasY - Grid::Y()) / PixelsInDiscretY()));
	}

	uint16 pos;
	uint16 data;
	bool operator < (const Point& point) const
	{
		return (pos < point.pos);
	}
	// Возвращает true, если курсор мыши находится над этой точкой
	double DistanceFromCanvas(int canvasX, int canvasY)
	{
        int dX = canvasX - CanvasX();

        int dY = canvasY - CanvasY();

        return std::sqrt(dX * dX + dY * dY);
	}
	// Масштаб по горизонтали - пикселей на точку
	static float PixelsInPointX()
	{
		return static_cast<float>(Grid::Width()) / static_cast<float>(Zoomer::NumberDrawingPoints());
	}
	// Масштаб по вертикали - пикселей на дискрет
	static float PixelsInDiscretY()
	{
		return static_cast<float>(Grid::Height()) / static_cast<float>(MAX);
	}

	// Преобразует координату мыши в точку [0 ... (Point::AMOUNT - 1)]
	static int MouseToPointX(int mouseX)
	{
		return Math::Round<int>(static_cast<float>(mouseX) / PixelsInPointX());
	}

	static int PointToMouseX(int x)
	{
		return Math::Round<int>(static_cast<float>(x) * PixelsInPointX());
	}

	// Нарисовать параметры точки
	void DrawParameters();

	// Рассчитать координаты для вывода параметров
	wxPoint CalculateCoordParameters();

	// Координата X точки на холсте
	int CanvasX();

	// Координата Y точки на холсте
	int CanvasY();
};


class Form
{
public:
    Form();
    ~Form();
    // Очистить сигнал
    void Clear();

	// Добавить маркер с координатами мыши
    void SetMarkerInMouseCoord(int mouseX, int mouseY);

	// Добавить точку с абсолютными значениями
    void SetPointInRealCoord(uint16 pos, uint16 data);

	// Заносит точку куда следует
    void SetPoint(Point point);

	// Вставить точку в позицию pos
    void SetPointInPosition(uint16 pos);

	// Возвращает true, если курсор мыши находится над поставленной точкой. Pressed - нажата ли кнопка мыши
	bool ExistMarker(int mouseX, int mouseY, bool pressed, uint16 *index = nullptr, uint16 *value = nullptr);

	// Удалить точку в позиции мыши
    void RemoveCurrentMarker();

	// Переместить маркер в новую позицию
    void MoveMarker(int canvasX, int canvasY);

	// Выровнять точку
    void AlignPoint(Align::E align);

    void Draw();

	void UndoHistory();

	void RedoHistory();

	// Установить дополнительную форму, которая будет рисоваться поверх основной. 
    void SetAdditionForm(const uint16 data[Point::AMOUNT]);

	// Установить основную форму
    void SetMainForm(const uint16 data[Point::AMOUNT], const std::vector<Point> *points);

    bool IsEquals(const Form *form) const;

    void SaveToFile(wxTextFile &file);

    void LoadFromFile(wxTextFile &file);

private:

	// Данные, готовые для передачи в прибор
	uint16 data[Point::AMOUNT];

	// Точки, поставленные мышью. Хранятся в системе координат прибора (pos = [0 ... Point::AMOUNT], data = [Point::MIN ... Point::MAX])
	std::vector<Point> markers;

	// Здесь хранится индекс точки, которой управляем в текущий момент
	uint iCurMarker = 0;

	// Возвращает index точки в позиции pos. 0xFFFFFFFF, если точки в этой позиции нет
	uint PointInPosition(uint16 pos);

	// Рассчитать соседние с point точки
	void CalculateNeighboringPoints(const Point& point);

	// Линейно интерполировать точки, расположенные между pos1 и pos2
	void LinearInterpolation(uint16 pos1, uint16 pos2);

	// Интерполировать точки слева от точки с индексом index из points
	void LinearInterpolationLeft(uint index);

	// Интерполировать точки справа от точки с индексом index из points
	void LinearInterpolationRight(uint index);
};


extern Form *TheForm;
