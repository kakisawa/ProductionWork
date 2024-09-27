#include "DxLib.h"
#include "CharacterBase.h"
#include "LoadData.h"
#include <fstream>
#include <sstream>
#include <cassert>

// 定数
namespace
{
	const char* const kCharStatusFileName = "data/csv/charaStatus.csv";	// キャラクターデータのファイル名
	const char* const kAnimSpeedFileName = "data/csv/animSpeed.csv";	// アニメーション速度データのファイル名
	const char* const kColFileName = "data/csv/collision.csv";			// 当たり判定データのファイル名
	const char* const kEnemyInfoFileName = "data/csv/enemyInfo.csv";	// 敵データのファイル名

	constexpr int kStatusNum = 20;		// 1キャラクターのステータス数
	constexpr int kAnimNum = 17;		// 1キャラクターのアニメーションの数
	constexpr int kColNum = 16;			// 1キャラクターの当たり判定の情報数
	constexpr int kEnemyInfoNum = 14;	// 1キャラクターの情報数
}


/// <summary>
/// 引数つきコンストラクタ
/// </summary>
/// <param name="data">キャラクター参照</param>
/// <param name="charType">キャラクターの種類</param>
LoadData::LoadData(CharacterBase& data, int charType)
{
	LoadCharaData(data, charType);
	LoadAnimSpeedData(data, charType);
	LoadColData(data, charType);
	if (charType != 0)
	{
		LoadEnemyData(data, charType);
	}

}


/// <summary>
/// デストラクタ
/// </summary>
LoadData::~LoadData()
{
}


/// <summary>
/// 描画
/// </summary>
void LoadData::Draw()
{
#ifdef _DEBUG
	// 読み込んだデータを表示する
	int y = 0;
	for (const auto& data : m_charData)
	{
		DrawFormatString(0, 100 + y, 0xffffff, "%f:", data);
		y += 20;
	}
#endif
}


/// <summary>
/// キャラクターのデータを読み込む
/// </summary>
/// <param name="data">キャラクター参照</param>
/// <param name="charCharType">キャラクターの種類</param>
void LoadData::LoadCharaData(CharacterBase& data, int charType)
{
	std::ifstream file(kCharStatusFileName);
	m_charData.clear();
	std::string line;

	// ファイルの入力取得
	// std::getline(読み取るファイルの変数, 入力文字列を格納する変数);
	while (std::getline(file, line))
	{
		std::istringstream stream(line);
		std::string field;

		// 文字列分割
		// getline(istringstream型の変数, 分割した文字列を格納する変数, ',で分割')
		while (getline(stream, field, ','))
		{
			try
			{
				// 文字列をfloatに変換して追加する
				m_charData.push_back(std::stof(field));
			}
			catch (const std::invalid_argument&)
			{
				// 無効な文字列をスキップ
			}
		}
	}

	// 外部ファイルの情報を入れる
	data.m_status.maxHp = m_charData[0 + charType * kStatusNum];
	data.m_status.punchReceptionTime = static_cast<int>(m_charData[1 + charType * kStatusNum]);
	data.m_status.punchCoolTime = static_cast<int>(m_charData[2 + charType * kStatusNum]);
	data.m_status.kickCoolTime = static_cast<int>(m_charData[3 + charType * kStatusNum]);
	data.m_status.punchTime = static_cast<int>(m_charData[4 + charType * kStatusNum]);
	data.m_status.kickTime = static_cast<int>(m_charData[5 + charType * kStatusNum]);
	data.m_status.punchPower = m_charData[6 + charType * kStatusNum];
	data.m_status.secondPunchPower = m_charData[7 + charType * kStatusNum];
	data.m_status.thirdPunchPower = m_charData[8 + charType * kStatusNum];
	data.m_status.kickPower = m_charData[9 + charType * kStatusNum];
	data.m_status.specialAttackPower = m_charData[10 + charType * kStatusNum];
	data.m_status.maxMoveSpeed = m_charData[11 + charType * kStatusNum];
	data.m_status.fightWalkSpeed = m_charData[12 + charType * kStatusNum];
	data.m_status.acceleration = m_charData[13 + charType * kStatusNum];
	data.m_status.deceleration = m_charData[14 + charType * kStatusNum];
	data.m_status.avoidDist = m_charData[15 + charType * kStatusNum];
	data.m_status.maxAvoidCount = static_cast<int>(m_charData[16 + charType * kStatusNum]);
	data.m_status.avoidCoolTime = static_cast<int>(m_charData[17 + charType * kStatusNum]);
	data.m_status.guardAnimTime = m_charData[18 + charType * kStatusNum];
	data.m_status.backMove = m_charData[19 + charType * kStatusNum];
}


