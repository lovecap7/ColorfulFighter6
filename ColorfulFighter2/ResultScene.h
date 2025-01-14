#pragma once
#include "SceneBase.h"
#include "game.h"
#include <memory>

class Player;
class Input;
class BGM;
class ResultScene :
    public SceneBase
{
private:
    //プレイヤーの画像
    int m_p1Handle;
    int m_p2Handle;
    //BGM
    std::shared_ptr<BGM> m_bgm;
public:
    ResultScene(SceneController& controller);
    //派生クラスで実装を実装
    virtual void Update(Input& input, Input& input2) override;

    virtual void Draw() override;

    //勝敗の結果を記録する
    void RecordResult();
};

