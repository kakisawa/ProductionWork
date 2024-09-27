#pragma once
#include <string>
#include <DxLib.h>

//オブジェクトの材料(見た目)
enum class Material
{
	Stone,					//石
	Iron,					//鉄
	Wood,					//木
	Other,					//その他
	max,
};

//オブジェクトのタイプ
enum class ObjectType
{
	//生物類
	Enemy,
	Player,
	Corpse,
	CorpseMountain,
	SignBoardPlayer,

	//ギミック類
	Lever,
	Crank,
	Switch,
	Elevator,
	HopStepJump,
	CrankScaffold,
	TransScaffold,
	PenetrationScaffld,

	//部屋に使う
	TV,
	Bed,
	Door,
	Book,
	Desk,
	Clock,
	Window,
	RoomWall,
	LightBulb,
	WoodenBox,
	WoodFloor,
	SignBoardType2,

	//メインステージに使う
	Tile,
	Fence,
	Train,
	Street,
	StoreC,
	TStreet,
	Station,
	WallBack,
	WallSide,
	TileType2,
	BuildingC,
	Building2A,
	TunnelWall,
	StreetLight,
	BigBuildingA,
	RedContainer,
	BlueContainer,
	SignBoardType1,
	TunnelEntrance,
	YellowContainer,
	OrangeContainer,

	//その他
	Rock,
	Number,
	WhiteBoard,

	Max,
};

//オブジェクトのデータ
struct ObjectData
{
	ObjectType type = {};			//オブジェクトのタイプ
	std::string name = {};			//名前
	Material material = {};			//素材
};

//オブジェクトのデータ
static ObjectData objData_[static_cast<int>(ObjectType::Max)]
{
	//敵
	ObjectType::Enemy,
	"Enemy",
	Material::Other,

	//プレイヤー
	ObjectType::Player,
	"Player",
	Material::Other,

	//死体
	ObjectType::Corpse,
	"Player",
	Material::Other,

	//死体の山
	ObjectType::CorpseMountain,
	"CorpseMountain",
	Material::Other,

	//看板の敵
	ObjectType::SignBoardPlayer,
	"SignBoardPlayer",
	Material::Other,


	//レバー
	ObjectType::Lever,
	"Lever",
	Material::Iron,

	//クランク
	ObjectType::Crank,
	"Crank",
	Material::Iron,

	//スイッチ
	ObjectType::Switch,
	"Switch",
	Material::Iron,

	//エレベーター
	ObjectType::Elevator,
	"Elevator",
	Material::Iron,

	//ホップステップジャンプ
	ObjectType::HopStepJump,
	"HopStepJump",
	Material::Other,

	//クランクの足場
	ObjectType::CrankScaffold,
	"CrankScaffold",
	Material::Iron,

	//透明な足場
	ObjectType::TransScaffold,
	"TransScaffold",
	Material::Iron,

	//貫通する足場
	ObjectType::PenetrationScaffld,
	"PenetrationScaffld",
	Material::Iron,

	//テレビ
	ObjectType::TV,
	"TV",
	Material::Iron,

	//ベッド
	ObjectType::Bed,
	"Bed",
	Material::Wood,

	//ドア
	ObjectType::Door,
	"Door",
	Material::Wood,

	//本
	ObjectType::Book,
	"Book",
	Material::Other,

	//机
	ObjectType::Desk,
	"Desk",
	Material::Wood,

	//時計
	ObjectType::Clock,
	"Clock",
	Material::Iron,

	//窓
	ObjectType::Window,
	"Window",
	Material::Wood,

	//部屋の壁
	ObjectType::RoomWall,
	"RoomWall",
	Material::Stone,

	//電球
	ObjectType::LightBulb,
	"LightBulb",
	Material::Other,

	//木の箱
	ObjectType::WoodenBox,
	"WoodenBox",
	Material::Wood,

	//木の床
	ObjectType::WoodFloor,
	"WoodFloor",
	Material::Wood,

	//看板タイプ2
	ObjectType::SignBoardType2,
	"SignBoardType2",
	Material::Iron,

	//タイル
	ObjectType::Tile,
	"Tile",
	Material::Stone,

	//フェンス
	ObjectType::Fence,
	"Fence",
	Material::Iron,

	//電車
	ObjectType::Train,
	"Train",
	Material::Iron,

	//道
	ObjectType::Street,
	"Street",
	Material::Stone,

	//店C
	ObjectType::StoreC,
	"StoreC",
	Material::Iron,

	//T型の道
	ObjectType::TStreet,
	"TStreet",
	Material::Stone,

	//駅
	ObjectType::Station,
	"Station",
	Material::Iron,

	//後ろの壁
	ObjectType::WallBack,
	"WallBack",
	Material::Stone,

	//壁の横
	ObjectType::WallSide,
	"WallSide",
	Material::Stone,

	//タイルタイプ2
	ObjectType::TileType2,
	"TileType2",
	Material::Stone,

	//建物C
	ObjectType::BuildingC,
	"BuildingC",
	Material::Iron,

	//建物A
	ObjectType::Building2A,
	"Building2A",
	Material::Iron,

	//トンネルの壁
	ObjectType::TunnelWall,
	"TunnelWall",
	Material::Stone,

	//街灯
	ObjectType::StreetLight,
	"StreetLight",
	Material::Iron,

	//でかい建物
	ObjectType::BigBuildingA,
	"BigBuildingA",
	Material::Iron,

	//赤いコンテナ
	ObjectType::RedContainer,
	"RedContainer",
	Material::Iron,

	//青いコンテナ
	ObjectType::BlueContainer,
	"BlueContainer",
	Material::Iron,

	//看板のタイプ1
	ObjectType::SignBoardType1,
	"SignBoardType1",
	Material::Iron,

	//トンネルの入口
	ObjectType::TunnelEntrance,
	"TunnelEntrance",
	Material::Stone,

	//イエローコンテナ
	ObjectType::YellowContainer,
	"YellowContainer",
	Material::Iron,

	//オレンジコンテナ
	ObjectType::OrangeContainer,
	"OrangeContainer",
	Material::Iron,

	//岩
	ObjectType::Rock,
	"Rock",
	Material::Stone,

	//数字
	ObjectType::Number,
	"Number",
	Material::Other,

	//ホワイトボード
	ObjectType::WhiteBoard,
	"WhiteBoard",
	Material::Other,
};

//計　76byte
struct ObjectInfo
{
	std::string name;	//オブジェクトの名前	//40byte
	VECTOR pos = {};	//ポジション			//12byte
	VECTOR rot = {};	//回転率				//12byte
	VECTOR scale = {};	//拡縮率				//12byte
};