/// <summary>
/// アニメーション速度のデータを読み込む
/// </summary>
/// <param name="data">キャラクター参照</param>
/// <param name="charType">キャラクターの種類</param>
void LoadData::LoadAnimSpeedData(CharacterBase& data, int charType)
{
	std::ifstream file(kAnimSpeedFileName);
	m_animSpeedData.clear();
	std::string line;

	// ファイルの入力取得
	// std::getline(読み取るファイルの変数, 入力文字列を格納する変数);
	while (std::getline(file, line))
	{
		std::istringstream stream(line);
		std::string field;

		// 文字列分割
		// getline(istringstream型の変数, 分割した文字列を格納する変数, ',で分割')
		while (getline(stream, field, ','))
		{
			try
			{
				// 文字列をfloatに変換してm_dataに追加する
				m_animSpeedData.push_back(std::stof(field));
			}
			catch (const std::invalid_argument&)
			{
				// 無効な文字列をスキップ
			}
		}
	}

	// 外部ファイルの情報を入れる
	data.m_animSpeed.none = m_animSpeedData[0+ charType * kAnimNum];
	data.m_animSpeed.avoid = m_animSpeedData[1 + charType * kAnimNum];
	data.m_animSpeed.down = m_animSpeedData[2 + charType * kAnimNum];
	data.m_animSpeed.fightIdle = m_animSpeedData[3 + charType * kAnimNum];
	data.m_animSpeed.fightWalk = m_animSpeedData[4 + charType * kAnimNum];
	data.m_animSpeed.gettingUp = m_animSpeedData[5 + charType * kAnimNum];
	data.m_animSpeed.grab = m_animSpeedData[6 + charType * kAnimNum];
	data.m_animSpeed.guard = m_animSpeedData[7 + charType * kAnimNum];
	data.m_animSpeed.kick = m_animSpeedData[8 + charType * kAnimNum];
	data.m_animSpeed.punch1 = m_animSpeedData[9 + charType * kAnimNum];
	data.m_animSpeed.punch2 = m_animSpeedData[10 + charType * kAnimNum];
	data.m_animSpeed.punch3 = m_animSpeedData[11 + charType * kAnimNum];
	data.m_animSpeed.specialAttack = m_animSpeedData[12 + charType * kAnimNum];
	data.m_animSpeed.receive = m_animSpeedData[13 + charType * kAnimNum];
	data.m_animSpeed.run = m_animSpeedData[14 + charType * kAnimNum];
	data.m_animSpeed.stand = m_animSpeedData[15 + charType * kAnimNum];
	data.m_animSpeed.Stumble = m_animSpeedData[16 + charType * kAnimNum];
}


