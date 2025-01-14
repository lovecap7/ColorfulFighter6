#include "SE.h"
#include <DxLib.h>
SE::SE():
	m_isPlay(false)
	//m_seHandle(-1),
	//m_seHit1(LoadSoundMem("./SE/Hit/Hit1.mp3")),
	//m_seHit2(LoadSoundMem("./SE/Hit/Hit2.mp3")),
	//m_seGuard(LoadSoundMem("./SE/Hit/Guard.mp3")),
	//m_seRound1(LoadSoundMem("./SE/Round/Round1.mp3")),
	//m_seRound2(LoadSoundMem("./SE/Round/Round2.mp3")),
	//m_seRound3(LoadSoundMem("./SE/Round/Round3.mp3")),
	//m_seRoundOver(LoadSoundMem("./SE/Round/RoundOver.mp3")),
	//m_seFight(LoadSoundMem("./SE/Round/Fight.mp3")),
	//m_seKo(LoadSoundMem("./SE/uwauwa/uwauwa.mp3"))
{
}

SE::~SE()
{
	Stop();
}

void SE::SetSE(int seHandle)
{
	//同じサウンドなら変更しない
	if (m_seHandle != seHandle)
	{
		Stop();
		m_seHandle = seHandle;
	}
}

void SE::PlayOnce()
{
	//一度だけ呼ぶ
	if (!m_isPlay)
	{
		PlaySoundMem(m_seHandle, DX_PLAYTYPE_BACK, true);
		m_isPlay = true;
	}
}

void SE::PlayLoop()
{
	//一度だけ呼ぶ
	if (!m_isPlay)
	{
		PlaySoundMem(m_seHandle, DX_PLAYTYPE_LOOP, true);
		m_isPlay = true;
	}
}

void SE::Stop()
{
	StopSoundMem(m_seHandle);
	m_isPlay = false;
}

bool SE::CheckEndSE()
{
	bool isEnd = CheckSoundMem(m_seHandle) == 0;
	return isEnd;
}

void SE::Volume(int volume)
{
	ChangeVolumeSoundMem(volume, m_seHandle);
}
