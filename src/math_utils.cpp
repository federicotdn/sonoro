#include <math_utils.h>

using namespace so;

int MathUtils::clamp(int min, int max, int val)
{
	if (val < min)
	{
		return min;
	}
	else if (val > max)
	{
		return max;
	}

	return val;
}
