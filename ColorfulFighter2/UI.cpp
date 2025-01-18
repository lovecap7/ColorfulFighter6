#include "UI.h"
#include <DxLib.h>
#include "game.h"
#include "GameManager.h"

namespace
{
	//HPバーの座標
	constexpr float kHpPosXP1 = (Game::kScreenWidth / 2) - 120.0f;//X座標の起点
	constexpr float kHpPosXP2 = (Game::kScreenWidth / 2) + 120.0f;//X座標の起点
	constexpr float kHpPosY = 60.0;
	constexpr float kHpFrameOffset = 10.0f;
	
	//HPバーの横幅と縦幅
	constexpr float kHpWidth = 500.0f;
	constexpr float kHpHeight = 50.0f;

	//KO
	constexpr float kCenterPosX = Game::kScreenWidth / 2;
	constexpr float kCenterPosY = Game::kScreenHeight / 2;
	//KOのsize
	constexpr int kKoWidth = 256;
	constexpr int kKoHeight = 256;
	//KOの文字が出てる時間
	constexpr int kKoFrame = 180;

	//ラウンドの勝利UIの色
	constexpr unsigned int kNoWinColor = 0x888888;
	constexpr unsigned int kWinColorP1 = 0xff0055;
	constexpr unsigned int kWinColorP2 = 0x0022ff;
	
	//コマンドの数
	constexpr int kCommandNum = 9;
	//コマンドアイコンの大きさ
	constexpr float kCommandIconImageScale = 0.5f;
	constexpr int kCommandIconImageWidth = 200;
	constexpr int kCommandIconImageHight = 200;

	constexpr int kDamageDisplayFrame = 60;
}

UI::UI(int* selectCommandIndexP1, int* selectCommandIndexP2):
	m_hpbarP1(0.0f),
	m_hpbarMaxP1(0.0f),
	m_hpbarP2(0.0f),
	m_hpbarMaxP2(0.0f),
	m_damagebarP1(0.0f),
	m_damagebarP2(0.0f),
	m_isTimeupOrKo(false),
	m_isResult(false),
	m_resultUiPos(kCenterPosX,kCenterPosY - 200,0),
	m_frameCount(0),
	m_startRoundCount(0),
	m_koHandle(-1),
	m_roundHandle(-1),
	m_roundNumHandle(-1),
	m_fightHandle(-1),
	m_sprite1Handle(-1),
	m_sprite2Handle(-1),
	m_sprite3Handle(-1),
	m_resultHandle(-1),
	m_finishRoundHandle(-1),
	m_fightScale(1.0f),
	m_roundUiPos(kCenterPosX - 220, kCenterPosY + 50, 0),
	m_roundUiScale(2.0f)
{
	m_fightHandle = LoadGraph("./img/UI/RoundText/FIGHT.png");//FIGHT
	m_winnerHandle = LoadGraph("./img/UI/RoundText/WINNER.png");//Winner
	m_drawHandle = LoadGraph("./img/UI/RoundText/DRAW.png");//引き分け
	m_koHandle = LoadGraph("./img/UI/RoundText/KO.png");//KO
	m_timeupHandle = LoadGraph("./img/UI/RoundText/TIMEUP.png");//TimeUp

	//タイマーの数字
	m_timeZero = LoadGraph("./img/UI/Timer/0_Timer.png");	//0
	m_timeOne = LoadGraph("./img/UI/Timer/1_Timer.png");	//1
	m_timeTwo = LoadGraph("./img/UI/Timer/2_Timer.png");	//2
	m_timeThree = LoadGraph("./img/UI/Timer/3_Timer.png");	//3
	m_timeFour = LoadGraph("./img/UI/Timer/4_Timer.png");	//4
	m_timeFive = LoadGraph("./img/UI/Timer/5_Timer.png");	//5
	m_timeSix = LoadGraph("./img/UI/Timer/6_Timer.png");	//6
	m_timeSeven = LoadGraph("./img/UI/Timer/7_Timer.png");	//7
	m_timeEight = LoadGraph("./img/UI/Timer/8_Timer.png");	//8
	m_timeNine = LoadGraph("./img/UI/Timer/9_Timer.png");	//9
	//HP
	m_hpFrameHandle = LoadGraph("./img/UI/Always/HpFrame.png");	//フレーム
	//勝利数
	m_winRoundP1Handle = LoadGraph("./img/UI/Always/1PWinNumIcon.png");
	m_winRoundP2Handle = LoadGraph("./img/UI/Always/2PWinNumIcon.png");
	int noWinRoundHandle = LoadGraph("./img/UI/Always/WinNullIcon.png");
	m_winRound1P1Handle = noWinRoundHandle;
	m_winRound1P2Handle = noWinRoundHandle;
	m_winRound2P1Handle = noWinRoundHandle;
	m_winRound2P2Handle = noWinRoundHandle;

	//選んだコマンドを表示する準備
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
	//それぞれの選んだコマンドの画像を取得
	for (int i = 0; i < 3; ++i)
	{
		//1P
		m_selectCommandIconP1Handle[i] = m_commandIconHandle[selectCommandIndexP1[i] - 1];//セレクト番号は1~9なので配列に合わせて-1引く
		//2P
		m_selectCommandIconP2Handle[i] = m_commandIconHandle[selectCommandIndexP2[i] - 1];
	}
}

