///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///
// Модуль обработки исключений.
// 
// Версия: 1.0.0.0
// Дата последнего изменения: 22:17 04.03.2023
// 
// Автор: Маслов А.С. (https://github.com/ArtemMaslov).
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///

#pragma once

#include <cstdarg>

#include "../General.h"

#include "../Logs/CodeLocation.h"
#include "../Text/StringBuffer.h"
#include "../Text/StringView.h"

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///

namespace UtilLib
{
	class Exception
	{
	public:
		Exception(const StringView message, const CodeLocation location);

		template <typename ... Args>
		Exception(const StringView formatStr, const CodeLocation location, Args ... args);

		Exception(const StringView formatStr, const CodeLocation location, std::va_list formatStrArgs);

	public:
		static constexpr csize_t MessageSize = 512;
		const StringBuffer<MessageSize> Message;

		const CodeLocation Location;
	};

	///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///
	///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///
	
	class MemoryAllocation : public Exception
	{
	public:
		MemoryAllocation(const StringView message, csize_t memAllocSize, 
						 const CodeLocation location);

		template <typename ... Args>
		MemoryAllocation(const StringView message, csize_t memAllocSize, 
						 const CodeLocation location, Args ...);

		MemoryAllocation(const StringView formatStr, csize_t memAllocSize, 
						 const CodeLocation location, std::va_list formatStrArgs);

	public:
		csize_t AllocatedMemorySize;
	};
}

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///