#pragma once
#include <list>
#include <DxLib.h>
#include <unordered_map>

class Aster
{
private:
	//メモリに保存されない(コンパイラ時に展開)
	enum class MasuMode {
		normalMode,				//通常モード
		doneMode,				//済みモード
		blockadeMode,			//封鎖モード
	};

	//メモリに保存されない(コンパイラ時に展開)
	enum class Direction {
		left,					//左
		topLeft,				//左上
		top,					//上
		topRight,				//右上
		right,					//右
		bottomRight,			//右下
		bottom,					//下
		bottomLeft,				//左下
	};

	//計　20byte
	struct MasuState {
		VECTOR centerPos;		//中心座標			//12byte
		MasuMode masuMode;		//升のモード		//メモリに保存されない(コンパイラ時に展開)
		int x;					//2次元配列のX		//4byte
		int z;					//2次元配列のY		//4byte
	};

	//計　20byte
	struct Score {
		int moveCost;			//移動コスト								//4byte
		int estimationCost;		//推定コスト								//4byte
		int score;				//プレイヤーを追跡するうえで得点を付ける	//4byte
		int currentIndex;		//移動先のインデックス						//4byte
		int destinationIndex;	//移動先のインデックス						//4byte
		Direction dir;			//方角										//メモリに保存されない(コンパイラ時に展開)
	};
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Aster();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Aster();

	/// <summary>
	/// 経路探索を行う配列の初期化
	/// </summary>
	/// <param name="pos">ポジション</param>
	void ArrayInit(const VECTOR& pos);

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void DebugDraw();

	/// <summary>
	/// 現在のインデックス情報を更新する
	/// </summary>
	/// <param name="playerPos">プレイヤーのポジション</param>
	/// <param name="enemyPos">敵のポジション</param>
	void CurrentIndexUpdate(const VECTOR& playerPos, const VECTOR& enemyPos);

	/// <summary>
	/// ポジション情報を元に配列の何番目に存在するか取得する
	/// </summary>
	/// <param name="playerPos">プレイヤーのポジション</param>
	/// <param name="enemyPos">敵のポジション</param>
	/// <returns>プレイヤーは移動したか true:した　false：していない</returns>
	bool LocationInformation(const VECTOR& playerPos, const VECTOR& enemyPos);

	/// <summary>
	/// 目的地の座標を取得する
	/// </summary>
	/// /// <param name="playerPos">プレイヤーのポジション</param>
	/// <returns>目的地のポジション</returns>
	VECTOR GetDestinationCoordinates(const VECTOR& playerPos);

	/// <summary>
	/// 経路探索
	/// </summary>
	void RouteSearch();

	/// <summary>
	/// 周囲の升が存在するか探す
	/// </summary>
	void SearchAroundSquares();

	/// <summary>
	/// 周囲の升を検索する
	/// </summary>
	void SearchSurroundingSquares(const bool skipCheckLeft, const bool skipCheckRight, const  bool skipCheckTop, const  bool skipCheckBottom);

	/// <summary>
	/// 升のスコアを取得する
	/// </summary>
	/// <param name="direction">方向</param>
	/// <param name="index">升の番号</param>
	void ScoreCaluculation(const Direction direction, const int index);

	/// <summary>
	/// 座標から升のインデックスを取得する
	/// </summary>
	/// <param name="pos"></param>
	/// <returns></returns>
	int SearchCurrentIndex(const VECTOR& pos);

	/// <summary>
	/// 敵からプレイヤーに向かうルートがないか取得する
	/// </summary>
	/// <returns>true:ない　false:ある</returns>
	bool GetIsRouteEmpty();

	/// <summary>
	/// 引数のポジションのマスがblockmodeか判断する
	/// </summary>
	/// <param name="pos">ポジション</param>
	/// <returns>true：blockmode　false：blockmodeではない</returns>
	bool SearchBlockadeMode(const VECTOR& pos);
private:

	int routeSearchEnemyIndex_ = 0;										//最短距離を調べるために敵のインデックスを更新する
	int currentEnemyIndex_ = 0;											//現在の敵のインデックス
	int currentPlayerIndex_ = 0;										//プレイヤーのインデックス
	int moveCount_ = 0;													//移動数

	std::list<int> route_;												//最短ルートを保存する

	std::unordered_map<int, MasuState> masu_;							//移動範囲
	std::unordered_map<int, Score> scoreTable_;							//移動範囲の升にスコアをつける
	std::unordered_map<int, Score> debugScoreTable;						//デバッグ用のスコアテーブル(削除してよい)
	std::unordered_map<int, std::list<int>> ShortestCandidateIndex_;	//最短ルートの候補としてでたインデックスと何回目に最短候補として出されたか保存する
};

