#include "GameMain.h"
#include "GameEnd.h"
#include "ScenePause.h" 
#include "SceneManager.h"

#include "../gimmick/Switch.h"
#include "../gimmick/GimmickBase.h"

#include "../object/Player.h"
#include "../object/Camera.h"
#include "../object/ObjectData.h"
#include "../object/ShotManager.h"
#include "../object/CharacterBase.h"
#include "../object/ObjectManager.h"

#include "../util/game.h"
#include "../util/Util.h"
#include "../util/model.h"
#include "../util/Tutorial.h"
#include "../util/InputState.h"
#include "../util/ExternalFile.h"
#include "../util/SoundManager.h"
#include "../util/ModelManager.h"
#include "../util/EffectManager.h"
#include "../util/CheckCollisionModel.h"

#include <algorithm>

namespace
{
	//�t�F�[�h�̍ő�l
	constexpr int max_fade_value = 255;

	//�K�E�X�����Ŏg�p����s�N�Z����
	constexpr int pixel_width = 32;

	//�ڂ����̃p�����[�^�[
	constexpr int pixel_param = 800;

	//�J�v�Z���̔��a
	constexpr float capsel_radius = 600.0f;

	//�X�J�C�h�[���̃T�C�Y
	constexpr float skydorm_scale = 30.0f;

	//3D�T�E���h��1m�̒l
	constexpr float sound_3D_1m_value = 1.0f;

	//�J�����̏����ʒu
	constexpr VECTOR camera_init_pos = { 0, 300, -550 };

	//�X�J�C�h�[���̏����ʒu
	constexpr VECTOR skydorm_init_pos = { 0, 200, 0 };

	//�f�B���N�V�������C�g�̊p�x
	constexpr VECTOR direction_light_dir = { 33.0f, -33.0f, 33.0f };
}

//�R���X�g���N�^
GameMain::GameMain(SceneManager& manager) : SceneBase(manager),updateFunc_(&GameMain::FadeInUpdate)
{
	//�Z�k��
	auto& file = ExternalFile::GetInstance();

	//�C���X�^���X��
	player_ = std::make_shared<Player>(file.GetMainSpecialObjectInfo("Player"));
	tutorial_ = std::make_shared<Tutorial>();
	shotManager_ = std::make_shared<ShotManager>();
	objManager_ = std::make_shared<ObjectManager>();
	checkCollisionModel_ = std::make_shared<CheckCollisionModel>();

	//�J�����̃|�W�V����
	VECTOR pos = VAdd(player_->GetStatus().pos, camera_init_pos);
	camera_ = std::make_shared<Camera>(pos, player_->GetStatus().pos);

	//�f�B���N�V�������C�g�ɐݒ肷��
	ChangeLightTypeDir(direction_light_dir);

	//���C�g�̃f�B�t�B�[�Y�J���[��ݒ肷��
	SetLightDifColor(GetColorF(1.0f, 1.0f, 1.0f, 1.0f));

	//�Q�[���I�u�W�F�N�g�̐���
	objManager_->MainStageObjectGenerator();
}

//�f�X�g���N�^
GameMain::~GameMain()
{
	//�Q�[�����C�����I���Ƃ��Ƀv���C���Ɏ��񂾉񐔂�
	//saveData�ɋL�^����Ă��鎀�S�񐔂𑫂�
	ExternalFile::GetInstance().SaveDataRewriteInfo(totalDeathNum_);
}

//������
void GameMain::Init()
{
	//�Z�k��
	auto& model = ModelManager::GetInstance();

	//�X�N���[���摜�̍쐬
	makeScreenHandle_ = MakeScreen(Game::screen_width, Game::screen_height, true);

	//1m�͈̔͂�ݒ肷��
	Set3DSoundOneMetre(sound_3D_1m_value);

	//�X�J�C�h�[���̐ݒ�
	skyHandle_ = model.GetModelHandle("SkyDorm");
	MV1SetScale(skyHandle_, VGet(skydorm_scale, skydorm_scale, skydorm_scale));
	MV1SetPosition(skyHandle_, skydorm_init_pos);
}

//�I��
void GameMain::End()
{
	DeleteGraph(makeScreenHandle_);
	MV1DeleteModel(skyHandle_);
}

//�X�V
void GameMain::Update()
{
	(this->*updateFunc_)();
}

