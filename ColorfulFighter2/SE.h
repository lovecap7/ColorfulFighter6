#pragma once
class SE
{
private:
	//���݂�BGM
	int m_seHandle;
	//��x�����Ă΂ꂽ���̂�bool�^�ň�x�Ă΂ꂽ��true�ɂ��ČĂׂȂ�����
	bool m_isPlay;


	//SE
	int m_seHit1;
	int m_seHit2;
	int m_seGuard;
	int m_seRound1;
	int m_seRound2;
	int m_seRound3;
	int m_seRoundOver;
	int m_seFight;
	int m_seKo;
public:
	SE();
	~SE();

	void SetSE(int seIndex);
	void PlayOnce();//�Đ�
	void PlayLoop();//Loop�Đ�
	void Stop();//��~
	bool CheckEndSE();//SE���I����������`�F�b�N
	void Volume(int volume);
};

