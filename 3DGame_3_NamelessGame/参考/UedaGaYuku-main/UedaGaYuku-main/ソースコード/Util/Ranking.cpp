#include "DxLib.h"
#include "Vec2.h"
#include "Font.h"
#include "ConversionTime.h"
#include "Ranking.h"

// �萔
namespace
{
	/*�e�L�X�g�֘A*/
	constexpr int kTextDisplayTime = 3;			// �e�L�X�g��\������Ԋu
	constexpr int kTextDisplayAnimTime = 220;	// �e�L�X�g�A�j���[�V�����̎���
	constexpr int kMaxAlpha = 255;				// �ő�A���t�@�l
	constexpr int kMinAlpha = 0;				// �ŏ��A���t�@�l
	constexpr int kTextTime = 120;				// �e�L�X�g��\������܂ł̎���

	/*�l�b�g���[�N�֘A*/
	// �h���C����
	const char* kDomainName = "rueda.zombie.jp";
	// �X�e�[�W1URI
	const char* kStage1CreateUri = "/Ranking/createRanking_stage1.php";				// �쐬
	const char* kStage1UpdateUri = "/Ranking/updateRanking_stage1.php?clearTime=";  // �X�V
	const char* kStage1GetUri = "/Ranking/getRanking_stage1.php";					// �擾
	// �X�e�[�W2URI
	const char* kStage2CreateUri = "/Ranking/createRanking_stage2.php";				// �쐬
	const char* kStage2UpdateUri = "/Ranking/updateRanking_stage2.php?clearTime=";	// �X�V
	const char* kStage2GetUri = "/Ranking/getRanking_stage2.php";					// �擾
	constexpr int kPortNum = 80;		// �|�[�g�ԍ�
	constexpr int kMaxRankNum = 10;		// �\�����郉���L���O��

	// �����L���O�\��
	constexpr float kRankingIntervalAdj = 28.0f;		// �\���ʒu����
	constexpr int kTextColor = 0xffffff;				// �e�L�X�g�̐F
	constexpr int kRankInTextColor = 0xff0000;			// �����L���O�����ɓ��������̃e�L�X�g�̐F
	constexpr float kRankingIntervalWidth = 400.0f;		// ���̕\���Ԋu
	// �X�e�[�W�I����
	const Vec2 kStageSlectRankPos = { 900, 320 };		// �\���ʒu
	constexpr float kStageSlectRankInterval = 60.0f;	// �c�̕\���Ԋu
	// �N���A��
	const Vec2 kRankingPos = { 1000.0f, 550.0f };		// �\���ʒu
	constexpr float kClearRankInterval = 80.0f;			// �c�̕\���Ԋu

}

/// <summary>
/// �R���X�g���N�^
/// </summary>
Ranking::Ranking():
	m_rankInTextDispTime(0),
	m_rankInTextAlpha(kMaxAlpha),
	m_pos(0),
	m_lineCount(0)
{
	m_rankingList.resize(kMaxRankNum);
}


/// <summary>
/// �f�X�g���N�^
/// </summary>
Ranking::~Ranking()
{
}


/// <summary>
/// �����L���O�쐬
/// </summary>
/// <param name="stageKind">�X�e�[�W�̎��</param>
void Ranking::CreateRanking(int stageKind)
{
	// �X�e�[�W�ɂ����URI��ύX����
	switch (stageKind)
	{
	case 1:
		createRank = HttpGet(kDomainName, kStage1CreateUri);
		break;
	case 2:
		createRank = HttpGet(kDomainName, kStage2CreateUri);
		break;
	}
}


/// <summary>
/// �����L���O�X�V
/// </summary>
/// <param name="stageKind">�X�e�[�W�̎��</param>
/// <param name="clearTime">�N���A�^�C��</param>
void Ranking::UpdateRanking(int stageKind, int clearTime)
{
	// �X�e�[�W�ɂ����URI��ύX����
	switch (stageKind)
	{
	case 1:
		uri = kStage1UpdateUri + std::to_string(clearTime);
		break;
	case 2:
		uri = kStage2UpdateUri + std::to_string(clearTime);
		break;
	}

	std::string getRank = HttpGet(kDomainName, uri.c_str());
}


