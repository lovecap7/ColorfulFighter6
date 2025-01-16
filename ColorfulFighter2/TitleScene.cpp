#include "TitleScene.h"
#include "Dxlib.h"
#include "game.h"
#include "Input.h"
#include "SceneController.h"
#include "CommandSelectScene.h"
#include "BGM.h"


namespace
{
	constexpr int kPosXOffset = 100;
	constexpr int kPosY1Offset = -100;
	constexpr int kPosY2Offset = -200;
	constexpr int kTextHiddenFrame = 60;
	constexpr int kFrameReset = kTextHiddenFrame * 2;

}

void TitleScene::BlinkingTextDraw()
{
	if (m_countFrame < kTextHiddenFrame)
	{
		DrawString((Game::kScreenWidth / 2) - 100, 700, "< PUSH A BUTON >", 0xffffff);
	}
	if (kFrameReset < m_countFrame)
	{
		m_countFrame = 0;
	}
}

TitleScene::TitleScene(SceneController& contoller) :
	SceneBase(contoller),
	m_titleHandle(LoadGraph("./img/title/ColorfulFighter_2.png")),
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
	if (input.IsTrigger("A"))
	{
		//�����ꂽ�玟�̏�Ԃɑ@��
		//���̏�Ԃ͂��̃N���X���o���Ă���
		m_controller.ChangeScene(std::make_shared<CommandSelectScene>(m_controller));
		return;//�Y�ꂸreturn
	}
}

void TitleScene::Draw()
{
#if _DEBUG	
	DrawString(10, 10, "Title Scene", 0xffffff);
#endif
	
	DrawExtendGraph(kPosXOffset, kPosY1Offset, Game::kScreenWidth - kPosXOffset, Game::kScreenHeight + kPosY2Offset,
		m_titleHandle, true);
	BlinkingTextDraw();
}
