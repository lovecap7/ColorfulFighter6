#include "ResultScene.h"
#include "SceneController.h"
#include "Input.h"
#include "CommandSelectScene.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "FadeManager.h"
#include <DxLib.h>
#include "game.h"
#include "BGM.h"

namespace
{
	//プレイヤーの画像
	constexpr int kP1ImagePosX = (Game::kScreenWidth / 2) - 500;
	constexpr int kP2ImagePosX = (Game::kScreenWidth / 2) + 100;
	constexpr int kPlayerImagePosY = (Game::kScreenHeight / 2) - 400;
	//セリフ数
	constexpr int kSerifNum = 5;
	constexpr int kSerifMaxIndex = kSerifNum - 1;
	constexpr int kSerifPosX = (Game::kScreenWidth / 2) - 300;
	constexpr int kSerifPosY = (Game::kScreenHeight / 2) + 50;
	//メニュー数
	constexpr int kMenuNum = 3;
	//BGMボリューム
	constexpr int kBgmVolume = 120;
	//SEボリューム
	constexpr int kSeVolume = 150;

	//カーソル
	constexpr int kMenuWidth = 375;
	constexpr int kCursorHeight = 100;
	constexpr int kCursorPosY = (Game::kScreenHeight / 2) + 120;
}

ResultScene::ResultScene(SceneController& controller):
	SceneBase(controller),
	m_p1Handle(-1),
	m_p2Handle(-1),
	m_isSelecting(false),
	m_isFadeIn(false),
	m_selectMenuIndex(0),
	//ローディング画面
	m_loadingHandle(LoadGraph("./img/Loading/NowLoading.png")),
	m_menuHandle(LoadGraph("./img/Result/ResultTextP1.png"))
{
	int serif;
	switch (GetRand(kSerifMaxIndex))
	{
	case 0:
		serif = LoadGraph("./img/Result/Serif/Serif1.png");
		break;
	case 1:
		serif = LoadGraph("./img/Result/Serif/Serif2.png");
		break;
	case 2:
		serif = LoadGraph("./img/Result/Serif/Serif3.png");
		break;
	case 3:
		serif = LoadGraph("./img/Result/Serif/Serif4.png");
		break;
	case 4:
		serif = LoadGraph("./img/Result/Serif/Serif5.png");
		break;
	default:
		break;
	}
	m_serifHandle = serif;

	//1Pが勝ったなら
	if (m_controller.GetWinPlayerIndex() == PlayerIndex::Player1)
	{
		//P1の勝利画像
		switch (m_controller.GetCharaColorIndexP1())
		{
		case CharaColorIndex::White:
			m_p1Handle = LoadGraph("./img/Result/Face/White_Win.png");
			break;
		case CharaColorIndex::Red:
			m_p1Handle = LoadGraph("./img/Result/Face/Red_Win.png");
			break;
		case CharaColorIndex::Blue:
			m_p1Handle = LoadGraph("./img/Result/Face/Blue_Win.png");
			break;
		case CharaColorIndex::Green:
			m_p1Handle = LoadGraph("./img/Result/Face/Green_Win.png");
			break;
		case CharaColorIndex::Yellow:
			m_p1Handle = LoadGraph("./img/Result/Face/Yellow_Win.png");
			break;
		default:
			break;
		}
		//P2の敗北画像
		switch (m_controller.GetCharaColorIndexP2())
		{
		case CharaColorIndex::White:
			m_p2Handle = LoadGraph("./img/Result/Face/White_Lose.png");
			break;
		case CharaColorIndex::Red:
			m_p2Handle = LoadGraph("./img/Result/Face/Red_Lose.png");
			break;
		case CharaColorIndex::Blue:
			m_p2Handle = LoadGraph("./img/Result/Face/Blue_Lose.png");
			break;
		case CharaColorIndex::Green:
			m_p2Handle = LoadGraph("./img/Result/Face/Green_Lose.png");
			break;
		case CharaColorIndex::Yellow:
			m_p2Handle = LoadGraph("./img/Result/Face/Yellow_Lose.png");
			break;
		default:
			break;
		}
	}
	else if (m_controller.GetWinPlayerIndex() == PlayerIndex::Player2)
	{
		//P2の勝利画像
		switch (m_controller.GetCharaColorIndexP2())
		{
		case CharaColorIndex::White:
			m_p2Handle = LoadGraph("./img/Result/Face/White_Win.png");
			break;
		case CharaColorIndex::Red:
			m_p2Handle = LoadGraph("./img/Result/Face/Red_Win.png");
			break;
		case CharaColorIndex::Blue:
			m_p2Handle = LoadGraph("./img/Result/Face/Blue_Win.png");
			break;
		case CharaColorIndex::Green:
			m_p2Handle = LoadGraph("./img/Result/Face/Green_Win.png");
			break;
		case CharaColorIndex::Yellow:
			m_p2Handle = LoadGraph("./img/Result/Face/Yellow_Win.png");
			break;
		default:
			break;
		}
		//P1の敗北画像
		switch (m_controller.GetCharaColorIndexP1())
		{
		case CharaColorIndex::White:
			m_p1Handle = LoadGraph("./img/Result/Face/White_Lose.png");
			break;
		case CharaColorIndex::Red:
			m_p1Handle = LoadGraph("./img/Result/Face/Red_Lose.png");
			break;
		case CharaColorIndex::Blue:
			m_p1Handle = LoadGraph("./img/Result/Face/Blue_Lose.png");
			break;
		case CharaColorIndex::Green:
			m_p1Handle = LoadGraph("./img/Result/Face/Green_Lose.png");
			break;
		case CharaColorIndex::Yellow:
			m_p1Handle = LoadGraph("./img/Result/Face/Yellow_Lose.png");
			break;
		default:
			break;
		}
	}

	m_bgm = std::make_shared<BGM>();
	int bgmhandle = LoadSoundMem("./BGM/BGM_Result.mp3");
	m_bgm->SetBGM(bgmhandle);
	m_bgm->Volume(kBgmVolume);
	m_bgm->PlayOnce();

	//フェードインするときに使う
	m_fadeManager = std::make_shared<FadeManager>();
}


