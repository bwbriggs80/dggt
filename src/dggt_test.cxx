#include <cstdio>
#include "dggt.h"

using namespace dggt;
using namespace dggt::mem;
using namespace dggt::coll;

void disp_size_t(const char* label,size_t size);
void disp_uint32(const char* label,uint32 val);
void disp_size(const char* label,size_t size);
void disp_msg(const char* msg);
void disp_alloc_available(allocator* alloc);
void disp_alloc_used(allocator* alloc);
void disp_alloc_size(allocator* alloc);
void test_pool_alloc(void* mem,size_t memSize);
void test_stack_alloc(void* mem,size_t memSize);
void test_free_block_alloc(void* mem,size_t memSize);
void test_darray_uint32(darray<uint32>* arr,allocator* alloc);

int main(int argc,char* argv[])
{
	size_t memSize=KILOBYTES(2);
	void* mem=alloc_mem(memSize);
	allocator freeBlockAlloc_=allocator(FREE_BLOCK_ALLOC,
			mem,memSize);
	allocator* freeBlockAlloc=&freeBlockAlloc_;

	darray<uint32> uint32Arr=create_darray<uint32>(freeBlockAlloc);
	test_darray_uint32(&uint32Arr,freeBlockAlloc);

	return 0;
}

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
	allocator poolAlloc_=allocator(mem,memSize,sizeof(double));
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

void disp_uint32(const char* label,uint32 val)
{
	printf("%s: ", label);
	printf("%d\n", val);
}
void disp_size_t(const char* label,size_t size)
{
	printf("%s: ", label);
	printf("%d\n", size);
}

void test_darray_uint32(darray<uint32>* arr,allocator* alloc)
{
	uint32 arrSize=10;
	for (uint32 i=0;i<arrSize;++i)
	{
		push(arr,i+1,alloc);
		disp_uint32("darray size",get_size(arr));
		disp_uint32("darray count",get_count(arr));
	}

	uint32 i=0;
	for (darray<uint32>::iter it=begin_iter(arr);
			!it.is_end();++it)
	{
		printf("array[%d]: %d\n",i,*it);
		++i;
	}

	while (get_count(arr))
	{
		if (get_count(arr)==2)
		{
			bool32 b=0;
		}
		pop(arr,alloc);
		disp_uint32("darray size",get_size(arr));
		disp_uint32("darray count",get_count(arr));
	}
}



