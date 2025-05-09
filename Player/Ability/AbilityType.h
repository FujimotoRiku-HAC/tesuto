#ifndef ABILITY_TYPE_H_
#define ABILITY_TYPE_H_

enum Ability {
	NoneAbility,
	BlowAttackAbility,			//付き飛ばし
	PushAttackAbility,			//突き上げ攻撃
	AvoidAttackAbility,			//回避しながら攻撃
	RotatingAttackAbility,		//回転斬り
	InpactGroundAttackAbility,	//周りに衝撃を与える
	SlamAttackAbility,			//空中に飛ばしてたたきつける
	WildDance,					//7連攻撃
	WaveWildDanceAbility,       //波動乱舞
	FangsAbility,				//牙突
	TotalAbilitynum
};

#endif