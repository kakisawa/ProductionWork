#pragma once
#include "KeyData.h"
#include <vector>
#include <map>
#include <unordered_map>
#include <string>

enum class InputType
{
	Pause,			// �|�[�Y�{�^��
	Up,				//��
	Down,			//��
	Left,			//��
	Right,			//�E
	UpArrow,		//��
	DownArrow,		//��
	LeftArrow,		//��
	RightArrow,		//�E
	Space,			//space
	Death,			//���S����
	Dush,			//����
	Activate,		//�^��
	Creative,		//�f�o�b�O�p
	Max,			//�ő���̓C���f�b�N�X
};

enum class InputCategory {
	keybd,		//�L�[�{�[�h
	pad,		//�Q�[���p�b�h
};

struct InputInfo {
	InputCategory cat;		//�������ɕۑ�����Ȃ�(�R���p�C�����W�J)
	int id;					//4byte
};

class KeyConfigScene;
class KeyConfigSceneForSceneTitle;

class InputState
{
	friend KeyConfigScene;
	friend KeyConfigSceneForSceneTitle;
public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~InputState();

	/// <summary>
	/// �C���X�^���X�쐬
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static InputState& GetInstance() {
		static InputState instance;
		return instance;
	}

	/// <summary>
	/// �������s�\���͊֐�
	/// </summary>
	/// <param name="type">�L�[�̃^�C�v</param>
	/// <returns>�����Ă��邩�ǂ���</returns>
	bool IsTriggered(const InputType type) const;

	/// <summary>
	/// �������\���͊֐�
	/// </summary>
	/// <param name="type">�L�[�̃^�C�v</param>
	/// <returns>�����Ă��邩�ǂ���</returns>
	bool IsPressed(const InputType type) const;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �ύX���̃L�[�R���t�B�O��tempMapTable�ɏ�������
	/// </summary>
	/// <param name="type">�L�[�^�C�v</param>
	/// <param name="cat">���͑��u�̃J�e�S��</param>
	/// <param name="id">�L�[�̔ԍ�</param>
	void RewriteInputInfo(const InputType type,const InputCategory cat,const int id);

	/// <summary>
	/// �L�[�̕ύX�����s����
	/// </summary>
	void CommitChangedInputInfo();

	/// <summary>
	/// �ύX�O�̃L�[�R���t�B�O�ɖ߂�
	/// </summary>
	void RollbackChangedInputInfo();

	/// <summary>
	/// �L�[�R���t�B�O������������
	/// </summary>
	void ResetInputInfo();

	/// <summary>
	/// �ύX�����L�[��߂�
	/// </summary>
	/// <param name="type">�L�[�̃^�C�v</param>
	/// <param name="cat">���͑��u�̃J�e�S��</param>
	void UndoSelectKey(const InputType type, const  InputCategory cat);

	/// <summary>
	/// �L�[�R���t�B�O���O���t�@�C���Ƃ��ĕۑ�����
	/// </summary>
	void SavekeyInfo();

	/// <summary>
	/// �L�[�R���t�B�O��ǂݍ���
	/// </summary>
	/// <param name="filename">�O���t�@�C���̃p�X</param>
	void LoadKeyInfo(const std::string& filename);

	/// <summary>
	/// �Ō�ɓ��͂��ꂽ���͑��u�𔻕ʂ���
	/// </summary>
	/// <returns>true:�L�[�{�[�h�@false�F�p�b�h</returns>
	bool LastInputDevice() const;

	/// <summary>
	/// key�摜�̕`��
	/// </summary>
	/// <param name="type">�`�悵����key�@cast��int�^�ɂ����l</param>
	/// <param name="posX">�`����WX</param>
	/// <param name="posY">�`����WY</param>
	/// <param name="size">�`��T�C�YY</param>
	void DrawKeyGraph(const int type, const float posX, const float posY, const float size);

	/// <summary>
	/// pad�̃{�^���̕`��
	/// </summary>
	/// <param name="type">�`�悵�����{�^�� cast��int�^�ɂ����l</param>
	/// <param name="posX">�`����WX</param>
	/// <param name="posY">�`����WY</param>
	/// <param name="scale">�`��T�C�YY</param>
	void DrawPadGraph(const int type, const  float posX, const  float posY, const float scale);

	/// <summary>
	/// ���O�̕`��
	/// </summary>
	/// <param name="type">�`�悵����key�@cast��int�^�ɂ����l</param>
	/// <param name="posX">�`����WX</param>
	/// <param name="posY">�`����WY</param>
	/// <param name="color">�F</param>
	/// <param name="fontHandle">�t�H���g</param>
	/// <param name="editName">���O��ҏW���邩</param>
	/// <param name="before">�O���ォ�@true : �O�@false�F���</param>
	/// <param name="sign">�L��</param>
	void DrawName(const int type, const  float posX, const  float posY, const  int color, const  int fontHandle, const bool editName, const bool before,const std::string& sign = "");

	/// <summary>
	/// ���͑��u�̔ԍ����擾����
	/// </summary>
	/// <param name="type">�{�^���̃^�C�v</param>
	/// <param name="cat">���͑��u�̃J�e�S���[</param>
	/// <returns>���͑��u�̔ԍ�</returns>
	int GetInputNum(const int num, const InputCategory cat);
private:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	InputState();

	InputState(const InputState&) = delete;
	void operator = (const InputState&) = delete;
private:

	bool currentInputDevice_ = false;					//true:�L�[�{�[�h�@false:�p�b�h

	using InputMap_t = std::map<InputType, std::vector<InputInfo>>;
	InputMap_t inputMapTable_;							//�Q�[���v���C���Ɏ��ۂɎg����input�e�[�u��

	InputMap_t tempMapTable_;							//���͊��蓖�Ă�ύX����Ƃ���inputMapTable�𒼐ڕύX���Ȃ����߂ɂ��̕ϐ���ύX����

	InputMap_t defaultMapTable_;						//�����̓��͊��蓖��

	std::map<int, Key> keyNum_;							//�L�[�ԍ�
	std::map<int, XboxBotton> padNum_;					//�p�b�h�ԍ�
	std::map<InputType, std::string> inputNameTable_;	//�L�[�ɖ��O��t����悤

	std::vector<bool> currentInput_;					//���݂̓���
	std::vector<bool> lastInput_;						//�Ō�̓���
};
