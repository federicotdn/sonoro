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
