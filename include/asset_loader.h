#ifndef ASSET_LOADER_H
#define ASSET_LOADER_H

#include <string>

namespace so
{
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