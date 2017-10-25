#pragma once

#include <string>
#include <vector>

struct dtPlayer {
	int InitTileX;
	int InitTileY;
};

struct dtWorld {
	int TileSize;
	std::vector<std::vector<int>> Rows;
};

struct dtLevel {
	std::string Id;
	
	dtPlayer PlayerData;
	dtWorld WorldData;
};

struct dtGameData {
	std::vector<dtLevel> Levels;

	dtLevel GetLevel(std::string id) {
		dtLevel result;

		for (int i = 0, length = Levels.size(); i < length; i++) {
			if (Levels.at(i).Id == id) {
				result = Levels.at(i);
				break;
			}
		}

		return result;
	}
};