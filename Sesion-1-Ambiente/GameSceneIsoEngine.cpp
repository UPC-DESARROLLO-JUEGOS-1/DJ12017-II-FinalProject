#include "GameSceneIsoEngine.h"

void GameSceneIsoEngine::Initialize() {
	
	mIE_Engine.Initialize("0");
}

void GameSceneIsoEngine::OnKeyDown(SDL_Keycode key) {

}

void GameSceneIsoEngine::OnKeyUp(SDL_Keycode key) {

}

void GameSceneIsoEngine::Update(float dt) {
	mIE_Engine.Update(dt);
}

void GameSceneIsoEngine::Draw(float dt) {
	mIE_Engine.Draw(dt);
}