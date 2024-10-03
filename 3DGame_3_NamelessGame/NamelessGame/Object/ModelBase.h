#pragma once
#include "DxLib.h"

class ModelBase
{
public:
	ModelBase();
	~ModelBase();

	virtual void Init();
	virtual void Update();
	virtual void Draw();

protected:

	// プレイヤーデータ
	struct CharaData
	{
		int maxHp;		// 最大HP
		float initPosX;	// 初期化座標X
		float initPosY; // 初期化座標Y
		float initPosZ; // 初期化座標Z
		float modelSize;// モデルサイズ
		float walkSpeed;// 歩く速度
		float rotaSpeed;// 回転速度
	}m_chara;

	int model;				// モデル
	float m_angle;			// プレイヤー向き角度


	VECTOR m_pos;			// 座標
	VECTOR m_move;			// 移動量
	VECTOR m_targetDir;		// プレイヤーが向くべき方向のベクトル


};