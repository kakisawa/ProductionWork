#pragma once
#include <deque>
#include <memory>

class SceneBase;
class InputState;

class SceneManager
{
public:
	/// <summary>
	/// �V�[���J��
	/// </summary>
	/// <param name="scene">�V�[���̃|�C���^</param>
	void ChangeScene(const std::shared_ptr<SceneBase>& scene);

	/// <summary>
	/// ���C���̃V�[����ۂ����܂ܕʂ̃V�[����O��ʂɕ\������
	/// </summary>
	/// <param name="scene">�V�[���̃|�C���^</param>
	void PushFrontScene(const std::shared_ptr<SceneBase>& scene);

	/// <summary>
	/// �V�[��������ւ���
	/// </summary>
	/// <param name="scene">�V�[���̃|�C���^</param>
	void SwapScene(const std::shared_ptr<SceneBase>& scene);

	/// <summary>
	/// �ЂƂO�̃V�[���ɖ߂�
	/// </summary>
	void PopFrontScene();	

	/// <summary>
	/// �V�[���̍X�V
	/// </summary>
	void Update();	

	/// <summary>
	/// �V�[���̕`��
	/// </summary>
	void Draw();	

	/// <summary>
	/// ���C��while�����I�������邽��
	/// </summary>
	/// <returns>�Q�[�����I�������邩 true:�I�� false:�I�����Ȃ�</returns>
	bool End() const { return isEnd_; };		

	////////////////Getter////////////////

	/// <summary>
	/// �E�B���h�E���[�h�̎擾
	/// </summary>
	/// <returns></returns>
	bool GetWindowMode() const { return windowMode_; }

	////////////////Setter////////////////

	/// <summary>
	/// �I���t���O��ύX����
	/// </summary>
	/// <param name="flag">�I�����邩</param>
	void SetEndFlag(const bool flag) { isEnd_ = flag; }

	/// <summary>
	/// �E�B���h�E���[�h��ύX����
	/// </summary>
	/// <param name="windowMode">true�FwindowMode�@false�F�t���X�N���[��</param>
	void SetChangeWindowMode(const bool windowMode);

private:
	int debugDrawTime_ = 0;								//�`��ɂ����鎞��(1�t���[��������)
	int debugUpdateTime_ = 0;							//�X�V�ɂ����鎞��(1�t���[��������)

	bool isEnd_ = false;								//�I���t���O
	bool windowMode_ = true;							//windowMode�@true�FwindowMode�@false�F�t���X�N���[��

	std::deque<std::shared_ptr<SceneBase>> scenes_;		//�V�[���{��
};

