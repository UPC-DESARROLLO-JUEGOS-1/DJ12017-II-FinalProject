#include "NSpriteBatch.h"
#include "NFramework.h"
#include "NSpriteShader.h"

void NSpriteBatch::Initialize(const std::string path)
{
	NDrawable2D::Initialize(0, 0);

	NFramework* framework = NFramework::GET_FRAMEWORK();
	NContentManagement* contentManagment = framework->GetContentManagement();
	NShaderManagement* shaderManagment = framework->GetShaderManagment();

	const std::string baseShaderPath = "Content/Shaders/SpriteShader";
	mCurrentShader = (NBaseShader*)shaderManagment->LoadAndGetShader<NSpriteShader>(baseShaderPath);

	mContent = contentManagment->LoadAndGetContent<NImageContent>(path);
	
	TextureBox.x = 0;
	TextureBox.y = 0;
	TextureBox.w = (float)mContent->GetWidth();
	TextureBox.h = (float)mContent->GetHeight();

	UVBox.x = TextureBox.x / mContent->GetWidth();
	UVBox.y = TextureBox.y / mContent->GetHeight();
	UVBox.w = (TextureBox.x + TextureBox.w) / mContent->GetWidth();
	UVBox.h = (TextureBox.y + TextureBox.h) / mContent->GetHeight();
}

void NSpriteBatch::Initialize(const std::string path, NRect rect)
{
	NDrawable2D::Initialize(0, 0);

	NFramework* framework = NFramework::GET_FRAMEWORK();
	NContentManagement* contentManagment = framework->GetContentManagement();
	NShaderManagement* shaderManagment = framework->GetShaderManagment();

	const std::string baseShaderPath = "Content/Shaders/SpriteShader";
	mCurrentShader = (NBaseShader*)shaderManagment->LoadAndGetShader<NSpriteShader>(baseShaderPath);

	mContent = contentManagment->LoadAndGetContent<NImageContent>(path);

	TextureBox.x = rect.x;
	TextureBox.y = rect.y;
	TextureBox.w = rect.w;
	TextureBox.h = rect.h;

	UVBox.x = TextureBox.x / mContent->GetWidth();
	UVBox.y = TextureBox.y / mContent->GetHeight();
	UVBox.w = (TextureBox.x + TextureBox.w) / mContent->GetWidth();
	UVBox.h = (TextureBox.y + TextureBox.h) / mContent->GetHeight();
}

void NSpriteBatch::Draw(float dt)
{
	// Blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	mCurrentShader->Use();

	GLuint mvpLocation = mCurrentShader->GetUniformLocation("wvp");
	glm::mat4 cameraMatrix = mRenderCamera->GetResultMatrix();
	glm::mat4 resultMatrix = cameraMatrix * mWorldMatrix;

	glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &(resultMatrix[0][0]));

	// Activo la textura
	glActiveTexture(GL_TEXTURE0);
	// Hago binding de la textura en GPU
	glBindTexture(GL_TEXTURE_2D, mContent->GetTextureId());

	glBindBuffer(GL_ARRAY_BUFFER, mVBO_ID);

	// Position
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
		sizeof(dtSpriteVertex),
		(void*)offsetof(dtSpriteVertex, dtSpriteVertex::Position));
	// Color
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE,
		sizeof(dtSpriteVertex),
		(void*)offsetof(dtSpriteVertex, dtSpriteVertex::Color));
	// UV
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE,
		sizeof(dtSpriteVertex),
		(void*)offsetof(dtSpriteVertex, dtSpriteVertex::UV));

	// Esto dibuja en Frame Buffer
	glDrawArrays(GL_TRIANGLES, 0, vertexData.size());

	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	mCurrentShader->Unuse();
}

