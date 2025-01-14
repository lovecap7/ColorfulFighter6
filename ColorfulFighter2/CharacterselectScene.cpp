#include "CharacterselectScene.h"
#include "Dxlib.h"
#include "Input.h"
#include "SceneController.h"
#include "GameScene.h"
#include "game.h"
#include "Chara.h"
#include "BGM.h"
#include "SE.h"

namespace
{
	constexpr int kCenterX = Game::kScreenWidth / 2;
	constexpr int kCenterY = Game::kScreenHeight / 2;
	//�J�[�\��
	constexpr int kCursorOffset = 50;
	//�A�C�R���̉���
	constexpr int kIconWidth = 200;
	//�����G�̉���
	constexpr int kImageWidth = 300;

	//�R�}���h�Z�̐�
	constexpr int kCommandIndexMin = 1;
	constexpr int kCommandIndexMax = 9;
	//�i
	constexpr int kOneRowCommandNum = 3;
	constexpr int kOneColumnCommandNum = 3;

	//�A�C�R���̔��a
	constexpr int kIconRadius = 50;
	constexpr int kIconPosOffset = 200;
	//�R�}���h�̐�
	constexpr int kCommandNum = 9;
	//�R�}���h�A�C�R���̑傫��
	constexpr float kCommandIconImageScale = 0.5f;
	constexpr int kCommandIconImageWidth = 200;
	constexpr int kCommandIconImageHight = 200;
}

CharacterselectScene::CharacterselectScene(SceneController& controller):
	SceneBase(controller),
	m_currentSelectCommandIndexP1(1),
	m_currentSelectCommandIndexP2(2),
	m_isSelectP1(false),
	m_isSelectP2(false),
	m_selectCommandIndexP1{0,0,0},
	m_selectCommandIndexP2{0,0,0},
	m_selectCommandIconP1Handle{-1,-1,-1},
	m_selectCommandIconP2Handle{-1,-1,-1}
	
{
	//�C���[�W�摜(�����G)
	m_imageChara1Handle = LoadGraph("img/CharacterSelect/FighterImage/Chara1Image.png");

	//�J�[�\��
	m_cursorP1Handle = LoadGraph("img/CharacterSelect/Icon/SelectFrameP1.png");
	m_cursorP2Handle = LoadGraph("img/CharacterSelect/Icon/SelectFrameP2.png");

	//BGM
	m_bgm = std::make_shared<BGM>();
	int bgmhandle = LoadSoundMem("./BGM/BGM_SelectScene.mp3");
	m_bgm->SetBGM(bgmhandle);
	m_bgm->Volume(100);
	m_bgm->PlayLoop();
	//SE
	m_seP1 = std::make_shared<SE>();
	m_seP2 = std::make_shared<SE>();
	m_selectSehandle = LoadSoundMem("./SE/Select/SelectSE.mp3");
	m_cursorMoveSehandle = LoadSoundMem("./SE/Select/CursorMoveSE.mp3");
	m_cancelSehandle = LoadSoundMem("./SE/Select/CancelSE.mp3");

	for (int i = 0; i < kCommandNum; ++i)
	{
		switch (i)
		{
		case 0:
			m_commandIconHandle[i] = LoadGraph("./img/CharacterSelect/Icon/CommandIcon/Command1.png");
			break;
		case 1:
			m_commandIconHandle[i] = LoadGraph("./img/CharacterSelect/Icon/CommandIcon/Command1.png");
			break;
		case 2:
			m_commandIconHandle[i] = LoadGraph("./img/CharacterSelect/Icon/CommandIcon/Command3.png");
			break;
		case 3:
			m_commandIconHandle[i] = LoadGraph("./img/CharacterSelect/Icon/CommandIcon/Command4.png");
			break;
		case 4:
			m_commandIconHandle[i] = LoadGraph("./img/CharacterSelect/Icon/CommandIcon/Command5.png");
			break;
		case 5:
			m_commandIconHandle[i] = LoadGraph("./img/CharacterSelect/Icon/CommandIcon/Command6.png");
			break;
		case 6:
			m_commandIconHandle[i] = LoadGraph("./img/CharacterSelect/Icon/CommandIcon/Command7.png");
			break;
		case 7:
			m_commandIconHandle[i] = LoadGraph("./img/CharacterSelect/Icon/CommandIcon/Command8.png");
			break;
		case 8:
			m_commandIconHandle[i] = LoadGraph("./img/CharacterSelect/Icon/CommandIcon/Command9.png");
			break;
		default:
			break;
		}
	}
}

