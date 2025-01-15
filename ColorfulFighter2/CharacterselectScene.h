#pragma once
#include "SceneBase.h""
#include <memory>

class BGM;
class SE;
class CharaBase;
class FadeManager;
class CharacterselectScene :
    public SceneBase
{
private:
    //�L�����N�^�[�̃A�C�R���n���h��
    int m_iconEmptyHandle;
    int m_iconChara1Handle;
    //�L�����N�^�[�̃C���[�W�摜
    int m_imageChara1Handle;
    //�R�}���h�̃A�C�R��
    int m_commandIconHandle[9];
    int m_selectCommandIconP1Handle[3];//P1���I�񂾃R�}���h��\��
    int m_selectCommandIconP2Handle[3];//P2���I�񂾃R�}���h��\��
    int m_nullCommandIconHandle;//�R�}���h�Z��I��ł��Ȃ����ɕ\������摜
    void DrawCommandIcon();

    //P1�̃J�[�\��
    int m_cursorP1Handle;
    //P2�̃J�[�\��
    int m_cursorP2Handle;
    void DrawCursor();
   
    //���݂̋Z�̃Z���N�g�ԍ�
    int m_currentSelectCommandIndexP1;
    int m_currentSelectCommandIndexP2;
    void DrawSelectPlayerCommandIcon();
    //�Z��3���肵����true
    bool m_isSelectFinishP1;
    bool m_isSelectFinishP2;
    //���ׂĂ̏������I�������true
    bool m_isReadyP1;
    bool m_isReadyP2;
    int m_gettingReadyHandle;//������
    int m_readyHandle;//��������
    int m_currentReadyP1Handle;//�����������ǂ�����\��
    int m_currentReadyP2Handle;//�����������ǂ�����\��

    //�I�񂾋Z��ۑ�
    int m_selectCommandIndexP1[3];
    int m_selectCommandIndexP2[3];

    //�T�E���h
    std::shared_ptr<BGM> m_bgm;
    std::shared_ptr<SE> m_seP1;
    std::shared_ptr<SE> m_seP2;
   
    //SE
    int m_selectSehandle;
    int m_cursorMoveSehandle;
    int m_cancelSehandle;

    //�w�i
    int m_backHandle;
    //�e�L�X�g
    int m_selectTextHandle;
    //�R�}���h�Z��I�ԏ���
    void SelectP1(Input& input);
    void SelectP2(Input& input);

    //�t�F�[�h�C�����邽�߂Ɏg��
    std::shared_ptr<FadeManager> m_fadeManager;
    bool m_isFadeIn;
public:
    CharacterselectScene(SceneController& controller);

    //�h���N���X�Ŏ���������
    virtual void Update(Input& input, Input& input2) override;

    virtual void Draw() override;


};

