#ifndef STONE_SOLDIER_MOTION_TYPE_H_
#define STONE_SOLDIER_MOTION_TYPE_H_

enum StoneSoldierMotion {
	StoneSoldierSpawn = 27,					//スポーン
	StoneSoldierIdle = 29,					//アイドル
	StoneSoldierForwardWalk = 32,			//前歩き
	StoneSoldierBackwardWalk = 31,			//後ろ歩き
	StoneSoldierBeforePounceAttack = 6,		//とびかかり攻撃前
	StoneSoldierBeforePeripheralAttack = 5,	//周辺攻撃前
	StoneSoldierPounceAttack = 0,			//とびかかり攻撃
	StoneSoldierPeripheralAttack = 2,		//周辺攻撃
	StoneSoldierFrontHit = 20,				//通常ヒット
	StoneSoldierFrontStrongHit = 28,		//強攻撃ヒット
	StoneSoldierFrontBlowHit = 16,			//ブローヒット
	StoneSoldierFrontPushUpHit = 12,		//プッシュヒット
	StoneSoldierBackHit = 19,				//後ろ通常ヒット
	StoneSoldierBackStrongHit = 26,			//後ろ強攻撃ヒット
	StoneSoldierBackBlowHit = 18,			//後ろブローヒット
	StoneSoldierBackPushUpHit = 10,			//後ろプッシュ攻撃
	StoneSoldierGetUp = 21,					//起き上がり
	StoneSoldierGetUpAfterPushUp = 35,		//プッシュ攻撃後の起き上がり
	StoneSoldierLeftThink = 34,				//左歩き
	StoneSoldierRightThink = 33,			//右歩き
	StoneSoldierBeforeDie = 36,				//死亡前
	StoneSoldierDie = 1,					//死亡
};

#endif