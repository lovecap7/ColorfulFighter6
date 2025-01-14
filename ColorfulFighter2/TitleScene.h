#pragma once
#include "SceneBase.h"
#include <memory>
class BGM;
class TitleScene :
    public SceneBase
{
private:
    int m_titleHandle;
    std::shared_ptr<BGM> m_bgm;
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

