#include "ResultScene.h"
#include "SceneController.h"
#include "Input.h"
#include "CommandSelectScene.h"
#include "GameScene.h"
#include "FadeManager.h"
#include <DxLib.h>
#include "game.h"
#include "BGM.h"

namespace
{
	
}


void ResultScene::Rematch()
{
	//�����ꂽ�玟�̏�Ԃɑ@��
	//���̏�Ԃ͂��̃N���X���o���Ă���
	m_controller.ChangeScene(std::make_shared<GameScene>(m_controller));
}

void ResultScene::Reselect()
{
	//�����ꂽ�玟�̏�Ԃɑ@��
	//���̏�Ԃ͂��̃N���X���o���Ă���
	m_controller.ChangeScene(std::make_shared<CommandSelectScene>(m_controller));
}

ResultScene::ResultScene(SceneController& controller):
	SceneBase(controller),
	m_p1Handle(-1),
	m_p2Handle(-1),
	m_isFadeIn(false),
	//���[�f�B���O���
	m_loadingHandle(LoadGraph("./img/Loading/NowLoading.png"))
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

	//�t�F�[�h�C������Ƃ��Ɏg��
	m_fadeManager = std::make_shared<FadeManager>();
}

void ResultScene::Update(Input& input, Input& input2)
{
	//BGM�̍Đ����I�������؂�ւ�
	if (m_bgm->CheckEndBGM())
	{
		m_isFadeIn = true;
		if (m_fadeManager->IsFinishFadeIn())
		{
			Rematch();//�Đ�
			return;//�Y�ꂸreturn
		}
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
	m_fadeManager->FadeDraw(m_isFadeIn);
	//�Ȃ���[�ł���
	if (m_fadeManager->IsFinishFadeIn())
	{
		DxLib::DrawGraph(0, 0, m_loadingHandle, true);
	}
}

void ResultScene::RecordResult()
{
}
