#include "GolemOwnedState.h"

void GolemAttackPushState::Enter() {
	m_Owner->AttackStart(true);
	m_Owner->ChangeMotion(MotionGolemPushAttack, false);
}

void GolemAttackPushState::Execute(float delta_time) {
	if (m_Owner->IsMotionEnd()) {
		m_Owner->ChangeState(GolemOwnedState::Idle);
		return;
	}
	//‰ñ“]
	if (m_Owner->GetAnimationTimer(AttackTurnTime)) {
		float angle = CLAMP(m_Owner->TargetSignedAngle(), -TurnAngle, TurnAngle) * delta_time;
		m_Owner->Transform().rotate(0.f, angle * delta_time, 0.f);
		return;
	}
}

void GolemAttackPushState::Exit() {
	m_Owner->ResetInterval(AttackInterval);
	m_Owner->AttackStart(false);
}
