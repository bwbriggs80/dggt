#ifndef _DGGT_DEFS_H_
#define _DGGT_DEFS_H_

#include <stddef.h>

#define MAX_UINT32 0xFFFFFFFF
#define MAX_UINT64 0xFFFFFFFFFFFFFFFF
#define MAX_SIZE MAX_UINT64

#define KILOBYTES(X) (1024U*X)
#define MEGABYTES(X) (1024U*KILOBYTES(X))
#define GIGABYTES(X) (1024U*MEGABYTES(X))
#define TRUE 1
#define FALSE 0

#ifdef DEBUG
#define ASSERT(P) if (!(P)) { *((int*)0)=1; }
#elif defined RELEASE
#define ASSERT(P)
#endif

/**
	@namespace dggt
	@brief Parent namespace of dggt library.
  */
namespace dggt
{
	typedef size_t msize;
	typedef char int8;
	typedef short int16;
	typedef int int32;
	typedef long long int64;
	typedef unsigned char uint8;
	typedef unsigned short uint16;
	typedef unsigned int uint32;
	typedef unsigned long long uint64;
	typedef float float32;
	typedef double float64;
	typedef uint32 bool32;
	typedef uint64 bool64;
	typedef int8 byte;
	typedef uint8 ubyte;
}

#endif
