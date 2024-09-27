#include "CheckCollisionModel.h"

#include "../object/Player.h"
#include "../object/ObjectBase.h"
#include "../object/ObjectManager.h"

#include "Model.h"
#include "Util.h"
#include "SoundManager.h"

#include <algorithm>

namespace {
	//半分
	constexpr int half = 2;

	//三角形の頂点の数
	constexpr int triangle_vertex_num = 3;

	//球の半径
	constexpr float collition_radius = 200.0f;

	//カプセルの半径
	constexpr float capsel_radius = 20.0f;
}

//コンストラクタ
CheckCollisionModel::CheckCollisionModel()
{
}

//デストラクタ
CheckCollisionModel::~CheckCollisionModel()
{
}

// 自分から一定範囲のオブジェクトとの衝突判定を得る
void CheckCollisionModel::CheckCollisionPersonalArea(const std::shared_ptr<Player>& player, const std::shared_ptr<ObjectManager>& objManager)
{
	//短縮化
	auto playerState = player->GetStatus();

	//更新前のポジションを取得する
	oldPos = playerState.pos;

	//更新後のポジションを取得する
	nowPos_ = VAdd(playerState.pos, playerState.moveVec);

	//モデルと球の当たり判定
	objManager->AddCheckCollModel();
	for (const auto& model : objManager->GetAllCheckCollModel()) {
		if (player->GetStatus().situation.isInTransit) {
			if (player->GetDeadPersonModelPointer()->GetModelPointer() == model) {
				continue;
			}
		}

		MV1RefreshCollInfo(model->GetModelHandle(), model->GetColFrameIndex());
		CollModelState state;
		state.hitDim = MV1CollCheck_Sphere(model->GetModelHandle(), model->GetColFrameIndex(), oldPos, collition_radius + VSize(playerState.moveVec));
		state.model = model;
		hitDim_.push_back(state);
	}
	

	//ベクトルの絶対値を取得し、動いたか動いていないかのフラグをとる
	if (fabs(playerState.moveVec.x) > 0.01f || fabs(playerState.moveVec.z) > 0.01f) {
		moveFlag = true;
	}
	else {
		moveFlag = false;
	}

	CheckWallAndFloor();
}

// 衝突した壁と床のポリゴン数を数える
void CheckCollisionModel::CheckWallAndFloor()
{
	hitWallNum = 0;
	hitFloorNum = 0;

	//前にとったモデルと球の当たり判定処理
	int i = 0;
	for (const auto& result : hitDim_)
	{
		for (i = 0; i < result.hitDim.HitNum; i++) 
		{
			//モデルの法線ベクトル
			if (result.hitDim.Dim[i].Normal.y < 0.000001f && result.hitDim.Dim[i].Normal.y > -0.000001f) 
			{
				//最大衝突数が壁の衝突数を超えていたらリターン
				if (hitWallNum > max_hit_coll)
				{
					return;
				}

				if (result.hitDim.Dim[i].Position[0].y > oldPos.y + 1.0f ||
					result.hitDim.Dim[i].Position[1].y > oldPos.y + 1.0f ||
					result.hitDim.Dim[i].Position[2].y > oldPos.y + 1.0f)
				{
					//壁との衝突情報の取得
					wallHitDim_[hitWallNum].hitDim = &result.hitDim.Dim[i];

					//衝突回数の増加
					hitWallNum++;
				}
			}
			else 
			{
				//床ポリゴン情報の取得？
				if (hitFloorNum > max_hit_coll)
				{
					return;
				}

				//床との衝突情報の取得
				floorHitDim_[hitFloorNum].hitDim = &result.hitDim.Dim[i];

				//衝突回数の増加
				hitFloorNum++;

			}
		}
	}
}

