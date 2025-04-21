#ifndef PLAYER_ANIM_H_
#define PLAYER_ANIM_H_

//モーション番号
enum TitlePlayerAnim {
	MotionTitle = 0
};

enum PlayerAnim {
	//コンボ3攻撃 0.1.2  ALL 3
	//コンボ4攻撃 4.5.6.7  ALL 8
	//コンボ7攻撃 9.10.11.12.13.14.15 ALL 16
	// ダッシュ攻撃40 41
	//死亡 17 21 32 84 起き上がる 18,29
	//攻撃ヒット 19 24 25 34 36 37 38 39 82 87 
	// 倒れる22 32 ローリング 42 43 44 45 アイドル 131 46 59
	//ターン　47 50 48 49 51 52 53 54 55 56 70 71 116 117 118 119 120 121 122 123
	// 124 127 128 129 130
	//  通常57 58
	//歩き 60 61 62.63 64 65 66 67 68 69 133 134 135 136 137 138 139 140 141
	// 142 143
	// 後ろ歩き 132
	//吹き飛ぶ 80 85 89 倒れてからローリング 81 83 86 88
	//ガード 90 92 93 94 95 ALL91
	//Dash  96 99 被弾時のダッシュ97 98 101 止まる 100
	// 倒れてから攻撃 115
	//Skill 102 104 105 106 107 110 111 112 114 125 126
	MotionPlayerIdle = 46,						//アイドル
	MotionFwdWalk = 65,							//前歩き
	MotionFirstAttack = 0,						//一撃目
	MotionSecondAttack = 1,						//二撃目
	MotionThirdAttack = 2,						//三撃目
	MotionHitForward = 34,						//前ヒット
	MotionHitBackward = 19,						//後ろヒット
	MotionHitLeft = 36,							//左ヒット
	MotionHitRight = 38,						//右ヒット
	MotionStrongHit = 85,						//強攻撃ヒット
	MotionGetUp = 29,							//起き上がり
	MotionDie = 21,								//死亡
	MotionDieLoop = 22,							//死亡ループ
	MotionRightAvoidance = 45,					//右回避
	MotionLeftAvoidance = 44,					//左回避
	MotionForwardAvoidance = 43,				//前回避
	MotionBackAvoidance = 42,					//後ろ回避
	MotionJustAvoidAttack = 8,					//カウンター
	MotionAbilityBlowAttack = 102,				//ブロー攻撃
	MotionAbilityBlowAttackReinforcement = 41,	//ブロー攻撃2撃目
	MotionAbilityPushAttack = 104,				//プッシュ攻撃
	MotionAbilityAvoidAttack = 105,				//回避攻撃
	MotionAbilityRotatingAttack = 106,			//回転斬り
	MotionAbilityInpactAttack = 107,			//インパクト攻撃
	MotionAbilitySlamAttack = 110,				//叩きつけ攻撃
	MotionAbilityWaveWildDance_1 = 111,			//波動攻撃
	MotionAbilityWaveWildDance_2 = 112,			//波動攻撃2撃目
	MotionAbilityWaveWildDance_3 = 114,			//波動攻撃3撃目
	MotionAbilityWildDanceAttack = 16,			//乱舞
	MotionAbilitySlashAttack = 40,				//牙突
};

#endif