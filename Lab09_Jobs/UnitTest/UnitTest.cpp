#include "stdafx.h"
#include <iostream>
#include "Physics.h"

int main()
{
	bool isOk = true;
	isOk &= Physics::UnitTest();
	assert(isOk);
	if (!isOk)
	{
		std::cerr << "Physics unit test failed!" << std::endl;
		return 1;
	}
	std::cout << "Physics unit test passed!" << std::endl;
	return 0;
}