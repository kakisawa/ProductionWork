#pragma once
#include "SceneBase.h"
#include "DxLib.h"
#include <memory>
#include <string>
#include <vector>

class GameMap;
class Camera;
class Player;
class EnemyRight;
class EnemyLeft;
class SceneGame :
    public SceneBase
{
public:
	SceneGame();
	virtual ~SceneGame();

	/// <summary>
	/// ����������
	/// </summary>
	virtual void Init();

	/// <summary>
	/// �X�V����
	/// </summary>
	virtual shared_ptr<SceneBase> Update();

	/// <summary>
	/// �`�揈��
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// �I������
	/// </summary>
	virtual void End();

private:
	int m_explanationGraph;	// ��������摜
	int m_waitCount;		// �Q�[���N���A�����B����̑ҋ@����

	struct Size {			// ��������摜���W
		int m_width;		// ����
		int m_height;		// ����
		int m_widthMax;		// �E��
		int m_heightMax;	// �E��
	}size;

	bool m_isGameClearFlag;			// �Q�[���N���A�t���O
	bool m_isExplanationSet;		// ��������摜�̈ړ��J�n�t���O
	bool m_isExplanationFinishFlag;	// ��������摜�̈ړ��I���t���O
	bool m_isExplanationView;		// ��������摜�̕\���ؑփt���O
	bool m_isPause;					// �|�[�Y�؂�ւ��t���O

	// �}�b�v
	shared_ptr<GameMap> m_pMap = make_shared<GameMap>();
	// �J����
	shared_ptr<Camera> m_pCamera = make_shared<Camera>();
	// �v���C���[
	shared_ptr<Player> m_pPlayer = make_shared<Player>();
	// �E�̓G
	shared_ptr<EnemyRight> m_pEnemyRight = make_shared<EnemyRight>();
	// ���̓G
	shared_ptr<EnemyLeft> m_pEnemyLeft = make_shared<EnemyLeft>();
};

