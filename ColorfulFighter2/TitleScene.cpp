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
		//押されたら次の状態に繊維
		//次の状態はこのクラスが覚えておく
		m_controller.ChangeScene(std::make_shared<CharacterselectScene>(m_controller));
		return;//忘れずreturn
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
