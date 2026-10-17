#include <assert.h>
#include <stdio.h>

#include "PoolAllocator.h"

PoolAllocator<int, 1024> g_intPool;

int main(int argc, char* argv[])
{
	bool isOk = true;

	int* pInt[1024];

	// Allocate everything in the pool
	for (int i = 0; i < 1024; ++i)
	{
		pInt[i] = g_intPool.Allocate();
		if (nullptr == pInt[i])
			isOk = false;
		else
			*pInt[i] = i;
	}
	assert(isOk);

	{	// try to allocate more than the pool can hold
		int* pFail = g_intPool.Allocate();
		if (nullptr != pFail)
			isOk = false;
	}
	assert(isOk);

	// make sure the values are correct
	for (int i = 0; i < 1024; ++i)
	{
		if (pInt[i] == nullptr)
			isOk = false;
		else
			isOk &= (*pInt[i] == i);
	}
	assert(isOk);
	
	// free the first 100 elements and re-allocate them
	for (int i = 0; i < 100; ++i)
	{
		g_intPool.Free(pInt[i]);
		pInt[i] = g_intPool.Allocate();
		if (nullptr == pInt[i])
			isOk = false;
		else
			*pInt[i] = 100 + i;
	}
	assert(isOk);

	{	// try to allocate more than the pool can hold
		int* pFail = g_intPool.Allocate();
		if (nullptr != pFail)
			isOk = false;
	}
	assert(isOk);

	// make sure the values are correct
	for (int i = 0; i < 1024; ++i)
	{
		if (pInt[i] == nullptr)
			isOk = false;
		else
		{
			if (i < 100)
				isOk &= (*pInt[i] == 100 + i);
			else
				isOk &= (*pInt[i] == i);
		}
	}
	assert(isOk);

	// free it all backwards
	for (int i = 1023; i >= 0; --i)
		g_intPool.Free(pInt[i]);
	assert(isOk);

	// Allocate everything in the pool
	for (int i = 0; i < 1024; ++i)
	{
		pInt[i] = g_intPool.Allocate();
		if (nullptr == pInt[i])
			isOk = false;
		else
			*pInt[i] = i;
	}
	assert(isOk);
	
	{	// try to allocate more than the pool can hold
		int* pFail = g_intPool.Allocate();
		if (nullptr != pFail)
			isOk = false;
	}
	assert(isOk);
	
	// make sure the values are correct
	for (int i = 0; i < 1024; ++i)
	{
		if (pInt[i] == nullptr)
			isOk = false;
		else
			isOk &= (*pInt[i] == i);
	}
	assert(isOk);

	return isOk ? 0 : -1;
}