#include "ObjectManager.h"

#include "Player.h"
#include "Corpse.h"
#include "EnemyBase.h"
#include "ThrowEnemy.h"
#include "ObjectBase.h"
#include "StreetLight.h"
#include "OrnamentBase.h"
#include "SignBoardEnemy.h"

#include "../gimmick/Elevator.h"
#include "../gimmick/HopStepJump.h"
#include "../gimmick/CrankScaffold.h"
#include "../gimmick/TransparentObject.h"
#include "../gimmick/PenetrationScaffld.h"

#include "../util/game.h"
#include "../util/Util.h"
#include "../util/InputState.h"
#include "../util/ExternalFile.h"
#include "../util/ModelManager.h"

namespace
{
	//�~�̔��a
	constexpr float distance_from_center = 200.0f;

	//�G���~����ɏo��������ۂ̈�̓�����̊p�x
	constexpr float center_angle = 10.0f;

	//�I�u�W�F�N�g�̍X�V���s��Ȃ��͈�
	constexpr float not_update_range = 4000.0f;

	//���̂𐶐�����ő吔
	constexpr int max_corpse_num = 10;
}

//�R���X�g���N�^
ObjectManager::ObjectManager()
{
}

//�f�X�g���N�^
ObjectManager::~ObjectManager()
{
}

//���C���X�e�[�W�̃I�u�W�F�N�g����
void ObjectManager::MainStageObjectGenerator()
{
	//�Z�k��
	auto& file = ExternalFile::GetInstance();

	//�u���̐���
	for (const auto& objTable : file.GetLoadMainStageObjectInfo())
	{
		for (const auto& objInfo : objTable.second)
		{
			for (const auto& data : objData_)
			{
				if (objInfo.name == data.name)
				{
					OrnamentGenerator(data.name, data.type, data.material, objInfo);
				}
			}
		}
	}

	//�M�~�b�N�̐���
	for (const auto& objTable : file.GetGimmickInfo())
	{
		for (const auto& objInfo : objTable.second)
		{
			for (const auto& data : objData_)
			{
				if (objTable.first == data.name)
				{
					GimmickObjectGenerator(data.name, data.type, data.material, objInfo);
				}
			}
		}
	}
}

//�I�[�v�j���O�V�[���̃I�u�W�F�N�g�𐶐�����
void ObjectManager::OpeningStageObjectGenerator()
{
	//�Z�k��
	auto& file = ExternalFile::GetInstance();

	//�u���̐���
	for (const auto& objTable : file.GetLoadOpeningStageObjectInfo())
	{
		for (const auto& objInfo : objTable.second)
		{
			for (const auto& data : objData_)
			{
				if (objInfo.name == data.name)
				{
					OrnamentGenerator(data.name, data.type, data.material, objInfo);
				}
			}
		}
	}
}

//�G���f�B���O�V�[���̃I�u�W�F�N�g�𐶐�����
void ObjectManager::EndStageObjectGenerator()
{
	//�Z�k��
	auto& file = ExternalFile::GetInstance();

	//�u���̐���
	for (const auto& objTable : file.GetLoadEndingStageObjectInfo())
	{
		for (const auto& objInfo : objTable.second)
		{
			for (const auto& data : objData_)
			{
				if (objInfo.name == data.name)
				{
					OrnamentGenerator(data.name, data.type, data.material, objInfo);
				}
			}
		}
	}
}

//���̐���
void ObjectManager::CorpseGenerator(const int handle, const ObjectInfo& objInfo)
{
	//���̂����������
	objects_[ObjectType::Corpse].push_back(std::make_shared<Corpse>(handle, Material::Other, objInfo));

	//���̂�10�����������烊�^�[��
	if (objects_[ObjectType::Corpse].size() < max_corpse_num) return;
	
	//���̂̃��X�g�̈�Ԑ擪(�Â�)���̂��폜����
	objects_[ObjectType::Corpse].remove(objects_[ObjectType::Corpse].front());
}