//�`��
void GameMain::Draw()
{
	SetDrawScreen(makeScreenHandle_);
	ClearDrawScreen();

	//�J�����̏�����
	//SetDrawScreen���s���ƃJ�����̏�񂪃��Z�b�g����邽�߂�
	camera_->Init(camera_->GetTarget());
	camera_->Update(player_->GetStatus().pos, player_->GetStatus().height);

	//�v���C���[�̕`��
	player_->Draw();

	//�I�u�W�F�N�g�̕`��
	objManager_->Draw();

	//�G�t�F�N�g�̕`��
	EffectManager::GetInstance().Draw();

	//�X�J�C�h�[���̕`��
//	MV1SetPosition(skyHandle_, player_->GetStatus().pos);
//	MV1DrawModel(skyHandle_);

	//�e�̕`��
	shotManager_->Draw();

	//�`���[�g���A���̕`��
	tutorial_->Draw();

#ifdef _DEBUG
	VECTOR pos = player_->GetStatus().pos;
	DrawFormatString(0, 48, 0xffffff, "%.2f,%.2f,%.2f", pos.x, pos.y, pos.z);
	int num = GetEnableLightHandleNum();
	DrawFormatString(0, 64, 0xffffff, "%d", num);
#endif // _DEBUG

	SetDrawScreen(DX_SCREEN_BACK);

	//�t�B���^�[�������s����
	if (isFilterOn_) 
	{
		GraphFilter(makeScreenHandle_, DX_GRAPH_FILTER_GAUSS, pixel_width, pixel_param);
	}

	//makescreenHandle�ɏ������񂾓��e��`�悷��
	DrawGraph(0, 0, makeScreenHandle_, true);

	//��ʑS�̂�^�����ɓh��Ԃ�
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawBox(0, 0, Game::screen_width, Game::screen_height, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

//�S�[�����Ă��邩�𔻒肷��
void GameMain::GoalCheck()
{
	//�Z�k��
	auto& file = ExternalFile::GetInstance();

	//�S�[���̔z�u�f�[�^���擾
	auto info = file.GetMainSpecifiedInfo("GOAL");

	//�Փ˔���
	auto result = MV1CollCheck_Capsule( player_->GetModelPointer()->GetModelHandle(),
										player_->GetModelPointer()->GetColFrameIndex(),
										VGet(info.pos.x, info.pos.y - info.scale.y, info.pos.z),
										VGet(info.pos.x, info.pos.y + info.scale.y, info.pos.z), capsel_radius);
	//�I������
	if (result.HitNum > 0)
	{
		totalDeathNum_ += player_->GetDeathCount();
		file.SetDeathCount(totalDeathNum_);
		updateFunc_ = &GameMain::FadeOutUpdate;
	}

	//�Փ˔���̍폜
	MV1CollResultPolyDimTerminate(result);
}

//TODO�F�ʂ̃t�F�[�h�C�����o���������
void GameMain::FadeInUpdate()
{
	fadeValue_ = static_cast <int>(max_fade_value * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0) 
	{
		updateFunc_ = &GameMain::NormalUpdate;
		fadeValue_ = 0;
	}
}

//�X�V
void GameMain::NormalUpdate()
{
	//�Z�k��
	auto& input = InputState::GetInstance();
	auto& file = ExternalFile::GetInstance();

	//�t�B���^�[�������s��Ȃ��p�ɂ���
	isFilterOn_ = false;

	//�v���C���[�̍X�V
	player_->Update();

	//�I�u�W�F�N�g�̍X�V
	objManager_->Update(*player_,shotManager_);

	//�G�t�F�N�g�̍X�V
	EffectManager::GetInstance().Update();

	//�v���C���[�Ƃ��̑��I�u�W�F�N�g�Ƃ̏Փ˔���
	checkCollisionModel_->CheckCollision(player_,objManager_);

	//�e�̍X�V
	shotManager_->Update();

	//�e�ƃv���C���[�̏Փ˔���
	shotManager_->Hit(*player_);

	//�`���[�g���A��
	tutorial_->Update(player_->GetStatus().pos);

	//���[�邵�Ă��邩�𔻒肷��
	GoalCheck();

	//���X�i�[�̈ʒu�ƕ�����ݒ�
	//����́A�v���C���[�ł͂Ȃ��J�����̍��W�ɂ��Ă���
	SoundManager::GetInstance().Set3DSoundListenerInfo(camera_->GetPos(), camera_->GetTarget());

	//�|�[�Y�V�[�����J��
	if (input.IsTriggered(InputType::Pause)) 
	{
		isFilterOn_ = true;
		manager_.PushFrontScene(std::shared_ptr<SceneBase>(std::make_shared<ScenePause>(manager_)));
	}
}

//TODO�F�ʂ̃t�F�[�h�C�����o���������
void GameMain::FadeOutUpdate()
{
	fadeValue_ = static_cast <int>(max_fade_value * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (++fadeTimer_ == fadeInterval_) 
	{
		manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<GameEnd>(manager_)));
		return;
	}
}
