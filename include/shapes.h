#ifndef SHAPES_H
#define SHAPES_H

namespace so
{
	/* Replace SDL_Rect and SDL_Point */

	//TODO: Delet this

	struct Point
	{
		Point() : 
			x(0), 
			y(0),
			z(0)
		{}

		int x, y, z;
	};

	struct Rect
	{
		Rect() :
			x(0),
			y(0),
			w(0),
			h(0)
		{}

		int x, y, w, h;
	};
}

#endif /* SHAPES_H */