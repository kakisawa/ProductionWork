#include "Aster.h"
#include "../object/ObjectManager.h"
#include "Util.h"
#include <algorithm>

namespace {
	//二次元配列の最大数 X
	constexpr int max_X = 11;

	//二次元配列の最大数 Z
	constexpr int max_Z = 11;

	//二次元配列の最大要素数
	constexpr int max_Index = max_X * max_Z;

	//半分
	constexpr int half = 2;

	//球の分割数
	constexpr int division_num = 32;

	//1マスの縦のサイズ
	constexpr float height_size = 100.0f;

	//1マスの横のサイズ
	constexpr float width_size = 100.0f;

	//球の半径
	constexpr float sphere_radius = 16.0f;
}

//コンストラクタ
Aster::Aster()
{
}

//デストラクタ
Aster::~Aster()
{
}

// 経路探索を行う配列の初期化
void Aster::ArrayInit(const VECTOR& pos)
{
	//縦横の半分のサイズを取得
	int halfX = max_X / half;
	int halfZ = max_Z / half;

	//中心座標
	VECTOR centerPos = {};

	//プレイヤーを中心としたmax_X × max_Zのマスを生成する
	for (int i = 0; i < max_Index; i++)
	{
		//中心座標の取得
		centerPos.x = (i % max_X - halfX) * width_size + pos.x;
		centerPos.y = pos.y;
		centerPos.z = (i / max_Z - halfZ) * height_size + pos.z;

		//マス配列の設定
		masu_[i].centerPos = centerPos;
		masu_[i].masuMode = MasuMode::normalMode;
		masu_[i].x = i % max_X;
		masu_[i].z = i / max_Z;
	}
}

//初期化
void Aster::Init()
{
   	route_.clear();
	scoreTable_.clear();

	for (auto& masu : masu_) 
	{
		if (masu.second.masuMode == MasuMode::doneMode) 
		{
			masu.second.masuMode = MasuMode::normalMode;
		}
	}
}

//更新
void Aster::Update()
{
	//プレイヤーと敵のインデックスが同じだった場合
	//そのマスの中心座標を返す
	if (currentPlayerIndex_ == currentEnemyIndex_) 
	{

		//最短ルートを削除する
		route_.clear();

		//最短ルートを探す際にマスのモードをdoneModeにしたため
		//doneModeからnormalModeに戻す
		for (auto& mode : masu_) {
			if (mode.second.masuMode != MasuMode::blockadeMode) 
			{
				mode.second.masuMode = MasuMode::normalMode;
			}
		}
	}
}

//デバッグ描画
void Aster::DebugDraw()
{
	//デバッグ用描画
	for (int i = 0; i < max_Index; i++) 
	{
		if (masu_[i].masuMode == MasuMode::normalMode) 
		{
			DrawSphere3D(masu_[i].centerPos, sphere_radius, division_num, 0x00ff00, 0x00ff00, true);
		}
		else if(masu_[i].masuMode == MasuMode::blockadeMode) 
		{
			DrawSphere3D(masu_[i].centerPos, sphere_radius, division_num, 0xff0000, 0xff0000, true);
		}
		else 
		{
			DrawSphere3D(masu_[i].centerPos, sphere_radius, division_num, 0xffff00, 0xffff00, true);
		}
		
		VECTOR pos = ConvWorldPosToScreenPos(masu_[i].centerPos);
		DrawFormatStringF(pos.x, pos.y, 0x448844, "%d", i);
	}

	for (auto& result : debugScoreTable)
	{
		VECTOR pos = ConvWorldPosToScreenPos(masu_[result.second.destinationIndex].centerPos);
		DrawFormatStringF(pos.x, pos.y, 0xff0000, "S:%d", result.second.score);
	}
}

//現在のインデックス情報を更新する
void Aster::CurrentIndexUpdate(const VECTOR& playerPos, const VECTOR& enemyPos)
{
	//敵のインデックスを取得
	currentEnemyIndex_ = SearchCurrentIndex(enemyPos);

	//プレイヤーのインデックスを取得
	currentPlayerIndex_ = SearchCurrentIndex(playerPos);
}

