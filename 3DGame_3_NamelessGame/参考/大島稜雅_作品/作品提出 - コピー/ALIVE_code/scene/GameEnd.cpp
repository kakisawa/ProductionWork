#include "GameEnd.h"
#include "SceneTitle.h"
#include "SceneManager.h"

#include "../object/Camera.h"
#include "../object/objectManager.h"

#include "../util/game.h"
#include "../util/Util.h"
#include "../util/Model.h"
#include "../util/InputState.h"
#include "../util/ModelManager.h"
#include "../util/ExternalFile.h"
#include "../util/FontsManager.h"
#include "../util/SoundManager.h"

#include <algorithm>

namespace 
{
	//����
	constexpr int half = 2;

	//�ߋ�5��̋L�^���c����1/5�͈Ⴄ���������邽��4��ɂ���
	constexpr int old_result_num = 4;

	//�␳�l
	constexpr int correction = 3;

	//����A�j���[�V�����̑�����炷�t���[����
	constexpr int run_anim_foot_step_frame_type_1 = 6;
	constexpr int run_anim_foot_step_frame_type_2 = 45;

	//���ʂ����������̃A�j���[�V�����t���[����
	constexpr int front_anim_frame = 30;

	//�i�s���������������̃A�j���[�V�����t���[����
	constexpr int direction_of_travel_anim_frame = 40;

	//���̓���Œu�����̂̐�
	constexpr int number_placed_in_first_order_action = 5;

	//�A�j���[�V������ύX����̂ɂ����鎞��
	constexpr int anim_change_time = 10;

	//�t�F�[�h�̍ő�l
	constexpr int max_fade_value = 255;

	//�F��ς���}�e���A���̔ԍ�
	constexpr int change_material_num = 8;

	//�_�O���t�̉��̃T�C�Y
	constexpr int bar_graph_width = 20;

	//�_�O���t�̏c�̃T�C�Y
	constexpr int bar_graph_height = 10;

	//���̕�����
	constexpr int division_width = 10;

	//�c�̕�����
	constexpr int division_height = 15;

	//�_�O���t��1�������̃T�C�Y
	constexpr int bar_graph_1_memory_size = 12;

	//������̕`��ʒu�̍����␳�l
	constexpr int str_height_correction = 15;

	//������̕`��ʒu�̉��̕␳�l
	constexpr int str_width_correction = 10;

	//���C�g�̊O���̊p�x
	constexpr float light_outer_angle = 130.0f;

	//���C�g�̓����̊p�x
	constexpr float light_inner_angle = 60.0f;

	//���C�g�͈̔�
	constexpr float light_range = 500.0f;

	//�����֌W�Ȃ���������p�����[�^�[
	constexpr float attenuation_regardless_of_distance = 0.0f;

	//�����ɔ�Ⴕ�Č�������p�����[�^�[
	constexpr float attenuation_proportional_to_distance = 0.0009f;

	//������2��ɔ�Ⴕ�Č�������p�����[�^�[
	constexpr float attenuation_proportional_to_the_square_of_the_distance = 0.0f;

	//������������͈�
	constexpr float sound_fange = 800.0f;

	//���̂̍����̊Ԋu
	constexpr float corpse_height_gap = 15.0f;

	//���̂ɐ��ʂ���������p�x
	constexpr float corpse_front_angle = -90.0f;

	//���̂�u���ꏊ�̕␳�l
	constexpr float corpse_put_pos = 50.0f;

	//�����J�����|�W�V����
	constexpr VECTOR camera_init_pos = { 265, 314, -803 };

	//�����J�����̃^�[�Q�b�g�|�W�V����
	constexpr VECTOR camera_init_target_pos = { 265, 314, 1777 };

	//�X�|�b�g���C�g�̃|�W�V����
	constexpr VECTOR spot_light_pos = { 235.0f,650.0f,-432.0f };

	//���C�g�̕���
	constexpr VECTOR light_dir = { 0.0f, -1.0f, 0.0f };

	//���̂̐F
	constexpr COLOR_F corpse_color = { 1.0f, 0.0f, 0.0f, 1.0f };
}

