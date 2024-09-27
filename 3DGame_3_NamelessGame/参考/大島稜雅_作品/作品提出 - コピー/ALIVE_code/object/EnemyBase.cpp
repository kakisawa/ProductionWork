#include "EnemyBase.h"

#include "Player.h"
#include "ShotManager.h"
#include "ObjectManager.h"

#include "../util/Util.h"
#include "../util/Model.h"
#include "../util/Aster.h"

namespace {
	//色を変更するマテリアルの番号
	constexpr int change_material_color_num = 8;

	//物を投げているときのアニメーションフレーム
	constexpr int throw_frame_time = 73;

	//アニメーションを変更するのにかける時間
	constexpr int change_anim_frame = 10;

	//落ち影の調点数
	constexpr int shadow_vertex_num = 7;

	//落ち影で作る三角形の数
	constexpr int shadow_triangle_num = 6;

	//三角形を作る順番の数
	constexpr int number_of_orders_to_form_a_triangle = 18;

	//直線を分割する距離
	constexpr float distance_to_divide = 50.0f;

	//落ち影の半径
	constexpr float shadow_radius = 25.0f;

	//落ち影の高さ補正
	constexpr float correction_chadow_height = 5.0f;

	//敵の視野角
	constexpr float viewing_angle = 45.0f;

	//敵がプレイヤーを視認できる範囲
	constexpr float visible_range = 1300.0f;

	//敵のスピード
	constexpr float move_speed = 3.0f;

	//敵モデルの高さ
	constexpr float model_height = 150.0f;

	//リーチ
	constexpr float within_reach = 80.0f;

	//石を投げる距離
	constexpr float throw_distance = 800.0f;

	//範囲内に入ったらプレイヤーの方向を向く距離
	constexpr float peasonal_space_range = 200.0f;

	//右手のフレーム名
	const char* const hand_framename = "mixamorig:RightHandIndex2";

	//モデルの初期回転ベクトル
	constexpr VECTOR model_front_vec = { 0.0f,0.0f,-1.0f };
}

//コンストラクタ
EnemyBase::EnemyBase(const int handle, const Material materialType, const ObjectInfo& objInfo) : CharacterBase(handle, materialType, objInfo)
{
	//インスタンス化（使っていない）
	Aster_ = std::make_shared<Aster>();

	//Asterの初期化
	Aster_->ArrayInit(objInfo.pos);

	//アニメーションの設定
	model_->SetAnimation(static_cast<int>(PlayerAnimType::Idle), true, false);

	//正面ベクトルの取得
	MATRIX matRotY = MGetRotY(objInfo.rot.y);
	frontVec_ = VTransform(model_front_vec, matRotY);

	//初期正面ベクトルの設定
	initFrontVec_ = frontVec_;

	//マテリアルの色を変える
	MV1SetMaterialDifColor(model_->GetModelHandle(), change_material_color_num, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));
}

//デストラクタ
EnemyBase::~EnemyBase()
{
}

//更新
void EnemyBase::Update(Player& player)
{
	//モデルの更新
	model_->Update();

	//Asterの更新
	Aster_->Update();

	if (isThrow_)
	{
		return;
	}

	//プレイヤーの座標
	VECTOR playerPos = player.GetStatus().pos;

	//プレイヤーを索敵する
	SearchForPlayer(playerPos);

	//プレイヤーを検知していなかったら
	//待機アニメーションに変更する
	if (!isDetection_)
	{
		model_->ChangeAnimation(static_cast<int>(PlayerAnimType::Idle), true, false, change_anim_frame);
	}

	//投げるアニメーションが終わったら
	//投げているというフラグをfalseにする
	if (model_->IsAnimEnd() && isThrow_)
	{
		model_->ChangeAnimation(static_cast<int>(PlayerAnimType::Idle), true, false, change_anim_frame);
		isThrow_ = false;
	}

	//索敵
   	if (IsThereAnObject(playerPos))
	{
		if (isDetection_) {
			//プレイヤーを追跡する
			TrackingUpdate(playerPos);
		}
	}
	else
	{
		if (Aster_->LocationInformation(playerPos, pos_)) {
			Aster_->RouteSearch();
		}
	}

	if (!Aster_->GetIsRouteEmpty()) {
		//現在のインデックス情報を更新する
		Aster_->CurrentIndexUpdate(playerPos, pos_);

		//移動
		RoutingUpdate(playerPos);
	}
}

