#include "SceneTitle.h"
#include "GameMain.h"
#include "ScenePause.h"
#include "SceneManager.h"
#include "SelectChapterScene.h"
#include "SettingSceneForSceneTitle.h"

#include "../object/Camera.h"
#include "../object/LightBulb.h"
#include "../object/ObjectManager.h"

#include "../util/game.h"
#include "../util/util.h"
#include "../util/Model.h"
#include "../util/InputState.h"
#include "../util/FontsManager.h"
#include "../util/SoundManager.h"
#include "../util/ExternalFile.h"
#include "../util/ModelManager.h"
#include "../util/GraphManager.h"
#include "../util/UIItemManager.h"

#include <algorithm>

namespace
{
	//�A�j���[�V������ύX����̂ɂ����鎞��
	constexpr int anim_change_time = 10;

	//��ʂ̕�����
	constexpr int screen_split_num = 128;

	//��ʂ̔���
	constexpr int screen_half = 2;

	//�e�摜�̉�ʂ̕����z�u�|�W�V����
	constexpr int up_split_num = 92;
	constexpr int down_split_num = 87;
	constexpr int left_split_num = 92;
	constexpr int right_split_num = 97;
	constexpr int decision_split_num = 112;
	constexpr int move_str_split_num = 100;
	constexpr int decision_str_split_num = 116;

	//�t�F�[�h�̍ő�l
	constexpr int max_fade_value = 255;

	//UI�z�u�|�W�V����X
	constexpr int UI_pos_x = 320;

	//UI�z�u�|�W�V����Y
	constexpr int UI_pos_y = 100;

	//�^�C�g���̃r���{�[�g�̃T�C�Y
	constexpr float title_bill_board_size = 300.0f;

	//�r���{�[�g�̃T�C�Y
	constexpr float bill_board_size = 200.0f;

	//���S�ʒu
	constexpr float center_pos_x = 0.5f;
	constexpr float center_pos_y = 0.5f;

	//�L�[�摜�̃T�C�Y
	constexpr float key_graph_size = 0.8f;

	//UI�摜�̍����̕␳
	constexpr int str_UI_correction_heignt = 55;
	constexpr float UI_correction_height = 40.0f;
	constexpr float up_UI_correction_height = 90.0f;

	//UImenu��Y���W�̏����ʒu
	constexpr float UI_menu_init_pos_y = 120.0f;

	//UImenu�̌��ԃT�C�Y
	constexpr float UI_menu_gap_size = 40.0f;

	//�X�|�b�g���C�g�̊O���̊p�x
	constexpr float spot_light_out_angle = 90.0f;

	//�X�|�b�g���C�g�̓����̊p�x
	constexpr float spot_light_in_angle = 30.0f;

	//�X�|�b�g���C�g�̌����͂��͈�
	constexpr float stop_light_range = 600.0f;

	//�����֌W�Ȃ���������p�����[�^�[
	constexpr float attenuation_regardless_of_distance = 0.0f;

	//�����ɔ�Ⴕ�Č�������p�����[�^�[
	constexpr float attenuation_proportional_to_distance = 0.003f;

	//������2��ɔ�Ⴕ�Č�������p�����[�^�[
	constexpr float attenuation_proportional_to_the_square_of_the_distance = 0.0f;

	//�J�����̏����ʒu
	constexpr VECTOR camera_init_pos = { 0, 140, 0 };

	//�ʏ펞�̃J�����̏�x�N�g��
	constexpr VECTOR normal_up_vec = { 0,1,0 };

	//�{�����鎞�̏�x�N�g��
	constexpr VECTOR book_up_vec = { 0,0,1 };

	//�f�B���N�V�������C�g�̊p�x�^�C�v1
	constexpr VECTOR direction_light_diagonally_lower_left = { -33.0f,-33.0f ,-33.0f };

	//�f�B���N�V�������C�g�̊p�x�^�C�v2
	constexpr VECTOR direction_light_diagonally_lower_right = { 33.0f,-33.0f ,33.0f };

