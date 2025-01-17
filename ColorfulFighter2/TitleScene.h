#pragma once
#include "SceneBase.h"
#include <memory>
class BGM;
class TitleScene :
    public SceneBase
{
private:
    int m_titleHandle;
    int m_textHandle;
    std::shared_ptr<BGM> m_bgm;

    //�e�L�X�g���o����������肷��
    int m_countFrame;
    void BlinkingTextDraw();
public:
    TitleScene(SceneController& contoller);

    /// <summary>
    /// �h���N���X�Ŏ���������
    /// virtual �͂�����p�����邩������Ȃ�����
    /// override�͖����I�ɃI�[�o�[���C�h���G�f�B�^�Ɏ�������
    /// </summary>
    virtual void Update(Input& input, Input& input2) override;

    virtual void Draw() override;
};

