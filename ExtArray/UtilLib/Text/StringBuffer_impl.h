///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///
// Модуль массива char постоянного размера.
// 
// Версия: 1.0.0.0
// Дата последнего изменения: 9:47 05.03.2023
// 
// Автор: Маслов А.С. (https://github.com/ArtemMaslov).
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///

#include <cassert>
#include <cstdio>
#include <cstring>

#include "../Logs/Logs.h"

#include "StringBuffer.h"

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///

namespace UtilLib
{
	template <size_t Size>
	StringBuffer<Size>::StringBuffer(cptr str, csize_t strSize) noexcept
	{
		SetStr(str, strSize);
	}

	template <size_t Size>
	void StringBuffer<Size>::SetStr(cptr str, csize_t strSize) noexcept
	{
		assert(str);
		// Меньше, потому что strSize - это размер без '\0'.
		// Чтобы массив стал строкой, в конце нужно добавить '\0'.
		assert(strSize < Size);

		std::memcpy(Buffer, str, strSize);

		if (strSize >= Size)
		{
			Buffer[Size - 1] = '\0';
			LOG_F_WRN("Попытка записать в StringBuffer строку, большую размера буфера. Часть строки отбрасывается.\n"
					  "Размер буфера: %zd.\n"
					  "Размер строки: %zd.\n"
					  "В буфер была записана строка:\n\n"
					  ""
					  "\"%s\"\n\n"
					  ""
					  "Исходная строка:\n\n"
					  ""
					  "\"%s\"",
					  Size,
					  strSize,
					  Buffer,
					  str);
		}
	}

	///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///
	///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///

	template <size_t Size>
	StringBufferFmt<Size>::StringBufferFmt(cptr formatStr, csize_t strSize, ...) noexcept
	{
		std::va_list formatStrArgs;
		va_start(formatStrArgs, strSize);
		SetStrFmt(formatStr, strSize, formatStrArgs);
		va_end(formatStrArgs);
	}

	template <size_t Size>
	StringBufferFmt<Size>::StringBufferFmt(cptr formatStr, csize_t strSize, std::va_list formatStrArgs) noexcept
	{
		SetStrFmt(formatStr, strSize, formatStrArgs);
	}

	///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///

	template <size_t Size>
	void StringBufferFmt<Size>::SetStrFmt(cptr formatStr, csize_t strSize, ...) noexcept
	{
		std::va_list formatStrArgs;
		va_start(formatStrArgs, strSize);
		SetStrFmt(formatStr, strSize, formatStrArgs);
		va_end(formatStrArgs);
	}

	template <size_t Size>
	void StringBufferFmt<Size>::SetStrFmt(cptr formatStr, csize_t strSize, std::va_list formatStrArgs) noexcept
	{
		assert(formatStr);
		// Проверяем, что в буфер помещается хотя бы строка форматирования.
		// Здесь не проверяется, что отформатированная строка поместится в буфер.
		// Меньше, потому что strSize - это размер без '\0'.
		// Чтобы массив стал строкой, в конце нужно добавить '\0'.
		//assert(strSize < Size);

		int status = std::vsnprintf(Buffer, 0, formatStr, formatStrArgs);

		if (status < 0)
		{
			LOG_F_WRN("Ошибка внутри vsnprintf().\n"
					  "Размер буфера: %zd.\n"
					  "Размер строки: %zd.\n"
					  "Функция vsnprintf вернула: %d.\n"
					  "В буфер была записана строка:\n\n"
					  ""
					  "\"%s\"\n\n"
					  ""
					  "Строка форматирования:\n\n"
					  ""
					  "\"%s\"",
					  Size,
					  strSize,
					  status,
					  Buffer,
					  formatStr);
		}
		else if (status >= Size)
		{
			Buffer[Size - 1] = '\0';
			LOG_F_WRN("Попытка записать в StringBufferFmt строку, большую размера буфера. Часть строки отбрасывается.\n"
					  "Размер буфера: %zd.\n"
					  "Размер строки: %zd.\n"
					  "Функция vsnprintf вернула: %d.\n"
					  "В буфер была записана строка:\n\n"
					  ""
					  "\"%s\"\n\n"
					  ""
					  "Строка форматирования:\n\n"
					  ""
					  "\"%s\"",
					  Size,
					  strSize,
					  status,
					  Buffer,
					  formatStr);
		}
	}
}

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///