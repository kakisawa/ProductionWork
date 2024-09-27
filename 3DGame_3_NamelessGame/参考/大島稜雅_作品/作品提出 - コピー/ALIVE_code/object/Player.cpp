#include "Player.h"

#include "../object/Corpse.h"
#include "../object/ObjectManager.h"

#include "../gimmick/Lever.h"
#include "../gimmick/ManualCrank.h"

#include "../util/Util.h"
#include "../util/Model.h"
#include "../util/InputState.h"
#include "../util/ExternalFile.h"
#include "../util/SoundManager.h"
#include "../util/ModelManager.h"
#include "../util/EffectManager.h"

#include <cmath>
#include <string>
#include <algorithm>

namespace
{
	//半分
	constexpr int half = 2;

	//走るアニメーションの足音を鳴らすフレーム数
	constexpr int run_anim_foot_step_frame_type_1 = 6;
	constexpr int run_anim_foot_step_frame_type_2 = 45;

	//歩くアニメーションの足音を鳴らすフレーム数
	constexpr int walk_anim_foot_step_frame_type_1 = 80;
	constexpr int walk_anim_foot_step_frame_type_2 = 160;

	//落ち影の調点数
	constexpr int shadow_vertex_num = 7;

	//落ち影で作る三角形の数
	constexpr int shadow_triangle_num = 6;

	//三角形を作る順番の数
	constexpr int number_of_orders_to_form_a_triangle = 18;

	//死体の数をカウントする最大数
	constexpr int max_death_count = 99;

	//アニメーションを変更するのにかかる時間
	constexpr int anim_change_time = 20;

	//落ち影の半径
	constexpr float shadow_radius = 25.0f;

	//音が聞こえる範囲
	constexpr float sound_radius = 1500.0f;

	//落ち影の高さ補正
	constexpr float correction_chadow_height = 5.0f;

	//重力
	constexpr float gravity = -0.4f;

	//プレイヤーの高さ
	constexpr float player_hegiht = 130.0f;

	//Z軸の移動制限
	constexpr float movement_restrictions_Z = -250.0f;

	//3Dサウンドの範囲
	constexpr float sound_range = 1500.0f;

	//フレームの名前
	const char* const frame_name = "hand.R_end";

	//操作を行うオブジェクトとの許容範囲
	constexpr float permissible_range = 30.0f;

	//90度
	constexpr float degree_of_90 = 90.0f;

	//現在の角度から目標の角度まで動かすとき
	//最大で何度まで動かすことが出来るか
	constexpr float max_difference_angle = 135.0f;

	//クランクを回すスピードの倍率
	constexpr float crank_rot_speed_rate = 3.0f;

	//ノックバック率
	constexpr float knockback_rate = 0.96f;

	//ノックバックベクトルの切り捨て範囲
	constexpr float knockback_vector_truncation_range = 2.0f;

	//初期正面ベクトル
	constexpr VECTOR front_vec = { 0,0,-1 };

	//クランクの持ち手の座標
	constexpr VECTOR crank_pos = { 0.0f,1.8f,0.0f };
}

//コンストラクタ
Player::Player(const ObjectInfo& info):updateFunc_(&Player::NormalUpdate),carryUpdateFunc_(&Player::CarryObject)
{
	//短縮化
	auto& model = ModelManager::GetInstance();
	auto& file = ExternalFile::GetInstance();

	//プレイヤー情報の初期化
	playerInfo_ = ExternalFile::GetInstance().GetPlayerInfo();

	//プレイヤーモデルの生成
	model_ = std::make_shared<Model>(model.GetModelHandle(objData_[static_cast<int>(ObjectType::Player)].name), Material::Other);

	//アニメーションの設定
	model_->SetAnimation(static_cast<int>(PlayerAnimType::Idle), true, false);

	//初期ポジションの取得
	VECTOR startPos = file.GetStartPos(file.GetStartName());

	//ポジションの設定
	model_->SetPos(startPos);

	//回転率の設定
	model_->SetRot(info.rot);

	//プレイヤーの大きさの調整
	model_->SetScale(info.scale);

	//コリジョンフレームの設定
	model_->SetUseCollision(true, true, "Character");

	//ポジションの初期化
	status_.pos = startPos;

	//拡縮率の初期化
	scale_ = info.scale;

	//ジャンプ情報の初期
	status_.jump.isJump = false;
	status_.jump.jumpVec = 0.0f;
	//プレイヤーの高さの設定
	status_.height = player_hegiht;
}

