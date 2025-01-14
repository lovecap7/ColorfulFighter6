#include "BGM.h"
#include <DxLib.h>

BGM::BGM():
	m_bgmHandle(-1),
	m_isPlay(false)
	////曲
	//m_bgmStage1(LoadSoundMem("./BGM/BGM_Stage1.mp3")),
	//m_bgmResult(LoadSoundMem("./BGM/BGM_Result.mp3")),
	//m_bgmTitleScene(LoadSoundMem("./BGM/BGM_Result.mp3")),
	//m_bgmSelectScene(LoadSoundMem("./BGM/BGM_SelectScene.mp3"))
{
}

BGM::~BGM()
{
	Stop();
}

void BGM::SetBGM(int bgmHandle)
{
	//同じサウンドなら変更しない
	if (m_bgmHandle != bgmHandle)
	{
		Stop();
		m_bgmHandle = bgmHandle;
	}
}

void BGM::PlayOnce()
{
	//一度だけ呼ぶ
	if (!m_isPlay)
	{
		PlaySoundMem(m_bgmHandle, DX_PLAYTYPE_BACK, true);
		m_isPlay = true;
	}
}

void BGM::PlayLoop()
{
	//一度だけ呼ぶ
	if (!m_isPlay)
	{
		PlaySoundMem(m_bgmHandle, DX_PLAYTYPE_LOOP, true);
		m_isPlay = true;
	}
}

void BGM::Stop()
{
	StopSoundMem(m_bgmHandle);
	m_isPlay = false;
}

bool BGM::CheckEndBGM()
{
	bool isEnd = CheckSoundMem(m_bgmHandle) == 0;
	return isEnd;
}

void BGM::Volume(int volume)
{
	ChangeVolumeSoundMem(volume, m_bgmHandle);
}