// 衝突したモデルのポリゴンが壁かを判断し、移動ベクトルを補正する
void CheckCollisionModel::CheckCollisionWall(const std::shared_ptr<Player>& player)
{
	//短縮化
	auto& playerState = player->GetStatus();

	//壁の処理
	if (hitWallNum != 0) 
	{
		hitFlag_ = false;
		//動いていたら
		if (moveFlag) 
		{
			int j = 0;
			for (int i = 0; i < hitWallNum; i++)
			{
				auto hitPoly = wallHitDim_[i];
				//プレイヤーを元にしたカプセルと壁ポリゴンの判定　　当たっていなかったらcontinue
				if (!HitCheck_Capsule_Triangle(nowPos_, VAdd(nowPos_, VGet(0.0f, playerState.height, 0.0f)), capsel_radius, hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2]))
				{
					continue;
				}

				hitFlag_ = true;

				if (isGoUpStep_) 
				{
					hitFlag_ = false;
					continue;
				}

				VECTOR slideVec = {};

				//プレイヤーのベクトルとポリゴンの法線ベクトルの外積を取得
				slideVec = VCross(playerState.moveVec, hitPoly.hitDim->Normal);

				//プレイヤーのベクトルとポリゴンの法線ベクトルの外積とポリゴンの外積の法線ベクトルの外積を取得
				slideVec = VCross(hitPoly.hitDim->Normal, slideVec);

				//更新前のプレイヤーのポジションと上記の外積を取得
				nowPos_ = VAdd(oldPos, slideVec);

				//移動後座標で壁との衝突判定
				CollisionDetectionWithWallAfterMovement(j, player);

				//当たっていなかったらフラグを折る
				if (j == hitWallNum)
				{
					hitFlag_ = false;
					break;
				}
			}
		}
	}
	else {
		//一つも壁とのhit情報がなかった場合
		for (int i = 0; i < hitWallNum; i++)
		{
			auto hitPoly = wallHitDim_[i];
			if (HitCheck_Capsule_Triangle(nowPos_, VAdd(nowPos_, VGet(0.0f, playerState.height, 0.0f)), capsel_radius, hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2]))
			{
				hitFlag_ = false;
				break;
			}
		}
	}


	// 壁に当たっていたら壁から押し出す処理を行う
	//当たったポリゴンの法線ベクトルの５倍をプレイヤーのポジションに足している
	if (hitFlag_ && !isGoUpStep_)
	{
		int i, j, k;
		for (k = 0; k < 16; k++)
		{
			for (i = 0; i < hitWallNum; i++)
			{
				auto& hitPoly = wallHitDim_[i];
				if (!HitCheck_Capsule_Triangle(nowPos_, VAdd(nowPos_, VGet(0.0f, playerState.height, 0.0f)), capsel_radius, hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2]))
				{
					continue;
				}

				//ベクトルを足した後のプレイヤーの座標
				nowPos_ = VAdd(nowPos_, VScale(hitPoly.hitDim->Normal, 5.0f));

				//移動後座標で壁との衝突判定
				CollisionDetectionWithWallAfterMovement(j, player);

				if (j == hitWallNum)break;
			}
			if (i != hitWallNum)break;
		}
	}
}

//移動後座標で壁との衝突判定
void CheckCollisionModel::CollisionDetectionWithWallAfterMovement(int& j, const std::shared_ptr<Player>& player)
{
	//短縮化
	auto& playerState = player->GetStatus();

	//また当たり判定？
	for (j = 0; j < hitWallNum; j++)
	{
		auto& hitPoly = wallHitDim_[j];
		if (HitCheck_Capsule_Triangle(nowPos_, VAdd(nowPos_, VGet(0.0f, playerState.height, 0.0f)), capsel_radius, hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2]))
		{
			player->SetMoveVec(VGet(0, 0, 0));
			break;
		}
	}
}