// デストラクタ
Player::~Player()
{
	ExternalFile::GetInstance().SetStartName("");
}

//更新
void Player::Update()
{
	//プレイヤーのアニメーション更新
	model_->Update();
	
	(this->*updateFunc_)();
}

//描画
void Player::Draw()
{
	//モデルの描画
	model_->Draw();

	//影もどきの描画
	DrawPolygon3D();
}

//現在の角度から近い角度に回る
void Player::RotateAtACloseAngle(float& differenceAngle, float targetAngle, float currentAngle)
{
	//一回転の角度
	float oneRevolution = 360.0f;

	if (differenceAngle >= oneRevolution / half)
	{
		differenceAngle = targetAngle - currentAngle - oneRevolution;
	}
	else if (differenceAngle <= -oneRevolution / half)
	{
		differenceAngle = targetAngle - currentAngle + oneRevolution;
	}
}

//弾に当たったらノックバックを追加する
void Player::BulletHitMe(const VECTOR& moveVec)
{
	//プレイヤーが死亡中だったらたまによるノックバックを無効にする
	if (updateFunc_ == &Player::DeathUpdate) 
	{
		return;
	}

	//ノックバックを追加する
	status_.moveVec = moveVec;

	//メンバ関数を変更する
	updateFunc_ = &Player::BulletHitMeUpdate;
}

//どんな回転状態か取得する
Player::RotationState Player::WhatRotationState()
{
	//短縮化
	auto& input = InputState::GetInstance();

	//インプット情報
	bool IsPressedUp = input.IsPressed(InputType::Up);
	bool IsPressedDown = input.IsPressed(InputType::Down);
	bool IsPressedLeft = input.IsPressed(InputType::Left);
	bool IsPressedRight = input.IsPressed(InputType::Right);

	//入力状況に応じて回転状態を返す
	if (IsPressedUp && IsPressedRight)		return RotationState::UpperRight;
	if (IsPressedUp && IsPressedLeft)		return RotationState::UpperLeft;
	if (IsPressedDown && IsPressedRight)	return RotationState::LowerRight;
	if (IsPressedDown && IsPressedLeft)		return RotationState::LowerLeft;
	if (IsPressedUp)						return RotationState::Up;
	if (IsPressedDown)						return RotationState::Down;
	if (IsPressedLeft)						return RotationState::Left;
	if (IsPressedRight)						return RotationState::Right;

	//現在のステートを返す
	return currentRotState_;
}

//目標の角度に回転することが出来るか
bool Player::CanRotation(float rotZ)
{
	//一回転の角度
	float oneRevolution = 360.0f;

	//目標の角度と現在の角度との差
	float differenceAngle = rotData_[static_cast<int>(currentRotState_)].targetAngle_ - rotZ;

	//現在の角度から近い角度に回る
	RotateAtACloseAngle(differenceAngle, rotData_[static_cast<int>(currentRotState_)].targetAngle_, rotZ);

	if (differenceAngle > max_difference_angle)			return false;
	if (rotZ + differenceAngle < 0.0f)					return false;
	if (rotZ + differenceAngle > crank_->GetMaxRotZ())	return false;

	return true;
}

//ポジションの設定
void Player::SetPos(const VECTOR& pos)
{
	//ステータスにポジションを代入する
	status_.pos = pos;

	//モデルにポジションを設定する
	model_->SetPos(status_.pos);
}

//ジャンプの設定
void Player::SetJumpInfo(bool isJump, float jumpVec)
{
	//ジャンプしているかのフラグを代入する
	status_.jump.isJump = isJump;

	//ジャンプベクトルを設定する
	status_.jump.jumpVec = jumpVec;

	//ポジションにジャンプベクトルを足す
	status_.pos.y += status_.jump.jumpVec;
}

//持ち運ぶ事が出来るフラグと持ち運ぶモデルのポインタを受け取る
void Player::SetCarryInfo(const bool isCarry, const std::shared_ptr<ObjectBase>& model)
{
	//持ち運び中フラグを設定する
	status_.situation.isCanBeCarried = isCarry;

	//死体のモデルポインタを取得
	corpseModelPointer_ = model;
}

