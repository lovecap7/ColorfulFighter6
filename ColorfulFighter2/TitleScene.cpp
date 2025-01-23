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
	constexpr float kPlayerScale = 5.0f;
	//再生速度
	constexpr int kAttackAnimFrame = 5;
	constexpr int kDeffenceAnimFrame = 10;
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

	m_animCountFrame++;
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
		}
	}
}

void TitleScene::NormalDraw()
{
#if _DEBUG	
	DrawString(10, 10, "Title Scene", 0xffffff);
#endif

	DrawGraph(0, 0, m_titleHandle, true);
	BlinkingTextDraw();
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
	DrawRectRotaGraphFast(200, kTextPosY,
		kPlayerWidth * cutX,
		kPlayerHeight * cutY,
		kPlayerWidth, kPlayerHeight,
		kPlayerScale, 0.0f, m_actor1.handle, true, false);
	GetGraphSize(m_actor2.handle, &sizeX, &sizeY);//画像サイズ
	cutX = m_actor2.animIndex % (sizeX / kPlayerWidth);//横
	cutY = m_actor2.animIndex / (sizeX / kPlayerWidth);//縦
	//描画
	DrawRectRotaGraphFast(1400, kTextPosY,
		kPlayerWidth * cutX,
		kPlayerHeight * cutY,
		kPlayerWidth, kPlayerHeight,
		kPlayerScale, 0.0f, m_actor2.handle, true, true);
	SetDrawBright(255, 255, 255);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

TitleScene::TitleScene(SceneController& contoller) :
	SceneBase(contoller),
	m_titleHandle(LoadGraph("./img/title/TitleBack.png")),
	m_textHandle(LoadGraph("./img/title/PressAnyButton.png")),
	m_textBlinkFrame(0),
	m_update(&TitleScene::NormalUpdate),
	m_draw(&TitleScene::NormalDraw),
	m_punchHandle(LoadGraph("./img/Chara/White/punch/punch_stand_001.png")),
	m_kickHandle(LoadGraph("./img/Chara/White/kick/kick_stand_002.png")),
	m_guardHandle(LoadGraph("./img/Chara/White/guard/guard_stand_001.png")),
	m_actor1(m_punchHandle, 11,kAttackAnimFrame),
	m_actor2(m_guardHandle, 6,kDeffenceAnimFrame)
{
	m_bgm = std::make_shared<BGM>();
	int bgmhandle = LoadSoundMem("./BGM/BGM_Title.mp3");
	m_bgm->SetBGM(bgmhandle);
	m_bgm->Volume(kBgmVolume);
	m_bgm->PlayLoop();
}

void TitleScene::Update(Input& input, Input& input2)
{
	(this->*m_update)(input, input2);
}

void TitleScene::Draw()
{
	(this->*m_draw)();
}
