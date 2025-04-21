#include "AbilityAnimation.h"
#include "Actor/Player/Player.h"
#include "Actor/Player/PlayerState/PlayerOwnedState.h"

//クールタイム
const float BlowTime{ 60 * 20 };
const float PushTime{ 60 * 25 };
const float AvoidTime{ 60 * 10 };
const float RotateTime{ 60 * 50 };
const float InpactTime{ 60 * 30 };
const float SlamTime{ 60 * 35 };
const float WildTime{ 60 * 45 };
const float WaveTime{ 60 * 40 };
const float FangsTime{ 60 * 10 };

void AbilityAnimation::UseAbility(IWorld* world, int ability){
	auto player = static_cast<Player*>(world->FindActor(st_Player));

	GSuint a;

	switch (ability)
	{
	case BlowAttackAbility:
		if (m_IsBlowAttack)return;
		a = PlayerOwnedState::AbilityBlowAttack;
		m_BlowCoolTime = BlowTime;
		m_IsBlowAttack = true;
		break;
	case PushAttackAbility:
		if (m_IsPushAttack)return;
		a = PlayerOwnedState::AbilityPushAttack;
		m_PushCoolTime = PushTime;
		m_IsPushAttack = true;
		break;
	case AvoidAttackAbility:
		if (m_IsAvoidAttack)return;
		a = PlayerOwnedState::AbilityAvoidAttack;
		m_AvoidCoolTime = AvoidTime;
		m_IsAvoidAttack = true;
		break;
	case RotatingAttackAbility:
		if (m_IsRotateAttack)return;
		a = PlayerOwnedState::AbilityRotatingAttack;
		m_RotateCoolTime = RotateTime;
		m_IsRotateAttack = true;
		break;
	case InpactGroundAttackAbility:
		if (m_IsInpactAttack)return;
		a = PlayerOwnedState::AbilityInpactAttack;
		m_InpactCoolTime = InpactTime;
		m_IsInpactAttack = true;
		break;
	case SlamAttackAbility:
		if (m_IsSlamAttack)return;
		a = PlayerOwnedState::AbilitySlamAttack;
		m_SlamCoolTime = SlamTime;
		m_IsSlamAttack = true;
		break;
	case WildDance:
		if (m_IsWildDanceAttack)return;
		a = PlayerOwnedState::AbilityWildDance;
		m_WildDanceCoolTime = WildTime;
		m_IsWildDanceAttack = true;
		break;
	case WaveWildDanceAbility:
		if (m_IsWaveAttack)return;
		a = PlayerOwnedState::AbilityWaveWildDance_1;
		m_WaveCoolTime = WaveTime;
		m_IsWaveAttack = true;
		break;
	case FangsAbility:
		if (m_IsFangsAttack)return;
		a = PlayerOwnedState::AbilitySlashAttack;
		m_FangsCoolTime = FangsTime;
		m_IsFangsAttack = true;
		break;
	default:
		break;
	}

	player->ChangeState(a);

}

float AbilityAnimation::GetTimer(int ability) const{
	switch (ability)
	{
	case BlowAttackAbility:
		return m_BlowCoolTime;
		break;
	case PushAttackAbility:
		return m_PushCoolTime;
		break;
	case AvoidAttackAbility:
		return m_AvoidCoolTime;
		break;
	case RotatingAttackAbility:
		return m_RotateCoolTime;
		break;
	case InpactGroundAttackAbility:
		return m_InpactCoolTime;
		break;
	case SlamAttackAbility:
		return m_SlamCoolTime;
		break;
	case WildDance:
		return m_WildDanceCoolTime;
		break;
	case WaveWildDanceAbility:
		return m_WaveCoolTime;
		break;
	case FangsAbility:
		return m_FangsCoolTime;
		break;
	default:
		break;
	}
	return 0.0f;
}

