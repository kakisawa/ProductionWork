#include "SceneOption.h"
#include "DxLib.h"
#include "SceneTitle.h"
#include "../Util/Pad.h"
#include "../Util/Fade.h"
#include <cassert>

namespace
{
	const char* const kUI[6]{
		"data/UI/OptionScene/�����w�i.png",		// �Z���N�g�w�i
		"data/UI/OptionScene/������.png",	// ������
		"data/UI/OptionScene/Bg.jpg",
		"data/UI/OptionScene/BGM.png",
		"data/UI/OptionScene/SE.png",
		"data/UI/OptionScene/�^�C�g���ɖ߂�.png",
	};

	// ���̏㉺�ړ���
	constexpr int kSelectMoveY = 230;

	constexpr float kScreenWidth = 1920.0f;
	constexpr float kScreenHeight = 1080.0f;

	// ���̔�
	constexpr float UnderBoxX = (kScreenWidth * 0.1f) * 1.4f;
	constexpr float UnderBoxY = (kScreenHeight * 0.5f) * 0.5f;
	// ���̔��̒���
	constexpr float UnderBoxWidth = (kScreenWidth * 0.8f) * 0.897f;
	constexpr float UnderBoxHeight = (kScreenHeight * 0.8f) * 0.1f;
	// ��̔�
	constexpr float UpBoxX = UnderBoxX;
	constexpr float UpBoxY = UnderBoxY;
	// ��̔��̒���
	constexpr float UpBoxWidth = UnderBoxWidth;
	constexpr float UpBoxHeight = UnderBoxHeight;
	// �^�C�g���ɖ߂�Box�ʒu
	constexpr float BackBoxX = UnderBoxX * 2.3f;
	constexpr float BackBoxY = UnderBoxY + (kSelectMoveY * 2);

	const VECTOR kSelectPos[6]{
		(VGet(UpBoxX,UnderBoxY,0.0f)),
		(VGet(UpBoxX + UpBoxWidth,UnderBoxHeight + UnderBoxY,0.0f)),
		(VGet(UpBoxX,UnderBoxY+ kSelectMoveY,0.0f)),
		(VGet(UpBoxX + UpBoxWidth,UpBoxY + UpBoxHeight+kSelectMoveY,0.0f)),
		(VGet(BackBoxX,BackBoxY,0.0f)),
		(VGet(BackBoxX + UnderBoxWidth * 0.5f,BackBoxY+ UnderBoxHeight,0.0f))
	};

	// BGM�����ʒu
	constexpr float BgmStringX = UnderBoxX;
	constexpr float BgmStringY = UnderBoxY - 80;
	// SE�����ʒu
	constexpr float SeStringX = UnderBoxX;
	constexpr float SeStringY = BgmStringY + kSelectMoveY;
	// �^�C�g���ɖ߂镶���ʒu
	constexpr float BackStringX = BackBoxX + 110;
	constexpr float BackStringY = BackBoxY + 10;

	constexpr float kBgScroll1 = 1.5f;

	constexpr int kExplanationBgPosY = 1003;
	constexpr int kExplanationPosY = 1015;
}

SceneOption::SceneOption() :
	m_scrollX_1(0.0f),
	m_select(Select::kBgmVolume)
{
}

SceneOption::~SceneOption()
{
}

void SceneOption::Init()
{
	// UI�摜�̓ǂݍ���
	for (int i = 0; i < m_uiGraph.size(); i++) {
		m_uiGraph[i] = LoadGraph(kUI[i]);
		assert(m_uiGraph[i] != -1);
	}

	m_pFade = std::make_shared<Fade>();

	m_isNextSceneFlag = false;

	m_pSound->Init();
	m_pSound->InitSound();	// �T�E���h�̏�����
	m_pSound->LoadBGM(SoundManager::BGM_Type::kOptionBGM);	// �T�E���h�̓ǂݍ���
	m_pSound->LoadSE(SoundManager::SE_Type::kSelectSE);
	m_pSound->LoadSE(SoundManager::SE_Type::kButtonSE);

	m_pSound->PlayBGM(SoundManager::BGM_Type::kOptionBGM, DX_PLAYTYPE_LOOP);

	m_select = kBgmVolume;
}

