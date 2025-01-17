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

ResultScene::ResultScene(SceneController& controller):
	SceneBase(controller),
	m_p1Handle(-1),
	m_p2Handle(-1),
	m_isFadeIn(false),
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

void ResultScene::Update(Input& input, Input& input2)
{
	//BGMの再生が終わったら切り替え
	if (m_bgm->CheckEndBGM())
	{
		m_isFadeIn = true;
		if (m_fadeManager->IsFinishFadeIn())
		{
			Rematch();//再戦
			return;//忘れずreturn
		}
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
	m_fadeManager->FadeDraw(m_isFadeIn);
	//なうろーでぃんぐ
	if (m_fadeManager->IsFinishFadeIn())
	{
		DxLib::DrawGraph(0, 0, m_loadingHandle, true);
	}
}

void ResultScene::RecordResult()
{
}
