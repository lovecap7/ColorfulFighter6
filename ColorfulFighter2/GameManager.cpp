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
	//ヒットストップの時間
	constexpr int kHitStopFinishFrame = 8;
	constexpr int kKoStopFinishFrame = 50;

	//2本先に取ったら勝ち
	constexpr int kGamesetWinNum = 2;

	//ラウンド切り替わりの時間
	constexpr int kChangeRoundFrame = 300;//5秒	

	//ラウンド開始の時間
	constexpr int kStartRoundFrame = 180;//2秒

	//1ラウンドの時間
	constexpr int kOneFrameTime = 60;
	constexpr int kRoundTime = 99;//99秒

	//ヒットエフェクトのアニメーション
	constexpr int kHitAnimFirstIndex = 236;
	constexpr int kHitAnimFinishIndex = 241;
	//ガードエフェクトのアニメーション
	constexpr int kGuardAnimFirstIndex = 205;
	constexpr int kGuardAnimFinishIndex = 210;
	
	//seボリューム
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
	//ヒットエフェクト
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
	//エフェクト
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
	m_hitStopFrameCount = 0;//ヒットストップのフレームを数える
	m_hitStopFinishFrame = kHitStopFinishFrame;
	m_changeRoundFrameCount = 0;//ラウンドの切り替わりのフレーム
	m_isHitStop = false;
	m_isChangeRound = false;
	m_isTimeUpOrKo = false;
	m_isStartRound = false;
	//フェードアウト
	m_isFadeIn = false;
	//タイマー
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
	//KOしてるかチェック
	CheckKO(p1, p2);

	//試合が開始したらカウントを進める
	if (m_isStartRound)
	{
		//試合時間
		Timer();
	}

	//フェードアウトしたらゲーム開始の準備
	if (m_fadeManager->GetFadeFrameCount() <= 0)
	{
		//ゲーム開始時
		m_startRoundCount++;
		//ラウンドコールSE
		if (m_startRoundCount >= 120 && m_startRoundCount < 180)
		{
			//Fight!
			m_se->SetSE(m_fightSeHandle);
			m_se->Volume(kSeVolume);
			m_se->PlayOnce();
		}
		else if (m_startRoundCount < 120)
		{
			//ラウンド1
			if (m_roundNumber == 1)
			{
				m_se->SetSE(m_round1SeHandle);
			}
			//ラウンド2
			else if (m_roundNumber == 2)
			{
				m_se->SetSE(m_round2SeHandle);
			}
			//ラウンド3
			else if (m_roundNumber == 3)
			{
				m_se->SetSE(m_round3SeHandle);
			}
			//ラウンドOver
			else
			{
				m_se->SetSE(m_roundOverSeHandle);
			}
			//Fight!
			m_se->Volume(kSeVolume);
			m_se->PlayOnce();
		}
	
		//ラウンドスタート
		if ((m_startRoundCount > kStartRoundFrame) && !m_isStartRound)
		{
			m_isStartRound = true;
		}
	}
	//ヒットストップ
	HitStop();

	//ヒットエフェクトのアニメーション
	//P1
	m_hitAnimCountFrameP1++;
	//アニメーションのフレームを数える
	if (m_hitAnimCountFrameP1 % 4 == 0 && m_hitAnimCountFrameP1 != 0)
	{
		m_hitAnimIndexP1++;
		//アニメーションの数が最大まで行ったとき
		if ((m_hitAnimIndexP1 >= m_hitAnimFinishIndexP1))
		{
			m_hitAnimIndexP1 = m_hitAnimFinishIndexP1;
		}
	}
	//P2
	m_hitAnimCountFrameP2++;
	//アニメーションのフレームを数える
	if (m_hitAnimCountFrameP2 % 4 == 0 && m_hitAnimCountFrameP2 != 0)
	{
		m_hitAnimIndexP2++;
		//アニメーションの数が最大まで行ったとき
		if ((m_hitAnimIndexP2 >= m_hitAnimFinishIndexP2))
		{
			m_hitAnimIndexP2 = m_hitAnimFinishIndexP2;
		}
	}
}

void GameManager::Draw(Camera& camera)
{
	FadeDraw();//フェード
	//ヒットエフェクト
	HitEffectDraw(camera);
}

void GameManager::FadeDraw()
{
	//フェードインアウト
	m_fadeManager->FadeDraw(m_isFadeIn);
}

