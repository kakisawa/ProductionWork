#pragma once
#include "SceneBase.h"
#include <vector>

class Player;
class Camera;
class Stage;
class EnemyBase;
class UIBattle;
class EffectManager;
class Light;

/// <summary>
/// �v���C�V�[���̊��N���X
/// </summary>
class SceneStageBase : public SceneBase
{
public:
	SceneStageBase();
	SceneStageBase(std::shared_ptr<Player> pPlayer, std::shared_ptr<Camera> pCamera, std::shared_ptr<Stage> pStage);
	virtual ~SceneStageBase();
	virtual void Init();
	virtual std::shared_ptr<SceneBase> Update(Input& input) = 0;
	virtual void Draw();
	virtual void ClearProduction();	// �N���A���o���s��
	// �������n�܂�܂ł̎��Ԃ��擾
	int GetBattleStartTime() const { return m_nextBattleTime; }
	// �|�[�Y��Ԃ��ǂ����擾
	bool IsGetPause() const {return m_isPause; }

protected:
	void UpdateNextBattle();	// �G��|���Ď��������n�܂�O�̏���
	void GameoverProduction();	// �Q�[���I�[�o�[���o���s��

protected:
	std::shared_ptr<Player> m_pPlayer;			// �v���C���[
	std::shared_ptr<Camera> m_pCamera;			// �J����
	std::shared_ptr<Stage> m_pStage;			// �X�e�[�W
	std::shared_ptr<EnemyBase> m_pEnemy;		// �G
	std::shared_ptr<UIBattle> m_pUIBattle;		// �o�g��UI
	std::shared_ptr<EffectManager> m_pEffect;	// �G�t�F�N�g
	int m_battleNum;							// ���݂̃o�g����
	int m_clearProductionTime;					// �N���A���o�̎���
	int m_gameoverProductionTime;				// �Q�[���I�[�o�[���o�̎���
	int m_nextBattleTime;						// ���̎������n�܂�܂ł̎���
	int m_elapsedTime;							// �o�ߎ���
	std::vector<int> m_clearTime;				// �e�������Ƃ̃N���A�^�C��
	bool m_isPause;								// �|�[�Y��Ԃ��ǂ���(true:�|�[�Y���)
	int m_shadowMap;							// �V���h�E�}�b�v
	int m_clearBackHandle;						// �N���A���̔w�i�摜
};

