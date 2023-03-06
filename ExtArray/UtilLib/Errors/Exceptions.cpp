///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///
// Модуль обработки исключений.
// 
// Версия: 1.0.0.0
// Дата последнего изменения: 22:17 04.03.2023
// 
// Автор: Маслов А.С. (https://github.com/ArtemMaslov).
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///

#include <cassert>
#include <cstdarg>

#include "../General.h"

#include "../Logs/CodeLocation.h"
#include "../Text/StringBuffer.h"
#include "../Text/StringView.h"

#include "Exceptions.h"

using namespace UtilLib;

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///

Exception::Exception(const StringView message, const CodeLocation location) :
	Message(message.String, message.Length),
	Location(location)
{
}

Exception::Exception(const StringView formatStr, const CodeLocation location, std::va_list formatStrArgs) :
	Message(StringBufferFmt<MessageSize>(formatStr.String, formatStr.Length, formatStrArgs)),
	Location(location)
{
}

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///

MemoryAllocation::MemoryAllocation(const StringView message, csize_t memAllocSize, 
								   const CodeLocation location) :
	Exception(message, location),
	AllocatedMemorySize(memAllocSize)
{
}

MemoryAllocation::MemoryAllocation(const StringView formatStr, csize_t memAllocSize, 
								   const CodeLocation location, std::va_list formatStrArgs) :
	Exception(formatStr, location, formatStrArgs),
	AllocatedMemorySize(memAllocSize)
{
}

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///

OutOfRange::OutOfRange(const StringView message, cll_t index, 
					   const CodeLocation location) :
	Exception(message, location),
	Index(index)
{
}

OutOfRange::OutOfRange(const StringView formatStr, cll_t index, 
					   const CodeLocation location, std::va_list formatStrArgs) :
	Exception(formatStr, location, formatStrArgs),
	Index(index)
{
}

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///