UI::~UI()
{
}

void UI::TimerUI(GameManager& gameManager)
{
	//10の位を表示
	int tensPlace = gameManager.GetTimer() / 10;
	SetTimeNumHandle(tensPlace, m_tensPlaceHandle);//10の位の数字の画像をセット
	
	//1の位を表示
	int onesPlace = gameManager.GetTimer() - (tensPlace * 10);
	SetTimeNumHandle(onesPlace, m_onesPlaceHandle);//1の位の数字の画像をセット
}

void UI::SetTimeNumHandle(int num, int& handle)
{
	switch (num)
	{
	case 0:
		handle = m_timeZero;
		break;
	case 1:
		handle = m_timeOne;
		break;
	case 2:
		handle = m_timeTwo;
		break;
	case 3:
		handle = m_timeThree;
		break;
	case 4:
		handle = m_timeFour;
		break;
	case 5:
		handle = m_timeFive;
		break;
	case 6:
		handle = m_timeSix;
		break;
	case 7:
		handle = m_timeSeven;
		break;
	case 8:
		handle = m_timeEight;
		break;
	case 9:
		handle = m_timeNine;
		break;
	default:
		break;
	}
}

void UI::Init(float p1Hp, float p2Hp, GameManager& gameManager)
{
	//リセット
	m_isTimeupOrKo = false;
	m_isResult = false;
	m_hpbarMaxP1 = p1Hp;
	m_hpbarMaxP2 = p2Hp;
	m_hpbarP1 = p1Hp;
	m_hpbarP2 = p2Hp;
	m_frameCount = 0;
	m_fightScale = 1.0f;
	m_beforeHpP1 = m_hpbarP1;
	m_beforeHpP2 = m_hpbarP2;
	m_damagebarP1 = m_hpbarP1;
	m_damagebarP2 = m_hpbarP2;
	m_damageDisplayCountFrameP1 = 0;
	m_damageDisplayCountFrameP2 = 0;
	//スプライト
	m_sprite1Handle = LoadGraph("./img/UI/splash/splash.png");
	m_sprite2Handle = LoadGraph("./img/UI/splash/splash2.png");
	m_sprite3Handle = LoadGraph("./img/UI/splash/splash3.png");
	//ラウンド○○
	m_roundHandle = LoadGraph("./img/UI/RoundText/ROUND.png");
	if (gameManager.GetRoundNumber() == 1)
	{
		m_roundNumHandle = LoadGraph("./img/UI/RoundText/1_ROUND.png");
	}
	else if (gameManager.GetRoundNumber() == 2)
	{
		m_roundNumHandle = LoadGraph("./img/UI/RoundText/2_ROUND.png");
	}
	else if (gameManager.GetRoundNumber() == 3)
	{
		m_roundNumHandle = LoadGraph("./img/UI/RoundText/3_ROUND.png");
	}
	else//オーバータイム
	{
		m_roundUiPos.x = kCenterPosX;
		m_roundUiScale = 3.0f;
		m_roundHandle = LoadGraph("./img/UI/RoundText/OVERTIME.png");
		m_roundNumHandle = -1;
	}
	//時間
	TimerUI(gameManager);
}