	//�f�B���N�V�������C�g�̐F
	constexpr COLOR_F direction_light_color = { 0.41f, 0.41f, 0.41f, 1.0f };

	//�X�|�b�g���C�g�̐F
	constexpr COLOR_F spot_light_color = { 0.87f, 0.72f, 0.52f, 1.0f };
}

//�R���X�g���N�^
SceneTitle::SceneTitle(SceneManager& manager): SceneBase(manager)
{
	//�Z�k��
	auto& file  = ExternalFile::GetInstance();
	auto& model = ModelManager::GetInstance();

	//�C���X�^���X��
	UI_					= std::make_shared<UIItemManager>();
	objManager_			= std::make_shared<ObjectManager>();
	subPlayerModel_		= std::make_shared<Model>(model.GetModelHandle(objData_[static_cast<int>(ObjectType::Player)].name),Material::Other);
	mainPlayerModel_	= std::make_shared<Model>(model.GetModelHandle(objData_[static_cast<int>(ObjectType::Player)].name),Material::Other);
	camera_				= std::make_shared<Camera>(file.GetCameraTargetPos("start"),file.GetCameraTargetPos("startTargetPos"));
	lightBulb_			= std::make_shared<LightBulb>(model.GetModelHandle(objData_[static_cast<int>(ObjectType::LightBulb)].name), file.GetMainSpecialObjectInfo("LightBulb"));

	//�v���C���[�̃��f���̏�����
	PlayerModelInit();

	//���C�g�̐ݒ�
	LightSetting();

	//�J�����̔z�u���̐ݒ�
	CameraPositionDataSetting();

	//�I�u�W�F�N�g�̐���
	objManager_->OpeningStageObjectGenerator();

	//UI�̐ݒ�
	UISetting();
}

//�f�X�g���N�^
SceneTitle::~SceneTitle()
{
	DeleteLightHandle(spotLightHandle_);
	DeleteLightHandle(directionLightHandle_);
}

//������
void SceneTitle::Init()
{
	updateFunc_ = &SceneTitle::FadeInUpdate;

	oldSelectScene_ = currentSelectScene_;
	currentSelectScene_ = SelectScene::NewGame;

	CameraTargetUpdate();

	fadeTimer_ = 0;
	fadeValue_ = 0;
}

//�I��
void SceneTitle::End()
{
}

//�X�V
void SceneTitle::Update()
{
	//�J�����̍X�V
	camera_->EasingMoveCamera();

	//�d���̍X�V
	lightBulb_->Update();

	//���C�g�̍X�V
	SetLightDirectionHandle(spotLightHandle_,lightBulb_->GetRotVec());
	SetLightPositionHandle (spotLightHandle_,lightBulb_->GetFramePos());

	//���C�g�̕����ݒ�
	SetLightDirectionHandle(directionLightHandle_, directionLightDir_);
	
	(this->*updateFunc_)();
}

