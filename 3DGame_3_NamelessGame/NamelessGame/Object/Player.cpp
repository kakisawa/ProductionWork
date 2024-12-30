﻿#include "Player.h"
#include "Enemy.h"
#include "Camera.h"
#include "../LoadCsv.h"
#include "../Input.h"
#include "../Manager/SoundManager.h"
#include <cmath>
#include <random>
#include <cassert>

using namespace MyInputInfo;

namespace 
{
	const char* kModelFilePath = "Data/Model/PlayerModel.mv1";			// プレイヤーモデルパス
	const char* kModelRightHandMiddle = "mixamorig:RightHandMiddle4";	// ハンドガン用右手パス
	const char* kModelRightHandRing3 = "mixamorig:RightHandRing3";		// マシンガン用右手パス
	const char* kModelRightHandRing4 = "mixamorig:RightHandRing4";		// ナイフ用右手パス
	const char* kKnifeModelTip = "Knife_Tip";							// ナイフ先端パス

	const char* const kWeaponPath[3] = 
	{
		"Data/Model/Weapon/HandGun.mv1",	// ハンドガン用パス
		"Data/Model/Weapon/MachineGun.mv1",	// マシンガン用パス
		"Data/Model/Weapon/new_Knife.mv1",		// ナイフ用パス
	};

	constexpr int kSecondAttackTime = 25;					// 2コンボ目の攻撃の入力受付時間
	constexpr int kThirdAttackTime = 40;					// 3コンボ目の攻撃の入力受付時間

	constexpr int kMedicRecoveryAmount = 10;				// 回復量

	constexpr float kInitFloat = 0.0f;				// float値初期化
	const VECTOR kInitVec = VGet(0.0f, 0.0f, 0.0f);	// Vector値初期価値

	bool is3Combo = false;	// ナイフ攻撃の3コンボ目かどうか

	bool isGunAnim = false;

	int machineGunCount = 0.0f;

	VECTOR roll = VGet(0.0f, 0.0f, 0.0f);



	bool isRollTest = false;

}

Player::Player() :
	m_useItem(0),
	m_attackTheEnemy(0),
	m_remainingBullets_handgun(100),
	m_remainingBullets_machinegun(100),
	m_getItem(0),
	m_getitemCount(0),
	m_inputX(0),
	m_inputY(0),
	m_isItem(false),
	m_isLookOn(false),
	m_isEnemy(false),
	m_isAttackToEnemy(false),
	m_isAttack(false),
	m_isInvincibleTime(false),
	m_colPos(kInitVec),
	m_targetLockPos(kInitVec),
	m_rightHandPos(kInitVec),
	m_KnifeTipPos(kInitVec),
	m_setItem(Item::ItemKind::NoItem),
	m_useWeapon(WeaponKind::HandGun),
	m_SetComboknife(Knife::Attack1)
{
	// プレイヤー外部データ読み込み
	LoadCsv::GetInstance().LoadCommonFileData(m_chara, "player");
	LoadCsv::GetInstance().LoadPlayerOnlyFileData(m_playerData);
	// プレイヤーモデルの読み込み
	m_model = MV1LoadModel(kModelFilePath);
	assert(m_model != -1);

	// 武器モデルの読み込み
	for (int i = 0; i < m_weapon.size(); i++) {
		m_weapon[i] = MV1LoadModel(kWeaponPath[i]);
		assert(m_weapon[i] != -1);
	}

	// プレイヤーモデルの座標初期値
	m_pos = VGet(m_chara.initPosX, m_chara.initPosY, m_chara.initPosZ);
	// プレイヤーモデルのサイズ初期化
	MV1SetScale(m_model, VGet(m_chara.modelSize, m_chara.modelSize, m_chara.modelSize));

	// プレイヤーの状態初期化
	m_status.situation.isMoving = false;		// 動いていない
	m_status.situation.isInstallation = false;	// 罠を仕掛けていない

	WeaponInfoSet();
}

Player::~Player()
{
	// 武器モデルの削除
	for (int i = 0; i < m_weapon.size(); i++) {
		MV1DeleteModel(m_weapon[i]);
	}

	// サウンドの解放
	m_pSound->ReleaseSound();
}