void NSpriteBatch::addQuad(NRect rect)
{
	dtSpriteVertex vertex[6];
	vertex[0].SetPosition(rect.x + rect.w, rect.y + rect.h);
	vertex[0].SetColor(255, 255, 255, 255);
	vertex[0].SetUV(UVBox.w, UVBox.h);
	vertexData.push_back(vertex[0]);

	vertex[1].SetPosition(rect.x, rect.y + rect.h);
	vertex[1].SetColor(255, 255, 255, 255);
	vertex[1].SetUV(UVBox.x, UVBox.h);
	vertexData.push_back(vertex[1]);

	vertex[2].SetPosition(rect.x, rect.y);
	vertex[2].SetColor(255, 255, 255, 255);
	vertex[2].SetUV(UVBox.x, UVBox.y);
	vertexData.push_back(vertex[2]);

	// seg triangulo
	vertex[3].SetPosition(rect.x, rect.y);
	vertex[3].SetColor(255, 255, 255, 255);
	vertex[3].SetUV(UVBox.x, UVBox.y);
	vertexData.push_back(vertex[3]);

	vertex[4].SetPosition(rect.x + rect.w, rect.y);
	vertex[4].SetColor(255, 255, 255, 255);
	vertex[4].SetUV(UVBox.w, UVBox.y);
	vertexData.push_back(vertex[4]);

	vertex[5].SetPosition(rect.x + rect.w, rect.y + rect.h);
	vertex[5].SetColor(255, 255, 255, 255);
	vertex[5].SetUV(UVBox.w, UVBox.h);
	vertexData.push_back(vertex[5]);

	int size = sizeof(dtSpriteVertex) * vertexData.size();

	glBindBuffer(GL_ARRAY_BUFFER, mVBO_ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertexData.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void NSpriteBatch::addQuad(NRect rect, NRect UV)
{
	UVBox.x = UV.x / mContent->GetWidth();
	UVBox.y = UV.y / mContent->GetHeight();
	UVBox.w = (UV.x + UV.w) / mContent->GetWidth();
	UVBox.h = (UV.y + UV.h) / mContent->GetHeight();

	dtSpriteVertex vertex[6];
	vertex[0].SetPosition(rect.x + rect.w, rect.y + rect.h);
	vertex[0].SetColor(255, 255, 255, 255);
	vertex[0].SetUV(UVBox.w, UVBox.h);
	vertexData.push_back(vertex[0]);

	vertex[1].SetPosition(rect.x, rect.y + rect.h);
	vertex[1].SetColor(255, 255, 255, 255);
	vertex[1].SetUV(UVBox.x, UVBox.h);
	vertexData.push_back(vertex[1]);

	vertex[2].SetPosition(rect.x, rect.y);
	vertex[2].SetColor(255, 255, 255, 255);
	vertex[2].SetUV(UVBox.x, UVBox.y);
	vertexData.push_back(vertex[2]);

	// seg triangulo
	vertex[3].SetPosition(rect.x, rect.y);
	vertex[3].SetColor(255, 255, 255, 255);
	vertex[3].SetUV(UVBox.x, UVBox.y);
	vertexData.push_back(vertex[3]);

	vertex[4].SetPosition(rect.x + rect.w, rect.y);
	vertex[4].SetColor(255, 255, 255, 255);
	vertex[4].SetUV(UVBox.w, UVBox.y);
	vertexData.push_back(vertex[4]);

	vertex[5].SetPosition(rect.x + rect.w, rect.y + rect.h);
	vertex[5].SetColor(255, 255, 255, 255);
	vertex[5].SetUV(UVBox.w, UVBox.h);
	vertexData.push_back(vertex[5]);

	int size = sizeof(dtSpriteVertex) * vertexData.size();

	glBindBuffer(GL_ARRAY_BUFFER, mVBO_ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertexData.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void NSpriteBatch::addQuad(NRect rect, NColor color)
{
	dtSpriteVertex vertex[6];
	vertex[0].SetPosition(rect.x + rect.w, rect.y + rect.h);
	vertex[0].SetColor(color.R, color.G, color.B, color.A);
	vertex[0].SetUV(UVBox.w, UVBox.h);
	vertexData.push_back(vertex[0]);

	vertex[1].SetPosition(rect.x, rect.y + rect.h);
	vertex[1].SetColor(color.R, color.G, color.B, color.A);
	vertex[1].SetUV(UVBox.x, UVBox.h);
	vertexData.push_back(vertex[1]);

	vertex[2].SetPosition(rect.x, rect.y);
	vertex[2].SetColor(color.R, color.G, color.B, color.A);
	vertex[2].SetUV(UVBox.x, UVBox.y);
	vertexData.push_back(vertex[2]);

	// seg triangulo
	vertex[3].SetPosition(rect.x, rect.y);
	vertex[3].SetColor(color.R, color.G, color.B, color.A);
	vertex[3].SetUV(UVBox.x, UVBox.y);
	vertexData.push_back(vertex[3]);

	vertex[4].SetPosition(rect.x + rect.w, rect.y);
	vertex[4].SetColor(color.R, color.G, color.B, color.A);
	vertex[4].SetUV(UVBox.w, UVBox.y);
	vertexData.push_back(vertex[4]);

	vertex[5].SetPosition(rect.x + rect.w, rect.y + rect.h);
	vertex[5].SetColor(color.R, color.G, color.B, color.A);
	vertex[5].SetUV(UVBox.w, UVBox.h);
	vertexData.push_back(vertex[5]);

	int size = sizeof(dtSpriteVertex) * vertexData.size();

	glBindBuffer(GL_ARRAY_BUFFER, mVBO_ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertexData.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void NSpriteBatch::addQuad(NRect rect, NColor color, NRect UV)
{
	UVBox.x = UV.x / mContent->GetWidth();
	UVBox.y = UV.y / mContent->GetHeight();
	UVBox.w = (UV.x + UV.w) / mContent->GetWidth();
	UVBox.h = (UV.y + UV.h) / mContent->GetHeight();

	dtSpriteVertex vertex[6];
	vertex[0].SetPosition(rect.x + rect.w, rect.y + rect.h);
	vertex[0].SetColor(color.R, color.G, color.B, color.A);
	vertex[0].SetUV(UVBox.w, UVBox.h);
	vertexData.push_back(vertex[0]);

	vertex[1].SetPosition(rect.x, rect.y + rect.h);
	vertex[1].SetColor(color.R, color.G, color.B, color.A);
	vertex[1].SetUV(UVBox.x, UVBox.h);
	vertexData.push_back(vertex[1]);

	vertex[2].SetPosition(rect.x, rect.y);
	vertex[2].SetColor(color.R, color.G, color.B, color.A);
	vertex[2].SetUV(UVBox.x, UVBox.y);
	vertexData.push_back(vertex[2]);

	// seg triangulo
	vertex[3].SetPosition(rect.x, rect.y);
	vertex[3].SetColor(color.R, color.G, color.B, color.A);
	vertex[3].SetUV(UVBox.x, UVBox.y);
	vertexData.push_back(vertex[3]);

	vertex[4].SetPosition(rect.x + rect.w, rect.y);
	vertex[4].SetColor(color.R, color.G, color.B, color.A);
	vertex[4].SetUV(UVBox.w, UVBox.y);
	vertexData.push_back(vertex[4]);

	vertex[5].SetPosition(rect.x + rect.w, rect.y + rect.h);
	vertex[5].SetColor(color.R, color.G, color.B, color.A);
	vertex[5].SetUV(UVBox.w, UVBox.h);
	vertexData.push_back(vertex[5]);

	int size = sizeof(dtSpriteVertex) * vertexData.size();

	glBindBuffer(GL_ARRAY_BUFFER, mVBO_ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertexData.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void NSpriteBatch::addQuad(float x, float y)
{
	dtSpriteVertex vertex[6];
	vertex[0].SetPosition(x + TextureBox.w, y + TextureBox.h);
	vertex[0].SetColor(255, 255, 255, 255);
	vertex[0].SetUV(UVBox.w, UVBox.h);
	vertexData.push_back(vertex[0]);

	vertex[1].SetPosition(x, y + TextureBox.h);
	vertex[1].SetColor(255, 255, 255, 255);
	vertex[1].SetUV(UVBox.x, UVBox.h);
	vertexData.push_back(vertex[1]);

	vertex[2].SetPosition(x, y);
	vertex[2].SetColor(255, 255, 255, 255);
	vertex[2].SetUV(UVBox.x, UVBox.y);
	vertexData.push_back(vertex[2]);

	// seg triangulo
	vertex[3].SetPosition(x, y);
	vertex[3].SetColor(255, 255, 255, 255);
	vertex[3].SetUV(UVBox.x, UVBox.y);
	vertexData.push_back(vertex[3]);

	vertex[4].SetPosition(x + TextureBox.w, y);
	vertex[4].SetColor(255, 255, 255, 255);
	vertex[4].SetUV(UVBox.w, UVBox.y);
	vertexData.push_back(vertex[4]);

	vertex[5].SetPosition(x + TextureBox.w, y + TextureBox.h);
	vertex[5].SetColor(255, 255, 255, 255);
	vertex[5].SetUV(UVBox.w, UVBox.h);
	vertexData.push_back(vertex[5]);

	int size = sizeof(dtSpriteVertex) * vertexData.size();

	glBindBuffer(GL_ARRAY_BUFFER, mVBO_ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertexData.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void NSpriteBatch::addQuad(float x, float y, NRect UV)
{
	UVBox.x = UV.x / mContent->GetWidth();
	UVBox.y = UV.y / mContent->GetHeight();
	UVBox.w = (UV.x + UV.w) / mContent->GetWidth();
	UVBox.h = (UV.y + UV.h) / mContent->GetHeight();

	dtSpriteVertex vertex[6];
	vertex[0].SetPosition(x + UV.w, y + UV.h);
	vertex[0].SetColor(255, 255, 255, 255);
	vertex[0].SetUV(UVBox.w, UVBox.h);
	vertexData.push_back(vertex[0]);

	vertex[1].SetPosition(x, y + UV.h);
	vertex[1].SetColor(255, 255, 255, 255);
	vertex[1].SetUV(UVBox.x, UVBox.h);
	vertexData.push_back(vertex[1]);

	vertex[2].SetPosition(x, y);
	vertex[2].SetColor(255, 255, 255, 255);
	vertex[2].SetUV(UVBox.x, UVBox.y);
	vertexData.push_back(vertex[2]);

	// seg triangulo
	vertex[3].SetPosition(x, y);
	vertex[3].SetColor(255, 255, 255, 255);
	vertex[3].SetUV(UVBox.x, UVBox.y);
	vertexData.push_back(vertex[3]);

	vertex[4].SetPosition(x + UV.w, y);
	vertex[4].SetColor(255, 255, 255, 255);
	vertex[4].SetUV(UVBox.w, UVBox.y);
	vertexData.push_back(vertex[4]);

	vertex[5].SetPosition(x + UV.w, y + UV.h);
	vertex[5].SetColor(255, 255, 255, 255);
	vertex[5].SetUV(UVBox.w, UVBox.h);
	vertexData.push_back(vertex[5]);

	int size = sizeof(dtSpriteVertex) * vertexData.size();

	glBindBuffer(GL_ARRAY_BUFFER, mVBO_ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertexData.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void NSpriteBatch::addQuad(float x, float y, float rotation, NRect UV, Vector2 scale)
{
	UVBox.x = UV.x / mContent->GetWidth();
	UVBox.y = UV.y / mContent->GetHeight();
	UVBox.w = (UV.x + UV.w) / mContent->GetWidth();
	UVBox.h = (UV.y + UV.h) / mContent->GetHeight();

	float tx = UV.w;
	float ty = UV.h;

	float cosAngle = cosf(rotation);
	float sinAngle = sinf(rotation);

	float x_0 = x + UV.w;
	float x_1 = x;
	float x_2 = x;
	float x_3 = x + UV.w;

	float y_0 = y + UV.h;
	float y_1 = y + UV.h;
	float y_2 = y;
	float y_3 = y;

	float centerX = (x_0 + x_1 + x_2 + x_3) / 4.0;
	float centerY = (y_0 + y_1 + y_2 + y_3) / 4.0;

	x_0 -= centerX;
	x_1 -= centerX;
	x_2 -= centerX;
	x_3 -= centerX;

	y_0 -= centerY;
	y_1 -= centerY;
	y_2 -= centerY;
	y_3 -= centerY;

	float _x_0 = x_0;
	float _x_1 = x_1;
	float _x_2 = x_2;
	float _x_3 = x_3;

	float _y_0 = y_0;
	float _y_1 = y_1;
	float _y_2 = y_2;
	float _y_3 = y_3;

	x_0 = (cosAngle * _x_0 - sinAngle * _y_0);
	y_0 = (sinAngle * _x_0 + cosAngle * _y_0);

	x_1 = (cosAngle * _x_1 - sinAngle * _y_1);
	y_1 = (sinAngle * _x_1 + cosAngle * _y_1);

	x_2 = (cosAngle * _x_2 - sinAngle * _y_2);
	y_2 = (sinAngle * _x_2 + cosAngle * _y_2);

	x_3 = (cosAngle * _x_3 - sinAngle * _y_3);
	y_3 = (sinAngle * _x_3 + cosAngle * _y_3);

	x_0 += centerX;
	x_1 += centerX;
	x_2 += centerX;
	x_3 += centerX;

	y_0 += centerY;
	y_1 += centerY;
	y_2 += centerY;
	y_3 += centerY;

	dtSpriteVertex vertex[6];
	vertex[0].SetPosition(x_0, y_0);
	vertex[0].SetColor(255, 255, 255, 255);
	vertex[0].SetUV(UVBox.w, UVBox.h);
	vertexData.push_back(vertex[0]);

	vertex[1].SetPosition(x_1, y_1);
	vertex[1].SetColor(255, 255, 255, 255);
	vertex[1].SetUV(UVBox.x, UVBox.h);
	vertexData.push_back(vertex[1]);

	vertex[2].SetPosition(x_2, y_2);
	vertex[2].SetColor(255, 255, 255, 255);
	vertex[2].SetUV(UVBox.x, UVBox.y);
	vertexData.push_back(vertex[2]);

	// seg triangulo
	vertex[3].SetPosition(x_2, y_2);
	vertex[3].SetColor(255, 255, 255, 255);
	vertex[3].SetUV(UVBox.x, UVBox.y);
	vertexData.push_back(vertex[3]);

	vertex[4].SetPosition(x_3, y_3);
	vertex[4].SetColor(255, 255, 255, 255);
	vertex[4].SetUV(UVBox.w, UVBox.y);
	vertexData.push_back(vertex[4]);

	vertex[5].SetPosition(x_0, y_0);
	vertex[5].SetColor(255, 255, 255, 255);
	vertex[5].SetUV(UVBox.w, UVBox.h);
	vertexData.push_back(vertex[5]);

	int size = sizeof(dtSpriteVertex) * vertexData.size();

	glBindBuffer(GL_ARRAY_BUFFER, mVBO_ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertexData.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void NSpriteBatch::addQuad(float x, float y, NColor color)
{
	dtSpriteVertex vertex[6];
	vertex[0].SetPosition(x + TextureBox.w, y + TextureBox.h);
	vertex[0].SetColor(color.R, color.G, color.B, color.A);
	vertex[0].SetUV(UVBox.w, UVBox.h);
	vertexData.push_back(vertex[0]);

	vertex[1].SetPosition(x, y + TextureBox.h);
	vertex[1].SetColor(color.R, color.G, color.B, color.A);
	vertex[1].SetUV(UVBox.x, UVBox.h);
	vertexData.push_back(vertex[1]);

	vertex[2].SetPosition(x, y);
	vertex[2].SetColor(color.R, color.G, color.B, color.A);
	vertex[2].SetUV(UVBox.x, UVBox.y);
	vertexData.push_back(vertex[2]);

	// seg triangulo
	vertex[3].SetPosition(x, y);
	vertex[3].SetColor(color.R, color.G, color.B, color.A);
	vertex[3].SetUV(UVBox.x, UVBox.y);
	vertexData.push_back(vertex[3]);

	vertex[4].SetPosition(x + TextureBox.w, y);
	vertex[4].SetColor(color.R, color.G, color.B, color.A);
	vertex[4].SetUV(UVBox.w, UVBox.y);
	vertexData.push_back(vertex[4]);

	vertex[5].SetPosition(x + TextureBox.w, y + TextureBox.h);
	vertex[5].SetColor(color.R, color.G, color.B, color.A);
	vertex[5].SetUV(UVBox.w, UVBox.h);
	vertexData.push_back(vertex[5]);

	int size = sizeof(dtSpriteVertex) * vertexData.size();

	glBindBuffer(GL_ARRAY_BUFFER, mVBO_ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertexData.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void NSpriteBatch::addQuad(float x, float y, NColor color, NRect UV)
{
	UVBox.x = UV.x / mContent->GetWidth();
	UVBox.y = UV.y / mContent->GetHeight();
	UVBox.w = (UV.x + UV.w) / mContent->GetWidth();
	UVBox.h = (UV.y + UV.h) / mContent->GetHeight();

	dtSpriteVertex vertex[6];
	vertex[0].SetPosition(x + UV.w, y + UV.h);
	vertex[0].SetColor(color.R, color.G, color.B, color.A);
	vertex[0].SetUV(UVBox.w, UVBox.h);
	vertexData.push_back(vertex[0]);

	vertex[1].SetPosition(x, y + UV.h);
	vertex[1].SetColor(color.R, color.G, color.B, color.A);
	vertex[1].SetUV(UVBox.x, UVBox.h);
	vertexData.push_back(vertex[1]);

	vertex[2].SetPosition(x, y);
	vertex[2].SetColor(color.R, color.G, color.B, color.A);
	vertex[2].SetUV(UVBox.x, UVBox.y);
	vertexData.push_back(vertex[2]);

	// seg triangulo
	vertex[3].SetPosition(x, y);
	vertex[3].SetColor(color.R, color.G, color.B, color.A);
	vertex[3].SetUV(UVBox.x, UVBox.y);
	vertexData.push_back(vertex[3]);

	vertex[4].SetPosition(x + UV.w, y);
	vertex[4].SetColor(color.R, color.G, color.B, color.A);
	vertex[4].SetUV(UVBox.w, UVBox.y);
	vertexData.push_back(vertex[4]);

	vertex[5].SetPosition(x + UV.w, y + UV.h);
	vertex[5].SetColor(color.R, color.G, color.B, color.A);
	vertex[5].SetUV(UVBox.w, UVBox.h);
	vertexData.push_back(vertex[5]);

	int size = sizeof(dtSpriteVertex) * vertexData.size();

	glBindBuffer(GL_ARRAY_BUFFER, mVBO_ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertexData.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void NSpriteBatch::addQuad(float x, float y, float width, float height)
{
	dtSpriteVertex vertex[6];
	vertex[0].SetPosition(x + width, y + height);
	vertex[0].SetColor(255, 255, 255, 255);
	vertex[0].SetUV(UVBox.w, UVBox.h);
	vertexData.push_back(vertex[0]);

	vertex[1].SetPosition(x, y + height);
	vertex[1].SetColor(255, 255, 255, 255);
	vertex[1].SetUV(UVBox.x, UVBox.h);
	vertexData.push_back(vertex[1]);

	vertex[2].SetPosition(x, y);
	vertex[2].SetColor(255, 255, 255, 255);
	vertex[2].SetUV(UVBox.x, UVBox.y);
	vertexData.push_back(vertex[2]);

	// seg triangulo
	vertex[3].SetPosition(x, y);
	vertex[3].SetColor(255, 255, 255, 255);
	vertex[3].SetUV(UVBox.x, UVBox.y);
	vertexData.push_back(vertex[3]);

	vertex[4].SetPosition(x + width, y);
	vertex[4].SetColor(255, 255, 255, 255);
	vertex[4].SetUV(UVBox.w, UVBox.y);
	vertexData.push_back(vertex[4]);

	vertex[5].SetPosition(x + width, y + height);
	vertex[5].SetColor(255, 255, 255, 255);
	vertex[5].SetUV(UVBox.w, UVBox.h);
	vertexData.push_back(vertex[5]);

	int size = sizeof(dtSpriteVertex) * vertexData.size();

	glBindBuffer(GL_ARRAY_BUFFER, mVBO_ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertexData.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void NSpriteBatch::addQuad(float x, float y, float width, float height, NRect UV)
{
	UVBox.x = UV.x / mContent->GetWidth();
	UVBox.y = UV.y / mContent->GetHeight();
	UVBox.w = (UV.x + UV.w) / mContent->GetWidth();
	UVBox.h = (UV.y + UV.h) / mContent->GetHeight();

	dtSpriteVertex vertex[6];
	vertex[0].SetPosition(x + width, y + height);
	vertex[0].SetColor(255, 255, 255, 255);
	vertex[0].SetUV(UVBox.w, UVBox.h);
	vertexData.push_back(vertex[0]);

	vertex[1].SetPosition(x, y + height);
	vertex[1].SetColor(255, 255, 255, 255);
	vertex[1].SetUV(UVBox.x, UVBox.h);
	vertexData.push_back(vertex[1]);

	vertex[2].SetPosition(x, y);
	vertex[2].SetColor(255, 255, 255, 255);
	vertex[2].SetUV(UVBox.x, UVBox.y);
	vertexData.push_back(vertex[2]);

	// seg triangulo
	vertex[3].SetPosition(x, y);
	vertex[3].SetColor(255, 255, 255, 255);
	vertex[3].SetUV(UVBox.x, UVBox.y);
	vertexData.push_back(vertex[3]);

	vertex[4].SetPosition(x + width, y);
	vertex[4].SetColor(255, 255, 255, 255);
	vertex[4].SetUV(UVBox.w, UVBox.y);
	vertexData.push_back(vertex[4]);

	vertex[5].SetPosition(x + width, y + height);
	vertex[5].SetColor(255, 255, 255, 255);
	vertex[5].SetUV(UVBox.w, UVBox.h);
	vertexData.push_back(vertex[5]);

	int size = sizeof(dtSpriteVertex) * vertexData.size();

	glBindBuffer(GL_ARRAY_BUFFER, mVBO_ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertexData.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void NSpriteBatch::addQuad(float x, float y, float width, float height, NColor color)
{
	dtSpriteVertex vertex[6];
	vertex[0].SetPosition(x + width, y + height);
	vertex[0].SetColor(color.R, color.G, color.B, color.A);
	vertex[0].SetUV(UVBox.w, UVBox.h);
	vertexData.push_back(vertex[0]);

	vertex[1].SetPosition(x, y + height);
	vertex[1].SetColor(color.R, color.G, color.B, color.A);
	vertex[1].SetUV(UVBox.x, UVBox.h);
	vertexData.push_back(vertex[1]);

	vertex[2].SetPosition(x, y);
	vertex[2].SetColor(color.R, color.G, color.B, color.A);
	vertex[2].SetUV(UVBox.x, UVBox.y);
	vertexData.push_back(vertex[2]);

	// seg triangulo
	vertex[3].SetPosition(x, y);
	vertex[3].SetColor(color.R, color.G, color.B, color.A);
	vertex[3].SetUV(UVBox.x, UVBox.y);
	vertexData.push_back(vertex[3]);

	vertex[4].SetPosition(x + width, y);
	vertex[4].SetColor(color.R, color.G, color.B, color.A);
	vertex[4].SetUV(UVBox.w, UVBox.y);
	vertexData.push_back(vertex[4]);

	vertex[5].SetPosition(x + width, y + height);
	vertex[5].SetColor(color.R, color.G, color.B, color.A);
	vertex[5].SetUV(UVBox.w, UVBox.h);
	vertexData.push_back(vertex[5]);

	int size = sizeof(dtSpriteVertex) * vertexData.size();

	glBindBuffer(GL_ARRAY_BUFFER, mVBO_ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertexData.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void NSpriteBatch::addQuad(float x, float y, float width, float height, NColor color, NRect UV)
{
	UVBox.x = UV.x / mContent->GetWidth();
	UVBox.y = UV.y / mContent->GetHeight();
	UVBox.w = (UV.x + UV.w) / mContent->GetWidth();
	UVBox.h = (UV.y + UV.h) / mContent->GetHeight();

	dtSpriteVertex vertex[6];
	vertex[0].SetPosition(x + width, y + height);
	vertex[0].SetColor(color.R, color.G, color.B, color.A);
	vertex[0].SetUV(UVBox.w, UVBox.h);
	vertexData.push_back(vertex[0]);

	vertex[1].SetPosition(x, y + height);
	vertex[1].SetColor(color.R, color.G, color.B, color.A);
	vertex[1].SetUV(UVBox.x, UVBox.h);
	vertexData.push_back(vertex[1]);

	vertex[2].SetPosition(x, y);
	vertex[2].SetColor(color.R, color.G, color.B, color.A);
	vertex[2].SetUV(UVBox.x, UVBox.y);
	vertexData.push_back(vertex[2]);

	// seg triangulo
	vertex[3].SetPosition(x, y);
	vertex[3].SetColor(color.R, color.G, color.B, color.A);
	vertex[3].SetUV(UVBox.x, UVBox.y);
	vertexData.push_back(vertex[3]);

	vertex[4].SetPosition(x + width, y);
	vertex[4].SetColor(color.R, color.G, color.B, color.A);
	vertex[4].SetUV(UVBox.w, UVBox.y);
	vertexData.push_back(vertex[4]);

	vertex[5].SetPosition(x + width, y + height);
	vertex[5].SetColor(color.R, color.G, color.B, color.A);
	vertex[5].SetUV(UVBox.w, UVBox.h);
	vertexData.push_back(vertex[5]);

	int size = sizeof(dtSpriteVertex) * vertexData.size();

	glBindBuffer(GL_ARRAY_BUFFER, mVBO_ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertexData.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}