/// <summary>
/// �����N�C�����̃e�L�X�g���X�V����
/// </summary>
void Ranking::UpdateRankInText()
{
	// �e�L�X�g�̃A���t�@�l�𒲐�����
	m_rankInTextDispTime += kTextDisplayTime;
	m_rankInTextDispTime %= kTextDisplayAnimTime;
	// MEMO:sin�g���g����0�`1�͈̔͂ɂ���
	float sinAlpha = 0.5f + 0.5f * sinf(static_cast<float>(m_rankInTextDispTime) / kTextDisplayAnimTime * DX_PI_F);
	m_rankInTextAlpha = kMinAlpha + static_cast<int>((kMaxAlpha - kMinAlpha) * sinAlpha);
}


/// <summary>
/// �����L���O�擾
/// </summary>
/// <param name="stageKind">�X�e�[�W�̎��</param>
void Ranking::GetRanking(int stageKind)
{
	// �X�e�[�W�ɂ����URI��ύX����
	switch (stageKind)
	{
	case 1:
		getRank = HttpGet(kDomainName, kStage1GetUri);
		break;
	case 2:
		getRank = HttpGet(kDomainName, kStage2GetUri);
		break;
	}

	m_lineCount = 0;  // �����L���O�J�E���g�����Z�b�g
	size_t m_pos = 0;

	// 1�ʁ`10�ʂ܂Ŏ擾����
	// MEMO:clearTime�����𒊏o����BclearTime��������Ȃ��ꍇ��npos��Ԃ�
	while (m_lineCount < kMaxRankNum && (m_pos = getRank.find("\"clearTime\":")) != std::string::npos)
	{
		m_pos += strlen("\"clearTime\":");
		int clearTime = 0;
		int numChars = 0;

		if (sscanf_s(getRank.c_str() + m_pos, "%d%n", &clearTime, &numChars) == 1)
		{
			if (m_lineCount < kMaxRankNum)
			{
				m_rankingList[m_lineCount] = clearTime;  // �����L���O���X�g�ɕۑ�
				m_lineCount++;
			}

			// ���̗v�f�ɐi��
			m_pos += numChars;				  // �ǂݎ�����������������i�߂�
			getRank = getRank.substr(m_pos);  // �c��̕�����������擾
		}
		else
		{
			break;  // sscanf_s�����s�����ꍇ�̓��[�v���I��
		}
	}
}


/// <summary>
/// �X�e�[�W�I���������L���O�\��
/// </summary>
void Ranking::DrawStageSelectRanking()
{
	m_pos = getRank.find_first_of("\r\n");

	for (int i = 0; i < m_lineCount; i++)
	{
		// �t���[��������b���ɕϊ�����
		int min = Conversion::ChangeMin(m_rankingList[i]);			 // ��
		int sec = Conversion::ChangeSec(m_rankingList[i]);			 // �b
		int milliSec = Conversion::ChangeMilliSec(m_rankingList[i]); // �~���b

		// 1�ʁ`5�ʁA6�ʁ`10�ʂ̕\���ʒu�����炷
		if (i < 5)
		{
			DrawFormatStringFToHandle(kStageSlectRankPos.x, kStageSlectRankPos.y + i * kStageSlectRankInterval,
				kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kRanking)], "%d�� %02d:%02d:%03d", (i + 1), min, sec, milliSec);
		}
		else if (i < 9)
		{
			DrawFormatStringFToHandle(kStageSlectRankPos.x + kRankingIntervalWidth, kStageSlectRankPos.y + (i - 5) * kStageSlectRankInterval,
				kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kRanking)], "%d�� %02d:%02d:%03d", (i + 1), min, sec, milliSec);
		}
		else
		{
			DrawFormatStringFToHandle(kStageSlectRankPos.x + kRankingIntervalWidth - kRankingIntervalAdj, kStageSlectRankPos.y + (i - 5) * kStageSlectRankInterval,
				kTextColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kRanking)], "%d�� %02d:%02d:%03d", (i + 1), min, sec, milliSec);
		}
	}
}