void Player::Init()
{
	ModelBase::Init();
	m_hp = m_chara.maxHp;		// HPに最大値を入れる
	m_attack = m_playerData["handGun"].attack;	// 攻撃力にハンドガンの攻撃力を入れる

	// 初期アニメーションの設定(待機状態)
	SetAnimation(static_cast<int>(PlayerAnim::Idle), m_animSpeed.Idle, true, false);

	// 仮設定
	m_item[0] = Item::ItemKind::NoItem;
	m_item[1] = Item::ItemKind::NoItem;
	m_item[2] = Item::ItemKind::NoItem;

	// SEの初期化・読み込み
	m_pSound->InitSE();
	m_pSound->LoadSE(SoundManager::SE_Type::kKnifeSE);
	m_pSound->LoadSE(SoundManager::SE_Type::kHandGunSE);

	//ChangeAnimNo(PlayerAnim::Idle, m_animSpeed.Idle, true, m_animChangeTime.Idle);
#ifdef _DEBUG
#endif
}

void Player::Update(const Enemy& enemy, const Item& item, const Camera& camera, Input& input)
{
	// 敵への攻撃力の初期化
	m_attackTheEnemy = 0;
	m_isEnemy = false;

	// 当たり判定の更新
	ColUpdate(enemy, item);
	GetItem();

	// 更新処理
	Move(camera);
	UseItem(input);
	Angle();
	LockOn(input, enemy);
	Roll(input);
	Hit(input,enemy);
	Death();

	SetModelFramePosition(m_model, kModelRightHandMiddle, m_weapon[0], m_weaponSize[0],	m_weaponRota[0]);
	SetModelFramePosition(m_model, kModelRightHandRing3, m_weapon[1], m_weaponSize[1], m_weaponRota[1]);
	SetModelFramePosition(m_model, kModelRightHandRing4, m_weapon[2], m_weaponSize[2], m_weaponRota[2]);

	// 攻撃
	AttackKnife(input);
	AttackGun(input);
	ChangeWeapon(input);

	

	// SEを鳴らす
	PlaySE();

	// 攻撃が当たっていたらエネミーへ攻撃値を渡す
	if (m_isAttack && m_isAttackToEnemy)
	{
		m_attackTheEnemy = m_attack;
		m_isAttack = false;
	}
	
	// アニメーションの更新
	ChangeAnimIdle();
	ModelBase::Update();
}

void Player::Draw()
{
	ModelBase::Draw();
	MV1DrawModel(m_weapon[0]);
	MV1DrawModel(m_weapon[1]);
	MV1DrawModel(m_weapon[2]);

#ifdef _DEBUG
	//DrawFormatString(0, 60, 0xffffff, "Playe:HP=%d", m_hp);
	//DrawFormatString(0, 100, 0xffffff, "Player:m_pos.x=%.2f:z=%.2f", m_pos.x,m_pos.z);
	//DrawFormatString(0, 220, 0xffffff, "Player:m_attack=%d", m_attack);
	//DrawFormatString(0, 240, 0xffffff, "Player:m_remainingBullets_handgun=%d", m_remainingBullets_handgun);
	//DrawFormatString(0, 260, 0xffffff, "Player:m_remainingBullets_machinegun=%d", m_remainingBullets_machinegun);
	//DrawFormatString(0, 280, 0xffffff, "Player:m_angle=%.2f", m_angle);
	DrawFormatString(0, 300, 0xffffff, "Player:m_move.x/y/z=%.2f/%.2f/%.2f", m_move.x, m_move.y, m_move.z);
	//DrawFormatString(0, 340, 0xffffff, "Player:m_targetDir=%.2f", m_targetDir);
	//DrawFormatString(0, 360, 0xffffff, "Player:inputX=%d", m_inputX);
	//DrawFormatString(0, 380, 0xffffff, "Player:inputY=%d", m_inputY);
	//DrawFormatString(0, 400, 0xffffff, "Player:item=%d", m_useItem);
	//DrawFormatString(0, 420, 0xffffff, "Player:m_isItem=%d", m_isItem);
	//DrawFormatString(0, 440, 0xffffff, "Player:m_getItem=%d", m_getItem);
	//DrawFormatString(0, 460, 0xffffff, "Player:itemCount=%d", m_getitemCount);
	//DrawFormatString(0, 480, 0xffffff, "Player:m_useItem[0]=%d", m_item[0]);
	//DrawFormatString(0, 500, 0xffffff, "Player:m_useItem[1]=%d", m_item[1]);
	//DrawFormatString(0, 520, 0xffffff, "Player:m_useItem[2]=%d", m_item[2]);
	//DrawFormatString(0, 540, 0xffffff, "Player:m_useWeapon=%d", m_useWeapon);
	DrawFormatString(0, 560, 0xffffff, "Player:m_animNext.totalTime=%.2f", m_animNext.totalTime);
	DrawFormatString(0, 580, 0xffffff, "Player:m_nextAnimTime=%.2f", m_nextAnimTime);
	//DrawFormatString(0, 640, 0xffffff, "Player:m_status.situation.isKnifeAttack=%d", m_status.situation.isKnifeAttack);
	//DrawFormatString(0, 660, 0xffffff, "Player:m_isEnemy=%d", m_isEnemy);
	//DrawFormatString(0, 680, 0xffffff, "Player:m_isAttackToEnemy=%d", m_isAttackToEnemy);
	//DrawFormatString(0, 700, 0xffffff, "Player:m_isAttack=%d", m_isAttack);
	//DrawFormatString(0, 720, 0xffffff, "Player:m_attackTheEnemy=%d", m_attackTheEnemy);

	DrawFormatString(0, 280, 0xffffff, "m_status.situation.isRoll=%d", m_status.situation.isRoll);
	DrawFormatString(0, 260, 0xffffff, "isRollTest=%d", isRollTest);

	//// 体の当たり判定描画
	//m_col.TypeChangeCapsuleDraw(m_col.m_colPlayer.m_body, 0xffff00, false);

	//// ナイフ当たり判定の描画
	//if (m_status.situation.isKnifeAttack) {
	//	m_col.TypeChangeCapsuleDraw(m_col.m_colPlayer.m_weapon, 0xff00ff, false);
	//}
#endif // DEBUG
}

