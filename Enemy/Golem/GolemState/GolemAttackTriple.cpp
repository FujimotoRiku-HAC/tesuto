#include "GolemOwnedState.h"

void GolemAttackTriple::Enter() {
	m_Owner->AttackStart(true);
	m_Owner->ChangeMotion(MotionGolemTripleAttack, false);
}

void GolemAttackTriple::Execute(float delta_time) {
	if (m_Owner->IsMotionEnd()) {
		m_Owner->ChangeState(GolemOwnedState::Idle);
		return;
	}
	//‰ñ“]
	float angle = CLAMP(m_Owner->TargetSignedAngle(), -TurnAngle, TurnAngle) * delta_time;
	m_Owner->Transform().rotate(0.f, angle * delta_time, 0.f);
}

void GolemAttackTriple::Exit() {
	m_Owner->ResetInterval(AttackInterval);
	m_Owner->AttackStart(false);
}
