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
	constexpr int kMenuNum = 2;
}

ResultScene::ResultScene(SceneController& controller):
	SceneBase(controller),
	m_p1Handle(-1),
	m_p2Handle(-1),
	m_isSelecting(false),
	m_isFadeIn(false),
	m_selectMenuIndex(0),
	//ローディング画面
	m_loadingHandle(LoadGraph("./img/Loading/NowLoading.png"))
{
	//1Pが勝ったなら
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

void ResultScene::RecordResult()
{
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
#endif
	//リザルトの画像
	DxLib::DrawGraph(100, (Game::kScreenHeight / 2) - 400, m_p1Handle, true);
	DxLib::DrawTurnGraph(Game::kScreenWidth - 600, (Game::kScreenHeight / 2) - 400, m_p2Handle, true);
	//メニュー
	if (m_isSelecting)
	{
		DrawString(400, 600, "再戦", 0xffffff);
		DrawString(400, 620, "技を選びなおす", 0xffffff);
		switch (m_selectMenuIndex)
		{
		case 0:
			DrawString(380, 600, "⇒", 0xff0000);
			break;
		case 1:
			DrawString(380, 620, "⇒", 0xff0000);
			break;
		default:
			break;
		}
	}

	//フェード
	m_fadeManager->FadeDraw(m_isFadeIn);
	//なうろーでぃんぐ
	if (m_fadeManager->IsFinishFadeIn())
	{
		DxLib::DrawGraph(0, 0, m_loadingHandle, true);
	}
}