void Player::SetModelFramePosition(int ModelHandle, const char* FrameName, int SetModelHandle, VECTOR ModelSize, VECTOR ModelRota)
{
	MATRIX FrameMatrix;
	int FrameIndex;
	MATRIX RotateMatrix;

	// フレーム名からフレーム番号を取得する
	FrameIndex = MV1SearchFrame(ModelHandle, FrameName);

	// フレームの現在のワールドでの状態を示す行列を取得する
	FrameMatrix = MV1GetFrameLocalWorldMatrix(ModelHandle, FrameIndex);

	// 角度を回転させる為
	CreateRotationXYZMatrix(&RotateMatrix, ModelRota.x, ModelRota.y, ModelRota.z);
	MV1SetMatrix(SetModelHandle, MMult(MMult(RotateMatrix, MGetScale(ModelSize)), FrameMatrix));
}

void Player::Move(const Camera& camera)
{
	if (m_status.situation.isUseItem || m_status.situation.isReload ||
		 m_status.situation.isDamageReceived ||
		m_status.situation.isKnifeAttack||
		m_status.situation.isRoll)	return;

	// カメラの向きベクトルを取得
	VECTOR cameraForwardVec = VSub(camera.GetTarget(), camera.GetPosition());
	cameraForwardVec.y = 0.0f; // 水平成分のみ考慮する
	cameraForwardVec = VNorm(cameraForwardVec); // 正規化

	// カメラの右方向ベクトルを取得（forwardベクトルとY軸上ベクトルの外積で算出）
	VECTOR cameraRightVec = VCross(cameraForwardVec, VGet(0.0f, 1.0f, 0.0f));

	// 入力の初期化
	m_move = kInitVec;
	m_inputX = m_inputY = 0;

	// 入力取得
	GetJoypadAnalogInput(&m_inputX, &m_inputY, DX_INPUT_KEY_PAD1);

	// カメラ基準でプレイヤーの移動ベクトルを設定
	m_move = VScale(cameraForwardVec, static_cast<float>(-m_inputY));  // 前後移動
	m_move = VAdd(m_move, VScale(cameraRightVec, static_cast<float>(-m_inputX)));  // 左右移動

	// 正規化と移動速度の適用
	if (VSize(m_move) > 0.0f)
	{
		m_move = VNorm(m_move); // 正規化
		m_targetDir = m_move;  // 目標方向を保存
		m_move = VScale(m_move, m_chara.walkSpeed); // 移動速度を適用
	}
	// 設置アニメーションを再生していないときは移動する
	m_pos = VAdd(m_pos, m_move);

	// 移動処理の更新
	MoveUpdate();
}

