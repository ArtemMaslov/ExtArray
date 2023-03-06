///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///
// Модуль выделителя памяти расширяющийся массив ExtArray.
// 
// Файлы с исходным кодом.
// 
// Версия: 1.0.1.0
// Дата последнего изменения: 17:38 28.01.2023
// 
// Автор: Маслов А.С. (https://github.com/ArtemMaslov).
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///

#pragma once

#include <cassert>
#include <utility>

#include <UtilLib/Errors/Exceptions.h>

#include "ExtArray.h"

#if (MODULE_EXT_ARRAY_DISABLE_LOGS == false)
	#include <UtilLib/Logs/Logs.h>
#else
	#include <UtilLib/DisableLogs.h>
#endif

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///

namespace UtilLib
{
	#if (MODULE_EXT_ARRAY_ENABLE_CTOR_TRACE)

		template <typename T>
		ExtArray<T>::ExtArray(const CodeLocation location,
							  csize_t  capacity,
							  cfloat_t resize) :
			Array(nullptr),
			Size(0),
			Capacity(capacity),
			ResizeCoefficient(resize),
			ReallocCounter(0),
			Location(location)
		{
			assert(capacity > 0);
			assert(resize   > 1);

			Array = AllocArray(capacity);
		}

	#else // (MODULE_EXT_ARRAY_ENABLE_CTOR_TRACE)

		template <typename T>
		ExtArray<T>::ExtArray(csize_t  capacity,
							  cfloat_t resize) :
		Array(new char[ElemSize * capacity]),
			Size(0),
			Capacity(capacity),
			ResizeCoefficient(resize),
			ReallocCounter(0)
		{
			assert(capacity > 0);
			assert(resize   > 1);
		}

	#endif // (MODULE_EXT_ARRAY_ENABLE_CTOR_TRACE)
	
	template <typename T>
	ExtArray<T>::~ExtArray() noexcept
	{
		if (ReallocCounter > 0)
		{
			LOG_WRN("В процессе работы ExtArray были перевыделения памяти.\n"
					  "Не эффективный начальный размер capacity.");

			#if MODULE_EXT_ARRAY_ENABLE_CTOR_TRACE
				LOG_F_WRN("Функция ~ExtArray() вызвана %s из файла %s в строке %zd.",
						  Location.FunctName, Location.FileName, Location.Line);
			#endif // !MODULE_EXT_ARRAY_ENABLE_CTOR_TRACE

			Dump(CODE_LOCATION);
		}

		for (size_t st = 0; st < Size; st++)
			Array[st].~T();

		delete [] static_cast<void*>(Array);
	}

	///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///
	///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///

	template <typename T>
	void ExtArray<T>::AddElem(const T& elem)
	{
		if (Size == Capacity)
			Resize(CODE_LOCATION);
		
		Array[Size++] = elem;
	}

	template <typename T>
	void ExtArray<T>::AddElem(T&& elem)
	{
		if (Size == Capacity)
			Resize(CODE_LOCATION);

		Array[Size++] = std::move(elem);
	}
	
	template <typename T>
	T& ExtArray<T>::operator [] (csize_t st)
	{
		assert(st < Size);

		return Array[st];
	}

	template <typename T>
	const T& ExtArray<T>::operator [] (csize_t st) const
	{
		assert(st < Size);

		return Array[st];
	}

	///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///
	///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///

	template <typename T>
	void ExtArray<T>::Dump(const CodeLocation location) const noexcept
	{
		LOG_F_DBG("Состояние ExtArray [%p]:\n"
				  "Array          = %p.\n"
				  "ElemSize       = %zd.\n"
				  "Size           = %zd.\n"
				  "Capacity       = %zd.\n"
				  "Resize         = %f.\n"
				  "ReallocCounter = %zd.\n"
				  "Функция Dump() вызвана %s из файла %s в строке %zd.",
				  this, Array, ElemSize, Size, Capacity, 
				  ResizeCoefficient, ReallocCounter, 
				  location.FunctName, location.FileName, location.Line);
	}

	///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///
	///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///

	template <typename T>
	void ExtArray<T>::Resize(const CodeLocation location)
	{
		assert(Capacity > 0);
		assert(ResizeCoefficient > 1);

		LOG_F_WRN("Выделение дополнительный памяти ExtArray.\n"
				  "Не эффективный начальный размер capacity.\n"
				  "Функция Resize() вызвана %s из файла %s в строке %zd.",
				  location.FunctName, location.FileName, location.Line);

		Dump(CODE_LOCATION);
	
		// Новое количество элементов массива.
		size_t newCapacity = (size_t)((float)Capacity * ResizeCoefficient);
	
		T* newData = AllocArray(newCapacity);

		memcpy(newData, Array, ElemSize * Size);

		delete [] static_cast<void*>(Array);

		Array    = newData;
		Capacity = newCapacity;
		ReallocCounter++;
	}

	template <typename T>
	T* ExtArray<T>::AllocArray(csize_t capacity) const
	{
		const size_t allocSize = ElemSize * capacity;
		T* data = static_cast<T*>(operator new(allocSize, std::nothrow));

		if (data == nullptr)
		{
			LOG_ERR_MEMORY(allocSize);
			Dump(CODE_LOCATION);

			throw MemoryAllocation(GET_STRING_VIEW("Ошибка выделения памяти в ExtArray."),
								   allocSize,
								   CODE_LOCATION);
		}

		return data;
	}
}

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///