void UI::Update(float p1Hp, float p2Hp,GameManager& gameManager)
{
	//タイマー
	TimerUI(gameManager);

	//ゲーム開始時の演出のためのカウント
	m_startRoundCount = gameManager.GetStartRoundCount();

	//HPの更新
	m_hpbarP1 = p1Hp;
	m_hpbarP2 = p2Hp;
	DamageUI();
	
	//どちらかが死んだらKOの表示で時間切れならTimeUpの表示
	if (((m_hpbarP1 <= 0 || m_hpbarP2 <= 0) || gameManager.GetTimer() <= 0) && m_frameCount < kKoFrame)
	{
		m_finishRoundHandle = m_koHandle;
		if (gameManager.GetTimer() <= 0)
		{
			m_finishRoundHandle = m_timeupHandle;
		}
		m_isTimeupOrKo = true;
		m_frameCount++;
	}
	else
	{
		m_isTimeupOrKo = false;
	}
	//勝ったほうにWinnerの文字
	if (((m_hpbarP1 <= 0 || m_hpbarP2 <= 0) || gameManager.GetTimer() <= 0) && !m_isTimeupOrKo)
	{
		if (m_hpbarP1 == m_hpbarP2)
		{
			//引き分け
			m_resultUiPos.x = kCenterPosX;
			m_resultHandle = m_drawHandle;
			m_isResult = true;
		}
		else if (m_hpbarP1 > m_hpbarP2)
		{
			//P1win
			m_resultHandle = m_winnerHandle;
			m_resultUiPos.x = kCenterPosX - 500;
			m_isResult = true;
		}
		else if (m_hpbarP1 < m_hpbarP2)
		{
			//P2win
			m_resultHandle = m_winnerHandle;
			m_resultUiPos.x = kCenterPosX + 500;
			m_isResult = true;
		}
	}
	

	//P1の勝利数
	if (gameManager.GetWinNumP1() > 0)
	{
		//ラウンドを１本取ってたら画像を変更
		m_winRound1P1Handle = m_winRoundP1Handle;
		if (gameManager.GetWinNumP1() > 1)
		{
			//ラウンドを２本取ってたら画像を変更
			m_winRound2P1Handle = m_winRoundP1Handle;
		}
	}
	//P2の勝利数
	if (gameManager.GetWinNumP2() > 0)
	{
		//ラウンドを１本取ってたら画像を変更
		m_winRound1P2Handle = m_winRoundP2Handle;
		if (gameManager.GetWinNumP2() > 1)
		{
			//ラウンドを２本取ってたら画像を変更
			m_winRound2P2Handle = m_winRoundP2Handle;
		}
	}

	//現在のHPを保存
	m_beforeHpP1 = m_hpbarP1;
	m_beforeHpP2 = m_hpbarP2;
}

void UI::DamageUI()
{
	//HPが変化したらカウント開始
	if (m_beforeHpP1 != m_hpbarP1)
	{
		m_damageDisplayCountFrameP1 = kDamageDisplayFrame;
	}
	if (m_beforeHpP2 != m_hpbarP2)
	{
		m_damageDisplayCountFrameP2 = kDamageDisplayFrame;
	}

	//0になるまでカウントを減らす
	//0になったら現在のHPバーに合わせる
	if (m_damageDisplayCountFrameP1 <= 0)
	{
		m_damagebarP1 = m_hpbarP1;
		m_damageDisplayCountFrameP1 = 0;
	}
	else
	{
		m_damageDisplayCountFrameP1--;
	}
	if (m_damageDisplayCountFrameP2 <= 0)
	{
		m_damagebarP2 = m_hpbarP2;
		m_damageDisplayCountFrameP2 = 0;
	}
	else
	{
		m_damageDisplayCountFrameP2--;
	}
}

