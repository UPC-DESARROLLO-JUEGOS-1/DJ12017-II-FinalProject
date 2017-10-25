#pragma once

#include <UPCFramework\NBaseScene.h>
#include "IE_Engine.h"

class GameSceneIsoEngine : public NBaseScene
{
public:
	GameSceneIsoEngine(NSceneGraph* sceneGraph) :
		NBaseScene::NBaseScene(sceneGraph) {}
	~GameSceneIsoEngine() {

	}

	void Initialize();
	void Update(float dt);
	void Draw(float dt);

	void OnKeyDown(SDL_Keycode key);
	void OnKeyUp(SDL_Keycode key);

private:
	IE_Engine mIE_Engine;
};