void Player::Angle()
{
	// プレイヤーの移動方向にモデルの方向を近づける
	float targetAngle;		// 目標角度
	float difference;		// 目標角度と現在の角度の差

	// 目標の方向ベクトルから角度値を算出する
	targetAngle = static_cast<float>(atan2(m_targetDir.x, m_targetDir.z));

	// 目標の角度と現在の角度との差を割り出す
	difference = targetAngle - m_angle;

	// 差の角度が180度以上になっていたら修正する
	if (difference < -DX_PI_F)
	{
		difference += DX_TWO_PI_F;
	}
	else if (difference > DX_PI_F)
	{
		difference -= DX_TWO_PI_F;
	}

	// 角度の差が0に近づける
	if (difference > 0.0f)
	{
		// 差がプラスの場合は引く
		difference -= m_chara.rotaSpeed;
		if (difference < 0.0f)
		{
			difference = 0.0f;
		}
	}
	else
	{
		// 差がマイナスの場合は足す
		difference += m_chara.rotaSpeed;
		if (difference > 0.0f)
		{
			difference = 0.0f;
		}
	}

	// 設置アニメーションを再生していないときは角度を変える
	if (!m_status.situation.isUseItem || m_status.situation.isReload) 
	{
		// モデルの角度を更新
		m_angle = targetAngle - difference;
		MV1SetRotationXYZ(m_model, VGet(0.0f, m_angle + DX_PI_F, 0.0f));
	}
}

void Player::MoveUpdate()
{
	// 移動値を入れる
	float movingSpeed = std::max(m_move.x, m_move.z);

	// プレイヤーの移動状態を初期化する
	m_status.situation.isMoving = false;

	// 移動値があった場合
	if (movingSpeed != 0.0f) 
	{
		// プレイヤーの移動状態をtrueにする
		m_status.situation.isMoving = true;

		// プレイヤーが罠設置状態でないとき、走るアニメーションを入れる
		if (!m_status.situation.isUseItem && !m_status.situation.isGunAttack && !m_status.situation.isKnifeAttack && !m_status.situation.isRoll
			&& !m_status.situation.isDamageReceived) 
		{
			ChangeAnimNo(PlayerAnim::Run, m_animSpeed.Run, true, m_animChangeTime.Idle);
		}
	}
}

void Player::ColUpdate(const Enemy& enemy, const Item& item)
{
	// プレイヤーの当たり判定更新
	m_colPos = VAdd(m_pos, VGet(0.0f,m_chara.bodyColUpY,0.0f));
	m_col.TypeChangeCapsuleUpdate(m_col.m_colPlayer.m_body, m_pos, m_colPos, m_chara.bodyColRad);

	// 敵・アイテムの当たり判定獲得
	Collision itemCol = item.GetCol();
	Collision enemyCol = enemy.GetCol();

	// アイテムとプレイヤーが当たったかどうかの判定
	m_isItem = m_col.IsTypeChageSphereToCapsuleCollision(m_col.m_colPlayer.m_body, itemCol.m_itemCol);

	// 敵とプレイヤーが当たったかどうかの判定
	if (m_col.IsTypeChageCupsuleCollision(m_col.m_colPlayer.m_body, enemyCol.m_colEnemy.m_body))
	{
		// 当たっていたらその分押し返す
		m_isEnemy = true;
		VECTOR colNormal = VNorm(VSub(m_pos, enemy.GetPos()));
		m_pos = VAdd(m_pos, VScale(colNormal, m_chara.walkSpeed));
	}

	// 敵にプレイヤーのナイフ攻撃が当たったかどうかの判定
	m_isAttackToEnemy = m_col.IsTypeChageCupsuleCollision(m_col.m_colPlayer.m_weapon, enemyCol.m_colEnemy.m_body);
}

