#include "StoneSoldierOwnedState.h"
#include "World/World.h"

void StoneSoldierPounceAttackState::Enter() {
	m_Owner->ResetVelocity();
	m_Owner->ChangeMotion(StoneSoldierPounceAttack, false);
}

void StoneSoldierPounceAttackState::Execute(float delta_time) {
	//ˆê’èŽžŠÔ“®‚­
	if (m_Owner->GetAnimationTimer(PounceTimerEnd, PounceTimerStart)) {
		m_Owner->ChangeVelocity(m_Owner->Transform().forward().normalized() * m_Owner->Speed() * PounceHitSpeed * delta_time);
		return;
	}
	if (m_Owner->IsMotionEnd()) {
		m_Owner->ChangeState(StoneSoldierOwnedState::Idle);
		return;
	}
	//ˆÚ“®—Ê‰Šú‰»
	m_Owner->ResetVelocity();
}

void StoneSoldierPounceAttackState::Exit() {
	m_Owner->ResetVelocity();
	m_Owner->ResetInterval(AttackInterval);
	m_Owner->AttackingCheck(true);
	m_Owner->HaveTicket(false);
	m_Owner->AttackStart(false);
}
