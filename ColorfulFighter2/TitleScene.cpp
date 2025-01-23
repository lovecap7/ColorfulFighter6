#include "TitleScene.h"
#include "Dxlib.h"
#include "game.h"
#include "Input.h"
#include "SceneController.h"
#include "CommandSelectScene.h"
#include "ResultScene.h"
#include "BGM.h"

namespace
{
	constexpr int kTextHiddenFrame = 60;
	constexpr int kFrameReset = kTextHiddenFrame * 2;
	constexpr int kTextWidth = 500;
	constexpr int kTextPosX = (Game::kScreenWidth / 2) - (kTextWidth / 2);
	constexpr int kTextPosY = 600;
	//BGMボリューム
	constexpr int kBgmVolume = 120;
	//SEボリューム
	constexpr int kSeVolume = 150;

	//キャラの画像の大きさ
	constexpr int kCharaWidth = 512;
	constexpr int kCharaHeight = 512;
	constexpr float kCharaScale = 3.0f;

	//キャラの初期位置
	constexpr int kActor1PosX = -((kCharaWidth / 2) * kCharaScale);
	constexpr int kActor1PosY = 600;
	constexpr int kActor2PosX = Game::kScreenWidth + ((kCharaWidth / 2) * kCharaScale);
	constexpr int kActor2PosY = 600;

	//再生速度
	constexpr int kIWalkOneAnimFrame = 5;
	constexpr int kIdleOneAnimFrame = 5;
	constexpr int kAttackOneAnimFrame = 5;
	constexpr int kDefenceOneAnimFrame = 10;
	//アニメーションの数
	constexpr int kAnimNum = 6;

	//目と目が合う距離
	constexpr int kEyeContactDistance = 500;
	constexpr int kEyeContactMoveSpeed = 5;

	//殴りあう距離
	constexpr int kFightDistance = 250;
	constexpr int kFightMoveSpeed = 15;
}

void TitleScene::BlinkingTextDraw()
{
	if (m_textBlinkFrame < kTextHiddenFrame)
	{
		DrawGraph(kTextPosX, kTextPosY, m_textHandle, true);
	}
	if (kFrameReset < m_textBlinkFrame)
	{
		m_textBlinkFrame = 0;
	}
}

