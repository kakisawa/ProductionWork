#pragma once
#include "SceneBase.h"
#include "DxLib.h"

#include <memory>
#include <string>
#include <vector>

class GameMap;
class Player;
class EnemyRight;
class EnemyLeft;
class Camera;
class SceneGame :
    public SceneBase
{
	/*���Ɨp
private:
	// �z�u���f�[�^
	struct LocationData {
		std::string name;	// ���O
		std::string tag;	// �^�O
		VECTOR pos;			// ���W
		VECTOR rot;			// ��]
		VECTOR scale;		// �X�P�[�����O
	};*/

public:
	SceneGame();
	virtual ~SceneGame();

	virtual void Init();
	virtual shared_ptr<SceneBase> Update();
	virtual void Draw();
	virtual void End();

	
	/*���Ɨp
	void LoadLocations();
	std::vector<LocationData> m_localData;
	*/

private:
	int m_modelMap;
	int m_modelCup;
	int m_arrowModel[4];

	VECTOR m_pos;

	shared_ptr<GameMap> m_pMap = make_shared<GameMap>();
	shared_ptr<Player> m_pPlayer = make_shared<Player>();
	shared_ptr<EnemyRight> m_pEnemyRight = make_shared<EnemyRight>();
	shared_ptr<EnemyLeft> m_pEnemyLeft = make_shared<EnemyLeft>();
	shared_ptr<Camera> m_pCamera = make_shared<Camera>();
};