bool AbilityAnimation::GetAbility(int ability) const{
	switch (ability)
	{
	case BlowAttackAbility:
		return m_IsBlowAttack;
		break;
	case PushAttackAbility:
		return m_IsPushAttack;
		break;
	case AvoidAttackAbility:
		return m_IsAvoidAttack;
		break;
	case RotatingAttackAbility:
		return m_IsRotateAttack;
		break;
	case InpactGroundAttackAbility:
		return m_IsInpactAttack;
		break;
	case SlamAttackAbility:
		return m_IsSlamAttack;
		break;
	case WildDance:
		return m_IsWildDanceAttack;
		break;
	case WaveWildDanceAbility:
		return m_IsWaveAttack;
		break;
	case FangsAbility:
		return m_IsFangsAttack;
		break;
	default:
		break;
	}
	return false;
}


void AbilityAnimation::Update(float delta_time){
	BlowUpdate(delta_time);
	PushUpdate(delta_time);
	AvoidAttackUpdate(delta_time);
	RotatingUpdate(delta_time);
	InpactUpdate(delta_time);
	SlamUpdate(delta_time);
	WildDanceUpdate(delta_time);
	WaveWildDanceUpdate(delta_time);
	FangsUpdate(delta_time);

}

void AbilityAnimation::BlowUpdate(float delta_time){
	//使ってなかったらリターン
	if (!m_IsBlowAttack)return;

	m_BlowCoolTime -= delta_time;

	//クールタイムが0.0になったら使用可能
	if (m_BlowCoolTime <= 0.0f) {
		m_IsBlowAttack = false;
		gsPlaySE(Charge_SE);
		return;
	}

}

void AbilityAnimation::PushUpdate(float delta_time){
	//使ってなかったらリターン
	if (!m_IsPushAttack)return;

	m_PushCoolTime -= delta_time;

	if (m_PushCoolTime <= 0.0f) {
		m_IsPushAttack = false;
		gsPlaySE(Charge_SE);
		return;
	}
}

void AbilityAnimation::AvoidAttackUpdate(float delta_time){
	//使ってなかったらリターン
	if (!m_IsAvoidAttack)return;

	m_AvoidCoolTime -= delta_time;

	if (m_AvoidCoolTime <= 0.0f) {
		m_IsAvoidAttack = false;
		gsPlaySE(Charge_SE);
		return;
	}
}

void AbilityAnimation::RotatingUpdate(float delta_time){
	//使ってなかったらリターン
	if (!m_IsRotateAttack)return;

	m_RotateCoolTime -= delta_time;

	if (m_RotateCoolTime <= 0.0f) {
		m_IsRotateAttack = false;
		gsPlaySE(Charge_SE);
		return;
	}
}

void AbilityAnimation::InpactUpdate(float delta_time){
	//使ってなかったらリターン
	if (!m_IsInpactAttack)return;

	m_InpactCoolTime -= delta_time;

	if (m_InpactCoolTime <= 0.0f) {
		m_IsInpactAttack = false;
		gsPlaySE(Charge_SE);
		return;
	}
}

void AbilityAnimation::SlamUpdate(float delta_time){
	//使ってなかったらリターン
	if (!m_IsSlamAttack)return;

	m_SlamCoolTime -= delta_time;

	if (m_SlamCoolTime <= 0.0f) {
		m_IsSlamAttack = false;
		gsPlaySE(Charge_SE);
		return;
	}
}

void AbilityAnimation::WildDanceUpdate(float delta_time) {
	//使ってなかったらリターン
	if (!m_IsWildDanceAttack)return;

	m_WildDanceCoolTime -= delta_time;

	if (m_WildDanceCoolTime <= 0.0f) {
		m_IsWildDanceAttack = false;
		gsPlaySE(Charge_SE);
		return;
	}
}

void AbilityAnimation::WaveWildDanceUpdate(float delta_time) {
	//使ってなかったらリターン
	if (!m_IsWaveAttack)return;

	m_WaveCoolTime -= delta_time;

	if (m_WaveCoolTime <= 0.0f) {
		m_IsWaveAttack = false;
		gsPlaySE(Charge_SE);
		return;
	}
}

void AbilityAnimation::FangsUpdate(float delta_time) {
	//使ってなかったらリターン
	if (!m_IsFangsAttack)return;

	m_FangsCoolTime -= delta_time;

	if (m_FangsCoolTime <= 0.0f) {
		m_IsFangsAttack = false;
		gsPlaySE(Charge_SE);
		return;
	}
}