void Player::GetItem()
{
	m_getitemCount++;	// アイテム獲得用のインターバル

	if (m_isItem && m_getitemCount >= kItemRespawnTime)
	{
		// ランダムで値を獲得する
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<> rand(1, kItemKind + 1);
		m_getItem = static_cast<int>(rand(mt));

		m_getitemCount = 0;

		ItemChange();

		// 空いているアイテム欄に獲得したアイテムを入れる
		if (m_item[0] == Item::ItemKind::NoItem)
		{
			m_item[0] = m_setItem;
		}
		else if (m_item[1] == Item::ItemKind::NoItem)
		{
			m_item[1] = m_setItem;
		}
		else if (m_item[2] == Item::ItemKind::NoItem)
		{
			m_item[2] = m_setItem;
		}
	}
}

void Player::ItemChange()
{
	if (m_getItem == 1)
	{
		m_setItem = Item::ItemKind::IceFloor;
	}
	if (m_getItem == 2)
	{
		m_setItem = Item::ItemKind::SwivelChair;
	}
	if (m_getItem == 3)
	{
		m_setItem = Item::ItemKind::landmine;
	}
	if (m_getItem == 4)
	{
		m_setItem = Item::ItemKind::SurpriseBox;
	}
	if (m_getItem == 5)
	{
		m_setItem = Item::ItemKind::RecoveryMedic;
	}
	if (m_getItem == 6)
	{
		m_setItem = Item::ItemKind::Ammunition;
	}
	if (m_getItem == 7)
	{
		m_setItem = Item::ItemKind::SummonBox;
	}
}

void Player::UseItem(Input& input)
{
	// Xボタンでアイテムを切り替える
	if (input.IsTrigger(InputInfo::UseItemChange))
	{
		m_useItem++;
		if (m_useItem >= 3)
		{
			m_useItem = 0;
		}
	}

	// Yボタンでアイテムを使う
	if (input.IsTrigger(InputInfo::UseItem))
	{
		// アイテムを持っていなかったら処理をしない
		if (m_item[m_useItem] == Item::ItemKind::NoItem)	return;

		// 状態をアイテム使用中にする
		m_status.situation.isUseItem = true;

		// 使用アイテムが罠系だった場合
		{
			// 使用するアイテムが氷床だった場合
			if (m_item[m_useItem] == Item::ItemKind::IceFloor)
			{
				//プレイヤーの罠設置状態をtrueにする
				m_status.situation.isInstallation = true;
				// 罠設置アニメーションを入れる
				ChangeAnimNo(PlayerAnim::Installation, m_animSpeed.Installation, false, m_animChangeTime.Installation);
			}

			// 使用するアイテムが回転椅子だった場合
			if (m_item[m_useItem] == Item::ItemKind::SwivelChair)
			{
				//プレイヤーの罠設置状態をtrueにする
				m_status.situation.isInstallation = true;
				// 罠設置アニメーションを入れる
				ChangeAnimNo(PlayerAnim::Installation, m_animSpeed.Installation, false, m_animChangeTime.Installation);
			}

			// 使用するアイテムが地雷だった場合
			if (m_item[m_useItem] == Item::ItemKind::landmine)
			{
				//プレイヤーの罠設置状態をtrueにする
				m_status.situation.isInstallation = true;
				// 罠設置アニメーションを入れる
				ChangeAnimNo(PlayerAnim::Installation, m_animSpeed.Installation, false, m_animChangeTime.Installation);
			}

			// 使用するアイテムがびっくり箱だった場合
			if (m_item[m_useItem] == Item::ItemKind::SurpriseBox)
			{
				//プレイヤーの罠設置状態をtrueにする
				m_status.situation.isInstallation = true;
				// 罠設置アニメーションを入れる
				ChangeAnimNo(PlayerAnim::Installation, m_animSpeed.Installation, false, m_animChangeTime.Installation);
			}
		}


		// 使用するアイテムが体力回復だった場合
		if (m_item[m_useItem] == Item::ItemKind::RecoveryMedic) 
		{
			// プレイヤーの飲む状態をtrueにする
			m_status.situation.isDrink = true;
			// 飲むアニメーションを入れる
			ChangeAnimNo(PlayerAnim::Drink, m_animSpeed.Drink, false, m_animChangeTime.Drink);
			if (m_hp < m_chara.maxHp) 
			{
				//m_hp += kMedicRecoveryAmount;

				m_hp += std::min(m_chara.maxHp - m_hp, kMedicRecoveryAmount);
			}
		}

		// 使用するアイテムが弾再装填アイテムだった場合
		if (m_item[m_useItem] == Item::ItemKind::Ammunition) 
		{
			// プレイヤーの弾再装填状態をtrueにする
			m_status.situation.isReload = true;
			// 弾再装填のアニメーションを入れる
			ChangeAnimNo(PlayerAnim::Reload, m_animSpeed.Reload, false, m_animChangeTime.Reload);
		}

		// 使用するアイテムが召喚アイテムだった場合
		if (m_item[m_useItem] == Item::ItemKind::SummonBox)
		{
			//プレイヤーの召喚状態をtrueにする
			m_status.situation.isSummon = true;
			//召喚するアニメーションを入れる
			ChangeAnimNo(PlayerAnim::Summon, m_animSpeed.Summon, false, m_animChangeTime.Summon);
		}
	}

	// プレイヤーがアイテム使用状態かつ、アイテム使用アニメーションが終了したら
	if (m_status.situation.isUseItem && IsAnimEnd())
	{
		// プレイヤーのアイテム使用状態を初期化する
		m_status.situation.isInstallation = false;
		m_status.situation.isDrink = false;
		m_status.situation.isReload = false;
		m_status.situation.isSummon = false;
		m_status.situation.isUseItem = false;

		m_item[m_useItem] = Item::ItemKind::NoItem;
	}
}

