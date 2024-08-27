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
	int m_model;	// �G���f��

	int m_hp;		// �GHP	

	VECTOR m_pos = m_pModel->GetPos();			// �v���C���[�ʒu

	//���f���N���X
	std::shared_ptr<Model> m_pModel;
};