void ResultScene::SelectMenu(Input& input)
{
	if (!m_isFadeIn)
	{
		if (input.IsTrigger("Up"))
		{
			m_selectMenuIndex--;
		}
		if (input.IsTrigger("Down"))
		{
			m_selectMenuIndex++;
		}
		if (m_selectMenuIndex < 0)
		{
			m_selectMenuIndex = (kMenuNum - 1);
		}
		if (m_selectMenuIndex > (kMenuNum - 1))
		{
			m_selectMenuIndex = 0;
		}
		//決定
		if (input.IsTrigger("A"))
		{
			m_isFadeIn = true;
		}
	}
}

void ResultScene::Rematch()
{
	//押されたら次の状態に繊維
	//次の状態はこのクラスが覚えておく
	m_controller.ChangeScene(std::make_shared<GameScene>(m_controller));
}

void ResultScene::Reselect()
{
	//押されたら次の状態に繊維
	//次の状態はこのクラスが覚えておく
	m_controller.ChangeScene(std::make_shared<CommandSelectScene>(m_controller));
}

void ResultScene::GameEnd()
{
	//押されたら次の状態に繊維
	//次の状態はこのクラスが覚えておく
	m_controller.ChangeScene(std::make_shared<TitleScene>(m_controller));
}

void ResultScene::Update(Input& input, Input& input2)
{
	if (m_isSelecting)
	{
		//メニューを選ぶ
		SelectMenu(input);
		//フェードインしたら
		if (m_fadeManager->IsFinishFadeIn())
		{
			//番号にあった関数を呼ぶ
			switch (m_selectMenuIndex)
			{
			case 0:
				Rematch();//再戦
				break;
			case 1:
				Reselect();//コマンドセレクト
				break;
			case 2:
				GameEnd();//タイトル画面
				break;
			default:
				Rematch();//再戦
				break;
			}
			return;//忘れずreturn
		}
	}
	//BGMの再生が終わったら切り替え
	if (m_bgm->CheckEndBGM() || input.IsTrigger("A"))
	{
		m_isSelecting = true;
	}
}

void ResultScene::Draw()
{
#if _DEBUG	
	DrawString(10, 10, "Result Scene", 0xffffff);
	DrawLine(Game::kScreenWidth / 2, 0, Game::kScreenWidth / 2, Game::kScreenHeight, 0xffffff);
#endif

	//リザルトの画像
	DxLib::DrawGraph(kP1ImagePosX, kPlayerImagePosY, m_p1Handle, true);
	DxLib::DrawTurnGraph(kP2ImagePosX, kPlayerImagePosY, m_p2Handle, true);
	DxLib::DrawGraph(kSerifPosX, kSerifPosY, m_serifHandle, true);
	//メニュー
	if (m_isSelecting)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
		DrawBox(0, kCursorPosY, kMenuWidth, kCursorPosY + (kCursorHeight * kMenuNum), 0xffffff, true);
		switch (m_selectMenuIndex)
		{
		case 0:
			DrawBox(0, kCursorPosY, kMenuWidth, kCursorPosY + kCursorHeight, 0xff5555, true);
			break;
		case 1:
			DrawBox(0, kCursorPosY + kCursorHeight, kMenuWidth, kCursorPosY + (kCursorHeight * 2), 0xff5555, true);
			break;
		case 2:
			DrawBox(0, kCursorPosY + (kCursorHeight * 2), kMenuWidth, kCursorPosY + (kCursorHeight * 3), 0xff5555, true);
			break;
		default:
			break;
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 200);
		DxLib::DrawGraph(0, kCursorPosY, m_menuHandle, true);
	}

	//フェード
	m_fadeManager->FadeDraw(m_isFadeIn);
	//なうろーでぃんぐ
	if (m_fadeManager->IsFinishFadeIn())
	{
		DxLib::DrawGraph(0, 0, m_loadingHandle, true);
	}
}

