#pragma once
#include "SceneStageBase.h"
#include <vector>

class Player;
class Camera;
class Stage;
class EnemyBase;
class CharacterBase;

/// <summary>
/// ステージ1
/// </summary>
class SceneStage1 : public SceneStageBase
{
public:
	SceneStage1():m_isTuto(false),m_tutoNum(0) {};
	SceneStage1(std::shared_ptr<Player> pPlayer, std::shared_ptr<Camera> pCamera, std::shared_ptr<Stage> pStage);
	virtual  ~SceneStage1();
	virtual void Init() override;
	virtual std::shared_ptr<SceneBase> Update(Input& input) override;
	virtual void Draw() override;
	void IsSetTuto(bool isTuto) { m_isTuto = isTuto; } // チュートリアル状態を変更

private:
	void UpdateSound();				// サウンド更新
	void UpdateTuto(Input& input);	// チュートリアルの表示状態を更新する
	void DrawTutorial();			// チュートリアル表示

private:
	int m_tutoNum;					// 現在のチュートリアル数
	bool m_isTuto;					// チュートリアルを表示するか(true:表示する)
	std::string m_tutoText;			// チュートリアル時に表示するテキスト
	std::vector<int> m_tutoHandle;	// チュートリアル時に表示する画像

	// チュートリアル画像
	enum TutoHandle
	{
		kTuto0,
		kTuto1,
		kTuto2,
		kTuto3,
		kTuto4,
		kTuto5,
		kTutoNum,		// チュートリアルの数
		kTextBox,		// テキストボックス
		kTutoHandleNum,	// 画像の数
	};
};