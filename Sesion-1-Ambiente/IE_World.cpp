#include "IE_World.h"
#include "IE_Engine.h"
#include <UPCFramework\NRect.h>
#include <UPCFramework\NFramework.h>

IE_World::IE_World(IE_Engine* engine)
{
	mEngine = engine;

	mSpriteBatch.Initialize("Content/Sprites/world_tiles.png");
	mSpriteBatch.SetRenderCamera(NFramework::GET_FRAMEWORK()->GetCamera());
}

void IE_World::Initialize(dtWorld worldData) {
	mWorldData = worldData;

	int tileSize = worldData.TileSize;

	for (int rows = 0, rLength = worldData.Rows.size(); rows < rLength; rows++) {
		for (int cols = 0, cLength = worldData.Rows.at(0).size(); cols < cLength; cols++) {
			int tileId = worldData.Rows.at(rows).at(cols);

			int positionX = cols * tileSize;
			int positionY = rows * tileSize;

			switch (tileId) {
			case IE_EnumTile::Grass:
				NRect rectUV = NRect::CreateRect(1 * tileSize,1 * tileSize, tileSize, tileSize);

				mSpriteBatch.addQuad(positionX, positionY, tileSize, tileSize, rectUV);
				break;
			default:
				break;
			}
		}
	}
}

void IE_World::Update(float dt) {
	mSpriteBatch.Update(dt);
}

void IE_World::Draw(float dt) {
	mSpriteBatch.Draw(dt);
}