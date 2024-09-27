#pragma once
#include "GimmickBase.h"
#include <memory>
#include <vector>
#include <DxLib.h>
#include <unordered_map>

class Switch;
class Lever;
class Model;

class Elevator final : public GimmickBase
{
private:
	//エレベーターのアニメタイプ
	enum class ElevatorAnimType
	{
		OpenIdle,
		CloseIdle,
		Open,
		Close,

		Max,
	};

	//エレベーターの状態
	enum class ElevatorState
	{
		Upper,
		Lower,
	};

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="handle">モデルハンドル</param>
	/// <param name="materialType">マテリアルのタイプ</param>
	/// <param name="objInfo">配置データ</param>
	Elevator(const int handle, const Material materialType, const ObjectInfo& objInfo);
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Elevator();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="player">プレイヤーの参照</param>
	void Update(Player& player)override;
	
	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;

	/// <summary>
	/// 音を鳴らす
	/// </summary>
	void PlayDoorSound();

	/// <summary>
	/// 出発することが可能か
	/// </summary>
	/// <returns></returns>
	bool CanDeparture();
	

	////////////////Getter////////////////

	/// <summary>
	/// 特殊な名前のオブジェクトの配置データを取得する
	/// </summary>
	/// <param name="name">名前</param>
	/// <param name="sign">記号</param>
	/// <param name="groupNum">グループ番号</param>
	/// <param name="num">番号</param>
	/// <returns>配置データ</returns>
	ObjectInfo GetSpecialNameObjectInfo(const std::string& name, const std::string& sign, int groupNum, int num);

	////////////////Setter////////////////

	/// <summary>
	/// 初期のエレベーターのステータスを設定する
	/// </summary>
	void SetInitState();

private:

	/// <summary>
	/// エレベーターを移動させる
	/// </summary>
	/// <returns>上昇ベクトル</returns>
	float Move();

	/// <summary>
	/// 移動先のポジションを取得する
	/// </summary>
	void TargetPosition();

private:

	float moveVecY_ = 0.0f;								//Y軸の移動ヴェクトル
	float elapsedTime_ = 0.0f;							//経過時間

	bool isDeparture_ = false;							//エレベーターが出発しているかどうか
	bool isPlaySound_ = false;							//サウンドが再生中か
	bool isStopping_ = false;							//エレベーターは停止しているか

	VECTOR targetPos_ = {};								//現在向かっているポジション

	ElevatorState state_ = {};							//エレベーターの状態

	std::unordered_map<ElevatorState, VECTOR> stopPos_;	//ストップポジション

	std::shared_ptr<Lever> movingLever_;				//レバークラスのスマートポインタ

	std::vector<std::shared_ptr<Lever>> levers_;		//レバークラスのスマートポインタのVector配列
};