void TitleScene::NormalUpdate(Input& input, Input& input2)
{
#if _DEBUG	
	if (input.IsTrigger("Start"))
	{
		//勝ったプレイヤーのインデックスをセット
		m_controller.SetWinPlayerIndex(PlayerIndex::Player1);
		//押されたら次の状態に繊維
		//次の状態はこのクラスが覚えておく
		m_controller.ChangeScene(std::make_shared<ResultScene>(m_controller));
		return;//忘れずreturn
	}
#endif

	m_textBlinkFrame++;
	if (input.IsTrigger("A") ||
		input.IsTrigger("B") ||
		input.IsTrigger("X") ||
		input.IsTrigger("Y") ||
		input2.IsTrigger("A") ||
		input2.IsTrigger("B") ||
		input2.IsTrigger("X") ||
		input2.IsTrigger("Y"))
	{
		//押されたら次の状態に繊維
		//次の状態はこのクラスが覚えておく
		m_controller.ChangeScene(std::make_shared<CommandSelectScene>(m_controller));
		return;//忘れずreturn
	}

	//キャラクターが画面外から特定の位置まで歩いてくる
	if (m_actor1Pos.x < (Game::kScreenWidth / 2 - kFightDistance))
	{
		m_actor1Velo.x = kFightMoveSpeed;
		m_actor1Pos += m_actor1Velo;
	}
	if (m_actor2Pos.x > (Game::kScreenWidth / 2 + kFightDistance))
	{
		m_actor2Velo.x = -kFightMoveSpeed;
		m_actor2Pos += m_actor2Velo;
	}

	int attackOneAnimFrame = 0;
	int attackAnimNum = 0;
	//攻撃しているほうのアニメーションに合わせてガードする
	if (m_actor1.handle != m_guardHandle)
	{
		attackOneAnimFrame = m_actor1.oneAnimFrame;
		attackAnimNum = m_actor1.animNum;
	}
	if (m_actor2.handle != m_guardHandle)
	{
		attackOneAnimFrame = m_actor2.oneAnimFrame;
		attackAnimNum = m_actor2.animNum;
	}
	int animMaxNum = attackAnimNum - 1;
	//アニメーションのフレームを数える
	if (m_animCountFrame % attackOneAnimFrame == 0 && m_animCountFrame != 0)
	{
		m_actor1.animIndex++;
		m_actor2.animIndex++;
		int attackAnimIndex = 0;
		if (m_actor1.handle != m_guardHandle)
		{
			attackAnimIndex = m_actor1.animIndex;
		}
		if (m_actor2.handle != m_guardHandle)
		{
			attackAnimIndex = m_actor2.animIndex;
		}
		//アニメーションの数が最大まで行ったとき
		if ((attackAnimIndex > animMaxNum))
		{
			m_actor1.animIndex = 0;
			m_actor2.animIndex = 0;
			if (m_actor1.handle != m_guardHandle)
			{
				if (GetRand(1))
				{
					m_actor1.handle = m_kickHandle;
				}
				else
				{
					m_actor1.handle = m_punchHandle;
				}
			}
			if (m_actor2.handle != m_guardHandle)
			{
				if (GetRand(1))
				{
					m_actor2.handle = m_kickHandle;
				}
				else
				{
					m_actor2.handle = m_punchHandle;
				}
			}

			int tmpHandle = m_actor1.handle;
			m_actor1.handle = m_actor2.handle;
			m_actor2.handle = tmpHandle;
			int tmpOneAnimFrame = m_actor1.oneAnimFrame;
			m_actor1.oneAnimFrame = m_actor2.oneAnimFrame;
			m_actor2.oneAnimFrame = tmpOneAnimFrame;
		}
	}
	//タイトルがだんだんはっきりしてくる
	if (m_titleFadeCountFrame < 255)
	{
		m_titleFadeCountFrame += 5;
	}
}

