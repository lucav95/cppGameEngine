#pragma once
#include <string>
#include "GameEngine.h"


class FileSystem {
public:
	static void loadAssets(const std::string& configPath, Assets& assets, std::string& shader);
	static void loadDialog(const std::string& dialogPath, DialogMap& dialogMap);
	static void loadGameMap(const std::string& gameMapPath, EntityManager& entities, Assets& assets);
};