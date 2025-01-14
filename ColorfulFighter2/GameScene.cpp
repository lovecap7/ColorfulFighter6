#include "GameScene.h"
#include "Dxlib.h"
#include "game.h"
#include "Input.h"
#include "SceneController.h"
#include "CharacterselectScene.h"
#include "ResultScene.h"
#include "Player.h"
#include "UI.h"
#include "GameManager.h"
#include "Camera.h"
#include "Bullet.h"
#include "BGM.h"

namespace
{
	constexpr float kPlayerFirstPosX = 200.0f;
	constexpr int kStageWidth = 2600;
	constexpr int kColorChangeFrame = 60;
	constexpr int kColorNum = 8;

	//�X�e�[�W
	constexpr float kStageBackPosX = 0.0f;
	constexpr float kStageBackPosY = 750.0f;
	constexpr float kStageFloorPosX = 0.0f;
	constexpr float kStageFloorPosY = 1250.0f;
}

void GameScene::CreateColorHandle()
{
	for (int i = 0;i < kColorNum;++i)
	{
		switch (i)
		{
		case 0:
			m_floorColorHandle[i] = LoadGraph("./img/stage/floor/red.png");
			m_backColorHandle[i] = LoadGraph("./img/stage/back/red.png");
			break;
		case 1:
			m_floorColorHandle[i] = LoadGraph("./img/stage/floor/orange.png");
			m_backColorHandle[i] = LoadGraph("./img/stage/back/orange.png");
			break;
		case 2:
			m_floorColorHandle[i] = LoadGraph("./img/stage/floor/yellow.png");
			m_backColorHandle[i] = LoadGraph("./img/stage/back/yellow.png");
			break;
		case 3:
			m_floorColorHandle[i] = LoadGraph("./img/stage/floor/green.png");
			m_backColorHandle[i] = LoadGraph("./img/stage/back/green.png");
			break;
		case 4:
			m_floorColorHandle[i] = LoadGraph("./img/stage/floor/cyan.png");
			m_backColorHandle[i] = LoadGraph("./img/stage/back/cyan.png");
			break;
		case 5:
			m_floorColorHandle[i] = LoadGraph("./img/stage/floor/blue.png");
			m_backColorHandle[i] = LoadGraph("./img/stage/back/blue.png");
			break;
		case 6:
			m_floorColorHandle[i] = LoadGraph("./img/stage/floor/purple.png");
			m_backColorHandle[i] = LoadGraph("./img/stage/back/purple.png");
			break;
		case 7:
			m_floorColorHandle[i] = LoadGraph("./img/stage/floor/pink.png");
			m_backColorHandle[i] = LoadGraph("./img/stage/back/pink.png");
			break;
		default:
			break;
		}
	}
}

void GameScene::GameInit()
{
	m_player1->Init((kStageWidth / 2) - kPlayerFirstPosX, false);
	m_player2->Init((kStageWidth / 2) + kPlayerFirstPosX, true);
	m_bullet1->Init();
	m_bullet2->Init();
	m_gameManager->Init();
	m_ui->Init(m_player1->GetHp(), m_player2->GetHp(),*m_gameManager);
	m_camera->Init(*m_player1, *m_player2);
}
void GameScene::StageDraw()
{
	DrawGraph(kStageBackPosX + static_cast<int>(m_camera->m_drawOffset.x), 
		kStageBackPosY + static_cast<int>(m_camera->m_drawOffset.y), 
		m_backBaseHandle, true);
	DrawGraph(kStageFloorPosX + static_cast<int>(m_camera->m_drawOffset.x), 
		kStageFloorPosY + static_cast<int>(m_camera->m_drawOffset.y), 
		m_floorBaseHandle, true);
	//�F
	//�F���������班�������ɂ��Ă�
	DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawGraph(kStageBackPosX + static_cast<int>(m_camera->m_drawOffset.x), 
		kStageBackPosY + static_cast<int>(m_camera->m_drawOffset.y), 
		m_backColorHandle[m_colorIndex], true);
	DrawGraph(kStageFloorPosX + static_cast<int>(m_camera->m_drawOffset.x), 
		kStageFloorPosY + static_cast<int>(m_camera->m_drawOffset.y), 
		m_floorColorHandle[m_colorIndex], true);
	DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 200);
}