void CharacterselectScene::Update(Input& input, Input& input2)
{

	//���̃V�[���ł�肽������
	//�L�����N�^�[�����肵���炻�̃L�����N�^�[��
	//�|�C���^�����̃V�[���ɓn������

	//���肵�Ă��Ȃ���ΑI�ׂ�
	if (!m_isSelectP1)
	{

		if (input.IsTrigger("Left"))
		{
			//�J�[�\���ړ��̉�
			m_seP1->Stop();
			m_seP1->SetSE(m_cursorMoveSehandle);
			m_seP1->Volume(100);
			m_seP1->PlayOnce();

			m_currentSelectCommandIndexP1--;
			if (m_currentSelectCommandIndexP1 % 3 == 0)
			{
				m_currentSelectCommandIndexP1 += kOneRowCommandNum;
			}
		}
		if (input.IsTrigger("Right"))
		{
			//�J�[�\���ړ��̉�
			m_seP1->Stop();
			m_seP1->SetSE(m_cursorMoveSehandle);
			m_seP1->Volume(100);
			m_seP1->PlayOnce();

			m_currentSelectCommandIndexP1++;
			if ((m_currentSelectCommandIndexP1 - 1) % kOneRowCommandNum == 0)
			{
				m_currentSelectCommandIndexP1 -= kOneRowCommandNum;
			}
		}
		if (input.IsTrigger("Up"))
		{
			//�J�[�\���ړ��̉�
			m_seP1->Stop();
			m_seP1->SetSE(m_cursorMoveSehandle);
			m_seP1->Volume(100);
			m_seP1->PlayOnce();

			m_currentSelectCommandIndexP1 -= kOneColumnCommandNum;
			if (m_currentSelectCommandIndexP1 < kCommandIndexMin)
			{
				m_currentSelectCommandIndexP1 += (kOneColumnCommandNum * kOneRowCommandNum);
			}
		}
		if (input.IsTrigger("Down"))
		{
			//�J�[�\���ړ��̉�
			m_seP1->Stop();
			m_seP1->SetSE(m_cursorMoveSehandle);
			m_seP1->Volume(100);
			m_seP1->PlayOnce();

			m_currentSelectCommandIndexP1 += kOneColumnCommandNum;
			if (m_currentSelectCommandIndexP1 > kCommandIndexMax)
			{
				m_currentSelectCommandIndexP1 -= (kOneColumnCommandNum * kOneRowCommandNum);
			}
			
		}

		
	}
	if (!m_isSelectP2)
	{

		if (input2.IsTrigger("Left"))
		{
			//�J�[�\���ړ��̉�
			m_seP2->Stop();
			m_seP2->SetSE(m_cursorMoveSehandle);
			m_seP2->Volume(100);
			m_seP2->PlayOnce();

			m_currentSelectCommandIndexP2--;
			if (m_currentSelectCommandIndexP2 % 3 == 0)
			{
				m_currentSelectCommandIndexP2 += kOneRowCommandNum;
			}
		}
		if (input2.IsTrigger("Right"))
		{
			//�J�[�\���ړ��̉�
			m_seP2->Stop();
			m_seP2->SetSE(m_cursorMoveSehandle);
			m_seP2->Volume(100);
			m_seP2->PlayOnce();

			m_currentSelectCommandIndexP2++;
			if ((m_currentSelectCommandIndexP2 - 1) % kOneRowCommandNum == 0)
			{
				m_currentSelectCommandIndexP2 -= kOneRowCommandNum;
			}
		}
		if (input2.IsTrigger("Up"))
		{
			//�J�[�\���ړ��̉�
			m_seP2->Stop();
			m_seP2->SetSE(m_cursorMoveSehandle);
			m_seP2->Volume(100);
			m_seP2->PlayOnce();

			m_currentSelectCommandIndexP2 -= kOneColumnCommandNum;
			if (m_currentSelectCommandIndexP2 < kCommandIndexMin)
			{
				m_currentSelectCommandIndexP2 += (kOneColumnCommandNum * kOneRowCommandNum);
			}
		}
		if (input2.IsTrigger("Down"))
		{
			//�J�[�\���ړ��̉�
			m_seP2->Stop();
			m_seP2->SetSE(m_cursorMoveSehandle);
			m_seP2->Volume(100);
			m_seP2->PlayOnce();

			m_currentSelectCommandIndexP2 += kOneColumnCommandNum;
			if (m_currentSelectCommandIndexP2 > kCommandIndexMax)
			{
				m_currentSelectCommandIndexP2 -= (kOneColumnCommandNum * kOneRowCommandNum);
			}

		}
	}
	

	if (input.IsTrigger("Ok"))
	{
		//����̉�
		m_seP1->Stop();
		m_seP1->SetSE(m_selectSehandle);
		m_seP1->Volume(150);
		m_seP1->PlayOnce();
		for (int i = 0;i < 3;++i)
		{
			//�����Z�͑I�ׂȂ�
			if (m_selectCommandIndexP1[i] == m_currentSelectCommandIndexP1)
			{
				break;
			}
			//��Ȃ�C���f�b�N�X������
			if (m_selectCommandIndexP1[i] == 0)
			{
				m_selectCommandIndexP1[i] = m_currentSelectCommandIndexP1;
				m_selectCommandIconP1Handle[i] = m_commandIconHandle[m_selectCommandIndexP1[i] - 1];//�A�C�R���\��
				break;
			}
		}
		//3�I�񂾂�J�[�\���𓮂����Ȃ�����
		if (m_selectCommandIndexP1[2] != 0)
		{
			m_isSelectP1 = true;
		}
	}
	//�I�тȂ���
	if (input.IsTrigger("Cancel"))
	{
		//�L�����Z���̉�
		m_seP1->Stop();
		m_seP1->SetSE(m_cancelSehandle);
		m_seP1->Volume(150);
		m_seP1->PlayOnce();
		m_isSelectP1 = false;
		for (int i = 0;i < 3;++i)
		{
			//�Ō�ɑI�񂾏��ɏ���
			if (m_selectCommandIndexP1[2 - i] != 0)
			{
				m_selectCommandIndexP1[2 - i] = 0;
				m_selectCommandIconP1Handle[2 - i] = -1;
				break;
			}
		}
	}

	if (input2.IsTrigger("Ok"))
	{
		//����̉�
		m_seP2->Stop();
		m_seP2->SetSE(m_selectSehandle);
		m_seP2->Volume(100);
		m_seP2->PlayOnce();
		for (int i = 0;i < 3;++i)
		{
			//�����Z�͑I�ׂȂ�
			if (m_selectCommandIndexP2[i] == m_currentSelectCommandIndexP2)
			{
				break;
			}
			//��Ȃ�C���f�b�N�X������
			if (m_selectCommandIndexP2[i] == 0)
			{
				m_selectCommandIndexP2[i] = m_currentSelectCommandIndexP2;
				m_selectCommandIconP2Handle[i] = m_commandIconHandle[m_selectCommandIndexP2[i] - 1];//�A�C�R���\��
				break;
			}
		}
		if (m_selectCommandIndexP2[2] != 0)
		{
			m_isSelectP2 = true;
		}
	}
	//�I�тȂ���
	if (input2.IsTrigger("Cancel"))
	{
		//�L�����Z���̉�
		m_seP2->Stop();
		m_seP2->SetSE(m_cancelSehandle);
		m_seP2->Volume(150);
		m_seP2->PlayOnce();
		m_isSelectP2 = false;
		//�Ō�ɑI�񂾏��ɏ���
		for (int i = 0;i < 3;++i)
		{
			if (m_selectCommandIndexP2[2 - i] != 0)
			{
				m_selectCommandIndexP2[2 - i] = 0;
				m_selectCommandIconP2Handle[2 - i] = -1;
				break;
			}
		}
	}
	//2�l���I�񂾂�Q�[���V�[����
	if ((m_isSelectP1 && m_isSelectP2) && (input.IsTrigger("Start") || input2.IsTrigger("Start")))
	{

		//�I�񂾃R�}���h�Z�̃C���f�b�N�X�����̃V�[���ɓn�����߂ɕۑ�
		m_controller.SaveSelectCommandIndex(m_selectCommandIndexP1, m_selectCommandIndexP2);

		//�����ꂽ�玟�̏�Ԃɑ@��
		//���̏�Ԃ͂��̃N���X���o���Ă���
		m_controller.ChangeScene(std::make_shared<GameScene>(m_controller));
		return;//�Y�ꂸreturn
	}
#if _DEBUG
	//�f�o�b�O�p
	if (input.IsTrigger("Select"))
	{
		////BGM���~�߂�
		//soundManager.StopBGM();

		for (int i = 0;i < 3;++i)
		{
			//��Ȃ�C���f�b�N�X������
			if (m_selectCommandIndexP1[i] == 0)
			{
				m_selectCommandIndexP1[i] = i + 1;
			}
			//��Ȃ�C���f�b�N�X������
			if (m_selectCommandIndexP2[i] == 0)
			{
				m_selectCommandIndexP2[i] = i + 1;
			}
		}

		//�I�񂾃R�}���h�Z�̃C���f�b�N�X�����̃V�[���ɓn�����߂ɕۑ�
		m_controller.SaveSelectCommandIndex(m_selectCommandIndexP1, m_selectCommandIndexP2);
		
		//�����ꂽ�玟�̏�Ԃɑ@��
		//���̏�Ԃ͂��̃N���X���o���Ă���
		m_controller.ChangeScene(std::make_shared<GameScene>(m_controller));
		return;//�Y�ꂸreturn
	}
#endif
}

