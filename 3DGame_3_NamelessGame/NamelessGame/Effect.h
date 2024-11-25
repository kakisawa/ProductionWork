#pragma once
#include <vector>
#include "DxLib.h"
#include <map>

class ModelBase;

class Effect
{
public:

	// エフェクトの種類
	enum class EffectKind {
		kKnife,			// ナイフ
		kHandGun,		// ハンドガン
		kMachineGun,	// マシンガン

	};

public:

	Effect();
	~Effect();

	void Init();
	void Update();
	void Draw();

	void ClearEffect();

	void AddEffect(const EffectKind kind,VECTOR pos);

private:

	

	struct effect {
		int m_handle;			// エフェクト素材
		VECTOR m_pos;			// 座標
		int m_time;				// 再生時間
		int m_scele;			// 拡大率
		int m_speed;			// 再生速度
		float m_rota;			// 回転率
	};
	
	std::map<EffectKind,effect> m_effect;
};

