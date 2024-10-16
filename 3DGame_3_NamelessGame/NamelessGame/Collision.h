#pragma once
#include <DxLib.h>

class Collision
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Collision();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Collision();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 球体の当たり判定更新
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="radius">半径</param>
	void SphereUpdate(const VECTOR pos, const float radius);

	/// <summary>
	/// カプセルの当たり判定更新
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="upPos">頂点座標</param>
	/// <param name="radius">半径</param>
	void CapsuleUpdate(const VECTOR pos, const VECTOR upPos, const float radius);

	/// <summary>
	/// 球体の当たり判定描画
	/// </summary>
	/// <param name="color">色</param>
	/// <param name="isFill">球体の色を塗りつぶすか</param>
	void SphereDraw(unsigned int color, bool isFill);

	/// <summary>
	/// カプセルの当たり判定描画
	/// </summary>
	/// <param name="color">色</param>
	/// <param name="isFill">球体の色を塗りつぶすか</param>
	void CapsuleDraw(unsigned int color, bool isFill);

	/// <summary>
	/// 球体の当たり判定
	/// </summary>
	/// <param name="col">もう1つの値</param>
	/// <returns>当たっていたらtrueを返す</returns>
	bool IsSphereCollision(const Collision& col);

	/// <summary>
	/// 球体とカプセルの当たり判定
	/// </summary>
	/// <param name="col">もう1つの値</param>
	/// <returns>当たっていたらtrueを返す</returns>
	bool IsSphereToCapsuleCollision(const Collision& col);

private:
	float m_radius;			// 当たり判定の半径

	VECTOR m_pos;			// 座標
	VECTOR m_vertexPos;		// カプセルの頂点座標
};