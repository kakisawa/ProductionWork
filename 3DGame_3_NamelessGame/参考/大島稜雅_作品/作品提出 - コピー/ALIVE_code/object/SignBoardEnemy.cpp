#include "SignBoardEnemy.h"

#include "Player.h"

#include "../util/Util.h"
#include "../util/Model.h"
#include "../util/FontsManager.h"
#include "../util/ModelManager.h"

namespace
{
	//0�`9�̐����o�͂��邽��
	constexpr int number_to_divide = 10;

	//�F��ς���}�e���A���̔ԍ�
	constexpr int change_material_num = 8;

	//�F
	constexpr COLOR_F color = { 1.0f, 0.0f, 0.0f, 1.0f };
}

//�R���X�g���N�^
SignBoardEnemy::SignBoardEnemy(const int handle, const Material materialType, const ObjectInfo& objInfo):EnemyBase(handle,materialType,objInfo)
{
	auto& model = ModelManager::GetInstance();

	//�}�e���A���̐F��ς���
	MV1SetMaterialDifColor(model_->GetModelHandle(), change_material_num, color);
	numModel_[0] = MV1DuplicateModel(model.GetModelHandle(objData_[static_cast<int>(ObjectType::Number)].name));
	numModel_[1] = MV1DuplicateModel(model.GetModelHandle(objData_[static_cast<int>(ObjectType::Number)].name));
}

//�f�X�g���N�^
SignBoardEnemy::~SignBoardEnemy()
{
}

//�X�V
void SignBoardEnemy::Update(Player& player)
{
	deathCount_ = player.GetDeathCount();
}

//�`��
void SignBoardEnemy::Draw()
{
	model_->Draw();

	//�w��̃t���[���C���f�b�N�X���擾
	int display1FrameNum = NumericTransformationMatrix(numModel_[0], "display1", std::to_string(deathCount_ / number_to_divide));
	int display2FrameNum = NumericTransformationMatrix(numModel_[1], "display2", std::to_string(deathCount_ % number_to_divide));

	//�w��t���[���̕`��
	MV1DrawFrame(numModel_[0], display1FrameNum);
	MV1DrawFrame(numModel_[1], display2FrameNum);
}

//�����̕ϊ��s����s���A�t���[���ԍ���Ԃ�
int SignBoardEnemy::NumericTransformationMatrix(int handle, const std::string& displayFrameName, const std::string& number)
{
	//����\������ꏊ���擾����
	VECTOR display1LocalPos = model_->GetFrameLocalPosition(displayFrameName);

	//�w��̃t���[���C���f�b�N�X���擾
	int displayFrameNum = MV1SearchFrame(handle, number.c_str());

	//�ʓx�@�̉�]Z
	float radianZ = MathUtil::DegreeToRadian(numRot_.z);

	//��]�s��
	MATRIX rotMtx = MGetRotZ(radianZ);

	//�g�k�s��
	MATRIX scaleMtx = MGetScale(numScale_);

	//���s�ړ��s��
	MATRIX displayTransMtx = MGetTranslate(display1LocalPos);

	//��]�Ɗg�k�s��̏�Z
	MATRIX displayMtx = MMult(rotMtx, scaleMtx);

	//��]�Ɗg�k�s��ƕ��s�ړ��s��̏�Z
	displayMtx = MMult(displayMtx, displayTransMtx);

	//�t���[���ɍs����g��
	MV1SetFrameUserLocalMatrix(handle, displayFrameNum, displayMtx);

	return displayFrameNum;
}
