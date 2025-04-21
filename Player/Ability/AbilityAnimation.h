#ifndef ABILITY_ANIMATION_H_
#define ABILITY_ANIMATION_H_

#include "AbilityType.h"

class AbilityAnimation {
public:

	//更新
	void Update(float delta_time);
	//必殺技使用
	void UseAbility(IWorld* world, int ability);

public:

	//各クールタイム取得
	float GetTimer(int ability)const;

public:

	//アビリティ取得
	bool GetAbility(int ability)const;

private:

	//必殺技ごとのクールタイム
	void BlowUpdate(float delta_time);
	void PushUpdate(float delta_time);
	void AvoidAttackUpdate(float delta_time);
	void RotatingUpdate(float delta_time);
	void InpactUpdate(float delta_time);
	void SlamUpdate(float delta_time);
	void WildDanceUpdate(float delta_time);
	void WaveWildDanceUpdate(float delta_time);
	void FangsUpdate(float delta_time);

private:

	//使ったか
	bool m_IsBlowAttack = false;
	bool m_IsPushAttack = false;
	bool m_IsAvoidAttack = false;
	bool m_IsRotateAttack = false;
	bool m_IsInpactAttack = false;
	bool m_IsSlamAttack = false;
	bool m_IsWildDanceAttack = false;
	bool m_IsWaveAttack = false;
	bool m_IsFangsAttack = false;

	//クールタイム
	float m_BlowCoolTime = 0.0f;
	float m_PushCoolTime = 0.0f;
	float m_AvoidCoolTime = 0.0f;
	float m_RotateCoolTime = 0.0f;
	float m_InpactCoolTime = 0.0f;
	float m_SlamCoolTime = 0.0f;
	float m_WildDanceCoolTime = 0.0f;
	float m_WaveCoolTime = 0.0f;
	float m_FangsCoolTime = 0.0f;
};

#endif