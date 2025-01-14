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
    //�v���C���[�̉摜
    int m_p1Handle;
    int m_p2Handle;
    //BGM
    std::shared_ptr<BGM> m_bgm;
public:
    ResultScene(SceneController& controller);
    //�h���N���X�Ŏ���������
    virtual void Update(Input& input, Input& input2) override;

    virtual void Draw() override;

    //���s�̌��ʂ��L�^����
    void RecordResult();
};