//ManualCrankのポインタを設定する
void Player::SetCrankPointer(const std::shared_ptr<ManualCrank>& crank)
{
	//操作中フラグをtrueに設定する
	status_.situation.isGimmickCanBeOperated = true;

	//クランクのポインタを設定する
	crank_ = crank;
}

//レバーのポインタを設定する
void Player::SetLeverPointer(const std::shared_ptr<Lever>& lever)
{
	//操作中フラグをtrueに設定する
	status_.situation.isGimmickCanBeOperated = true;

	//レバーのポインタを設定する
	lever_ = lever;
}

//地面に描画する影の高さと踏んでいるオブジェクトの素材を設定する
void Player::SetRoundShadowHeightAndMaterial(const float height, const  Material materialType)
{
	//影の高さを設定する
	roundShadowHeight_ = height;

	//足元のオブジェクトの素材を設定する
	materialSteppedOn_ = materialType;
}

//通常時の更新
void Player::NormalUpdate()
{
	//短縮化
	auto& input = InputState::GetInstance();

#ifdef _DEBUG
	//削除予定
	if (input.IsTriggered(InputType::Creative))
	{
		debugCreativeMode_ = !debugCreativeMode_;
	}
#endif
	
	//持ち運び可能なオブジェクトに対してアクションを起こす
	if (input.IsTriggered(InputType::Activate))
	{
		if (status_.situation.isCanBeCarried) 
		{
			(this->*carryUpdateFunc_)();
		}
	}

	//操作を必要とするギミックに対してアクションを起こす
	if (input.IsTriggered(InputType::Activate))
	{
		//ベクトルを0にする
		status_.moveVec = VGet(0, 0, 0);

		//ギミックを操作出来るフラグがtrueならば
		//メンバ関数を変更する
		if (status_.situation.isGimmickCanBeOperated)
		{
			if (crank_ != nullptr)
			{
				//アニメーションの変更
				ChangeAnimNo(PlayerAnimType::Walk, true, anim_change_time);

				//クランクを動かす準備をする
				updateFunc_ = &Player::GoCrankRotationPosition;
			}
			else if (lever_ != nullptr)
			{
				//アニメーションの変更
				ChangeAnimNo(PlayerAnimType::Walk, true, anim_change_time);

				//レバーを動かす準備をする
				updateFunc_ = &Player::GoLeverPullPosition;
			}

			return;
		}
	}
	else {
		//クランクを操作しない場合ポインターをリセットする
		crank_.reset();

		//レバーを操作しない場合ポインターをリセットする
		lever_.reset();
	}

	status_.situation.isGimmickCanBeOperated = false;

	//死体を持ち運び中か
	if (status_.situation.isInTransit)
	{
		//プレイヤーの回転とともに死体のモデルも回転させる
		corpseModelPointer_->GetModelPointer()->SetRot(MathUtil::VECTORDegreeToRadian(status_.rot));

		//死体のポジションを常にプレイヤーの手の座標にする
		VECTOR framePos = model_->GetFrameLocalPosition(frame_name);
		corpseModelPointer_->GetModelPointer()->SetPos(framePos);
	}
	else
	{
		status_.situation.isCanBeCarried = false;
	}

	//アニメーションを待機にする
	ChangeAnimIdle();

	//移動
	MovingUpdate();

	//足音を鳴らす
	FootStepSound();

#ifdef _DEBUG
	//空中にいるとき
	//重力をベクトルに足してポジションに足す
	if (!debugCreativeMode_)
	{
		if (status_.jump.isJump)
		{
			status_.jump.jumpVec += gravity;
			status_.pos.y += status_.jump.jumpVec;
			model_->SetPos(status_.pos);
		}
		else 
		{
			status_.jump.jumpVec = 0.0f;
		}
	}
#else
	//空中にいるとき
	//重力をベクトルに足してポジションに足す
	if (status_.jump.isJump) {
		status_.jump.jumpVec += gravity;
		status_.pos.y += status_.jump.jumpVec;
		model_->SetPos(status_.pos);
	}
	else {
		status_.jump.jumpVec = 0.0f;
	}
#endif // _DEBUG

	//持ち運び中だったら
	//以降の処理を行わない
	if (status_.situation.isInTransit)
	{
		return;
	}

#ifdef _DEBUG
	//メンバ関数ポインタをjumpUpdateに変更する
	if (!debugCreativeMode_)
	{
		if (input.IsTriggered(InputType::Space))
		{
			if (!status_.jump.isJump)
			{
				SetJumpInfo(true,playerInfo_.jumpPower);
			}
			ChangeAnimNo(PlayerAnimType::Jump, false, anim_change_time);
			updateFunc_ = &Player::JumpUpdate;
			return;
		}
	}
	else {
		if (input.IsPressed(InputType::Space))
		{
			status_.moveVec.y = playerInfo_.jumpPower;
		}
	}
#else
	//メンバ関数ポインタをjumpUpdateに変更する
	if (input.IsTriggered(InputType::Space))
	{
		//ジャンプしているフラグがfalseだったら
		//ジャンプ処理を行う
		if (!status_.jump.isJump) {
			SetJumpInfo(true, playerInfo_.jumpPower);
		}

		//アニメーションをジャンプに変更する
		ChangeAnimNo(PlayerAnimType::Jump, false, anim_change_time);

		//メンバ関数を変更する
		updateFunc_ = &Player::JumpUpdate;
		return;
	}
#endif // _DEBUG

	//メンバ関数ポインタをDeathUpdateに変更する
	if (input.IsTriggered(InputType::Death))
	{
		//死体の数をカウントする
		deathCount_ = (std::min)(deathCount_ + 1, max_death_count);

		//移動ベクトルを0にする
		status_.moveVec = VGet(0, 0, 0);

		//メンバ関数を変更する
		updateFunc_ = &Player::DeathUpdate;
		return;
	}
}

