#pragma once

#if defined(MSVC)

	// Отключить предупреждения в сторонней библиотеке.
	#pragma warning (push)
	#pragma warning (disable: 4365)
	#include <string>
	#pragma warning (pop)

#else

	#include <string>

#endif