void Player::LockOn(Input& input, const Enemy& enemy)
{
	m_isLookOn = false;
	if (input.IsPress(InputInfo::TargetLockOn))
	{
		m_targetLockPos = enemy.GetPos();
		m_isLookOn = true;
	}
}

void Player::ChangeWeapon(Input& input)
{
	if (m_status.situation.isGunAttack || m_status.situation.isKnifeAttack)	return;

	if (input.IsTrigger(InputInfo::ChangeWeapon))
	{
		// 現在の武器がハンドガンなら、マシンガンへ変更する
		if (m_useWeapon == WeaponKind::HandGun)
		{
			m_useWeapon = WeaponKind::MachineGun;
			m_attack = m_playerData["machineGun"].attack;
		}// 現在の武器がマシンガンなら、ナイフへ変更する
		else if (m_useWeapon == WeaponKind::MachineGun)
		{
			m_useWeapon = WeaponKind::Knife;
			m_attack = m_playerData["knife"].attack;
		}// 現在の武器がナイフなら、ハンドガンへ変更する
		else if (m_useWeapon == WeaponKind::Knife)
		{
			m_useWeapon = WeaponKind::HandGun;
			m_attack = m_playerData["handGun"].attack;
		}
	}
}

void Player::AttackGun(Input& input)
{
	// TODO:処理の途中
		// 弾を放つ処理をしたい

	if (m_status.situation.isRoll) return;
	if (m_useWeapon == WeaponKind::Knife)	return;

	m_status.situation.isGunAttack = false;

	if (input.IsPress(InputInfo::Attack) && (m_useWeapon == WeaponKind::HandGun || m_useWeapon == WeaponKind::MachineGun))
	{
		m_status.situation.isGunAttack = true;

		if (m_useWeapon == WeaponKind::HandGun)
		{
			SetModelFramePosition(m_model, kModelRightHandRing3, m_weapon[0], m_weaponSize[0], m_weaponRota[0]);
			MV1SetVisible(m_weapon[0], true);

			ChangeAnimNo(PlayerAnim::HandGun2, m_animSpeed.HandGun, true, m_animChangeTime.HandGun);
			
			if (m_isLoopFinish)
			{
				isGunAnim = false;
			}

			if (m_nextAnimTime < 1.0f && isGunAnim == false)
			{
				if (m_remainingBullets_handgun > 0) 
				{
					m_remainingBullets_handgun--;
					m_pSound->PlaySE(SoundManager::SE_Type::kHandGunSE, DX_PLAYTYPE_BACK);
					isGunAnim = true;
				}
			}
		}
		else if (m_useWeapon == WeaponKind::MachineGun)
		{
			SetModelFramePosition(m_model, kModelRightHandRing3, m_weapon[1], m_weaponSize[1], m_weaponRota[1]);
			MV1SetVisible(m_weapon[1], true);

			ChangeAnimNo(PlayerAnim::MachineGun2, m_animSpeed.MachineGun, true, m_animChangeTime.MachineGun);

			machineGunCount++;
			if (machineGunCount >= 5)
			{
				if (m_remainingBullets_machinegun > 0)
				{
					m_remainingBullets_machinegun--;
					m_pSound->PlaySE(SoundManager::SE_Type::kHandGunSE, DX_PLAYTYPE_BACK);
					machineGunCount = 0;
				}
			}
		}
	}
}

