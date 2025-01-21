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

    //セリフ
    int m_serifHandle;

    //メニュー
    bool m_isSelecting;//trueなら現在メニューを選んでいる
    int m_selectMenuIndex;
    int m_menuHandle;
    int m_menuP2Handle;
    void SelectMenu(Input& input);
    void Rematch();//再戦
    void Reselect();//コマンドの選び直し
    void GameEnd();//タイトル画面へ

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
};