void UI::DrawBack()
{
	//KO勝利なら
	if ((m_finishRoundHandle == m_koHandle) && m_isTimeupOrKo)
	{
		DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0xffffff, true);
	}
	//決着がついた際の文字（KOとかTIMEUPとか)
	if (m_isTimeupOrKo)
	{
		DrawRectRotaGraphFast(
			kCenterPosX, kCenterPosY,
			0, 0, 512, 512,
			2.0f, 0.0f, m_finishRoundHandle, true, false);
	}

	////P1のHPのバック
	DrawBoxAA(kHpPosXP1, kHpPosY,
		kHpPosXP1 - kHpWidth, kHpPosY + kHpHeight,
		0x22222, true);
	////P2のHPのバック
	DrawBoxAA(kHpPosXP2, kHpPosY,
		kHpPosXP2 + kHpWidth, kHpPosY + kHpHeight,
		0x222222, true);

	////P1のダメージ
	DrawBoxAA(kHpPosXP1, kHpPosY,
		kHpPosXP1 - kHpWidth * (m_damagebarP1 / m_hpbarMaxP1), kHpPosY + kHpHeight,
		0xff3333, true);
	////P2のダメージ
	DrawBoxAA(kHpPosXP2, kHpPosY,
		kHpPosXP2 + kHpWidth * (m_damagebarP2 / m_hpbarMaxP2), kHpPosY + kHpHeight,
		0xff3333, true);

	////P1のHP
	DrawBoxAA(kHpPosXP1, kHpPosY,
		kHpPosXP1 - kHpWidth * (m_hpbarP1 / m_hpbarMaxP1), kHpPosY + kHpHeight,
		0xffff55, true);
	////P2のHP
	DrawBoxAA(kHpPosXP2, kHpPosY,
		kHpPosXP2 + kHpWidth * (m_hpbarP2 / m_hpbarMaxP2), kHpPosY + kHpHeight,
		0xffff55, true);
	//フレーム1P
	DrawGraph(kHpPosXP1 - kHpWidth - kHpFrameOffset, kHpPosY - kHpFrameOffset, m_hpFrameHandle, true);
	//フレーム2P
	DrawGraph(kHpPosXP2 - kHpFrameOffset, kHpPosY - kHpFrameOffset, m_hpFrameHandle, true);
	
	//タイマー
	//10の位
	DrawRectRotaGraphFast(
		Game::kScreenWidth / 2 - 40, 80,
		0, 0, 512, 512,
		0.2f, 0.0f, m_tensPlaceHandle, true, false);
	//1の位
	DrawRectRotaGraphFast(
		Game::kScreenWidth / 2 + 40, 80,
		0, 0, 512, 512,
		0.2f, 0.0f, m_onesPlaceHandle, true, false);

	//コマンド
	for (int i = 0; i < 3; ++i)
	{
		DrawRectRotaGraphFast(
			(kCommandIconImageWidth * kCommandIconImageScale) * (i + 1), 
			(Game::kScreenHeight - (kCommandIconImageHight * kCommandIconImageScale * kCommandIconImageScale)),
			0, 0, kCommandIconImageWidth, kCommandIconImageHight,
			kCommandIconImageScale, 0.0f, m_selectCommandIconP1Handle[i], true);
		DrawRectRotaGraphFast(
			Game::kScreenWidth - ((kCommandIconImageWidth * kCommandIconImageScale) * (i + 1)),
			(Game::kScreenHeight - (kCommandIconImageHight * kCommandIconImageScale * kCommandIconImageScale)),
			0, 0, kCommandIconImageWidth, kCommandIconImageHight,
			kCommandIconImageScale, 0.0f, m_selectCommandIconP2Handle[i], true);
	}

	//勝ったラウンドの数UI
	//勝った数表示
	//P1
	DrawGraph((Game::kScreenWidth / 2) - 200, 0, m_winRound1P1Handle, true);
	DrawGraph((Game::kScreenWidth / 2) - 150, 0, m_winRound2P1Handle, true);
	//P2
	DrawGraph((Game::kScreenWidth / 2) + 110, 0, m_winRound2P2Handle, true);
	DrawGraph((Game::kScreenWidth / 2) + 160, 0, m_winRound1P2Handle, true);
	
}

void UI::DrawFront()
{
	//試合結果
	if(m_isResult)
	{
		DrawRectRotaGraphFast(
			m_resultUiPos.x, m_resultUiPos.y,
			0, 0, 512, 512,
			1.0f, 0.0f, m_resultHandle, true, false);
	}

	

	//ラウンド初めの文字
	if (m_startRoundCount >= 120 && m_startRoundCount < 180)
	{
		m_fightScale += 0.02f;
		//スプライト
		DrawRectRotaGraphFast(
			kCenterPosX - 300, kCenterPosY,
			0, 0, 512, 512,
			2.0f, 0.0f, m_sprite3Handle, true, false);
		DrawRectRotaGraphFast(
			kCenterPosX + 300, kCenterPosY,
			0, 0, 512, 512,
			2.0f, 0.0f, m_sprite2Handle, true, false);
		//FIGHT
		DrawRectRotaGraphFast(
			kCenterPosX , kCenterPosY,
			0, 0, 512, 512,
			m_fightScale, 0.0f, m_fightHandle, true, false);
	}
	else if (m_startRoundCount < 120)
	{
		if (m_startRoundCount > 40)
		{
			//スプライト
			DrawRectRotaGraphFast(
				kCenterPosX + 400, kCenterPosY,
				0, 0, 512, 512,
				2.0f, 0.0f, m_sprite1Handle, true, false);
		}
		if (m_startRoundCount > 60)
		{
			//スプライト
			DrawRectRotaGraphFast(
				kCenterPosX + 400, kCenterPosY,
				0, 0, 512, 512,
				1.5f, 0.0f, m_sprite2Handle, true, false);
		}
		
		//ラウンド
		DrawRectRotaGraphFast(
			m_roundUiPos.x, m_roundUiPos.y,
			0, 0, 512, 512,
			m_roundUiScale, 0.0f, m_roundHandle, true, false);
		//ラウンド○○
		DrawRectRotaGraphFast(
			kCenterPosX + 350, kCenterPosY,
			0, 0, 512, 512,
			1.0f, 0.0f, m_roundNumHandle, true, false);
	}
}


