///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///
// Модуль массива char постоянного размера.
// 
// Поддерживает форматирование строки в стиле printf.
// 
// Версия: 1.0.0.0
// Дата последнего изменения: 9:47 05.03.2023
// 
// Автор: Маслов А.С. (https://github.com/ArtemMaslov).
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///

#pragma once

#include <cstdarg>

#include "../General.h"

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///

namespace UtilLib
{
	template <size_t Size = 0>
	class StringBuffer
	{
	public:
		StringBuffer() = default;

		StringBuffer(cptr str, csize_t strSize) noexcept;

		void SetStr(cptr str, csize_t strSize) noexcept;

	protected:
		static_assert(Size != 0);
		char Buffer[Size] = "";
	};

	template <size_t Size = 0>
	class StringBufferFmt : public StringBuffer<Size>
	{
		using StringBuffer<Size>::Buffer;

	public:
		StringBufferFmt(cptr formatStr, csize_t strSize, ...) noexcept;

		StringBufferFmt(cptr formatStr, csize_t strSize, std::va_list formatStrArgs) noexcept;

		void SetStrFmt(cptr formatStr, csize_t strSize, ...) noexcept;

		void SetStrFmt(cptr formatStr, csize_t strSize, std::va_list formatStrArgs) noexcept;
	};
}

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///

#include "StringBuffer_impl.h"

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///