#include <cstdio>
#include "dggt.h"

using namespace dggt::mem;

void disp_size(const char* label,size_t size)
{
	printf("%s: ",label);
	printf("%d\n",size);
}

void disp_msg(const char* msg)
{
	printf("%s\n",msg);
}

void disp_alloc_available(allocator* alloc)
{
	if (alloc)
	{
		disp_size("available",alloc->get_available());
	}
}

void disp_alloc_used(allocator* alloc)
{
	if (alloc)
	{
		disp_size("used",alloc->get_used());
	}
}

void disp_alloc_size(allocator* alloc)
{
	if (alloc)
	{
		disp_size("size",alloc->get_size());
	}
}

void test_stack_alloc(void* mem,size_t memSize)
{
	allocator stackAlloc_=allocator(STACK_ALLOC,
		mem,memSize);
	allocator* stackAlloc=&stackAlloc_;

	disp_alloc_available(stackAlloc);
	disp_msg("allocating 10 floats");
	float* floatArr=stackAlloc->alloc<float>(10);
	disp_alloc_available(stackAlloc);
	disp_msg("saving alloc state");
	stack_state state=stackAlloc->save_state();
	disp_msg("allocating 5 ints");
	int* intAlloc=stackAlloc->alloc<int>(5);
	disp_alloc_available(stackAlloc);
	disp_msg("restoring state");
	stackAlloc->restore_state(state);
	disp_alloc_available(stackAlloc);

	stackAlloc->clear();
	stackAlloc->clear_buff();
}

void test_pool_alloc(void* mem,size_t memSize)
{
	allocator poolAlloc_=allocator(POOL_ALLOC,mem,memSize,sizeof(double));
	allocator* poolAlloc=&poolAlloc_;

	disp_alloc_available(poolAlloc);
	disp_msg("allocating a double");
	double* dbl0=poolAlloc->alloc<double>();
	disp_alloc_available(poolAlloc);
	*dbl0=10.0f;
	printf("%f\n",*dbl0);
	double* dbl10=poolAlloc->alloc<double>();
	disp_msg("allocating a double");
	disp_alloc_available(poolAlloc);

	disp_msg("clearing allocator");
	poolAlloc->clear();
	disp_alloc_available(poolAlloc);

	poolAlloc->clear_buff();
}

void test_free_block_alloc(void* mem,size_t memSize)
{
	allocator freeAlloc_=allocator(FREE_BLOCK_ALLOC,
		mem,memSize);
	allocator* freeAlloc=&freeAlloc_;

	int floatCount=2;
	int intCount=5;
	int dblCount=2;

	disp_alloc_available(freeAlloc);
	disp_msg("allocating floats");
	float* floatArr=freeAlloc->alloc<float>(floatCount);

	disp_alloc_available(freeAlloc);

	disp_msg("allocating ints");
	int* intArr=freeAlloc->alloc<int>(intCount);
	intArr[0]=5;

	disp_alloc_available(freeAlloc);

	disp_msg("allocating doubles");
	double* dblArr=freeAlloc->alloc<double>(dblCount);
	disp_alloc_available(freeAlloc);

	disp_msg("freeing doubles");
	freeAlloc->free(dblArr,dblCount);

	disp_msg("freeing ints");
	freeAlloc->free(intArr,intCount);

	disp_alloc_available(freeAlloc);

	disp_msg("freeing floats");
	freeAlloc->free(floatArr,floatCount);

	disp_alloc_available(freeAlloc);

	disp_msg("clearing allocator");
	freeAlloc->clear();
	disp_alloc_available(freeAlloc);
	freeAlloc->clear_buff();
}

int main(int argc,char* argv[])
{
	size_t memSize=70;
	void* mem=alloc_mem(memSize);
	test_free_block_alloc(mem,memSize);

	return 0;
}
