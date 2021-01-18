#include "dggt_math.h"

namespace dggt::math
{
	float pow(float val,uint32 pow)
	{
		float result=1;
		for (uint32 i=0;i<pow;++i)
		{
			result*=val;
		}
		return result;
	}
}