/// <summary>
/// 当たり判定情報を読み込む
/// </summary>
/// <param name="data">キャラクター参照</param>
/// <param name="charType">キャラクターの種類</param>
void LoadData::LoadColData(CharacterBase& data, int charType)
{
	std::ifstream file(kColFileName);
	m_colData.clear();
	std::string line;

	// ファイルの入力取得
	// std::getline(読み取るファイルの変数, 入力文字列を格納する変数);
	while (std::getline(file, line))
	{
		std::istringstream stream(line);
		std::string field;

		// 文字列分割
		// getline(istringstream型の変数, 分割した文字列を格納する変数, ',で分割')
		while (getline(stream, field, ','))
		{
			try
			{
				// 文字列をfloatに変換してm_dataに追加する
				m_colData.push_back(std::stof(field));
			}
			catch (const std::invalid_argument&)
			{
				// 無効な文字列をスキップ
			}
		}
	}

	// 外部ファイルの情報を入れる
	data.m_colInfo.bodyHeight = m_colData[0 + charType * kColNum];
	data.m_colInfo.bodyRadius = m_colData[1 + charType * kColNum];
	data.m_colInfo.aimRadius = m_colData[2 + charType * kColNum];
	data.m_colInfo.legRadius = m_colData[3 + charType * kColNum];
	data.m_colInfo.armStartPos.x = m_colData[4 + charType * kColNum];
	data.m_colInfo.armStartPos.y = m_colData[5 + charType * kColNum];
	data.m_colInfo.armStartPos.z = m_colData[6 + charType * kColNum];
	data.m_colInfo.armEndPos.x = m_colData[7 + charType * kColNum];
	data.m_colInfo.armEndPos.y = m_colData[8 + charType * kColNum];
	data.m_colInfo.armEndPos.z = m_colData[9 + charType * kColNum];
	data.m_colInfo.legStartPos.x = m_colData[10 + charType * kColNum];
	data.m_colInfo.legStartPos.y = m_colData[11 + charType * kColNum];
	data.m_colInfo.legStartPos.z = m_colData[12 + charType * kColNum];
	data.m_colInfo.legEndPos.x = m_colData[13 + charType * kColNum];
	data.m_colInfo.legEndPos.y = m_colData[14 + charType * kColNum];
	data.m_colInfo.legEndPos.z = m_colData[15 + charType * kColNum];
}


/// <summary>
/// 敵情報を読み込む
/// </summary>
/// <param name="data">敵参照</param>
void LoadData::LoadEnemyData(CharacterBase& data, int charType)
{
	std::ifstream file(kEnemyInfoFileName);
	m_enemyData.clear();
	std::string line;

	// ファイルの入力取得
	// std::getline(読み取るファイルの変数, 入力文字列を格納する変数);
	while (std::getline(file, line))
	{
		std::istringstream stream(line);
		std::string field;

		// 文字列分割
		// getline(istringstream型の変数, 分割した文字列を格納する変数, ',で分割')
		while (getline(stream, field, ','))
		{
			try
			{
				// 文字列をfloatに変換してm_dataに追加する
				m_enemyData.push_back(std::stof(field));
			
			}
			catch (const std::invalid_argument&)
			{
				// 無効な文字列をスキップ
			}
		}
	}

	// 外部ファイルの情報を入れる
	// MEMO:敵番号が1番から始まるため、charType-1をする
	data.m_enemyInfo.approachRange = m_enemyData[0 + (charType - 1) * kEnemyInfoNum];
	data.m_enemyInfo.attackRange = m_enemyData[1 + (charType - 1) * kEnemyInfoNum];
	data.m_enemyInfo.minStopTime = static_cast<int>(m_enemyData[2 + (charType - 1) * kEnemyInfoNum]);
	data.m_enemyInfo.maxStopTime = static_cast<int>(m_enemyData[3 + (charType - 1) * kEnemyInfoNum]);
	data.m_enemyInfo.maxProb = static_cast<int>(m_enemyData[4 + (charType - 1) * kEnemyInfoNum]);
	data.m_enemyInfo.punchProb = static_cast<int>(m_enemyData[5 + (charType - 1) * kEnemyInfoNum]);
	data.m_enemyInfo.kickProb = static_cast<int>(m_enemyData[6 + (charType - 1) * kEnemyInfoNum]);
	data.m_enemyInfo.avoidProb = static_cast<int>(m_enemyData[7 + (charType - 1) * kEnemyInfoNum]);
	data.m_enemyInfo.guardProb = static_cast<int>(m_enemyData[8 + (charType - 1) * kEnemyInfoNum]);
	data.m_enemyInfo.grabProb = static_cast<int>(m_enemyData[9 + (charType - 1) * kEnemyInfoNum]);
	data.m_enemyInfo.changeAngleProb = static_cast<int>(m_enemyData[10 + (charType - 1) * kEnemyInfoNum]);
	data.m_enemyInfo.changeAngleFrame = static_cast<int>(m_enemyData[11 + (charType - 1) * kEnemyInfoNum]);
	data.m_enemyInfo.guardTime = static_cast<int>(m_enemyData[12 + (charType - 1) * kEnemyInfoNum]);
	data.m_enemyInfo.stateIntervalTime = static_cast<int>(m_enemyData[13 + (charType - 1) * kEnemyInfoNum]);
}
