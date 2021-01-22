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

/**
	@namespace dggt
	@brief Parent namespace of dggt library.
  */
namespace dggt
{
	typedef unsigned int uint32;
	typedef float float32;
	typedef double float64;
	typedef uint32 bool32;
}

#endif
