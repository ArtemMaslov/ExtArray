#pragma once

#if defined(WINDOWS)
#if defined(MSVC)

	// Отключить предупреждения в сторонней библиотеке.
	// Microsoft Visual Studio 2022. 
	// Platform Toolset Visual Studio 2022 v143.
	#pragma warning (push)
	#pragma warning (disable: 4668 5039)
	#include <Windows.h>
	#pragma warning (pop)

#else // MSVC

	#include <Windows.h>

#endif // !MSVC
#endif // !WINDOWS