#include "FadeManager.h"
#include "game.h"
#include <DxLib.h>

namespace
{
	constexpr int fadeSpeed = 5;
}

FadeManager::FadeManager():
	m_fadeFrameCount(255)
{
}

FadeManager::~FadeManager()
{
}

void FadeManager::Fade(bool isFadeIn)
{
	//フェードイン
	if (isFadeIn)
	{
		m_fadeFrameCount += fadeSpeed;
		DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeFrameCount);
		DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
		DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		if (m_fadeFrameCount >= 255)
		{
			m_fadeFrameCount = 255;
		}
	}
	//フェードアウト
	else
	{
		m_fadeFrameCount -= fadeSpeed;
		DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeFrameCount);
		DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
		DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		if (m_fadeFrameCount <= 0)
		{
			m_fadeFrameCount = 0;
		}
	}
}

