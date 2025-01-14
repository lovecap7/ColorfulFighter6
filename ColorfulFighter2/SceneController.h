#pragma once
//�X�}�[�g�|�C���^���g��
#include<memory>
#include "game.h"
class Input;
class CharaBase;
class SoundManager;
class SceneBase;

/// <summary>
/// �e�V�[�����Ǘ�����N���X
/// �Ǘ��͂��邪�A�@�ۂ��Ǘ��͂��Ȃ�(�@�ۂ̓N���X�ǂ������)
/// �C���^�[�t�F�C�X��񋟂���N���X
/// </summary>
class SceneController
{
public:
	SceneController();

	/// <summary>
	/// Application����Ă΂��֐��B���݂̃V�[����Update�����̂܂܎��s�B
	/// </summary>
	/// <param name="input"></param>
	void Update(Input& input, Input& input2);

	/// <summary>
	/// Application����Ă΂��֐��B���݂̃V�[����Draw�����̂܂܎��s�B
	/// </summary>
	/// <param name="input"></param>
	void Draw();

	/// <summary>
	/// ���̏�Ԃ��Z�b�g����(�Z�b�g����̂͊e��Ԃ̖���)
	/// �e�V�[�����R���g���[���[�����Đ؂�ւ�������
	/// </summary>
	/// <param name="">���̏�Ԃ̃|�C���^</param>
	void ChangeScene(std::shared_ptr<SceneBase>);

	//�I�񂾋Z�̏��
	void SaveSelectCommandIndex(int indexP1[3], int indexP2[3]);//�ۑ�
	int* GetSelectCommandIndex(PlayerIndex playerIndex);

	//�������v���C���[�̃C���f�b�N�X
	void SetWinPlayerIndex(PlayerIndex winPlayerIndex) { m_winPlayerIndex = winPlayerIndex; }
	PlayerIndex GetWinPlayerIndex() { return m_winPlayerIndex; }
private:
	//���ݎ��s���̃V�[��(���������͒m��Ȃ�)
	std::shared_ptr<SceneBase> m_scene;

	//�I�񂾃R�}���h�Z�̃C���f�b�N�X��ۑ�
	int m_selectCommandIndexP1[3];
	int m_selectCommandIndexP2[3];

	//�������v���C���[�̏��
	PlayerIndex m_winPlayerIndex;
};

