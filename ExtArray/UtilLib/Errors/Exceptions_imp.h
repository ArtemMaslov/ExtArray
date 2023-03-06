///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///
// Модуль обработки исключений.
// 
// Версия: 1.0.0.0
// Дата последнего изменения: 14:17 06.03.2023
// 
// Автор: Маслов А.С. (https://github.com/ArtemMaslov).
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///

#pragma once

#include "Exceptions.h"

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///

namespace UtilLib
{
	template <typename ... Args>
	Exception::Exception(const StringView formatStr, const CodeLocation location, Args ... args) :
		Message(StringBufferFmt<MessageSize>(formatStr.String, formatStr.Length, args)),
		Location(location)
	{
	}

	template <typename ... Args>
	MemoryAllocation::MemoryAllocation(const StringView formatStr, csize_t memAllocSize, 
									   const CodeLocation location, Args ... args) :
		Exception(formatStr, location, args)
	{
	}

	template <typename ... Args>
	OutOfRange::OutOfRange(const StringView formatStr, cll_t index, 
						   const CodeLocation location, Args ... args) :
		Exception(formatStr, location, args)
	{
	}
}

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///