//�R���X�g���N�^
GameEnd::GameEnd(SceneManager& manager) : SceneBase(manager),updateFunc_(&GameEnd::FadeInUpdate)
{
	//�Z�k��
	auto& model = ModelManager::GetInstance();
	auto& file = ExternalFile::GetInstance();
	auto& font = FontsManager::GetInstance();
	auto& sound = SoundManager::GetInstance();

	//�C���X�^���X��
	objManager_ = std::make_shared<ObjectManager>();
	camera_ = std::make_shared<Camera>(camera_init_pos, camera_init_target_pos);
	playerModel_ = std::make_shared<Model>(model.GetModelHandle(objData_[static_cast<int>(ObjectType::Player)].name), Material::Other);
	boardModel_  = std::make_shared<Model>(model.GetModelHandle(objData_[static_cast<int>(ObjectType::WhiteBoard)].name), Material::Iron);

	//���f���̐ݒ�
	ModelSetting();

	//�I�u�W�F�N�g�̐���
	objManager_->EndStageObjectGenerator();

	//�_�O���t�̏�������
	for (int i = 0; i < file.GetTotalDeathNum().size(); i++)
	{
		barGraphHeight_.push_back(Game::screen_height / division_height * bar_graph_1_memory_size);
	}

	//���̂�u���ꏊ����ʂ𕪊����Č��߂�
	//��ʂ̕������̎擾
	divisionNum_ = file.GetTotalDeathNum().back() / static_cast<int>(file.GetTotalDeathNum().size()) + 1;
	if (file.GetTotalDeathNum().back() % file.GetTotalDeathNum().size() > 0)
	{
		divisionNum_++;
	}

	//�t�H���g�n���h���̎擾
	pigumo21FontHandle_ = font.GetFontHandle("�s�O�� 0021");
	pigumo42FontHandle_ = font.GetFontHandle("�s�O�� 0042");

	//�O�p���ʓx�@�ɕϊ�����
	float outerAngle = MathUtil::DegreeToRadian(light_outer_angle);

	//���p���ʓx�@�ɕϊ�����
	float innerAngle = MathUtil::DegreeToRadian(light_inner_angle);

	//�X�|�b�g���C�g�n���h���̍쐬
	lightHandle_ = CreateSpotLightHandle(spot_light_pos, light_dir,
										 outerAngle, innerAngle,
										 light_range,
										 attenuation_regardless_of_distance,
										 attenuation_proportional_to_distance,
										 attenuation_proportional_to_the_square_of_the_distance);

	//3D�T�E���h���X�i�[�̏���ݒ肷��
	sound.Set3DSoundListenerInfo(camera_->GetPos(), camera_->GetTarget());
}

//�f�X�g���N�^
GameEnd::~GameEnd()
{
}

//������
void GameEnd::Init()
{
}

//�I��
void GameEnd::End()
{
}

//�X�V
void GameEnd::Update()
{
	(this->*updateFunc_)();
}

