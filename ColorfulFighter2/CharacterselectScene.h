#pragma once
#include "SceneBase.h""
#include <memory>

class BGM;
class SE;
class CharaBase;
class CharacterselectScene :
    public SceneBase
{
private:
    //キャラクターのアイコンハンドル
    int m_iconEmptyHandle;
    int m_iconChara1Handle;
    //キャラクターのイメージ画像
    int m_imageChara1Handle;
    //コマンドのアイコン
    int m_commandIconHandle[9];
    int m_selectCommandIconP1Handle[3];//P1が選んだコマンドを表示
    int m_selectCommandIconP2Handle[3];//P2が選んだコマンドを表示

    //P1のカーソル
    int m_cursorP1Handle;
    //P2のカーソル
    int m_cursorP2Handle;
   
    //現在の技のセレクト番号
    int m_currentSelectCommandIndexP1;
    int m_currentSelectCommandIndexP2;
    //技を3つ決定したらtrue
    bool m_isSelectP1;
    bool m_isSelectP2;

    //選んだ技を保存
    int m_selectCommandIndexP1[3];
    int m_selectCommandIndexP2[3];

    //サウンド
    std::shared_ptr<BGM> m_bgm;
    std::shared_ptr<SE> m_seP1;
    std::shared_ptr<SE> m_seP2;
   
    //SE
    int m_selectSehandle;
    int m_cursorMoveSehandle;
    int m_cancelSehandle;
public:
    CharacterselectScene(SceneController& controller);

    //派生クラスで実装を実装
    virtual void Update(Input& input, Input& input2) override;

    virtual void Draw() override;


};

