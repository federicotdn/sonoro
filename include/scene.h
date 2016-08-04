#ifndef SCENE_H
#define SCENE_H

#include <sonoro.h>

namespace so
{
	class Scene
	{
	public:
		Scene(Sonoro &app) : m_app(app) 
		{}

		virtual ~Scene()
		{ }

		virtual int initialize() { return 0; }
		virtual void update() = 0;
		virtual void draw() = 0;
		virtual std::string sceneName() = 0;
	protected:
		Sonoro &m_app;
	};
}

#endif /* SCENE_H */