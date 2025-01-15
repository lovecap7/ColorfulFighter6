#include "GameManager.h"
#include "CollisionCheck.h"
#include "Player.h"
#include "Camera.h"
#include "UI.h"
#include "FadeManager.h"
#include "game.h"
#include "SE.h"
#include <cmath>
#include <DxLib.h>

namespace
{
	//�q�b�g�X�g�b�v�̎���
	constexpr int kHitStopFinishFrame = 8;
	constexpr int kKoStopFinishFrame = 50;

	//2�{��Ɏ�����珟��
	constexpr int kGamesetWinNum = 2;

	//���E���h�؂�ւ��̎���
	constexpr int kChangeRoundFrame = 300;//5�b	

	//���E���h�J�n�̎���
	constexpr int kStartRoundFrame = 180;//2�b

	//1���E���h�̎���
	constexpr int kOneFrameTime = 60;
	constexpr int kRoundTime = 99;//99�b

	//�q�b�g�G�t�F�N�g�̃A�j���[�V����
	constexpr int kHitAnimFirstIndex = 236;
	constexpr int kHitAnimFinishIndex = 241;
	//�K�[�h�G�t�F�N�g�̃A�j���[�V����
	constexpr int kGuardAnimFirstIndex = 205;
	constexpr int kGuardAnimFinishIndex = 210;
	
	//se�{�����[��
	constexpr int kSeVolume = 120;
}

GameManager::GameManager():
	m_hitStopFrameCount(0),
	m_changeRoundFrameCount(0),
	m_isHitStop(false),
	m_hitStopFinishFrame(kHitStopFinishFrame),
	m_isChangeRound(false),
	m_isGameset(false),
	m_isTimeUpOrKo(false),
	m_isStartRound(false),
	m_startRoundCount(0),
	m_winNumP1(0),
	m_winNumP2(0),
	m_roundNumber(1),
	m_isFadeIn(false),
	m_time(kRoundTime),
	m_countFrame(0),
	m_isDrawFrontP1(false),
	//�q�b�g�G�t�F�N�g
	//P1
	m_hitEffectPosP1(),
	m_hitAnimCountFrameP1(0),
	m_hitAnimIndexP1(kHitAnimFinishIndex),
	m_hitAnimFinishIndexP1(kHitAnimFinishIndex),
	//P2
	m_hitEffectPosP2(),
	m_hitAnimCountFrameP2(0),
	m_hitAnimIndexP2(kHitAnimFinishIndex),
	m_hitAnimFinishIndexP2(kHitAnimFinishIndex)
{
	m_collcheck = std::make_shared< CollisionCheck>();
	m_fadeManager = std::make_shared<FadeManager>();
	//�G�t�F�N�g
	m_hitHandle = LoadGraph("./img/Bullet/YellowBullet32x32.png");
	m_guardHandle = LoadGraph("./img/Bullet/BlueBullet32x32.png");
	m_hitEffectHandleP1 = m_hitHandle;
	m_hitEffectHandleP2 = m_hitHandle;
	//SE
	m_se = std::make_shared<SE>();
	m_round1SeHandle = LoadSoundMem("./SE/Round/Round1.mp3");
	m_round2SeHandle = LoadSoundMem("./SE/Round/Round2.mp3");
	m_round3SeHandle = LoadSoundMem("./SE/Round/Round3.mp3");
	m_roundOverSeHandle = LoadSoundMem("./SE/Round/RoundOver.mp3");
	m_fightSeHandle = LoadSoundMem("./SE/Round/Fight.mp3");
	m_koSeHandle = LoadSoundMem("./SE/uwauwa/uwauwa.mp3");
}

GameManager::~GameManager()
{
}

void GameManager::Init()
{
	m_hitStopFrameCount = 0;//�q�b�g�X�g�b�v�̃t���[���𐔂���
	m_hitStopFinishFrame = kHitStopFinishFrame;
	m_changeRoundFrameCount = 0;//���E���h�̐؂�ւ��̃t���[��
	m_isHitStop = false;
	m_isChangeRound = false;
	m_isTimeUpOrKo = false;
	m_isStartRound = false;
	//�t�F�[�h�A�E�g
	m_isFadeIn = false;
	//�^�C�}�[
	m_time = kRoundTime;
	m_countFrame = 0;
}

void GameManager::Update(Player& p1, Player& p2, Bullet& bulletP1, Bullet& bulletP2, 
	Camera& camera, UI& ui)
{
	m_collcheck->Update(p1, p2,bulletP1,bulletP2, camera,*this);

#if _DEBUG
	DrawFormatString(0, 400, 0xffffff, "p1(%2.0f,%2.0f) p2(%2.0f,%2.0f)", p1.GetPos().x, p1.GetPos().y, p2.GetPos().x, p2.GetPos().y);
#endif
}

