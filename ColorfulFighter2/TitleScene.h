#pragma once
#include "SceneBase.h"
#include <memory>
class BGM;
class Chara;
class TitleScene :
    public SceneBase
{
private:
    int m_titleHandle;
    int m_textHandle;
    std::shared_ptr<BGM> m_bgm;

    //テキストが出たり消えたりする
    int m_countFrame;
    void BlinkingTextDraw();
	//キャラクターを描画
    std::shared_ptr<Chara> m_chara;

    using UpdateFunc_t = void(TitleScene::*)(Input& input, Input& input2);
    UpdateFunc_t m_update;
    //Draw
    using DrawFunc_t = void(TitleScene::*)();
    DrawFunc_t m_draw;

	void NormalUpdate(Input& input, Input& input2);
	void NormalDraw();

	void DemoUpdate(Input& input, Input& input2);
	void DemoDraw();

public:
    TitleScene(SceneController& contoller);

    /// <summary>
    /// 派生クラスで実装を実装
    /// virtual はこれを継承するかもしれないから
    /// overrideは明示的にオーバーライドをエディタに示すため
    /// </summary>
    virtual void Update(Input& input, Input& input2) override;

    virtual void Draw() override;
};