void Player::AttackKnife(Input& input)
{
	// 回避行動をとっている場合処理しない
	if (m_status.situation.isRoll) return;

	// 武器の種類が銃だった場合処理しない
	if (m_useWeapon == WeaponKind::HandGun)	return;
	if (m_useWeapon == WeaponKind::MachineGun)	return;

	// 武器を持たせる場所の獲得
	int handle = MV1SearchFrame(m_model, kModelRightHandRing4);
	int knifeTip = MV1SearchFrame(m_weapon[2], kKnifeModelTip);

	// 武器を持たせる場所のセット
	m_rightHandPos = MV1GetFramePosition(m_model, handle);  // ボーンの位置を取得
	m_KnifeTipPos = MV1GetFramePosition(m_weapon[2], knifeTip);

	// ナイフで攻撃をしている時のみ、当たり判定も移動させる
	if (m_status.situation.isKnifeAttack)
	{
		m_col.TypeChangeCapsuleUpdate(m_col.m_colPlayer.m_weapon, m_rightHandPos, m_KnifeTipPos, m_playerData["knife"].HitRad);
	}

	// 攻撃アニメーション系処理
	if (input.IsTrigger(InputInfo::Attack)) 
	{
		// 1コンボ目攻撃
		if (m_SetComboknife == Knife::Attack1) 
		{
			m_status.situation.isKnifeAttack = true;
			m_SetComboknife = Knife::Attack2;
			ChangeAnimNo(PlayerAnim::Knife1, m_animSpeed.Knife1, false, m_animChangeTime.Knife1);
			m_isAttack = true;

			SetModelFramePosition(m_model, kModelRightHandRing4, m_weapon[2], m_weaponSize[2], kInitVec);
			MV1SetVisible(m_weapon[2], true);
		}

		// 2コンボ目
		else if (m_SetComboknife == Knife::Attack2 && (m_nextAnimTime >= kSecondAttackTime)) 
		{
			m_status.situation.isKnifeAttack = true;
			m_SetComboknife = Knife::Attack3;
			ChangeAnimNo(PlayerAnim::Knife2, m_animSpeed.Knife2, false, m_animChangeTime.Knife2);
			m_isAttack = true;

			SetModelFramePosition(m_model, kModelRightHandRing4, m_weapon[2], m_weaponSize[2], kInitVec);
			MV1SetVisible(m_weapon[2], true);
		}

		// 3コンボ目
		else if (m_SetComboknife == Knife::Attack3 && (m_nextAnimTime >= kThirdAttackTime))
		{
			m_status.situation.isKnifeAttack = true;
			ChangeAnimNo(PlayerAnim::Knife3, m_animSpeed.Knife3, false, m_animChangeTime.Knife3);
			if(!is3Combo)
			{
				m_isAttack = true;
			}

			SetModelFramePosition(m_model, kModelRightHandRing4, m_weapon[2], m_weaponSize[2], m_weaponRota[2]);
			MV1SetVisible(m_weapon[2], true);
			is3Combo = true;
		}
	}

	// アニメーションが終わったらコンボ攻撃を初期化する
	if (m_status.situation.isKnifeAttack && IsAnimEnd())
	{
		m_status.situation.isKnifeAttack = false;
		m_SetComboknife = Knife::Attack1;
		m_isAttack = false;
		is3Combo = false;
	}

}

void Player::Roll(Input& input)
{
	if (m_status.situation.isDamageReceived)return;
	

	if (input.IsTrigger(InputInfo::Roll))
	{
		ChangeAnimNo(PlayerAnim::Roll, m_animSpeed.Roll, false, m_animChangeTime.Roll);
		m_status.situation.isRoll = true;

		if (VSize(m_move) <= 0.0f)
		{
			roll = m_targetDir;
			roll = VScale(roll, m_chara.walkSpeed);
		}
		else {
			roll = m_move;
		}
	}

	if (!m_status.situation.isRoll) return;

	m_pos = VAdd(m_pos,roll);

	if (m_status.situation.isRoll && IsAnimEnd())
	{
		m_status.situation.isRoll = false;
		roll = VGet(0.0f, 0.0f, 0.0f);
	}
}