void GameManager::NoStopUpdate(Player& p1, Player& p2)
{
	//KO���Ă邩�`�F�b�N
	CheckKO(p1, p2);

	//�������J�n������J�E���g��i�߂�
	if (m_isStartRound)
	{
		//��������
		Timer();
	}

	//�t�F�[�h�A�E�g������Q�[���J�n�̏���
	if (m_fadeManager->GetFadeFrameCount() <= 0)
	{
		//�Q�[���J�n��
		m_startRoundCount++;
		//���E���h�R�[��SE
		if (m_startRoundCount >= 120 && m_startRoundCount < 180)
		{
			//Fight!
			m_se->SetSE(m_fightSeHandle);
			m_se->Volume(kSeVolume);
			m_se->PlayOnce();
		}
		else if (m_startRoundCount < 120)
		{
			//���E���h1
			if (m_roundNumber == 1)
			{
				m_se->SetSE(m_round1SeHandle);
			}
			//���E���h2
			else if (m_roundNumber == 2)
			{
				m_se->SetSE(m_round2SeHandle);
			}
			//���E���h3
			else if (m_roundNumber == 3)
			{
				m_se->SetSE(m_round3SeHandle);
			}
			//���E���hOver
			else
			{
				m_se->SetSE(m_roundOverSeHandle);
			}
			//Fight!
			m_se->Volume(kSeVolume);
			m_se->PlayOnce();
		}
	
		//���E���h�X�^�[�g
		if ((m_startRoundCount > kStartRoundFrame) && !m_isStartRound)
		{
			m_isStartRound = true;
		}
	}
	//�q�b�g�X�g�b�v
	HitStop();

	//�q�b�g�G�t�F�N�g�̃A�j���[�V����
	//P1
	m_hitAnimCountFrameP1++;
	//�A�j���[�V�����̃t���[���𐔂���
	if (m_hitAnimCountFrameP1 % 4 == 0 && m_hitAnimCountFrameP1 != 0)
	{
		m_hitAnimIndexP1++;
		//�A�j���[�V�����̐����ő�܂ōs�����Ƃ�
		if ((m_hitAnimIndexP1 >= m_hitAnimFinishIndexP1))
		{
			m_hitAnimIndexP1 = m_hitAnimFinishIndexP1;
		}
	}
	//P2
	m_hitAnimCountFrameP2++;
	//�A�j���[�V�����̃t���[���𐔂���
	if (m_hitAnimCountFrameP2 % 4 == 0 && m_hitAnimCountFrameP2 != 0)
	{
		m_hitAnimIndexP2++;
		//�A�j���[�V�����̐����ő�܂ōs�����Ƃ�
		if ((m_hitAnimIndexP2 >= m_hitAnimFinishIndexP2))
		{
			m_hitAnimIndexP2 = m_hitAnimFinishIndexP2;
		}
	}
}

void GameManager::Draw(Camera& camera)
{
	FadeDraw();//�t�F�[�h
	//�q�b�g�G�t�F�N�g
	HitEffectDraw(camera);
}

void GameManager::FadeDraw()
{
	//�t�F�[�h�C���A�E�g
	m_fadeManager->FadeDraw(m_isFadeIn);
}

void GameManager::HitEffectDraw(Camera& camera)
{
	//�`��
	//P1
	//�؂�����v�Z����
	int sizeX, sizeY;
	int cutX, cutY;
	GetGraphSize(m_hitEffectHandleP1, &sizeX, &sizeY);//�摜�T�C�Y
	cutX = m_hitAnimIndexP1 % (sizeX / 32);//��
	cutY = m_hitAnimIndexP1 / (sizeX / 32);//�c
	DrawRectRotaGraphFast(static_cast<int>(m_hitEffectPosP1.x) + static_cast<int>(camera.m_drawOffset.x),
		static_cast<int>(m_hitEffectPosP1.y) + static_cast<int>(camera.m_drawOffset.y),
		32 * cutX,
		32 * cutY,
		32, 32,
		5.0f, 0.0f, m_hitEffectHandleP1, true, false);
#if _DEBUG
	DrawFormatString(0, 700, 0xffff00, "P1�̃q�b�g�G�t�F�N�g(%2.0f,%2.0f)", m_hitEffectPosP1.x, m_hitEffectPosP1.y);
#endif
	
	//P2
	//�؂�����v�Z����
	GetGraphSize(m_hitEffectHandleP2, &sizeX, &sizeY);//�摜�T�C�Y
	cutX = m_hitAnimIndexP2 % (sizeX / 32);//��
	cutY = m_hitAnimIndexP2 / (sizeX / 32);//�c
	DrawRectRotaGraphFast(static_cast<int>(m_hitEffectPosP2.x) + static_cast<int>(camera.m_drawOffset.x),
		static_cast<int>(m_hitEffectPosP2.y) + static_cast<int>(camera.m_drawOffset.y),
		32 * cutX,
		32 * cutY,
		32, 32,
		5.0f, 0.0f, m_hitEffectHandleP2, true, true);
#if _DEBUG
	DrawFormatString(0, 720, 0xffff00, "P2�̃q�b�g�G�t�F�N�g(%2.0f,%2.0f)", m_hitEffectPosP2.x, m_hitEffectPosP2.y);
#endif
}

