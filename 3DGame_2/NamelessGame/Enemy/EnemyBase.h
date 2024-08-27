#pragma once
#include "../Model.h"
#include "DxLib.h"
#include <memory>

class Player;
class EnemyBase
{
public:
	EnemyBase(const char* model,VECTOR pos);
	virtual ~EnemyBase();

	virtual void Init();
	virtual void Update(const Player& player);
	virtual void Draw();
	virtual void End();

protected:
	int m_model;	// 敵モデル

	int m_hp;		// 敵HP	

	VECTOR m_pos = m_pModel->GetPos();			// プレイヤー位置

	//モデルクラス
	std::shared_ptr<Model> m_pModel;
};