//移動の更新
void Player::MovingUpdate()
{
	//短縮化
	auto& input = InputState::GetInstance();

	//移動スピードの取得
	float movingSpeed = Move();
	
	if (movingSpeed != 0.0f) 
	{
		if (movingSpeed > playerInfo_.walkSpeed)
		{
			//アニメーションの変更
			ChangeAnimNo(PlayerAnimType::Run, true, anim_change_time);
		}
		else if (movingSpeed <= playerInfo_.walkSpeed)
		{
			//アニメーションの変更
			ChangeAnimNo(PlayerAnimType::Walk, true, anim_change_time);
		}
		
	}

	if (VSize(status_.moveVec) == 0.0f) 
	{
		status_.situation.isMoving = false;
		return;
	}

	//移動ベクトルを用意する
	status_.moveVec = VScale(VNorm(status_.moveVec), movingSpeed);
}

//移動処理
float Player::Move()
{
	//短縮化
	auto& input = InputState::GetInstance();

	//キーの押下をブール型に格納
	bool pressedUp = input.IsPressed(InputType::Up);
	bool pressedDown = input.IsPressed(InputType::Down);
	bool pressedLeft = input.IsPressed(InputType::Left);
	bool pressedRight = input.IsPressed(InputType::Right);
	bool pressedShift = input.IsPressed(InputType::Dush);

	status_.moveVec = VGet(0,0,0);

	status_.situation.isMoving = false;
	float movingSpeed = 0.0f;

	if (pressedUp || pressedDown || pressedLeft || pressedRight) 
	{
		movingSpeed = PlayerSpeed(pressedShift);
		status_.situation.isMoving = true;
	}

	//HACK：汚い、リファクタリング必須
	if (pressedUp) 
	{
		status_.moveVec.z += movingSpeed;
		targetAngle_ = 180.0f;
	}
	if (pressedDown) 
	{
		status_.moveVec.z -= movingSpeed;
		targetAngle_ = 0.0f;
	}
	if (pressedLeft) 
	{
		status_.moveVec.x -= movingSpeed;
		targetAngle_ = 90.0f;
	}
	if (pressedRight) 
	{
		status_.moveVec.x += movingSpeed;
		targetAngle_ = 270.0f;
	}
	if (pressedUp && pressedRight)
	{
		targetAngle_ = 225.0f;
	}
	if (pressedUp && pressedLeft) 
	{
		targetAngle_ = 135.0f;
	}
	if (pressedDown && pressedLeft) 
	{
		targetAngle_ = 45.0f;
	}
	if (pressedDown && pressedRight) 
	{
		targetAngle_ = 315.0f;
	}

	//回転処理
	RotationUpdate();

	VECTOR destination = VAdd(status_.pos, status_.moveVec);
	if (destination.z < movement_restrictions_Z) 
	{
		status_.moveVec.z = 0.0f;
	}

	return movingSpeed;
}

