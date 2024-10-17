#pragma once
#include "Item.h"
#include "ModelBase.h"
#include "DxLib.h"
#include <string>
#include <array>

class Enemy;
class Camera;
class Input;
class Player :
	public ModelBase
{
public:
	struct Situation {
		bool isMoving = false;			// 移動中
		bool isUseItem = false;			// アイテム使用中
		bool isInstallation = false;	// 罠使用中
		bool isDrink = false;			// 回復アイテム使用中
		bool isReload = false;			// 弾再装填アイテム使用中
		bool isSummon = false;			// 召喚アイテム使用中
		bool isAttack = false;			// 攻撃中
	};

	// プレイヤーの状態
	struct PlayerStatus {
		int animNo = -1;				// アニメーション番号
		float animSpeed = 0.0f;			// アニメーション再生スピード
		Situation situation;			// 状態
		bool isLoop = false;			// ループ再生
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
		int Gun1 = 5;			// 撃つ(ハンドガン)
		int Gun2 = 5;			// 撃つ(マシンガン)
		int Knife1 = 5;			// ナイフ攻撃1
		int Knife2 = 5;			// ナイフ攻撃2
		int Knife3 = 5;			// ナイフ攻撃3
		int Installation = 5;	// 罠設置
		int Drink = 1;			// 飲む
		int Summon = 5;			// 召喚
		int Reload = 5;			// 弾再装填

		// ここから下未定
		int Hit = 5;			// 被ダメージ
		int Roll = 5;			// 回避
		int Dying = 5;			// 死亡
	}m_animChangeTime;

	// アニメーションの再生速度
	struct AnimSpeed {
		float Default = 0.5f;		// 基本
		float Idle = 0.5f;			// 待機
		float Run = 0.5f;			// 走る
		float Gun1 = 1.7f;			// 撃つ(ハンドガン)
		float Gun2 = 0.5f;			// 撃つ(マシンガン)
		float Knife1 = 1.0f;		// ナイフ攻撃1
		float Knife2 = 1.0f;		// ナイフ攻撃2
		float Knife3 = 1.0f;		// ナイフ攻撃3
		float Installation = 1.3f;	// 罠設置
		float Drink = 1.3f;			// 飲む
		float Summon = 1.0f;		// 召喚
		float Reload = 0.6f;		// 弾再装填
		float Hit;					// 被ダメージ
		float Roll = 1.0f;			// 回避
		float Dying;				// 死亡
	}m_animSpeed;

	enum class WeaponKind {
		Gun1,		// 銃1
		Gun2,		// 銃2
		Knife,		// ナイフ
	};

	enum class Knife {
		Attack1,	// 1コンボ目
		Attack2,	// 2コンボ目
		Attack3,	// 3コンボ目
	};

	// 所持しているアイテム
	std::array<Item::ItemKind, 3>  m_useItem;

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
	/// <param name="enemy">敵情報参照</param>
	/// <param name="item">アイテム参照</param>
	/// <param name="camera">カメラ参照</param>
	/// <param name="input">入力情報</param>
	void Update(const Enemy& enemy, const Item& item, const Camera& camera, Input& input);

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
	/// 当たり判定更新
	/// </summary>
	/// <param name="enemy">敵情報参照</param>
	/// <param name="item">アイテム参照</param>
	void ColUpdate(const Enemy& enemy, const Item& item);

	/// <summary>
	/// アイテム獲得
	/// </summary>
	void GetItem();

	/// <summary>
	/// 獲得したアイテムを入れ替える
	/// </summary>
	void ItemChange();

	/// <summary>
	/// アイテム使用
	/// </summary>
	void UseItem(Input& input);

	/// <summary>
	/// 対象を追跡する
	/// </summary>
	/// <param name="input">入力</param>
	/// <param name="enemy">敵情報参照</param>
	void LockOn(Input& input,const Enemy& enemy);

	/// <summary>
	/// 武器切り替え
	/// </summary>
	/// <param name="input">入力</param>
	void ChangeWeapon(Input& input);
	
	/// <summary>
	/// ハンドガン攻撃
	/// </summary>
	/// <param name="input">入力</param>
	void AttackHandGun(Input& input);

	/// <summary>
	/// ナイフ攻撃
	/// </summary>
	/// <param name="input">入力</param>
	void AttackKnife(Input& input);

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


	float GetAngle() const { return m_angle; }

	/// <summary>
	/// ロックオンフラグ渡し
	/// </summary>
	/// <returns>ロックオンフラグ</returns>
	bool GetLockOn() const { return m_isLookOn; }

	/// <summary>
	/// 座標渡し
	/// </summary>
	/// <returns>プレイヤーの座標</returns>
	VECTOR GetPos() const { return m_pos; }

	/// <summary>
	/// 照準座標渡し
	/// </summary>
	/// <returns>ロックオン時の照準座標渡し</returns>
	VECTOR GetTargetPos() const { return m_targetPos; }

	/// <summary>
	/// 当たり判定の値渡し
	/// </summary>
	/// <returns>プレイヤーの当たり判定の値</returns>
	Collision GetCol() const { return m_col; }

private:
	int m_item;					// 所持している3つのうち、使用するアイテム
	int m_getItem;				// アイテムをランダムで獲得する際に使用する
	int m_itemGetCount;			// アイテムを獲得するインターバル用
	bool m_isItem;				// アイテムとの当たり判定
	bool m_isLookOn;			// ロックオンフラグ

	VECTOR m_colPos;			// 当たり判定用座標
	VECTOR m_targetPos;			// ロックオン時の照準座標


	Item::ItemKind m_setItem;	// 獲得したアイテムをセットするための値
	WeaponKind m_useWeapon;		// 使用中の武器
	Knife m_knifeSetCombo;		// ナイフ攻撃のコンボ数

	// 左パッド入力用
	int m_inputX, m_inputY;
	DINPUT_JOYSTATE m_input;
};