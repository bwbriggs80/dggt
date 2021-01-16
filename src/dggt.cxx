#include <cstdio>
#include "dggt.h"

using namespace dggt::mem;

int main(int argc,char* argv[])
{
	size_t memSize=70;
	void* mem=alloc_mem(memSize);

	allocator stackAlloc=allocator(STACK_ALLOC,
		mem,memSize);

	
	printf("%d\n",stackAlloc.get_available());
	float* floatArr=stackAlloc.alloc<float>(10);
	printf("%d\n",stackAlloc.get_available());
	stack_state state=stackAlloc.save_state();
	int* intAlloc=stackAlloc.alloc<int>(5);
	printf("%d\n",stackAlloc.get_available());
	stackAlloc.restore_state(state);
	printf("%d\n",stackAlloc.get_available());

	stackAlloc.clear();
	return 0;
}
