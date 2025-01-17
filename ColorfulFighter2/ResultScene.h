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
    //�v���C���[�̉摜
    int m_p1Handle;
    int m_p2Handle;
    //BGM
    std::shared_ptr<BGM> m_bgm;

    //���j���[
    int m_selectMenuIndex;
    void Rematch();//�Đ�
    void Reselect();//�R�}���h�̑I�ђ���

    //�t�F�[�h�C�����邽�߂Ɏg��
    std::shared_ptr<FadeManager> m_fadeManager;
    bool m_isFadeIn;
    //Loading���̉摜
    int m_loadingHandle;
public:
    ResultScene(SceneController& controller);
    //�h���N���X�Ŏ���������
    virtual void Update(Input& input, Input& input2) override;

    virtual void Draw() override;

    //���s�̌��ʂ��L�^����
    void RecordResult();
};

