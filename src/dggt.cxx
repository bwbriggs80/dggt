#include <cstdio>
#include "dggt.h"

using namespace dggt::mem;

int main(int argc,char* argv[])
{
	size_t memSize=50;
	void* mem=alloc_mem(memSize);

	allocator linAlloc=allocator(LINEAR_ALLOC,mem,memSize);

	size_t floatCount=10;
	float* floatArr=linAlloc.alloc<float>(floatCount);
	for (int i=0;i<floatCount;++i)
	{
		floatArr[i]=2*i;
		float val=floatArr[i];
		printf("%f\n",val);
	}

	size_t uCharCount=10;
	unsigned char* uCharArr=
		linAlloc.alloc<unsigned char>(uCharCount);
	for (int i=0;i<uCharCount;++i)
	{
		uCharArr[i]='x';
		unsigned char val=uCharArr[i];
		printf("%c\n",val);
	}


	linAlloc.clear();


	return 0;
}