//�`��
void SceneTitle::Draw()
{
	//�Z�k��
	auto& input = InputState::GetInstance();
	auto& file = ExternalFile::GetInstance();

	//�J�����̏�����
	camera_->Init(camera_init_pos);

	//�I�u�W�F�N�g�̕`��
	objManager_->Draw();

	//�d���̕`��
	SetUseLighting(false);
	lightBulb_->Draw();
	SetUseLighting(true);

	//���f���̕`��
	mainPlayerModel_->Draw();
	subPlayerModel_->Draw();

	//UI�̕`��
	UI_->DrawBillBoard(menuDrawPos_,UIfadeValue_, bill_board_size);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, UIfadeValue_);

	if (input.LastInputDevice()) 
	{
		//�L�[�摜�`��
		input.DrawKeyGraph(static_cast<int>(InputType::Up)		, Game::screen_width / screen_split_num * up_split_num		, Game::screen_height - up_UI_correction_height, key_graph_size);
		input.DrawKeyGraph(static_cast<int>(InputType::Down)	, Game::screen_width / screen_split_num * left_split_num	, Game::screen_height - UI_correction_height, key_graph_size);
		input.DrawKeyGraph(static_cast<int>(InputType::Left)	, Game::screen_width / screen_split_num * down_split_num	, Game::screen_height - UI_correction_height, key_graph_size);
		input.DrawKeyGraph(static_cast<int>(InputType::Right)	, Game::screen_width / screen_split_num * right_split_num	, Game::screen_height - UI_correction_height, key_graph_size);
		input.DrawKeyGraph(static_cast<int>(InputType::Space)	, Game::screen_width / screen_split_num * decision_split_num, Game::screen_height - UI_correction_height, key_graph_size);
	}
	else 
	{
		input.DrawPadGraph(static_cast<int>(InputType::Up)		, Game::screen_width / screen_split_num * up_split_num		, Game::screen_height - up_UI_correction_height, key_graph_size);
		input.DrawPadGraph(static_cast<int>(InputType::Down)	, Game::screen_width / screen_split_num * left_split_num	, Game::screen_height - UI_correction_height, key_graph_size);
		input.DrawPadGraph(static_cast<int>(InputType::Left)	, Game::screen_width / screen_split_num * down_split_num	, Game::screen_height - UI_correction_height, key_graph_size);
		input.DrawPadGraph(static_cast<int>(InputType::Right)	, Game::screen_width / screen_split_num * right_split_num	, Game::screen_height - UI_correction_height, key_graph_size);
		input.DrawPadGraph(static_cast<int>(InputType::Space)	, Game::screen_width / screen_split_num * decision_split_num, Game::screen_height - UI_correction_height, key_graph_size);
	}	

	//�L�[�̖�����̕�����̕`��
	DrawStringToHandle(Game::screen_width / screen_split_num * move_str_split_num, Game::screen_height - str_UI_correction_heignt, "�ړ�", 0xffffff, fontHandle_);
	input.DrawName(static_cast<int>(InputType::Space), Game::screen_width / screen_split_num * decision_str_split_num, Game::screen_height - str_UI_correction_heignt, 0xffffff, fontHandle_, true, true, "/");

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//fadeValue_�̒l�ɂ���ē��ߋ���ω�����^�C�g���̕`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, UIfadeValue_);
	DrawBillboard3D(menuDrawPos_["�^�C�g��"], center_pos_x, center_pos_y, title_bill_board_size, 0.0f, GraphManager::GetInstance().GetGraph("title"), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//�����t�F�[�h�pbox�̕`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawBox(0, 0, Game::screen_width, Game::screen_height, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

//�v���C���[�̃��f���̏�����
void SceneTitle::PlayerModelInit()
{
	//�Z�k��
	auto& file = ExternalFile::GetInstance();

	//���C���v���C���[���f���̔z�u�f�[�^���Z�b�g����
	auto mainPlayerInfo = file.GetMainSpecialObjectInfo("MainPlayer");
	mainPlayerModel_->SetScale(mainPlayerInfo.scale);
	mainPlayerModel_->SetPos(mainPlayerInfo.pos);
	mainPlayerModel_->SetRot(mainPlayerInfo.rot);
	mainPlayerModel_->SetAnimation(static_cast<int>(PlayerAnimType::StopTimer), false, true);

	//�T�u�v���C���[���f���̔z�u�f�[�^���Z�b�g����
	auto subPlayerInfo = file.GetMainSpecialObjectInfo("SubPlayer");
	subPlayerModel_->SetScale(subPlayerInfo.scale);
	subPlayerModel_->SetPos(subPlayerInfo.pos);
	subPlayerModel_->SetRot(subPlayerInfo.rot);
	subPlayerModel_->SetAnimation(static_cast<int>(PlayerAnimType::Jump), false, true);
}

//UI�̐ݒ�
void SceneTitle::UISetting()
{
	//�Z�k��
	auto& file = ExternalFile::GetInstance();

	//UI��\��������W���擾
	menuDrawPos_["SE"]				= file.GetUIPos("SEUIPos");
	menuDrawPos_["BGM"]				= file.GetUIPos("BGMUIPos");
	menuDrawPos_["�I��"]			= file.GetUIPos("endDrawPos");
	menuDrawPos_["�ݒ�"]			= file.GetUIPos("settingDrawPos");
	menuDrawPos_["�߂�"]			= file.GetUIPos("backUIPos");
	menuDrawPos_["���[�h"]			= file.GetUIPos("windowModeUIPos");
	menuDrawPos_["�^�C�g��"]		= file.GetUIPos("titleDrawPos");
	menuDrawPos_["����ݒ�"]		= file.GetUIPos("advancedSettingUIPos");
	menuDrawPos_["�V�[���I��"]		= file.GetUIPos("continueDrawPos");
	menuDrawPos_["�j���[�Q�[��"]	= file.GetUIPos("startDrawPos");

	//UI������̍쐬
	menuName_.push_back("�j���[�Q�[��");
	menuName_.push_back("�V�[���I��");
	menuName_.push_back("�ݒ�");
	menuName_.push_back("�I��");

	//UI�摜�̍쐬
	fontHandle_ = FontsManager::GetInstance().GetFontHandle("�s�O�� 0033");

	//UImenu�̍쐬
	float y = UI_menu_init_pos_y;
	for (const auto& menu : menuName_)
	{
		UI_->AddMenu(static_cast<float>(Game::screen_width / screen_half), static_cast<float>(Game::screen_height / screen_half) + y, UI_pos_x, UI_pos_y, menu.c_str(), fontHandle_);
		y += UI_menu_gap_size;
	}
}

//���C�g�̐ݒ�
void SceneTitle::LightSetting()
{
	SetUseLighting(true);

	//���C�g�̃f�B�t���[�Y�J���[�̐ݒ�
	SetLightDifColor(direction_light_color);

	//�f�B���N�V�������C�g�p�x�擾
	VECTOR dir = direction_light_diagonally_lower_left;
	dir = MathUtil::VECTORDegreeToRadian(dir);

	//�f�B���N�V�������C�g�̍쐬
	directionLightHandle_ = CreateDirLightHandle(dir);
	directionLightDir_ = dir;
	SetLightDifColorHandle(directionLightHandle_, direction_light_color);

	//�X�|�b�g���C�g�̍쐬
	outAngle_ = MathUtil::DegreeToRadian(spot_light_out_angle);
	inAngle_ = MathUtil::DegreeToRadian(spot_light_in_angle);

	spotLightHandle_ = CreateSpotLightHandle(lightBulb_->GetFramePos(), lightBulb_->GetRotVec(),
											 outAngle_, inAngle_,
											 stop_light_range, 
											 attenuation_regardless_of_distance,
											 attenuation_proportional_to_distance,
											 attenuation_proportional_to_the_square_of_the_distance);

	SetLightDifColorHandle(spotLightHandle_, spot_light_color);
}


//�J�����̐ݒ�
void SceneTitle::CameraPositionDataSetting()
{
	//�Z�k��
	auto& file = ExternalFile::GetInstance();

	//�J�����̏�����
	camera_->Init(camera_init_pos);

	//�J�����̃|�W�V�����ƌ���ʒu�̐ݒ�
	camera_->SetCameraTargetPosAndView(file.GetCameraTargetPos("start"), file.GetCameraTargetPos("startTargetPos"), normal_up_vec);

	//�J�������̐ݒ�
	CameraInfo info = {};

	//�ݒ�I�u�W�F�N�g������J����
	info.targetPos = file.GetCameraTargetPos("setting");
	info.targetView = file.GetCameraTargetPos("settingTargetPos");
	info.upVec = normal_up_vec;

	cameraInfo_.push_back(info);

	//�x�b�h������J����
	info.targetPos = file.GetCameraTargetPos("start");
	info.targetView = file.GetCameraTargetPos("startTargetPos");
	info.upVec = normal_up_vec;

	cameraInfo_.push_back(info);

	//�{������J����
	info.targetPos = file.GetCameraTargetPos("continue");
	info.targetView = file.GetCameraTargetPos("continueTargetPos");
	info.upVec = book_up_vec;

	cameraInfo_.push_back(info);

	//��������J����
	info.targetPos = file.GetCameraTargetPos("end");
	info.targetView = file.GetCameraTargetPos("endTargetPos");
	info.upVec = normal_up_vec;

	cameraInfo_.push_back(info);
}

//�J�����̖ڕW���W�̍X�V
void SceneTitle::CameraTargetUpdate()
{
	//�J�����̖ڕW���W�̍X�V
	camera_->SetCameraTargetPosAndView(	cameraInfo_[static_cast<int>(currentSelectScene_)].targetPos,
										cameraInfo_[static_cast<int>(currentSelectScene_)].targetView,
										cameraInfo_[static_cast<int>(currentSelectScene_)].upVec);
	//�o�ߎ��Ԃ̃��Z�b�g
	camera_->ResetElapsedTime();
}

//�I��ԍ��̍X�V
void SceneTitle::SelectSceneUpdate()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	//�L�[�������ꂽ��
	bool isTriggerUp	= input.IsTriggered(InputType::Up);
	bool isTriggerDown	= input.IsTriggered(InputType::Down);
	bool isTriggerLeft	= input.IsTriggered(InputType::Left);
	bool isTriggerRight = input.IsTriggered(InputType::Right);

	//�w��̕����Ɉړ��\��
	bool canMoveUp		= selectData_[static_cast<int>(currentSelectScene_)].up;
	bool canMoveDown	= selectData_[static_cast<int>(currentSelectScene_)].down;
	bool canMoveLeft	= selectData_[static_cast<int>(currentSelectScene_)].left;
	bool canMoveRight	= selectData_[static_cast<int>(currentSelectScene_)].rignt;

	//��
	if (isTriggerUp && canMoveUp)
	{
		oldSelectScene_ = currentSelectScene_;
		currentSelectScene_ = selectData_[static_cast<int>(currentSelectScene_)].upScene;
		CameraTargetUpdate();
	}
	//��
	else if (isTriggerDown && canMoveDown)
	{
		oldSelectScene_ = currentSelectScene_;
		currentSelectScene_ = selectData_[static_cast<int>(currentSelectScene_)].downScene;
		CameraTargetUpdate();
	}
	//��
	else if (isTriggerLeft && canMoveLeft)
	{
		oldSelectScene_ = currentSelectScene_;
		currentSelectScene_ = selectData_[static_cast<int>(currentSelectScene_)].leftScene;
		CameraTargetUpdate();
	}
	//�E
	else if (isTriggerRight && canMoveRight)
	{
		oldSelectScene_ = currentSelectScene_;
		currentSelectScene_ = selectData_[static_cast<int>(currentSelectScene_)].rigntScene;
		CameraTargetUpdate();
	}
}

