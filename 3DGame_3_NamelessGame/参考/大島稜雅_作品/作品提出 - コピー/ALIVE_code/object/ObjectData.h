#pragma once
#include <string>
#include <DxLib.h>

//�I�u�W�F�N�g�̍ޗ�(������)
enum class Material
{
	Stone,					//��
	Iron,					//�S
	Wood,					//��
	Other,					//���̑�
	max,
};

//�I�u�W�F�N�g�̃^�C�v
enum class ObjectType
{
	//������
	Enemy,
	Player,
	Corpse,
	CorpseMountain,
	SignBoardPlayer,

	//�M�~�b�N��
	Lever,
	Crank,
	Switch,
	Elevator,
	HopStepJump,
	CrankScaffold,
	TransScaffold,
	PenetrationScaffld,

	//�����Ɏg��
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

	//���C���X�e�[�W�Ɏg��
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

	//���̑�
	Rock,
	Number,
	WhiteBoard,

	Max,
};

//�I�u�W�F�N�g�̃f�[�^
struct ObjectData
{
	ObjectType type = {};			//�I�u�W�F�N�g�̃^�C�v
	std::string name = {};			//���O
	Material material = {};			//�f��
};

//�I�u�W�F�N�g�̃f�[�^
static ObjectData objData_[static_cast<int>(ObjectType::Max)]
{
	//�G
	ObjectType::Enemy,
	"Enemy",
	Material::Other,

	//�v���C���[
	ObjectType::Player,
	"Player",
	Material::Other,

	//����
	ObjectType::Corpse,
	"Player",
	Material::Other,

	//���̂̎R
	ObjectType::CorpseMountain,
	"CorpseMountain",
	Material::Other,

	//�Ŕ̓G
	ObjectType::SignBoardPlayer,
	"SignBoardPlayer",
	Material::Other,


	//���o�[
	ObjectType::Lever,
	"Lever",
	Material::Iron,

	//�N�����N
	ObjectType::Crank,
	"Crank",
	Material::Iron,

	//�X�C�b�`
	ObjectType::Switch,
	"Switch",
	Material::Iron,

	//�G���x�[�^�[
	ObjectType::Elevator,
	"Elevator",
	Material::Iron,

	//�z�b�v�X�e�b�v�W�����v
	ObjectType::HopStepJump,
	"HopStepJump",
	Material::Other,

	//�N�����N�̑���
	ObjectType::CrankScaffold,
	"CrankScaffold",
	Material::Iron,

	//�����ȑ���
	ObjectType::TransScaffold,
	"TransScaffold",
	Material::Iron,

	//�ђʂ��鑫��
	ObjectType::PenetrationScaffld,
	"PenetrationScaffld",
	Material::Iron,

	//�e���r
	ObjectType::TV,
	"TV",
	Material::Iron,

	//�x�b�h
	ObjectType::Bed,
	"Bed",
	Material::Wood,

	//�h�A
	ObjectType::Door,
	"Door",
	Material::Wood,

	//�{
	ObjectType::Book,
	"Book",
	Material::Other,

	//��
	ObjectType::Desk,
	"Desk",
	Material::Wood,

	//���v
	ObjectType::Clock,
	"Clock",
	Material::Iron,

	//��
	ObjectType::Window,
	"Window",
	Material::Wood,

	//�����̕�
	ObjectType::RoomWall,
	"RoomWall",
	Material::Stone,

	//�d��
	ObjectType::LightBulb,
	"LightBulb",
	Material::Other,

	//�؂̔�
	ObjectType::WoodenBox,
	"WoodenBox",
	Material::Wood,

	//�؂̏�
	ObjectType::WoodFloor,
	"WoodFloor",
	Material::Wood,

	//�Ŕ^�C�v2
	ObjectType::SignBoardType2,
	"SignBoardType2",
	Material::Iron,

	//�^�C��
	ObjectType::Tile,
	"Tile",
	Material::Stone,

	//�t�F���X
	ObjectType::Fence,
	"Fence",
	Material::Iron,

	//�d��
	ObjectType::Train,
	"Train",
	Material::Iron,

	//��
	ObjectType::Street,
	"Street",
	Material::Stone,

	//�XC
	ObjectType::StoreC,
	"StoreC",
	Material::Iron,

	//T�^�̓�
	ObjectType::TStreet,
	"TStreet",
	Material::Stone,

	//�w
	ObjectType::Station,
	"Station",
	Material::Iron,

	//���̕�
	ObjectType::WallBack,
	"WallBack",
	Material::Stone,

	//�ǂ̉�
	ObjectType::WallSide,
	"WallSide",
	Material::Stone,

	//�^�C���^�C�v2
	ObjectType::TileType2,
	"TileType2",
	Material::Stone,

	//����C
	ObjectType::BuildingC,
	"BuildingC",
	Material::Iron,

	//����A
	ObjectType::Building2A,
	"Building2A",
	Material::Iron,

	//�g���l���̕�
	ObjectType::TunnelWall,
	"TunnelWall",
	Material::Stone,

	//�X��
	ObjectType::StreetLight,
	"StreetLight",
	Material::Iron,

	//�ł�������
	ObjectType::BigBuildingA,
	"BigBuildingA",
	Material::Iron,

	//�Ԃ��R���e�i
	ObjectType::RedContainer,
	"RedContainer",
	Material::Iron,

	//���R���e�i
	ObjectType::BlueContainer,
	"BlueContainer",
	Material::Iron,

	//�Ŕ̃^�C�v1
	ObjectType::SignBoardType1,
	"SignBoardType1",
	Material::Iron,

	//�g���l���̓���
	ObjectType::TunnelEntrance,
	"TunnelEntrance",
	Material::Stone,

	//�C�G���[�R���e�i
	ObjectType::YellowContainer,
	"YellowContainer",
	Material::Iron,

	//�I�����W�R���e�i
	ObjectType::OrangeContainer,
	"OrangeContainer",
	Material::Iron,

	//��
	ObjectType::Rock,
	"Rock",
	Material::Stone,

	//����
	ObjectType::Number,
	"Number",
	Material::Other,

	//�z���C�g�{�[�h
	ObjectType::WhiteBoard,
	"WhiteBoard",
	Material::Other,
};

//�v�@76byte
struct ObjectInfo
{
	std::string name;	//�I�u�W�F�N�g�̖��O	//40byte
	VECTOR pos = {};	//�|�W�V����			//12byte
	VECTOR rot = {};	//��]��				//12byte
	VECTOR scale = {};	//�g�k��				//12byte
};