GameScene::GameScene(SceneController& controller):
	SceneBase(controller),
	m_color(0x000000),
	m_colorCountFrame(0),
	m_colorIndex(0),
	m_backColorHandle{},
	m_floorColorHandle{}
{

	//�������m��
	m_player1 = std::make_shared<Player>(PlayerIndex::Player1, m_controller.GetSelectCommandIndex(PlayerIndex::Player1));
	m_player2 = std::make_shared<Player>(PlayerIndex::Player2, m_controller.GetSelectCommandIndex(PlayerIndex::Player2));
	//�Q�[���}�l�[�W���[
	m_gameManager = std::make_shared<GameManager>();
	//UI
	m_ui = std::make_shared<UI>(m_controller.GetSelectCommandIndex(PlayerIndex::Player1), m_controller.GetSelectCommandIndex(PlayerIndex::Player2));
	//�e
	m_bullet1 = std::make_shared<Bullet>(PlayerIndex::Player1);
	m_bullet2 = std::make_shared<Bullet>(PlayerIndex::Player2);

	//�X�e�[�W
	m_floorBaseHandle = LoadGraph("./img/stage/floor/base.png");//��
	m_backBaseHandle = LoadGraph("./img/stage/back/base.png");;//�w�i
	//8�F�Ɍ��点�鏀��
	CreateColorHandle();
	
	//�J����
	m_camera = std::make_shared<Camera>();

	//BGM
	m_bgm = std::make_shared<BGM>();
	int bgmhandle = LoadSoundMem("./BGM/BGM_Stage2.mp3");
	m_bgm->SetBGM(bgmhandle);
	m_bgm->Volume(100);
	m_bgm->PlayLoop();

	//������
	GameInit();
}

void GameScene::Update(Input& input, Input& input2)
{
#if _DEBUG
	if (input.IsTrigger("Pause"))
	{
		//�����ꂽ�玟�̏�Ԃɑ@��
		//���̏�Ԃ͂��̃N���X���o���Ă���
		m_controller.ChangeScene(std::make_shared<CharacterselectScene>(m_controller));
		return;//�Y�ꂸreturn
	}
#endif
	m_camera->Update(*m_player1, *m_player2, *m_gameManager);
	//�퓬�O��Update���~�߂�
	//�q�b�g�X�g�b�v����Update���~�߂�
	if (!m_gameManager->GetIsHitStop() && m_gameManager->GetIsStartRound())
	{
		m_player1->Update(input, m_player2, m_bullet1, *m_gameManager);
		m_player2->Update(input2, m_player1, m_bullet2, *m_gameManager);
		m_bullet1->Update(*m_player2, *m_bullet2, *m_camera);
		m_bullet2->Update(*m_player1, *m_bullet1, *m_camera);
		m_gameManager->Update(*m_player1, *m_player2, *m_bullet1, *m_bullet2,
			*m_camera, *m_ui);
	}

	m_gameManager->NoStopUpdate(*m_player1, *m_player2);
	m_ui->Update(m_player1->GetHp(), m_player2->GetHp(), *m_gameManager);

	//2�{�������Q�[���I��
	if (m_gameManager->GetIsGameset())
	{
		////BGM���~�߂�
		//soundManager.StopBGM();

		//�������v���C���[�̃C���f�b�N�X���Z�b�g
		m_controller.SetWinPlayerIndex(m_gameManager->ResultWinPlayerIndex());
		m_controller.ChangeScene(std::make_shared<ResultScene>(m_controller));
		return;//�Y�ꂸreturn
	}
	//���E���h�̐؂�ւ��
	if (m_gameManager->GetIsChangeRound())
	{
		//���Z�b�g
		GameInit();
		m_gameManager->OffIsChangeRound();
	}

	m_colorCountFrame++;
	if ((m_colorCountFrame % kColorChangeFrame) == 0)
	{
		m_colorIndex++;
	}
	if (m_colorIndex >= kColorNum)
	{
		m_colorIndex = 0;
	}
}

void GameScene::Draw()
{
	//�X�e�[�W
	StageDraw();

	m_ui->AlwaysDraw();
	m_ui->DirectionDraw();
	//�U�������ق���O�ɕ`��
	if (m_gameManager->GetIsDrawFrontP1())
	{
		m_player2->Draw(*m_camera);
		m_player1->Draw(*m_camera);
	}
	else
	{
		m_player1->Draw(*m_camera);
		m_player2->Draw(*m_camera);
	}
	m_bullet1->Draw(*m_camera);
	m_bullet2->Draw(*m_camera);
	//�t�F�[�h�C���A�E�g
	m_gameManager->Draw(*m_camera);

#if _DEBUG	
	DrawString(10, 10, "GameScene", 0xffffff);
	DrawCircleAA(m_camera->m_drawOffset.x, m_camera->m_drawOffset.y, 50, 0xff0000, true);
	DrawCircleAA(m_camera->m_pos.x, m_camera->m_pos.y, 40, 0xff0000, true);
#endif
}

