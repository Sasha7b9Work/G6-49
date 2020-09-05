#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


struct Bitmap
{
public:
	static wxBitmap* LoadFromResource(int name);
};
