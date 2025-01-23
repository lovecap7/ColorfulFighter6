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

	//プレイヤーの画像の大きさ
	constexpr int kPlayerWidth = 512;
	constexpr int kPlayerHeight = 512;
	constexpr float kPlayerScale = 3.0f;
	//再生速度
	constexpr int kAttackAnimFrame = 5;
	constexpr int kDeffenceAnimFrame = 5;
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

			int tmp = m_actor1.handle;
			m_actor1.handle = m_actor2.handle;
			m_actor2.handle = tmp;
		}
	}
}

void TitleScene::NormalDraw()
{
#if _DEBUG	
	DrawString(10, 10, "Title Scene", 0xffffff);
#endif
	ActorDraw();
	BlinkingTextDraw();
	DrawGraph(205, -5, m_titleHandle, true);
}

void TitleScene::OpeningUpdate(Input& input, Input& input2)
{
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
				m_actor1.animNum = 6;
				m_actor2.animNum = 6;
				m_actor1.handle = m_guardHandle;
				m_actor2.handle = m_punchHandle;
				m_update = &TitleScene::NormalUpdate;
				m_draw = &TitleScene::NormalDraw;
				return;
			}
		}
	}
	m_actor1Velo.x = 0;
	m_actor2Velo.x = 0;
	//キャラクターが画面外から特定の位置まで歩いてくる
	if (m_actor1Pos.x < (Game::kScreenWidth / 2 - 500))
	{
		m_actor1Velo.x = 2;
		m_actor1Pos += m_actor1Velo;
	}
	else
	{
		m_actor1.handle = m_idleHandle;
		m_actor1.animNum = 6;
	}
	if (m_actor2Pos.x > (Game::kScreenWidth / 2 + 500))
	{
		m_actor2Velo.x = -2;
		m_actor2Pos += m_actor2Velo;
	}
	else
	{
		m_actor2.handle = m_idleHandle;
		m_actor2.animNum = 6;
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
	int cutX = m_actor1.animIndex % (sizeX / kPlayerWidth);//横
	int cutY = m_actor1.animIndex / (sizeX / kPlayerWidth);//縦
	//描画
	DrawRectRotaGraphFast(m_actor1Pos.x, m_actor1Pos.y,
		kPlayerWidth * cutX,
		kPlayerHeight * cutY,
		kPlayerWidth, kPlayerHeight,
		kPlayerScale, 0.0f, m_actor1.handle, true, false);
	GetGraphSize(m_actor2.handle, &sizeX, &sizeY);//画像サイズ
	cutX = m_actor2.animIndex % (sizeX / kPlayerWidth);//横
	cutY = m_actor2.animIndex / (sizeX / kPlayerWidth);//縦
	//描画
	DrawRectRotaGraphFast(m_actor2Pos.x, m_actor2Pos.y,
		kPlayerWidth * cutX,
		kPlayerHeight * cutY,
		kPlayerWidth, kPlayerHeight,
		kPlayerScale, 0.0f, m_actor2.handle, true, true);
	SetDrawBright(255, 255, 255);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

TitleScene::TitleScene(SceneController& contoller) :
	SceneBase(contoller),
	m_backHandle(LoadGraph("./img/title/TitleBack.png")),
	m_titleHandle(LoadGraph("./img/title/Title.png")),
	m_textHandle(LoadGraph("./img/title/PressAnyButton.png")),
	m_textBlinkFrame(0),
	m_update(&TitleScene::OpeningUpdate),
	m_draw(&TitleScene::OpeningDraw),
	m_idleHandle(LoadGraph("./img/Chara/White/playerbase/idle_001.png")),
	m_punchHandle(LoadGraph("./img/Chara/White/punch/punch_stand_002.png")),
	m_kickHandle(LoadGraph("./img/Chara/White/kick/kick_stand_002.png")),
	m_guardHandle(LoadGraph("./img/Chara/White/guard/guard_stand_001.png")),
	m_shouryuuHandle(LoadGraph("./img/Chara/White/waza/shouryuu.png")),
	m_walkHandle(LoadGraph("./img/Chara/White/playerbase/walk_front_001.png")),
	m_actor1(m_walkHandle, 6,kAttackAnimFrame),
	m_actor2(m_walkHandle, 6,kDeffenceAnimFrame),
	m_animCountFrame(0),
	m_actor1Pos(Vector3(0, kTextPosY, 0)),
	m_actor1Velo(Vector3(0, 0, 0)),
	m_actor2Pos(Vector3(Game::kScreenWidth, kTextPosY, 0)),
	m_actor2Velo(Vector3(0, 0, 0))
{
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
