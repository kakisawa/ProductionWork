#include "Elevator.h"
#include "Switch.h"
#include "Lever.h"

#include "../util/Util.h"
#include "../util/Model.h"
#include "../util/Easing.h"
#include "../util/InputState.h"
#include "../util/ExternalFile.h"
#include "../util/SoundManager.h"

#include "../object/Player.h"

namespace 
{
	//アニメーションが変わるフレーム数
	constexpr int anim_cange_frame = 10;

	//レバーの生成数
	constexpr int lever_generate_num = 3;

	//停止ポジションの数
	constexpr int stop_position_num = 2;

	//到達時間
	constexpr float total_time = 180.0f;

	//サウンドが聞こえる範囲
	constexpr float sound_range = 1500.0f;

	//タイマーを進める時間
	constexpr float add_time = 1.0f;

	//レバーに関するオブジェクトのY座標の補正値
	constexpr float correction_y = 8.0f;

	//エレベーターのカプセルの半径
	constexpr float elevator_capsel_range = 200.0f;

	//プレイヤーのカプセルの半径
	constexpr float player_capsel_range = 20.0f;
}

//コンストラクタ
Elevator::Elevator(const int handle, const Material materialType, const ObjectInfo& objInfo):GimmickBase(handle, materialType, objInfo)
{
	//このエレベーターが何番目のエレベーターかを名前の末尾から取得する
	int elevatorNum = StrUtil::GetNumberFromString(objInfo.name, ".");

	//スイッチの配置データを取得する
	ObjectInfo leverData = GetSpecialNameObjectInfo("MovingLever", "-", elevatorNum, 0);

	//スイッチのインスタンス化
	movingLever_ = std::make_shared<Lever>(leverData);

	//レバーのインスタンス化
	for (int i = 1; i < lever_generate_num; i++)
	{
		ObjectInfo LeverData = GetSpecialNameObjectInfo("ElevatorLever", "-", elevatorNum, i);
		levers_.push_back(std::make_shared<Lever>(LeverData));
	}

	//停止ポジションの取得
	for (int i = 0; i < stop_position_num; i++)
	{
		ObjectInfo pointData = GetSpecialNameObjectInfo("ElevatorPoint", "-", elevatorNum, i);
		stopPos_[static_cast<ElevatorState>(i)] = pointData.pos;
	}

	//ターゲットポジションの初期化
	targetPos_ = pos_;

	//衝突判定用フレームの設定
	model_->SetUseCollision(true, false);

	//アニメーションの設定
	model_->SetAnimation(static_cast<int>(ElevatorAnimType::OpenIdle), true, false);

	//現在のエレベーターのステータスを設定する
	SetInitState();
}

//デストラクタ
Elevator::~Elevator()
{
}

//更新
void Elevator::Update(Player& player)
{
	//短縮化
 	auto& sound = SoundManager::GetInstance();

	//プレイヤーのポジションの取得
	VECTOR playerPos = player.GetStatus().pos;

	//モデルの更新
	model_->Update();

	//レバーの更新
	movingLever_->Update();

	//動くレバーとの衝突判定
	if (movingLever_->CollCheck(playerPos))
	{
		player.SetLeverPointer(movingLever_);
	}

	//レバーの更新
	for (const auto& lever : levers_)
	{
		lever->Update();
	}

	//レバーとプレイヤーの衝突判定を行い
	//衝突していた場合プレイヤーにポインタを設定する
	for (const auto& lever : levers_)
	{
		if (lever->CollCheck(playerPos)) 
		{
			player.SetLeverPointer(lever);
		}
	}

	//出発出来るかを取得する
	isDeparture_ = CanDeparture();

	//ターゲットポジションの変更
	if (targetPos_.y == pos_.y)
	{
		TargetPosition();
	}

	//アニメーションが終了次第、移動を開始する
	if (model_->IsAnimEnd()) 
	{
		//上昇ベクトルの取得
		float upVecY = Move();

		//衝突判定を行う
		int result = HitCheck_Capsule_Capsule(pos_, VGet(pos_.x, pos_.y + scale_.y * 2, pos_.z), elevator_capsel_range, playerPos, VGet(playerPos.x, playerPos.y, playerPos.z), player_capsel_range);

		//衝突していたら
		if (result > 0)
		{
			//プレイヤーの移動ベクトルに上昇ベクトルを足す
			VECTOR playerMoveVec = player.GetStatus().moveVec;
			playerMoveVec.y += upVecY;

			//プレイヤーの移動ベクトルを設定する
			player.SetMoveVec(playerMoveVec);
		}
	}
}

//描画
void Elevator::Draw()
{
	//モデルの描画
	model_->Draw();

	//スイッチの描画
	//switch_->Draw();
	movingLever_->Draw();

	//レバーの描画
	for (auto& lever : levers_) 
	{
		lever->Draw();
	}
}

