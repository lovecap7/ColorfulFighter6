#pragma once
#include "Vector3.h"
class GameManager;
class UI
{
private:
	float m_hpbarMaxP1;
	float m_hpbarMaxP2;
	float m_hpbarP1;
	float m_hpbarP2;
	int m_frameCount;
	//試合終了の際のUI
	bool m_isTimeupOrKo;
	int m_koHandle;
	int m_timeupHandle;
	int m_finishRoundHandle;
	//試合結果の表示
	bool m_isResult;
	//結果の表示位置
	Vector3 m_resultUiPos;
	//ラウンド開始時のUI
	int m_startRoundCount;
	int m_roundHandle;
	int m_roundNumHandle;
	int m_fightHandle;
	int m_sprite1Handle;
	int m_sprite2Handle;
	int m_sprite3Handle;

	Vector3 m_roundUiPos;
	float m_roundUiScale;
	//試合結果
	int m_winnerHandle;
	int m_drawHandle;
	int m_resultHandle;
	
	float m_fightScale;//FIGHTの文字をだんだん大きくする

	//タイマーUI
	void TimerUI(GameManager& gameManager);
	int m_onesPlaceHandle;//1の位
	int m_tensPlaceHandle;//10の位

	//タイマーの数字
	void SetTimeNumHandle(int num,int& handle);
	int m_timeZero;		//0
	int m_timeOne;		//1
	int m_timeTwo;		//2
	int m_timeThree;	//3
	int m_timeFour;		//4
	int m_timeFive;		//5
	int m_timeSix;		//6
	int m_timeSeven;	//7
	int m_timeEight;	//8
	int m_timeNine;		//9

	//HP
	int m_hpFrameHandle;
	//勝利数のUI
	int m_winRoundP1Handle;
	int m_winRoundP2Handle;
	//P1
	int m_winRound1P1Handle;
	int m_winRound2P1Handle;
	//P2
	int m_winRound1P2Handle;
	int m_winRound2P2Handle;

	//コマンドのアイコン
	int m_commandIconHandle[9];
	int m_selectCommandIconP1Handle[3];//P1が選んだコマンドを表示
	int m_selectCommandIconP2Handle[3];//P2が選んだコマンドを表示

	//ダメージ表記
	int m_damageDisplayCountFrameP1;
	int m_damageDisplayCountFrameP2;
	void DamageUI();
	float m_damagebarP1;
	float m_damagebarP2;
	//1フレーム前のHPの値を保持
	int m_beforeHpP1;
	int m_beforeHpP2;

public:
	UI(int* selectCommandIndexP1, int* selectCommandIndexP2);
	~UI();

	void Init(float p1Hp, float p2Hp, GameManager& gameManager);
	void Update(float p1Hp, float p2Hp,GameManager& gameManager);
	void DrawBack();//常時出てるUI
	void DrawFront();//演出で出るUI
};