void GameManager::HitEffectDraw(Camera& camera)
{
	//描画
	//P1
	//切り取るを計算する
	int sizeX, sizeY;
	int cutX, cutY;
	GetGraphSize(m_hitEffectHandleP1, &sizeX, &sizeY);//画像サイズ
	cutX = m_hitAnimIndexP1 % (sizeX / 32);//横
	cutY = m_hitAnimIndexP1 / (sizeX / 32);//縦
	DrawRectRotaGraphFast(static_cast<int>(m_hitEffectPosP1.x) + static_cast<int>(camera.m_drawOffset.x),
		static_cast<int>(m_hitEffectPosP1.y) + static_cast<int>(camera.m_drawOffset.y),
		32 * cutX,
		32 * cutY,
		32, 32,
		5.0f, 0.0f, m_hitEffectHandleP1, true, false);
#if _DEBUG
	DrawFormatString(0, 700, 0xffff00, "P1のヒットエフェクト(%2.0f,%2.0f)", m_hitEffectPosP1.x, m_hitEffectPosP1.y);
#endif
	
	//P2
	//切り取るを計算する
	GetGraphSize(m_hitEffectHandleP2, &sizeX, &sizeY);//画像サイズ
	cutX = m_hitAnimIndexP2 % (sizeX / 32);//横
	cutY = m_hitAnimIndexP2 / (sizeX / 32);//縦
	DrawRectRotaGraphFast(static_cast<int>(m_hitEffectPosP2.x) + static_cast<int>(camera.m_drawOffset.x),
		static_cast<int>(m_hitEffectPosP2.y) + static_cast<int>(camera.m_drawOffset.y),
		32 * cutX,
		32 * cutY,
		32, 32,
		5.0f, 0.0f, m_hitEffectHandleP2, true, true);
#if _DEBUG
	DrawFormatString(0, 720, 0xffff00, "P2のヒットエフェクト(%2.0f,%2.0f)", m_hitEffectPosP2.x, m_hitEffectPosP2.y);
#endif
}

void GameManager::LoadHitEffect(PlayerIndex playerIndex)
{
	if (playerIndex == PlayerIndex::Player1)
	{
		//ヒットハンドルをセット
		m_hitEffectHandleP1 = m_hitHandle;
		m_hitAnimCountFrameP1 = 0;
		m_hitAnimIndexP1 = kHitAnimFirstIndex;
		m_hitAnimFinishIndexP1 = kHitAnimFinishIndex;
	}
	else if (playerIndex == PlayerIndex::Player2)
	{
		//ヒットハンドルをセット
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
		//ヒットハンドルをセット
		m_hitEffectHandleP1 = m_guardHandle;
		m_hitAnimCountFrameP1 = 0;
		m_hitAnimIndexP1 = kGuardAnimFirstIndex;
		m_hitAnimFinishIndexP1 = kGuardAnimFinishIndex;
	}
	else if (playerIndex == PlayerIndex::Player2)
	{
		//ヒットハンドルをセット
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



//試合時間
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
	//どちらかのHPが尽きたらまたは時間が0になったら
	if (p1.GetHp() <= 0 || p2.GetHp() <= 0 || m_time <= 0)
	{
		//次のラウンドに切り替えるまでのフレームを数える
		m_changeRoundFrameCount++;
		//長めにヒットストップをかける
		m_hitStopFinishFrame = kKoStopFinishFrame;
		//Fight!
		m_se->SetSE(m_koSeHandle);
		m_se->Volume(kSeVolume);
		m_se->PlayOnce();
		//相打ち
		if ((p1.GetHp() <= 0 && p2.GetHp() <= 0) || (p1.GetHp() == p2.GetHp() && m_time <= 0))
		{
			//引き分けはそのままラウンドチェンジ
			return;
		}
		//P1の勝ち
		else if (((p1.GetHp() > 0 && p2.GetHp() <= 0) || (p1.GetHp() > p2.GetHp() && m_time <= 0)) && !m_isTimeUpOrKo)
		{
			m_isTimeUpOrKo = true;
			m_winNumP1++;
		}
		//P2の勝ち
		else if (((p1.GetHp() <= 0 && p2.GetHp() > 0) || (p1.GetHp() < p2.GetHp() && m_time <= 0)) && !m_isTimeUpOrKo)
		{
			m_isTimeUpOrKo = true;
			m_winNumP2++;
		}
	}

	//ラウンドをゲームシーンで切り替え
	if (m_changeRoundFrameCount > kChangeRoundFrame && !m_isGameset)
	{
		//フェードインしきったらラウンドを切り替え
		m_isFadeIn = true;
		if (m_fadeManager->GetFadeFrameCount() >= 255)
		{
			//2本とったらゲーム終了
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
			//次のラウンド
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
	//ヒットストップ
	if (m_isHitStop)
	{
		//動き出す
		if (m_hitStopFrameCount >= m_hitStopFinishFrame)
		{
			m_hitStopFrameCount = 0;
			m_isHitStop = false;
		}
		m_hitStopFrameCount++;
	}
}






