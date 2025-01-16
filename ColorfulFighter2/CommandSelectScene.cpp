#include "CommandSelectScene.h"
#include "Dxlib.h"
#include "Input.h"
#include "SceneController.h"
#include "GameScene.h"
#include "game.h"
#include "Chara.h"
#include "BGM.h"
#include "SE.h"
#include "FadeManager.h"

namespace
{
	constexpr int kCenterX = Game::kScreenWidth / 2;
	constexpr int kCenterY = Game::kScreenHeight / 2;
	//�J�[�\��
	constexpr int kCursorOffset = 50;
	//�����G�̑傫��
	constexpr int kPlayerImageWidth = 300;
	constexpr int kPlayerImageHeight = 500;

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
	//�I�ׂ�R�}���h�Z�̐�
	constexpr int kSelectCommandNum = 3;

	//Ready
	constexpr int kReadyPosXP1 = 50;
	constexpr int kReadyPosXP2 = Game::kScreenWidth - 350;
	constexpr int kReadyPosY = 650;
}

CommandSelectScene::CommandSelectScene(SceneController& controller) :
	SceneBase(controller),
	m_currentSelectCommandIndexP1(1),
	m_currentSelectCommandIndexP2(2),
	m_isSelectFinishP1(false),
	m_isSelectFinishP2(false),
	m_isReadyP1(false),
	m_isReadyP2(false),
	m_selectCommandIndexP1{ 0,0,0 },
	m_selectCommandIndexP2{ 0,0,0 },
	m_isFadeIn(false),
	m_countFrame(0),
	//�C���[�W�摜(�����G)
	m_imageChara1Handle(LoadGraph("img/CharacterSelect/FighterImage/Chara1Image.png")),
	//�J�[�\��
	m_cursorP1Handle(LoadGraph("img/CharacterSelect/Icon/SelectFrameP1.png")),
	m_cursorP2Handle(LoadGraph("img/CharacterSelect/Icon/SelectFrameP2.png")),
	//SE�̉���
	m_selectSehandle(LoadSoundMem("./SE/Select/SelectSE.mp3")),
	m_cursorMoveSehandle(LoadSoundMem("./SE/Select/CursorMoveSE.mp3")),
	m_cancelSehandle(LoadSoundMem("./SE/Select/CancelSE.mp3")),
	//�w�i
	m_backHandle(LoadGraph("img/CharacterSelect/SelectBack.png")),
	//�e�L�X�g
	m_selectTextHandle(LoadGraph("img/CharacterSelect/SelectText.png"))

	
{
	//BGM
	m_bgm = std::make_shared<BGM>();
	int bgmhandle = LoadSoundMem("./BGM/BGM_SelectScene.mp3");
	m_bgm->SetBGM(bgmhandle);
	m_bgm->Volume(100);
	m_bgm->PlayLoop();
	//SE
	m_seP1 = std::make_shared<SE>();
	m_seP2 = std::make_shared<SE>();
	//�A�C�R��
	for (int i = 0; i < kCommandNum; ++i)
	{
		switch (i)
		{
		case 0:
			m_commandIconHandle[i] = LoadGraph("./img/CharacterSelect/Icon/CommandIcon/Command1.png");
			break;
		case 1:
			m_commandIconHandle[i] = LoadGraph("./img/CharacterSelect/Icon/CommandIcon/Command2.png");
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
	//��ł��邱�Ƃ�\��
	m_nullCommandIconHandle = LoadGraph("./img/CharacterSelect/Icon/CommandIcon/SelectNull.png");
	for (int i = 0; i < kSelectCommandNum; ++i)
	{
		m_selectCommandIconP1Handle[i] = m_nullCommandIconHandle;
		m_selectCommandIconP2Handle[i] = m_nullCommandIconHandle;
	}
	m_gettingReadyHandle = LoadGraph("./img/CharacterSelect/Ready_Off.png");//������
	m_readyHandle = LoadGraph("./img/CharacterSelect/Ready_On.png");//��������
	m_currentReadyP1Handle = m_gettingReadyHandle;//�����������ǂ�����\��
	m_currentReadyP2Handle = m_gettingReadyHandle;

	//�t�F�[�h�C������Ƃ��Ɏg��
	m_fadeManager = std::make_shared<FadeManager>();
	//���[�f�B���O���
	m_loadingHandle = LoadGraph("./img/Loading/NowLoading.png");
}

void CommandSelectScene::Update(Input& input, Input& input2)
{
	//���������Ɏg��
	m_countFrame++;
	//���̃V�[���ł�肽������
	//�L�����N�^�[�����肵���炻�̃L�����N�^�[��
	//�|�C���^�����̃V�[���ɓn������
	//P1�̃Z���N�g
	SelectP1(input);
	//P2�̃Z���N�g
	SelectP2(input2);
	//2�l����������������Q�[���V�[����
	if (m_isReadyP1 && m_isReadyP2)
	{
		m_isFadeIn = true;
		//�t�F�[�h�C�����Ă���V�[���ړ�
		if (m_fadeManager->IsFinishFadeIn())
		{
			//�I�񂾃R�}���h�Z�̃C���f�b�N�X�����̃V�[���ɓn�����߂ɕۑ�
			m_controller.SaveSelectCommandIndex(m_selectCommandIndexP1, m_selectCommandIndexP2);

			//�����ꂽ�玟�̏�Ԃɑ@��
			//���̏�Ԃ͂��̃N���X���o���Ă���
			m_controller.ChangeScene(std::make_shared<GameScene>(m_controller));
			return;//�Y�ꂸreturn
		}
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


void CommandSelectScene::SelectP1(Input& input)
{
	if (m_isSelectFinishP1)
	{
		if (input.IsTrigger("A"))
		{
			//��������
			m_currentReadyP1Handle = m_readyHandle;
			m_isReadyP1 = true;
		}
		//�L�����Z��
		if (input.IsTrigger("B"))
		{
			//������
			if (m_isReadyP1)
			{
				//�m�F�ɕϊ�
				m_currentReadyP1Handle = m_gettingReadyHandle;
				m_isReadyP1 = false;
			}
			else//�܂��I�ђ���
			{
				//�L�����Z���̉�
				m_seP1->Stop();
				m_seP1->SetSE(m_cancelSehandle);
				m_seP1->Volume(150);
				m_seP1->PlayOnce();
				m_isSelectFinishP1 = false;
				for (int i = 0; i < 3; ++i)
				{
					//�Ō�ɑI�񂾏��ɏ���
					if (m_selectCommandIndexP1[2 - i] != 0)
					{
						m_selectCommandIndexP1[2 - i] = 0;
						m_selectCommandIconP1Handle[2 - i] = m_nullCommandIconHandle;
						break;
					}
				}
			}

		}
	}
	else
	{
		//�I��
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
		if (input.IsTrigger("A"))
		{
			//����̉�
			m_seP1->Stop();
			m_seP1->SetSE(m_selectSehandle);
			m_seP1->Volume(150);
			m_seP1->PlayOnce();
			for (int i = 0; i < 3; ++i)
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
				m_isSelectFinishP1 = true;
			}
		}
		//�I�тȂ���
		if (input.IsTrigger("B"))
		{
			//�L�����Z���̉�
			m_seP1->Stop();
			m_seP1->SetSE(m_cancelSehandle);
			m_seP1->Volume(150);
			m_seP1->PlayOnce();
			m_isSelectFinishP1 = false;
			for (int i = 0; i < 3; ++i)
			{
				//�Ō�ɑI�񂾏��ɏ���
				if (m_selectCommandIndexP1[2 - i] != 0)
				{
					m_selectCommandIndexP1[2 - i] = 0;
					m_selectCommandIconP1Handle[2 - i] = m_nullCommandIconHandle;
					break;
				}
			}
		}
		//�������ł��Ă��Ȃ����Ƃ�\��
		m_currentReadyP1Handle = m_gettingReadyHandle;
		m_isReadyP1 = false;
	}
}

void CommandSelectScene::SelectP2(Input& input)
{
	if (m_isSelectFinishP2)
	{
		if (input.IsTrigger("A"))
		{
			//��������
			m_currentReadyP2Handle = m_readyHandle;
			m_isReadyP2 = true;
		}
		//������
		if (input.IsTrigger("B"))
		{
			if (m_isReadyP2)
			{
				//�m�F�ɕϊ�
				m_currentReadyP2Handle = m_gettingReadyHandle;
				m_isReadyP2 = false;
			}
			else//�܂��I�ђ���
			{
				//�L�����Z���̉�
				m_seP2->Stop();
				m_seP2->SetSE(m_cancelSehandle);
				m_seP2->Volume(150);
				m_seP2->PlayOnce();
				m_isSelectFinishP2 = false;
				//�Ō�ɑI�񂾏��ɏ���
				for (int i = 0; i < 3; ++i)
				{
					if (m_selectCommandIndexP2[2 - i] != 0)
					{
						m_selectCommandIndexP2[2 - i] = 0;
						m_selectCommandIconP2Handle[2 - i] = m_nullCommandIconHandle;
						break;
					}
				}
			}
		}
	}
	else
	{
		if (input.IsTrigger("Left"))
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
		if (input.IsTrigger("Right"))
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
		if (input.IsTrigger("Up"))
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
		if (input.IsTrigger("Down"))
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
		//����
		if (input.IsTrigger("A"))
		{
			//����̉�
			m_seP2->Stop();
			m_seP2->SetSE(m_selectSehandle);
			m_seP2->Volume(100);
			m_seP2->PlayOnce();
			for (int i = 0; i < 3; ++i)
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
				m_isSelectFinishP2 = true;
			}
		}
		//�I�тȂ���
		if (input.IsTrigger("B"))
		{
			//�L�����Z���̉�
			m_seP2->Stop();
			m_seP2->SetSE(m_cancelSehandle);
			m_seP2->Volume(150);
			m_seP2->PlayOnce();
			m_isSelectFinishP2 = false;
			//�Ō�ɑI�񂾏��ɏ���
			for (int i = 0; i < 3; ++i)
			{
				if (m_selectCommandIndexP2[2 - i] != 0)
				{
					m_selectCommandIndexP2[2 - i] = 0;
					m_selectCommandIconP2Handle[2 - i] = m_nullCommandIconHandle;
					break;
				}
			}
		}
		m_currentReadyP2Handle = m_gettingReadyHandle;
		m_isReadyP2 = false;
	}
}


void CommandSelectScene::Draw()
{
	//�w�i
	DxLib::DrawGraph(0, 0, m_backHandle, true);
	//�����G
	DxLib::DrawGraph(0, 20, m_imageChara1Handle, true);//1P
	DxLib::DrawTurnGraph(Game::kScreenWidth - kPlayerImageWidth, 20, m_imageChara1Handle, true);//2P

	//�����ł������̊m�F
	if (m_isSelectFinishP1)
	{
		DxLib::DrawGraph(kReadyPosXP1, kReadyPosY, m_currentReadyP1Handle, true);//1P��Ready
	}
	if (m_isSelectFinishP2)
	{
		DxLib::DrawGraph(kReadyPosXP2, kReadyPosY, m_currentReadyP2Handle, true);//2P��Ready
	}
	//�Z�̃A�C�R��
	DrawCommandIcon();

	//�J�[�\��
	DrawCursor();

	//�I�񂾃R�}���h�̕\��
	DrawSelectPlayerCommandIcon();
	//"�R�}���h�Z��3�I��ł�������"�̃e�L�X�g
	DrawRotaGraph(kCenterX, 100, 1.0, 0, m_selectTextHandle, true, 0, 0);

	//�t�F�[�h�C��
	m_fadeManager->FadeDraw(m_isFadeIn);
	//�t�F�[�h�C���������Ă���\��
	if (m_fadeManager->IsFinishFadeIn())
	{
		DxLib::DrawGraph(0, 0, m_loadingHandle, true);
	}
	
#if _DEBUG	
	DxLib::DrawString(10, 10, "CharacterselectScene", 0xffffff);
	for (int i = 0;i < 3;++i)
	{
		DrawFormatString(400, 800 + (20 * i), 0xff3333, "selectNumP1 = %d", m_selectCommandIndexP1[i]);
		DrawFormatString(1000, 800 + (20 * i), 0x0000ff, "selectNumP2 = %d", m_selectCommandIndexP2[i]);
	}
#endif
	
}

void CommandSelectScene::DrawCommandIcon()
{
	//�Z1
	DrawCircle(kCenterX - kIconPosOffset, kCenterY - kIconPosOffset, kIconRadius, 0x22ff22, true, true);
	//�Z2
	DrawCircle(kCenterX, kCenterY - kIconPosOffset, kIconRadius, 0xff2222, true, true);
	//�Z3
	DrawCircle(kCenterX + kIconPosOffset, kCenterY - kIconPosOffset, kIconRadius, 0x2222ff, true, true);
	//�Z4
	DrawCircle(kCenterX - kIconPosOffset, kCenterY, kIconRadius, 0xff22ff, true, true);
	//�Z5
	DrawCircle(kCenterX, kCenterY, kIconRadius, 0x22ffff, true, true);
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
}

void CommandSelectScene::DrawCursor()
{
	//�J�[�\��
	//P1
	//��������
	
	if ((m_countFrame % 10 == 0) && !m_isSelectFinishP1)
	{
		SetDrawBlendMode(DX_BLENDMODE_INVSRC, 255);
	}
	switch (m_currentSelectCommandIndexP1)
	{
	case 1:
		//�Z1
		DxLib::DrawRotaGraph(kCenterX - kIconPosOffset, kCenterY - kIconPosOffset, 1.0, 0, m_cursorP1Handle, true, 0, 0);
		break;
	case 2:
		//�Z2
		DxLib::DrawRotaGraph(kCenterX, kCenterY - kIconPosOffset, 1.0, 0, m_cursorP1Handle, true, 0, 0);
		break;
	case 3:
		//�Z3
		DxLib::DrawRotaGraph(kCenterX + kIconPosOffset, kCenterY - kIconPosOffset, 1.0, 0, m_cursorP1Handle, true, 0, 0);
		break;
	case 4:
		//�Z4
		DxLib::DrawRotaGraph(kCenterX - kIconPosOffset, kCenterY, 1.0, 0, m_cursorP1Handle, true, 0, 0);
		break;
	case 5:
		//�Z5
		DxLib::DrawRotaGraph(kCenterX, kCenterY, 1.0, 0, m_cursorP1Handle, true, 0, 0);
		break;
	case 6:
		//�Z6
		DxLib::DrawRotaGraph(kCenterX + kIconPosOffset, kCenterY, 1.0, 0, m_cursorP1Handle, true, 0, 0);
		break;
	case 7:
		//�Z7
		DxLib::DrawRotaGraph(kCenterX - kIconPosOffset, kCenterY + kIconPosOffset, 1.0, 0, m_cursorP1Handle, true, 0, 0);
		break;
	case 8:
		//�Z8
		DxLib::DrawRotaGraph(kCenterX, kCenterY + kIconPosOffset, 1.0, 0, m_cursorP1Handle, true, 0, 0);
		break;
	case 9:
		//�Z9
		DxLib::DrawRotaGraph(kCenterX + kIconPosOffset, kCenterY + kIconPosOffset, 1.0, 0, m_cursorP1Handle, true, 0, 0);
		break;
	default:
		break;
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);


	//P2
	//��������
	if ((m_countFrame % 10 == 0) && !m_isSelectFinishP2)
	{
		SetDrawBlendMode(DX_BLENDMODE_INVSRC, 255);
	}
	switch (m_currentSelectCommandIndexP2)
	{
	case 1:
		//�Z1
		DxLib::DrawRotaGraph(kCenterX - kIconPosOffset, kCenterY - kIconPosOffset, 1.0, 0, m_cursorP2Handle, true, 0, 0);
		break;
	case 2:
		//�Z2
		DxLib::DrawRotaGraph(kCenterX, kCenterY - kIconPosOffset, 1.0, 0, m_cursorP2Handle, true, 0, 0);
		break;
	case 3:
		//�Z3
		DxLib::DrawRotaGraph(kCenterX + kIconPosOffset, kCenterY - kIconPosOffset, 1.0, 0, m_cursorP2Handle, true, 0, 0);
		break;
	case 4:
		//�Z4
		DxLib::DrawRotaGraph(kCenterX - kIconPosOffset, kCenterY, 1.0, 0, m_cursorP2Handle, true, 0, 0);
		break;
	case 5:
		//�Z5
		DxLib::DrawRotaGraph(kCenterX, kCenterY, 1.0, 0, m_cursorP2Handle, true, 0, 0);
		break;
	case 6:
		//�Z6
		DxLib::DrawRotaGraph(kCenterX + kIconPosOffset, kCenterY, 1.0, 0, m_cursorP2Handle, true, 0, 0);
		break;
	case 7:
		//�Z7
		DxLib::DrawRotaGraph(kCenterX - kIconPosOffset, kCenterY + kIconPosOffset, 1.0, 0, m_cursorP2Handle, true, 0, 0);
		break;
	case 8:
		//�Z8
		DxLib::DrawRotaGraph(kCenterX, kCenterY + kIconPosOffset, 1.0, 0, m_cursorP2Handle, true, 0, 0);
		break;
	case 9:
		//�Z9
		DxLib::DrawRotaGraph(kCenterX + kIconPosOffset, kCenterY + kIconPosOffset, 1.0, 0, m_cursorP2Handle, true, 0, 0);
		break;
	default:
		break;
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

void CommandSelectScene::DrawSelectPlayerCommandIcon()
{
	//�R�}���h
	for (int i = 0; i < 3; ++i)
	{
		//�I�񂾋Z�̃A�C�R��
		DrawRectRotaGraphFast(
			(kCommandIconImageWidth * kCommandIconImageScale) * (i + 1),
			kPlayerImageHeight + (kCommandIconImageHight * kCommandIconImageScale),
			0, 0, kCommandIconImageWidth, kCommandIconImageHight,
			kCommandIconImageScale, 0.0f, m_selectCommandIconP1Handle[i], true);
		DrawRectRotaGraphFast(
			Game::kScreenWidth - ((kCommandIconImageWidth * kCommandIconImageScale) * (i + 1)),
			kPlayerImageHeight + (kCommandIconImageHight * kCommandIconImageScale),
			0, 0, kCommandIconImageWidth, kCommandIconImageHight,
			kCommandIconImageScale, 0.0f, m_selectCommandIconP2Handle[i], true);
	}
}
