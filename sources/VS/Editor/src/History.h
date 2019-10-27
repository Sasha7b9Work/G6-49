#pragma once
#include "Form.h"


struct History
{
	/// Добавляет форму в историю
	static void Add(Form *form);
	/// Извлекает предыдущую форму из история
	static Form* Prev();
	/// Извлекает следующую форму из истории
	static Form* Next();
};