void TitleScene::NormalDraw()
{
#if _DEBUG	
	DrawString(10, 10, "Title Scene", 0xffffff);
#endif
	ActorDraw();
	BlinkingTextDraw();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_titleFadeCountFrame);
	DrawGraph(205, -5, m_titleHandle, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TitleScene::OpeningUpdate(Input& input, Input& input2)
{
	m_actor1Velo.x = 0;
	m_actor2Velo.x = 0;
	//キャラクターが画面外から特定の位置まで歩いてくる
	if (m_actor1Pos.x < (Game::kScreenWidth / 2 - kEyeContactDistance))
	{
		m_actor1Velo.x = kEyeContactMoveSpeed;
		m_actor1Pos += m_actor1Velo;
	}
	else
	{
		m_actor1.handle = m_idleHandle;
	}
	if (m_actor2Pos.x > (Game::kScreenWidth / 2 + kEyeContactDistance))
	{
		m_actor2Velo.x = -kEyeContactMoveSpeed;
		m_actor2Pos += m_actor2Velo;
	}
	else
	{
		m_actor2.handle = m_idleHandle;
	}
	//アニメーションの1枚目を0番として数えるので
	//アニメーションの最大数から-1した値が最後のアニメーション
	int animMaxNum = m_actor1.animNum - 1;
	//アニメーションのフレームを数える
	if (m_animCountFrame % m_actor1.oneAnimFrame == 0 && m_animCountFrame != 0)
	{
		m_actor1.animIndex++;
		m_actor2.animIndex++;
		//アニメーションの数が最大まで行ったとき
		if ((m_actor1.animIndex > animMaxNum))
		{
			m_actor1.animIndex = 0;
			m_actor2.animIndex = 0;

			if (m_actor1.handle == m_idleHandle && m_actor2.handle == m_idleHandle)
			{
				m_actor1.handle = m_guardHandle;
				m_actor1.oneAnimFrame = kDefenceOneAnimFrame;
				m_actor2.handle = m_punchHandle;
				m_actor2.oneAnimFrame = kAttackOneAnimFrame;
				m_update = &TitleScene::NormalUpdate;
				m_draw = &TitleScene::NormalDraw;
				return;
			}
		}
	}
}

void TitleScene::OpeningDraw()
{
	ActorDraw();
}

void TitleScene::DemoUpdate(Input& input, Input& input2)
{
	
}

void TitleScene::DemoDraw()
{
	
}

//裏で戦っているキャラクター
void TitleScene::ActorDraw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	SetDrawBright(0, 0, 0);
	//切り取るを計算する
	int sizeX, sizeY;
	GetGraphSize(m_actor1.handle, &sizeX, &sizeY);//画像サイズ
	int cutX = m_actor1.animIndex % (sizeX / kCharaWidth);//横
	int cutY = m_actor1.animIndex / (sizeX / kCharaWidth);//縦
	//描画
	DrawRectRotaGraphFast(m_actor1Pos.x, m_actor1Pos.y,
		kCharaWidth * cutX,
		kCharaHeight * cutY,
		kCharaWidth, kCharaHeight,
		kCharaScale, 0.0f, m_actor1.handle, true, false);
	GetGraphSize(m_actor2.handle, &sizeX, &sizeY);//画像サイズ
	cutX = m_actor2.animIndex % (sizeX / kCharaWidth);//横
	cutY = m_actor2.animIndex / (sizeX / kCharaWidth);//縦
	//描画
	DrawRectRotaGraphFast(m_actor2Pos.x, m_actor2Pos.y,
		kCharaWidth * cutX,
		kCharaHeight * cutY,
		kCharaWidth, kCharaHeight,
		kCharaScale, 0.0f, m_actor2.handle, true, true);
	SetDrawBright(255, 255, 255);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

TitleScene::TitleScene(SceneController& contoller) :
	SceneBase(contoller),
	m_backHandle(LoadGraph("./img/title/TitleBack.png")),
	m_titleHandle(LoadGraph("./img/title/Title.png")),
	m_titleFadeCountFrame(0),
	m_textHandle(LoadGraph("./img/title/PressAnyButton.png")),
	m_textBlinkFrame(0),
	m_update(&TitleScene::OpeningUpdate),
	m_draw(&TitleScene::OpeningDraw),
	m_idleHandle(LoadGraph("./img/Chara/White/playerbase/idle_001.png")),
	m_punchHandle(LoadGraph("./img/Chara/White/punch/punch_stand_002.png")),
	m_kickHandle(LoadGraph("./img/Chara/White/kick/kick_stand_001.png")),
	m_guardHandle(LoadGraph("./img/Chara/White/guard/guard_stand_001.png")),
	m_walkHandle(LoadGraph("./img/Chara/White/playerbase/walk_front_001.png")),
	m_actor1(-1, kAnimNum, kIWalkOneAnimFrame),
	m_actor2(-1, kAnimNum, kIWalkOneAnimFrame),
	m_animCountFrame(0),
	m_actor1Pos(Vector3(kActor1PosX, kActor1PosY, 0)),
	m_actor1Velo(Vector3(0, 0, 0)),
	m_actor2Pos(Vector3(kActor2PosX, kActor2PosY, 0)),
	m_actor2Velo(Vector3(0, 0, 0))
{
	m_actor1.handle = m_walkHandle;
	m_actor2.handle = m_walkHandle;
	m_bgm = std::make_shared<BGM>();
	int bgmhandle = LoadSoundMem("./BGM/BGM_Title.mp3");
	m_bgm->SetBGM(bgmhandle);
	m_bgm->Volume(kBgmVolume);
	m_bgm->PlayLoop();
}

void TitleScene::Update(Input& input, Input& input2)
{
	m_animCountFrame++;
	(this->*m_update)(input, input2);
}

void TitleScene::Draw()
{
	DrawGraph(0, 0, m_backHandle, true);
	(this->*m_draw)();
}
