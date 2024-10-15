#include "Player.h"
#include "../Camera.h"
#include "../LoadCsv.h"
#include "../Input.h"
#include <cmath>
#include <cassert>

namespace {
	const VECTOR kInitVec = VGet(0.0f, 0.0f, 0.0f);	// Vector値初期価値
	constexpr float kInitFloat = 0.0f;				// float値初期化

	const char* kModelFilePath = "Data/Model/PlayerModel.mv1";	// プレイヤーモデルパス
}

Player::Player() :
	m_item(0),
	m_inputX(0),
	m_inputY(0)
{
	// プレイヤー外部データ読み込み
	LoadCsv::GetInstance().LoadData(m_chara, "player");
	// モデルの読み込み
	m_model = MV1LoadModel(kModelFilePath);
	assert(m_model != -1);

	// 座標初期値
	m_pos = VGet(m_chara.initPosX, m_chara.initPosY, m_chara.initPosZ);
	MV1SetScale(m_model, VGet(m_chara.modelSize, m_chara.modelSize, m_chara.modelSize));

	// プレイヤーの状態初期化
	m_status.situation.isMoving = false;
	m_status.situation.isInstallation = false;
}

Player::~Player()
{
}

void Player::Init()
{
	ModelBase::Init();

	// 初期アニメーションの設定
	SetAnimation(static_cast<int>(PlayerAnim::Idle),m_animSpeed.Idle, true, false);


	// 仮設定
	m_useItem[0] = Item::ItemKind::IceFloor;
	m_useItem[1] = Item::ItemKind::RecoveryMedic;
	m_useItem[2] = Item::ItemKind::SummonBox;
}

void Player::Update(const Camera& camera, Input& input)
{
	// 更新処理
	Move(camera);
	UseItem(input);
	Angle();
	
	// アニメーションの更新
	ChangeAnimIdle();
	ModelBase::Update();
}

void Player::Draw()
{
	ModelBase::Draw();

#ifdef _DEBUG
	DrawFormatString(0, 300, 0xffffff, "m_move.x/y/z=%.2f/%.2f/%.2f", m_move.x, m_move.y, m_move.z);
	DrawFormatString(0, 340, 0xffffff, "m_targetDir=%.2f", m_targetDir);
	DrawFormatString(0, 360, 0xffffff, "inputX=%d", m_inputX);
	DrawFormatString(0, 380, 0xffffff, "inputY=%d", m_inputY);
	DrawFormatString(0, 400, 0xffffff, "item=%d", m_item);
#endif // DEBUG
}

void Player::Move(const Camera& camera)
{
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
	if (VSize(m_move) > 0.0f) {
		m_move = VNorm(m_move); // 正規化
		m_targetDir = m_move;  // 目標方向を保存
		m_move = VScale(m_move, m_chara.walkSpeed); // 移動速度を適用
	}

	// 設置アニメーションを再生していないときは移動する
	if (!m_status.situation.isUseItem) {
		m_pos = VAdd(m_pos, m_move);
	}

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
	if (!m_status.situation.isUseItem) {
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
	if (movingSpeed != 0.0f) {
		
		// プレイヤーの移動状態をtrueにする
		m_status.situation.isMoving = true;

		// プレイヤーが罠設置状態でないとき、走るアニメーションを入れる
		if (!m_status.situation.isUseItem) {
			ChangeAnimNo(PlayerAnim::Run, m_animSpeed.Run,true, m_animChangeTime.Idle);
		}
	}
}

void Player::UseItem(Input& input)
{
	// Xボタンでアイテムを切り替える
	if (input.IsTrigger(InputInfo::UseItemChange))
	{
		m_item++;
		if (m_item >= 3)
		{
			m_item = 0;
		}
	}


	// Yボタンでアイテムを使う
	if (input.IsTrigger(InputInfo::UseItem))
	{
		// 状態をアイテム使用中にする
		m_status.situation.isUseItem = true;

		//// 使用するアイテムが罠類だった場合
		if (m_useItem[m_item] == Item::ItemKind::IceFloor) {
		//プレイヤーの罠設置状態をtrueにする
		m_status.situation.isInstallation = true;
		// 罠設置アニメーションを入れる
		ChangeAnimNo(PlayerAnim::Installation, m_animSpeed.Installation, false, m_animChangeTime.Installation);
		}


		// 使用するアイテムが回復系だった場合
		if (m_useItem[m_item] == Item::ItemKind::RecoveryMedic) {
		// プレイヤーの飲む状態をtrueにする
		m_status.situation.isDrink = true;
		// 飲むアニメーションを入れる
		ChangeAnimNo(PlayerAnim::Drink, m_animSpeed.Drink, false, m_animChangeTime.Drink);
		}


		// 使用するアイテムが召喚系だった場合
		if (m_useItem[m_item] == Item::ItemKind::SummonBox) {
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
		m_status.situation.isSummon = false;

		m_status.situation.isUseItem = false;
	}
}

void Player::ChangeAnimNo(const PlayerAnim anim, const float animSpeed,const bool isAnimLoop, const int changeTime)
{
	m_status.animNo = static_cast<int>(anim);
	m_status.animSpeed = animSpeed;
	m_status.isLoop = isAnimLoop;
	ChangeAnimation(m_status.animNo, animSpeed,m_status.isLoop, false, changeTime);
}

void Player::ChangeAnimIdle()
{
	// 待機アニメーションに変更する
	if (!m_status.situation.isUseItem && !m_status.situation.isMoving) {
		ChangeAnimNo(PlayerAnim::Idle,m_animSpeed.Idle, true, m_animChangeTime.Run);
	}	
}