#pragma once
class SE
{
private:
	//現在のBGM
	int m_seHandle;
	//一度だけ呼ばれたいのでbool型で一度呼ばれたらtrueにして呼べなくする
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
	void PlayOnce();//再生
	void PlayLoop();//Loop再生
	void Stop();//停止
	bool CheckEndSE();//SEが終わったかをチェック
	void Volume(int volume);
};

