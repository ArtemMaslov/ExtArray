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

	arr1.PushBack(10);
	arr1.PushBack(40);
	arr1.PushBack(50);

	arr1[1] = 20;
	arr1.PushBack(60);
	arr1.PushBack(70);

	const ExtArray<Float> arr2(CODE_LOCATION, 3);
	Float a = arr2[0];
	//arr2.PushBack({3.14f});
	//arr2.PushBack({2.71f});
	//arr2.PushBack({1.602e-19f});
	//arr2[0].print();
	//arr2[2].print();

	//Float* arr = new Float[10];

	LogsDestructor();

	return 0;
}