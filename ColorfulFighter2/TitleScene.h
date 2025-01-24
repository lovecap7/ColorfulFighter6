#pragma once
#include "SceneBase.h"
#include <memory>
#include "Vector3.h"
class BGM;
class TitleScene :
    public SceneBase
{
private:
    int m_backHandle;
	int m_titleHandle;
    int m_titleFadeCountFrame;
    int m_textHandle;
    std::shared_ptr<BGM> m_bgm;

    //テキストが出たり消えたりする
    int m_textBlinkFrame;
    void BlinkingTextDraw();

    using UpdateFunc_t = void(TitleScene::*)(Input& input, Input& input2);
    UpdateFunc_t m_update;
    //Draw
    using DrawFunc_t = void(TitleScene::*)();
    DrawFunc_t m_draw;

	void NormalUpdate(Input& input, Input& input2);
	void NormalDraw();

    //オープニング
	void OpeningUpdate(Input& input, Input& input2);
	void OpeningDraw();

    //デモ映像
	void DemoUpdate(Input& input, Input& input2);
	void DemoDraw();

	//フェード
    void FadeUpdate(Input& input, Input& input2);
    void FadeDraw();
	int m_fadeCountFrame;

    struct Actor
    {
        int handle;
        int animIndex;	//アニメーションの番号を数える(1増える毎にアニメーションが進む)
        int animNum;		//アニメーションの数
        int oneAnimFrame;	//1枚のアニメーションにかかるフレーム
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="handle">ハンドル</param>
		/// <param name="animNum">絵の枚数</param>
		Actor(int Handle, int AnimNum,int OneAnimFrame) :
			handle(Handle),
			animIndex(0),
			animNum(AnimNum),
            oneAnimFrame(OneAnimFrame)
		{
		}
    };

    //裏で戦っているキャラクター
	Actor m_actor1;
	Actor m_actor2;
	void ActorDraw();
    //アニメーション関連
    int m_animCountFrame;	
	int m_idleHandle;
	int m_punchHandle;
	int m_kickHandle;
	int m_guardHandle;
	int m_walkHandle;
  
	Vector3 m_actor1Pos;
	Vector3 m_actor2Pos;
	Vector3 m_actor1Velo;
	Vector3 m_actor2Velo;
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

