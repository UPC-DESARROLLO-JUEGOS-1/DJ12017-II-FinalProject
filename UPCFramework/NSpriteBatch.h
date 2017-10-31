#pragma once

#include "NImageContent.h"
#include "NDrawable2D.h"
#include "dtSpriteVertex.h"
#include "NRect.h"

#include <string>

	class NSpriteBatch : public NDrawable2D
{
public:
	NSpriteBatch() { }
	~NSpriteBatch() { }

	void Initialize(const std::string path);
	void Initialize(const std::string path, NRect rect);
	void Draw(float dt);
	void addQuad(NRect rect);
	void addQuad(NRect rect, NRect UV);
	void addQuad(NRect rect, NColor color);
	void addQuad(NRect rect, NColor color, NRect UV);
	void addQuad(float x, float y);
	void addQuad(float x, float y, NRect UV);
	void addQuad(float x, float y, float rotation, NRect UV, Vector2 scale);
	void addQuad(float x, float y, NColor color);
	void addQuad(float x, float y, NColor color, NRect UV);
	void addQuad(float x, float y, float width, float height);
	void addQuad(float x, float y, float width, float height, NRect UV);
	void addQuad(float x, float y, float width, float height, NColor color);
	void addQuad(float x, float y, float width, float height, NColor color, NRect UV);
protected:
	NRect TextureBox;
	NRect UVBox;

	std::vector<dtSpriteVertex> vertexData;

	NImageContent* mContent;
};