// ポジション情報を元に配列の何番目に存在するか取得する
bool Aster::LocationInformation(const VECTOR& playerPos, const VECTOR& enemyPos)
{
	//enemyとplayerがいたインデックス座標
	int oldEnemyIndex = currentEnemyIndex_;
	int oldPlayerIndex = currentPlayerIndex_;

	//現在のインデックス情報を更新する
	CurrentIndexUpdate(playerPos, enemyPos);

	//プレイヤーのインデックス座標が前のフレームと比べたとき
	//違ったら、再度経路探索を行う
	if (oldPlayerIndex != currentPlayerIndex_ && masu_[currentPlayerIndex_].masuMode != MasuMode::blockadeMode)
	{
		Init();
		return true;
	}

	return false;
}

// 目的地の座標を取得する
VECTOR Aster::GetDestinationCoordinates(const VECTOR& playerPos)
{
	//追跡ルートが存在しない場合、現在の座標を返す
	if (route_.empty())
	{
		return masu_[currentEnemyIndex_].centerPos;
	}

	//プレイヤーが追跡不可能なオブジェクトの上に立っている時
	//エネミーが立っているインデックスの中心ポジションを返す
	if (masu_[currentPlayerIndex_].masuMode == MasuMode::blockadeMode) 
	{
		return masu_[currentEnemyIndex_].centerPos;
	}

	//目標インデックス座標の中心ポジション
	VECTOR targetPos = masu_[route_.front()].centerPos;

	//プレイヤーを追跡するルートで通った升を追跡ルート配列から削除する
	if (currentEnemyIndex_ == route_.front()) 
	{
		route_.pop_front();
	}

	//目標ポジションを返す
	return targetPos;
}

//経路探索
void Aster::RouteSearch()
{
	//現在のエネミーがいるインデックス座標を取得
	int currentIndex = currentEnemyIndex_;
	routeSearchEnemyIndex_ = currentEnemyIndex_;

	//エネミーとプレイヤーのインデックス座標が一致しない間
	while (routeSearchEnemyIndex_ != currentPlayerIndex_)
	{
		if (masu_[routeSearchEnemyIndex_].masuMode != MasuMode::blockadeMode) {
			masu_[routeSearchEnemyIndex_].masuMode = MasuMode::doneMode;
		}

		SearchAroundSquares();

		//次にスコアを出すマスが既にスコアが出してあった場合
		//何回目にスコア計算が行われたかを取得する
		for (auto& list : ShortestCandidateIndex_) {
			for (auto& index : list.second) {
				if (index == routeSearchEnemyIndex_) {
					moveCount_ = list.first;
				}
			}
		}
		moveCount_++;
	}

	moveCount_ = 0;

	int targetIndex = currentPlayerIndex_;

	//最短ルートをscoreTableから抽出する
	while (currentIndex != targetIndex)
	{
		targetIndex = scoreTable_[targetIndex].currentIndex;
		route_.push_front(targetIndex);
	}
	//最後にプレイヤーがいるインデックス座標を追加する
	route_.push_back(currentPlayerIndex_);

	//リセット
	scoreTable_.clear();
	debugScoreTable.clear();
	ShortestCandidateIndex_.clear();
}

/// 周囲の升が存在するか探す
void Aster::SearchAroundSquares()
{
	//両端、上下の端のマスを取得
	int leftEnd = routeSearchEnemyIndex_ % max_X;
	int rightEnd = routeSearchEnemyIndex_ % max_X;
	int bottom = routeSearchEnemyIndex_ / max_X;
	int top = routeSearchEnemyIndex_ / max_X;

	//マスがないところは見ないようにする
	bool isCheckLeft = false;
	bool isCheckBottom = false;
	bool isCheckRight = false;
	bool isCheckTop = false;

	if (leftEnd == 0) {
		isCheckLeft = true;
	}
	if (bottom == 0) {
		isCheckBottom = true;
	}
	if (rightEnd == max_X - 1) {
		isCheckRight = true;
	}
	if (top == max_Z) {
		isCheckTop = true;
	}

	SearchSurroundingSquares(isCheckLeft, isCheckRight, isCheckTop, isCheckBottom);
}

