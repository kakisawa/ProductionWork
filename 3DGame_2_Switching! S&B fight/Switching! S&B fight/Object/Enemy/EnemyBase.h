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
	int m_model;	// 敵モデル
	int m_hp;		// 敵HP	

	bool m_isAlive;	// 生存しているかフラグ

	VECTOR m_pos;

	//モデルクラス
	std::shared_ptr<Model> m_pModel;
};