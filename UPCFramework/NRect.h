#pragma once

#include <GL\glew.h>

struct NRect
{
	float x;
	float y;
	float w;
	float h;

	NRect static CreateRect(float x, float y, float w, float h)
	{
		NRect rect;
		rect.x = x;
		rect.y = y;
		rect.w = w;
		rect.h = h;

		return rect;
	}
};