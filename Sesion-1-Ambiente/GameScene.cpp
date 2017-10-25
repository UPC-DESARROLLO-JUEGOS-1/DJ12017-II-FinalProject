#include "GameScene.h"


#include <UPCFramework\NFramework.h>
#include <UPCFramework\NCommon.h>
#include <UPCFramework\pugixml.hpp>
#include <UPCFramework\FatalError.h>

void GameScene::Initialize() {
	mQuad = new NPrimitiveQuad();
	mQuad->Initialize(100, 100, 40, 80, 255, 0, 0, 255);
	mQuad->SetRenderCamera(NFramework::GET_FRAMEWORK()->GetCamera());

	mCircle = new NPrimitiveCircumference();
	mCircle->Initialize(200, 100, 40, 255, 0, 0, 255);
	mCircle->SetRenderCamera(NFramework::GET_FRAMEWORK()->GetCamera());

	mSprite = new NSprite();
	mSprite->Initialize(300, 100, 80, 80, "Content/Sprites/mario.png");
	mSprite->SetRenderCamera(NFramework::GET_FRAMEWORK()->GetCamera());

	// Text
	mTextfield = new NTextfield2D();
	mTextfield->Initialize(50, 200, "Content/Fonts/kronika_32.fnt");
	mTextfield->SetText("Aloha soy una prueba,\n en UPC Bitch!");
	mTextfield->SetRenderCamera(NFramework::GET_FRAMEWORK()->GetCamera());
	mTextfield->SetScaleX(0.5f);
	mTextfield->SetScaleY(0.5f);
	//mTextfield->SetLetterColor(2, NColor::Red);
	//mTextfield->SetLetterColor(3, NColor::Blue);
}

void GameScene::OnKeyDown(SDL_Keycode key)
{
	switch (key)
	{
	case SDLK_BACKSPACE:
		if (mCurrentText.length() > 0) {
			mCurrentText.pop_back();
		}
		break;
	case SDLK_RETURN:
		mCurrentText += "\n";
		break;
	default:
		std::string letter = NCommon::UnicodeToUTF8(key);
		mCurrentText += letter;
		break;
	}

	mTextfield->SetText(mCurrentText);
}

void GameScene::OnKeyUp(SDL_Keycode key)
{
	
}

void GameScene::Update(float dt)
{
	mQuad->Update(dt);
	mCircle->Update(dt);
	mSprite->Update(dt);
	mTextfield->Update(dt);
}

void GameScene::Draw(float dt)
{
	mQuad->Draw(dt);
	mCircle->Draw(dt);
	mSprite->Draw(dt);
	mTextfield->Draw(dt);
}
