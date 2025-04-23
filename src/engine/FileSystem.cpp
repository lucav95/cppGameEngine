#include "FileSystem.h"
#include <json.hpp>
#include <fstream>
#include <sstream>

void FileSystem::loadAssets(const std::string& configPath, Assets& assets, std::string& shader) {
	std::ifstream fin(configPath);
	nlohmann::json json = nlohmann::json::parse(fin);
	fin.close();

	for (auto& j : json) {
		if (j["type"] == "TEX") {
			assets.addTexture(j["name"], j["path"], j["repeated"].get<bool>());
		}
		if (j["type"] == "ANI") {
			assets.addTexture(j["name"], j["path"], false);
			Animation animation = Animation(
				j["name"],
				assets.getTexture(j["name"]),
				j["frameCount"].get<int>(),
				j["speed"].get<int>());
			assets.addAnimation(j["name"], animation);
		}
		if (j["type"] == "FON") {
			assets.addFont(j["name"], j["path"]);
		}
		if (j["type"] == "SHA") {
			shader = j["path"].get<std::string>();
		}
	}
}

void FileSystem::loadDialog(const std::string& dialogPath, DialogMap& dialogMap) {
	std::ifstream fin(dialogPath);

	std::string line;
	while (std::getline(fin, line)) {

		std::stringstream stream(line);
		std::vector<std::string> row;
		std::string cell;
		while (std::getline(stream, cell, ';')) {
			row.push_back(cell);
		}
		dialogMap[row.at(0)] = row.at(1);
	}
	fin.close();
}

void FileSystem::loadGameMap(const std::string& gameMapPath, EntityManager& entities, Assets& assets) {
	std::fstream fin(gameMapPath);
	nlohmann::json json = nlohmann::json::parse(fin);
	fin.close();

	for (auto& j : json) {
		if (j["type"] == "RECT") {

			auto e = entities.addEntity(j["name"]);

			if (j["texture"] != "none") {
				auto& graphics = e->addComponent<CGraphics>(j["texture"]);
				graphics.repeated = assets.getTexture(j["texture"]).isRepeated();
				graphics.background = j["background"];
			}
			auto& transform = e->addComponent<CTransform>(
				Vec2(j["worldPos"]["x"].get<float>(), j["worldPos"]["y"].get<float>()),
				Vec2(0.0, 0.0),
				0,
				Vec2(j["size"]["w"].get<float>(), j["size"]["h"].get<float>()));
			transform.scale = Vec2(j["scale"]["w"].get<float>(), j["scale"]["h"].get<float>());
			transform.zIndex = j["zIndex"];
			if (j["boundingBox"]["w"].get<float>() > 0 && j["boundingBox"]["h"].get<float>() > 0) {
				e->addComponent<CBoundingBox>(
					Vec2(j["boundingBox"]["w"].get<float>(), j["boundingBox"]["h"].get<float>()),
					Vec2(j["boundingBox"]["relativeX"].get<float>(), j["boundingBox"]["relativeY"].get<float>()));
			}
		}
		if (j["type"] == "TEX_MAP") {

			auto e = entities.addEntity(j["name"]);

			std::vector<CGraphics::Tile> tiles;
			CGraphics::Tile tile;
			for (auto& t : j["tiles"]) {
				tile.textureSize = Vec2(t["textureSize"]["w"].get<float>(), t["textureSize"]["h"].get<float>());
				tile.texturePos = Vec2(t["texturePos"]["x"].get<float>(), t["texturePos"]["y"].get<float>());
				tile.posIndex = Vec2(t["posIndex"]["x"].get<float>(), t["posIndex"]["y"].get<float>());
				tiles.push_back(tile);
			}
			auto& graphics = e->addComponent<CGraphics>(j["texture"], tiles);
			graphics.textureMapStartingPos = Vec2(j["textureMapStartingPos"]["x"].get<float>(), j["textureMapStartingPos"]["y"].get<float>());
			graphics.background = j["background"];
			auto& transform = e->addComponent<CTransform>();
			transform.scale = Vec2(j["scale"]["w"].get<float>(), j["scale"]["h"].get<float>());
			transform.zIndex = j["zIndex"];
		}
	}
}