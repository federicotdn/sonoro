#include <asset_loader.h>

#include <fstream>
#include <sstream>

using namespace so;

AssetLoader::~AssetLoader()
{
}

AssetLoader::AssetLoader()
{
}

bool AssetLoader::readTextFile(std::string path, std::string &contents)
{
	std::ifstream in(path);
	if (!in.good())
	{
		return false;
	}

	std::stringstream buffer;
	buffer << in.rdbuf();
	contents.append(buffer.str());
	return true;
}

bool AssetLoader::loadOBJFile(std::string path, Asset3DModel &modelData)
{
	std::string contents;
	bool ok = readTextFile(path, contents);
	if (!ok)
	{
		return false;
	}

	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> uvs;

	std::istringstream iss(contents);
	for (std::string line; std::getline(iss, line); )
	{
		if (line.front() == '#' || line.empty())
		{
			continue;
		}

		std::istringstream lineStream(line);
		std::string type;

		lineStream >> type;

		//TODO: Normals and UVs

		if (type == "v")
		{
			float x, y, z;
			lineStream >> x >> y >> z;
			positions.emplace_back(x, y, z);
		}
		else if (type == "f")
		{
			std::string	s[3];
			int indices[3];
			int j = 0;

			lineStream >> s[0] >> s[1] >> s[2];
			for (std::string point : s)
			{
				// variable point is one of the following:
				// v1
				// v1/vt1
				// v1/vt1/vn1
				// v1//vn1

				std::istringstream info(point);
				info >> indices[j++];
			}

			for (j = 0; j < 3; j++)
			{
				glm::vec3 pos = positions[indices[j] - 1];
				modelData.vertices.push_back(pos);
			}
		}
	}

	return true;
}

Asset3DModel AssetLoader::getFullscreenQuad()
{
	Asset3DModel asset;

	asset.vertices.emplace_back(-1.0f, 1.0f, 0.0f);
	asset.vertices.emplace_back(-1.0f, -1.0f, 0.0f);
	asset.vertices.emplace_back(1.0f, -1.0f, 0.0f);

	asset.vertices.emplace_back(-1.0f, 1.0f, 0.0f);
	asset.vertices.emplace_back(1.0f, -1.0f, 0.0f);
	asset.vertices.emplace_back(1.0f, 1.0f, 0.0f);

	asset.uvs.emplace_back(0.0f, 1.0f);
	asset.uvs.emplace_back(0.0f, 0.0f);
	asset.uvs.emplace_back(1.0f, 0.0f);

	asset.uvs.emplace_back(0.0f, 1.0f);
	asset.uvs.emplace_back(1.0f, 0.0f);
	asset.uvs.emplace_back(1.0f, 1.0f);

	return asset;
}