//�t�F�[�h�C���̍X�V
void SceneTitle::FadeInUpdate()
{
	//�t�F�[�h�C��
	float timer = (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_));
	UIfadeValue_ = static_cast <int>(max_fade_value * timer);
	if (++fadeTimer_ == fadeInterval_)
	{
		updateFunc_ = &SceneTitle::UIUpdate;
		UIfadeValue_ = max_fade_value;
		return;
	}
}

//UI�̍X�V
void SceneTitle::UIUpdate()
{
	//�Z�k��
	auto& file = ExternalFile::GetInstance();
	auto& input = InputState::GetInstance();

	if (camera_->GetMoving()) 
	{
		return;
	}

	if (oldSelectScene_ != currentSelectScene_)
	{
		SceneChange();
		oldSelectScene_ = currentSelectScene_;
	}

	//�I��
	SelectSceneUpdate();

	if (input.IsTriggered(InputType::Space))
	{
		NewGameOrGameEndChange();
	}
}

//�I�[�v�j���O�̍X�V
void SceneTitle::OpeningUpdate()
{
	//�Z�k��
	auto& sound = SoundManager::GetInstance();

	//�A�j���[�V�����ԍ��`�F�b�N
	bool checkWakeUpAnim = PlayerAnimType::WakeUp == static_cast<PlayerAnimType>(mainPlayerModel_->GetCurrentAnimNo());
	bool checkStopTimerAnim = PlayerAnimType::StopTimer == static_cast<PlayerAnimType>(mainPlayerModel_->GetCurrentAnimNo());
	bool checkStopTimerCancelAnim = PlayerAnimType::StopTimerCancel == static_cast<PlayerAnimType>(mainPlayerModel_->GetCurrentAnimNo());

	//���f���̍X�V
	mainPlayerModel_->Update();

	//���v���Ƃ߂�A�j���[�V�����̌�����ɖ߂�
	if (mainPlayerModel_->IsAnimEnd() && checkStopTimerAnim)
	{
		sound.StopSE("alarm");
		sound.PlaySE("stopAlarm");
		mainPlayerModel_->ChangeAnimation(static_cast<int>(PlayerAnimType::StopTimerCancel), false, false, anim_change_time);
	}

	//�����ɂȂ�A�j���[�V�������I�������
	if (mainPlayerModel_->IsAnimEnd() && checkStopTimerCancelAnim)
	{
		mainPlayerModel_->ChangeAnimation(static_cast<int>(PlayerAnimType::WakeUp), false, false, anim_change_time);
	}

	//�N���オ��A�j���[�V�������I�������
	if (mainPlayerModel_->IsAnimEnd() && checkWakeUpAnim)
	{
		updateFunc_ = &SceneTitle::SceneTitleFadeOutUpdate;
	}
}