shared_ptr<SceneBase> SceneOption::Update()
{
	m_pFade->FadeIn(m_pFade->GatFadeInFlag());

	// �w�i�X�N���[��
	m_scrollX_1 += kBgScroll1;

	m_pSound->SelectOption();

	m_pFade->FadeOut(m_pFade->GatFadeOutFlag());
	m_isNextSceneFlag = m_pFade->GatNextSceneFlag();

	if (m_isNextSceneFlag)
	{
		return make_shared<SceneTitle>();	// �^�C�g���V�[���֍s��
	}

	if (m_pSound->GetSoundSelect() == m_pSound->Select::kBack && Pad::IsTrigger(PAD_INPUT_R))		// RB�{�^��
	{
		m_pFade->SetFadeOutFlag(true);
		m_pSound->PlaySE(SoundManager::SE_Type::kButtonSE, DX_PLAYTYPE_BACK);
	}
	

	m_pSound->ChangeSound();

	if (Pad::IsTrigger(PAD_INPUT_DOWN))
	{
		m_select += 1;
		if (m_select >= 3)
		{
			m_select = Select::kBgmVolume;
		}
	}
	if (Pad::IsTrigger(PAD_INPUT_UP))
	{
		m_select -= 1;
		if (m_select <= -1)
		{
			m_select = Select::kBack;
		}
	}

	if (m_select == Select::kBgmVolume)
	{
		c1 = { kSelectPos[0],kSelectPos[1] };
	}
	if (m_select == Select::kSeVolume)
	{
		c1 = { kSelectPos[2],kSelectPos[3] };
	}
	if (m_select == Select::kBack)
	{
		c1 = { kSelectPos[4],kSelectPos[5] };
	}

#ifdef _DEBUG
#endif // DEBUG

	return shared_from_this();
}

void SceneOption::Draw()
{
	DrawExtendGraph(0, 0, 1920, 1080, m_uiGraph[2], false);
	DrawBox(250, 150, 1670, 880, 0x000000, true);
	DrawSelect();
	DrawExplanation();

	m_pFade->Draw();
#ifdef _DEBUG
	DrawString(0, 0, "SceneOption", 0xffffff);
	DrawString(0, 20, "Please Press Button START", 0x00ffff);
	DrawFormatString(0, 40, 0xffffff, "select=%d", m_pSound->GetSoundSelect());
#endif // DEBUG
}

void SceneOption::End()
{
	// UI�摜�̍폜
	for (int i = 0; i < m_uiGraph.size(); i++) {
		DeleteGraph(m_uiGraph[i]);
	}

	SceneBase::End();
}

void SceneOption::DrawSelect()
{
	DrawBoxAA(kSelectPos[0].x, kSelectPos[0].y,
		kSelectPos[1].x - UpBoxWidth + m_pSound->GetBgmVolume() * 5.4f, kSelectPos[1].y,
		0x0095d9, true, 2.0f);
	DrawBoxAA(kSelectPos[2].x, kSelectPos[2].y,
		kSelectPos[3].x - UpBoxWidth + m_pSound->GetSeVolume() * 5.4f, kSelectPos[3].y,
		0x0095d9, true, 2.0f);
	DrawBox(kSelectPos[4].x, kSelectPos[4].y,
		kSelectPos[5].x, kSelectPos[5].y,
		0xff00ff, false);

	for (int i = 0; i < 2; i++)
	{
		DrawBoxAA(UnderBoxX, UnderBoxY + (kSelectMoveY * i),
			UnderBoxX + UnderBoxWidth, UnderBoxY + UnderBoxHeight + (kSelectMoveY * i),
			0xFFFFFF, false, 2.0f);
	}
	DrawBoxAA(kSelectPos[4].x, kSelectPos[4].y,
		kSelectPos[5].x, kSelectPos[5].y,
		0xffffff, false, 3.0f);

	DrawBoxAA(c1.m_selectBox1.x - 3, c1.m_selectBox1.y - 3,
		c1.m_selectBox2.x + 3, c1.m_selectBox2.y + 3,
		0xff0000, false, 3.0f);

	DrawGraph(BgmStringX, BgmStringY, m_uiGraph[3],true);
	DrawGraph(SeStringX, SeStringY, m_uiGraph[4], true);
	DrawGraph(BackStringX, BackStringY, m_uiGraph[5], true);
}

void SceneOption::DrawExplanation()
{
	// �w�i�摜�̃T�C�Y�擾
	Size expBgSize, expSize;
	GetGraphSize(m_uiGraph[0], &expBgSize.m_width, &expBgSize.m_height);
	GetGraphSize(m_uiGraph[1], &expSize.m_width, &expSize.m_height);

	// �X�N���[���ʂ��v�Z
	int scrollBg = static_cast<int>(m_scrollX_1) % static_cast<int>(expBgSize.m_width);

	// �������w�i�`��
	for (int index = 0; index < 4; index++)
	{
		DrawGraph(-scrollBg + index * expBgSize.m_width,
			static_cast<int>(kExplanationBgPosY),
			m_uiGraph[0], true);

		DrawGraph(-scrollBg + index * expSize.m_width,
			static_cast<int>(kExplanationPosY),
			m_uiGraph[1], true);
	}
}
