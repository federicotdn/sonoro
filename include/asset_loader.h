#ifndef ASSET_LOADER_H
#define ASSET_LOADER_H

#include <string>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

namespace so
{
	struct Asset3DModel {
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> uvs;
	};

	class AssetLoader
	{
	public:
		static AssetLoader &getInstance()
		{
			static AssetLoader instance;
			return instance;
		}

		bool readTextFile(std::string path, std::string &contents);
		bool loadOBJFile(std::string path, Asset3DModel &modelData);
		Asset3DModel getFullscreenQuad();

		~AssetLoader();

	private:
		AssetLoader();
	};
}

#endif /* ASSET_LOADER_H */