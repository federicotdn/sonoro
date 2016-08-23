#ifndef ASSET_LOADER_H
#define ASSET_LOADER_H

#include <string>
#include <vector>
#include <glm/vec3.hpp>

namespace so
{
	struct Asset3DModel {
		std::vector<glm::vec3> m_vertices;
		std::vector<glm::vec3> m_normals;
		std::vector<glm::vec2> m_uvs;
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

		~AssetLoader();

	private:
		AssetLoader();
	};
}

#endif /* ASSET_LOADER_H */