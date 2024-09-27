#pragma once

enum class PlayerAnimType {
	Idle,			//待機状態
	Walk,			//歩く
	Run,			//走る
	Jump,			//ジャンプ
	Crank,			//クランクを回す
	Death,			//死ぬ
	IdleToSitup,	//idle状態から座る
	SitupToIdle,	//座っている状態からidle
	LeverOn,		//レバーを起動する
	WakeUp,			//起き上がる
	Throw,			//投げる
	StopTimer,		//タイマーをとめる
	StopTimerCancel,//タイマーを止める行動をとめる
	Put,			//物を置く
	Pull,			//物を引く
	max
};

//計　20byte
struct PlayerInfo {
	float jumpPower;			//4byte
	float runningJumpPower;		//4byte
	float rotSpeed;				//4byte
	float walkSpeed;			//4byte
	float runningSpeed;			//4byte
};