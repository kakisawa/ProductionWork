#pragma once
#include "DxLib.h"

class Collision
{
public:
	Collision();
	virtual ~Collision();

	void Init();

	// 描画
	void DrawMain(unsigned int color, bool isFill);
	void DrawAttack(unsigned int color, bool isFill);

	/// <summary>
	/// 当たり判定の更新
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="upPos">当たり判定頂点座標</param>
	/// <param name="attackRange">攻撃範囲</param>
	/// <param name="colRadius">当たり判定半径</param>
	/// <param name="attackColRadius">攻撃範囲半径</param>
	void UpdateCol(VECTOR pos,VECTOR upPos, VECTOR attackRange, float colRadius, float attackColRadius);

	/// <summary>
	/// 攻撃が当たったかどうかの判定
	/// </summary>
	/// <param name="col">もう片方の当たり判定</param>
	/// <returns>攻撃が当たっていたらtrueを返す</returns>
	bool IsAttackCollision(const Collision& col);

	/// <summary>
	/// 体が当たったかどうかの判定
	/// </summary>
	/// <param name="col">もう片方の当たり判定</param>
	/// <returns>体にあたっていたらtrueを返す</returns>
	bool IsBodyCollision(const Collision& col);

private:
	float m_attackColRadius;
	float m_colRadius;

	VECTOR m_pos;
	VECTOR m_vertexPos;
	VECTOR m_attackRange;
};