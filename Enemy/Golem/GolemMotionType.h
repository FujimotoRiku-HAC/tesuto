#ifndef GOLEM_MOTION_TYPE_H_
#define GOLEM_MOTION_TYPE_H_

enum GolemMotion {
	MotionGolemIdle = 7,						//アイドル
	MotionGolemForwardWalk = 4,					//前歩き
	MotionGolemLeftWalk = 9,					//左歩き
	MotionGolemRightWalk = 11,					//右歩き
	MotionGolemBackwardWalk = 0,				//後ろ歩き
	MotionGolemJump = 8,						//ジャンプ
	MotionGolemTripleAttack = 13,				//3連攻撃
	MotionGolemForAttack = 18,					//溜め攻撃
	MotionGolemJumpingAttack = 14,				//ジャンプ攻撃
	MotionGolemWildDanceRotatingAttack = 15,	//乱舞回転斬り
	MotionGolemChargeSpinAttack = 17,			//溜め回転斬り
	MotionGolemChargeDanceAttack = 19,			//溜め乱舞
	MotionGolemPushAttack = 12,					//吹き飛ばし攻撃
	MotionGolemGenerate = 10,					//出現状態
	MotionGolemStanStart = 3,					//スタンスタート
	MotionGolemStanLoop = 2,					//スタンループ
	MotionGolemStanEnd = 1,						//スタンエンド
	MotionGolemDie = 5,							//死亡
};

#endif