#pragma once
#include "SceneBase.h"
#include "game.h"
#include <memory>

class Player;
class Input;
class BGM;
class FadeManager;
class ResultScene :
    public SceneBase
{
private:
    //プレイヤーの画像
    int m_p1Handle;
    int m_p2Handle;
    //BGM
    std::shared_ptr<BGM> m_bgm;

    //メニュー
    int m_selectMenuIndex;
    void Rematch();//再戦
    void Reselect();//コマンドの選び直し

    //フェードインするために使う
    std::shared_ptr<FadeManager> m_fadeManager;
    bool m_isFadeIn;
    //Loading中の画像
    int m_loadingHandle;
public:
    ResultScene(SceneController& controller);
    //派生クラスで実装を実装
    virtual void Update(Input& input, Input& input2) override;

    virtual void Draw() override;

    //勝敗の結果を記録する
    void RecordResult();
};