//�`��
void GameEnd::Draw()
{
	//�Z�k��
	auto& file = ExternalFile::GetInstance();

	camera_->Init(camera_init_target_pos);

	//�v���C���[���f���̕`��
	playerModel_->Draw();

	//�z���C�g�{�[�h�̕`��
	boardModel_->Draw();

	//���̃��f���̕`��
	for (const auto& corpse : corpseModel_)
	{
		corpse.second->Draw();
	}

	//�I�u�W�F�N�g�̕`��
	objManager_->Draw();

	//�v���C���[�̍��W���X�N���[�����W�ɂ���
	VECTOR screenPos = ConvWorldPosToScreenPos(boardModel_->GetPos());

	//int�^�̃X�N���[�����W
	int castScreenPosX = static_cast<int>(screenPos.x);

	//�F
	int color = 0x0000ff;

	for (int i = 0; i < file.GetTotalDeathNum().size();i++)
	{

		barGraphHeight_[i] = (std::max)(barGraphHeight_[i] - 1, Game::screen_height / division_height * bar_graph_1_memory_size - file.GetTotalDeathNum()[i] * bar_graph_height);

		if (static_cast<int>(file.GetTotalDeathNum().size() - 1) - i == 0) 
		{
			color = 0xff0000;
		}

		//�_�O���t�̕`��
		DrawBox(Game::screen_width / division_width * (i + correction) - bar_graph_width - (Game::screen_width / half - castScreenPosX),
				barGraphHeight_[i],
				Game::screen_width / division_width * (i + correction) + bar_graph_width - (Game::screen_width / half - castScreenPosX),
				Game::screen_height / division_height * bar_graph_1_memory_size,
				color, true);

		//�t�H���g��K�p����������̃T�C�Y���擾
		int size = GetDrawFormatStringWidthToHandle(pigumo42FontHandle_,"%d��", file.GetTotalDeathNum()[i]);

		//�����̕`��
		DrawFormatStringToHandle(Game::screen_width / division_width * (i + correction) - size / half - (Game::screen_width / half - castScreenPosX),
								 Game::screen_height / division_height * bar_graph_1_memory_size - file.GetTotalDeathNum()[i] * bar_graph_height - size,
								 0x000000, pigumo42FontHandle_, "%d��", file.GetTotalDeathNum()[i]);

		if (old_result_num - i == 0)
		{
			DrawVStringToHandle(Game::screen_width / division_width * (i + correction) - str_width_correction - (Game::screen_width / half - castScreenPosX),
								Game::screen_height / division_height * bar_graph_1_memory_size + str_height_correction,
								"����", 0xff0000, pigumo21FontHandle_);
			continue;
		}

		DrawFormatVStringToHandle(Game::screen_width / division_width * (i + correction) - str_width_correction - (Game::screen_width / half - castScreenPosX),
								  Game::screen_height / division_height * bar_graph_1_memory_size + str_height_correction,
								  0x000000,
								  pigumo21FontHandle_,
								  "%d��O", old_result_num - i);
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	//��ʑS�̂�^�����ɓh��Ԃ�
	DrawBox(0, 0, Game::screen_width, Game::screen_height, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

//���f���̐ݒ�
void GameEnd::ModelSetting()
{
	//�Z�k��
	auto& file = ExternalFile::GetInstance();
	auto& model = ModelManager::GetInstance();

	//���f���̐ݒ�
	auto playerInfo = file.GetEndSpecifiedInfo("player");
	playerModel_->SetPos(playerInfo.pos);
	playerModel_->SetRot(playerInfo.rot);
	playerModel_->SetScale(playerInfo.scale);
	//�A�j���[�V�����̕ύX
	playerModel_->SetAnimation(static_cast<int>(PlayerAnimType::Run), true, true);

	//���f���̐ݒ�
	auto boardInfo = file.GetEndSpecifiedInfo("WhiteBoard");
	boardModel_->SetPos(boardInfo.pos);
	boardModel_->SetRot(boardInfo.rot);
	boardModel_->SetScale(boardInfo.scale);

	//�v���C���[�̎�̍��W���擾
	VECTOR pos = playerModel_->GetFrameLocalPosition("hand.R_end");
	float height = 0.0f;

	//���̃��f���̐���
	for (int i = 0; i < file.GetTotalDeathNum().back(); i++)
	{
		corpseModel_.push_back(std::make_pair(false, std::make_shared<Model>(model.GetModelHandle(objData_[static_cast<int>(ObjectType::Player)].name), Material::Other)));
		corpseModel_[i].second->SetPos({ pos.x,pos.y + height,pos.z });
		corpseModel_[i].second->SetRot(playerInfo.rot);
		corpseModel_[i].second->SetScale(playerInfo.scale);
		corpseModel_[i].second->SetAnimEndFrame(static_cast<int>(PlayerAnimType::Death));
		//�}�e���A���̐F��ς���
		MV1SetMaterialDifColor(corpseModel_[i].second->GetModelHandle(), change_material_num, corpse_color);
		height += corpse_height_gap;
	}
}

//���̂̍X�V
void GameEnd::CorpseUpdate(const VECTOR& playerPos)
{
	//���̂̍����̕␳
	float height = 0.0f;

	//���̂̃��f������]������
	if (isTurn_) 
	{
		for (int i = 0; i < corpseModel_.size(); i++) 
		{
			if (corpseModel_[i].first)
			{
				continue;
			}
			VECTOR rot = {};
			rot.y = MathUtil::DegreeToRadian(0);
			corpseModel_[i].second->SetRot(rot);
		}
	}

	//�A�j���[�V�������I��������A�����Ɏ��̂�u��
	if (playerModel_->IsAnimEnd()) 
	{
		for (int i = corpseNum_; i < corpseNum_ + number_placed_in_first_order_action; i++)
		{
			//���݂̔ԍ������̂̑����𒴂�����ȍ~�̏������s��Ȃ�
			if (i > static_cast<int>(corpseModel_.size() - 1)) 
			{
				continue;
			}

			//���̂̍��W���v���C���[�̍��W�ɐݒ肷��
			corpseModel_[i].second->SetPos({ playerPos.x,playerPos.y + height, playerPos.z });

			//�t���O��true�ɂ���
			corpseModel_[i].first = true;

			//������ύX����
			height += corpse_height_gap;
		}

		//�܂��u����Ă��Ȃ����̂̃��f������]������
		for (int i = 0; i < corpseModel_.size(); i++) 
		{
			if (corpseModel_[i].first) 
			{
				continue;
			}

			VECTOR rot = {};
			rot.y = MathUtil::DegreeToRadian(corpse_front_angle);
			corpseModel_[i].second->SetRot(rot);
		}

		//���̂�5�����ɒu������A�܂��茳�Ɏ��̂��c���Ă�����
		//���삷�鎀�̂̔ԍ��͈͂�ς���
		if ((static_cast<int>(corpseModel_.size()) - 1 - corpseNum_ + 1) / number_placed_in_first_order_action > 0)
		{
			corpseNum_ += number_placed_in_first_order_action;
		}

		//�t���O��܂�
		isPutting_ = false;
	}

	//�v���C���[���f���̎�̍��W���擾
	VECTOR pos = playerModel_->GetFrameLocalPosition("hand.R_end");
	height = 0.0f;

	//���̃��f���̍��W��ύX
	for (int i = 0; i < corpseModel_.size(); i++)
	{
		if (corpseModel_[i].first) 
		{
			continue;
		}

		corpseModel_[i].second->SetPos({ pos.x,pos.y + height,pos.z });
		height += corpse_height_gap;
	}
}

//�z���C�g�{�[�h�̍X�V
void GameEnd::WhiteBoardUpdate()
{
	//�Z�k��
	auto& sound = SoundManager::GetInstance();

	if (isWentToRightSide_) 
	{
		return;
	}

	if (!isResultDisplaying_)
	{
		boardIsMoving_ = true;
		boardModel_->SetPos(VAdd(boardModel_->GetPos(), moveVec_));
		if (!sound.CheckSoundFile("pull")) 
		{
			sound.PlaySE("pull");
		}
	}

	//�v���C���[�̍��W���X�N���[�����W�ɂ���
	VECTOR screenPos = ConvWorldPosToScreenPos(boardModel_->GetPos());

	//int�^�̃X�N���[�����W
	int castScreenPosX = static_cast<int>(screenPos.x);

	//��ʂ̔����̃T�C�Y(��)
	int screenWidthHalf = Game::screen_width / half;

	if (castScreenPosX >= screenWidthHalf - 1 && castScreenPosX <= screenWidthHalf + 1)
	{
		boardIsMoving_ = false;
		isResultDisplaying_ = true;
		playerModel_->ChangeAnimation(static_cast<int>(PlayerAnimType::Idle), true, false, anim_change_time);
	}

	//�z���C�g�{�[�h����ʊO�ɏo����t�F�[�h�A�E�g����
	if (castScreenPosX < -Game::screen_width / half)
	{
		updateFunc_ = &GameEnd::FadeOutUpdate;
	}
}

//�t�F�[�h�C���̍X�V
void GameEnd::FadeInUpdate()
{
	fadeValue_ = static_cast <int>(max_fade_value * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0)
	{
		updateFunc_ = &GameEnd::NormalUpdate;
		fadeValue_ = 0;
	}
}

//�ʏ펞�̍X�V
void GameEnd::NormalUpdate()
{
	//�Z�k��
	auto& input = InputState::GetInstance();
	auto& file = ExternalFile::GetInstance();
	auto& sound = SoundManager::GetInstance();

	//�v���C���[���f���̍��W
	VECTOR playerPos = playerModel_->GetPos();

	//�v���C���[���f���̍X�V
	playerModel_->Update();

	//�v���C���[�̍��W���X�N���[�����W�ɂ���
	VECTOR screenPos = ConvWorldPosToScreenPos(playerModel_->GetPos());

	//�v���C���[����ʊO�ɍs�����瓮���Ă���t���O��false�ɂ���
	if (screenPos.x > Game::screen_width && isWentToRightSide_) 
	{
		isWentToRightSide_ = false;
		moveVec_.x = -1;
		playerModel_->SetPos(file.GetEndSpecifiedInfo("secondPos").pos);
		playerModel_->ChangeAnimation(static_cast<int>(PlayerAnimType::Pull), true, false, anim_change_time);
		playerPos = playerModel_->GetPos();
	}

	if (!isPutting_ && !isResultDisplaying_) 
	{
		//�v���C���[���f���̍��W�ύX
		playerModel_->SetPos(VAdd(playerPos, moveVec_));
		if (playerModel_->GetSpecifiedAnimTime(run_anim_foot_step_frame_type_1) || playerModel_->GetSpecifiedAnimTime(run_anim_foot_step_frame_type_2))
		{
			sound.Set3DSoundInfo(playerPos, sound_fange, "asphaltStep");
			sound.PlaySE("asphaltStep");
		}
	}

	//�z���C�g�{�[�h�̍X�V
	WhiteBoardUpdate();

	if (input.IsTriggered(InputType::Space) && isResultDisplaying_) 
	{
		isResultDisplaying_ = false;
	}

	if (!isWentToRightSide_)
	{
		return;
	}

	//��ʂ𕪊�������̈������̉��̒���
	int divisionPosX = Game::screen_width / divisionNum_;

	//�v���C���[�̍��W�����Ɏ擾�����X�N���[�����W��
	//��L�̃T�C�Y�ŗ]������߂�
	int errorRange = static_cast<int>(screenPos.x) % (divisionPosX * currentDivisionNum_);

	if (screenPos.x > corpse_put_pos && screenPos.x < Game::screen_width - corpse_put_pos)
	{
		//�덷�͈͓���������A�v���C���[�̃A�j���[�V������ύX����
		if (errorRange >= -1 && errorRange <= 1 && !isPutting_) 
		{
			//�A�j���[�V������ύX����
			playerModel_->ChangeAnimation(static_cast<int>(PlayerAnimType::Put), false, true, anim_change_time);

			//�t���O�𗧂Ă�
			isPutting_ = true;

			currentDivisionNum_++;
		}
	}

	//�v���C���[������̃A�j���[�V�����t���[���ɂȂ�����t���O�𗧂Ă�
	if (isPutting_) 
	{
		if (playerModel_->GetSpecifiedAnimTime(front_anim_frame))
		{
			isTurn_ = true;
		}
		else if (playerModel_->GetSpecifiedAnimTime(direction_of_travel_anim_frame))
		{
			isTurn_ = false;
		}
	}

	//���̂̍X�V
	CorpseUpdate(playerPos);

	if (!isPutting_)
	{
		//�A�j���[�V������ύX����
		playerModel_->ChangeAnimation(static_cast<int>(PlayerAnimType::Run), true, false, anim_change_time);
	}
}

//�t�F�[�h�A�E�g�̍X�V
void GameEnd::FadeOutUpdate()
{
	fadeValue_ = static_cast <int>(max_fade_value * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (++fadeTimer_ == fadeInterval_) 
	{
		manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<SceneTitle>(manager_)));
		return;
	}
}