//�X�V
void ObjectManager::Update(Player& player,const std::shared_ptr<ShotManager>& shotManager)
{
	//objects_�̊e�v�f��isEnable���擾���A�����ɂȂ��Ă���ΊY���R���e�i�̍폜
	for (auto& list : objects_)
	{
		list.second.remove_if([](std::shared_ptr<ObjectBase> obj) {return !obj->GetIsEnabled(); });
	}

	//�Z�k��
	auto& model = ModelManager::GetInstance();

	float distanceSize = 0.0f;
	VECTOR playerPos = player.GetStatus().pos;

	//���̂𐶐�����
	if (player.GetIsCorpseGeneratable())
	{
		CorpseGenerator(model.GetModelHandle(objData_[static_cast<int>(ObjectType::Corpse)].name), player.GetCorpseInfo());
		player.SetIsCorpseGeneratable(false);
	}

	//���̂Ƃ��̑��̃I�u�W�F�N�g�̏Փ˔�����s��
	for (const auto& list : objects_)
	{
		for (const auto& obj : list.second)
		{
			if (list.first == ObjectType::Corpse) 
			{
				continue;
			}
			for (const auto& deadperson : objects_[ObjectType::Corpse])
			{
				distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(obj->GetPos(), playerPos);
				if (distanceSize < not_update_range)
				{
					obj->UpdateForCorpse(deadperson);
				}
			}
		}
	}

	//enemy��Shot
	for (const auto& obj : objects_[ObjectType::Enemy])
	{
		if (std::dynamic_pointer_cast<ThrowEnemy>(obj) != nullptr)
		{
			std::dynamic_pointer_cast<ThrowEnemy>(obj)->Shot(shotManager, playerPos, player.GetStatus().height);
		}
	}

	//�X�V
	for (const auto& list : objects_)
	{
		for (const auto& obj : list.second)
		{
			distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(obj->GetPos(), playerPos);
			if (distanceSize < not_update_range)
			{
				obj->Update(player);
			}
		}
	}

	//�G�̐���
	for (const auto& enemy : ExternalFile::GetInstance().GetEnemyInfo(playerPos))
	{
		if (!usedEnemyList_[enemy.name])
		{
			EnemyGenerator(player.GetDeathCount(), enemy);
		}
	}

	//���̂̎R�̐���
	for (const auto& table : ExternalFile::GetInstance().GetGimmickInfo())
	{
		if (table.first != "CorpseMountain")
		{
			continue;
		}

		for (const auto& corpseMt : table.second)
		{
			if (!usedCorpseMtList_[corpseMt.name]) 
			{
				GenerateCorpseMountain(player.GetDeathCount(), corpseMt);
			}
		}
	}
}

//�`��
void ObjectManager::Draw()
{
	for (const auto& objs : objects_)
	{
		for (const auto& obj : objs.second)
		{
			obj->Draw();
		}
	}
}

//�Փ˔���Ɏg�p���郂�f�����擾����
const std::list<std::shared_ptr<Model>>& ObjectManager::GetAllCheckCollModel()
{
	for (const auto& obj : objects_)
	{
		for (const auto& model : obj.second)
		{
			if (model->GetIsCollCheck())
			{
				checkCollList_.push_back(model->GetModelPointer());
			}
		}
	}

	return checkCollList_;
}

//����̃��f���|�C���^���擾����
std::shared_ptr<Model> ObjectManager::GetSpecificModel(const ObjectType type)
{
	std::shared_ptr<Model> specificPoint;

	//�����Ŏw�肳�ꂽ�I�u�W�F�N�g�̃��f���|�C���^����L�Ő錾����
	//�z��ɑ������
	for (const auto& obj : objects_)
	{
		//�����ƈႤ�^�C�v�������ꍇcontinue
		if (obj.first != type) 
		{
			continue;
		}
		//�w�肳�ꂽ�^�C�v�������ꍇ
		//���̃I�u�W�F�N�g�̃��X�g���烂�f���|�C���^���擾��
		//specificList�ɑ������
		for (auto& objSecond : obj.second) {
			specificPoint = objSecond->GetModelPointer();
		}
	}

	//���X�g��Ԃ�
	return specificPoint;
}

//����̃I�u�W�F�N�g�x�[�X�|�C���^���擾����
std::list<std::shared_ptr<ObjectBase>> ObjectManager::GetSpecificObject(const ObjectType type)
{
	std::list<std::shared_ptr<ObjectBase>> obj = {};

	if (objects_.count(type) > 0)
	{
		obj = objects_[type];
	}

	return obj;
}

//�Փ˔�����s�����f����ǉ�����
void ObjectManager::AddCheckCollModel()
{
	checkCollList_.clear();

	for (const auto& obj : objects_)
	{
		for (const auto& objSecond : obj.second)
		{
			if (objSecond->AddCollModel() != nullptr) 
			{
				checkCollList_.push_back(objSecond->AddCollModel());
			}
		}
	}
}

//�~����̃|�W�V�������擾����
void ObjectManager::CircumferencePosition(const float angle,VECTOR& infoPos, const VECTOR& playerPos)
{
	VECTOR pos = {};
	float radian = MathUtil::DegreeToRadian(angle);

	pos.x = sin(radian);
	pos.z = cos(radian);
	pos.y = 0.0f;

	pos = VScale(pos, distance_from_center);

	pos.y = infoPos.y;

	pos = VAdd(playerPos, pos);

	pos.y = infoPos.y;

	infoPos = pos;
}

