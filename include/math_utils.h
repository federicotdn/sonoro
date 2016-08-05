#ifndef MATH_UTILS_H
#define MATH_UTILS_H

namespace so
{
	class MathUtils
	{
	public:
		template<class T> static inline T clamp(T min, T max, T val)
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
	};

}

#endif /* MATH_UTILS_H */