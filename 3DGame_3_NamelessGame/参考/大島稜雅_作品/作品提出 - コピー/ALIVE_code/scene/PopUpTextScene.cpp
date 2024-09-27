#include "PopUpTextScene.h"
#include "SceneManager.h"
#include "ScenePause.h"

#include "../util/game.h"
#include "../util/InputState.h"

#include "../util/FontsManager.h"

namespace
{
	//����
	constexpr int half = 2;

	//�w�i�̃A���t�@�l
	constexpr int background_alpha_value = 150;
}

//�R���X�g���N�^
PopUpTextScene::PopUpTextScene(SceneManager& manager):SceneBase(manager)
{
}

//�f�X�g���N�^
PopUpTextScene::~PopUpTextScene()
{
}

//������
void PopUpTextScene::Init()
{
}

//�I��
void PopUpTextScene::End()
{
}

//�X�V
void PopUpTextScene::Update()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	if (input.IsTriggered(InputType::Space))
	{
		manager_.PopFrontScene();
	}
}

//�`��
void PopUpTextScene::Draw()
{
	//�Z�k��
	auto& font = FontsManager::GetInstance();

	//�w�i�̕`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, background_alpha_value);
	DrawBox(0, 0, Game::screen_width, Game::screen_height, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//������̕`��
	std::string str = "�ύX���܂���";

	int width = GetDrawStringWidthToHandle(str.c_str(), static_cast<int>(str.size()), font.GetFontHandle("�s�O�� 0042"));

	DrawStringToHandle(Game::screen_width / half - width / half, Game::screen_height / half, str.c_str(), 0xffffff, font.GetFontHandle("�s�O�� 0042"));
}
