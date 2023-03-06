#include <inc_windows.h>

#include "ExtArray.h"

#include <UtilLib/Text/StringBuffer.h>

using namespace UtilLib;

class Float
{
public:
	Float(float value) : value(value) {}

	float value;

	void print()
	{
		printf("%g\n", value);
	}
};

int main()
{
	#if defined(WINDOWS)
		SetConsoleCP(65001);
		SetConsoleOutputCP(65001);
	#endif // WINDOWS

	LogsConstructor();

	ExtArray<int> arr1(CODE_LOCATION, 4);

	arr1.AddElem(10);
	arr1.AddElem(40);
	arr1.AddElem(50);

	arr1[1] = 20;
	arr1.AddElem(60);
	arr1.AddElem(70);

	ExtArray<Float> arr2(CODE_LOCATION, 3);
	arr2.AddElem({3.14f});
	arr2.AddElem({2.71f});
	arr2.AddElem({1.602e-19f});
	arr2[0].print();
	arr2[2].print();

	//Float* arr = new Float[10];

	LogsDestructor();

	return 0;
}