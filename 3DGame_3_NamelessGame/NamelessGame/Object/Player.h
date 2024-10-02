#pragma once
#include "DxLib.h"
#include <string>

class Camera;
class Player
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Player();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="camera">カメラ参照</param>
	void Update(const Camera& camera);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 移動処理
	/// </summary>
	/// <param name="camera">カメラ参照</param>
	void Move(const Camera& camera);

	void leftStick(VECTOR moveUp,VECTOR moveLeft);

	/// <summary>
	/// 回転処理
	/// </summary>
	void Angle();

	VECTOR GetPos() const { return m_pos; }	// 座標渡し

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

private:
	int model;				// モデル
	float m_angle;			// プレイヤー向き角度


	int inputX, inputY;



	VECTOR m_pos;			// 座標
	VECTOR m_move;			// 移動量
	VECTOR m_targetDir;		// プレイヤーが向くべき方向のベクトル

	DINPUT_JOYSTATE input;
};