//描画
void EnemyBase::Draw()
{
	model_->Draw();
#ifdef _DEBUG
//	Aster_->DebugDraw();
#endif
}

// プレイヤーを追跡する
void EnemyBase::TrackingUpdate(const VECTOR& playerPos)
{
	//プレイヤーと自分の差を算出する
	VECTOR distancePlayerAndEnemy = VSub(playerPos, pos_);

	//Y座標の追跡は考慮しない
	distancePlayerAndEnemy.y = 0;

	//移動ベクトルの生成
	VECTOR moveVec = VScale(VNorm(distancePlayerAndEnemy), move_speed);

	//回転行列と拡縮行列の合成行列
	MATRIX mtx = CombiningRotAndScallMat(distancePlayerAndEnemy);

	//ポジションの移動
	pos_ = VAdd(pos_, moveVec);

	//回転行列と拡縮行列を掛けた行列に平行移動行列を書ける
	mtx = MMult(mtx, MGetTranslate(pos_));

	//行列をモデルにセットする
	MV1SetMatrix(model_->GetModelHandle(), mtx);
}

//プレイヤーを索敵する
void EnemyBase::SearchForPlayer(const VECTOR& playerPos)
{
	//敵からプレイヤーの直線距離
	float distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(playerPos, pos_);
	float innerProduct = 0.0f;

	//内積を取得する(返り値はコサイン)
	innerProduct = VDot(frontVec_, VNorm(VSub(playerPos, pos_)));

	//上記の結果を度数法に変える
	float radian = acos(innerProduct);
	innerProduct = MathUtil::RadianToDegree(radian);

	//視野の範囲内かつ距離がプレイヤーを視認できる距離よりも
	//短かったらプレイヤーを検知したことにする
	if (innerProduct < viewing_angle && distanceSize < visible_range)
	{
		isDetection_ = true;
	}
	else {
		isDetection_ = false;
	}

	//一定距離内になったらプレイヤーを検知したことにする
	if (distanceSize < peasonal_space_range)
	{
		isDetection_ = true;
	}
}

//ルート通りに移動する
void EnemyBase::RoutingUpdate(const VECTOR& playerPos)
{
	//エネミーが次に目指す升の中心座標
	VECTOR targetPos = Aster_->GetDestinationCoordinates(playerPos);

	//目標地点とポジションの距離を取得
	VECTOR distance = VSub(targetPos, pos_);

	//エネミーと目指す升の中心座標との距離のベクトルサイズ
	float size = VSize(distance);

	//回転行列と拡縮行列の合成行列
	MATRIX mtx = CombiningRotAndScallMat(distance);

	//回転行列と拡縮行列を掛けた行列に平行移動行列を書ける
	mtx = MMult(mtx, MGetTranslate(pos_));

	//行列をモデルにセットする
	MV1SetMatrix(model_->GetModelHandle(), mtx);

	if (size > 1.0f)
	{
		//正規化
		VECTOR norm = VNorm(distance);
		//移動ベクトルを作成
		VECTOR moveVec = VScale(norm, move_speed);
		//移動
		pos_ = VAdd(pos_, moveVec);
	}
}