//回転の処理
void Player::RotationUpdate()
{
	//一回転の角度
	float oneRevolution = 360.0f;

	//目標の角度から現在の角度を引いて差を出している
	differenceAngle_ = targetAngle_ - angle_;

	//常にプレイヤーモデルを大周りさせたくないので
	//181度又は-181度以上の場合、逆回りにしてあげる
	RotateAtACloseAngle(differenceAngle_, targetAngle_, angle_);

	//滑らかに回転させるため
	//現在の角度に回転スピードを増減させている
	if (differenceAngle_ < 0.0f) 
	{
		status_.rot.y -= playerInfo_.rotSpeed;
		angle_ -= playerInfo_.rotSpeed;
	}
	else if (differenceAngle_ > 0.0f) 
	{
		status_.rot.y += playerInfo_.rotSpeed;
		angle_ += playerInfo_.rotSpeed;
	}

	//360度、一周したら0度に戻すようにしている
	if (angle_ == oneRevolution || angle_ == -oneRevolution)
	{
		angle_ = 0.0f;
	}
	if (status_.rot.y == oneRevolution || status_.rot.y == -oneRevolution)
	{
		status_.rot.y = 0.0f;
	}

	//結果をモデルの回転情報として送る
	model_->SetRot(MathUtil::VECTORDegreeToRadian(status_.rot));
}

//ジャンプ
void Player::JumpUpdate()
{
	//プレイヤー移動関数
	Move();

	//ジャンプ処理
	//ジャンプベクトルが0でジャンプ中ではなかったら
	//idle状態のアップデートに変更する、アニメーションも変更する
	if (!status_.jump.isJump) 
	{
		updateFunc_ = &Player::NormalUpdate;
		return;
	}

	status_.jump.jumpVec += gravity;
	status_.pos.y += status_.jump.jumpVec;
	model_->SetPos(status_.pos);
}

// プレイヤーの死体に与える情報を作る関数
void Player::DeathUpdate()
{
	//アニメーションの変更
	ChangeAnimNo(PlayerAnimType::Death, false, anim_change_time);

	if (model_->IsAnimEnd()) 
	{
		CorpsePostProsessing();
	}
}

//死体の後処理
void Player::CorpsePostProsessing()
{
	//死体を生成する関数
	CorpseInfoGenerater();			

	updateFunc_ = &Player::NormalUpdate;
}

// プレイヤーの死体をvector配列で管理する関数
void Player::CorpseInfoGenerater()
{
	//配置データの作成
	corpseInfo_ = {};

	//モデルの中から指定のフレームの番号を取得する
	int frameNo = MV1SearchFrame(model_->GetModelHandle(), "PlaceToPutTheCorpse");

	//指定のフレームの座標を取得する
	VECTOR putPos = MV1GetFramePosition(model_->GetModelHandle(), frameNo);

	//取得した座標を死体のポジションとする
	corpseInfo_.pos = putPos;

	//プレイヤーの回転値を死体の回転値とする
	corpseInfo_.rot = MathUtil::VECTORDegreeToRadian(status_.rot);

	//拡縮率を同じにする
	corpseInfo_.scale = scale_;

	//死体を生成できるフラグをtrueにする
	isCorpseGeneratable_ = true;
}

//荷物を運ぶ
void Player::CarryObject()
{
	if (!status_.situation.isCanBeCarried) return;

	status_.situation.isInTransit = true;

	corpseModelPointer_->SetIsTransit(true);

	carryUpdateFunc_ = &Player::DropOffObject;
}

//荷物をおろす
void Player::DropOffObject()
{
	if (status_.situation.isCanBeCarried)
	{
		status_.situation.isCanBeCarried = false;
		corpseModelPointer_->SetIsTransit(false);
		int frameNo = MV1SearchFrame(model_->GetModelHandle(), "PlaceToPutTheCorpse");
		VECTOR putPos = MV1GetFramePosition(model_->GetModelHandle(), frameNo);
		corpseModelPointer_->GetModelPointer()->SetPos(putPos);
		corpseModelPointer_.reset();
	}

	status_.situation.isInTransit = false;
	status_.situation.isCanBeCarried = false;

	carryUpdateFunc_ = &Player::CarryObject;
}

