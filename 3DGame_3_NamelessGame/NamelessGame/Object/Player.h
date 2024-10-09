#pragma once
#include "DxLib.h"
#include "ModelBase.h"
#include <string>

class Camera;
class Input;
class Player :public ModelBase
{
	struct Situation {
		bool isMoving = false;					//移動中か	
	};

	struct PlayerStatus {
		int animNo;				// アニメーション番号
		Situation situation;	// 状態
		bool isLoop;			// ループ再生
	}m_status;

	// アニメーション
	enum class PlayerAnim {
		Idle,			// 待機
		Run,			// 走る
		Gun1,			// 撃つ(ハンドガン)
		Gun2,			// 撃つ(マシンガン)
		Knife1,			// ナイフ攻撃1
		Knife2,			// ナイフ攻撃2
		Knife3,			// ナイフ攻撃3
		Installation,	// 罠設置
		Drink,			// 飲む
		Reload,			// 弾再装填
		Hit,			// 被ダメージ
		BlownAway,		// 吹っ飛び		// 検討中_未設定
		Roll,			// 回避
		Death,			// 死亡
		Jump,			// ジャンプ		// 下2つ使用検討中
		Landing,		// 落下
		MAX,
	};

	struct AnimChangeTime
	{
		int Idle = 7;
		int Run = 5;

		// ここから下未定
		int Gun = 5;
		int Knife = 5;
		int Installation = 5;
		int Drink = 5;
		int Reload = 5;
		int Hit = 5;
		int Roll = 5;
		int Death = 5;
	}m_animChangeTime;

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Player();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="camera">カメラ参照</param>
	void Update(const Camera& camera);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;

	/// <summary>
	/// 移動処理
	/// </summary>
	/// <param name="camera">カメラ参照</param>
	void Move(const Camera& camera);

	/// <summary>
	/// 回転処理
	/// </summary>
	void Angle();

	/// <summary>
	/// 移動処理更新
	/// </summary>
	void MoveUpdate();


	/// <summary>
	/// アイテム使用
	/// </summary>
	void UseItem();


	/// <summary>
	/// アニメーションの変更
	/// </summary>
	/// <param name="anim">選択するアニメーション</param>
	/// <param name="isAnimLoop">ループさせるか</param>
	/// <param name="changeTime">切り替えにかかる時間</param>
	void ChangeAnimNo(const PlayerAnim anim, const bool isAnimLoop, const int changeTime);

	/// <summary>
	/// アニメーションを待機状態に変更する
	/// </summary>
	void ChangeAnimIdle();

	VECTOR GetPos() const { return m_pos; }	// 座標渡し

private:
	int inputX, inputY;

	DINPUT_JOYSTATE input;
};