#include "TitleScene.h"
#include "Dxlib.h"
#include "game.h"
#include "Input.h"
#include "SceneController.h"
#include "CharacterselectScene.h"
#include "BGM.h"


namespace
{
	constexpr int kPosXOffset = 100;
	constexpr int kPosY1Offset = -100;
	constexpr int kPosY2Offset = -200;
}

TitleScene::TitleScene(SceneController& contoller) :
	SceneBase(contoller),
	m_titleHandle(LoadGraph("./img/title/ColorfulFighter_2.png"))
{
	m_bgm = std::make_shared<BGM>();
	int bgmhandle = LoadSoundMem("./BGM/BGM_Title.mp3");
	m_bgm->SetBGM(bgmhandle);
	m_bgm->Volume(100);
	m_bgm->PlayLoop();
}

void TitleScene::Update(Input& input, Input& input2)
{
	if (input.IsTrigger("A"))
	{
		//‰Ÿ‚³‚ê‚½‚çŸ‚Ìó‘Ô‚É‘@ˆÛ
		//Ÿ‚Ìó‘Ô‚Í‚±‚ÌƒNƒ‰ƒX‚ªŠo‚¦‚Ä‚¨‚­
		m_controller.ChangeScene(std::make_shared<CharacterselectScene>(m_controller));
		return;//–Y‚ê‚¸return
	}
}

void TitleScene::Draw()
{
#if _DEBUG	
	DrawString(10, 10, "Title Scene", 0xffffff);
#endif
	
	DrawExtendGraph(kPosXOffset, kPosY1Offset, Game::kScreenWidth - kPosXOffset, Game::kScreenHeight + kPosY2Offset,
		m_titleHandle, true);
	//DrawString(550, 500, "GAME START", 0xffffff);
}
