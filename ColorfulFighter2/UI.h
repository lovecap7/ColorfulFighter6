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
	//�����I���̍ۂ�UI
	bool m_isTimeupOrKo;
	int m_koHandle;
	int m_timeupHandle;
	int m_finishRoundHandle;
	//�������ʂ̕\��
	bool m_isResult;
	//���ʂ̕\���ʒu
	Vector3 m_resultUiPos;
	//���E���h�J�n����UI
	int m_startRoundCount;
	int m_roundHandle;
	int m_roundNumHandle;
	int m_fightHandle;
	int m_sprite1Handle;
	int m_sprite2Handle;
	int m_sprite3Handle;

	Vector3 m_roundUiPos;
	float m_roundUiScale;
	//��������
	int m_winnerHandle;
	int m_drawHandle;
	int m_resultHandle;
	
	float m_fightScale;//FIGHT�̕��������񂾂�傫������

	//�^�C�}�[UI
	void TimerUI(GameManager& gameManager);
	int m_onesPlaceHandle;//1�̈�
	int m_tensPlaceHandle;//10�̈�

	//�^�C�}�[�̐���
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
	//��������UI
	int m_winRoundP1Handle;
	int m_winRoundP2Handle;
	//P1
	int m_winRound1P1Handle;
	int m_winRound2P1Handle;
	//P2
	int m_winRound1P2Handle;
	int m_winRound2P2Handle;

	//�R�}���h�̃A�C�R��
	int m_commandIconHandle[9];
	int m_selectCommandIconP1Handle[3];//P1���I�񂾃R�}���h��\��
	int m_selectCommandIconP2Handle[3];//P2���I�񂾃R�}���h��\��

	//�_���[�W�\�L
	int m_damageDisplayCountFrameP1;
	int m_damageDisplayCountFrameP2;
	void DamageUI();
	float m_damagebarP1;
	float m_damagebarP2;
	//1�t���[���O��HP�̒l��ێ�
	int m_beforeHpP1;
	int m_beforeHpP2;

public:
	UI(int* selectCommandIndexP1, int* selectCommandIndexP2);
	~UI();

	void Init(float p1Hp, float p2Hp, GameManager& gameManager);
	void Update(float p1Hp, float p2Hp,GameManager& gameManager);
	void DrawBack();//�펞�o�Ă�UI
	void DrawFront();//���o�ŏo��UI
};

