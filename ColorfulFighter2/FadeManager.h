#pragma once
class FadeManager
{
private:
	int m_fadeFrameCount;
public:
	FadeManager();
	~FadeManager();

	void Fade(bool isFadeIn);

	int GetFadeFrameCount() { return m_fadeFrameCount; }
};

