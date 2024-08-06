#pragma once
class Animation
{
public:
	Animation();
	~Animation();

	virtual void Init();
	virtual void Update(int model);
	virtual void Draw();

public:
	// アニメーションの制御

	/// <summary>
	/// アニメーションを設定する(すぐに切り替える)
	/// </summary>
	/// <param name="animNo">変更先アニメーション番号</param>
	/// <param name="isLoop">アニメーションをループさせるか</param>
	/// <param name="isForceChange">既に指定されたアニメーションが再生されている場合も変更するか</param>
	void SetAnim(int model,int animNo, bool isLoop, bool isForceChange);


	/// <summary>
	/// アニメーション変更させる
	/// </summary>
	/// <param name="animNo">変更先アニメーション番号</param>
	/// <param name="isLoop">ループ再生させるか</param>
	/// <param name="isForceChange">既に指定されたアニメーションが再生されている場合も再生するか</param>
	/// <param name="isChangeFrame">何フレームかけて切り替える</param>
	void ChangeAnim(int model,int animNo, bool isLoop, bool isForceChange, bool isChangeFrame);

	// 現在のアニメーションが終了しているかどうかを取得する(Loopアニメの場合は取得できない = falseを返す)
	bool IsAnimEnd(int model);

private:
	// アニメーション情報
	struct AnimData			// アニメーションデータ
	{
		int animNo;			// アニメーション番号
		int attachNo;		// アタッチ番号
		float totalTime;	// アニメーションの総再生時間
		float elapsedTime;	// アニメーションの経過時間
		bool isLoop;		// アニメーションがループするか
	};

protected:
	// アニメーション状態の初期化
	void InitAnim(AnimData& anim);

	// アニメーションの更新
	void UpdateAnimation(int model, AnimData anim, float dt = 1.0f);

	// 現在のアニメーション切り替わり情報からアニメーションのブレンド率を設定する
	void UpdateAnimBlendRate(int model);

private:
	// アニメーションの切り替え情報
	int m_animChangeFrame;			// 現在の切り替えフレーム数
	int m_animChangeFrameTotal;		// 切り替えにかける総フレーム数

	float m_animBlendRate;		// 現在と過去のアニメーションの合成割合	(フレーム)
									// 0.0f:prevが再生される
									// 1.0f:currentが再生される

	float m_animSpeed;		// アニメーション変更速度

	// アニメーション情報
	AnimData m_current;		// 変更後アニメーションデータ
	AnimData m_prev;		// 変更前アニメーションデータ

};