/// <summary>
/// �N���A���̃����L���O�`��
/// </summary>
void Ranking::DrawClearRanking(int totalClearTime)
{
	m_pos = getRank.find_first_of("\r\n");
	UpdateRankInText(); // �e�L�X�g�̕\�����X�V����

	for (int i = 0; i < m_lineCount; i++)
	{
		// �t���[��������b���ɕϊ�����
		const int min = Conversion::ChangeMin(m_rankingList[i]);			 // ��
		const int sec = Conversion::ChangeSec(m_rankingList[i]);			 // �b
		const int milliSec = Conversion::ChangeMilliSec(m_rankingList[i]);   // �~���b
		int textColor = kTextColor;

		// 1�ʁ`5�ʁA6�ʁ`10�ʂ̕\���ʒu�����炷
		if (i < 5)
		{
			textColor = CheckRankIn(totalClearTime, i); // �����L���O�����ɓ��������`�F�b�N����

			DrawFormatStringFToHandle(kRankingPos.x, kRankingPos.y + i * kClearRankInterval,
				textColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kRanking)], "%d�� %02d:%02d:%03d", (i + 1), min, sec, milliSec);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		else if(i < 9)
		{
			textColor = CheckRankIn(totalClearTime, i);

			DrawFormatStringFToHandle(kRankingPos.x + kRankingIntervalWidth, kRankingPos.y + (i - 5) * kClearRankInterval,
				textColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kRanking)], "%d�� %02d:%02d:%03d", (i + 1), min, sec, milliSec);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		else
		{
			textColor = CheckRankIn(totalClearTime, i);

			DrawFormatStringFToHandle(kRankingPos.x + kRankingIntervalWidth - kRankingIntervalAdj, kRankingPos.y + (i - 5) * kClearRankInterval,
				textColor, Font::m_fontHandle[static_cast<int>(Font::FontId::kRanking)], "%d�� %02d:%02d:%03d", (i + 1), min, sec, milliSec);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}
}


/// <summary>
/// �����L���O�����ɓ��������`�F�b�N����
/// </summary>
/// <param name="totalClearTime">�g�[�^���̃N���A�^�C��</param>
/// <param name="index">�m�F���鏇��</param>
/// <returns>�e�L�X�g�̐F</returns>
int Ranking::CheckRankIn(int totalClearTime, int index)
{
	// �����L���O�����ɓ������ꍇ
	if (totalClearTime == m_rankingList[index])
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_rankInTextAlpha); // �e�L�X�g�̃��l��ύX����
		return kRankInTextColor; // �e�L�X�g�̐F��ύX����
	}
	else
	{
		return kTextColor;
	}
}


/// <summary>
/// Http�ʐM��Get���߂𑗂�
/// </summary>
/// <param name="domain">�h���C����</param>
/// <param name="uri">URI</param>
/// <returns>�����L���O���擾</returns>
std::string Ranking::HttpGet(const char* domain, const char* uri)
{
	char HttpCmd[128] = ""; 	// Http�ʐM���쐬���邽�߂̕ϐ�

	// DxLib�̕s�v�ȋ@�\��off�ɂ���
	SetUseDXNetWorkProtocol(false);

	// DNS����h���C������IP�A�h���X���擾
	GetHostIPbyName(domain, &Ip);

	// �ʐM���m��
	NetHandle = ConnectNetWork(Ip, kPortNum);

	// �m�������������ꍇ�̂ݒ��̏���������
	if (NetHandle != -1)
	{
		//Http���߂̍쐬
		sprintf_s(HttpCmd, "GET %s HTTP/1.1\nHost: %s\n\n", uri, domain);
#ifdef _DEBUG
		//DrawFormatString(0, 60, 0xffffff, "HttpCmd:\n%s", HttpCmd);
#endif // _DEBUG

		// �f�[�^���M(http���߂𑗂�)
		NetWorkSend(NetHandle, HttpCmd, static_cast<int>(strlen(HttpCmd)));

		// �f�[�^������̂�҂�
		while (!ProcessMessage())
		{
			// �擾���Ă��Ȃ���M�f�[�^�ʂ𓾂�
			DataLength = GetNetWorkDataLength(NetHandle);

			// �擾���ĂȂ���M�f�[�^�ʂ�-1����Ȃ��ꍇ�̓��[�v�𔲂���
			if (DataLength != -1)
			{
				break;
			}
		}

		// �f�[�^��M
		NetWorkRecv(NetHandle, StrBuf, kDataSize);    // �f�[�^���o�b�t�@�Ɏ擾

		// �ڑ���f��
		CloseNetWork(NetHandle);
	}


#ifdef _DEBUG
	//�擾����IP�A�h���X�̊m�F
	//DrawFormatString(0, 20, 0xffffff, "IpAdress;%d,%d,%d,%d", Ip.d1, Ip.d2, Ip.d3, Ip.d4);
	//DrawFormatString(0, 40, 0xffffff, "NetHandle:%d", NetHandle);
#endif // _DEBUG

	return StrBuf;
}