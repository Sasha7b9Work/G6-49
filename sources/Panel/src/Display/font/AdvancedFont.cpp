// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "AdvancedFont.h"
#include "fontGOSTB20.inc"
#include "fontGOST28.inc"


namespace AdvancedFont
{
    TypeFont::E currentType = TypeFont::Count;
}


struct NativeSymbol
{
    uint8 width;        // Ширина символа в пикселях
    uint8 height;       // Высота символа в пикселях
    uint8 firstRow;     // Номер первой непустой строки. Именно её первый байт хранится в data

    // Возвращает количество байт в строке
    int BytesInRow();
    // Возвращает указатель на первый байт строки
    uint8 *GetRow(int row);
    // Возвращает указатель на первый байт данных
    uint8 *Data();

    bool BitIsExist(int row, int bit);
};

// Структрура заголовка
struct HeaderFont
{
    uint16       offsets[256];  // Смещения 256 символов таблицы. 0 означает, что символ отсутствует
    NativeSymbol symbol;        // Первый символ в таблице его смещение 256

    // Возвращает указатель на символ, если он присутствует в таблице и nullptr в обратном случае
    NativeSymbol *GetSymbol(uint8 num);

    static HeaderFont *Sefl();
};


namespace AFont
{
    static const unsigned char *font = nullptr;
}


void AdvancedFont::Set(TypeFont::E type)
{
    currentType = type;

    if(type == TypeFont::_GOSTB20)
    {
        AFont::font = fontGOSTB20;
    }
    else if(type == TypeFont::_GOST28)
    {
        AFont::font = fontGOST28;
    }
    else
    {
        AFont::font = nullptr;
    }
}


bool AdvancedFont::RowNotEmpty(uint8 s, int r)
{
    HeaderFont *header = HeaderFont::Sefl();

    NativeSymbol *symbol = header->GetSymbol(s);

    if (symbol)
    {
        if (r < symbol->firstRow)
        {
            return false;
        }

        uint8 *row = symbol->GetRow(r);

        if (row)
        {
            for (int i = 0; i < symbol->BytesInRow(); i++)
            {
                if (row[i] != 0)
                {
                    return true;
                }
            }
        }
    }

    return false;
}


uint8 AdvancedFont::GetWidth(uint8 num)
{
    NativeSymbol *symbol = HeaderFont::Sefl()->GetSymbol(num);

    return symbol ? symbol->width : 0U;
}

uint8 AdvancedFont::GetHeight(uint8 num)
{
    NativeSymbol *symbol = HeaderFont::Sefl()->GetSymbol(num);

    return symbol ? symbol->height : 0U;
}


uint8 AdvancedFont::GetHeight()
{
    uint8 result = 0;

    for (int i = 0; i < 256; i++)
    {
        NativeSymbol *symbol = HeaderFont::Sefl()->GetSymbol((uint8)(i));

        if (symbol && symbol->height > result)
        {
            result = symbol->height;
        }
    }

    return result;
}


int NativeSymbol::BytesInRow()
{
    int result = width / 8;

    if (width % 8)
    {
        result++;
    }

    return result;
}


uint8 *NativeSymbol::GetRow(int row)
{
    if (row > height - 1)
    {
        return nullptr;
    }

    if (row < firstRow)
    {
        return nullptr;
    }

    return Data() + (row - firstRow) * BytesInRow();
}


NativeSymbol *HeaderFont::GetSymbol(uint8 num)
{
    HeaderFont *header = HeaderFont::Sefl();

    if (header->offsets[num] == 0)
    {
        return nullptr;
    }

    uint8 *offset = (uint8 *)header + header->offsets[num];

    return (NativeSymbol *)offset;
}


HeaderFont *HeaderFont::Sefl()
{
    return (HeaderFont *)AFont::font; //-V2567
}


uint8 *NativeSymbol::Data()
{
    return (uint8 *)(this) + sizeof(*this);
}


bool AdvancedFont::BitIsExist(uint8 s, int row, int bit)
{
    NativeSymbol *symbol = HeaderFont::Sefl()->GetSymbol(s);

    return symbol ? symbol->BitIsExist(row, bit) : false;
}


bool NativeSymbol::BitIsExist(int r, int b)
{
    uint8 *row = GetRow(r);

    if (row == nullptr)
    {
        return false;
    }

    while (b > 7)       // Перемещаеммся к байту, содержащему наш бит
    {
        row++;
        b -= 8;
    }

    return ((*row) & (1 << (7 - b))) != 0;
}