//�G����
void ObjectManager::EnemyGenerator(const int deathCount, const ObjectInfo& info)
{
	//�Z�k��
	auto& model = ModelManager::GetInstance();

	//������̃T�C�Y���擾����
	int size = static_cast<int>(info.name.size());

	//�u.�v�ȍ~�̕�����ɂ����
	//�G�l�~�[�̏����p�^�[����ύX����
	if (size > 0) {
		std::string numStr = StrUtil::GetStringAfterSign(info.name, ".");
		numStr = StrUtil::GetStringBeforeSign(numStr, "-");

		std::string str = StrUtil::GetStringBeforeSign(info.name, ".");

		//�����񂪁uALL�v��������
		if (numStr == "ALL") 
		{
			GeneratedForTheNumberOfTimesYouDie(deathCount, info);
		}
		else if (str == "SignBoardEnemy") 
		{
			objects_[ObjectType::Enemy].push_back(std::make_shared<SignBoardEnemy>(model.GetModelHandle(objData_[static_cast<int>(ObjectType::SignBoardPlayer)].name), Material::Other, info));
			usedEnemyList_[info.name] = true;
		}
		else 
		{
			GeneratePredeterminedNumberOfTimes(deathCount, numStr, info);
		}
	}
}

//���񂾉񐔕���������
void ObjectManager::GeneratedForTheNumberOfTimesYouDie(const int deathCount,ObjectInfo info)
{
	//�Z�k��
	auto& model = ModelManager::GetInstance();

	float angle = 0.0f;
	for (int i = 0; i < deathCount; i++) 
	{
		//�v���C���[�𒆐S�ɉ~����ŃX�|�[��������
		CircumferencePosition(angle, info.pos, info.pos);

		//�C���X�^���X��
		objects_[ObjectType::Enemy].push_back(std::make_shared<ThrowEnemy>(model.GetModelHandle(objData_[static_cast<int>(ObjectType::Player)].name), Material::Other, info));
		angle -= center_angle;

		usedEnemyList_[info.name] = true;
	}
}

//����̉񐔕���������
void ObjectManager::GeneratePredeterminedNumberOfTimes(const int deathCount, const std::string& str, const ObjectInfo& info)
{
	//�Z�k��
	auto& model = ModelManager::GetInstance();

	//�Ō�̕������int�^�̐��l�ɕϊ�����
	int num = atoi(str.c_str());

	//������̍Ō�̐�����deathCount���������
	//�G�l�~�[����������
	if (num <= deathCount) 
	{
		objects_[ObjectType::Enemy].push_back(std::make_shared<ThrowEnemy>(model.GetModelHandle(objData_[static_cast<int>(ObjectType::Player)].name), Material::Other, info));
		usedEnemyList_[info.name] = true;
	}
}

//���񂾉񐔂ɂ���ĕ⏕����𐶐�����
void ObjectManager::GenerateCorpseMountain(const int deathCount, const ObjectInfo& info)
{
	//�Z�k��
	auto& model = ModelManager::GetInstance();

	std::string str = StrUtil::GetStringAfterSign(info.name, ".");
	str = StrUtil::GetStringBeforeSign(str, "-");

	//�Ō�̕������int�^�̐��l�ɕϊ�����
	int num = atoi(str.c_str());

	//������̍Ō�̐�����deathCount���������
	//�G�l�~�[����������
	if (num <= deathCount) 
	{
		objects_[ObjectType::CorpseMountain].push_back(std::make_shared<OrnamentBase>(model.GetModelHandle(objData_[static_cast<int>(ObjectType::CorpseMountain)].name), Material::Other, info));
		usedCorpseMtList_[info.name] = true;
	}
}

//�u�������@
void ObjectManager::OrnamentGenerator(const std::string& name, const ObjectType objType, const Material materialType, const ObjectInfo& objInfo)
{
	//�Z�k��
	auto& model = ModelManager::GetInstance();

	//���C�g�̃C���X�^���X��
	if (objType == ObjectType::StreetLight)
	{
		objects_[objType].push_front(std::make_shared<StreetLight>(model.GetModelHandle(name), materialType, objInfo));
		return;
	}

	objects_[objType].push_front(std::make_shared<OrnamentBase>(model.GetModelHandle(name), materialType, objInfo));
}

//�M�~�b�N�����@
void ObjectManager::GimmickObjectGenerator(const std::string& name, const ObjectType objType, const  Material materialType, const ObjectInfo& objInfo)
{
	//�Z�k��
	auto& model = ModelManager::GetInstance();

	switch (objType) 
	{
	case ObjectType::TransScaffold:
		objects_[objType].push_front(std::make_shared<TransparentObject>(model.GetModelHandle(name),materialType, objInfo));
		break;
	case ObjectType::Elevator:
		objects_[objType].push_front(std::make_shared<Elevator>(model.GetModelHandle(name),materialType, objInfo));
		break;
	case ObjectType::CrankScaffold:
		objects_[objType].push_front(std::make_shared<CrankScaffold>(model.GetModelHandle(name),materialType, objInfo));
		break;
	case ObjectType::PenetrationScaffld:
		objects_[objType].push_front(std::make_shared<PenetrationScaffld>(model.GetModelHandle("Train"), materialType, objInfo));
		break;
	case ObjectType::HopStepJump:
		objects_[objType].push_front(std::make_shared<HopStepJump>(model.GetModelHandle(name), materialType, objInfo));
		break;
	}
}
