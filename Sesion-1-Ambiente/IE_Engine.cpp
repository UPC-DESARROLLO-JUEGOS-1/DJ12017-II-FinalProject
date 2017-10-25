#include "IE_Engine.h"

IE_Engine::IE_Engine() {
	mWorld = new IE_World(this);
}

void IE_Engine::Initialize(std::string levelId) {
	dtGameData gameData = IE_Import::Load("Content/Xml/TB2D_WorldConfig.xml");
	mLevelData = gameData.GetLevel(levelId);

	mWorld->Initialize(mLevelData.WorldData);
}

void IE_Engine::Update(float dt) {
	mWorld->Update(dt);
}

void IE_Engine::Draw(float dt) {
	mWorld->Draw(dt);
}