void GameManager::LoadHitEffect(PlayerIndex playerIndex)
{
	if (playerIndex == PlayerIndex::Player1)
	{
		//�q�b�g�n���h�����Z�b�g
		m_hitEffectHandleP1 = m_hitHandle;
		m_hitAnimCountFrameP1 = 0;
		m_hitAnimIndexP1 = kHitAnimFirstIndex;
		m_hitAnimFinishIndexP1 = kHitAnimFinishIndex;
	}
	else if (playerIndex == PlayerIndex::Player2)
	{
		//�q�b�g�n���h�����Z�b�g
		m_hitEffectHandleP2 = m_hitHandle;
		m_hitAnimCountFrameP2 = 0;
		m_hitAnimIndexP2 = kHitAnimFirstIndex;
		m_hitAnimFinishIndexP2 = kHitAnimFinishIndex;
	}
}

void GameManager::LoadGuardEffect(PlayerIndex playerIndex)
{
	if (playerIndex == PlayerIndex::Player1)
	{
		//�q�b�g�n���h�����Z�b�g
		m_hitEffectHandleP1 = m_guardHandle;
		m_hitAnimCountFrameP1 = 0;
		m_hitAnimIndexP1 = kGuardAnimFirstIndex;
		m_hitAnimFinishIndexP1 = kGuardAnimFinishIndex;
	}
	else if (playerIndex == PlayerIndex::Player2)
	{
		//�q�b�g�n���h�����Z�b�g
		m_hitEffectHandleP2 = m_guardHandle;
		m_hitAnimCountFrameP2 = 0;
		m_hitAnimIndexP2 = kGuardAnimFirstIndex;
		m_hitAnimFinishIndexP2 = kGuardAnimFinishIndex;
	}
}

PlayerIndex GameManager::ResultWinPlayerIndex()
{
	if (m_winNumP1 > m_winNumP2)
	{
		return PlayerIndex::Player1;
	}
	else if (m_winNumP1 < m_winNumP2)
	{
		return PlayerIndex::Player2;
	}
}



//��������
void GameManager::Timer()
{
	m_countFrame++;
	if ((m_countFrame >= kOneFrameTime) && !m_isTimeUpOrKo)
	{
		m_time--;
		m_countFrame = 0;
	}
	
	if (m_time <= 0)
	{
		m_time = 0;
	}
}

void GameManager::CheckKO(Player& p1, Player& p2)
{
	//�ǂ��炩��HP���s������܂��͎��Ԃ�0�ɂȂ�����
	if (p1.GetHp() <= 0 || p2.GetHp() <= 0 || m_time <= 0)
	{
		//���̃��E���h�ɐ؂�ւ���܂ł̃t���[���𐔂���
		m_changeRoundFrameCount++;
		//���߂Ƀq�b�g�X�g�b�v��������
		m_hitStopFinishFrame = kKoStopFinishFrame;
		//Fight!
		m_se->SetSE(m_koSeHandle);
		m_se->Volume(kSeVolume);
		m_se->PlayOnce();
		//���ł�
		if ((p1.GetHp() <= 0 && p2.GetHp() <= 0) || (p1.GetHp() == p2.GetHp() && m_time <= 0))
		{
			//���������͂��̂܂܃��E���h�`�F���W
			return;
		}
		//P1�̏���
		else if (((p1.GetHp() > 0 && p2.GetHp() <= 0) || (p1.GetHp() > p2.GetHp() && m_time <= 0)) && !m_isTimeUpOrKo)
		{
			m_isTimeUpOrKo = true;
			m_winNumP1++;
		}
		//P2�̏���
		else if (((p1.GetHp() <= 0 && p2.GetHp() > 0) || (p1.GetHp() < p2.GetHp() && m_time <= 0)) && !m_isTimeUpOrKo)
		{
			m_isTimeUpOrKo = true;
			m_winNumP2++;
		}
	}

	//���E���h���Q�[���V�[���Ő؂�ւ�
	if (m_changeRoundFrameCount > kChangeRoundFrame && !m_isGameset)
	{
		//�t�F�[�h�C�����������烉�E���h��؂�ւ�
		m_isFadeIn = true;
		if (m_fadeManager->GetFadeFrameCount() >= 255)
		{
			//2�{�Ƃ�����Q�[���I��
			if (m_winNumP1 >= kGamesetWinNum)
			{
				m_isGameset = true;
				return;
			}
			else if (m_winNumP2 >= kGamesetWinNum)
			{
				m_isGameset = true;
				return;
			}
			//���̃��E���h
			m_isChangeRound = true;
			m_isTimeUpOrKo = false;
			m_isStartRound = false;
			m_startRoundCount = 0;
			m_roundNumber++;
			return;
		}
	}
}

void GameManager::HitStop()
{
	//�q�b�g�X�g�b�v
	if (m_isHitStop)
	{
		//�����o��
		if (m_hitStopFrameCount >= m_hitStopFinishFrame)
		{
			m_hitStopFrameCount = 0;
			m_isHitStop = false;
		}
		m_hitStopFrameCount++;
	}
}






