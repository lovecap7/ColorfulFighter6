#include "ResultScene.h"
#include "SceneController.h"
#include "Input.h"
#include "CharacterselectScene.h"
#include <DxLib.h>
#include "game.h"
#include "BGM.h"

namespace
{
	
}


ResultScene::ResultScene(SceneController& controller):
	SceneBase(controller),
	m_p1Handle(-1),
	m_p2Handle(-1)
{
	//1P���������Ȃ�
	if (m_controller.GetWinPlayerIndex() == PlayerIndex::Player1)
	{
		m_p1Handle = LoadGraph("./img/Result/Chara1_win.png");
		m_p2Handle = LoadGraph("./img/Result/Chara1_lose.png");
	}
	else if (m_controller.GetWinPlayerIndex() == PlayerIndex::Player2)
	{
		m_p1Handle = LoadGraph("./img/Result/Chara1_lose.png");
		m_p2Handle = LoadGraph("./img/Result/Chara1_win.png");
	}

	m_bgm = std::make_shared<BGM>();
	int bgmhandle = LoadSoundMem("./BGM/BGM_Result.mp3");
	m_bgm->SetBGM(bgmhandle);
	m_bgm->Volume(100);
	m_bgm->PlayOnce();
}

void ResultScene::Update(Input& input, Input& input2)
{

	//BGM�̍Đ����I�������؂�ւ�
	if (m_bgm->CheckEndBGM())
	{
		//�����ꂽ�玟�̏�Ԃɑ@��
		//���̏�Ԃ͂��̃N���X���o���Ă���
		m_controller.ChangeScene(std::make_shared<CharacterselectScene>(m_controller));
		return;//�Y�ꂸreturn
	}
}

void ResultScene::Draw()
{
#if _DEBUG	
	DrawString(10, 10, "Result Scene", 0xffffff);
#endif
	//���U���g�̉摜
	DxLib::DrawGraph(100, (Game::kScreenHeight / 2) - 400, m_p1Handle, true);
	DxLib::DrawTurnGraph(Game::kScreenWidth - 600, (Game::kScreenHeight / 2) - 400, m_p2Handle, true);

}

void ResultScene::RecordResult()
{
}
