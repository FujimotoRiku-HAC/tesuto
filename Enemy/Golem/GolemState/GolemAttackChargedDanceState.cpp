#include "GolemOwnedState.h"

void GolemAttackChargedDanceState::Enter() {
	m_Owner->AttackStart(true);
	m_Owner->ChangeMotion(MotionGolemChargeDanceAttack, false);
}

void GolemAttackChargedDanceState::Execute(float delta_time) {
	if (m_Owner->IsMotionEnd()) {
		m_Owner->ChangeState(GolemOwnedState::Idle);
		return;
	}
	//��]
	float angle = CLAMP(m_Owner->TargetSignedAngle(), -TurnAngle, TurnAngle) * delta_time;
	m_Owner->Transform().rotate(0.f, angle * delta_time, 0.f);
}

void GolemAttackChargedDanceState::Exit() {
	m_Owner->ResetInterval(AttackInterval);
	m_Owner->AttackStart(false);
}
