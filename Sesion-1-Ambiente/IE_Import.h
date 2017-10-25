#pragma once

#include <string>
#include <vector>
#include <UPCFramework\pugixml.hpp>
#include <UPCFramework\NCommon.h>
#include <UPCFramework\FatalError.h>

#include "dtGameData.h"

class IE_Import
{
public:

	dtGameData static Load(std::string xmlPath) {
		dtGameData result;

		// XML Sample
		pugi::xml_document xDoc;
		pugi::xml_parse_result xmlResult = xDoc.load_file(xmlPath.c_str());

		if (xmlResult.status == pugi::status_ok) {
			pugi::xml_node xData = xDoc.child("data");
			pugi::xml_node xLevel = xData.child("level");
			pugi::xml_node xPlayer = xLevel.child("player");
			pugi::xml_node xWorld = xLevel.child("world");

			while (xLevel != nullptr) {
				dtLevel dataLevel;
				dataLevel.Id = std::string(xLevel.attribute("id").as_string());
				dataLevel.PlayerData.InitTileX = xPlayer.attribute("ix").as_int();
				dataLevel.PlayerData.InitTileY = xPlayer.attribute("iy").as_int();
				dataLevel.WorldData.TileSize = xWorld.attribute("tileSize").as_int();

				pugi::xml_node xLevelRow = xWorld.child("row");

				while (xLevelRow != nullptr) {
					std::string xAttr_LevelRowData = std::string(xLevelRow.attribute("data").as_string());
					std::vector<std::string> splittedLevelRow = NCommon::SplitString(xAttr_LevelRowData, ',');
					std::vector<int> outputLevelRow = NCommon::CastVecToIntPtr(splittedLevelRow);

					// push it
					dataLevel.WorldData.Rows.push_back(outputLevelRow);

					// Next node
					xLevelRow = xLevelRow.next_sibling();
				}

				// add it to the levels in game data
				result.Levels.push_back(dataLevel);

				// Next node
				xLevel = xLevel.next_sibling();
			}
		}
		else {
			std::string errorMessage = "IE_Import::Load Error: " +
				std::string(xmlResult.description());

			FatalError(errorMessage);
		}

		return result;
	}
};