//UI�̃t�F�[�h�A�E�g
void SceneTitle::UIFadeOutUpdate()
{
	//UI�̃t�F�[�h�A�E�g
	UIfadeValue_ = static_cast <int>(max_fade_value * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0)
	{
		UIfadeValue_ = 0;
		updateFunc_ = &SceneTitle::OpeningUpdate;
		SoundManager::GetInstance().PlaySE("alarm");
		return;
	}
}

//�^�C�g���̃t�F�[�h�A�E�g
void SceneTitle::SceneTitleFadeOutUpdate()
{
	//�t�F�[�h�A�E�g
	fadeValue_ = static_cast <int>(max_fade_value * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (++fadeTimer_ == fadeInterval_)
	{
		manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<GameMain>(manager_)));
		return;
	}
}

//�V�[����؂�ւ���
void SceneTitle::SceneChange()
{
	if (currentSelectScene_ == SelectScene::Setting)
	{
		Init();
		manager_.PushFrontScene(std::shared_ptr<SceneBase>(std::make_shared<SettingSceneForSceneTitle>(manager_)));
		return;
	}

	if (currentSelectScene_ == SelectScene::SelectChapter)
	{
		Init();
		manager_.PushFrontScene(std::shared_ptr<SceneBase>(std::make_shared<SelectChapterScene>(manager_,objManager_->GetSpecificModel(ObjectType::Book))));
		return;
	}
}

//�j���[�Q�[�����̓Q�[���I�[�o�[���I�����ꂽ���̕ύX
void SceneTitle::NewGameOrGameEndChange()
{

	if (currentSelectScene_ == SelectScene::NewGame)
	{
		updateFunc_ = &SceneTitle::UIFadeOutUpdate;
	}

	if (currentSelectScene_ == SelectScene::End)
	{
		manager_.SetEndFlag(true);
		return;
	}
}
