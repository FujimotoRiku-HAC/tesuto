#include "GolemOwnedState.h"

void GolemAttackWildDanceRotationState::Enter() {
	m_Owner->AttackStart(true);
	m_Owner->ChangeMotion(MotionGolemWildDanceRotatingAttack, false);
}

void GolemAttackWildDanceRotationState::Execute(float delta_time) {
	if (m_Owner->IsMotionEnd()) {
		m_Owner->ChangeState(GolemOwnedState::Idle);
		return;
	}
	//‰ñ“]
	if (m_Owner->GetAnimationTimer(WildDanceAttackTurnTime)) {
		float angle = CLAMP(m_Owner->TargetSignedAngle(), -TurnAngle, TurnAngle) * delta_time;
		m_Owner->Transform().rotate(0.f, angle * delta_time, 0.f);
		return;
	}
}

void GolemAttackWildDanceRotationState::Exit() {
	m_Owner->ResetInterval(AttackInterval);
	m_Owner->AttackStart(false);
}
