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
    //�L�����N�^�[�̃A�C�R���n���h��
    int m_iconEmptyHandle;
    int m_iconChara1Handle;
    //�L�����N�^�[�̃C���[�W�摜
    int m_imageChara1Handle;
    //�R�}���h�̃A�C�R��
    int m_commandIconHandle[9];
    int m_selectCommandIconP1Handle[3];//P1���I�񂾃R�}���h��\��
    int m_selectCommandIconP2Handle[3];//P2���I�񂾃R�}���h��\��

    //P1�̃J�[�\��
    int m_cursorP1Handle;
    //P2�̃J�[�\��
    int m_cursorP2Handle;
   
    //���݂̋Z�̃Z���N�g�ԍ�
    int m_currentSelectCommandIndexP1;
    int m_currentSelectCommandIndexP2;
    //�Z��3���肵����true
    bool m_isSelectP1;
    bool m_isSelectP2;

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
public:
    CharacterselectScene(SceneController& controller);

    //�h���N���X�Ŏ���������
    virtual void Update(Input& input, Input& input2) override;

    virtual void Draw() override;


};

