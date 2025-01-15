#include "FadeManager.h"
#include "game.h"
#include <DxLib.h>

namespace
{
	constexpr int fadeSpeed = 5;
}

FadeManager::FadeManager():
	m_fadeFrameCount(255),
	m_isFinishFadeIn(false),
	m_isFinishFadeOut(false)
{
}

FadeManager::~FadeManager()
{
}

void FadeManager::FadeDraw(bool isFadeIn)
{
	//フェードイン
	if (isFadeIn)
	{
		m_fadeFrameCount += fadeSpeed;
		DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeFrameCount);
		DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
		DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	//フェードアウト
	else
	{
		m_fadeFrameCount -= fadeSpeed;
		DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeFrameCount);
		DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
		DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	//最大値に行ったとき
	if (m_fadeFrameCount >= 255)
	{
		m_isFinishFadeIn = true;
		m_fadeFrameCount = 255;
	}
	else
	{
		m_isFinishFadeIn = false;
	}
	//最小値に行ったとき
	if (m_fadeFrameCount <= 0)
	{
		m_isFinishFadeOut = true;
		m_fadeFrameCount = 0;
	}
	else
	{
		m_isFinishFadeOut = false;
	}
}

