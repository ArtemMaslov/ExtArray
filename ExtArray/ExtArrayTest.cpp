#include "ExtArray.h"

using namespace UtilLib;

int main()
{
	ExtArray extArray = {};

	if (ExtArrayConstructor(&extArray, sizeof(int), 32, 2) != ProgramStatus::Ok)
	{
		return -1;
	}



	ExtArrayDestructor(&extArray);

	return 0;
}