//敵からプレイヤーの直線距離にオブジェクトがあるか
bool EnemyBase::IsThereAnObject(const VECTOR& playerPos)
{
	//エネミーとプレイヤーの距離
	VECTOR distance = VSub(playerPos, pos_);

	//エネミーとプレイヤーの距離のベクトルサイズ
	int size = static_cast<int>(VSize(distance) / distance_to_divide);

	//エネミーとプレイヤーの距離の正規化
	VECTOR norm = VNorm(distance);
	
	//敵からプレイヤーの直線状にオブジェクトがあるか
	bool noObject = true;

	//敵からプレイヤーの距離を50で割っていき
	//敵からプレイヤーの間にオブジェクトがあるか判断する
	for (int i = 1; i < size; i++)
	{
		//敵からプレイヤーの距離を50×iで分割する
		VECTOR pointPos = VScale(norm, distance_to_divide * i);

		//敵の座標とpoint座標を足す
		pointPos = VAdd(pos_, pointPos);

		//オブジェクトがあるか取得する
		noObject = !Aster_->SearchBlockadeMode(pointPos);

		//オブジェクトがあったらブレイクする
		if (!noObject)
		{

			//経路探索の結果がなかったら
			//経路探索用の配列を初期化する
			if (Aster_->GetIsRouteEmpty()) 
			{
				Aster_->ArrayInit(playerPos);
			}

			break;
		}
	}

	//結果を返す
	return noObject;
}

//弾を発射する
void EnemyBase::Shot(const std::shared_ptr<ShotManager>& shotManager, const VECTOR& playerPos, const  float height)
{
	//プレイヤーを検知しているかどうか
	//検知していなかったらreturn
	if (!isDetection_)
	{
		return;
	}

	//今、投げているフラグがtrueではなかったら
	//アニメーションを投げるアニメーションに変更する
	model_->ChangeAnimation(static_cast<int>(PlayerAnimType::Throw), false, false, change_anim_frame);

	//投げているアニメーションの特定フレームで
	//弾を発射する
	//投げているフラグをtrueにする
	if (model_->GetSpecifiedAnimTime(throw_frame_time))
	{
		VECTOR framePos = model_->GetFrameLocalPosition(hand_framename);
		shotManager->Fire(framePos, playerPos, height);
		isThrow_ = true;
	}
}

//回転行列と拡縮行列を乗算した行列を取得する
MATRIX EnemyBase::CombiningRotAndScallMat(const VECTOR& distance)
{
	//回転行列の取得
	MATRIX rotMtx = MGetRotVec2(model_front_vec, distance);

	//正規化した正面ベクトルを取得する
	frontVec_ = VTransform(model_front_vec, rotMtx);

	//拡縮行列の取得
	MATRIX scaleMtx = MGetScale(scale_);

	//回転行列と拡縮行列の掛け算
	MATRIX mtx = MMult(rotMtx, scaleMtx);

	return mtx;
}

//プレイヤーの落ち影に使用する頂点を取得
VECTOR EnemyBase::VertexPosition(const float angle)
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
	pos = VAdd(pos_, pos);

	//Y座標は線とポリゴンの当たり判定で取得した
	//一番近いポリゴンのY座標を代入する
	pos.y = pos_.y;

	return pos;
}

//落ち影を作成、描画
void EnemyBase::DrawPolygon3D()
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
	vertex[0].pos = VGet(pos_.x, pos_.y + correction_chadow_height, pos_.z);
	vertex[0].norm = norm;
	vertex[0].dif = difColor;
	vertex[0].spc = GetColorU8(0, 0, 0, 0);
	vertex[0].u = 0.0f;
	vertex[0].v = 0.0f;
	vertex[0].su = 0.0f;
	vertex[0].sv = 0.0f;

	//角度ごとの頂点を取得
	for (int i = 1; i < shadow_vertex_num; i++) {
		vertex[i].pos = VertexPosition(angle);
		vertex[i].norm = norm;
		vertex[i].dif = difColor;
		vertex[i].spc = spcColor;
		vertex[i].u = 0.0f;
		vertex[i].v = 0.0f;
		vertex[i].su = 0.0f;
		vertex[i].sv = 0.0f;
		angle += 360.0f / shadow_triangle_num;
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