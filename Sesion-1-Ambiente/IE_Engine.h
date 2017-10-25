#pragma once

#include <string>
#include "dtGameData.h"
#include "IE_Import.h"
#include "IE_World.h"

class IE_Engine
{
public:
	IE_Engine();
	~IE_Engine() {}

	void Initialize(std::string levelId);
	void Update(float dt);
	void Draw(float dt);

private:
	dtLevel mLevelData;
	IE_World* mWorld;
};

