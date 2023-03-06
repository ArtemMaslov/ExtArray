///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///
// Общий файл настроек конфигурации стандартной библиотеки.
// 
// Версия: 1.0.0.1
// Дата последнего изменения: 21:08 04.03.2023
// 
// Автор: Маслов А.С. (https://github.com/ArtemMaslov).
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///

#pragma once

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///

#if defined(WINDOWS)

	#define PATH_SEPARATOR "\\"

#elif defined(LINUX)

	#define PATH_SEPARATOR "/"

#endif

namespace UtilLib
{
	typedef const char*         const cptr;
	typedef const int*          const cint_ptr;
	typedef const unsigned int* const cuint_ptr;
	typedef const size_t*       const csize_ptr;
	typedef const long*         const clong_ptr;
	typedef const long long*    const cll_ptr;
	typedef const float*        const cfloat_ptr;
	typedef const double*       const cdouble_ptr;

	typedef const char             cchar_t;
	typedef const int              cint_t;
	typedef const unsigned int     cuint_t;
	typedef const size_t           csize_t;
	typedef const long             clong_t;
	typedef const long long        cll_t;
	typedef const float            cfloat_t;
	typedef const double           cdouble_t;

	class NonCopyable 
	{
	protected:
		NonCopyable() = default;
		~NonCopyable() = default;

		NonCopyable(const NonCopyable& that) = delete;
		NonCopyable& operator = (const NonCopyable& that) = delete;
	};

	class NonMoveable
	{
	protected:
		NonMoveable() = default;
		~NonMoveable() = default;

		// Устанавливаем операции копирования явно по умолчанию.
		// Иначе они неявно удаляются MSVC.
		NonMoveable(const NonMoveable& that) = default;
		NonMoveable& operator = (const NonMoveable& that) = default;

		// Удаляем операции перемещения.
		NonMoveable(NonMoveable&& that) = delete;
		NonMoveable& operator = (NonMoveable&& that) = delete;
	};
}

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***///