//周囲の升のスコアを取得する
void Aster::SearchSurroundingSquares(const bool skipCheckLeft, const  bool skipCheckRight, const  bool skipCheckTop, const  bool skipCheckBottom)
{
	//周囲の升のインデックス
	int left = routeSearchEnemyIndex_ - 1;
	int right = routeSearchEnemyIndex_ + 1;
	int top = routeSearchEnemyIndex_ + max_X;
	int bottom = routeSearchEnemyIndex_ - max_X;

	if (!skipCheckTop) {
		//上の升のスコアを取得する
		if (masu_[top].masuMode == MasuMode::normalMode) {
			ScoreCaluculation(Direction::top, top);
		}
	}

	if (!skipCheckBottom) {
		//下の升のスコアを取得する
		if (masu_[bottom].masuMode == MasuMode::normalMode) {
			ScoreCaluculation(Direction::bottom, bottom);
		}
	}

	if (!skipCheckLeft) {
		//左の升のスコアを取得する
		if (masu_[left].masuMode == MasuMode::normalMode) {
			ScoreCaluculation(Direction::left, left);
		}
	}

	if (!skipCheckRight) {
		//右の升のスコアを取得する
		if (masu_[right].masuMode == MasuMode::normalMode) {
			ScoreCaluculation(Direction::right, right);
		}
	}

	//初期化
	int score = 1000;

	//最短ルートを求める際に一番低いスコアのインデックスマスを次の探索マスにする
	for (auto& result : scoreTable_) {
		if (masu_[result.second.destinationIndex].masuMode == MasuMode::normalMode) {
			if (score > result.second.score) {
				score = result.second.score;
				routeSearchEnemyIndex_ = result.second.destinationIndex;
			}
		}
	}

	//デバッグ用
	debugScoreTable = scoreTable_;
}

//特定の升のスコアを取得する
void Aster::ScoreCaluculation(const Direction direction, const  int index)
{
	//推定コスト
	int estimationCostX = 0;
	int estimationCostZ = 0;

	//XとZどこに移動したか
	estimationCostX = masu_[currentPlayerIndex_].x - masu_[index].x;
	estimationCostZ = masu_[currentPlayerIndex_].z - masu_[index].z;

	//推定コストをdirectionの配列に入れる
	if (estimationCostX < 0 || estimationCostZ < 0)
	{
		if (estimationCostX < 0) {
			estimationCostX = -(std::min)(estimationCostX, 0);
		}
		if (estimationCostZ < 0) {
			estimationCostZ = -(std::min)(estimationCostZ, 0);
		}
		
		//四方向の時
		scoreTable_[index].estimationCost = estimationCostX + estimationCostZ;
	}
	else 
	{
		//四方向の時
		scoreTable_[index].estimationCost = estimationCostX + estimationCostZ;
	}

	//移動量をカウントする
	if (masu_[index].masuMode == MasuMode::normalMode) 
	{
		scoreTable_[index].moveCost = moveCount_;
		scoreTable_[index].score = scoreTable_[index].moveCost + scoreTable_[index].estimationCost;
		scoreTable_[index].destinationIndex = index;
		scoreTable_[index].currentIndex = routeSearchEnemyIndex_;
		scoreTable_[index].dir = direction;
	}

	ShortestCandidateIndex_[moveCount_].push_back(index);
}

//座標から升のインデックスを取得する
int Aster::SearchCurrentIndex(const VECTOR& pos)
{
	int index = 0;
	float min = 1000.0f;
	float distanceSize = 0.0f;

	//引数の座標から一番近いマスの中心を見つけindex番号を取得する
	for (int i = 0; i < max_Index; i++)
	{
		//中心から敵のポジションの距離をとる
		distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(masu_[i].centerPos, pos);

		//過去最短に近い結果と比べる
		if (min > distanceSize)
		{
			//過去最短を更新
			min = distanceSize;

			//インデックスを取得
			index = i;
		}
	}

	//インデックスを取得
	return index;
}

//敵からプレイヤーに向かうルートがないか取得する
bool Aster::GetIsRouteEmpty()
{
	//ルートが無い時、trueを返す
	if (route_.empty())
	{
		return true;
	}

	return false;
}

//引数のポジションのマスがblockmodeか判断する
bool Aster::SearchBlockadeMode(const VECTOR& pos)
{
	//引数の座標のインデックス座標を取得
	int pointIndex = SearchCurrentIndex(pos);

	//取得したインデックス座標の升がblockadeModeだったらtrueを返す
	if (masu_[pointIndex].masuMode == MasuMode::blockadeMode)
	{
		return true;
	}

	return false;
}