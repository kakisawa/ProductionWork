#pragma once
#include "DxLib.h"
#include "ModelBase.h"
#include <string>
#include <array>

class Camera;
class Input;
class Player :public ModelBase
{
public:
	struct Situation {
		bool isMoving = false;			//移動中
		bool isUseItem = false;			// アイテム使用中
		bool isInstallation = false;	// 罠使用中
		bool isDrink = false;			// 回復アイテム使用中
		bool isSummon = false;			// 召喚アイテム使用中
	};

	// プレイヤーの状態
	struct PlayerStatus {
		int animNo;				// アニメーション番号
		float animSpeed;		// アニメーション再生スピード
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
		Summon,			// 召喚
		Reload,			// 弾再装填
		Hit,			// 被ダメージ
		BlownAway,		// 吹っ飛び		// 検討中_未設定
		Roll,			// 回避
		Dying,			// 死亡
		Jump,			// ジャンプ		// 下2つ使用検討中
		Landing,		// 落下
		MAX,
	};

	// アニメーション切り替えにかかる時間
	struct AnimChangeTime
	{
		int Idle = 7;			// 待機
		int Run = 5;			// 走る
		int Installation = 5;	// 罠設置
		int Drink = 1;			// 飲む

		// ここから下未定
		int Gun1 = 5;			// 撃つ(ハンドガン)
		int Gun2 = 5;			// 撃つ(マシンガン)
		int Knife1 = 5;			// ナイフ攻撃1
		int Knife2 = 5;			// ナイフ攻撃2
		int Knife3 = 5;			// ナイフ攻撃3
		int Summon = 5;			// 召喚
		int Reload = 5;			// 弾再装填
		int Hit = 5;			// 被ダメージ
		int Roll = 5;			// 回避
		int Dying = 5;			// 死亡
	}m_animChangeTime;

	// アニメーションの再生速度
	struct AnimSpeed {
		float Default = 0.5f;		// 基本
		float Idle = 0.5f;			// 待機
		float Run = 0.5f;			// 走る
		float Gun1;					// 撃つ(ハンドガン)
		float Gun2;					// 撃つ(マシンガン)
		float Knife1;				// ナイフ攻撃1
		float Knife2;				// ナイフ攻撃2
		float Knife3;				// ナイフ攻撃3
		float Installation = 1.3f;	// 罠設置
		float Drink = 1.3f;			// 飲む
		float Summon=1.0f;			// 召喚
		float Reload;				// 弾再装填
		float Hit;					// 被ダメージ
		float Roll;					// 回避
		float Dying;				// 死亡
	}m_animSpeed;

	// アイテムのジャンル
	enum class Item {
		NoItem,			// アイテム無し
		IceFloor,		// 氷床
		SwivelChair,	// 回転椅子
		landmine,		// 地雷
		SurpriseBox,	// びっくり箱
		RecoveryMedic,	// 体力回復薬
		Ammunition,		// 弾薬
		SummonBox,		// 召喚BOX
	};

	std::array<Item, 3>  m_useItem;

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
	/// <param name="input">入力情報</param>
	void Update(const Camera& camera, Input& input);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

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
	void UseItem(Input& input);

	/// <summary>
	/// アニメーションの変更
	/// </summary>
	/// <param name="anim">選択するアニメーション</param>
	/// <param name="animSpeed">アニメーションの再生速度</param>
	/// <param name="isAnimLoop">ループさせるか</param>
	/// <param name="changeTime">切り替えにかかる時間</param>
	void ChangeAnimNo(const PlayerAnim anim, const float animSpeed, const bool isAnimLoop, const int changeTime);

	/// <summary>
	/// アニメーションを待機状態に変更する
	/// </summary>
	void ChangeAnimIdle();

	VECTOR GetPos() const { return m_pos; }	// 座標渡し

private:

	// 左パッド入力用
	int m_inputX, m_inputY;	
	DINPUT_JOYSTATE m_input;
};