// 衝突したモデルのポリゴンが床かを判断する
void CheckCollisionModel::CheckCollisionFloor(const std::shared_ptr<Player>& player)
{
	//短縮化
	auto playerState = player->GetStatus();

	float jumpVec = playerState.jump.jumpVec;
	bool isJump = playerState.jump.isJump;

	//床との当たり判定
	if (hitFloorNum != 0)
	{
		//ジャンプ中かつベクトルがマイナスでなければ
		if (isJump && jumpVec > 0.0f)
		{
			float minY = 0.0f;
			hitFlag_ = false;
			for (int i = 0; i < hitFloorNum; i++)
			{
				//データのコピー
				auto& hitPoly = floorHitDim_[i];

				//衝突判定結果
				hitLineResult_.push_back(HitCheck_Line_Triangle(nowPos_, VAdd(nowPos_, VGet(0.0f, playerState.height, 0.0f)), hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2]));

				//衝突判定結果で衝突してなかったら以降の処理を行わない
				if (hitLineResult_.back().HitFlag == false)
				{
					continue;
				}

				//衝突判定結果で衝突してなかったら以降の処理を行わない
				if (hitFlag_ == true && minY < hitLineResult_.back().Position.y)
				{
					continue;
				}
				hitFlag_ = true;
				minY = hitLineResult_.back().Position.y;

				hitLineResult_.clear();

			}
			if (hitFlag_) {
				nowPos_.y = minY - playerState.height;
				if (jumpVec != 36.0f) 
				{
					jumpVec = -jumpVec;
				}
			}
		}
		else {
			float maxY = nowPos_.y + playerState.height;
			float correction = capsel_radius;
			hitFlag_ = false;
			for (int i = 0; i < hitFloorNum; i++)
			{
				auto& hitPoly = floorHitDim_[i];
				
				hitLineResult_.push_back(HitCheck_Line_Triangle(VAdd(nowPos_, VGet(0, playerState.height, 0)), nowPos_, hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2]));
				hitLineResult_.push_back(HitCheck_Line_Triangle(VAdd(nowPos_, VGet(correction, playerState.height,  0))	, VAdd(nowPos_, VGet(correction, 0, 0))	, hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2]));
				hitLineResult_.push_back(HitCheck_Line_Triangle(VAdd(nowPos_, VGet(-correction, playerState.height, 0))	, VAdd(nowPos_, VGet(-correction, 0, 0)), hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2]));
				hitLineResult_.push_back(HitCheck_Line_Triangle(VAdd(nowPos_, VGet(0, playerState.height,  correction))	, VAdd(nowPos_, VGet(0, 0, correction))	, hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2]));
				hitLineResult_.push_back(HitCheck_Line_Triangle(VAdd(nowPos_, VGet(0, playerState.height, -correction))	, VAdd(nowPos_, VGet(0, 0, -correction)), hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2]));

				int hitCount = 0;
				HITRESULT_LINE hitLine = {};

				for (auto& result : hitLineResult_) 
				{
					if (result.HitFlag == 1) 
					{
						hitCount++;
						hitLine = result;
					}
				}
				
				if (hitCount < 1) 
				{
					hitLineResult_.clear();
					continue;
				}

				hitFlag_ = true;
				maxY = hitLine.Position.y;
				hitLineResult_.clear();
			}
			if (hitFlag_)
			{
				nowPos_.y = maxY;
				jumpVec = 0.0f;
				if (isJump) {
  					isJump = false;
				}
			}
			else
			{
 				isJump = true;
			}
		}
	}

	player->SetJumpInfo(isJump, jumpVec);

}

// checkCollisionPersonalArea、checkCollisionWall、checkCollisionFloorを呼び出す。
// 最後、上記の関数で取得した値を消去する
void CheckCollisionModel::CheckCollision(const std::shared_ptr<Player>& player, const std::shared_ptr<ObjectManager>& objManager)
{

	//プレイヤーから一定範囲の衝突判定をとる
	CheckCollisionPersonalArea(player, objManager);

	CheckCollCorpseModel(player, objManager);
	//衝突したオブジェクトが乗り越えることが出来るオブジェクトか判断する
	CheckStepDifference(player);
	//取得した衝突結果から壁に当たった場合の処理
	CheckCollisionWall(player);
	//取得した衝突結果から床に当たった場合の処理
	CheckCollisionFloor(player);

	FindThePolygonBelowThePlayer(player, objManager);

	//ポジションのセット
	player->SetPos(nowPos_);

	//衝突判定の消去
	for (auto& hit : hitDim_)
	{
		MV1CollResultPolyDimTerminate(hit.hitDim);
	}

	hitDim_.clear();
}

// 衝突したオブジェクトが乗り越えられるか判断する
void CheckCollisionModel::CheckStepDifference(const std::shared_ptr<Player>& player)
{
	//短縮化
	auto playerState = player->GetStatus();

	objectHeightY_ = 0;
	isGoUpStep_ = false;

	//乗り越えられる段差か判断するため
	bool overHeight = false;

	//プレイヤーを基準にしたカプセルと当たったポリゴンの当たり判定を行い
	//プレイヤーの高さよりもポリゴンの高さが低かったら段差を上る処理を作る
	if (hitWallNum != 0)
	{
		for (int i = 0; i < hitWallNum; i++)
		{
			auto hitPoly = wallHitDim_[i];
			if (!HitCheck_Capsule_Triangle(VGet(nowPos_.x, nowPos_.y, nowPos_.z), VAdd(nowPos_, VGet(0.0f, playerState.height, 0.0f)), capsel_radius, hitPoly.hitDim->Position[0], hitPoly.hitDim->Position[1], hitPoly.hitDim->Position[2]))
			{
				continue;
			}

			//衝突したポリゴンの一番Y軸の高い頂点を見つけ出し
			//乗り越えられる段差以上だったらoverHeightをtrueにする
			for (int i = 0; i < triangle_vertex_num; i++)
			{
				if (nowPos_.y + playerState.height / half < hitPoly.hitDim->Position[i].y)
				{
					overHeight = true;
				}
			}

			//乗り越えることができる高さで一番高いY軸の値を見つける
			if (!overHeight) {
				for (int i = 0; i < triangle_vertex_num; i++)
				{
					if (objectHeightY_ < hitPoly.hitDim->Position[i].y)
					{
						objectHeightY_ = hitPoly.hitDim->Position[i].y;
						isGoUpStep_ = true;
					}
				}
			}
		}
	}

	//代入
	if (objectHeightY_ > 0.0f)
	{
		nowPos_.y = objectHeightY_;
	}
}