void Player::Hit(Input& input, const Enemy& enemy)
{
	if (m_status.situation.isDeath) return;

	isRollTest = false;

	/*回避開始から20フレームの間は以下の処理に進まないようにするプログラム*/
	/*作成途中*/
	if (m_status.situation.isRoll && (m_nextAnimTime <= 20.0f))return;

	isRollTest = true;

	// 敵からの攻撃が当たったらの処理
	if (enemy.GetAttack() > 0) 
	{
		ChangeAnimNo(PlayerAnim::DamageReceived, m_animSpeed.DamageReceived, false, m_animChangeTime.DamageReceived);
		m_status.situation.isDamageReceived = true;
		
		if (!m_isInvincibleTime) 
		{
			m_hp -= enemy.GetAttack();
			m_isInvincibleTime = true;
		}
	}

	if (m_status.situation.isDamageReceived && IsAnimEnd())
	{
		m_status.situation.isDamageReceived = false;
		m_isInvincibleTime = false;
	}
}

void Player::ChangeAnimNo(const PlayerAnim anim, const float animSpeed, const bool isAnimLoop, const int changeTime)
{
	m_status.animNo = static_cast<int>(anim);
	m_status.animSpeed = animSpeed;
	m_status.isLoop = isAnimLoop;
	ChangeAnimation(m_status.animNo, animSpeed, m_status.isLoop, false, changeTime);
}

void Player::ChangeAnimIdle()
{
	// 待機アニメーションに変更する
	if (!m_status.situation.isUseItem && !m_status.situation.isMoving && !m_status.situation.isGunAttack && !m_status.situation.isKnifeAttack
		&& !m_status.situation.isRoll && !m_status.situation.isDamageReceived && !m_status.situation.isDeath) 
	{
		ChangeAnimNo(PlayerAnim::Idle, m_animSpeed.Idle, true, m_animChangeTime.Idle);
		MV1SetVisible(m_weapon[0], false);
		MV1SetVisible(m_weapon[1], false);
		MV1SetVisible(m_weapon[2], false);
	}
}

void Player::WeaponInfoSet()
{
	m_weaponSize[0] = VGet(m_playerData["handGun"].sizeX, m_playerData["handGun"].sizeY, m_playerData["handGun"].sizeZ);
	m_weaponSize[1] = VGet(m_playerData["machineGun"].sizeX, m_playerData["machineGun"].sizeY, m_playerData["machineGun"].sizeZ);
	m_weaponSize[2] = VGet(m_playerData["knife"].sizeX, m_playerData["knife"].sizeY, m_playerData["knife"].sizeZ);

	m_weaponRota[0] = VGet(m_playerData["handGun"].RotaX, m_playerData["handGun"].RotaY, m_playerData["handGun"].RotaZ);
	m_weaponRota[1] = VGet(m_playerData["machineGun"].RotaX, m_playerData["machineGun"].RotaY, m_playerData["machineGun"].RotaZ);
	m_weaponRota[2] = VGet(m_playerData["knife"].RotaX, m_playerData["knife"].RotaY, m_playerData["knife"].RotaZ);
}

void Player::Death()
{
	// HPが0になったら死亡アニメーションを流す
	if (m_hp <= 0)
	{
		m_status.situation.isDeath = true;
		ChangeAnimNo(PlayerAnim::Dying, m_animSpeed.Dying, false, m_animChangeTime.Dying);
	}

	// 死亡アニメーションが終わったらゲームオーバーシーンへ切り替わる
	if (m_status.situation.isDeath && IsAnimEnd())
	{
		m_deathFlag = true;
	}
}

void Player::PlaySE()
{
	if (m_useWeapon == WeaponKind::Knife) 
	{
		if (m_isAttack && m_isAttackToEnemy)
		{
			m_pSound->PlaySE(SoundManager::SE_Type::kKnifeSE, DX_PLAYTYPE_BACK);
		}
	}
}
