#include "TitleScene.h"
#include "Dxlib.h"
#include "game.h"
#include "Input.h"
#include "SceneController.h"
#include "CommandSelectScene.h"
#include "BGM.h"


namespace
{
	constexpr int kTextHiddenFrame = 60;
	constexpr int kFrameReset = kTextHiddenFrame * 2;
	constexpr int kTextWidth = 500;
	constexpr int kTextPosX = (Game::kScreenWidth / 2) - (kTextWidth / 2);
	constexpr int kTextPosY = 600;
}

void TitleScene::BlinkingTextDraw()
{
	if (m_countFrame < kTextHiddenFrame)
	{
		DrawGraph(kTextPosX, kTextPosY, m_textHandle, true);
	}
	if (kFrameReset < m_countFrame)
	{
		m_countFrame = 0;
	}
}

TitleScene::TitleScene(SceneController& contoller) :
	SceneBase(contoller),
	m_titleHandle(LoadGraph("./img/title/Title.png")),
	m_textHandle(LoadGraph("./img/title/PressAnyButton.png")),
	m_countFrame(0)
{
	m_bgm = std::make_shared<BGM>();
	int bgmhandle = LoadSoundMem("./BGM/BGM_Title.mp3");
	m_bgm->SetBGM(bgmhandle);
	m_bgm->Volume(100);
	m_bgm->PlayLoop();
}

void TitleScene::Update(Input& input, Input& input2)
{
	m_countFrame++;
	if (input.IsTrigger("A") ||
		input.IsTrigger("B") ||
		input.IsTrigger("X") ||
		input.IsTrigger("Y") || 
		input2.IsTrigger("A") ||
		input2.IsTrigger("B") ||
		input2.IsTrigger("X") ||
		input2.IsTrigger("Y") )
	{
		//‰Ÿ‚³‚ê‚½‚çŸ‚Ìó‘Ô‚É‘@ˆÛ
		//Ÿ‚Ìó‘Ô‚Í‚±‚ÌƒNƒ‰ƒX‚ªŠo‚¦‚Ä‚¨‚­
		m_controller.ChangeScene(std::make_shared<CommandSelectScene>(m_controller));
		return;//–Y‚ê‚¸return
	}
}

void TitleScene::Draw()
{
#if _DEBUG	
	DrawString(10, 10, "Title Scene", 0xffffff);
#endif
	
	DrawGraph(0, 0, m_titleHandle, true);
	BlinkingTextDraw();
}