//クランクを回すためにクランクを回すポジションへと移動する
void Player::GoCrankRotationPosition()
{
	//クランクの立ってほしいポジションを取得する
	VECTOR standPos = crank_->GetStandingPosition();

	//立ってほしいポジションとプレイヤーの距離のサイズを取得する
	float distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(status_.pos, standPos);

	//distanceSizeが一定の範囲外だったら
	//一定の速度で立ってほしいポジションに向かう
	if (distanceSize > permissible_range)
	{
		VECTOR distance = VNorm(VSub(standPos, status_.pos));
		VECTOR moveVec = VScale(distance, playerInfo_.walkSpeed);
		status_.pos = VAdd(status_.pos, moveVec);
		model_->SetPos(status_.pos);
	}
	//distanceSizeが一定の範囲内に入ったら
	//立ってほしいポジションをプレイヤーのポジションとする
	else 
	{
		status_.pos = standPos;
		model_->SetPos(status_.pos);
		angle_ = -degree_of_90;
		status_.rot = VGet(0, angle_, 0);
		model_->SetRot(MathUtil::VECTORDegreeToRadian(status_.rot));
		ChangeAnimNo(PlayerAnimType::Crank, false, anim_change_time);
		updateFunc_ = &Player::CrankUpdate;
	}
}

//クランクを回転させるアップデート
void Player::CrankRotationUpdate(float rotZ)
{
	float radian = MathUtil::DegreeToRadian(rotZ);

	int frameNo = MV1SearchFrame(crank_->GetModelPointer()->GetModelHandle(), "Crank");

	VECTOR pos = MV1GetFramePosition(crank_->GetModelPointer()->GetModelHandle(), frameNo);

	VECTOR distance = crank_pos;

	MATRIX mat = {};

	float x = MathUtil::DegreeToRadian(degree_of_90);

	//平行移動行列
	MATRIX posMat = MGetTranslate(distance);
	//回転行列
	MATRIX rotMatZ = MGetRotZ(-radian);
	MATRIX rotMatX = MGetRotX(x);

	mat = MMult(rotMatX, rotMatZ);
	mat = MMult(mat, posMat);

	MV1SetFrameUserLocalMatrix(crank_->GetModelPointer()->GetModelHandle(), frameNo, mat);
	
	crank_->SetRotZ(rotZ);
}

//クランクの更新
void Player::CrankUpdate()
{
	//短縮化
	auto& input = InputState::GetInstance();
	auto& sound = SoundManager::GetInstance();

	//移動ベクトルを0にする
	status_.moveVec = VGet(0, 0, 0);

	//クランクの回転を取得する(実際に値を変えるよう)
	float rotZ = crank_->GetRotZ();

	//クランクを回す処理を止める
	if (input.IsTriggered(InputType::Activate))
	{
		crank_.reset();
		status_.situation.isGimmickCanBeOperated = false;
		updateFunc_ = &Player::NormalUpdate;
		return;
	}

	//1フレーム前の回転状態
	RotationState oldRotState = currentRotState_;

	//どんな回転状態か取得する
	currentRotState_ = WhatRotationState();

	//目標の角度に回転することが出来るか
	if (!CanRotation(rotZ))
	{
		currentRotState_ = oldRotState;
	}

	//クランクが目指す角度
	crankTargetAngle_ = rotData_[static_cast<int>(currentRotState_)].targetAngle_;

	//現在の角度から目標の角度の差
	float differenceAngle = crankTargetAngle_ - rotZ;
	
	//現在の角度から近い角度に回る
	RotateAtACloseAngle(differenceAngle, crankTargetAngle_, rotZ);

	//目標の角度と現在の角度が同じで無ければ以降の処理を行う
	if (differenceAngle != 0.0f)
	{
		//-の記号の付かない差の角度を取得する
		float unsignedAngle = (std::max)(differenceAngle, -differenceAngle);

		//回転のスピードを取得する
		float angleSpeed = differenceAngle / unsignedAngle;

		//実際に現在の角度にスピードを足す
		rotZ = rotZ + angleSpeed * crank_rot_speed_rate;

		//クランクを回転させるアップデート
		CrankRotationUpdate(rotZ);
	}
	
	//角度に対するアニメーションフレーム数の取得
	int naturalNumber = static_cast<int>((std::max)(rotZ, -rotZ));
	float animTime = static_cast<float>(naturalNumber % static_cast<int>(crank_->GetMaxRotZ())) / crank_rot_speed_rate;

	//アニメーションフレームの設定
	model_->SetAnimationFrame(animTime);
}

