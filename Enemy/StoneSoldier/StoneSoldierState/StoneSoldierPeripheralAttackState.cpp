#include "StoneSoldierOwnedState.h"
#include "World/World.h"

void StoneSoldierPeripheralAttackState::Enter() {
	m_Owner->ChangeMotion(StoneSoldierPeripheralAttack, false);
	m_Owner->ResetVelocity();
}

void StoneSoldierPeripheralAttackState::Execute(float delta_time) {
	if (m_Owner->IsMotionEnd()) {
		m_Owner->ChangeState(StoneSoldierOwnedState::Idle);
		return;
	}
}

void StoneSoldierPeripheralAttackState::Exit() {
	m_Owner->ResetInterval(AttackInterval);
	m_Owner->ResetVelocity();
	m_Owner->AttackingCheck(true);
	m_Owner->HaveTicket(false);
	m_Owner->AttackStart(false);
}

