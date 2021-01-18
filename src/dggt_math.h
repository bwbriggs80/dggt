#ifndef _MATH_H_
#define _MATH_H_

#include "dggt_defs.h"

#define PI 3.14f

#define ABS(X) ((X)>0?(X):-(X))
#define MIN(A,B) ((A)<(B)?(A):(B))
#define MAX(A,B) ((A)>(B)?(A):(B))

namespace dggt::math
{
	float pow(float val,uint32 pow);

	template <uint32 Val>
	uint32 pow()
	{
		return 1;
	}

	template <uint32 Val,uint32 Pow>
	uint32 pow()
	{
		if (Pow)
		{
			return pow<Val,Pow-1>()*Val;
		}
		else
		{
			return pow<Val>();
		}
	}
}
#endif