//レバーを倒すポジションへ行く
void Player::GoLeverPullPosition()
{
	//クランクの立ってほしいポジションを取得する
	VECTOR standPos = lever_->GetStandingPosition();
	//立ってほしいポジションとプレイヤーの距離のサイズを取得する
	float distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(status_.pos, standPos);

	//distanceSizeが一定の範囲外だったら
	//一定の速度で立ってほしいポジションに向かう
	if (distanceSize > permissible_range)
	{
		VECTOR distance = VNorm(VSub(standPos, status_.pos));
		VECTOR moveVec = VScale(distance, playerInfo_.walkSpeed);
		status_.pos = VAdd(status_.pos, moveVec);
		model_->SetPos(status_.pos);
	}
	//distanceSizeが一定の範囲内に入ったら
	//立ってほしいポジションをプレイヤーのポジションとする
	else
	{
		status_.pos = standPos;
		model_->SetPos(status_.pos);
		angle_ = 0.0f;
		lever_->OnAnimation();
		status_.rot = VGet(0, angle_, 0);
		model_->SetRot(MathUtil::VECTORDegreeToRadian(status_.rot));
		ChangeAnimNo(PlayerAnimType::LeverOn, false, anim_change_time);
		updateFunc_ = &Player::LeverUpdate;
	}
}

//レバーの更新
void Player::LeverUpdate()
{
	if (!lever_->IsOn())
	{
		updateFunc_ = &Player::NormalUpdate;
	}
}

//投擲物との衝突アップデート
void Player::BulletHitMeUpdate()
{
	//重力
	status_.jump.jumpVec += gravity;
	status_.moveVec.y = status_.jump.jumpVec;

	//ノックバック
	status_.moveVec = VScale(status_.moveVec, knockback_rate);

	//移動ベクトルを足した行き先を取得する
	VECTOR destinationPos = VAdd(status_.pos, status_.moveVec);

	//移動ベクトルを足した行き先が-250以下だったら
	//移動ベクトルのZ値を0にする
	if (destinationPos.z < movement_restrictions_Z)
	{
		status_.moveVec.z = 0.0f;
	}

	//プレイヤーのポジションに移動ベクトルを足す
	status_.pos = VAdd(status_.pos, status_.moveVec);

	//モデルにポジションを設定する
	model_->SetPos(status_.pos);

	float moveVecSize = VSize(status_.moveVec);

	if (moveVecSize < knockback_vector_truncation_range)
	{
		updateFunc_ = &Player::NormalUpdate;
	}
}

//アニメーションを待機に変更する
void Player::ChangeAnimIdle()
{
	//待機アニメーションに戻す
	if (!status_.situation.isMoving)
	{
		ChangeAnimNo(PlayerAnimType::Idle, true, anim_change_time);
	}
}

//アニメーションの変更を行う
void Player::ChangeAnimNo(PlayerAnimType type, bool isAnimLoop, int changeTime)
{
	status_.animNo = static_cast<int>(type);
	status_.isAnimLoop = isAnimLoop;
	model_->ChangeAnimation(status_.animNo, status_.isAnimLoop, false, changeTime);
}

//プレイヤーの速度設定
float Player::PlayerSpeed(bool pressedShift)
{
	//シフトが押されているかどうか
	if (pressedShift)
	{
#ifdef _DEBUG
		if (debugCreativeMode_) {
			return playerInfo_.runningSpeed * 3;
		}
		return playerInfo_.runningSpeed;
#else
		return playerInfo_.runningSpeed;
#endif // _DEBUG
	} 
	
	return playerInfo_.walkSpeed;
}

