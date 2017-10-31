#include "IE_World.h"
#include "IE_Engine.h"
#include <UPCFramework\NRect.h>
#include <UPCFramework\NFramework.h>

#define PI 3.14159265358979

IE_World::IE_World(IE_Engine* engine)
{
	mEngine = engine;

	mSpriteBatch.Initialize("Content/Sprites/world_tiles.png");
	mSpriteBatch.SetRenderCamera(NFramework::GET_FRAMEWORK()->GetCamera());
	//mSpriteBatch.SetScaleX(0.5f);
	//mSpriteBatch.SetScaleY(0.5f);
	mSpriteBatch.SetRotationZ(PI / 4.0);
	mSpriteBatch.SetScaleY(0.5f);
	mSpriteBatch.SetX(200);
	mSpriteBatch.SetY(100);
}

void IE_World::Initialize(dtWorld worldData) {
	mWorldData = worldData;

	float tileSize = worldData.TileSize;

	float rotation = 0;
	Vector2 scale = Vector2::Create(1, 1);
	NRect rectUV = NRect::CreateRect(1 * tileSize, 1 * tileSize, tileSize, tileSize);
	//mSpriteBatch.addQuad(0, 0, rotation, rectUV, scale);
	//mSpriteBatch.addQuad(tileSize, 0, rotation, rectUV, scale);

	for (int rows = 0, rLength = worldData.Rows.size(); rows < rLength; rows++) {
		for (int cols = 0, cLength = worldData.Rows.at(0).size(); cols < cLength; cols++) {
			int tileId = worldData.Rows.at(rows).at(cols);

			//float positionX = (float)(cols - rows) * tileSize;
			//float positionY = ((float)(cols + rows) * tileSize) / 2.0;

			float positionX = cols * tileSize;
			float positionY = rows * tileSize;

			switch (tileId) {
			case IE_EnumTile::Grass:
				mSpriteBatch.addQuad(positionX, positionY, rotation, rectUV, scale);
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