void CharacterselectScene::Draw()
{
	//�w�i
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x555555, true);
	//�����G
	DxLib::DrawGraph(0, 100, m_imageChara1Handle, true);//1P
	DxLib::DrawTurnGraph(Game::kScreenWidth - kImageWidth, 100, m_imageChara1Handle, true);//2P

	//�Z1
	DrawCircle(kCenterX - kIconPosOffset, kCenterY - kIconPosOffset, kIconRadius, 0x22ff22, true, true);
	//�Z2
	DrawCircle(kCenterX, kCenterY - kIconPosOffset, kIconRadius, 0xff2222, true, true);
	//�Z3
	DrawCircle(kCenterX + kIconPosOffset, kCenterY - kIconPosOffset, kIconRadius, 0x2222ff, true, true);
	//�Z4
	DrawCircle(kCenterX - kIconPosOffset, kCenterY, kIconRadius, 0xff22ff, true, true);
	//�Z5
	DrawCircle(kCenterX, kCenterY, kIconRadius,0x22ffff, true, true);
	//�Z6
	DrawCircle(kCenterX + kIconPosOffset, kCenterY, kIconRadius, 0xffff22, true, true);
	//�Z7
	DrawCircle(kCenterX - kIconPosOffset, kCenterY + kIconPosOffset, kIconRadius, 0xcff7700, true, true);
	//�Z8
	DrawCircle(kCenterX, kCenterY + kIconPosOffset, kIconRadius, 0x000000, true, true);
	//�Z9
	DrawCircle(kCenterX + kIconPosOffset, kCenterY + kIconPosOffset, kIconRadius, 0xffffff, true, true);

	for (int i = 0; i < kCommandNum; ++i)
	{
		switch (i)
		{
		case 0:
			//�Z1
			DxLib::DrawRotaGraph(kCenterX - kIconPosOffset, kCenterY - kIconPosOffset, 1.0, 0, m_commandIconHandle[i], true, 0, 0);
			break;
		case 1:
			//�Z2
			DxLib::DrawRotaGraph(kCenterX, kCenterY - kIconPosOffset, 1.0, 0, m_commandIconHandle[i], true, 0, 0);
			break;
		case 2:
			//�Z3
			DxLib::DrawRotaGraph(kCenterX + kIconPosOffset, kCenterY - kIconPosOffset, 1.0, 0, m_commandIconHandle[i], true, 0, 0);
			break;
		case 3:
			//�Z4
			DxLib::DrawRotaGraph(kCenterX - kIconPosOffset, kCenterY, 1.0, 0, m_commandIconHandle[i], true, 0, 0);
			break;
		case 4:
			//�Z5
			DxLib::DrawRotaGraph(kCenterX, kCenterY, 1.0, 0, m_commandIconHandle[i], true, 0, 0);
			break;
		case 5:
			//�Z6
			DxLib::DrawRotaGraph(kCenterX + kIconPosOffset, kCenterY, 1.0, 0, m_commandIconHandle[i], true, 0, 0);
			break;
		case 6:
			//�Z7
			DxLib::DrawRotaGraph(kCenterX - kIconPosOffset, kCenterY + kIconPosOffset, 1.0, 0, m_commandIconHandle[i], true, 0, 0);
			break;
		case 7:
			//�Z8
			DxLib::DrawRotaGraph(kCenterX, kCenterY + kIconPosOffset, 1.0, 0, m_commandIconHandle[i], true, 0, 0);
			break;
		case 8:
			//�Z9
			DxLib::DrawRotaGraph(kCenterX + kIconPosOffset, kCenterY + kIconPosOffset, 1.0, 0, m_commandIconHandle[i], true, 0, 0);
			break;
		default:
			break;
		}
	}

	//�J�[�\��
	for (int i = 1; i <= 2;++i)
	{
		//P1
		int selectIndex = m_currentSelectCommandIndexP1;
		int cursorHandle = m_cursorP1Handle;
		//P2
		if (i == 2)
		{
			selectIndex = m_currentSelectCommandIndexP2;
			cursorHandle = m_cursorP2Handle;
		}

		switch (selectIndex)
		{
		case 1:
			//�Z1
			DxLib::DrawRotaGraph(kCenterX - kIconPosOffset, kCenterY - kIconPosOffset, 1.0, 0, cursorHandle, true, 0, 0);
			break;
		case 2:
			//�Z2
			DxLib::DrawRotaGraph(kCenterX, kCenterY - kIconPosOffset, 1.0, 0, cursorHandle, true, 0, 0);
			break;
		case 3:
			//�Z3
			DxLib::DrawRotaGraph(kCenterX + kIconPosOffset, kCenterY - kIconPosOffset, 1.0, 0, cursorHandle, true, 0, 0);
			break;
		case 4:
			//�Z4
			DxLib::DrawRotaGraph(kCenterX - kIconPosOffset, kCenterY, 1.0, 0, cursorHandle, true, 0, 0);
			break;
		case 5:
			//�Z5
			DxLib::DrawRotaGraph(kCenterX, kCenterY, 1.0, 0, cursorHandle, true, 0, 0);
			break;
		case 6:
			//�Z6
			DxLib::DrawRotaGraph(kCenterX + kIconPosOffset, kCenterY, 1.0, 0, cursorHandle, true, 0, 0);
			break;
		case 7:
			//�Z7
			DxLib::DrawRotaGraph(kCenterX - kIconPosOffset, kCenterY + kIconPosOffset, 1.0, 0, cursorHandle, true, 0, 0);
			break;
		case 8:
			//�Z8
			DxLib::DrawRotaGraph(kCenterX, kCenterY + kIconPosOffset, 1.0, 0,cursorHandle, true, 0, 0);
			break;
		case 9:
			//�Z9
			DxLib::DrawRotaGraph(kCenterX + kIconPosOffset, kCenterY + kIconPosOffset, 1.0, 0, cursorHandle, true, 0, 0);
			break;
		default:
			break;
		}
	}

	//�I�񂾃R�}���h�̕\��
		//�R�}���h
	for (int i = 0; i < 3; ++i)
	{
		DrawRectRotaGraphFast(
			(kCommandIconImageWidth * kCommandIconImageScale) * (i + 1),
			(Game::kScreenHeight / 2),
			0, 0, kCommandIconImageWidth, kCommandIconImageHight,
			kCommandIconImageScale, 0.0f, m_selectCommandIconP1Handle[i], true);
		DrawRectRotaGraphFast(
			Game::kScreenWidth - ((kCommandIconImageWidth * kCommandIconImageScale) * (i + 1)),
			(Game::kScreenHeight / 2),
			0, 0, kCommandIconImageWidth, kCommandIconImageHight,
			kCommandIconImageScale, 0.0f, m_selectCommandIconP2Handle[i], true);
	}
#if _DEBUG	
	DxLib::DrawString(10, 10, "CharacterselectScene", 0xffffff);
	for (int i = 0;i < 3;++i)
	{
		DrawFormatString(400, 50 + (20 * i), 0xff3333, "selectNumP1 = %d", m_selectCommandIndexP1[i]);
		DrawFormatString(1000, 50 + (20 * i), 0x0000ff, "selectNumP2 = %d", m_selectCommandIndexP2[i]);
	}
#endif
	
}
