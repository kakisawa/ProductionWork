#pragma once
#include "DxLib.h"

class Enemy
{
public:
	Enemy(VECTOR pos);
	~Enemy();

	void Init();
	void Update();
	void Draw();
	void End();

	void SetAttackNum(int num) { m_attackNum = num; }
	void SetAddModelScale(float scale) { m_addScale = scale; }
	void SetAddMove(bool ismove) { m_isAddMove = ismove; }
	void SetAttack(bool isattack) { m_isAttack = isattack; }
	void SetModelAngle(VECTOR angle) { m_angle = angle; }
	void SetMove(VECTOR move) { m_move = move; }
	VECTOR GetPos() { return m_pos; }
	
	
private:

	int m_model;		// “Gƒ‚ƒfƒ‹
	int m_modelBase;	// “Gƒ‚ƒfƒ‹(ƒx[ƒX)
	int m_attackNum;	// “|‚ê‚é•ûŒü‚Ì”Ô†
	int m_attackMoveNum;// “|‚ê‚é

	float m_scale;		// “GƒTƒCƒY
	float m_addScale;	// “GƒTƒCƒY(’Ç‰Á’²®•ª)

	float m_gravity;	// “G‚É‚©‚©‚éd—Í

	bool m_isAddMove;	// “G‚ªˆÚ“®‚·‚é‚©ƒtƒ‰ƒO
	bool m_isAttack;	// “G‚ÌUŒ‚ƒtƒ‰ƒO
	bool m_isFall;		// “G‚ª“|‚ê‚éƒtƒ‰ƒO
	bool m_isMoveStop;
	
	// “G
	VECTOR m_pos;		// “GÀ•W
	VECTOR m_angle;		// “G•ûŒü
	VECTOR m_move;		// “GˆÚ“®—Ê
};

