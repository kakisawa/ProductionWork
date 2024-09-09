#pragma once
#include "../Model.h"
#include "DxLib.h"
#include <memory>

class Player;
class GameMap;
class EnemyBase
{
public:
	EnemyBase(const char* model,VECTOR pos);
	virtual ~EnemyBase();

	virtual void Init(std::shared_ptr<GameMap> pMap);
	virtual void Update(const Player& player);
	virtual void Draw();
	virtual void End();

protected:
	int m_model;	// �G���f��
	int m_hp;		// �GHP	

	bool m_isAlive;	// �������Ă��邩�t���O

	VECTOR m_pos;

	VECTOR m_move;
	//���f���N���X
	std::shared_ptr<Model> m_pModel;
};