//音を鳴らす
void Elevator::PlayDoorSound()
{
	//短縮化
	auto& sound = SoundManager::GetInstance();

	//音の設定
	sound.Set3DSoundInfo(pos_, sound_range, "door");

	//音の再生
	sound.PlaySE("door");
}

//出発することが可能か
bool Elevator::CanDeparture()
{
	//レバーのIsOn(引かれる)がtrueになったら
	//出発変数をtrueにする
	if (movingLever_->IsOn())
	{
		return true;
	}

	//レバーのIsOn(引かれる)がtrueになったら
	//出発変数をtrueにする
	for (auto& lever : levers_)
	{
		if (lever->IsOn())
		{
			return true;
		}
	}

	return false;
}

//特殊な名前のオブジェクトの配置データを取得する
ObjectInfo Elevator::GetSpecialNameObjectInfo(const std::string& name, const std::string& sign, int groupNum, int num)
{
	//短縮化
	auto& file = ExternalFile::GetInstance();

	//名前とグループ番号を結合した文字列を取得する
	std::string str = StrUtil::GetConcatenateNumAndStrings(name, ".", groupNum);

	//名前と記号と番号を結合した文字列を取得する
	str = StrUtil::GetConcatenateNumAndStrings(str, "-", num);

	//配置データの取得
	ObjectInfo objInfo = file.GetSpecifiedGimmickInfo(str);

	return objInfo;
}

//現在のエレベーターのステータスを設定する
void Elevator::SetInitState()
{
	//停止ポジションとの距離を取得
	float upperDistanceSize = MathUtil::GetSizeOfDistanceTwoPoints(pos_, stopPos_[ElevatorState::Upper]);
	float lowerDistanceSize = MathUtil::GetSizeOfDistanceTwoPoints(pos_, stopPos_[ElevatorState::Lower]);

	//現在のポジションと停止ポジションとの距離が近い方の状態を保存する
	if (upperDistanceSize > lowerDistanceSize)
	{
		state_ = ElevatorState::Lower;
	}
	else
	{
		state_ = ElevatorState::Upper;
	}
}

//エレベーターを移動させる
float Elevator::Move()
{
	//1フレーム前のY座標のベクトルを取得する
	float oldPosY = pos_.y;
	
	//時間の更新
	elapsedTime_ = (std::min)(elapsedTime_ + add_time, total_time);

	//座標の移動
	pos_.y = Easing::InOutCubic(elapsedTime_, total_time, targetPos_.y, pos_.y);

	//移動終了後アニメーションを変更する
	if (elapsedTime_ == total_time && !isStopping_)
	{
		isStopping_ = true;
		PlayDoorSound();
		model_->ChangeAnimation(static_cast<int>(ElevatorAnimType::Open), false, false, anim_cange_frame);
	}

	if (model_->IsAnimEnd() && model_->GetCurrentAnimNo() == static_cast<int>(ElevatorAnimType::Open))
	{
		model_->ChangeAnimation(static_cast<int>(ElevatorAnimType::OpenIdle), true, false, anim_cange_frame);
	}

	//ポジションの設定
	model_->SetPos(pos_);

	//スイッチのポジションの設定
	movingLever_->GetModelPointer()->SetPos(VGet(pos_.x, pos_.y + correction_y, pos_.z));

	//レバーを引く立ち位置の取得
	VECTOR standingPos = movingLever_->GetStandingPosition();

	//レバーを引く立ち位置の設定
	movingLever_->SetStandingPos(VGet(standingPos.x, pos_.y + correction_y, standingPos.z));

	//過去と現在のポジションの差を取得する
	float upVecY = pos_.y - oldPosY;

	return upVecY;
}

//移動先のポジションを取得する
void Elevator::TargetPosition()
{
	if (!isDeparture_)
	{
		return;
	}

	//サウンドを鳴らす
	PlayDoorSound();

	//経過時間を0にする
	elapsedTime_ = 0;

	//停止しているフラグをfalseにする
	isStopping_ = false;

	//アニメーションを変更
	model_->ChangeAnimation(static_cast<int>(ElevatorAnimType::Close), false, false, anim_cange_frame);

	//elevatorの状態によって
	//elevatorの停止ポジションを変更する
	if (state_ == ElevatorState::Upper)
	{
		targetPos_ = stopPos_[ElevatorState::Lower];
		state_ = ElevatorState::Lower;
	}
	else
	{
		targetPos_ = stopPos_[ElevatorState::Upper];
		state_ = ElevatorState::Upper;
	}

	//出発できるフラグをfalseにする
	isDeparture_ = false;
}