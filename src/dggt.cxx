#include <cstdio>

#include "dggt.h"

int main(int argc,char* argv[])
{
	float test=5.0f;
	float squaredVal=dggt::square(PI*test);
	printf("Hello World!\n%f",squaredVal);
	return 0;
}