//足音の再生
void Player::FootStepSound()
{
	//音が鳴っているか
	bool playSound = false;

	//プレイヤーのアニメーションによって
	//アニメーションフレームの特定フレームで
	//音が鳴っているかのフラグをtrueにする
	switch (static_cast<PlayerAnimType>(status_.animNo))
	{
	case PlayerAnimType::Walk:

		if (model_->GetSpecifiedAnimTime(walk_anim_foot_step_frame_type_1) || model_->GetSpecifiedAnimTime(walk_anim_foot_step_frame_type_2)) {
			playSound = true;
		}

		break;
	case PlayerAnimType::Run:

		if (model_->GetSpecifiedAnimTime(run_anim_foot_step_frame_type_1) || model_->GetSpecifiedAnimTime(run_anim_foot_step_frame_type_2)) {
			playSound = true;
		}

		break;
	}

	//音が鳴っていない場合リターン
	if (!playSound)
	{
		return;
	}

	//プレイヤーの足元のオブジェクトの素材によって
	//鳴らす音を変える
	switch (materialSteppedOn_)
	{
	case Material::Iron:
		SoundManager::GetInstance().Set3DSoundInfo(status_.pos, sound_radius, "ironStep");
		SoundManager::GetInstance().PlaySE("ironStep");
		break;
	case Material::Stone:
		SoundManager::GetInstance().Set3DSoundInfo(status_.pos, sound_radius, "asphaltStep");
		SoundManager::GetInstance().PlaySE("asphaltStep");
		break;
	}
}

//落ち影を作成、描画
void Player::DrawPolygon3D()
{
	//頂点の数分配列を作る
	VERTEX3D vertex[shadow_vertex_num] = {};
	//三角形を作成する順番を保存する配列
	WORD index[number_of_orders_to_form_a_triangle] = {};

	//カラー
	COLOR_U8 difColor = GetColorU8(51, 51, 51, 125);
	COLOR_U8 spcColor = GetColorU8(0, 0, 0, 0);

	//法線ベクトル
	VECTOR norm = VGet(0.0f, 1.0f, 0.0f);

	//角度に寄って頂点の位置を変更する
	float angle = 0.0f;

	//六角形の中心座標を取得
	vertex[0].pos = VGet(status_.pos.x, roundShadowHeight_, status_.pos.z);
	vertex[0].norm = norm;
	vertex[0].dif = difColor;
	vertex[0].spc = GetColorU8(0, 0, 0, 0);
	vertex[0].u = 0.0f;
	vertex[0].v = 0.0f;
	vertex[0].su = 0.0f;
	vertex[0].sv = 0.0f;

	//角度ごとの頂点を取得
	for (int i = 1; i < shadow_vertex_num; i++)
	{
		vertex[i].pos = VertexPosition(angle);
		vertex[i].norm = norm;
		vertex[i].dif = difColor;
		vertex[i].spc = spcColor;
		vertex[i].u = 0.0f;
		vertex[i].v = 0.0f;
		vertex[i].su = 0.0f;
		vertex[i].sv = 0.0f;
		angle += 360.0f / shadow_vertex_num;
	}

	//三角形を作成する順番
	index[0] = 0;
	index[1] = 1;
	index[2] = 2;
	index[3] = 0;
	index[4] = 2;
	index[5] = 3;
	index[6] = 0;
	index[7] = 3;
	index[8] = 4;
	index[9] = 0;
	index[10] = 4;
	index[11] = 5;
	index[12] = 0;
	index[13] = 5;
	index[14] = 6;
	index[15] = 0;
	index[16] = 6;
	index[17] = 1;

	DrawPolygonIndexed3D(vertex, shadow_vertex_num, index, shadow_triangle_num, DX_NONE_GRAPH, true);
}

//プレイヤーの落ち影に使用する頂点を取得
VECTOR Player::VertexPosition(float angle)
{

	VECTOR pos = {};

	//度数法を弧度法に変換する
	float radian = MathUtil::DegreeToRadian(angle);
	
	//角度による座標を取得する
	pos.x = sin(radian);
	pos.z = cos(radian);
	pos.y = 0.0f;

	//ポジションを25倍する(サイズ調整)
	pos = VScale(pos, shadow_radius);

	//プレイヤーのポジションと上記で取得したポジションを足す
	pos = VAdd(status_.pos, pos);

	//Y座標は線とポリゴンの当たり判定で取得した
	//一番近いポリゴンのY座標を代入する
	pos.y = roundShadowHeight_;

	return pos;
}
