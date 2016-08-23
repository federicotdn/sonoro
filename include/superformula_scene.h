#ifndef SUPERFORMULA_H
#define SUPERFORMULA_H

#include <scene.h>

namespace so
{
	class SuperformulaScene : public Scene
	{
	public:
		SuperformulaScene(Sonoro &app);
		~SuperformulaScene();

		int initialize() override;
		void update() override;
		void draw() override;

		std::string sceneName() override { return "Superformula Scene"; }

	private:
	};
}

#endif /* SUPERFORMULA_H */