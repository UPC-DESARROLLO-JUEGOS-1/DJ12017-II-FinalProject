#include "NPrimitiveQuad.h"
#include "NFramework.h"
#include "NShaderManagement.h"
#include "NPrimitiveShader.h"
#include "dtPrimitiveVertex.h"

void NPrimitiveQuad::Initialize(float x, float y, int width,
	int height, GLubyte r, GLubyte g, GLubyte b, GLubyte a) {

	NDrawable2D::Initialize(x, y);

	NFramework* framework = NFramework::GET_FRAMEWORK();
	NContentManagement* contentManagement = framework->GetContentManagement();
	NShaderManagement* shaderManagement = framework->GetShaderManagment();

	const std::string baseShaderPath = "Content/Shaders/PrimitiveShader";
	mCurrentShader = shaderManagement->LoadAndGetShader<NPrimitiveShader>(baseShaderPath);

	mWidth = width;
	mHeight = height;

	// 3 vertices por triangulo, 2 triangulos conforman un quad
	dtPrimitiveVertex vertexData[6]; 
	// ::: PRIMER TRIANGULO :::
	// primer vertice
	vertexData[0].SetPosition(mWidth, mHeight);
	vertexData[0].SetColor(r, g, b, a);
	// segundo vertice
	vertexData[1].SetPosition(0, mHeight);
	vertexData[1].SetColor(r, g, b, a);
	// tercer vertice
	vertexData[2].SetPosition(0, 0);
	vertexData[2].SetColor(r, g, b, a);

	// ::: SEGUNDO TRIANGULO :::
	// primer vertice
	vertexData[3].SetPosition(0, 0);
	vertexData[3].SetColor(r, g, b, a);
	// segundo vertice
	vertexData[4].SetPosition(mWidth, 0);
	vertexData[4].SetColor(r, g, b, a);
	// tercer vertice
	vertexData[5].SetPosition(mWidth, mHeight);
	vertexData[5].SetColor(r, g, b, a);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO_ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void NPrimitiveQuad::Draw(float dt) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	mCurrentShader->Use();

	GLuint wvpLocation = mCurrentShader->GetUniformLocation("wvp");
	glm::mat4 cameraMatrix = mRenderCamera->GetResultMatrix();
	glm::mat4 resultMatrix = cameraMatrix * mWorldMatrix;
	// pasamos la data de la matrix al shader
	glUniformMatrix4fv(wvpLocation, 1, false, &(resultMatrix[0][0]));

	glBindBuffer(GL_ARRAY_BUFFER, mVBO_ID);
	
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(dtPrimitiveVertex),
		(void*)offsetof(dtPrimitiveVertex, Position));
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(dtPrimitiveVertex),
		(void*)offsetof(dtPrimitiveVertex, dtPrimitiveVertex::Color));

	// Dibuja en pantalla
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	mCurrentShader->Unuse();
}