// プレイヤーの下に影もどきを描画したいために
// プレイヤーの真下の一番近いポリゴンの高さを取得する
void CheckCollisionModel::FindThePolygonBelowThePlayer(const std::shared_ptr<Player>& player, const std::shared_ptr<ObjectManager>& objManager)
{
	std::unordered_map<Material, std::list<MV1_COLL_RESULT_POLY>> hitLine;

	//オブジェクトのポリゴンを取得する
	for (const auto& model : objManager->GetAllCheckCollModel())
	{
		//プレイヤーが何かを持ち運んでいる場合
		//持ち運んでいるオブジェクトのポリゴンを取得しないようにしている
		if (player->GetStatus().situation.isInTransit)
		{
			if (player->GetDeadPersonModelPointer()->GetModelPointer() == model)
			{
				continue;
			}
		}

		//オブジェクトとプレイヤーの高さの差を取得する
		float distance = nowPos_.y - model->GetPos().y;
		distance = (std::max)(distance, -distance);

		//モデルと線の当たり判定を取る
		MV1RefreshCollInfo(model->GetModelHandle(), model->GetColFrameIndex());
		VECTOR playerHeadPos = VGet(nowPos_.x, nowPos_.y + player->GetStatus().height, nowPos_.z);
		VECTOR lowPos = VGet(nowPos_.x, nowPos_.y - distance, nowPos_.z);
		hitLine[model->GetMaterialType()].push_back(MV1CollCheck_Line(model->GetModelHandle(), model->GetColFrameIndex(), playerHeadPos, lowPos));
		if (hitLine[model->GetMaterialType()].back().HitFlag == 0) 
		{
			hitLine[model->GetMaterialType()].pop_back();
		}
		else {
			int a = 0;
		}
	}

	float nearPosY = 5000.0f;
	float resultY = 0.0f;
	float distanceY = 0.0f;
	Material materialType = Material::max;

	//当たり判定の結果から一番近いポリゴンのY座標を取得する
	for (const auto& list : hitLine)
	{
		for (const auto& result : list.second)
		{
			
			if (result.HitFlag == 0) 
			{
				continue;
			}

			distanceY = nowPos_.y - result.HitPosition.y;
			if (nearPosY > distanceY)
			{
				nearPosY = distanceY;
				resultY = result.HitPosition.y;
				materialType = list.first;
			}
		}
	}

	//一番近いY座標を丸影のY座標として使う
	//当たり判定の結果何とも当たっていなかった場合
	//Y座標を0にする
	player->SetRoundShadowHeightAndMaterial(resultY + 1,materialType);

	hitLine.clear();
}

//死体との衝突判定
void CheckCollisionModel::CheckCollCorpseModel(const std::shared_ptr<Player>& player, const std::shared_ptr<ObjectManager>& objManager)
{

	//短縮化
	auto playerState = player->GetStatus();

	//プレイヤーが現状別の死体を持ち運んでいたら取得しない
	if (player->GetStatus().situation.isInTransit)
	{
		return;
	}

	//持ち運ぶ死体を取得する
	for (const auto& obj : objManager->GetSpecificObject(ObjectType::Corpse))
	{
		for (const auto& hit : hitDim_)
		{
			//衝突結果が死体以外だったらcontinue
			if (hit.model != obj->GetModelPointer())
			{
				continue;
			}

			//死体のモデルとプレイヤーの座標を基準に作成されたカプセルとの衝突判定
			auto result = MV1CollCheck_Capsule(hit.model->GetModelHandle(), hit.model->GetColFrameIndex(), playerState.pos, VAdd(playerState.pos, VGet(0, playerState.height, 0)), capsel_radius);

			//上記の衝突判定の結果が1つでもあればプレイヤーに
			//その死体のポインターと持ち運べるフラグを送る
			if (result.HitNum > 0)
			{
				player->SetCarryInfo(true,obj);
			}

			MV1CollResultPolyDimTerminate(result);

		}
	}
}