#pragma once

#include "dtGameData.h"
#include "IE_EnumTile.h"
#include <UPCFramework\NSpriteBatch.h>

class IE_Engine;

class IE_World
{
public:
	IE_World(IE_Engine* engine);
	~IE_World() {
		mEngine = nullptr;
	}

	void Initialize(dtWorld worldData);
	void Update(float dt);
	void Draw(float dt);

private:
	IE_Engine* mEngine;
	dtWorld mWorldData;

	NSpriteBatch mSpriteBatch;
};

