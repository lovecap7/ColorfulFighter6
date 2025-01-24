#pragma once
#include "SceneBase.h""
#include <memory>

class BGM;
class SE;
class CharaBase;
class FadeManager;
class CommandSelectScene :
    public SceneBase
{
private:
    //キャラクターのアイコンハンドル
    int m_iconEmptyHandle;
    int m_iconChara1Handle;
  
    //コマンドのアイコン
    int m_commandIconHandle[9];
    int m_selectCommandIconP1Handle[3];//P1が選んだコマンドを表示
    int m_selectCommandIconP2Handle[3];//P2が選んだコマンドを表示
    int m_nullCommandIconHandle;//コマンド技を選んでいない時に表示する画像
    //9個並んでるアイコンの描画
    void DrawCommandIcon();

    //P1のカーソル
    int m_cursorP1Handle;
    //P2のカーソル
    int m_cursorP2Handle;
    //カーソルの描画
    void DrawCursor();
    int m_blinkCountFrame;
   
    //現在の技のセレクト番号
    int m_currentSelectCommandIndexP1;
    int m_currentSelectCommandIndexP2;
    //選んだコマンド技の描画
    void DrawSelectPlayerCommandIcon();
    //技を3つ決定したらtrue
    bool m_isSelectFinishP1;
    bool m_isSelectFinishP2;
    //すべての準備が終わったらtrue
    bool m_isReadyP1;
    bool m_isReadyP2;
    int m_gettingReadyHandle;//準備中
    int m_readyHandle;//準備完了
    int m_currentReadyP1Handle;//準備完了かどうかを表示
    int m_currentReadyP2Handle;//準備完了かどうかを表示
	void ReadyDraw();

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

    //背景
    int m_backHandle;
    //テキスト
    int m_selectTextHandle;
    //コマンド技を選ぶ処理
    void SelectCommandP1(Input& input);
    void SelectCommandP2(Input& input);

    //選んでいる色
    int m_currentColorIndexP1;
    int m_currentColorIndexP2;
    //キャラクター
    int m_charaP1Handle;
    int m_charaP2Handle;
	int m_animIndex;
	int m_animCountFrame;
	//キャラクターの色
	int m_charaColorHandle[5];
	//キャラクターの描画
    void CharaDraw();
    //色を選ぶ処理
    void SelectColorP1(Input& input);
    void SelectColorP2(Input& input);

    //フェードインするために使う
    std::shared_ptr<FadeManager> m_fadeManager;
    bool m_isFadeIn;
    //Loading中の画像
    int m_loadingHandle;

    //操作に関するUI
	int m_operationHandle;

public:
    CommandSelectScene(SceneController& controller);

    //派生クラスで実装を実装
    virtual void Update(Input& input, Input& input2) override;

    virtual void Draw() override;


};

