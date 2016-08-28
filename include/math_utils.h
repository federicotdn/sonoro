#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <cstdlib>

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

		template<class T> static inline T lerp(T a, T b, T t)
		{
			t = clamp((T)0, (T)1, t);
			return a + (b - a) * t;
		}

		static float rand()
		{
			return (float)std::rand() / (RAND_MAX + 1);
		}
	};

}

#endif /* MATH_UTILS_H */